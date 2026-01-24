#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <model_spectra_db.h>

using namespace msdb;
//@@TODO: this database is not thread- or process-safe.  Operations may be simultaneously adding, updating, or destroying in separate processes or threads.
//@@TODO: this database has no security other than general file access.  spectra added by one user may not be accessible by other users.  



PARAMETERS Generate_Corrected_Parameters(const PARAMETERS & i_cParameters, SPECTRUM_TYPE i_eSpectrum_Type)
{
	PARAMETERS cRet = i_cParameters;
	switch (i_eSpectrum_Type)
	{
	case CONTINUUM:
		cRet.m_uiModel_ID = 0;
		cRet.m_dEjecta_Effective_Log_Scalar = -20.0;
		cRet.m_dShell_Effective_Log_Scalar = -20.0;
		break;
	case EJECTA_ONLY:
		cRet.m_dShell_Effective_Log_Scalar = -20.0;
		break;
	case SHELL_ONLY:
		cRet.m_dEjecta_Effective_Log_Scalar = -20.0;
		break;
	}
	return cRet;
}


class DATABASE_HEADER
{
public:
	char	m_lpchDatabase_Identifier[4];
	double	m_dVersion;
	unsigned int	m_uiNum_Records;
	DATABASE_HEADER(const double i_dVersion = 1.0)
	{
		m_lpchDatabase_Identifier[0] = 'S';
		m_lpchDatabase_Identifier[1] = 'P';
		m_lpchDatabase_Identifier[2] = 'D';
		m_lpchDatabase_Identifier[3] = 'B';
		m_dVersion = i_dVersion;
		m_uiNum_Records = 0;
	}
		
};

DATABASE_RECORD::DATABASE_RECORD(void)
{
	m_dRecord_Version = 1.0;
	m_uiRecord_Id = 0;
	m_dbidRecord_Unique_ID = 0;
}
DATABASE_RECORD::DATABASE_RECORD(unsigned int i_uiRecord_ID, const PARAMETERS		&i_cParameters)
{
	timespec	stTime;
	clock_gettime(CLOCK_REALTIME,&stTime);
	m_dbidRecord_Unique_ID = stTime.tv_sec * 1000000000LL + stTime.tv_nsec;


	m_dRecord_Version = 1.0;
	m_uiRecord_Id = i_uiRecord_ID;
	m_cParameters = i_cParameters;
}
void	DATABASE_RECORD::Read_Record(FILE * i_fileDatabase_Index)
{
	fread(&m_dRecord_Version,sizeof(double),1,i_fileDatabase_Index);
	fread(&m_uiRecord_Id,sizeof(m_uiRecord_Id),1,i_fileDatabase_Index);
	fread(&m_dbidRecord_Unique_ID,sizeof(dbid),1,i_fileDatabase_Index);

	// version 1.0
	fread(&m_cParameters,sizeof(m_cParameters),1,i_fileDatabase_Index);
}

dbid	DATABASE_RECORD::Write_Record(FILE * i_fileDatabase_Index)
{
	fwrite(&m_dRecord_Version,sizeof(double),1,i_fileDatabase_Index);
	fwrite(&m_uiRecord_Id,sizeof(m_uiRecord_Id),1,i_fileDatabase_Index);
	fwrite(&m_dbidRecord_Unique_ID,sizeof(dbid),1,i_fileDatabase_Index);

	// version 1.0
	fwrite(&m_cParameters,sizeof(m_cParameters),1,i_fileDatabase_Index);

	return m_dbidRecord_Unique_ID;
}
void printcompare(unsigned int &uiA, unsigned int &uiB)
{
	printf("%i\t%i\t%i\n",uiA,uiB,memcmp(&uiA,&uiB,sizeof(unsigned int)));
}
void printcompare(const double &dA, const double &dB)
{
	printf("%f\t%f\t%i\n",dA,dB,memcmp(&dA,&dB,sizeof(double)));
}

dbid DATABASE::Get_DB_ID(const PARAMETERS & i_cParameters) const
{
	dbid dbidRet = 0;
	//printf("Searching database %s\n",m_lpszFile_Path);
	if (m_lpszFile_Path)
	{
		DATABASE_RECORD	cRecord;

		FILE * fileDatabase = fopen(m_lpszFile_Path,"rb");
		if (fileDatabase)
		{
			DATABASE_HEADER cDatabase_Header;
			fread(&cDatabase_Header,sizeof(cDatabase_Header),1,fileDatabase);
			//printf("Searching %i records\n",cDatabase_Header.m_uiNum_Records);
			for (unsigned int uiI = 0; uiI < cDatabase_Header.m_uiNum_Records && dbidRet == 0; uiI++)
			{
				cRecord.Read_Record(fileDatabase);
				PARAMETERS cParam = cRecord.Get_Parameters();
				//printcompare(cParam.m_uiModel_ID,i_cParameters.m_uiModel_ID);
				//printcompare(cParam.m_uiIon,i_cParameters.m_uiIon);
				//printcompare( cParam.m_dPhotosphere_Velocity_kkms,i_cParameters.m_dPhotosphere_Velocity_kkms);
				//printcompare(cParam.m_dPhotosphere_Temp_kK,i_cParameters.m_dPhotosphere_Temp_kK);
				//printcompare( cParam.m_dEjecta_Effective_Log_Scalar,i_cParameters.m_dEjecta_Effective_Log_Scalar);
				//printcompare( cParam.m_dShell_Effective_Log_Scalar,i_cParameters.m_dShell_Effective_Log_Scalar);
				//printcompare( cParam.m_dWavelength_Range_Lower_Ang,i_cParameters.m_dWavelength_Range_Lower_Ang);
				//printcompare( cParam.m_dWavelength_Range_Upper_Ang,i_cParameters.m_dWavelength_Range_Upper_Ang);
				//printcompare( cParam.m_dWavelength_Delta_Ang,i_cParameters.m_dWavelength_Delta_Ang);

				int iTest = memcmp(&cParam,&i_cParameters,sizeof(PARAMETERS));
				if (iTest == 0)
				{
					//printf("Match\n");
					dbidRet = cRecord.Get_ID();
				}
				//else
					//printf("No match: %i (%i,%i,%f,%f,%f,%f,%f,%f,%f)\n",iTest,cParam.m_uiModel_ID,cParam.m_uiIon,cParam.m_dPhotosphere_Velocity_kkms,cParam.m_dPhotosphere_Temp_kK,cParam.m_dEjecta_Effective_Log_Scalar,cParam.m_dShell_Effective_Log_Scalar,cParam.m_dWavelength_Range_Lower_Ang,cParam.m_dWavelength_Range_Upper_Ang,cParam.m_dWavelength_Delta_Ang);
			
			}
			fclose(fileDatabase);
		}
	}
	return dbidRet;
}
dbid DATABASE::Get_Parameters(dbid i_dbidID, PARAMETERS & o_cParameters) const
{
	dbid dbidRet = 0;
	if (m_lpszFile_Path)
	{

		FILE * fileDatabase = fopen(m_lpszFile_Path,"rb");
		if (fileDatabase)
		{
			DATABASE_HEADER cDatabase_Header;
			DATABASE_RECORD	cRecord;

			fread(&cDatabase_Header,sizeof(cDatabase_Header),1,fileDatabase);
			for (unsigned int uiI = 0; uiI < cDatabase_Header.m_uiNum_Records && dbidRet == 0; uiI++)
			{
				cRecord.Read_Record(fileDatabase);
				if (i_dbidID == cRecord.Get_ID())
				{
					o_cParameters = cRecord.Get_Parameters();
					dbidRet = cRecord.Get_ID();
				}
			}
			fclose(fileDatabase);
		}
	}
	return dbidRet;
}
dbid	DATABASE::Get_Spectrum(const PARAMETERS & i_cParameters, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & o_cSpectrum) const
{
	
	dbid dbidID = Get_DB_ID(Generate_Corrected_Parameters(i_cParameters,i_eSpectrum_Type));
//	printf("%i %i\n",i_cParameters.m_uiIon,dbidID);
	Get_Spectrum(dbidID, i_eSpectrum_Type, o_cSpectrum);
	return dbidID;
}

dbid	DATABASE::Get_Spectrum(dbid i_dbidID, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & o_cSpectrum) const
{
	dbid dbidID = 0;
	PARAMETERS cParameters_ID;
	if (Get_Parameters(i_dbidID,cParameters_ID) > 0)
	{
		PARAMETERS cParameters = Generate_Corrected_Parameters(cParameters_ID,i_eSpectrum_Type);
		dbidID = Get_DB_ID(cParameters);
		if (dbidID != i_dbidID)
			fprintf(stderr,"Warning: msdb get_spectrum by ID may retrieve wrong spectrum\n");
		if (dbidID > 0)
		{
			char lpszFilename[256];
			char	chType;
			switch (i_eSpectrum_Type)
			{
			case CONTINUUM:
				chType = 'C';
				break;
			case COMBINED:
				chType = 'B';
				break;
			case EJECTA_ONLY:
				chType = 'E';
				break;
			case SHELL_ONLY:
				chType = 'S';
				break;
			}
			sprintf(lpszFilename,"%s/%i%llu%c.spec",m_lpszSpectra_Path,cParameters.m_uiModel_ID,i_dbidID,chType);
//			printf("reading %i:%s\n",i_dbidID,lpszFilename);
			FILE * fileSpectrum = fopen(lpszFilename,"rb");
			if (fileSpectrum)
			{
				unsigned int uiNum_Entries;
				fread(&uiNum_Entries,sizeof(unsigned int), 1, fileSpectrum);
				if (uiNum_Entries > 0)
				{
					double	lpdData[3];
					o_cSpectrum = ES::Spectrum::create_from_size(uiNum_Entries);
					for (unsigned int uiI = 0; uiI < uiNum_Entries; uiI++)
					{
						fread(lpdData,sizeof(double), 3, fileSpectrum);
						o_cSpectrum.wl(uiI) = lpdData[0];
						o_cSpectrum.flux(uiI) = lpdData[1];
						o_cSpectrum.flux_error(uiI) = lpdData[2];
					}
				}
				else
					dbidID = 0;
				fclose(fileSpectrum);
			}
			else
				dbidID = 0;
		}
		else
			fprintf(stderr,"msdb failed to find entry for Get_Spectrum by ID\n");
	}
	return dbidID;
}


dbid	DATABASE::Add_Spectrum(const PARAMETERS & i_cParameters, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & i_cSpectrum) const
{
	PARAMETERS cCorr_Param = Generate_Corrected_Parameters(i_cParameters,i_eSpectrum_Type);
	dbid dbidID = Get_DB_ID(cCorr_Param);
//	if (dbidID != 0)
//		printf("Found id %i\n",dbidID);
	if (dbidID == 0)
	{ // parameters aren't listed in parameter table.  Add them.
		if (m_lpszFile_Path)
		{
			FILE * fileDatabase = fopen(m_lpszFile_Path,"r+b");
			if (fileDatabase)
			{
				DATABASE_HEADER cDatabase_Header;
				fread(&cDatabase_Header,sizeof(cDatabase_Header),1,fileDatabase);
				cDatabase_Header.m_uiNum_Records++;
				fseek(fileDatabase,0,SEEK_SET); // rewind to beginning of file
				fwrite(&cDatabase_Header,sizeof(cDatabase_Header),1,fileDatabase); // update header with new size
				PARAMETERS cParameters = i_cParameters;
				DATABASE_RECORD	cRecord(cDatabase_Header.m_uiNum_Records,cCorr_Param);
				PARAMETERS cParam = cRecord.Get_Parameters();
//				printf("Model id at write: %i\n",cParam.m_uiModel_ID);
				fseek(fileDatabase,0,SEEK_END); // forward to end of file
				cRecord.Write_Record(fileDatabase);
				fclose(fileDatabase);
				dbidID = cRecord.Get_ID();
				if (dbidID == 0)
					fprintf(stderr,"msdb: Parameter save failed\n");
			}
			else
				printf("Failed to open database for edit\n");
		}
	}
	if (dbidID != 0)
		Write_Spectrum(dbidID, cCorr_Param.m_uiModel_ID, i_eSpectrum_Type, i_cSpectrum);

	return dbidID;
}
dbid	DATABASE::Add_Spectrum(dbid i_dbidID, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & i_cSpectrum) const
{
	dbid dbidID;
	if (i_dbidID != 0)
	{
		PARAMETERS	cParams;
		if (Get_Parameters(i_dbidID,cParams) == i_dbidID)
		{
			dbidID = Add_Spectrum(cParams, i_eSpectrum_Type, i_cSpectrum);
			//Write_Spectrum(i_dbidID, cParams.m_uiModel_ID, i_eSpectrum_Type, i_cSpectrum);
		}
	}
	return dbidID;
}
dbid DATABASE::Update_Spectrum(dbid i_dbidID, SPECTRUM_TYPE i_eSpectrum_Type, ES::Spectrum & i_cSpectrum) const
{
	dbid dbidID = 0;
	if (i_dbidID != 0)
	{
		PARAMETERS	cParams;
		if (Get_Parameters(i_dbidID,cParams) == i_dbidID)
		{
			PARAMETERS cCorr_Param = Generate_Corrected_Parameters(cParams,i_eSpectrum_Type);
			dbidID = Get_DB_ID(cCorr_Param);
			Write_Spectrum(dbidID, cCorr_Param.m_uiModel_ID, i_eSpectrum_Type, i_cSpectrum);
		}
	}
	return dbidID;
}
void DATABASE::Delete_Spectrum(dbid i_dbidID, SPECTRUM_TYPE i_eSpectrum_Type) const
{
	if (i_dbidID != 0)
	{
		PARAMETERS	cParams;
		if (Get_Parameters(i_dbidID,cParams) == i_dbidID)
		{
			PARAMETERS cCorr_Param = Generate_Corrected_Parameters(cParams,i_eSpectrum_Type);
			dbid dbidID = Get_DB_ID(cCorr_Param);
			if (dbidID != 0)
			{
				char lpszTemp_Database[256];
				char lpszCommand[256];
				FILE * fileDatabase = fopen(m_lpszFile_Path,"rb");
				sprintf(lpszTemp_Database,"%s.tmp",m_lpszFile_Path);
				FILE * fileTmpDatabase = fopen(lpszTemp_Database,"wb");
				if (fileDatabase && fileTmpDatabase)
				{
					DATABASE_HEADER cDatabase_Header;
					DATABASE_RECORD	cRecord;

					fread(&cDatabase_Header,sizeof(cDatabase_Header),1,fileDatabase);
					fwrite(&cDatabase_Header,sizeof(cDatabase_Header),1,fileTmpDatabase);
					for (unsigned int uiI = 0; uiI < cDatabase_Header.m_uiNum_Records; uiI++)
					{
						cRecord.Read_Record(fileDatabase);
						if (cRecord.Get_ID() != dbidID)
							cRecord.Write_Record(fileTmpDatabase);
						else
						{
							char	chType;
							switch (i_eSpectrum_Type)
							{
							case msdb::CONTINUUM:
								chType = 'C';
								break;
							case msdb::COMBINED:
								chType = 'B';
								break;
							case msdb::EJECTA_ONLY:
								chType = 'E';
								break;
							case msdb::SHELL_ONLY:
								chType = 'S';
								break;
							}
							sprintf(lpszCommand,"rm %s/%i%llu%c.spec",m_lpszSpectra_Path,cCorr_Param.m_uiModel_ID,dbidID,chType);
							//printf("Writing %i:%s\n",dbidID,lpszFilename);
							system(lpszCommand);
						}
							
					}
					fclose(fileDatabase);
					fclose(fileTmpDatabase);
					sprintf(lpszCommand,"rm %s",m_lpszFile_Path);
					system(lpszCommand);
					sprintf(lpszCommand,"mv %s %s",lpszTemp_Database,m_lpszFile_Path);
					system(lpszCommand);

				}
			}
		}
	}
}

std::vector<dbid> DATABASE::Get_Spectra_List(void) // retrieve list of all dbid in the database
{
	std::vector<dbid> vRet;
	if (m_lpszFile_Path)
	{

		FILE * fileDatabase = fopen(m_lpszFile_Path,"rb");
		if (fileDatabase)
		{
			DATABASE_HEADER cDatabase_Header;
			DATABASE_RECORD	cRecord;

			fread(&cDatabase_Header,sizeof(cDatabase_Header),1,fileDatabase);
			for (unsigned int uiI = 0; uiI < cDatabase_Header.m_uiNum_Records; uiI++)
			{
				cRecord.Read_Record(fileDatabase);
				vRet.push_back(cRecord.Get_ID());
			}
			fclose(fileDatabase);
		}
	}
	return vRet;
}


dbid	DATABASE::Write_Spectrum(dbid i_dbID, unsigned int i_uiModel_ID, msdb::SPECTRUM_TYPE i_eSpectrum_Type, const ES::Spectrum & i_cSpectrum) const
{
	dbid dbidID = i_dbID;
	if (dbidID > 0)
	{
		char lpszFilename[256];
		char	chType;
		switch (i_eSpectrum_Type)
		{
		case msdb::CONTINUUM:
			chType = 'C';
			break;
		case msdb::COMBINED:
			chType = 'B';
			break;
		case msdb::EJECTA_ONLY:
			chType = 'E';
			break;
		case msdb::SHELL_ONLY:
			chType = 'S';
			break;
		}
		sprintf(lpszFilename,"%s/%i%llu%c.spec",m_lpszSpectra_Path,i_uiModel_ID,dbidID,chType);
		//printf("Writing %i:%s\n",dbidID,lpszFilename);
		FILE * fileSpectrum = fopen(lpszFilename,"wb");
		if (fileSpectrum)
		{
			unsigned int uiNum_Entries = i_cSpectrum.size();
			fwrite(&uiNum_Entries,sizeof(unsigned int), 1, fileSpectrum);
			if (uiNum_Entries > 0)
			{
				double	lpdData[3];
				for (unsigned int uiI = 0; uiI < uiNum_Entries; uiI++)
				{
					lpdData[0] = i_cSpectrum.wl(uiI);
					lpdData[1] = i_cSpectrum.flux(uiI);
					lpdData[2] = i_cSpectrum.flux_error(uiI);
					fwrite(lpdData,sizeof(double), 3, fileSpectrum);
				}
			}
			else
				dbidID = 0;
			fclose(fileSpectrum);
		}
		else
			dbidID = 0;
	}
	return dbidID;
}
DATABASE::DATABASE(bool i_bAllow_Database_Creation)
{
	m_dVersion = 1.0;
	m_lpszFile_Path = NULL;
	m_dFile_Version = 0.0;

	DATABASE_HEADER	cDatabase_Header;
	DATABASE_HEADER cDB_Header_Ref(m_dVersion);

	char lpszFilename[256];
	char lpszSpectra_Path[256];
	char * lpszDatabase_Path = getenv("MSDB_PATH");
	if (lpszDatabase_Path)
	{
		sprintf(lpszFilename,"%s/.specdbidx.index",lpszDatabase_Path);
		sprintf(lpszSpectra_Path,"%s/.specdb",lpszDatabase_Path);
		FILE * fileDatabase = fopen(lpszFilename,"rb");
		if (fileDatabase)
		{
			char	lpchDatabase_Identifier[4];
			fread(&cDatabase_Header,sizeof(cDatabase_Header),1,fileDatabase);
			if (memcmp(cDatabase_Header.m_lpchDatabase_Identifier,cDB_Header_Ref.m_lpchDatabase_Identifier,sizeof(cDB_Header_Ref.m_lpchDatabase_Identifier)) == 0)
			{
				if (cDatabase_Header.m_dVersion < m_dVersion)
				{
					fprintf(stderr,"Database (%s) out of date: file version %f, current software version %f.\n",lpszFilename,m_dFile_Version,m_dVersion);
					fflush(stderr);
				}
				m_dFile_Version = cDatabase_Header.m_dVersion;
				m_lpszFile_Path = new char[strlen(lpszFilename) + 1];
				strcpy(m_lpszFile_Path,lpszFilename);
				m_lpszSpectra_Path = new char[strlen(lpszSpectra_Path) + 1];
				strcpy(m_lpszSpectra_Path,lpszSpectra_Path);
			}
			else
			{
				fprintf(stderr,"Invalid spectral database (%s)\n",lpszFilename);
				fflush(stderr);
			}
			fclose(fileDatabase);
		}
		else if (i_bAllow_Database_Creation)
		{
			fileDatabase = fopen(lpszFilename,"wb");
			fwrite(&cDB_Header_Ref,sizeof(cDB_Header_Ref),1,fileDatabase);
			fclose(fileDatabase);
				
			mkdir(lpszSpectra_Path,0755);
			m_dFile_Version = m_dVersion;
			m_lpszFile_Path = new char[strlen(lpszFilename) + 1];
			strcpy(m_lpszFile_Path,lpszFilename);
			m_lpszSpectra_Path = new char[strlen(lpszSpectra_Path) + 1];
			strcpy(m_lpszSpectra_Path,lpszSpectra_Path);
		}
		else// if (!i_bAllow_Database_Creation)
		{
			fprintf(stderr,"Unable to open spectral database.  Ensure that msdb_PATH is set to the correct value.\n");
			fflush(stderr);
		}

	}
	else// if (!i_bAllow_Database_Creation)
	{
		fprintf(stderr,"msdb_PATH has not been specified.\n");
		fflush(stderr);
	}
}

DATABASE::DATABASE(const DATABASE & i_cRHO)
{
	m_dVersion = i_cRHO.m_dVersion;
	m_dFile_Version = i_cRHO.m_dFile_Version;
	m_lpszFile_Path = new char[strlen(i_cRHO.m_lpszFile_Path) + 1];
	strcpy(m_lpszFile_Path,i_cRHO.m_lpszFile_Path);
	m_lpszSpectra_Path = new char[strlen(i_cRHO.m_lpszSpectra_Path) + 1];
	strcpy(m_lpszSpectra_Path,i_cRHO.m_lpszSpectra_Path);
}

DATABASE::~DATABASE(void)
{
	if (m_lpszFile_Path)
		delete [] m_lpszFile_Path;
	m_lpszFile_Path = NULL;
	if (m_lpszSpectra_Path)
		delete [] m_lpszSpectra_Path;
	m_lpszSpectra_Path = NULL;
}

bool DATABASE::Database_Exists(void) const
{
	return  m_dFile_Version != 0.0;
}

SPECTRUM_TYPE msdb::Identify_Spectrum_Type(const PARAMETERS & i_cParameters)
{
	msdb::SPECTRUM_TYPE eType = msdb::COMBINED;
	if (i_cParameters.m_uiModel_ID == 0)
		eType = msdb::CONTINUUM;
	else if (i_cParameters.m_dEjecta_Effective_Log_Scalar == -20.0 && i_cParameters.m_dShell_Effective_Log_Scalar == -20.0)
		eType = msdb::CONTINUUM;
	else if (i_cParameters.m_dEjecta_Effective_Log_Scalar == -20.0)
		eType = msdb::SHELL_ONLY;
	else if (i_cParameters.m_dShell_Effective_Log_Scalar == -20.0)
		eType = msdb::EJECTA_ONLY;
	return eType;
}


	
