// Name: Furkan Bas
// Seneca Student ID: 121540215
// Seneca email: fbas@myseneca.ca
// Date of completion: August 5, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#include <fstream>
#include <string>
#include <algorithm>
#include "LineManager.h"
#include "Utilities.h"


namespace sdds
{
	LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations)
	{
		if (file.empty())
		{
			throw std::string("ERROR: No filename provided.");
		}
		std::ifstream filein(file);
		if (!filein)
			throw std::string("Unable to open [") + file + "] file.";

		std::string line;
		while (std::getline(filein, line))
		{
			std::string workstation = line.substr(0, line.find_first_of('|'));
			std::string nextWorkstation = line.erase(0, workstation.size() + 1);

			auto ws = std::find_if(stations.begin(), stations.end(), [&](Workstation* ws1)
				{
					return ws1->getItemName() == workstation;
				});
			m_activeLine.push_back(*ws);

			if (!nextWorkstation.empty())
			{
				for_each(stations.begin(), stations.end(), [&](Workstation* ws1)
					{
						if (ws1->getItemName() == nextWorkstation)
						m_activeLine.back()->setNextStation(ws1);
					});
			}
			else
			{
				m_activeLine.back()->setNextStation(nullptr);
			}
		}

		for_each(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* ws)
			{
				auto first = std::find_if(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* ws1)
					{
						return ws == ws1->getNextStation();
					});
		if (first == m_activeLine.end())
		{
			m_firstStation = ws;
		}
			});

		m_cntCustomerOrder = g_pending.size();
	}


	void LineManager::reorderStations()
	{
		std::vector<Workstation*> reorder;

		reorder.push_back(m_firstStation);
		if (m_firstStation->getNextStation()) {
			reorder.push_back(m_firstStation->getNextStation());
		}

		std::for_each(m_activeLine.begin(), m_activeLine.end() - 2, [&](Workstation* ws)
			{
				reorder.push_back(reorder.back()->getNextStation());
			});

		m_activeLine = reorder;
	}


	bool LineManager::run(std::ostream& os)
	{
		static unsigned int COUNTER = 0u;
		os << "Line Manager Iteration: " << ++COUNTER << std::endl;
		if (!g_pending.empty())
		{
			*m_firstStation += std::move(g_pending.front());
			g_pending.pop_front();

		}
		std::for_each(m_activeLine.begin(), m_activeLine.end(), [&os](Workstation* ws)
			{
				ws->fill(os);
			});
		std::for_each(m_activeLine.begin(), m_activeLine.end(), [](Workstation* ws)
			{
				ws->attemptToMoveOrder();
			});

		return g_completed.size() + g_incomplete.size() == m_cntCustomerOrder;
	}


	void LineManager::display(std::ostream& os) const
	{
		for_each(m_activeLine.begin(), m_activeLine.end(), [&](const Workstation* ws)
			{
				ws->display(os);
			});
	}
}