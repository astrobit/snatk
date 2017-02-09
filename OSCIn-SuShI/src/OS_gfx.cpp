#include <OSCIn-SuShI>
#include <sstream>
#include <line_routines.h>
#include <iomanip>

void OSCIn_SuShI_main::gfx_init(void) // initialization routine; rendering context already created
{
	glClearColor(1.0,1.0,1.0,0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialf(GL_FRONT,GL_SPECULAR,10.0);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	SelectFontFace(SANS,true,false);
}
void OSCIn_SuShI_main::gfx_reshape(const PAIR<unsigned int> & i_tNew_Size) // window size change
{
	PAIR<unsigned int> m_tViewport_Size = i_tNew_Size;
	glViewport(0,0,(GLsizei) m_tViewport_Size.m_tX, (GLsizei) m_tViewport_Size.m_tY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,m_tViewport_Size.m_tX / m_tViewport_Size.m_tY,0.0,1.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);

	PAIR<unsigned int> tWindow_BL(0,0);
	PAIR<unsigned int> tWindow_TR((unsigned int)m_tViewport_Size.m_tX,(unsigned int)(m_tViewport_Size.m_tY));
	PAIR<unsigned int> tError_BL = tWindow_TR * 0.25;
	PAIR<unsigned int> tError_TR = tWindow_TR * 0.75;

	Move_Pane(m_idPane,QUAD<unsigned int> (tWindow_BL,tWindow_TR));
	Move_Pane(m_idError_Pane,QUAD<unsigned int> (tError_BL,tError_TR));

	double dAR = Get_Pane_Aspect_Ratio(m_idError_Pane);
	m_mError_Pane_Buttons.clear();
	m_mError_Pane_Buttons[error_ack] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(dAR * 0.5 - 0.05,0.20),PAIR<double>(0.14,0.07),error_ack);
	PAIR<double> pSB_Size = PAIR<double>(0.05,0.05);
	PAIR<double> pSmSB_Size = PAIR<double>(0.025,0.025);
	PAIR<double> pLB_Size = PAIR<double>(0.15,0.05);
	PAIR<double> pSmB_Size = PAIR<double>(0.05,0.05);
	PAIR<double> pTB_Size = PAIR<double>(0.10,0.10);
	PAIR<double> pIB_Size = PAIR<double>(0.10,0.04);
	PAIR<double> pIB_Size_Sml = PAIR<double>(0.10,0.03);

	dAR = Get_Pane_Aspect_Ratio(m_idPane);
	m_mMain_Pane_Buttons.clear();
	m_mMain_Pane_Buttons[spectrum_select_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.01,0.155),pSB_Size,spectrum_select_up);
	m_mMain_Pane_Buttons[spectrum_select_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.01,0.10),pSB_Size,spectrum_select_down);
	m_mMain_Pane_Buttons[feature_select_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.01,0.300),pSB_Size,feature_select_up);
	m_mMain_Pane_Buttons[feature_select_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.01,0.245),pSB_Size,feature_select_down);
	m_mMain_Pane_Buttons[spectrum_display_area] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(dAR * 0.01,0.30),PAIR<double>(dAR * 0.98,0.7),spectrum_display_area);

	m_mMain_Pane_Buttons[model_select_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.300),pSmSB_Size,model_select_up);
	m_mMain_Pane_Buttons[model_select_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.275),pSmSB_Size,model_select_down);
	m_mMain_Pane_Buttons[temp_select_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.250),pSmSB_Size,temp_select_up);
	m_mMain_Pane_Buttons[temp_select_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.225),pSmSB_Size,temp_select_down);
	m_mMain_Pane_Buttons[velocity_select_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.200),pSmSB_Size,velocity_select_up);
	m_mMain_Pane_Buttons[velocity_select_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.175),pSmSB_Size,velocity_select_down);
	m_mMain_Pane_Buttons[shell_scalar_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.150),pSmSB_Size,shell_scalar_up);
	m_mMain_Pane_Buttons[shell_scalar_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.125),pSmSB_Size,shell_scalar_down);
	m_mMain_Pane_Buttons[ejecta_scalar_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.100),pSmSB_Size,ejecta_scalar_up);
	m_mMain_Pane_Buttons[ejecta_scalar_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.65,0.075),pSmSB_Size,ejecta_scalar_down);

	m_mMain_Pane_Buttons[norm_wl_blue_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.95,0.150),pSmSB_Size,norm_wl_blue_up);
	m_mMain_Pane_Buttons[norm_wl_blue_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.95,0.125),pSmSB_Size,norm_wl_blue_down);
	m_mMain_Pane_Buttons[norm_wl_red_up] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.95,0.100),pSmSB_Size,norm_wl_red_up);
	m_mMain_Pane_Buttons[norm_wl_red_down] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.95,0.075),pSmSB_Size,norm_wl_red_down);


	m_mMain_Pane_Buttons[fit_request] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.70,0.05),pLB_Size,fit_request);
	m_mMain_Pane_Buttons[save_request] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(0.90,0.05),pLB_Size,save_request);
	m_mMain_Pane_Buttons[quit_request] = BUTTON_INFO(BUTTON_INFO::RECTANGLE,PAIR<double>(1.75,0.15),pLB_Size,quit_request);

}
void OSCIn_SuShI_main::gfx_close(void) // graphics exiting; rendering context still active
{
}

void Draw_Button_With_Text(const double & i_dSize, std::string i_szText)
{
	Draw_Rounded_Rectangle(true);
	glColor4d(0.0,0.0,0.0,1.0);
	glLineWidth(2.0);
	Draw_Rounded_Rectangle(false);
	glPushMatrix();
		glTranslated(0.5,-0.5,0.0);
		glScaled(1.0/i_dSize,1.0,1.0);
		glPrintJustified(0.4,0.0,0.0,0.0,i_szText.c_str(),HJ_CENTER,VJ_MIDDLE);
	glPopMatrix();
}

void OSCIn_SuShI_main::gfx_display(pane_id i_idPane) // primary display routine
{
	if (g_vEllipse.empty())
	{
		GenerateEllipse(1.0,1.0,g_vEllipse);
	}
	if (i_idPane == m_idPane)
	{
		double dSize = Get_Pane_Aspect_Ratio(m_idPane);
		glColor4d(0.375,0.375,0.375,1.0);
		glBegin(GL_QUADS);
			glVertex2d(0.0,0.3);
			glVertex2d(dSize,0.3);
			glVertex2d(dSize,0.0);
			glVertex2d(0.0,0.0);
		glEnd();
		glColor4d(0.0,0.0,0.0,1.0);
		std::ostringstream ossMJD;
		std::string szFeature;

		double dText_Y = 0.02;
		ossMJD << "MJD " << m_idSelected_ID.m_dDate_MJD;

		std::ostringstream ossTemp;
		ossTemp << m_dPS_Temp << " K";

		std::ostringstream ossVel;
		ossVel << m_dPS_Velocity << " x1000 km/s";

		std::ostringstream ossSs;
		ossSs << "Ss: " << std::setprecision(3) << m_dShell_Scalar;

		std::ostringstream ossSe;
		ossSe << "Se: " << std::setprecision(3) << m_dEjecta_Scalar;

		std::ostringstream ossNorm_Blue;
		ossNorm_Blue << "Norm: " << std::setprecision(4) << m_dNorm_Blue;
		std::ostringstream ossNorm_Red;
		ossNorm_Red << "    : " << std::setprecision(4) << m_dNorm_Red;


		std::string szModel;

		switch (m_uiModel)
		{
		case 61:
			szModel = "61: 0.003 Msun";
			break;
		case 57:
			szModel = "57: 0.005 Msun";
			break;
		case 41:
			szModel = "41: 0.008 Msun";
			break;
		case 49:
			szModel = "49: 0.010 Msun";
			break;
		case 45:
			szModel = "45: 0.012 Msun";
			break;
		}

		glPushMatrix();
			glPrintJustified(0.05,0.075,dText_Y,0.0,m_idSelected_ID.m_szSources.c_str(),HJ_LEFT,VJ_BOTTOM);
			dText_Y += 0.06;
			glPrintJustified(0.05,0.075,dText_Y,0.0,m_idSelected_ID.m_szInstrument.c_str(),HJ_LEFT,VJ_BOTTOM);
			dText_Y += 0.06;
			glPrintJustified(0.05,0.075,dText_Y,0.0,ossMJD.str().c_str(),HJ_LEFT,VJ_BOTTOM);
			switch (m_eFeature_Select)
			{
			case fs_full:
				szFeature = "full";
				break;
			case fs_CaHK:
				szFeature = "Ca H&K";
				break;
			case fs_Si6355:
				szFeature = "Si 6355";
				break;
			case fs_CaNIR:
				szFeature = "Ca NIR";
				break;
			}
			glPrintJustified(0.05,0.075,0.25,0.0,szFeature.c_str(),HJ_LEFT,VJ_MIDDLE);


			glPrintJustified(0.03,0.70,0.275,0.0,szModel.c_str(),HJ_LEFT,VJ_MIDDLE);
			glPrintJustified(0.03,0.70,0.225,0.0,ossTemp.str().c_str(),HJ_LEFT,VJ_MIDDLE);
			glPrintJustified(0.03,0.70,0.175,0.0,ossVel.str().c_str(),HJ_LEFT,VJ_MIDDLE);
			glPrintJustified(0.03,0.70,0.125,0.0,ossSs.str().c_str(),HJ_LEFT,VJ_MIDDLE);
			glPrintJustified(0.03,0.70,0.075,0.0,ossSe.str().c_str(),HJ_LEFT,VJ_MIDDLE);

			glPrintJustified(0.03,0.98,0.125,0.0,ossNorm_Blue.str().c_str(),HJ_LEFT,VJ_MIDDLE);
			glPrintJustified(0.03,0.98,0.075,0.0,ossNorm_Red.str().c_str(),HJ_LEFT,VJ_MIDDLE);

			if (g_bGen_In_Progress && m_bFlasher_1s_50p)
				glPrintJustified(0.03,0.050,0.90,0.0,"Generating",HJ_LEFT,VJ_MIDDLE);

		glPopMatrix();
		for (std::map<button_id,BUTTON_INFO>::const_iterator cI = m_mMain_Pane_Buttons.begin(); cI != m_mMain_Pane_Buttons.end(); cI++)
		{
			PAIR<double> pdPosition = cI->second.GetPosition();
			PAIR<double> pdSize = cI->second.GetSize();

			glPushMatrix();
				glTranslated(pdPosition.m_tX,pdPosition.m_tY,0.0);
				glScaled(pdSize.m_tX,pdSize.m_tY,1.0);
				int iStatus = cI->second.GetStatus();
				switch (cI->second.GetID())
				{
				case spectrum_select_up:
				case feature_select_up:
				case model_select_up:
				case temp_select_up:
				case velocity_select_up:
				case shell_scalar_up:
				case ejecta_scalar_up:
				case norm_wl_blue_up:
				case norm_wl_red_up:
					Draw_Scroll_Button(SBD_UP, true);
					break;
				case spectrum_select_down:
				case feature_select_down:
				case model_select_down:
				case temp_select_down:
				case velocity_select_down:
				case shell_scalar_down:
				case ejecta_scalar_down:
				case norm_wl_blue_down:
				case norm_wl_red_down:
					Draw_Scroll_Button(SBD_DOWN, true);
					break;
				case fit_request:
					glColor4d(0.75,0.75,0.75,1.0);
					Draw_Button_With_Text(dSize,"Gen");
					break;
				case quit_request:
					glColor4d(0.75,0.75,0.75,1.0);
					Draw_Button_With_Text(dSize,"Quit");
					break;
				case save_request:
					glColor4d(0.75,0.75,0.75,1.0);
					Draw_Button_With_Text(dSize,"Save");
					break;

				case spectrum_display_area:
				
					if (m_specSelected_Spectrum.size() > 0)
					{
						//glTranslated(0.0,-1.0,0.0);
						{
							double dWL_Start = m_specSelected_Spectrum.wl(0);
							double dWL_End = m_specSelected_Spectrum.wl(m_specSelected_Spectrum.size() - 1);
							double dFlux_Max = m_dSelected_Flux_Max[0];
							switch (m_eFeature_Select)
							{
							case fs_CaHK:
								dWL_Start = 3000.0;
								dWL_End = 5000.0;
								dFlux_Max = m_dSelected_Flux_Max[1];
								break;
							case fs_Si6355:
								dWL_Start = 5000.0;
								dWL_End = 7000.0;
								dFlux_Max = m_dSelected_Flux_Max[2];
								break;
							case fs_CaNIR:
								dWL_Start = 7000.0;
								dWL_End = 9000.0;
								dFlux_Max = m_dSelected_Flux_Max[3];
								break;
							}
							glColor4d(0.0,0.0,0.0,1.0);

							glPushMatrix();
								double dWL_scale = 0.90 / (dWL_End - dWL_Start);
								double dFlux_Scale = 0.90 * 0.5 / (dFlux_Max * 1.25);
								glTranslated(0.05,0.1,0.0);

								double dWL_Values_Start;
								double dWL_Values_Step = 200.0;
								if ((dWL_End - dWL_Start) <= 2000.0)
									dWL_Values_Start = dWL_Start - fmod(dWL_Start,200.0);
								else if ((dWL_End - dWL_Start) <= 7000.0)
								{
									dWL_Values_Step = 500.0;
									dWL_Values_Start = dWL_Start - fmod(dWL_Start,1000.0);
								}
								else if ((dWL_End - dWL_Start) <= 10000.0)
								{
									dWL_Values_Step = 1000.0;
									dWL_Values_Start = dWL_Start - fmod(dWL_Start,1000.0);
								}
								else// if ((dWL_End - dWL_Start) <= 10000.0)
								{
									dWL_Values_Step = 2000.0;
									dWL_Values_Start = dWL_Start - fmod(dWL_Start,2000.0);
								}
								while (dWL_Values_Start < dWL_Start)
									dWL_Values_Start += dWL_Values_Step;

								for (double dWL_Val = dWL_Values_Start; dWL_Val <= dWL_End; dWL_Val += dWL_Values_Step)
								{
									double dX = (dWL_Val - dWL_Start) * dWL_scale;
									double dFont_Size = 0.03;
									if ((dWL_End - dWL_Start) > 7000.0)
									{
										glColor4d(0.75,0.75,0.75,1.0);
										dFont_Size = 0.03;
									}
									else if (fmod(dWL_Val,1000.0) == 0.0)
									{
										glColor4d(0.75,0.75,0.75,1.0);
										dFont_Size = 0.05;
									}
									else
										glColor4d(0.9325,0.9325,0.9325,1.0);
									glBegin(GL_LINES);
										glVertex2d(dX,0.0);
										glVertex2d(dX,1.0);
									glEnd();
									std::ostringstream ossWL;
									ossWL << dWL_Val;
									glColor4d(0.0,0.0,0.0,1.0);
									glPushMatrix();
										glTranslated(dX,0.0,0.0);
										glScaled(1.0/dSize,1.0,1.0);
										glPrintJustified(dFont_Size,0.0,-0.01,0.0,ossWL.str().c_str(),HJ_CENTER,VJ_TOP);
									glPopMatrix();
								}
								glColor4d(0.0,0.0,0.0,1.0);
								glBegin(GL_LINE_STRIP);
								for (unsigned int uiI = 0; uiI < m_specSelected_Spectrum.size(); uiI++)
								{
									if (m_specSelected_Spectrum.wl(uiI) >= dWL_Start && m_specSelected_Spectrum.wl(uiI) <= dWL_End)
									{
										double dX = (m_specSelected_Spectrum.wl(uiI) - dWL_Start) * dWL_scale;
										double dY = m_specSelected_Spectrum.flux(uiI) * dFlux_Scale;
										glVertex2d(dX,dY);
									}
								}
								glEnd();
								if (m_sdGenerated_Spectrum_Prev.m_bValid)
								{
									glColor4d(0.75,0.75,1.0,1.0);
									glBegin(GL_LINE_STRIP);
									for (unsigned int uiI = 0; uiI < m_sdGenerated_Spectrum_Prev.m_specResult.size(); uiI++)
									{
										if (m_sdGenerated_Spectrum_Prev.m_specResult.wl(uiI) >= dWL_Start && m_sdGenerated_Spectrum_Prev.m_specResult.wl(uiI) <= dWL_End)
										{
											double dX = (m_sdGenerated_Spectrum_Prev.m_specResult.wl(uiI) - dWL_Start) * dWL_scale;
											double dY = m_sdGenerated_Spectrum_Prev.m_specResult.flux(uiI) * m_dGenerated_Spectrum_Prev_Norm * dFlux_Scale;
											glVertex2d(dX,dY);
										}
									}
									glEnd();
								}
								if (m_sdGenerated_Spectrum.m_bValid)
								{
									glColor4d(0.0,0.0,1.0,1.0);
									glBegin(GL_LINE_STRIP);
									for (unsigned int uiI = 0; uiI < m_sdGenerated_Spectrum.m_specResult.size(); uiI++)
									{
										if (m_sdGenerated_Spectrum.m_specResult.wl(uiI) >= dWL_Start && m_sdGenerated_Spectrum.m_specResult.wl(uiI) <= dWL_End)
										{
											double dX = (m_sdGenerated_Spectrum.m_specResult.wl(uiI) - dWL_Start) * dWL_scale;
											double dY = m_sdGenerated_Spectrum.m_specResult.flux(uiI) * m_dGenerated_Spectrum_Norm * dFlux_Scale;
											glVertex2d(dX,dY);
										}
									}
									glEnd();
								}
							}
						glPopMatrix();
					}
					break;
				}
			glPopMatrix();
		}
	}
}

