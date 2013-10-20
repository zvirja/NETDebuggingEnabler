#include "baseset.h"
#include "CheckTextBoxesBundle.h"


namespace UI
{
	CheckTextBoxesBundle::CheckTextBoxesBundle(wxCheckBox* checkbox, wxTextCtrl* textCtrl):
		CheckTextBoxesBundle()
	{
		m_checkbox = checkbox;
		m_textCtrl = textCtrl;
		RenewTextCtrlEnabled();
		SubscribeToEvents();
	}


	CheckTextBoxesBundle::~CheckTextBoxesBundle()
	{
	}

	void CheckTextBoxesBundle::RenewTextCtrlEnabled()
	{
		m_textCtrl->Enable(m_checkbox->IsChecked());
	}

	void CheckTextBoxesBundle::SubscribeToEvents()
	{
		auto ffff = this;
		auto ref2 = this->m_textCtrl;
		m_checkbox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			RenewTextCtrlEnabled();
			bool isChecked = m_checkbox->GetValue();
			for (auto& handler : m_checkedChanged)
				handler(isChecked);
			event.Skip();
		});

		m_textCtrl->Bind(wxEVT_TEXT, [&](wxCommandEvent& event)
		{
			wxString textValue = m_textCtrl->GetValue();
			for (auto& handler : m_textChanged)
				handler(textValue);
			event.Skip();
		});
	}

}