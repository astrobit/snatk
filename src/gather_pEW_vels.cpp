#include <xstdlib.h>
#include <xio.h>
#include <cstdio>
#include <map>
#include <vector>
#include <xlinalg.h>


class	FIT_DATA
{
public:
	double	m_dpEW;
	double	m_dVel;
	
	FIT_DATA(void)
	{
		m_dpEW = -1.0;
		m_dVel = -1.0;
	}
};

class DATA_CONTAINER
{
public:
	unsigned int m_uiModel;
	FIT_DATA	m_cFlat_Raw;
	FIT_DATA	m_cFlat_Ejecta;
	FIT_DATA	m_cFlat_Shell;
	FIT_DATA	m_cJeff_SF;
	FIT_DATA	m_cJeff_DF;
	FIT_DATA	m_cFlat_SF;
	FIT_DATA	m_cFlat_DF;

	DATA_CONTAINER(void) : m_uiModel(0), m_cJeff_SF(), m_cJeff_DF(), m_cFlat_SF(), m_cFlat_DF(), m_cFlat_Raw(), m_cFlat_Ejecta(), m_cFlat_Shell() {}
};

class GAUSS_FIT_PARAMETERS
{
public:
	double	m_dWl[3];
	double	m_dStr[3];
	double	m_dW_Ratio_1;
	double	m_dW_Ratio_2;
	
	double	m_dH_Ratio_1;
	double	m_dH_Ratio_2;
	GAUSS_FIT_PARAMETERS(void)
	{
		m_dW_Ratio_1 = m_dW_Ratio_2 = m_dH_Ratio_1 = m_dH_Ratio_2 = 0.0;
	}

	GAUSS_FIT_PARAMETERS(const double & i_dWL_1, const double & i_dStrength_1, const double & i_dWL_2, const double & i_dStrength_2, const double & i_dWL_3 = 0.0, const double & i_dStrength_3 = 0.0)
	{
		m_dWl[0] = i_dWL_1;
		m_dWl[1] = i_dWL_2;
		m_dWl[2] = i_dWL_3;

		m_dStr[0] = i_dStrength_1;
		m_dStr[1] = i_dStrength_2;
		m_dStr[2] = i_dStrength_3;

		m_dW_Ratio_1 = i_dWL_1 / i_dWL_2;
		m_dW_Ratio_2 = i_dWL_3 / i_dWL_2;
		m_dH_Ratio_1 = i_dStrength_1 / i_dStrength_2;
		m_dH_Ratio_2 = i_dStrength_2 / i_dStrength_2;
	}
};

GAUSS_FIT_PARAMETERS	g_cgfpCaNIR(8662.14,160.0,8542.09,170.0,8498.02,130.0);


XVECTOR Gaussian(const double & i_dX, const XVECTOR & i_vA, void * i_lpvData)
{
	XVECTOR vOut;

	vOut.Set_Size(4); // function, and derivatives wrt each a parameter

	GAUSS_FIT_PARAMETERS *lpvParam = (GAUSS_FIT_PARAMETERS *) i_lpvData;
	double	dDelx_X, dDel_X_Sigma_1, dDel_X_Sigma_2, dDel_X_Sigma_3 = 0.0;
	double	dExp_1, dExp_2, dExp_3 = 0.0;
	double	dDel_X;
	double	dInv_Sigma;

	dInv_Sigma = 1.0 / i_vA.Get(1);
	dDel_X = i_dX - i_vA.Get(2);
	dDel_X_Sigma_2 = dDel_X * dInv_Sigma;


	dDel_X = i_dX / lpvParam->m_dW_Ratio_1 - i_vA.Get(2);
	dDel_X_Sigma_1 = dDel_X * dInv_Sigma;

	dExp_1 = exp(-0.5 * dDel_X_Sigma_1 * dDel_X_Sigma_1) * lpvParam->m_dH_Ratio_1;
	dExp_2 = exp(-0.5 * dDel_X_Sigma_2 * dDel_X_Sigma_2);
	dExp_3 = 0.0;

	if (lpvParam->m_dW_Ratio_2 != 0.0)
	{
		dDel_X = i_dX / lpvParam->m_dW_Ratio_2 - i_vA.Get(2);
		dDel_X_Sigma_3 = dDel_X * dInv_Sigma;
		dExp_3 = exp(-0.5 * dDel_X_Sigma_3 * dDel_X_Sigma_3) * lpvParam->m_dH_Ratio_2;
	}
	vOut.Set(0,i_vA.Get(0) * (dExp_1 + dExp_2 + dExp_3));
	vOut.Set(1,dExp_1 + dExp_2 + dExp_3);
	vOut.Set(2,i_vA.Get(0) * dInv_Sigma * (dExp_1 * dDel_X_Sigma_1 * dDel_X_Sigma_1 + dExp_2 * dDel_X_Sigma_2 * dDel_X_Sigma_2 + dExp_3 * dDel_X_Sigma_3 * dDel_X_Sigma_3));
	vOut.Set(3,i_vA.Get(0) * dInv_Sigma * (dExp_1 * dDel_X_Sigma_1 + dExp_2 * dDel_X_Sigma_2 + dExp_3 * dDel_X_Sigma_3));
	return vOut;
}
XVECTOR Multi_Gaussian(const double & i_dX, const XVECTOR & i_vA, void * i_lpvData)
{
	XVECTOR vOut;
	XVECTOR vOut1;
	XVECTOR vOut2;

	XVECTOR vA_lcl;
	if (i_vA.Get_Size() == 6)
	{
		vOut.Set_Size(7);
		vA_lcl.Set_Size(3);
		vA_lcl.Set(0,i_vA.Get(3));
		vA_lcl.Set(1,i_vA.Get(4));
		vA_lcl.Set(2,i_vA.Get(5));

		vOut1 = Gaussian(i_dX,i_vA,i_lpvData);
		vOut2 = Gaussian(i_dX,vA_lcl,i_lpvData);

		vOut.Set(0,vOut1.Get(0) + vOut2.Get(0));
		vOut.Set(1,vOut1.Get(1));
		vOut.Set(2,vOut1.Get(2));
		vOut.Set(3,vOut1.Get(3));
		vOut.Set(4,vOut2.Get(1));
		vOut.Set(5,vOut2.Get(2));
		vOut.Set(6,vOut2.Get(3));
	}
	else if (i_vA.Get_Size() == 3)
	{
		vOut = Gaussian(i_dX,i_vA,i_lpvData);
	}
	return vOut;
}

double	Compute_Velocity(const double & i_dObserved_Wavelength, const double & i_dRest_Wavelength)
{
	double	dz = i_dObserved_Wavelength / i_dRest_Wavelength;
	double	dz_sqr = dz * dz;
	return (2.99792458e5 * (dz_sqr - 1.0) / (dz_sqr + 1.0));
}

double Compute_pEW(XVECTOR & i_vA, const double & i_dWL_Min, const double & i_dWL_Max)
{
	double dpEW = 0.0;
	double	ddWL = (i_dWL_Max - i_dWL_Min) / 512.0;
	for (double dWL = i_dWL_Min; dWL <= i_dWL_Max; dWL += (i_dWL_Max - i_dWL_Min) / 512.0)
	{
		dpEW += (-Gaussian(dWL,i_vA,&g_cgfpCaNIR).Get(0)) * ddWL;
	}
	return dpEW;
}

enum GROUP {RAW,EJECTA,SHELL,JEFF_FIT,FLAT_FIT,ALL};
enum COMPONENT {PEW, VELOCITY};
void Write_Datafile(const char * i_lpszFilename, GROUP i_eGroup, COMPONENT i_eComponent, const std::map<unsigned int, std::map<unsigned int, DATA_CONTAINER> > & i_cFull_Map)
{
	FILE * fileFile = fopen(i_lpszFilename,"wt");
	const FIT_DATA * lpCtr = NULL;


	if (i_eGroup != ALL)
	{
		fprintf(fileFile,"Day");
		for (std::map<unsigned int, DATA_CONTAINER>::const_iterator cIterJ = (i_cFull_Map.begin())->second.begin(); cIterJ != (i_cFull_Map.begin())->second.end(); cIterJ++)
		{
			fprintf(fileFile,", %i",cIterJ->first);
		}
		fprintf(fileFile,"\n");
	}
	else
		fprintf(fileFile,"Day, Model, v min (min - combined flat), v min (min - ejecta flat), v min (min - shell flat), v min (Jeff - single), v min (Jeff - double), v min (Flat - single), v min (Flat- double), pEW (combined flat), pEW (ejecta flat), pEW (shell flat), pEW (Jeff - single), pEW (Jeff - double), pEW (Flat - single), pEW (Flat- double)\n");
	for (std::map<unsigned int, std::map<unsigned int, DATA_CONTAINER> >::const_iterator cIterI = i_cFull_Map.begin(); cIterI != i_cFull_Map.end(); cIterI++)
	{
		if (i_eGroup != ALL)
		fprintf(fileFile,"%i",cIterI->first);
		for (std::map<unsigned int, DATA_CONTAINER>::const_iterator cIterJ = cIterI->second.begin(); cIterJ != cIterI->second.end(); cIterJ++)
		{
			if (i_eGroup == ALL)
			{
				fprintf(fileFile,"%i",cIterI->first);
				fprintf(fileFile,", %i",cIterJ->first);
				for (unsigned int uiI = 0; uiI < 2; uiI++)
				{
					for (unsigned int uiJ = 0; uiJ <=6; uiJ++)
					{
						switch (uiJ)
						{
						case 0: lpCtr = &cIterJ->second.m_cFlat_Raw; break;
						case 1: lpCtr = &cIterJ->second.m_cFlat_Ejecta; break;
						case 2: lpCtr = &cIterJ->second.m_cFlat_Shell; break;
						case 3: lpCtr = &cIterJ->second.m_cJeff_SF; break;
						case 4: lpCtr = &cIterJ->second.m_cJeff_DF; break;
						case 5: lpCtr = &cIterJ->second.m_cFlat_SF; break;
						case 6: lpCtr = &cIterJ->second.m_cFlat_DF; break;
						}
						if (uiI == 0 && lpCtr->m_dVel < 500. ||
							uiI == 0 && lpCtr->m_dVel > 40000.0 ||
							uiI != 0 && lpCtr->m_dpEW < 1.)
							fprintf(fileFile,", ");
						else if (uiI == 0)
							fprintf(fileFile,", %.0f",lpCtr->m_dVel);
						else
							fprintf(fileFile,", %.1f",lpCtr->m_dpEW);
					}
				}
				fprintf(fileFile,"\n");
			}
			else
			{
				switch (i_eGroup)
				{
				case RAW:
					lpCtr = &cIterJ->second.m_cFlat_Raw;
					break;
				case EJECTA:
					lpCtr = &cIterJ->second.m_cFlat_Ejecta;
					break;
				case SHELL:
					lpCtr = &cIterJ->second.m_cFlat_Shell;
					break;
				case JEFF_FIT:
					lpCtr = &cIterJ->second.m_cJeff_SF;
					break;
				case FLAT_FIT:
					lpCtr = &cIterJ->second.m_cFlat_SF;
					break;
				}
				if (i_eComponent == VELOCITY && lpCtr->m_dVel < 500. ||
					i_eComponent == VELOCITY && lpCtr->m_dVel > 40000.0 ||
					i_eComponent != VELOCITY && lpCtr->m_dpEW < 1.)
					fprintf(fileFile,", ");
				else if (i_eComponent == VELOCITY)
					fprintf(fileFile,", %.0f",lpCtr->m_dVel);
				else
					fprintf(fileFile,", %.1f",lpCtr->m_dpEW);
			}
		}
		if (i_eGroup != ALL)
			fprintf(fileFile,"\n");
	}
	fclose(fileFile);
}
int main(int i_uiArg_Count, const char * i_lpszArg_Values[])
{
	std::map<unsigned int, DATA_CONTAINER>	cMap;
	std::map<unsigned int, std::map<unsigned int, DATA_CONTAINER> > cFull_Map;

	double dWL_Ref = (8498.02 * pow(10.0,-0.39) + 8542.09 * pow(10,-0.36) + 8662.14 * pow(10.0,-0.622)) / (pow(10.0,-0.39) + pow(10.0,-0.36) + pow(10.0,-0.622));

	for (unsigned int uiI = 1; uiI < i_uiArg_Count; uiI++)
	{
		XDATASET	cDatafile;
		char * lpszCursor = strstr((char *)i_lpszArg_Values[uiI],".eps.data.csv");
		if (lpszCursor)
		{
			lpszCursor-=3; // back up to get
			if (strncmp(lpszCursor,"NRM",3) == 0)
				lpszCursor -= 4;
			if (lpszCursor[0] == 'd')
			{
				lpszCursor++;
				unsigned int uiDay = atoi(lpszCursor);
				cMap.clear();
				if (cFull_Map.count(uiDay) == 0)
					cFull_Map[uiDay] = cMap;
				printf("Reading %s\n",i_lpszArg_Values[uiI]);
				cDatafile.ReadDataFile(i_lpszArg_Values[uiI],false,false,',',1);
				for (unsigned int uiJ = 0; uiJ < cDatafile.GetNumElements(); uiJ++)
				{
					unsigned int uiModel = cDatafile.GetElement(0,uiJ);	
					printf("Considering data for model %i\n",uiModel);
					if (cFull_Map[uiDay].count(uiModel) == 0) 
					{
						printf("Gathering data for model %i\n",uiModel);
						DATA_CONTAINER cData;
						XVECTOR vA(3);
						cData.m_uiModel = uiModel;
						cData.m_cFlat_Raw.m_dpEW = cDatafile.GetElement(1,uiJ);
						cData.m_cFlat_Raw.m_dVel = cDatafile.GetElement(2,uiJ);
						cData.m_cFlat_Ejecta.m_dpEW = cDatafile.GetElement(3,uiJ);
						cData.m_cFlat_Ejecta.m_dVel = cDatafile.GetElement(4,uiJ);
						cData.m_cFlat_Shell.m_dpEW = cDatafile.GetElement(5,uiJ);
						cData.m_cFlat_Shell.m_dVel = cDatafile.GetElement(6,uiJ);
						if (cDatafile.GetElement(19,uiJ) != -1)
						{
							vA.Set(0,cDatafile.GetElement(15,uiJ));
							vA.Set(1,cDatafile.GetElement(17,uiJ));
							vA.Set(2,cDatafile.GetElement(19,uiJ));
							cData.m_cJeff_SF.m_dVel = -Compute_Velocity(cDatafile.GetElement(19,uiJ),dWL_Ref);
							cData.m_cJeff_SF.m_dpEW = Compute_pEW(vA,7500.0,9000.0);
						}
						if (cDatafile.GetElement(25,uiJ) != -1)
						{
							vA.Set(0,cDatafile.GetElement(21,uiJ));
							vA.Set(1,cDatafile.GetElement(23,uiJ));
							vA.Set(2,cDatafile.GetElement(25,uiJ));
							cData.m_cJeff_DF.m_dVel = -Compute_Velocity(cDatafile.GetElement(25,uiJ),dWL_Ref);
							cData.m_cJeff_DF.m_dpEW = Compute_pEW(vA,7500.0,9000.0);
						}
						if (cDatafile.GetElement(31,uiJ) != -1)
						{
							vA.Set(0,cDatafile.GetElement(27,uiJ));
							vA.Set(1,cDatafile.GetElement(29,uiJ));
							vA.Set(2,cDatafile.GetElement(31,uiJ));
							cData.m_cFlat_SF.m_dVel = -Compute_Velocity(cDatafile.GetElement(31,uiJ),dWL_Ref);
							cData.m_cFlat_SF.m_dpEW = Compute_pEW(vA,7500.0,9000.0);
						}
						if (cDatafile.GetElement(37,uiJ) != -1)
						{
							vA.Set(0,cDatafile.GetElement(33,uiJ));
							vA.Set(1,cDatafile.GetElement(35,uiJ));
							vA.Set(2,cDatafile.GetElement(37,uiJ));
							cData.m_cFlat_DF.m_dVel = -Compute_Velocity(cDatafile.GetElement(37,uiJ),dWL_Ref);
							cData.m_cFlat_DF.m_dpEW = Compute_pEW(vA,7500.0,9000.0);
						}
						(cFull_Map[uiDay])[uiModel] = cData;
					}
				}
			}
		}
	}

	for (std::map<unsigned int, std::map<unsigned int, DATA_CONTAINER> >::iterator cIterI = cFull_Map.begin(); cIterI != cFull_Map.end(); cIterI++)
	{
		for (std::map<unsigned int, DATA_CONTAINER>::iterator cIterJ = cIterI->second.begin(); cIterJ != cIterI->second.end(); cIterJ++)
		{
			for (std::map<unsigned int, std::map<unsigned int, DATA_CONTAINER> >::iterator cIterK = cFull_Map.begin(); cIterK != cFull_Map.end(); cIterK++)
			{
				if (cIterK != cIterI)
				{
					if (cIterK->second.count(cIterJ->second.m_uiModel) == 0)
					{
						DATA_CONTAINER cData;
						cData.m_uiModel = cIterJ->second.m_uiModel;
						(cFull_Map[cIterK->first])[cData.m_uiModel] = cData;
					}
				}
			}
		}
	}


	Write_Datafile("vel_pEW_data.csv",ALL,VELOCITY,cFull_Map);

	Write_Datafile("vel_Jeff_data.csv",JEFF_FIT,VELOCITY,cFull_Map);
	Write_Datafile("vel_Flat_data.csv",FLAT_FIT,VELOCITY,cFull_Map);
	Write_Datafile("vel_Raw_data.csv",RAW,VELOCITY,cFull_Map);
	Write_Datafile("vel_Ejecta_data.csv",EJECTA,VELOCITY,cFull_Map);
	Write_Datafile("vel_Shell_data.csv",SHELL,VELOCITY,cFull_Map);

	Write_Datafile("pEW_Jeff_data.csv",JEFF_FIT,PEW,cFull_Map);
	Write_Datafile("pEW_Flat_data.csv",FLAT_FIT,PEW,cFull_Map);
	Write_Datafile("pEW_Raw_data.csv",RAW,PEW,cFull_Map);
	Write_Datafile("pEW_Ejecta_data.csv",EJECTA,PEW,cFull_Map);
	Write_Datafile("pEW_Shell_data.csv",SHELL,PEW,cFull_Map);


	return 0;
}