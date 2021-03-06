#include <core.hpp>
#include <xio.h>
#include <xlinalg.h>
#include <line_routines.h>
#include <thread>
#include <OSC.hpp>
#include <opacity_profile_data.h>

enum button_id
{
	error_ack,
	spectrum_select_up,
	spectrum_select_down,
	feature_select_up,
	feature_select_down,
	model_select_up,
	model_select_down,
	temp_select_up,
	temp_select_down,
	exc_temp_select_up,
	exc_temp_select_down,
	exc_temp_select_big_up,
	exc_temp_select_big_down,
	elem_select_up,
	elem_select_down,
	ion_select_up,
	ion_select_down,
	temp_select_big_up,
	temp_select_big_down,
	velocity_select_big_up,
	velocity_select_big_down,
	velocity_select_up,
	velocity_select_down,
	shell_scalar_up,
	shell_scalar_down,
	ejecta_scalar_up,
	ejecta_scalar_down,
	shell_scalar_big_up,
	shell_scalar_big_down,
	ejecta_scalar_big_up,
	ejecta_scalar_big_down,
	norm_wl_blue_up,
	norm_wl_blue_down,
	norm_wl_red_up,
	norm_wl_red_down,
	norm_wl_blue_big_up,
	norm_wl_blue_big_down,
	norm_wl_red_big_up,
	norm_wl_red_big_down,
	ref_fit_range_blue_big_up,
	ref_fit_range_blue_up,
	ref_fit_range_blue_big_down,
	ref_fit_range_blue_down,
	ref_fit_range_red_big_up,
	ref_fit_range_red_up,
	ref_fit_range_red_big_down,
	ref_fit_range_red_down,
	abort_request,
	gen_request,
	copy_refine_request,
	save_request,
	save_refine_request,
	refine_request,
	generated_fit_request,
	data_fit_request,
	gauss_fit_data_range_blue_big_up,
	gauss_fit_data_range_blue_up,
	gauss_fit_data_range_blue_big_down,
	gauss_fit_data_range_blue_down,
	gauss_fit_data_range_red_big_up,
	gauss_fit_data_range_red_up,
	gauss_fit_data_range_red_big_down,
	gauss_fit_data_range_red_down,
	gauss_fit_gen_range_blue_big_up,
	gauss_fit_gen_range_blue_up,
	gauss_fit_gen_range_blue_big_down,
	gauss_fit_gen_range_blue_down,
	gauss_fit_gen_range_red_big_up,
	gauss_fit_gen_range_red_up,
	gauss_fit_gen_range_red_big_down,
	gauss_fit_gen_range_red_down,
	flux_zoom_in,
	flux_zoom_out,
	toggle_component_display_request,
	toggle_refine_display_request,
	toggle_generated_display_request,
	clear_request,
	spectrum_display_area,
	quit_request

};

enum feature_select {fs_start,fs_full,fs_CaHK,fs_Si6355,fs_CaNIR,fs_end};

class gaussian_fit_data
{
public:
	bool			m_bValid;
	bool			m_bType; // 1 = generated, 0 = spectrum. Not needed by fitting routine.

	double			m_dRange_WL_Blue;
	double			m_dRange_WL_Red;

	double			m_d_pEW[2];
	double			m_dVelocity[2];

	double			m_dQuality_of_Fit;

	feature_select	m_eFeature;

	OSCspectrum		m_specResult;
	OSCspectrum		m_specResult_Error;

	gaussian_fit_data(void)
	{
		m_bValid = false;
	}
};

void Process_Generate_Requests(void);
void Process_Refine_Requests(void);
void Process_Fit_Requests(void);

double Get_Norm_Const(const OSCspectrum & i_cTarget, const double & i_dNorm_Blue, const double & i_dNorm_Red);
double Get_Norm_Const(const ES::Spectrum & i_cTarget, const double & i_dNorm_Blue, const double & i_dNorm_Red);

OSCspectrum Copy(const ES::Spectrum & i_cRHO);

class OSCIn_SuShI_main : public main
{
private:
	pane_id	m_idPane;
	pane_id m_idError_Pane;

	OSCfile	m_OSCfile;
	std::map<OSCspectra_id, OSCspectrum >::iterator m_iterSelected_ID;
	std::map<OSCspectra_id, OSCspectrum >::iterator m_iterLast_ID;

	// items for display
	OSCspectra_id	m_idSelected_ID;
	OSCspectrum		m_specSelected_Spectrum;
	double			m_dSelected_Flux_Max[4];

	spectrum_data	m_sdGenerated_Spectrum;
	double			m_dGenerated_Spectrum_Norm;
	spectrum_data	m_sdGenerated_Spectrum_Prev;
	double			m_dGenerated_Spectrum_Prev_Norm;

	std::map<button_id, BUTTON_INFO> m_mMain_Pane_Buttons;
	std::map<button_id, BUTTON_INFO> m_mError_Pane_Buttons;
	std::vector<std::string> m_vsError_Info;
	criticalsection	m_csEvent_Queue;

	feature_select m_eFeature_Select;
	std::thread 	m_thrGen;
	std::thread 	m_thrRefine;
	std::thread		m_thrFit;

	bool			m_bQuit_Request_Pending;


	double			m_dPS_Temp;
	double			m_dPS_Velocity;
	double			m_dEjecta_Scalar;
	double			m_dShell_Scalar;
	double			m_dExc_Temp;
	unsigned int	m_uiModel;

	double			m_dNorm_Blue;
	double			m_dNorm_Red;

	double			m_dRefine_Blue;
	double			m_dRefine_Red;

	double			m_dGauss_Fit_Blue;
	double			m_dGauss_Fit_Red;

	double			m_dGauss_Fit_Gen_Blue;
	double			m_dGauss_Fit_Gen_Red;

	double			m_dFlux_Zoom;

	unsigned int	m_uiElement;
	unsigned int	m_uiIon;

	double			m_dRow_0_Text;
	double			m_dRow_1_Text;
	double			m_dRow_2_Text;
	double			m_dRow_3_Text_Labels;
	double			m_dRow_3_Text;
	double			m_dRow_3a_Text;
	double			m_dRow_4_Text;
	double			m_dRow_4a_Text;
	double			m_dRow_5_Text;
	double			m_dRow_5a_Text;
	double			m_dRow_6_Text;
	double			m_dRow_6a_Text;
	double			m_dRow_7_Text;
	double			m_dRow_7a_Text;
	double			m_dText_Y[20];
	double			m_dText_Comp_Y[20];

	spectrum_data		m_sdRefine_Spectrum_Curr;
	double				m_dRefine_Spectrum_Norm;
	spectrum_data		m_sdRefine_Spectrum_Best;
	double				m_dRefine_Spectrum_Best_Norm;
	unsigned int		m_uiRefine_Result_ID;

	gaussian_fit_data	m_cTarget_Gaussian_Fit;
	gaussian_fit_data	m_cModel_Gaussian_Fit;

	double				m_dDirect_Measure_pEW;
	double				m_dGen_Direct_Measure_pEW;
	double				m_dRefine_Direct_Measure_pEW;

	double				m_dDirect_Measure_Vel;
	double				m_dGen_Direct_Measure_Vel;
	double				m_dRefine_Direct_Measure_Vel;

	double				m_dTimer;
	bool				m_bFlasher_1s_50p;
	std::deque<button_id> m_qEvent_List;

	std::map<unsigned int, model> m_mapModels;

	bool				m_bDisplay_Components;

	void Process_Selected_Spectrum(void);
	void Normalize(void);
	void Calc_Observed_pEW(void);

	double Calc_pEW(const OSCspectrum & i_cSpectrum, const double & i_dBlue_WL, const double & i_dRed_WL);
	double Calc_Vel(const OSCspectrum & i_cSpectrum, const double & i_dBlue_WL, const double & i_dRed_WL);



	void Display_Spectrum_Information(const spectrum_data & i_cSpectrum, const double & i_dX_Position, const double * i_lpdColor);
	void Save_Result(const std::string &i_szFilename, unsigned int m_uiModel, const spectrum_data & i_cSpectrum, bool i_bRefined_Data);

public:
	OSCIn_SuShI_main(void) : m_thrGen(Process_Generate_Requests), m_thrRefine(Process_Refine_Requests), m_thrFit(Process_Fit_Requests)  {m_thrGen.detach();m_thrRefine.detach();m_thrFit.detach();};
private:
	void on_key_down(KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State);
	void on_key_up(KEYID eKey_ID, unsigned char chScan_Code, unsigned int uiRepeat_Count, bool bExtended_Key, bool bPrevious_Key_State);
	void on_mouse_button_double_click(MOUSEBUTTON i_eButton, const PAIR<unsigned int> &i_tMouse_Position);
	void on_mouse_button_down(MOUSEBUTTON i_eButton, const PAIR<unsigned int> &i_tMouse_Position);
	void on_mouse_button_up(MOUSEBUTTON i_eButton, const PAIR<unsigned int> &i_tMouse_Position);
	void on_mousemove(const PAIR<unsigned int> &i_tMouse_Position);
	void on_mouse_wheel(MOUSEBUTTON i_eWheel, int i_iWheel_Delta, const PAIR<unsigned int> &i_tMouse_Position);
	void on_timer(unsigned int i_uiTimer_ID, const double & i_dDelta_Time_s);

	void init(void); // initialization routine; rendering context not created
	void gfx_display(pane_id i_idPane); // primary display routine
	void gfx_init(void); // initialization routine; rendering context already created
	void gfx_reshape(const PAIR<unsigned int> & i_tNew_Size); // window size change
	void gfx_close(void); // graphics exiting; rendering context still active
	void close(void); // program exiting; rendering context destroyed
};

extern bool				g_bAbort_Request;
extern bool				g_bQuit_Thread;
extern bool 			g_bGen_Process_Request;
extern bool				g_bGen_In_Progress;
extern bool				g_bGen_Done;
extern spectrum_data		g_sdGen_Spectrum_Result;
extern model *				g_lpmGen_Model;
extern bool				g_bGen_Thread_Running;

extern bool				g_bRefine_Process_Request;
extern bool				g_bRefine_In_Progress;
extern bool				g_bRefine_Done;
extern bool				g_bRefine_Thread_Running;
extern unsigned int		g_uiRefine_Result_ID;
extern spectrum_data		g_sdRefine_Result;
extern model *				g_lpmRefine_Model;
extern spectrum_data		g_sdRefine_Result_Curr;
extern spectrum_data		g_sdRefine_Result_Curr_Best;
extern unsigned int		g_uiRefine_Max;

extern bool				g_bFit_Process_Request;
extern bool				g_bFit_In_Progress;
extern bool				g_bFit_Done;
extern bool				g_bFit_Thread_Running;
extern gaussian_fit_data	g_cFit_Result;

