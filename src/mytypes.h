#pragma once

#include <string>

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

class CModelColumnsOnlyID : public Gtk::TreeModel::ColumnRecord
{
	public:
		Gtk::TreeModelColumn < int > id;
		
		CModelColumnsOnlyID ()
		{
			add(id);
		}
};

class CStudentData
{
	public:
		std::string name;
		std::string middle;
		std::string surname;
		int id;
		int birthday;
		int birthmonth;
		int birthyear;
		int schoolID;
		int level;
		
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
		
		CStudentData (const CStudentData& copy)
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

class COlympData
{
	public:
		int id;
		int subject;
		int school;
		int student;
		
	public:
		COlympData (int _id, int _subject, int _student, int _school)
		{
			id = _id;
			subject = _subject;
			student = _student;
			school = _school;
		}
		
		COlympData (const COlympData& copy)
		{
			id = copy.id;
			subject = copy.subject;
			student = copy.student;
			school = copy.school;
		}
		
		COlympData ()
		{
			id = -1;
			subject = -1;
			school = -1;
			student = -1;		
		}
				
		bool is_empty ()
		{
			return (id == -1);
		}
		
		bool is_valid ()
		{
			return !(subject == -1 || school == -1 || student == -1);
		}
};

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

class CSchoolData
{
	public:
		std::string name;
		std::string longName;
		std::string format;
		std::string city;
		int id;
		
	public:
		CSchoolData (int _id, std::string _name, std::string _longName, std::string _format, std::string _city)
		{
			id = _id;
			name = _name;
			longName = _longName;
			format = _format;
			city = _city;
		}
		
		CSchoolData (const CSchoolData& copy)
		{
			id = copy.id;
			name = copy.name;	
			longName = copy.longName;
			format = copy.format;
			city = copy.city;
		}
		
		CSchoolData ()
		{
			id = -1;
		}
		
		bool is_empty ()
		{
			return (id == -1);
		}
		
		bool is_valid ()
		{
			return !(name.empty() || longName.empty() || format.empty() || city.empty());
		}
};


