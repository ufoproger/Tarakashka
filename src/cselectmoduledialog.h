#pragma once

#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/radiobutton.h>

#include <vector>

#include "cmodulebase.h"
#include "mytypes.h"

class CSelectModuleDialog : public Gtk::Dialog
{
	private:
		Gtk::Label		labelTitle;
		Gtk::VBox 		box;
		Gtk::RadioButtonGroup group;

		std::vector < Gtk::RadioButton* > radioButtons;				
		std::vector < CModuleBase* > modules;
		
	public:
		CSelectModuleDialog ():
			labelTitle("Выберите требуемый модуль:")
		{
			set_title("Выбор модуля");
			
			get_vbox()->pack_start(box);			
			box.pack_start(labelTitle);

			box.set_border_width(10);
			box.set_spacing(5);
			
			add_button("Готово", Gtk::RESPONSE_OK);
			add_button("Отменить", Gtk::RESPONSE_CANCEL);		

			show_all_children();
		}
		
		~CSelectModuleDialog ()
		{
		}
		
		void set_data (const std::vector < CModuleBase* > &_modules)
		{
			modules = _modules;

			for (int i = 0; i < modules.size(); ++i)
			{
				radioButtons.push_back(Gtk::manage(new Gtk::RadioButton(group, modules[i]->get_name())));
				box.pack_start(*radioButtons[i]);
			}
			
			show_all_children();
		}
		
		int get_data ()
		{
			for (int i = 0; i < modules.size(); ++i)
				if (radioButtons[i]->get_active())
					return i;
					
			return 0;
		}
};
