#pragma once

#include <gtkmm/treemodel.h>
#include <gtkmm/dialog.h>
#include <gtkmm/combobox.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>

#include "Poco/Data/Common.h"

#include "mytypes.h"

class CEditStudentDialog : public Gtk::Dialog
{
	private:
		static const int MIN_LEVEL = 9;
		static const int MIN_YEAR = 1990;
		static const int MAX_YEAR = 1999;
		
		Gtk::ComboBox	comboDay;
		Gtk::ComboBox	comboMonth;
		Gtk::ComboBox	comboYear;
		Gtk::ComboBox	comboSchool;
		Gtk::ComboBox	comboLevel;
		Gtk::Label		labelName;
		Gtk::Label		labelBirthday;
		Gtk::Label		labelSchool;
		Gtk::Entry		entryName;
		Gtk::Entry		entryMiddle;
		Gtk::Entry		entrySurname;
		Gtk::HBox		boxName;
		Gtk::HBox		boxBirthday;
		Gtk::HBox		boxSchool;
		Gtk::VBox 		box;
		
		Glib::RefPtr < Gtk::ListStore > refListStoreDays, refListStoreMonths, refListStoreYears, refListStoreSchools, refListStoreLevels;
		
		std::vector < Poco::Tuple < int , std::string , std::string > > schools;

		CModelColumnsComboBox comboColumns;
		CStudentData data;

	public:
		CEditStudentDialog ():
			labelBirthday("Дата рождения: "),
			labelSchool("Школа, класс: "),
			labelName("ФИО: ")
		{
			set_title("Редактирование данных ученика");
			
			refListStoreDays = Gtk::ListStore::create(comboColumns);
			refListStoreMonths = Gtk::ListStore::create(comboColumns);
			refListStoreYears = Gtk::ListStore::create(comboColumns);
			refListStoreSchools = Gtk::ListStore::create(comboColumns);
			refListStoreLevels = Gtk::ListStore::create(comboColumns);
			
			get_vbox()->pack_start(box);
			box.pack_start(boxName, Gtk::PACK_SHRINK);
			box.pack_start(boxBirthday, Gtk::PACK_SHRINK);
			box.pack_start(boxSchool, Gtk::PACK_SHRINK);
			boxName.pack_start(labelName, Gtk::PACK_SHRINK);
			boxName.pack_start(entrySurname);
			boxName.pack_start(entryName);
			boxName.pack_start(entryMiddle);
			boxBirthday.pack_start(labelBirthday, Gtk::PACK_SHRINK);
			boxBirthday.pack_start(comboDay);
			boxBirthday.pack_start(comboMonth);
			boxBirthday.pack_start(comboYear);
			boxSchool.pack_start(labelSchool, Gtk::PACK_SHRINK);
			boxSchool.pack_start(comboSchool);
			boxSchool.pack_start(comboLevel);

			comboDay.pack_start(comboColumns.text);
			comboMonth.pack_start(comboColumns.text);
			comboYear.pack_start(comboColumns.text);
			comboSchool.pack_start(comboColumns.text);
			comboLevel.pack_start(comboColumns.text);

			box.set_border_width(10);
			box.set_spacing(5);
			boxName.set_spacing(5);
			boxBirthday.set_spacing(5);
			boxSchool.set_spacing(5);
			
			comboDay.set_model(refListStoreDays);
			comboMonth.set_model(refListStoreMonths);
			comboYear.set_model(refListStoreYears);
			comboSchool.set_model(refListStoreSchools);
			comboLevel.set_model(refListStoreLevels);
		
			for (int i = 1; i <= 31; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreDays->append());
				
				row[comboColumns.id] = i;
				row[comboColumns.text] = Glib::ustring::compose("%1", i);
			}
			
			for (int i = MIN_LEVEL; i <= 11; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreLevels->append());
				
				row[comboColumns.id] = i;
				row[comboColumns.text] = Glib::ustring::compose("%1 класс", i);
			}
			
			for (int i = 0; i < 12; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreMonths->append());
				
				row[comboColumns.id] = i + 1;
				row[comboColumns.text] = get_month(i + 1);
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
		
		~CEditStudentDialog ()
		{
		}

		void on_response (int responseID)
		{
			data.name = entryName.get_text();
			data.middle = entryMiddle.get_text();
			data.surname = entrySurname.get_text();
			data.birthday = (*comboDay.get_active())[comboColumns.id];
			data.birthmonth = (*comboMonth.get_active())[comboColumns.id];
			data.birthyear = (*comboYear.get_active())[comboColumns.id];
			data.schoolID = (*comboSchool.get_active())[comboColumns.id];
			data.level = (*comboLevel.get_active())[comboColumns.id];
		}
		
		void set_schools (std::vector < Poco::Tuple < int , std::string , std::string > > _schools)
		{
			schools = _schools;
			
			sort(schools.begin(), schools.end(), compare);

			refListStoreSchools->clear();

			for (std::vector < Poco::Tuple < int , std::string , std::string > >::iterator it = schools.begin(); it != schools.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreSchools->append());
				
				row[comboColumns.id] = it->get<0>();
				row[comboColumns.text] = Glib::ustring::compose("%1 (%2)", it->get<1>(), it->get<2>());
			}
		}
		
		void set_data (CStudentData _data)
		{
			data = _data;

			comboDay.set_active(data.birthday - 1);
			comboMonth.set_active(data.birthmonth - 1);
			comboYear.set_active(data.birthyear - MIN_YEAR);
			comboLevel.set_active(data.level - MIN_LEVEL);
			entryName.set_text(data.name);
			entryMiddle.set_text(data.middle);
			entrySurname.set_text(data.surname);
			
			for (int i = 0; i < schools.size(); ++i)
				if (schools[i].get<0>() == data.schoolID)
				{
					comboSchool.set_active(i);
					break;
				}
		}
		
		CStudentData get_data ()
		{
			return data;
		}
		
		static bool compare (const Poco::Tuple < int , std::string , std::string > &a, const Poco::Tuple < int , std::string , std::string > &b)
		{
			return (a.get<2>() == b.get<2>()) ? (a.get<1>() < b.get<1>()) : (a.get<2>() < b.get<2>());
		}
		
};
