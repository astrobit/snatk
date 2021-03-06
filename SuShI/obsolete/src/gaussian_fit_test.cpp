
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xio.h>
#include <xmath.h>
#include <xlinalg.h>
#include <xstdlib.h>
#include <xfit.h>
#include "ES_Synow.hh"
//#include "ES_Generic_Error.hh"
#include <Plot_Utilities.h>
#include <float.h>
#include <best_fit_data.h>
#include <line_routines.h>
#include <wordexp.h>
#include <model_spectra_db.h>
#include <opacity_profile_data.h>



class FEATURE_PARAMETERS
{
public:
	double	m_d_pEW;
	double	m_dVmin;
	double	m_dFlux_vmin;

	void Process_Vmin(const double &i_dWL, const double &i_dFlux, const double & i_dWL_Ref, bool i_bProcess = true)
	{
		if (i_bProcess)
		{
			if (i_dFlux < m_dFlux_vmin)
			{
				m_dVmin = Compute_Velocity(i_dWL,i_dWL_Ref);
				m_dFlux_vmin = i_dFlux;
			}
		}
	}
	void Process_pEW(const double & i_dFlux, const double & i_dDelta_WL)
	{
		m_d_pEW += (1.0 - i_dFlux) * i_dDelta_WL;
	}
	void	Reset(void)
	{
		m_d_pEW = 0.0;
		m_dVmin = 0.0;
		m_dFlux_vmin = DBL_MAX;
	}
	FEATURE_PARAMETERS(void) { Reset();}
};


void Process_Model_List(const char * lpszModel_List_String, const char ** &o_lpszModel_List, unsigned int &o_uiModel_Count)
{
	char * lpszCursor = (char *)lpszModel_List_String;
	// count number of models to process
	o_uiModel_Count = 0;
	while (lpszCursor && lpszCursor[0] != 0)
	{
		// bypass whitespace
		while (lpszCursor[0] != 0 && (lpszCursor[0] == ' ' || lpszCursor[0] == '\t'))
			lpszCursor++;
		// count model
		if (lpszCursor[0] != 0 && lpszCursor[0] != ',')
		{
			o_uiModel_Count++;
			while (lpszCursor[0] != 0 && lpszCursor[0] != ',')
				lpszCursor++;
		}
		if(lpszCursor[0] == ',')
			lpszCursor++; // bypass comma
	}
	// allocate model name pointer list
	o_lpszModel_List = new const char *[o_uiModel_Count];
	// count number of models to process
	o_uiModel_Count = 0;
	lpszCursor = (char *)lpszModel_List_String; // reset cursor
	while (lpszCursor && lpszCursor[0] != 0)
	{
		// bypass whitespace
		while (lpszCursor[0] != 0 && (lpszCursor[0] == ' ' || lpszCursor[0] == '\t'))
			lpszCursor++;
		// add pointer to model name and count model
		if (lpszCursor[0] != 0 && lpszCursor[0] != ',')
		{
			o_lpszModel_List[o_uiModel_Count] = lpszCursor;
			o_uiModel_Count++;
			while (lpszCursor[0] != 0 && lpszCursor[0] != ',')
				lpszCursor++;
		}
		if(lpszCursor[0] == ',')
		{
			lpszCursor[0] = 0;
			lpszCursor++; // bypass comma
		}
	}
}


int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	OPACITY_PROFILE_DATA::GROUP eScalar_Type;
	msdb::DATABASE cMSDB(true);
	gauss_fit_parameters * lpgfpParamters;
	double	dDay = xParse_Command_Line_Dbl(i_iArg_Count,(const char **)i_lpszArg_Values,"--day",1.0);
	double	dPS_Velocity = xParse_Command_Line_Dbl(i_iArg_Count,(const char **)i_lpszArg_Values,"--ps-velocity",-1.0);
	double	dPS_Temp = xParse_Command_Line_Dbl(i_iArg_Count,(const char **)i_lpszArg_Values,"--ps-temp",1.0);
	double	dEjecta_Scalar = xParse_Command_Line_Dbl(i_iArg_Count,(const char **)i_lpszArg_Values,"--ejecta-scalar",1.0);
	double	dShell_Scalar = xParse_Command_Line_Dbl(i_iArg_Count,(const char **)i_lpszArg_Values,"--shell-scalar",1.0);
    unsigned int uiPS_Velocity_Ion_State = xParse_Command_Line_UInt(i_iArg_Count,(const char **)i_lpszArg_Values,"--use-computed-ps-velocity",3);

	double dV_ps, dSlope;
//	printf("here 0\n");
	// get model list string

	const char *	lpszModel_List_String = xParse_Command_Line_Data_Ptr(i_iArg_Count,(const char **)i_lpszArg_Values,"--model");
	if (lpszModel_List_String == NULL)
	{
		fprintf(stderr,"Model (--model=X) must be specified.\n");
		return 1;
	}
	char lpszModel[64];
	if (lpszModel_List_String[0] >= '0' && lpszModel_List_String[0] <= '9' &&
		lpszModel_List_String[1] >= '0' && lpszModel_List_String[1] <= '9' &&
		lpszModel_List_String[2] == 0)
	{
		sprintf(lpszModel,"run%s",lpszModel_List_String);
	}
	else
		strcpy(lpszModel,lpszModel_List_String);

	enum	tFit_Region		{CANIR,CAHK,SI5968,SI6355,OINIR,OTHER};
	tFit_Region	eFit_Region = OTHER;
	unsigned int uiIon = 0;
	char lpszOutput_Name[256];
	xParse_Command_Line_String(i_iArg_Count,(const char **)i_lpszArg_Values,"--output-name",lpszOutput_Name,sizeof(lpszOutput_Name),"");



	// Identify which line we are interested in.
	char lpszFitRegion[8];
	xParse_Command_Line_String(i_iArg_Count,(const char **)i_lpszArg_Values,"--line-region",lpszFitRegion,sizeof(lpszFitRegion),"");

	if (strcmp(lpszFitRegion,"CaHK") == 0 ||
		strcmp(lpszFitRegion,"CaNIR") == 0)
	{
		uiIon = 2001;
		eFit_Region = (lpszFitRegion[2] == 'H' ? CAHK : CANIR);
	}
	else if (strcmp(lpszFitRegion,"Si6355") == 0)
	{
		uiIon = 1401;
		eFit_Region = SI6355;
	}
	else if (strcmp(lpszFitRegion,"Si5968") == 0)
	{
		uiIon = 1401;
		eFit_Region = SI5968;
	}
	else if (strcmp(lpszFitRegion,"OI8446") == 0)
	{
		uiIon = 800;
		eFit_Region = OINIR;
	}
	// set wavelength reference and a normalization point
	double dNorm_WL = -1.0;
	double dWL_Ref;
	double dRange_Min;
	double dRange_Max;
	msdb::USER_PARAMETERS	cParam;
	
	cParam.m_uiIon = uiIon;
	cParam.m_dTime_After_Explosion = dDay;
//	cParam.m_dPhotosphere_Velocity_kkms = dPS_Velocity;
	cParam.m_dPhotosphere_Temp_kK = dPS_Temp;
	cParam.m_dEjecta_Effective_Temperature_kK = 10.0;
	cParam.m_dShell_Effective_Temperature_kK = 10.0;

	switch (eFit_Region)
	{
	case CANIR:
		dNorm_WL = 7750.0;
		dWL_Ref = (8498.02 * pow(10.0,-0.39) + 8542.09 * pow(10,-0.36) + 8662.14 * pow(10.0,-0.622)) / (pow(10.0,-0.39) + pow(10.0,-0.36) + pow(10.0,-0.622));
		dRange_Min = 7000.0;
		dRange_Max = 9500.0;
		cParam.m_eFeature = msdb::CaNIR;
		lpgfpParamters = &g_cgfpCaNIR;
		eScalar_Type = OPACITY_PROFILE_DATA::SILICON;
		break;
	case CAHK:
		dNorm_WL = 3750.0;
		dWL_Ref = (3934.777 * pow(10.0,0.135) + 3969.592 * pow(10,-0.18)) / (pow(10.0,0.135) + pow(10.0,-0.18));
		dRange_Min = 1800.0;
		dRange_Max = 4000.0;
		cParam.m_eFeature = msdb::CaHK;
		lpgfpParamters = &g_cgfpCaHK;
		eScalar_Type = OPACITY_PROFILE_DATA::SILICON;
		break;
	case SI5968:
		dNorm_WL = 5750.0;
		dWL_Ref = (5959.21 * pow(10.0,-0.225) + 5980.59 * pow(10,0.084)) / (pow(10.0,-0.225) + pow(10.0,0.084));
		dRange_Min = 5000.0;
		dRange_Max = 7000.0;
		lpgfpParamters = &g_cgfpSi5968;
		eScalar_Type = OPACITY_PROFILE_DATA::SILICON;
		break;
	case SI6355:
		dNorm_WL = 5750.0;
		dWL_Ref = (6348.85 * pow(10.0,0.149)  + 6373.12 * pow(10.0,-0.082)) / (pow(10.0,0.149) + pow(10.0,-0.082));
		dRange_Min = 5000.0;
		dRange_Max = 7000.0;
		cParam.m_eFeature = msdb::Si6355;
		lpgfpParamters = &g_cgfpSi6355;
		eScalar_Type = OPACITY_PROFILE_DATA::SILICON;
		break;
	case OINIR:
		dNorm_WL = 7100.0;
		dWL_Ref = (7774.083 * pow(10.0,0.369) + 7776.305 * pow(10.0,0.223) + 7777.528 * pow(10.0,0.002)) / (pow(10.0,0.369) + pow(10.0,0.223) + pow(10.0,0.002));
		dRange_Min = 6000.0;
		dRange_Max = 8000.0;
		cParam.m_eFeature = msdb::O7773;
		lpgfpParamters = &g_cgfpOINIR;
		eScalar_Type = OPACITY_PROFILE_DATA::OXYGEN;
		break;
	}

	// generate model spectra
	if (dNorm_WL > 0.0)
	{
		cParam.m_dWavelength_Range_Lower_Ang = dRange_Min;
		cParam.m_dWavelength_Range_Upper_Ang = dRange_Max;
		cParam.m_dWavelength_Delta_Ang = 1.25;
		//printf("%f %f\n",dRange_Min,dRange_Max);
		//printf("%f %f %f\n", cParam.m_dWavelength_Range_Lower_Ang,cParam.m_dWavelength_Range_Upper_Ang,cParam.m_dWavelength_Delta_Ang);


		ES::Spectrum cSpectra[4];
		cSpectra[0] = ES::Spectrum::create_from_range_and_step(dRange_Min,dRange_Max, cParam.m_dWavelength_Delta_Ang);
		cSpectra[1] = ES::Spectrum::create_from_range_and_step(dRange_Min,dRange_Max, cParam.m_dWavelength_Delta_Ang);
		cSpectra[2] = ES::Spectrum::create_from_range_and_step(dRange_Min,dRange_Max, cParam.m_dWavelength_Delta_Ang);
		cSpectra[3] = ES::Spectrum::create_from_range_and_step(dRange_Min,dRange_Max, cParam.m_dWavelength_Delta_Ang);
		XVECTOR cParameters(7);
		XVECTOR cContinuum_Parameters(7);

//		printf("here 1\n");

		double	dEjecta_Scalar_Ref = -1.0;
		double	dShell_Scalar_Ref = -1.0;
		double	dEjecta_Scalar_Prof = -1.0;
		double	dShell_Scalar_Prof = -1.0;
		OPACITY_PROFILE_DATA	cOpacity_Profile;

		char lpszOpacity_File_Ejecta[64], lpszOpacity_File_Shell[64];
		char	lpszPhotosphere_File_Ejecta[64];
		XDATASET cOpacity_Map_Shell;
		XDATASET cOpacity_Map_Ejecta;


		if (strncmp(lpszModel,"run",3) == 0)
			cParam.m_uiModel_ID = atoi(&lpszModel[3]);
		//printf("Param model id %i\n",cParam.m_uiModel_ID);
		switch (eFit_Region)
		{
		case CANIR:
		case CAHK:
		case SI6355:
		case SI5968:
			sprintf(lpszOpacity_File_Ejecta,"%s/opacity_map_ejecta.Si.xdataset",lpszModel);
			break;
		case OINIR:
			sprintf(lpszOpacity_File_Ejecta,"%s/opacity_map_ejecta.O.xdataset",lpszModel);
			break;
		case OTHER:
			if (uiIon / 100 >= 14 && uiIon / 100 <= 20)
				sprintf(lpszOpacity_File_Ejecta,"%s/opacity_map_ejecta.Si.xdataset",lpszModel);
			else if (uiIon / 100 == 6)
				sprintf(lpszOpacity_File_Ejecta,"%s/opacity_map_ejecta.C.xdataset",lpszModel);
			else if (uiIon / 100 == 8)
				sprintf(lpszOpacity_File_Ejecta,"%s/opacity_map_ejecta.O.xdataset",lpszModel);
			else if (uiIon / 100 >= 22)
				sprintf(lpszOpacity_File_Ejecta,"%s/opacity_map_ejecta.Fe.xdataset",lpszModel);
			else if (uiIon / 100 >= 10 && uiIon / 100 <= 12)
				sprintf(lpszOpacity_File_Ejecta,"%s/opacity_map_ejecta.Mg.xdataset",lpszModel);
			break;
		}
		if (dPS_Velocity > 0.0)
			cContinuum_Parameters.Set(0,dPS_Velocity);
		else
		{
			XDATASET cData;
			sprintf(lpszPhotosphere_File_Ejecta,"%s/photosphere.csv",lpszModel);
			cData.ReadDataFile(lpszPhotosphere_File_Ejecta,false,false,',',1);
			bool bFound = false;
			for (unsigned int uiJ = 0; uiJ  < cData.GetNumElements() && !bFound; uiJ++)
			{
				if (dDay == cData.GetElement(0,uiJ))
				{
					cContinuum_Parameters.Set(0,cData.GetElement(uiPS_Velocity_Ion_State,uiJ)*1e-8);
					bFound = true;
				}
			}
			if (!bFound)
			{
				fprintf(stderr,"Unable to find data in photosphere table for day %.2f for model %s\n",dDay,lpszModel);
				exit(1);
			}
		}
		char lpszOpacity_Profile[128];
		sprintf(lpszOpacity_Profile,"%s/opacity_map_scalars.opdata",lpszModel);
		cOpacity_Profile.Load(lpszOpacity_Profile);
		dEjecta_Scalar_Prof = cOpacity_Profile.Get_Scalar(eScalar_Type);
		dShell_Scalar_Prof = cOpacity_Profile.Get_Scalar(OPACITY_PROFILE_DATA::SHELL);
		if (dEjecta_Scalar_Ref == -1.0)
			dEjecta_Scalar_Ref = dEjecta_Scalar_Prof;
		if (dShell_Scalar_Ref == -1.0)
			dShell_Scalar_Ref = dShell_Scalar_Prof;
//				printf("%.2e/%.2e [%.2e] %.2e/%.2e [%.2e]\n",dEjecta_Scalar_Prof,dEjecta_Scalar_Ref,log10(dEjecta_Scalar_Prof/dEjecta_Scalar_Ref),dShell_Scalar_Prof,dShell_Scalar_Ref,log10(dShell_Scalar_Prof/dShell_Scalar_Ref));


		dV_ps = cContinuum_Parameters.Get(0);
		cContinuum_Parameters.Set(1,dPS_Temp);
		cContinuum_Parameters.Set(2,-20.0); // PS ion log tau
		cContinuum_Parameters.Set(3,10.0); // PS ion exctication temp
		cContinuum_Parameters.Set(4,dV_ps); // PS ion vmin
		cContinuum_Parameters.Set(5,80.0); // PS ion vmax
		cContinuum_Parameters.Set(6,1.0); // PS ion vscale

		cParam.m_uiModel_ID = 0;
		cParam.m_dPhotosphere_Velocity_kkms = cContinuum_Parameters.Get(0);
		//if (cMSDB.Get_Spectrum(cParam, msdb::CONTINUUM, cSpectra[0]) == 0)
		{
			//printf("generating continuua\n");
			Generate_Synow_Spectra_Exp(cSpectra[0],2001,cContinuum_Parameters,cSpectra[0]); // ion irrelevant for this 
			//printf("Adding to db\n");
			//cMSDB.Add_Spectrum(cParam, msdb::CONTINUUM, cSpectra[0]);
			//printf("done\n");
		}

		sprintf(lpszOpacity_File_Shell,"%s/opacity_map_shell.xdataset",lpszModel);
		cOpacity_Map_Ejecta.ReadDataFileBin(lpszOpacity_File_Ejecta);
		cOpacity_Map_Shell.ReadDataFileBin(lpszOpacity_File_Shell);
		bool bShell = cOpacity_Map_Shell.GetNumElements() != 0;

		cParam.m_dEjecta_Log_Scalar = dEjecta_Scalar;
		cParam.m_dShell_Log_Scalar = dShell_Scalar;


		cParameters.Set_Size(bShell ? 7 : 5);
		cParameters.Set(0,dDay);
		cParameters.Set(1,cContinuum_Parameters.Get(0));
		cParameters.Set(2,dPS_Temp);

		cParameters.Set(3,10.0); // fix excitation temp
		cParameters.Set(4,cParam.m_dEjecta_Log_Scalar);
//				printf("%.5e %.5e\n",dEjecta_Scalar,dEjecta_Scalar + log10(dEjecta_Scalar_Prof / dEjecta_Scalar_Ref));
		if (bShell)
		{
			cParameters.Set(5,10.0); // fix excitation temp
			cParameters.Set(6,cParam.m_dShell_Log_Scalar);
		}

		if (cMSDB.Get_Spectrum(cParam, msdb::COMBINED, cSpectra[1]) == 0)
		{
			Generate_Synow_Spectra(cSpectra[1], cOpacity_Map_Ejecta, cOpacity_Map_Shell, uiIon, cParameters, cSpectra[1]);
			printf("Adding to db for %s...",lpszModel);
			fflush(stdout);
			msdb::dbid dbidID = cMSDB.Add_Spectrum(cParam, msdb::COMBINED, cSpectra[1]);
			printf(".");
			fflush(stdout);
			if (!bShell)
			{
				cSpectra[2] = cSpectra[1];
				cMSDB.Add_Spectrum(dbidID, msdb::EJECTA_ONLY, cSpectra[2]);
				printf(".");
				fflush(stdout);
			}
			else
			{
				cParameters.Set(6,-40.0);
				Generate_Synow_Spectra(cSpectra[2], cOpacity_Map_Ejecta, cOpacity_Map_Shell, uiIon, cParameters, cSpectra[2]);
				printf(".");
				fflush(stdout);
				cMSDB.Add_Spectrum(dbidID, msdb::EJECTA_ONLY, cSpectra[2]);

				cParameters.Set(6,dShell_Scalar);
				cParameters.Set(4,-40.0);
				Generate_Synow_Spectra(cSpectra[3], cOpacity_Map_Ejecta, cOpacity_Map_Shell, uiIon, cParameters, cSpectra[3]);
				printf(".");
				fflush(stdout);
				cMSDB.Add_Spectrum(dbidID, msdb::SHELL_ONLY, cSpectra[3]);
			}
			printf("Done\n");
		}
		else
		{
			if (cMSDB.Get_Spectrum(cParam, msdb::EJECTA_ONLY, cSpectra[2]) == 0)
			{
				cParameters.Set(6,-40.0);
				Generate_Synow_Spectra(cSpectra[2], cOpacity_Map_Ejecta, cOpacity_Map_Shell, uiIon, cParameters, cSpectra[1]);
				printf("Adding E-O to db for %s...",lpszModel);
				fflush(stdout);
				msdb::dbid dbidID = cMSDB.Add_Spectrum(cParam, msdb::EJECTA_ONLY, cSpectra[2]);
				cParameters.Set(6,dShell_Scalar);
			}
			if (bShell)
			{
				if (cMSDB.Get_Spectrum(cParam, msdb::SHELL_ONLY, cSpectra[3]) == 0)
				{
					cParameters.Set(4,-40.0);
					Generate_Synow_Spectra(cSpectra[3], cOpacity_Map_Ejecta, cOpacity_Map_Shell, uiIon, cParameters, cSpectra[3]);
					printf("Adding S-O to db for %s...",lpszModel);
					fflush(stdout);
					msdb::dbid dbidID = cMSDB.Add_Spectrum(cParam, msdb::SHELL_ONLY, cSpectra[3]);
					cParameters.Set(4,dEjecta_Scalar);
				}
			}
		}

//		printf("here 2\n");
		epsplot::page_parameters	cPlot_Parameters;
		epsplot::data cPlot;
		epsplot::axis_parameters	cX_Axis_Parameters;
		epsplot::axis_parameters	cY_Axis_Parameters;


		cPlot_Parameters.m_uiNum_Columns = 1;
		cPlot_Parameters.m_uiNum_Rows = 1;
		cPlot_Parameters.m_dWidth_Inches = 11.0;
		cPlot_Parameters.m_dHeight_Inches = 8.5;

		cX_Axis_Parameters.Set_Title("Wavelength [A]");
		cX_Axis_Parameters.m_dMajor_Label_Size = 24.0;
		cY_Axis_Parameters.Set_Title("Flux");
		cY_Axis_Parameters.m_dLower_Limit = 0.0;
		cY_Axis_Parameters.m_dUpper_Limit = 2.0;

		unsigned int uiX_Axis = cPlot.Set_X_Axis_Parameters( cX_Axis_Parameters);
		unsigned int uiY_Axis = cPlot.Set_Y_Axis_Parameters( cY_Axis_Parameters);


		char lpszFilename[512];
		double * lpdSpectra_WL;
		double * lpdSpectra_Flux;
		double * lpdSpectra_WL_EO;
		double * lpdSpectra_Flux_EO;
		double * lpdSpectra_WL_SO;
		double * lpdSpectra_Flux_SO;
		double * lpdContinuum_WL, * lpdContinuum_Flux;
		unsigned int  uiSpectra_Count, uiContinuum_Count, uiSpectra_Count_EO, uiSpectra_Count_SO;
		int iColor = (int)epsplot::BLACK;
		int iStipple = (int)epsplot::SOLID;
		epsplot::line_parameters cLine_Parameters;

		// Get spectral data in the desired range
		cLine_Parameters.m_dWidth = 1.0;
//		cPlot.Set_Plot_Data(lpdContinuum_WL, lpdContinuum_Flux, uiNum_Points_Continuum, cLine_Parameters, 0, 0);
		lpdContinuum_WL = NULL;
		lpdContinuum_Flux = NULL;
		uiContinuum_Count = 0;
		lpdSpectra_WL = NULL;
		lpdSpectra_Flux = NULL;
		lpdSpectra_WL_EO = NULL;
		lpdSpectra_Flux_EO = NULL;
		lpdSpectra_WL_SO = NULL;
		lpdSpectra_Flux_SO = NULL;
		uiContinuum_Count = 0;
		uiSpectra_Count = 0;
		uiSpectra_Count_EO = 0;
		uiSpectra_Count_SO = 0;
		Get_Spectra_Data(cSpectra[0], lpdContinuum_WL, lpdContinuum_Flux, uiContinuum_Count, dRange_Min, dRange_Max);
		Get_Spectra_Data(cSpectra[1], lpdSpectra_WL, lpdSpectra_Flux, uiSpectra_Count, dRange_Min, dRange_Max);
		Get_Spectra_Data(cSpectra[2], lpdSpectra_WL_EO, lpdSpectra_Flux_EO, uiSpectra_Count_EO, dRange_Min, dRange_Max);
		Get_Spectra_Data(cSpectra[3], lpdSpectra_WL_SO, lpdSpectra_Flux_SO, uiSpectra_Count_SO, dRange_Min, dRange_Max);
		for (unsigned int uiJ = 0; uiJ < uiSpectra_Count; uiJ++)
		{
			lpdSpectra_Flux[uiJ] /= lpdContinuum_Flux[uiJ];
			lpdSpectra_Flux_EO[uiJ] /= lpdContinuum_Flux[uiJ];
			lpdSpectra_Flux_SO[uiJ] /= lpdContinuum_Flux[uiJ];
//				printf("%.0f %.4f %.4f %.4f\n",lpdContinuum_WL[uiI][uiJ],lpdContinuum_Flux[uiI][uiJ],cSpectra[1].flux(uiJ),lpdSpectra_Flux[uiI][uiJ]);
		}
		cLine_Parameters.m_eColor = epsplot::BLACK;
		cLine_Parameters.m_eStipple = epsplot::SOLID;
		cPlot.Set_Plot_Data(lpdSpectra_WL, lpdSpectra_Flux, uiSpectra_Count, cLine_Parameters, uiX_Axis, uiY_Axis);
//		printf("here 3\n");
//		fflush(stdout);

	
//		printf("here 4\n");
//		fflush(stdout);
		XVECTOR	vX, vY, vA, vW,vA_Single, vA_Double, vA_Flat, vA_Single_Flat, vA_Double_Flat;
		double	dSmin_Single = DBL_MAX;
		XSQUARE_MATRIX mCovariance_Matrix;
		XSQUARE_MATRIX mCovariance_Matrix_Single;
		XSQUARE_MATRIX mCovariance_Matrix_Flat;
		XSQUARE_MATRIX mCovariance_Matrix_Single_Flat;
		double	dSmin;
		double	dSmin_Flat;
		double	dSmin_Single_Flat;

		// compute pEW values for features of interest.
		FEATURE_PARAMETERS	cCombined_Flat;
		FEATURE_PARAMETERS	cEO_Flat;
		FEATURE_PARAMETERS	cSO_Flat;
		FEATURE_PARAMETERS	cCombined_Unflat;
		FEATURE_PARAMETERS	cEO_Unflat;
		FEATURE_PARAMETERS	cSO_Unflat;
		unsigned int uiContinuum_Blue_Idx = 0;
		unsigned int uiContinuum_Red_Idx = 0;
		bool	bIn_feature = false;
		double	dMin_Flux = DBL_MAX;
		double	dP_Cygni_Peak_Flux = 0.0;
		unsigned int uiMin_Flux_Idx = -1;
		double	dMin_Flux_Flat = DBL_MAX;
		unsigned int uiP_Cygni_Min_Idx = 0;
		// first generate pEW based on flattened spectra and excluding the p Cygni peak
		for (unsigned int uiJ = 0; uiJ < uiSpectra_Count; uiJ++)
		{
			double dFlat_Flux = lpdSpectra_Flux[uiJ];
			double dFlat_Flux_EO = lpdSpectra_Flux_EO[uiJ];
			double dFlat_Flux_SO = lpdSpectra_Flux_SO[uiJ];

			cCombined_Flat.Process_Vmin(lpdSpectra_WL[uiJ],lpdSpectra_Flux[uiJ],dWL_Ref);
			cEO_Flat.Process_Vmin(lpdSpectra_WL[uiJ],lpdSpectra_Flux_EO[uiJ],dWL_Ref);
			cSO_Flat.Process_Vmin(lpdSpectra_WL[uiJ],lpdSpectra_Flux_SO[uiJ],dWL_Ref);

			cCombined_Flat.Process_pEW(lpdSpectra_Flux[uiJ],cParam.m_dWavelength_Delta_Ang);
			cEO_Flat.Process_pEW(lpdSpectra_Flux_EO[uiJ],cParam.m_dWavelength_Delta_Ang);
			cSO_Flat.Process_pEW(lpdSpectra_Flux_SO[uiJ],cParam.m_dWavelength_Delta_Ang);

			cCombined_Unflat.Process_Vmin(lpdSpectra_WL[uiJ],cSpectra[1].flux(uiJ),dWL_Ref,lpdSpectra_Flux[uiJ] < 1.0);
			cEO_Unflat.Process_Vmin(lpdSpectra_WL[uiJ],cSpectra[2].flux(uiJ),dWL_Ref, lpdSpectra_Flux_EO[uiJ] < 1.0);
			cSO_Unflat.Process_Vmin(lpdSpectra_WL[uiJ],cSpectra[3].flux(uiJ),dWL_Ref, lpdSpectra_Flux_SO[uiJ] < 1.0);

			bIn_feature |= (1.0 - lpdSpectra_Flux[uiJ]) > 1.0e-2;
			//printf("%.0f %.5f\n",lpdSpectra_WL[uiI][uiJ],lpdSpectra_Flux[uiI][uiJ]);
			if (!bIn_feature)
				uiContinuum_Blue_Idx = uiJ;
//				if (lpdSpectra_Flux[uiI][uiJ] > 1.0001 && cSpectra[1].flux(uiJ) > dP_Cygni_Peak_Flux)
//				{
//					dP_Cygni_Peak_Flux = cSpectra[1].flux(uiJ);
//					uiContinuum_Red_Idx = uiJ;
//				}
//				if (bIn_feature && lpdSpectra_Flux[uiI][uiJ] < 1.000 && cSpectra[1].flux(uiJ) < dMin_Flux)
//				{
//					dMin_Flux = cSpectra[1].flux(uiJ);
//				}
			if (lpdSpectra_Flux[uiJ] < 1.000 && lpdSpectra_Flux[uiJ] < dMin_Flux_Flat)
			{
				dMin_Flux_Flat = lpdSpectra_Flux[uiJ];
				uiMin_Flux_Idx = uiJ;
			}

			if (bIn_feature && lpdSpectra_Flux[uiJ] > 1.0000 && uiP_Cygni_Min_Idx == 0) // determine max index of absorption region
				uiP_Cygni_Min_Idx = uiJ;
		}	
		double	dV_Jeff_HVF = 0.0, dV_Jeff_PVF = 0.0, dpEW_Jeff_HVF = 0.0, dpEW_Jeff_PVF = 0.0;
		uiContinuum_Red_Idx = uiMin_Flux_Idx;
		if (uiMin_Flux_Idx != (unsigned int)(-1))
		{
			while (uiContinuum_Blue_Idx > 0 && lpdSpectra_Flux[uiContinuum_Blue_Idx] < 0.99)
				uiContinuum_Blue_Idx--;
			for (unsigned int uiJ = uiMin_Flux_Idx; uiJ < uiSpectra_Count; uiJ++)
			{
				if (lpdSpectra_Flux[uiJ] > 0.99 && cSpectra[1].flux(uiJ) > dP_Cygni_Peak_Flux)
				{
					dP_Cygni_Peak_Flux = cSpectra[1].flux(uiJ);
					uiContinuum_Red_Idx = uiJ;
				}
			}
//		printf("here 4a\n");
			unsigned int uiNum_Points = uiContinuum_Red_Idx - uiContinuum_Blue_Idx + 1;
//			printf("%i %i %i %i\n",uiContinuum_Blue_Idx, uiContinuum_Red_Idx, uiNum_Points, lpuiSpectra_Count[uiI]);
//		fflush(stdout);
	
			// perform Gaussian fitting routine
			// first define the pseudo-continuum as defined by Silverman
			dSlope = (cSpectra[1].flux(uiContinuum_Red_Idx) - cSpectra[1].flux(uiContinuum_Blue_Idx)) / (cSpectra[1].wl(uiContinuum_Red_Idx) - cSpectra[1].wl(uiContinuum_Blue_Idx));
	
			vY.Set_Size(uiNum_Points);
			vX.Set_Size(uiNum_Points);
			vW.Set_Size(uiNum_Points);

			fflush(stdout);
			for (unsigned int uiJ = 0; uiJ < uiNum_Points; uiJ++)
			{
				vX.Set(uiJ,cSpectra[1].wl(uiJ + uiContinuum_Blue_Idx));
				vW.Set(uiJ,0.01); // arbitrary weight
				vY.Set(uiJ,cSpectra[1].flux(uiJ + uiContinuum_Blue_Idx) - ((cSpectra[1].wl(uiJ + uiContinuum_Blue_Idx) - cSpectra[1].wl(uiContinuum_Blue_Idx)) * dSlope + cSpectra[1].flux(uiContinuum_Blue_Idx)));

				cCombined_Unflat.Process_pEW(vY.Get(uiJ),cParam.m_dWavelength_Delta_Ang);
			}
			// try single gaussian fit
			// some rough initial guesses for the parameters
			vA.Set_Size(3);
			vA.Set(0,-dMin_Flux_Flat/3.0);
			vA.Set(1,200.0);
			vA.Set(2,lpdSpectra_WL[uiMin_Flux_Idx]);

			// Perform LSQ fit
			if (GeneralFit(vX, vY ,vW, Multi_Gaussian, vA, mCovariance_Matrix, dSmin, lpgfpParamters,100))
			{
				vA_Single = vA;
				dSmin_Single = dSmin;
				mCovariance_Matrix_Single = mCovariance_Matrix;
			}
			// try double gaussian fit
			vA.Set_Size(6);
			vA.Set(0,-dMin_Flux_Flat/6.0);
			vA.Set(1,200.0);
			vA.Set(2,lpdSpectra_WL[uiMin_Flux_Idx] - 250.0);
			vA.Set(3,-dMin_Flux_Flat/6.0);
			vA.Set(4,200.0);
			vA.Set(5,lpdSpectra_WL[uiMin_Flux_Idx] + 250.0);
			// Perform LSQ fit
			if (GeneralFit(vX, vY ,vW, Multi_Gaussian, vA, mCovariance_Matrix, dSmin, lpgfpParamters,100))
			{
				vA_Double = vA;
				// if the single guassian fit is better, use those results
				if (dSmin > dSmin_Single)
				{
					vA = vA_Single;
					dSmin = dSmin_Single;
					mCovariance_Matrix = mCovariance_Matrix_Single;
				}
			}
			else
			{
				// if the double guassian fit fails, use single gaussian results
				vA = vA_Single;
				dSmin = dSmin_Single;
				mCovariance_Matrix = mCovariance_Matrix_Single;
			}

//			printf("here 4c\n");
//			fflush(stdout);
			if (vA.Get_Size() == 6)
			{
				dV_Jeff_HVF = Compute_Velocity(vA.Get(2),lpgfpParamters->m_dWl[1]);
				dV_Jeff_PVF = Compute_Velocity(vA.Get(5),lpgfpParamters->m_dWl[1]);
			}
			else
			{
				dV_Jeff_PVF = Compute_Velocity(vA.Get(2),lpgfpParamters->m_dWl[1]);
			}
			for (unsigned int uiJ = uiContinuum_Blue_Idx; uiJ < uiContinuum_Red_Idx; uiJ++)
			{
				XVECTOR vF = Multi_Gaussian(vX.Get(uiJ), vA, lpgfpParamters);
				if (vA.Get_Size() == 6)
				{
					XVECTOR vAlcl;
					vAlcl.Set_Size(3);
					vAlcl.Set(0,vA.Get(0));
					vAlcl.Set(1,vA.Get(1));
					vAlcl.Set(2,vA.Get(2));

					Gaussian(vX.Get(uiJ), vAlcl, lpgfpParamters);
					dpEW_Jeff_HVF -= vF.Get(0) * cParam.m_dWavelength_Delta_Ang; // - sign to keep pEW positive

					vAlcl.Set(0,vA.Get(3));
					vAlcl.Set(1,vA.Get(4));
					vAlcl.Set(2,vA.Get(5));

					Gaussian(vX.Get(uiJ), vAlcl, lpgfpParamters);
					dpEW_Jeff_PVF -= vF.Get(0) * cParam.m_dWavelength_Delta_Ang;
				}
				else
				{
					Gaussian(vX.Get(uiJ), vA, lpgfpParamters);
					dpEW_Jeff_PVF -= vF.Get(0) * cParam.m_dWavelength_Delta_Ang; // - sign to keep pEW positive
				}
			}

			vX.Set_Size(uiP_Cygni_Min_Idx);
			vY.Set_Size(uiP_Cygni_Min_Idx);
			vW.Set_Size(uiP_Cygni_Min_Idx);
			for (unsigned int uiJ = 0; uiJ < uiP_Cygni_Min_Idx; uiJ++)
			{
				vX.Set(uiJ,lpdSpectra_WL[uiJ]);
				vW.Set(uiJ,0.01); // arbitrary weight
				vY.Set(uiJ,lpdSpectra_Flux[uiJ] - 1.0);
			}
			// try single gaussian fit
			// some rough initial guesses for the parameters
			vA_Flat.Set_Size(3);
			vA_Flat.Set(0,-dMin_Flux_Flat/3.0);
			vA_Flat.Set(1,200.0);
			vA_Flat.Set(2,lpdSpectra_WL[uiMin_Flux_Idx]);

			// Perform LSQ fit
			if (GeneralFit(vX, vY ,vW, Multi_Gaussian, vA_Flat, mCovariance_Matrix_Flat, dSmin_Flat, lpgfpParamters,100))
			{
				vA_Single_Flat = vA_Flat;
				dSmin_Single_Flat = dSmin_Flat;
				mCovariance_Matrix_Single_Flat = mCovariance_Matrix_Flat;
			}
			// try double gaussian fit
			vA_Flat.Set_Size(6);
			vA_Flat.Set(0,-dMin_Flux_Flat/6.0);
			vA_Flat.Set(1,200.0);
			vA_Flat.Set(2,lpdSpectra_WL[uiMin_Flux_Idx] - 250.0);
			vA_Flat.Set(3,-dMin_Flux_Flat/6.0);
			vA_Flat.Set(4,200.0);
			vA_Flat.Set(5,lpdSpectra_WL[uiMin_Flux_Idx] + 250.0);
			// Perform LSQ fit
			if (GeneralFit(vX, vY ,vW, Multi_Gaussian, vA_Flat, mCovariance_Matrix_Flat, dSmin_Flat, lpgfpParamters,100))
			{
				vA_Double_Flat = vA_Flat;
				// if the single guassian fit is better, use those results
				if (dSmin_Flat > dSmin_Single_Flat)
				{
					vA_Flat = vA_Single_Flat;
					dSmin_Flat = dSmin_Single_Flat;
					mCovariance_Matrix_Flat = mCovariance_Matrix_Single_Flat;
				}
			}
			else
			{
				// if the double guassian fit fails, use single gaussian results
				vA_Flat = vA_Single_Flat;
				dSmin_Flat = dSmin_Single_Flat;
				mCovariance_Matrix_Flat = mCovariance_Matrix_Single_Flat;
			}

		}

		double * lpdCombined_Flux = new double [uiSpectra_Count];
		double * lpdGaussian_Flux = new double [uiSpectra_Count];
		double * lpdGaussian_Dbl_Flux = new double [uiSpectra_Count];
		double * lpdFlat_Gaussian_Flux = new double [uiSpectra_Count];
		double * lpdFlat_Gaussian_Dbl_Flux = new double [uiSpectra_Count];
		for (unsigned int uiI = 0; uiI < uiSpectra_Count; uiI++)
		{
			if (vA_Single_Flat.Get_Size() > 0)
				lpdFlat_Gaussian_Flux[uiI] = Multi_Gaussian(lpdSpectra_WL[uiI], vA_Single_Flat, lpgfpParamters).Get(0) + 1.0;
			else
				lpdFlat_Gaussian_Flux[uiI] = 1.0;
			if (vA_Double_Flat.Get_Size() > 0)
				lpdFlat_Gaussian_Dbl_Flux[uiI] = Multi_Gaussian(lpdSpectra_WL[uiI], vA_Double_Flat, lpgfpParamters).Get(0) + 1.0;
			else
				lpdFlat_Gaussian_Dbl_Flux[uiI] = 1.0;

			double dY = (cSpectra[1].wl(uiI) - cSpectra[1].wl(uiContinuum_Blue_Idx)) * dSlope + cSpectra[1].flux(uiContinuum_Blue_Idx);

			lpdCombined_Flux[uiI] = cSpectra[1].flux(uiI) / cSpectra[1].flux(uiContinuum_Blue_Idx) + 0.5;
			if (vA_Single.Get_Size() > 0)
				lpdGaussian_Flux[uiI] = (Multi_Gaussian(lpdSpectra_WL[uiI], vA_Single, lpgfpParamters).Get(0) + dY) / cSpectra[1].flux(uiContinuum_Blue_Idx) + 0.5;
			else
				lpdGaussian_Flux[uiI] = 1.5;
			if (vA_Double.Get_Size() > 0)
				lpdGaussian_Dbl_Flux[uiI] = (Multi_Gaussian(lpdSpectra_WL[uiI], vA_Double, lpgfpParamters).Get(0) + dY)  / cSpectra[1].flux(uiContinuum_Blue_Idx) + 0.5;
			else
				lpdGaussian_Dbl_Flux[uiI] = 1.5;
		}
		cLine_Parameters.m_eColor = epsplot::BLACK;
		cLine_Parameters.m_eStipple = epsplot::SHORT_DASH;
		cPlot.Set_Plot_Data(lpdSpectra_WL, lpdCombined_Flux, uiSpectra_Count, cLine_Parameters, uiX_Axis, uiY_Axis);

		if (vA_Single.Get_Size() > 0)
		{
			cLine_Parameters.m_eColor = epsplot::RED;
			cLine_Parameters.m_eStipple = epsplot::SOLID;
			cPlot.Set_Plot_Data(lpdSpectra_WL, lpdGaussian_Flux, uiSpectra_Count, cLine_Parameters, uiX_Axis, uiY_Axis);
		}
		if (vA_Double.Get_Size() > 0)
		{
			cLine_Parameters.m_eColor = epsplot::RED;
			cLine_Parameters.m_eStipple = epsplot::SHORT_DASH;
			cPlot.Set_Plot_Data(lpdSpectra_WL, lpdGaussian_Dbl_Flux, uiSpectra_Count, cLine_Parameters, uiX_Axis, uiY_Axis);
		}

		if (vA_Single_Flat.Get_Size() > 0)
		{
			cLine_Parameters.m_eColor = epsplot::BLUE;
			cLine_Parameters.m_eStipple = epsplot::SOLID;
			cPlot.Set_Plot_Data(lpdSpectra_WL, lpdFlat_Gaussian_Flux, uiSpectra_Count, cLine_Parameters, uiX_Axis, uiY_Axis);
		}
		if (vA_Double_Flat.Get_Size() > 0)
		{
			cLine_Parameters.m_eColor = epsplot::BLUE;
			cLine_Parameters.m_eStipple = epsplot::SHORT_DASH;
			cPlot.Set_Plot_Data(lpdSpectra_WL, lpdFlat_Gaussian_Dbl_Flux, uiSpectra_Count, cLine_Parameters, uiX_Axis, uiY_Axis);
		}
		wordexp_t cResults;
		if (wordexp(lpszOutput_Name,&cResults,WRDE_NOCMD|WRDE_UNDEF) == 0)
		{
			strcpy(lpszOutput_Name, cResults.we_wordv[0]);
			wordfree(&cResults);
		}


		cPlot.Set_Plot_Filename(lpszOutput_Name);
		cPlot.Plot(cPlot_Parameters);



//		printf("here 5\n");
//		fflush(stdout);
		delete [] lpdSpectra_WL;
		delete [] lpdSpectra_Flux;
		delete [] lpdSpectra_WL_EO;
		delete [] lpdSpectra_Flux_EO;
		delete [] lpdSpectra_WL_SO;
		delete [] lpdSpectra_Flux_SO;

		delete [] lpdGaussian_Flux;
		delete [] lpdGaussian_Dbl_Flux;
		delete [] lpdFlat_Gaussian_Flux;
		delete [] lpdFlat_Gaussian_Dbl_Flux;
		delete [] lpdCombined_Flux;
	}
	

	return 0;
}
