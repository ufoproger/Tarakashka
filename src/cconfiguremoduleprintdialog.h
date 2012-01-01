#pragma once

#include "Poco/Foundation.h"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>

#include "mytypes.h"

class CConfigureModulePrintDialog : public Gtk::Dialog
{
	private:
		Gtk::Button		buttonFilename;
		Gtk::Label		labelFilename;
		Gtk::HBox		boxFilename;
		Gtk::VBox 		box;
		
		std::string filename;

	private:
		void buttonFilename_clicked ()
		{
			Gtk::FileChooserDialog dialog("Выберите файл с шаблоном диалога");
			
			dialog.add_button("Выбрать", Gtk::RESPONSE_OK);
			dialog.add_button("Отмена", Gtk::RESPONSE_CANCEL);
			
			Glib::RefPtr < Gtk::FileFilter > refFilter = Gtk::FileFilter::create();
			
			refFilter->set_name("Таблицы ODS");
			refFilter->add_mime_type("application/vnd.oasis.opendocument.spreadsheet");
			dialog.add_filter(refFilter);
			
			if (dialog.run() == Gtk::RESPONSE_OK)
			{		
				filename = dialog.get_filename();
				labelFilename.set_text(Glib::ustring::compose("Файл шаблона: %1", Poco::Path(filename).getFileName()));
			}
		}
		
	public:
		CConfigureModulePrintDialog ():
			labelFilename("Имя файла с шаблоном: "),
			buttonFilename("Выбрать")
		{
			set_title("Параметры модуля");
			
			get_vbox()->pack_start(box);
			box.pack_start(boxFilename, Gtk::PACK_SHRINK);
			boxFilename.pack_start(labelFilename, Gtk::PACK_SHRINK);
			boxFilename.pack_start(buttonFilename, Gtk::PACK_SHRINK);

			box.set_border_width(10);
			box.set_spacing(5);
			boxFilename.set_spacing(5);
		
			buttonFilename.signal_clicked().connect(sigc::mem_fun(*this, &CConfigureModulePrintDialog::buttonFilename_clicked));

			add_button("Готово", Gtk::RESPONSE_OK);
			add_button("Отменить", Gtk::RESPONSE_CANCEL);		

			show_all_children();
		}
		
		~CConfigureModulePrintDialog ()
		{
		}

		void on_response (int responseID)
		{
			set_data("filename", &filename);
			hide();
		}
};
