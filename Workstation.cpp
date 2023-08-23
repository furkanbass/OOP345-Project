// Name: Furkan Bas
// Seneca Student ID: 121540215
// Seneca email: fbas@myseneca.ca
// Date of completion: August 5, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#include "Workstation.h"

namespace sdds
{
    std::deque<CustomerOrder> g_pending{};
    std::deque<CustomerOrder> g_completed{};
    std::deque<CustomerOrder> g_incomplete{};

    Workstation::Workstation(const std::string& str) : Station(str)
    {}

    void Workstation::fill(std::ostream& os)
    {
        if (!m_orders.empty()) {
            if (m_orders.size()) {
                m_orders.front().fillItem(*this, os);
            }
        }
    }

    bool Workstation::attemptToMoveOrder()
    {
        bool flag = false;

        if (!m_orders.empty()) {
            CustomerOrder& order = m_orders.front();

            if (order.isItemFilled(this->getItemName()) || this->getQuantity() == 0) {
                if (m_pNextStation) {
                    *m_pNextStation += (std::move(order));
                }
                else
                {
                    if (order.isOrderFilled()) {
                        g_completed.push_back(std::move(order));
                    }
                    else {
                        g_incomplete.push_back(std::move(order));
                    }
                }
                flag = true;
                m_orders.pop_front();
            }
        }

        return flag;
    }

    void Workstation::setNextStation(Workstation* station)
    {
        m_pNextStation = station;
    }

    Workstation* Workstation::getNextStation() const
    {
        return m_pNextStation;
    }

    void Workstation::display(std::ostream& os) const
    {
        if (!m_pNextStation)
        {
            os << getItemName() << " --> " << "End of Line" << std::endl;
        }
        else
        {
            os << getItemName() << " --> " << m_pNextStation->getItemName() << std::endl;
        }
    }

    Workstation& Workstation::operator+=(CustomerOrder&& newOrder)
    {
        m_orders.push_back(std::move(newOrder));
        return *this;
    }


}