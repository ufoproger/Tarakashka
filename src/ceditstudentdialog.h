#pragma once

#include <gtkmm.h>

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

class CStudentData
{
	public:
		int id;
		std::string name, middle, surname;
		int birthday, birthmonth, birthyear;
		int level;
		int schoolID;
		
	public:
		CStudentData (int _id, std::string _name, std::string _middle, std::string _surname, int _level, int _schoolID, int _birthday, int _birthmonth, int _birthyear)
		{
			id = _id;
			name = _name;
			middle = _middle;
			surname = _surname;
			level = _level;
			schoolID = _schoolID;
			birthday = _birthday;
			birthmonth = _birthmonth;
			birthyear = _birthyear;
		}
		
		CStudentData (const CStudentData &copy)
		{
			id = copy.id;
			name = copy.name;
			middle = copy.middle;
			surname = copy.surname;
			level = copy.level;
			schoolID = copy.schoolID;
			birthday = copy.birthday;
			birthmonth = copy.birthmonth;
			birthyear = copy.birthyear;		
		}
		
		CStudentData ()
		{
			id = -1;
			birthday = -1;
			birthmonth = -1;
			birthyear = -1;
			schoolID = -1;
			level = -1;
		}
		
		bool is_empty ()
		{
			return (id == -1);
		}
		
		bool is_valid ()
		{
			return !(name.empty() || middle.empty() || surname.empty() || birthday == -1 || birthmonth == -1 || birthyear == -1 || level == -1 || schoolID == -1);
		}
};

class CEditStudentDialog : public Gtk::Dialog
{
	private:
		Gtk::Button button1, button2;
		Gtk::VBox box;
		Gtk::Label labelName, labelBirthday, labelSchool, labelLevel;
		Gtk::HBox boxName, boxBirthday, boxSchool;
		Gtk::Entry entryName, entryMiddle, entrySurname;
		Gtk::ComboBox comboDay, comboMonth, comboYear, comboSchool, comboLevel;
		Glib::RefPtr < Gtk::ListStore > refListStoreDays, refListStoreMonths, refListStoreYears, refListStoreSchools, refListStoreLevels;
		CModelColumnsComboBox comboColumns;
	
	private:	
		std::vector < Poco::Tuple < int , std::string , std::string > > schools;
		CStudentData data;

	public:
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
				row[comboColumns.text] = Glib::ustring::compose("%1, (%2)", it->get<1>(), it->get<2>());
			}
			
			std::cout << "schools: " << schools.size() << std::endl;
		}
		
		void set_data (CStudentData _data)
		{
			data = _data;

			comboDay.set_active(data.birthday - 1);
			comboMonth.set_active(data.birthmonth - 1);
			comboYear.set_active(data.birthyear - 1990);
			comboLevel.set_active(data.level - 9);
			
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
		
		CEditStudentDialog ():
			button1("Hello"),
			button2("Oh no!"),
			labelName("ФИО: "),
			labelBirthday("Дата рождения: "),
			labelSchool("Школа, класс: "),
			labelLevel("Класс: ")
		{
			set_title("Редактирование данных ученика");
			
			refListStoreDays = Gtk::ListStore::create(comboColumns);
			refListStoreMonths = Gtk::ListStore::create(comboColumns);
			refListStoreYears = Gtk::ListStore::create(comboColumns);
			refListStoreSchools = Gtk::ListStore::create(comboColumns);
			refListStoreLevels = Gtk::ListStore::create(comboColumns);
			
			get_vbox()->pack_start(box);

			box.set_border_width(10);

			box.pack_start(boxName, Gtk::PACK_SHRINK);
			
			box.set_spacing(5);
			boxName.set_spacing(5);
			boxBirthday.set_spacing(5);
			boxSchool.set_spacing(5);
			
			boxName.pack_start(labelName, Gtk::PACK_SHRINK);
			boxName.pack_start(entrySurname);
			boxName.pack_start(entryName);
			boxName.pack_start(entryMiddle);
			
			box.pack_start(boxBirthday, Gtk::PACK_SHRINK);
	
			boxBirthday.pack_start(labelBirthday, Gtk::PACK_SHRINK);
			boxBirthday.pack_start(comboDay);
			boxBirthday.pack_start(comboMonth);
			boxBirthday.pack_start(comboYear);
			
			box.pack_start(boxSchool, Gtk::PACK_SHRINK);
			
			boxSchool.pack_start(labelSchool, Gtk::PACK_SHRINK);
			boxSchool.pack_start(comboSchool);
			boxSchool.pack_start(comboLevel);
			
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
			
			for (int i = 9; i <= 11; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreLevels->append());
				
				row[comboColumns.id] = i;
				row[comboColumns.text] = Glib::ustring::compose("%1 класс", i);
			}
			
			Glib::ustring monthName[12] = {"января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
			
			for (int i = 0; i < 12; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreMonths->append());
				
				row[comboColumns.id] = i + 1;
				row[comboColumns.text] = monthName[i];
			}

			for (int i = 1990; i <= 1996; ++i)
			{
				Gtk::TreeModel::Row row = *(refListStoreYears->append());
				
				row[comboColumns.id] = i;
				row[comboColumns.text] = Glib::ustring::compose("%1", i);
			}
			
			
			comboDay.pack_start(comboColumns.text);
			comboMonth.pack_start(comboColumns.text);
			comboYear.pack_start(comboColumns.text);
			comboSchool.pack_start(comboColumns.id);
			comboSchool.pack_start(comboColumns.text);
			comboLevel.pack_start(comboColumns.text);
			
			add_button("Готово", Gtk::RESPONSE_OK);
			add_button("Отменить", Gtk::RESPONSE_CANCEL);		

			show_all_children();
		}
		
		~CEditStudentDialog ()
		{
		}
};
