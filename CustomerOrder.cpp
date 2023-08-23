// Name: Furkan Bas
// Seneca Student ID: 121540215
// Seneca email: fbas@myseneca.ca
// Date of completion: August 5, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#include <iostream>
#include <iomanip>
#include "CustomerOrder.h"
#include "Utilities.h"

namespace sdds
{
	size_t CustomerOrder::m_widthField{ 0 };

	CustomerOrder::CustomerOrder(const std::string& str)
	{
		Utilities u;
		size_t next_pos = 0;
		auto more = true;

		m_name = u.extractToken(str, next_pos, more);
		m_product = u.extractToken(str, next_pos, more);
		m_cntItem = 0;

		auto start_pos = next_pos;

		while (more)
		{
			u.extractToken(str, next_pos, more);
			m_cntItem++;
		}

		m_lstItem = new Item * [m_cntItem];
		more = true;

		for (size_t i = 0; i < m_cntItem; i++)
		{
			m_lstItem[i] = new Item(u.extractToken(str, start_pos, more));
		}

		if (m_widthField < u.getFieldWidth())
		{
			m_widthField = u.getFieldWidth();
		}
	}

	CustomerOrder::CustomerOrder(const CustomerOrder& src)
	{
		throw std::string("Cannot make copies.");
	}

	CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept
	{
		*this = std::move(src);
	}

	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept
	{
		if (this != &src) {
			for (size_t i = 0; i < m_cntItem; i++) {
				delete m_lstItem[i];
			}
			if (m_lstItem != nullptr)
				delete[] m_lstItem;
			m_lstItem = src.m_lstItem;
			m_name = src.m_name;
			m_product = src.m_product;
			m_cntItem = src.m_cntItem;
			src.m_lstItem = nullptr;
			src.m_name = {};
			src.m_product = {};
			src.m_cntItem = 0;
		}
		return *this;
	}

	CustomerOrder::~CustomerOrder()
	{
		for (size_t i = 0; i < m_cntItem; i++) {
			if (m_lstItem[i] != nullptr) {
				delete m_lstItem[i];
				m_lstItem[i] = nullptr;
			}
		}
		if (m_lstItem != nullptr) {
			delete[] m_lstItem;
			m_lstItem = nullptr;
		}
	}

	bool CustomerOrder::isOrderFilled() const
	{
		bool flag = true;

		for (size_t i = 0; i < m_cntItem && flag; i++) {
			if (!m_lstItem[i]->m_isFilled)
				flag = false;
		}
		return flag;
	}

	bool CustomerOrder::isItemFilled(const std::string& itemName) const
	{
		bool flag = true;
		for (size_t i = 0; i < m_cntItem; i++) {
			if (m_lstItem[i]->m_itemName == itemName) {
				flag = m_lstItem[i]->m_isFilled;
			}
		}
		return flag;
	}

	void CustomerOrder::fillItem(Station& station, std::ostream& os)
	{
		for (auto i = 0u; i < m_cntItem; i++) {
			if (m_lstItem[i]->m_itemName == station.getItemName()) {
				if (station.getQuantity() > 0 && !m_lstItem[i]->m_isFilled)
				{
					station.updateQuantity();
					m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
					m_lstItem[i]->m_isFilled = true;
					os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
					break;
				}
				else if (station.getQuantity() == 0) {
					os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
				}
			}
		}
	}

	void CustomerOrder::display(std::ostream& os) const
	{
		os << m_name << " - " << m_product << std::endl;
		for (size_t i = 0; i < m_cntItem; i++)
		{
			os << "[" << std::setw(6) << std::right << std::setfill('0') << m_lstItem[i]->m_serialNumber << "] " << std::setw(m_widthField) << std::left << std::setfill(' ') << m_lstItem[i]->m_itemName << " - ";
			if (m_lstItem[i]->m_isFilled)
			{
				os << "FILLED" << std::endl;
			}
			else
			{
				os << "TO BE FILLED" << std::endl;
			}
		}
	}


}