#include <specfit.h>

//------------------------------------------------------------------------------
//
//
//
//	Deredden
//
//
//
//------------------------------------------------------------------------------

void specfit::Deredden(std::vector <specfit::fit> &io_vfitFits)
{
	// process each model, and make sure that it has all of the needed info; fill the data points in the vector in the fit class for each one.
	for (std::vector <specfit::fit>::iterator iterFit = io_vfitFits.begin(); iterFit != io_vfitFits.end(); iterFit++)
	{
		if (!std::isnan(iterFit->m_dE_BmV) && iterFit->m_dE_BmV != 0.0 && !iterFit->m_vData.empty())
		{
			std::cout << "Dereddending " << std::setprecision(7) << iterFit->m_dMJD << " from " << iterFit->m_szSource << " using " << iterFit->m_szInstrument << " with E(B-v) = " << iterFit->m_dE_BmV << std::endl;
			iterFit->m_vData.Deredden(iterFit->m_dE_BmV);
		}
	}
}

