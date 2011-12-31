#pragma once

#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>

#include "mytypes.h"

class CEditSchoolDialog : public Gtk::Dialog
{
	private:
		Gtk::Label		labelName;
		Gtk::Entry		entryName;
		Gtk::VBox 		box;
		Gtk::Label		labelCity;
		Gtk::Entry		entryCity;
		Gtk::HBox		boxName;
		Gtk::HBox		boxLongName;
		Gtk::HBox		boxFormat;
		Gtk::Label		labelLongName;
		Gtk::Label		labelShowFormat;
		Gtk::Entry		entryLongName;
		Gtk::Label		labelFormat;
		Gtk::Entry		entryFormat;
		Gtk::Label		labelFormatHelp;
				
		CSchoolData data;

	private:
		void entryFormat_changed ()
		{
			std::string s = entryFormat.get_text();
			
			my_replace(s, "$", "\n");
			
			if (s.empty())
				labelShowFormat.set_text("Пока формат отображения не задан!");
			else
				labelShowFormat.set_text(s);		
		}
		
	public:
		CEditSchoolDialog ():
			labelName("Школа: "),
			labelCity("Район: "),
			labelLongName("Расшифровка: "),
			labelFormatHelp("Символ \"$\" при печати означает перенос строки"),
			labelShowFormat("Тест")
		{
			set_title("Редактирование данных ученика");
			resize(700, get_height());
			
			get_vbox()->pack_start(box);
			box.pack_start(boxName, Gtk::PACK_SHRINK);
			box.pack_start(boxLongName, Gtk::PACK_SHRINK);
			box.pack_start(boxFormat, Gtk::PACK_SHRINK);
			box.pack_start(labelFormatHelp);
			boxName.pack_start(labelName, Gtk::PACK_SHRINK);
			boxName.pack_start(entryName);
			boxName.pack_start(labelCity, Gtk::PACK_SHRINK);
			boxName.pack_start(entryCity);
			boxLongName.pack_start(labelLongName, Gtk::PACK_SHRINK);
			boxLongName.pack_start(entryLongName);
			boxFormat.pack_start(labelFormat, Gtk::PACK_SHRINK);
			boxFormat.pack_start(entryFormat);
			box.pack_start(labelShowFormat);
			
			entryFormat.signal_changed().connect(sigc::mem_fun(*this, &CEditSchoolDialog::entryFormat_changed));
					
			box.set_border_width(10);
			box.set_spacing(5);
			boxName.set_spacing(5);
			boxLongName.set_spacing(5);
			boxFormat.set_spacing(5);
			
			add_button("Готово", Gtk::RESPONSE_OK);
			add_button("Отменить", Gtk::RESPONSE_CANCEL);		

			show_all_children();
		}
		
		~CEditSchoolDialog ()
		{
		}

		void on_response (int responseID)
		{
			data.name = entryName.get_text();
			data.longName = entryLongName.get_text();
			data.city = entryCity.get_text();
			data.format = entryFormat.get_text();
		}
		
		void set_data (CSchoolData _data)
		{
			data = _data;

			entryName.set_text(data.name);
			entryLongName.set_text(data.longName);
			entryCity.set_text(data.city);
			entryFormat.set_text(data.format);
			
			entryFormat_changed();
		}
		
		CSchoolData get_data ()
		{
			return data;
		}
				
};
