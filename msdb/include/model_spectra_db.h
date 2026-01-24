#pragma once
#include <cmath>
#include <ctime>
#include "ES_Synow.hh"

namespace msdb
{
	typedef time_t dbid;

	enum	FEATURE	{CaNIR,CaHK,Si6355,O7773, Generic};
	enum	SPECTRUM_TYPE	{CONTINUUM, COMBINED, EJECTA_ONLY, SHELL_ONLY};

	inline double	Get_Line_Reference_Energy(FEATURE i_eFeature, unsigned int uiIon)
	{
		double	dRet = 0.0;
		switch (i_eFeature)
		{
		case CaNIR:
			dRet = (1.692408 * 2.0 + 1.699932) / 3.0;
			break;
		case CaHK:
			dRet = 0.0;
			break;
		case Si6355:
			dRet = 8.1312284;
			break;
		case O7773:
			dRet = 9.1460907;
			break;
		case Generic:
			if (uiIon == 2001)
				dRet = (1.692408 * 2.0 + 1.699932) / 3.0;
			else if (uiIon == 800)
				dRet = 9.1460907;
			else if (uiIon == 1401)
				dRet = 8.1312284;
			else
				std::cerr << "msdb: line reference energy unknown for ion " << uiIon << std::endl;
			break;
		}
		return dRet;
	}
	inline bool	Test_Ion(FEATURE i_eFeature, unsigned int i_uiIon)
	{
		bool	bRet;
		bRet = (i_eFeature == CaNIR || i_eFeature == CaHK) && i_uiIon == 2001;
		bRet |= (i_eFeature == O7773) && i_uiIon == 800;
		bRet |= (i_eFeature == Si6355) && i_uiIon == 1401;
		bRet |= (i_eFeature == Generic) && i_uiIon != 801; // O II causes crash in syn++
		return bRet;

	}
	class USER_PARAMETERS
	{
	public:
		unsigned int	m_uiModel_ID;
		unsigned int	m_uiIon;
		FEATURE m_eFeature;
		double	m_dTime_After_Explosion;
		double	m_dPhotosphere_Velocity_kkms;
		double	m_dPhotosphere_Temp_kK;
		double	m_dEjecta_Log_Scalar;
		double	m_dShell_Log_Scalar;
		double	m_dEjecta_Effective_Temperature_kK;
		double	m_dShell_Effective_Temperature_kK;
		double	m_dWavelength_Range_Lower_Ang;
		double	m_dWavelength_Range_Upper_Ang;
		double	m_dWavelength_Delta_Ang;
		double	m_dEjecta_Scalar_Time_Power_Law; // the power law which determines the rate at which the features weaken over time for the ejecta
		double	m_dShell_Scalar_Time_Power_Law; // the power law which determines the rate at which the features weaken over time for the shell

		USER_PARAMETERS(void)
		{
			m_uiModel_ID = 0;
			m_uiIon = 100; // Hydrogen
			m_eFeature = CaNIR;
			m_dTime_After_Explosion = 18.0;
			m_dPhotosphere_Velocity_kkms = 12.0;
			m_dPhotosphere_Temp_kK = 10.0;
			m_dEjecta_Log_Scalar = 1.0;
			m_dShell_Log_Scalar = 1.0;
			m_dEjecta_Effective_Temperature_kK = 10.0;
			m_dShell_Effective_Temperature_kK = 10.0;
			m_dWavelength_Range_Lower_Ang = 1500.0;
			m_dWavelength_Range_Upper_Ang = 10000.0;
			m_dWavelength_Delta_Ang = 1.0;
			m_dEjecta_Scalar_Time_Power_Law = -2.0;
			m_dShell_Scalar_Time_Power_Law = -2.0;
		}
	};


	class PARAMETERS
	{
	public:
		unsigned int	m_uiModel_ID;
		unsigned int	m_uiIon;
		double	m_dPhotosphere_Velocity_kkms;
		double	m_dPhotosphere_Temp_kK;
		double	m_dEjecta_Effective_Log_Scalar;
		double	m_dShell_Effective_Log_Scalar;
		double	m_dWavelength_Range_Lower_Ang;
		double	m_dWavelength_Range_Upper_Ang;
		double	m_dWavelength_Delta_Ang;

		PARAMETERS(void)
		{
			m_dPhotosphere_Velocity_kkms = 15.0;
			m_dPhotosphere_Temp_kK = 10.0;
			m_dEjecta_Effective_Log_Scalar = 0.0;
			m_dShell_Effective_Log_Scalar = 0.0;
			m_dWavelength_Range_Lower_Ang = 2500.0;
			m_dWavelength_Range_Upper_Ang = 9000.0;
			m_dWavelength_Delta_Ang = 5.0;
		}
		PARAMETERS(USER_PARAMETERS & i_cUser_Parameters)
		{
			m_uiModel_ID = i_cUser_Parameters.m_uiModel_ID;
			m_uiIon = i_cUser_Parameters.m_uiIon;
			m_dPhotosphere_Velocity_kkms = i_cUser_Parameters.m_dPhotosphere_Velocity_kkms;
			m_dPhotosphere_Temp_kK = i_cUser_Parameters.m_dPhotosphere_Temp_kK;
			Set_Ejecta_Effective_Scalar(i_cUser_Parameters.m_dTime_After_Explosion, i_cUser_Parameters.m_eFeature, i_cUser_Parameters.m_dEjecta_Effective_Temperature_kK, i_cUser_Parameters.m_dEjecta_Log_Scalar,i_cUser_Parameters.m_dEjecta_Scalar_Time_Power_Law);
			Set_Shell_Effective_Scalar(i_cUser_Parameters.m_dTime_After_Explosion, i_cUser_Parameters.m_eFeature, i_cUser_Parameters.m_dShell_Effective_Temperature_kK, i_cUser_Parameters.m_dShell_Log_Scalar,i_cUser_Parameters.m_dShell_Scalar_Time_Power_Law);
			m_dWavelength_Range_Lower_Ang = i_cUser_Parameters.m_dWavelength_Range_Lower_Ang;
			m_dWavelength_Range_Upper_Ang = i_cUser_Parameters.m_dWavelength_Range_Upper_Ang;
			m_dWavelength_Delta_Ang = i_cUser_Parameters.m_dWavelength_Delta_Ang;
		}
		double Generate_Effective_Scalar(const double	&i_dDays_After_Explosion, FEATURE i_eFeature, const double & i_dEffective_Temperature_kK, const double & i_dLog_Scalar, const double & i_dScalar_Power = -2.0) const
		{
			double	dRet = nan("");
			// 5.039... = log10 (e) / (k_B * 1000K)
			// Boltzmann constant from http://en.wikipedia.org/wiki/Boltzmann_constant, Ret. 27 May 2015, 17:40pm
			// days after explosion adjustment is relative to 1.0 day
			if (Test_Ion(i_eFeature, m_uiIon))
				dRet = i_dLog_Scalar + i_dScalar_Power * log10(i_dDays_After_Explosion) - Get_Line_Reference_Energy(i_eFeature,m_uiIon) * 5.03977864393654450859 * (1.0 / i_dEffective_Temperature_kK - 0.1); // effective temp relative to 10,000 K
			return dRet;
		}
		void	Set_Ejecta_Effective_Scalar(const double	&i_dDays_After_Explosion, FEATURE i_eFeature, const double & i_dEffective_Temperature_kK, const double & i_dLog_Scalar, const double & i_dScalar_Power = -2.0)
		{
			m_dEjecta_Effective_Log_Scalar = Generate_Effective_Scalar(i_dDays_After_Explosion, i_eFeature, i_dEffective_Temperature_kK, i_dLog_Scalar, i_dScalar_Power);
		}
		void	Set_Shell_Effective_Scalar(const double	&i_dDays_After_Explosion, FEATURE i_eFeature, const double & i_dEffective_Temperature_kK, const double & i_dLog_Scalar, const double & i_dScalar_Power = -2.0)
		{
			m_dShell_Effective_Log_Scalar = Generate_Effective_Scalar(i_dDays_After_Explosion, i_eFeature, i_dEffective_Temperature_kK, i_dLog_Scalar, i_dScalar_Power);
		}
	};


	class DATABASE_RECORD
	{
	private:
		unsigned int	m_uiRecord_Id;
		double			m_dRecord_Version;
		dbid			m_dbidRecord_Unique_ID;
		PARAMETERS		m_cParameters;
	public:

		DATABASE_RECORD(void);
		DATABASE_RECORD(unsigned int i_uiRecord_ID, const PARAMETERS		&i_cParameters);
		void	Read_Record(FILE * i_fileDatabase_Index);
		dbid	Write_Record(FILE * i_fileDatabase_Index);
		dbid	Get_ID(void) const{return m_dbidRecord_Unique_ID;}
		PARAMETERS		Get_Parameters(void) const {return m_cParameters;}
	};

	class DATABASE
	{
	private:
		double	m_dVersion; // software version
		double	m_dFile_Version; // current file version
		char * 	m_lpszFile_Path;
		char *	m_lpszSpectra_Path;
		char * 	m_lpszLock_Path;
		dbid	Write_Spectrum(dbid i_dbID, unsigned int i_uiModel_ID, SPECTRUM_TYPE i_eSpectrum_Type, const ES::Spectrum & i_cSpectrum) const;

	public:
		DATABASE(bool i_bAllow_Database_Creation = false);
		DATABASE(const DATABASE & i_cRHO);
		~DATABASE(void);

		bool	Database_Exists(void) const;
		dbid	Get_Spectrum(dbid i_dbidID, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & o_cSpectrum) const;
		dbid	Get_Spectrum(const PARAMETERS & i_cParameters, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & o_cSpectrum) const;
		dbid	Add_Spectrum(dbid i_dbidID, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & i_cSpectrum) const;
		dbid	Add_Spectrum(const PARAMETERS & i_cParameters, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & i_cSpectrum) const;

		// The following functions are for database manipulation.  General purpose users should not use these
		dbid			Get_DB_ID(const PARAMETERS & i_cParameters) const;
		dbid			Get_Parameters(dbid i_dbidID, PARAMETERS & o_cParameters) const;
		dbid			Update_Spectrum(dbid i_dbidRecord_ID, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & i_cSpectrum) const;
		void			Delete_Spectrum(dbid i_dbidRecord_ID, SPECTRUM_TYPE i_eSpectrum_Type) const;

		std::vector<dbid> Get_Spectra_List(void); // retrieve list of all dbid in the database
	};
	SPECTRUM_TYPE Identify_Spectrum_Type(const PARAMETERS & i_cParameters);

}
