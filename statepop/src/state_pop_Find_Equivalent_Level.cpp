#include <statepop.h>
opacity_project_level_descriptor statepop::Find_Equivalent_Level(const statepop::floattype & i_dElement_Code, const statepop::floattype & i_lpdEnergy_Level_cm, const statepop::floattype & i_dGamma, const std::string & i_sLabel, const mcfg &i_mcfgConfigs, const vecconfig & i_vcfgConfig, bool i_bQuiet)
{
	opacity_project_level_descriptor opldEquivalent_Level;
	bool bFound = false;
	for(auto iterK = i_mcfgConfigs.cbegin(); iterK != i_mcfgConfigs.cend() && !bFound; iterK++)
	{
		if (i_vcfgConfig == iterK->second)
		{
			bFound = true;
			opldEquivalent_Level = iterK->first;
		}
	}
	if (!bFound)
	{
		// try again with lower state SLP info stripped
		std::vector<opacity_project_level_descriptor> vMatches;
		statepop::vecconfig vcTest = i_vcfgConfig;
		if (vcTest[1].m_uiS != -1 || vcTest[1].m_uiP != -1 || vcTest[1].m_uiL != -1)
		{
			vcTest[1].m_uiS = vcTest[1].m_uiP = -1; vcTest[1].m_uiL = -1;
			for(auto iterK = i_mcfgConfigs.cbegin(); iterK != i_mcfgConfigs.cend() && !bFound; iterK++)
			{
				if (vcTest == iterK->second)
				{
					bFound = true;
					opldEquivalent_Level = iterK->first;
				}
			}
		}
		if (!bFound  &&  !i_bQuiet)
		{
			// try again with highest state SLP info stripped
			vcTest[0].m_uiS = vcTest[0].m_uiP = -1; vcTest[0].m_uiL = -1;
			for(auto iterK = i_mcfgConfigs.cbegin(); iterK != i_mcfgConfigs.cend(); iterK++)
			{
				//Print_Config_Vector(iterK->second,std::cerr);
				if (vcTest == iterK->second)
				{
					vMatches.push_back(iterK->first);
				}
			}

			std::cerr << "Unable to correlate Kurucz state (" << i_dElement_Code << ") " << i_sLabel << " [" << i_lpdEnergy_Level_cm << ", " << i_dGamma << "]" << std::endl;
			std::cerr << "Possible matches:" << std::endl;
			if (vMatches.size() >= 1)
			{
				for (auto iterK = vMatches.begin(); iterK != vMatches.end(); iterK++)
				{
					std::cerr << iterK->m_uiS << " " << iterK->m_uiL << " " << iterK->m_uiP << " " << iterK->m_uiLvl_ID << std::endl;
				}
			}
			else
				std::cerr << "None" << std::endl;
			Print_Config_Vector(i_vcfgConfig,std::cerr);
		}
	}
	return opldEquivalent_Level;
}
