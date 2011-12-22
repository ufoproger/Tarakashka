#pragma once

#include <gtkmm/treemodel.h>
#include <gtkmm/dialog.h>
#include <gtkmm/combobox.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>

#include "Poco/Data/Common.h"
/*
class CModelColumnsComboBox : public Gtk::TreeModel::ColumnRecord
{
	public:
		Gtk::TreeModelColumn < int > id;
		Gtk::TreeModelColumn < Glib::ustring > text;
		
		CModelColumnsComboBox ()
		{
			add(id);
			add(text);
		}
};
*/
class CSubjectData
{
	public:
		std::string name;
		std::string format;
		int id;
		int day;
		int month;
		int year;
		
	public:
		CSubjectData (int _id, std::string _name, std::string _format, int _day, int _month, int _year)
		{
			id = _id;
			name = _name;
			format = _format;
			day = _day;
			month = _month;
			year = _year;
		}
		
		CSubjectData (const CSubjectData& copy)
		{
			id = copy.id;
			name = copy.name;
			format = copy.format;
			day = copy.day;
			month = copy.month;
			year = copy.year;
		}
		
		CSubjectData ()
		{
			id = -1;
			day = -1;
			month = -1;
			year = -1;
		}
				
		bool is_empty ()
		{
			return (id == -1);
		}
		
		bool is_valid ()
		{
			return !(name.empty() || day == -1 || month == -1 || year == -1);
		}
};

class CEditSubjectDialog : public Gtk::Dialog
{
	private:
		static const int MIN_YEAR = 2010;
		static const int MAX_YEAR = 2012;
		
		Gtk::ComboBox	comboDay;
		Gtk::ComboBox	comboMonth;
		Gtk::ComboBox	comboYear;
		Gtk::Label		labelName;
		Gtk::Label		labelFormat;
		Gtk::Label		labelDate;
		Gtk::Entry		entryName;
		Gtk::Entry		entryFormat;
		Gtk::HBox		boxName;
		Gtk::HBox		boxFormat;
		Gtk::HBox		boxDate;
		Gtk::VBox 		box;
		
		Glib::RefPtr < Gtk::ListStore > refListStoreDays, refListStoreMonths, refListStoreYears;

		CModelColumnsComboBox comboColumns;
		CSubjectData data;

	public:
		CEditSubjectDialog ():
			labelName("Предмет: "),
			labelFormat("Формат: "),
			labelDate("Дата проведения: ")
		{
			set_title("Редактирование данных предмета");
			
			refListStoreDays = Gtk::ListStore::create(comboColumns);
			refListStoreMonths = Gtk::ListStore::create(comboColumns);
			refListStoreYears = Gtk::ListStore::create(comboColumns);
			
			get_vbox()->pack_start(box);
			box.pack_start(boxName, Gtk::PACK_SHRINK);
			box.pack_start(boxFormat, Gtk::PACK_SHRINK);
			box.pack_start(boxDate, Gtk::PACK_SHRINK);
			boxName.pack_start(labelName, Gtk::PACK_SHRINK);
			boxName.pack_start(entryName);
			boxName.pack_start(labelFormat, Gtk::PACK_SHRINK);
			boxName.pack_start(entryFormat);
			boxDate.pack_start(labelDate, Gtk::PACK_SHRINK);
			boxDate.pack_start(comboDay);
			boxDate.pack_start(comboMonth);
			boxDate.pack_start(comboYear);

			comboDay.pack_start(comboColumns.text);
			comboMonth.pack_start(comboColumns.text);
			comboYear.pack_start(comboColumns.text);

			box.set_border_width(10);
			box.set_spacing(5);
			boxName.set_spacing(5);
			boxDate.set_spacing(5);
			
			comboDay.set_model(refListStoreDays);
			comboMonth.set_model(refListStoreMonths);
			comboYear.set_model(refListStoreYears);
		
			for (int i = 1; i <= 31; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreDays->append());
				
				row[comboColumns.id] = i;
				row[comboColumns.text] = Glib::ustring::compose("%1", i);
			}
			
			Glib::ustring monthName[12] = {"января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
			
			for (int i = 0; i < 12; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreMonths->append());
				
				row[comboColumns.id] = i + 1;
				row[comboColumns.text] = monthName[i];
			}

			for (int i = MIN_YEAR; i <= MAX_YEAR; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreYears->append());
				
				row[comboColumns.id] = i;
				row[comboColumns.text] = Glib::ustring::compose("%1", i);
			}
			
			add_button("Готово", Gtk::RESPONSE_OK);
			add_button("Отменить", Gtk::RESPONSE_CANCEL);

			show_all_children();
		}
		
		~CEditSubjectDialog ()
		{
		}

		void on_response (int responseID)
		{
			data.name = entryName.get_text();
			data.format = entryFormat.get_text();
			data.day = (*comboDay.get_active())[comboColumns.id];
			data.month = (*comboMonth.get_active())[comboColumns.id];
			data.year = (*comboYear.get_active())[comboColumns.id];
		}
		
		void set_data (CSubjectData _data)
		{
			data = _data;

			comboDay.set_active(data.day - 1);
			comboMonth.set_active(data.month - 1);
			comboYear.set_active(data.year - MIN_YEAR);
			entryName.set_text(data.name);
			entryFormat.set_text(data.format);
		}
		
		CSubjectData get_data ()
		{
			return data;
		}
};
