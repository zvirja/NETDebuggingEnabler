#pragma once
#include "baseset.h"
#include <functional>

namespace UI
{
	class CheckTextBoxesBundle
	{
	public:
		CheckTextBoxesBundle(wxCheckBox* checkbox, wxTextCtrl* textCtrl);
		CheckTextBoxesBundle() :m_checkbox(nullptr), m_textCtrl(nullptr), m_checkedChanged(), m_textChanged(){};
		~CheckTextBoxesBundle();
		void AddEnabledChangedHanler(std::function<void(bool)> hanlder) { m_checkedChanged.push_back(hanlder); }
		void AddTextChangedHanler(std::function<void(wxString&)> hanlder) { m_textChanged.push_back(hanlder); }
		void SetEnabled(bool enabled) { m_checkbox->SetValue(enabled); RenewTextCtrlEnabled(); }
		bool GetEnabled() { return m_checkbox->GetValue(); }
		wxString GetTextValue() { return m_textCtrl->GetValue(); }
		void SetTextValue(const wxString& newText) { m_textCtrl->SetValue(newText); }
	private:
		wxCheckBox* m_checkbox;
		wxTextCtrl* m_textCtrl;
		std::vector<std::function<void(bool)>> m_checkedChanged;
		std::vector<std::function<void(wxString&)>> m_textChanged;

		void RenewTextCtrlEnabled();
		void SubscribeToEvents();
	};
}