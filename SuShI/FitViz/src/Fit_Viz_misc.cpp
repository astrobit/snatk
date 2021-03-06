#include <FitViz.hpp>
#include <sstream>
#include <xio.h>
#include <cmath>
#include <line_routines.h>
#include <eps_plot.h>
//#include <cstdio>

void FIT_VIZ_MAIN::Load_Model_Day_Lists(void)
{
	std::vector<std::string> vsFile_List = Get_Directory_File_List(m_szDirectory);
	std::map<unsigned int, unsigned int> mDay_Map;
	bool bFirst = true;
	std::map<unsigned int, unsigned int> mModel_List;
	m_dPS_Temp = -1;
	m_dEjecta_Scalar = nan("");
	m_dShell_Scalar = nan("");
	m_dShell_Power_Law = 2;
	m_dEjecta_Power_Law = 2;
	m_szRef_Model.clear();
	for (std::vector<std::string>::iterator cI = vsFile_List.begin(); cI != vsFile_List.end(); cI++)
	{
		size_t zPos;
		if ((*cI) == "params.txt")
		{
			XMAP	cMap;
			cMap.Read_File("params.txt");
			m_dPS_Temp = cMap.Get_Value_Double("PS_temp");
			m_dEjecta_Scalar = cMap.Get_Value_Double("Ejecta_scalar");
			m_dShell_Scalar = cMap.Get_Value_Double("Shell_scalar");
			m_dShell_Power_Law = cMap.Get_Value_Double("Shell_power_law");
			m_dEjecta_Power_Law = cMap.Get_Value_Double("Ejecta_power_law");
			m_szRef_Model = cMap.Get_Value_String("Ref_model");
		}
		else if ((zPos = cI->find(".eps.data.csv")) != std::string::npos)
		{
			if (zPos >= 2) // make sure there isn't a file called .eps.data.csv or something weird
			{
				size_t zPosNRM;
				if ((zPosNRM = cI->find("_NRM.eps.data.csv")) != std::string::npos)
					zPos = zPosNRM;
				unsigned int uiDay = ((*cI)[zPos - 2] - '0') * 10 + ((*cI)[zPos - 1] - '0');
				mDay_Map[uiDay] = 1;
				if (bFirst)
				{
					XDATASET	cData;
					cData.ReadDataFile((*cI).c_str(),false,false,',',1);
					if (cData.GetNumRows() != 0)
					{
						for (unsigned int uiI = 0; uiI < cData.GetNumRows(); uiI++)
						{
							mModel_List[cData.GetElement(0,uiI)] = 1;
						}
						bFirst = false;
						m_szFile_Prefix = (*cI);
						if (zPosNRM != std::string::npos)
							m_szFile_Prefix.erase(zPos - 2,19); // get rid of the (d)xx_NRM.eps.data.csv part
						else
							m_szFile_Prefix.erase(zPos - 2,15); // get rid of the (d)xx.eps.data.csv part
					}
				}
			}
		}
		//else if ((zPos = cI->find(".eps.dat")) != string::npos)
		//{
		//}

	}
	m_vModel_List.clear();
	m_vDay_List.clear();
	m_uiDay_Data_Loaded = -1;
	if (!mDay_Map.empty() && !mModel_List.empty())
	{
		for (std::map<unsigned int, unsigned int>::iterator cI = mDay_Map.begin(); cI != mDay_Map.end(); cI++)
		{
			m_vDay_List.push_back(cI->first);
		}
		for (std::map<unsigned int, unsigned int>::iterator cI = mModel_List.begin(); cI != mModel_List.end(); cI++)
		{
			m_vModel_List.push_back(cI->first);
		}
		m_uiSelected_Model = 0;
		m_uiSelected_Day = 0;
		Load_Display_Info();
	}
	else
	{
		m_vsError_Info.push_back("Directory contiains no fit data files.");
	}
}

void FIT_VIZ_MAIN::Load_Display_Info(void)
{
	if (!m_vDay_List.empty() && !m_vModel_List.empty())
	{
		if (m_uiDay_Data_Loaded != m_vDay_List[m_uiSelected_Day])
		{
			std::ostringstream szFilename_xio;
			std::ostringstream szFilename_spectra;
			szFilename_xio << m_szFile_Prefix;
			szFilename_xio.width(2);
			szFilename_xio.fill('0');
			szFilename_xio << m_vDay_List[m_uiSelected_Day];
			if (m_szRef_Model == "-")
				szFilename_xio << "_NRM";
			szFilename_xio << ".eps.data.csv";

			szFilename_spectra << m_szFile_Prefix;
			szFilename_spectra.width(2);
			szFilename_spectra.fill('0');
			szFilename_spectra << m_vDay_List[m_uiSelected_Day];
			if (m_szRef_Model == "-")
				szFilename_spectra << "_NRM";
			szFilename_spectra << ".eps.dat";
			m_dDay_Fit_Data.ReadDataFile(szFilename_xio.str().c_str(),false,false,',',1);
			m_dDay_Spectrum_Data.ReadDataFile(szFilename_spectra.str().c_str(),true,false,0,1);
			m_uiDay_Data_Loaded = m_vDay_List[m_uiSelected_Day];
		}
		m_vWavelength.clear();
		m_vFlux.clear();
		m_vFlux_Flat.clear();
		m_vFlux_Unflat.clear();
		m_vFlux_Shell.clear();
		m_vFlux_Ejecta.clear();
		m_dMax_Flux = -1;
		m_dShell_Flat_pEW = -1;
		m_dEjecta_Flat_pEW = -1;
		m_dTotal_Flat_pEW = -1;
		m_bNo_Shell = true;
		m_uiManual_Fit_Blue_Idx = -1;
		m_uiManual_Fit_Red_Idx= -1;
		m_uiManual_Fit_Central_Idx = -1;
		m_uiManual_Fit_Central_Second_Idx = -1;
		m_vManual_Fit_Data.Set(0,0);
		m_vManual_Fit_Data.Set(1,0);
		m_vManual_Fit_Data.Set(2,0);
		// determine row number for fit info
		if (m_dDay_Fit_Data.GetNumRows() > 0 && m_dDay_Spectrum_Data.GetNumRows() > 0)
		{
			unsigned int uiFound_Row = -1;
			for (unsigned int uiI = 0; uiI < m_dDay_Fit_Data.GetNumRows() && uiFound_Row == -1; uiI++)
			{
				if (m_dDay_Fit_Data.GetElement(0,uiI) == m_vModel_List[m_uiSelected_Model])
				{
					uiFound_Row = uiI;
					if (m_eFit_Method == fm_flat || m_eFit_Method == fm_manual || m_eFit_Method == fm_auto)
					{
						if (m_dDay_Fit_Data.GetElement(33,uiI) == -1)
							m_vGaussian_Fit_Data.Set_Size(3);
						else
						{
							m_vGaussian_Fit_Data.Set_Size(6);
							m_vGaussian_Fit_Data.Set(3,m_dDay_Fit_Data.GetElement(33,uiI));
							m_vGaussian_Fit_Data.Set(4,m_dDay_Fit_Data.GetElement(35,uiI));
							m_vGaussian_Fit_Data.Set(5,m_dDay_Fit_Data.GetElement(37,uiI));
						}
						m_vGaussian_Fit_Data.Set(0,m_dDay_Fit_Data.GetElement(27,uiI));
						m_vGaussian_Fit_Data.Set(1,m_dDay_Fit_Data.GetElement(29,uiI));
						m_vGaussian_Fit_Data.Set(2,m_dDay_Fit_Data.GetElement(31,uiI));
					}
					else
					{
						if (m_dDay_Fit_Data.GetElement(21,uiI) == -1)
							m_vGaussian_Fit_Data.Set_Size(3);
						else
						{
							m_vGaussian_Fit_Data.Set_Size(6);
							m_vGaussian_Fit_Data.Set(3,m_dDay_Fit_Data.GetElement(21,uiI));
							m_vGaussian_Fit_Data.Set(4,m_dDay_Fit_Data.GetElement(23,uiI));
							m_vGaussian_Fit_Data.Set(5,m_dDay_Fit_Data.GetElement(25,uiI));
						}
						m_vGaussian_Fit_Data.Set(0,m_dDay_Fit_Data.GetElement(15,uiI));
						m_vGaussian_Fit_Data.Set(1,m_dDay_Fit_Data.GetElement(17,uiI));
						m_vGaussian_Fit_Data.Set(2,m_dDay_Fit_Data.GetElement(19,uiI));
					}
				}
			}
			
			if (uiFound_Row != -1)
			{
//				m_vGaussian_Fit_Data.Print(stdout);
				unsigned int uiFlux_Col = uiFound_Row * 7 + 1;
				unsigned int uiFlux_Ejecta_Col = uiFound_Row * 7 + 1;
				unsigned int uiFlux_Shell_Col = uiFound_Row * 7 + 1;

				if (m_eFit_Method == fm_flat || m_eFit_Method == fm_manual || m_eFit_Method == fm_auto)
				{
					uiFlux_Col += 2;
					uiFlux_Ejecta_Col += 4;
					uiFlux_Shell_Col += 6;
				}
				else
				{
					uiFlux_Col += 1;
					uiFlux_Ejecta_Col += 3;
					uiFlux_Shell_Col += 5;
				}

				unsigned uiFlux_Col_Flat = uiFound_Row * 7 + 3;
				unsigned uiFlux_Col_Jeff = uiFound_Row * 7 + 2;
				double dMin_Flux_Flat = 1.0e300;
				bool	bIn_feature = false;
				unsigned int uiContinuum_Blue_Idx = -1;
				unsigned int uiContinuum_Red_Idx = -1;
				unsigned int uiMin_Flux_Idx = -1;
				unsigned int uiP_Cygni_Min_Idx = 0;
				double dP_Cygni_Peak_Flux = -1;
				double dMax_Flux_Flat = -1;
				unsigned int uiMax_Flux_Idx = -1;
				double dDelta_Lambda = m_dDay_Spectrum_Data.GetElement(0,1) - m_dDay_Spectrum_Data.GetElement(0,0); 
				m_bNo_Shell = m_dDay_Spectrum_Data.GetElement(uiFlux_Shell_Col,0) == 0.0;
				for (unsigned int uiI = 0; uiI < m_dDay_Spectrum_Data.GetNumRows(); uiI++)
				{
					double dWL = m_dDay_Spectrum_Data.GetElement(0,uiI);
					double dFlux = m_dDay_Spectrum_Data.GetElement(uiFlux_Col,uiI);
					double dFlux_Ejecta = m_dDay_Spectrum_Data.GetElement(uiFlux_Ejecta_Col,uiI);
					double dFlux_Shell = m_dDay_Spectrum_Data.GetElement(uiFlux_Shell_Col,uiI);
					m_vWavelength.push_back(dWL);
					m_vFlux.push_back(dFlux);
					m_vFlux_Flat.push_back(m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Flat,uiI));
					m_vFlux_Unflat.push_back(m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Jeff,uiI));
					m_vFlux_Shell.push_back(dFlux_Shell);
					m_vFlux_Ejecta.push_back(dFlux_Ejecta);
					if (m_eFit_Method == fm_flat || m_eFit_Method == fm_manual || m_eFit_Method == fm_auto)
					{
						if (dFlux < 1.0)
							m_dTotal_Flat_pEW += dDelta_Lambda * (1.0 - dFlux);
						if (dFlux_Shell < 1.0 && m_dDay_Spectrum_Data.GetElement(uiFlux_Shell_Col,0) != 0.0)
							m_dShell_Flat_pEW += dDelta_Lambda * (1.0 - dFlux_Shell);
						if (dFlux_Ejecta < 1.0)
							m_dEjecta_Flat_pEW += dDelta_Lambda * (1.0 - dFlux_Ejecta);
					}
					if (m_dMax_Flux < dFlux)
						m_dMax_Flux = dFlux;
				// determine parameters for Jeff's fit
					double dFlux_Flat = m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Flat,uiI);
					bIn_feature |= (1.0 - dFlux_Flat) > 1.0e-2;
					if (!bIn_feature)
						uiContinuum_Blue_Idx = uiI;
	//				if (lpdSpectra_Flux[uiI][uiJ] > 1.0001 && lpcSpectrum[uiI][1].flux(uiJ) > dP_Cygni_Peak_Flux)
	//				{
	//					dP_Cygni_Peak_Flux = lpcSpectrum[uiI][1].flux(uiJ);
	//					uiContinuum_Red_Idx = uiJ;
	//				}
	//				if (bIn_feature && lpdSpectra_Flux[uiI][uiJ] < 1.000 && lpcSpectrum[uiI][1].flux(uiJ) < dMin_Flux)
	//				{
	//					dMin_Flux = lpcSpectrum[uiI][1].flux(uiJ);
	//				}
					if (dFlux_Flat < 1.000 && dFlux_Flat < dMin_Flux_Flat)
					{
						dMin_Flux_Flat = dFlux_Flat;
						uiMin_Flux_Idx = uiI;
					}
					
				}
				// find the global maximum over this range
				for (unsigned int uiJ = m_dDay_Spectrum_Data.GetNumRows() - 1; uiJ > uiMin_Flux_Idx; uiJ--)
				{
					double dFlux_Flat = m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Flat,uiJ);
					if (dFlux_Flat > dMax_Flux_Flat)
					{
						dMax_Flux_Flat = dFlux_Flat;
						uiMax_Flux_Idx = uiJ;
					}
				}
				// find the edge of the absorbtion region blueward of the peak
				for (unsigned int uiJ = uiMax_Flux_Idx; uiJ > uiMin_Flux_Idx && uiP_Cygni_Min_Idx == 0; uiJ--)
				{
					double dFlux_Flat = m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Flat,uiJ);
					if (dFlux_Flat < 1.0000 && uiP_Cygni_Min_Idx == 0) // determine max index of absorption region
						uiP_Cygni_Min_Idx = uiJ;
				}

				if (uiP_Cygni_Min_Idx == 0)
					uiP_Cygni_Min_Idx = m_dDay_Spectrum_Data.GetNumRows() - 1;
				if (uiMin_Flux_Idx == -1)
				{
					uiMin_Flux_Idx = 0;
				}
				uiContinuum_Red_Idx = uiMin_Flux_Idx;
				uiContinuum_Blue_Idx = uiMin_Flux_Idx;
				while (uiContinuum_Blue_Idx > 0 && m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Flat,uiContinuum_Blue_Idx) < 0.99)
					uiContinuum_Blue_Idx--;
				for (unsigned int uiJ = uiMin_Flux_Idx; uiJ < m_dDay_Spectrum_Data.GetNumRows(); uiJ++)
				{
					if (m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Flat,uiJ) > 0.99 && m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Jeff,uiJ) > dP_Cygni_Peak_Flux)
					{
						dP_Cygni_Peak_Flux = m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Jeff,uiJ);
						uiContinuum_Red_Idx = uiJ;
					}
				}
				m_pGlobal_Min.m_tX = m_dDay_Spectrum_Data.GetElement(0,uiMin_Flux_Idx);
				m_pGlobal_Min.m_tY = m_dDay_Spectrum_Data.GetElement(uiFlux_Col,uiMin_Flux_Idx);
				m_pP_Cygni_Emission_Edge.m_tX = m_dDay_Spectrum_Data.GetElement(0,uiP_Cygni_Min_Idx);
				m_pP_Cygni_Emission_Edge.m_tY = m_dDay_Spectrum_Data.GetElement(uiFlux_Col,uiP_Cygni_Min_Idx);
				m_pJeff_Blue.m_tX = m_dDay_Spectrum_Data.GetElement(0,uiContinuum_Blue_Idx);
				m_pJeff_Blue.m_tY = m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Jeff,uiContinuum_Blue_Idx);
				m_pJeff_Red.m_tX = m_dDay_Spectrum_Data.GetElement(0,uiContinuum_Red_Idx);
				m_pJeff_Red.m_tY = m_dDay_Spectrum_Data.GetElement(uiFlux_Col_Jeff,uiContinuum_Red_Idx);
				m_dJeff_WL = m_pJeff_Blue.m_tX;
				m_dJeff_Flux = m_pJeff_Blue.m_tY;

				m_dJeff_Slope = (m_pJeff_Red.m_tY - m_pJeff_Blue.m_tY) / (m_pJeff_Red.m_tX - m_pJeff_Blue.m_tX);
				double 		dWL_Ref = (8498.02 * pow(10.0,-0.39) + 8542.09 * pow(10,-0.36) + 8662.14 * pow(10.0,-0.622)) / (pow(10.0,-0.39) + pow(10.0,-0.36) + pow(10.0,-0.622));
				m_dFit_Velocity_PVF = -Compute_Velocity(m_vGaussian_Fit_Data.Get(2),dWL_Ref);
				m_dFit_Velocity_HVF = nan("");
				if (m_vGaussian_Fit_Data.Get_Size() == 6)
				{
					m_dFit_Velocity_HVF = -Compute_Velocity(m_vGaussian_Fit_Data.Get(5),dWL_Ref);
					if (m_dFit_Velocity_HVF < m_dFit_Velocity_PVF)
					{
						double dTemp = m_dFit_Velocity_HVF;
						m_dFit_Velocity_HVF = m_dFit_Velocity_PVF;
						m_dFit_Velocity_PVF = dTemp;
					}
				}
				xvector	vX;
				vX = m_vWavelength;
				gauss_fit_parameters * lpgfpParamters = &g_cgfpCaNIR;
				double dpEW_PVF, dpEW_HVF;
				Compute_Gaussian_Fit_pEW(vX, m_vGaussian_Fit_Data, m_vWavelength[1] - m_vWavelength[0], lpgfpParamters, dpEW_PVF, dpEW_HVF);
				m_dFit_pEW = dpEW_HVF + dpEW_PVF;

				m_vFlux_Fit.clear();
				m_vFit_Residuals.clear();
				if (m_eFit_Method != fm_manual && m_eFit_Method != fm_auto)
				{
					for (unsigned int uiI = 0; uiI < m_vWavelength.size(); uiI++)
					{
						double dY;
						switch (m_eFit_Method)
						{
						case fm_flat:
							dY = 1.0 + Multi_Gaussian(m_vWavelength[uiI],m_vGaussian_Fit_Data,lpgfpParamters).Get(0);
							break;
						case fm_jeff:
							dY = Multi_Gaussian(m_vWavelength[uiI],m_vGaussian_Fit_Data,lpgfpParamters).Get(0);
							dY += (m_vWavelength[uiI] - m_dJeff_WL) * m_dJeff_Slope + m_dJeff_Flux;
							break;
						}
						m_vFlux_Fit.push_back(dY);
						m_vFit_Residuals.push_back(m_vFlux[uiI] - dY);
					}
				}
				std::vector<double> vDeriv;
				m_vFlux_Deriv.clear();
				vDeriv.push_back(0.0);
				vDeriv.push_back(0.0);
				vDeriv.push_back(0.0);
				vDeriv.push_back(0.0);
				m_dFlux_Deriv_Max = 0.0;
				double	dInv_dW = 1.0 / (m_vWavelength[1] - m_vWavelength[0]);
				for (unsigned int uiI = 4; uiI < (m_vFlux.size() - 4); uiI++)
				{
					double	dA = m_vFlux[uiI + 4] * 0.0;// / -280.0;
					double	dB = m_vFlux[uiI + 3] * 0.0;// / 105.0 * 4.0;
					double	dC = m_vFlux[uiI + 2] * 0.0;// / -5.0;
					double	dD = m_vFlux[uiI + 1] * 0.5; // / 5.0 * 4.0;
//					double	dE = m_vFlux[uiI + 0] / -280.0;
					double	dF = m_vFlux[uiI - 1] * -0.5; // / -5.0 * 4.0;
					double	dG = m_vFlux[uiI - 2] * 0.0;// / 5.0;
					double	dH = m_vFlux[uiI - 3] * 0.0;// / -105.0 * 4.0;
					double	dI = m_vFlux[uiI - 4] * 0.0;// / 280.0;
 
					double dDeriv = (dA + dB + dC + dD + dF + dG + dH + dI) * dInv_dW;
					vDeriv.push_back(dDeriv);
					if (fabs(dDeriv) > m_dFlux_Deriv_Max)
						m_dFlux_Deriv_Max = fabs(dDeriv);
				}
				vDeriv.push_back(0.0);
				vDeriv.push_back(0.0);
				vDeriv.push_back(0.0);
				vDeriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				for (unsigned int uiI = 7; uiI < (vDeriv.size() - 7); uiI++)
				{
					double dVal = 0.0;
					for (int iJ = -7; iJ <= 7; iJ++)
						dVal += vDeriv[uiI - iJ] / 15.0;
					m_vFlux_Deriv.push_back(dVal);
				}
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);
				m_vFlux_Deriv.push_back(0.0);

				m_vFlux_Second_Deriv.clear();
				m_vFlux_Second_Deriv.push_back(0.0);
				m_vFlux_Second_Deriv.push_back(0.0);
				m_vFlux_Second_Deriv.push_back(0.0);
				m_vFlux_Second_Deriv.push_back(0.0);
				m_dFlux_Second_Deriv_Max = 0.0;
				for (unsigned int uiI = 4; uiI < (m_vFlux.size() - 4); uiI++)
				{
					double	dA = m_vFlux[uiI + 4] / -560.0;
					double	dB = m_vFlux[uiI + 3] / 315.0 * 8.0;
					double	dC = m_vFlux[uiI + 2] / -5.0;
					double	dD = m_vFlux[uiI + 1] / 5.0 * 8.0;
					double	dE = m_vFlux[uiI] / 72.0 * -205.0;
					double	dF = m_vFlux[uiI - 1] / 5.0 * 8.0;
					double	dG = m_vFlux[uiI - 2] / -5.0;
					double	dH = m_vFlux[uiI - 3] / 315.0 * 8.0;
					double	dI = m_vFlux[uiI - 4] / -560.0;
 
					double dDeriv = (dA + dB + dC + dD + dE + dF + dG + dH + dI) * dInv_dW * dInv_dW;
					m_vFlux_Second_Deriv.push_back(dDeriv);
					if (fabs(dDeriv) > m_dFlux_Second_Deriv_Max)
						m_dFlux_Second_Deriv_Max = fabs(dDeriv);
				}
				m_vFlux_Second_Deriv.push_back(0.0);
				m_vFlux_Second_Deriv.push_back(0.0);
				m_vFlux_Second_Deriv.push_back(0.0);
				m_vFlux_Second_Deriv.push_back(0.0);
			}
		}
	}
}

void FIT_VIZ_MAIN::Export_Graphic(void)
{
	epsplot::page_parameters	cPlot_Parameters;
	epsplot::data cPlot;
	epsplot::axis_parameters	cX_Axis_Parameters;
	epsplot::axis_parameters	cY_Axis_Parameters;
	epsplot::line_parameters	cLine_Parameters;
	std::vector<epsplot::eps_pair>		vpdGaussian;
	cPlot_Parameters.m_uiNum_Columns = 1;
	cPlot_Parameters.m_uiNum_Rows = 1;
	cPlot_Parameters.m_dWidth_Inches = 11.0;
	cPlot_Parameters.m_dHeight_Inches = 8.5;

	cX_Axis_Parameters.Set_Title("Wavelength [\\Aring]");
	cX_Axis_Parameters.m_dMajor_Label_Size = 24.0;
	cY_Axis_Parameters.Set_Title("Flux");
	cY_Axis_Parameters.m_dLower_Limit = 0.0;
	cY_Axis_Parameters.m_dUpper_Limit = 2.0;
	cY_Axis_Parameters.m_bLabel_Major_Indices = false;

	unsigned int uiX_Axis = cPlot.Set_X_Axis_Parameters( cX_Axis_Parameters);
	unsigned int uiY_Axis = cPlot.Set_Y_Axis_Parameters( cY_Axis_Parameters);
	cLine_Parameters.m_eColor = epsplot::BLACK;
	cLine_Parameters.m_eStipple = epsplot::SOLID;
	cPlot.Set_Plot_Data(m_vWavelength, m_vFlux, cLine_Parameters, uiX_Axis, uiY_Axis);


	gauss_fit_parameters * lpgfpParamters = &g_cgfpCaNIR;
	for (unsigned int uiI = 0; uiI < m_vWavelength.size(); uiI++)
	{
		double dX = (m_vWavelength[uiI] - m_vWavelength[0]);
		double dY;
		switch (m_eFit_Method)
		{
		case fm_flat:
			dY = (1.0 + Multi_Gaussian(m_vWavelength[uiI],m_vGaussian_Fit_Data,lpgfpParamters).Get(0));
			break;
		case fm_jeff:
			dY = Multi_Gaussian(m_vWavelength[uiI],m_vGaussian_Fit_Data,lpgfpParamters).Get(0);
			dY += (m_vWavelength[uiI] - m_dJeff_WL) * m_dJeff_Slope + m_dJeff_Flux;
			break;
		case fm_manual:
			dY = Multi_Gaussian(m_vWavelength[uiI],m_vManual_Fit_Data,lpgfpParamters).Get(0);
			dY += (m_vWavelength[uiI] - m_dMF_WL) * m_dMF_Slope + m_dMF_Flux;
			break;
		}
		vpdGaussian.push_back(epsplot::eps_pair(m_vWavelength[uiI],dY));
	}
	cLine_Parameters.m_eColor = epsplot::RED;
	cLine_Parameters.m_eStipple = epsplot::DOTTED;
	cPlot.Set_Plot_Data(vpdGaussian, cLine_Parameters, uiX_Axis, uiY_Axis);

	
	cLine_Parameters.m_eColor = epsplot::GREY_75;
	cLine_Parameters.m_eStipple = epsplot::LONG_DASH;
	vpdGaussian.clear();
	switch (m_eFit_Method)
	{
	case fm_flat:
		vpdGaussian.push_back(epsplot::eps_pair(m_vWavelength[0],1.0));
		vpdGaussian.push_back(epsplot::eps_pair(m_vWavelength[m_vWavelength.size() - 1],1.0));
		break;
	case fm_jeff:
		vpdGaussian.push_back(epsplot::eps_pair(m_vWavelength[0],(m_vWavelength[0] - m_dJeff_WL) * m_dJeff_Slope + m_dJeff_Flux));
		vpdGaussian.push_back(epsplot::eps_pair(m_vWavelength[m_vWavelength.size() - 1],(m_vWavelength[m_vWavelength.size() - 1] - m_dJeff_WL) * m_dJeff_Slope + m_dJeff_Flux));
		break;
	case fm_manual:
		vpdGaussian.push_back(epsplot::eps_pair(m_vWavelength[0],(m_vWavelength[0] - m_dMF_WL) * m_dMF_Slope + m_dMF_Flux));
		vpdGaussian.push_back(epsplot::eps_pair(m_vWavelength[m_vWavelength.size() - 1],(m_vWavelength[m_vWavelength.size() - 1] - m_dMF_WL) * m_dMF_Slope + m_dMF_Flux));
		break;
	}
	
	cPlot.Set_Plot_Data(vpdGaussian, cLine_Parameters, uiX_Axis, uiY_Axis);

	std::ostringstream szFilename;
	szFilename << "FitViz_export_" << m_vModel_List[m_uiSelected_Model] << "_" << m_vDay_List[m_uiSelected_Day];
	if (m_eFit_Method == fm_flat)
		szFilename << "_Flat.eps";
	else if (m_eFit_Method == fm_jeff)
		szFilename << "_Jeff.eps";
	else // if (m_eFit_Method == fm_manual)
		szFilename << "_Manual.eps";

	cPlot.Set_Plot_Filename(szFilename.str().c_str());
	cPlot.Plot(cPlot_Parameters);

}
