#pragma once

#include "Poco/Foundation.h"

#include <string>
#include <vector>
#include <map>

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/grid.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/menu.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/menushell.h>

#include "mytypes.h"

class CConfigureModulePrintTableDialog : public Gtk::Dialog
{
	private:
		Gtk::ScrolledWindow scrolledWindow;
		Gtk::Button		buttonFilename;
		Gtk::Label		labelFilename;
		Gtk::HBox		boxFilename;
		Gtk::VBox 		box;
		Gtk::Grid		grid;
		Gtk::TreeView	treeView;
		Gtk::Menu		menu;		
		Gtk::MenuItem 	menuItemAdd;
		Gtk::MenuItem 	menuItemDelete;
	
		Glib::RefPtr < Gtk::ListStore > refListStore;
		
		CModelColumnsForProperty tableColumns;
		
		std::map < std::string , std::string > extraReplaces;
		std::vector < Gtk::Entry* > Entries;
		std::vector < Gtk::Label* > labels;
		
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

		bool treeView_button_press_event (GdkEventButton* event)
		{
			if (event->type == GDK_BUTTON_PRESS && event->button == 3)
			{
				menu.popup(event->button, event->time);
				return true;
			}
			
			return false;
		}
		
		bool menuItemAdd_button_press_event (GdkEventButton* event)
		{
			if (event->type == GDK_BUTTON_PRESS && event->button == 1)
			{
				Gtk::TreeModel::Row row = *(refListStore->append());
				
				row[tableColumns.label] = "####";
				return true;
			}
			
			return false;
		}		
		
		bool menuItemDelete_button_press_event (GdkEventButton* event)
		{
			if (event->type == GDK_BUTTON_PRESS && event->button == 1)
			{
				Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeView.get_selection();

				if (refSelection)
				{
					Gtk::TreeModel::iterator it = refSelection->get_selected();

					if	(it)
						refListStore->erase(it);
				}
				
				return true;
			}
			
			return false;
		}
		
	public:
		CConfigureModulePrintTableDialog ():
			labelFilename("Имя файла с шаблоном: "),
			buttonFilename("Выбрать"),
			menuItemAdd("Добавить"),
			menuItemDelete("Удалить")
		{
			refListStore = Gtk::ListStore::create(tableColumns);
			
			resize(400, 500);
			set_title("Параметры модуля");
			
			get_vbox()->pack_start(box);
			box.pack_start(boxFilename, Gtk::PACK_SHRINK);
			box.pack_start(scrolledWindow);
			boxFilename.pack_start(labelFilename, Gtk::PACK_SHRINK);
			boxFilename.pack_start(buttonFilename, Gtk::PACK_SHRINK);
			scrolledWindow.add(treeView);
			
			box.set_border_width(10);
			box.set_spacing(5);
			boxFilename.set_spacing(5);
		
			treeView.set_model(refListStore);
			treeView.property_enable_grid_lines().set_value(true);					
			
			treeView.append_column_editable("Свойство", tableColumns.label);
			treeView.append_column_editable("Значение", tableColumns.text);
			
			std::string items[] = {"##SUBJECT_NAME##", "##SUBJECT_FORMAT##", "##SUBJECT_DATE##"};
			
			for (int i = 0; i < 3; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStore->append());
				
				row[tableColumns.label] = items[i];
			}
			
			menu.add(menuItemAdd);
			menu.add(menuItemDelete);
			menu.show_all();
			
			add_button("Готово", Gtk::RESPONSE_OK);
			add_button("Отменить", Gtk::RESPONSE_CANCEL);		

			buttonFilename.signal_clicked().connect(sigc::mem_fun(*this, &CConfigureModulePrintTableDialog::buttonFilename_clicked));
			treeView.signal_button_press_event().connect(sigc::mem_fun(*this, &CConfigureModulePrintTableDialog::treeView_button_press_event), false);
			menuItemAdd.signal_button_press_event().connect(sigc::mem_fun(*this, &CConfigureModulePrintTableDialog::menuItemAdd_button_press_event), false);
			menuItemDelete.signal_button_press_event().connect(sigc::mem_fun(*this, &CConfigureModulePrintTableDialog::menuItemDelete_button_press_event), false);
			
			show_all_children();
		}
		
		~CConfigureModulePrintTableDialog ()
		{
		}

		void on_response (int responseID)
		{
			set_data("filename", &filename);

			for(Gtk::TreeModel::Children::iterator it = refListStore->children().begin(); it != refListStore->children().end(); ++it)
			{
				Gtk::TreeModel::Row row = *it;
				std::string label = row[tableColumns.label];
				std::string text = row[tableColumns.text];

				if (!text.empty() && !label.empty())
					extraReplaces[label] = text;
			}
			
			set_data("extra_replaces", &extraReplaces);
						
			hide();
		}
};
