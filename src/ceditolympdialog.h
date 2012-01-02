#pragma once

#include <gtkmm/treemodel.h>
#include <gtkmm/dialog.h>
#include <gtkmm/combobox.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/liststore.h>

#include "Poco/Data/Common.h"

#include <algorithm>

#include "ceditschooldialog.h"
#include "ceditstudentdialog.h"
#include "ceditsubjectdialog.h"
#include "mytypes.h"

class CEditOlympDialog : public Gtk::Dialog
{
	private:
		Gtk::ComboBox		comboSubject;
		Gtk::ComboBox		comboStudent;
		Gtk::ComboBox		comboSchool;
		Gtk::Label			labelSubject;
		Gtk::Label			labelStudent;
		Gtk::Label			labelSchool;
		Gtk::HBox			boxSubject;
		Gtk::HBox			boxStudent;
		Gtk::HBox			boxSchool;
		Gtk::VBox 			box;
		Gtk::HBox			boxHere;
		Gtk::CheckButton	checkButtonHere;
		
		Glib::RefPtr < Gtk::ListStore > refListStoreSubjects;
		Glib::RefPtr < Gtk::ListStore > refListStoreStudents;

		CModelColumnsComboBox comboColumns;
		COlympData data;
		
		std::vector < CStudentData > students;
		std::vector < CSubjectData > subjects;
		std::vector < CSchoolData > schools;

	private:
		static bool compare_students (const CStudentData &a, const CStudentData &b)
		{
			std::string strA = Glib::ustring::compose("%3 %1 %2 (%4 класс)", a.name, a.middle, a.surname, a.level);
			std::string strB = Glib::ustring::compose("%3 %1 %2 (%4 класс)", b.name, b.middle, b.surname, b.level);
			
			return (strA < strB);
		}
		
		static bool compare_subjects (const CSubjectData &a, const CSubjectData &b)
		{
			if (a.year == b.year)
			{
				if (a.month == b.month)
				{
					if (a.day == b.day)
					{
						return (a.name < b.name);
					}
					else
						return (a.day < b.day);
				}
				else
					return (a.month < b.month);
			}
			else
				return (a.year < b.year);
		}
		
		void comboStudent_changed ()
		{
			int student = (*comboStudent.get_active())[comboColumns.id];
			int school = -1;
			
			for (std::vector < CStudentData >::const_iterator it = students.begin(); it != students.end(); ++it)
				if (it->id == student)
				{
					school = it->schoolID;
					break;
				}
				
			for (std::vector < CSchoolData >::const_iterator it = schools.begin(); it != schools.end(); ++it)
				if (it->id == school)
				{
					labelSchool.set_text(Glib::ustring::compose("%1 (%2)", it->name, it->city));
					break;
				}
		}
		
		void on_response (int responseID)
		{
			data.student = (*comboStudent.get_active())[comboColumns.id];
			data.subject = (*comboSubject.get_active())[comboColumns.id];
			data.here = (int)checkButtonHere.get_active();
		}

	public:
		CEditOlympDialog ():
			labelSubject("Предмет: "),
			labelStudent("Ученик: "),
			labelSchool("Выберите ученика"),
			checkButtonHere("Явился на олимпиаду")
		{
			set_title("Редактирование данных олимпиады");
			
			refListStoreSubjects = Gtk::ListStore::create(comboColumns);
			refListStoreStudents = Gtk::ListStore::create(comboColumns);
			
			get_vbox()->pack_start(box);
			box.pack_start(boxSubject, Gtk::PACK_SHRINK);
			box.pack_start(boxStudent, Gtk::PACK_SHRINK);
			box.pack_start(boxSchool, Gtk::PACK_SHRINK);
			box.pack_start(boxHere, Gtk::PACK_SHRINK);
			boxSubject.pack_start(labelSubject, Gtk::PACK_SHRINK);
			boxSubject.pack_start(comboSubject);
			boxStudent.pack_start(labelStudent, Gtk::PACK_SHRINK);
			boxStudent.pack_start(comboStudent);
			boxSchool.pack_start(labelSchool);
			boxHere.pack_start(checkButtonHere);
			
			comboSubject.pack_start(comboColumns.text);
			comboStudent.pack_start(comboColumns.text);
			
			box.set_border_width(10);
			box.set_spacing(5);
			boxSubject.set_spacing(5);
			boxStudent.set_spacing(5);
			boxSchool.set_spacing(5);
			boxHere.set_spacing(5);
			
			comboSubject.set_model(refListStoreSubjects);
			comboStudent.set_model(refListStoreStudents);

			add_button("Готово", Gtk::RESPONSE_OK);
			add_button("Отменить", Gtk::RESPONSE_CANCEL);

			comboStudent.signal_changed().connect(sigc::mem_fun(*this, &CEditOlympDialog::comboStudent_changed));

			show_all_children();
		}
		
		~CEditOlympDialog ()
		{
		}
		
		void set_data (COlympData _data)
		{
			data = _data;
			
			checkButtonHere.set_active((bool)data.here);
			
			for (int i = 0; i < students.size(); ++i)
				if (students[i].id == data.student)
				{
					comboStudent.set_active(i);
					break;
				}
				
			for (int i = 0; i < subjects.size(); ++i)
				if (subjects[i].id == data.subject)
				{
					comboSubject.set_active(i);
					break;
				}
				
			comboStudent_changed();
		}

		void set_subjects (std::vector < CSubjectData > _subjects)
		{
			subjects = _subjects;
			refListStoreSubjects->clear();
			
			std::sort(subjects.begin(), subjects.end(), compare_subjects);

			for (std::vector < CSubjectData >::const_iterator it = subjects.begin(); it != subjects.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreSubjects->append());
				
				row[comboColumns.id] = it->id;
				row[comboColumns.text] = Glib::ustring::compose("%1 (%2)", it->name, print_date_short(it->day, it->month, it->year));
			}			
		}

		void set_students (std::vector < CStudentData > _students)
		{
			students = _students;
			refListStoreStudents->clear();
			
			std::sort(students.begin(), students.end(), compare_students);
			
			for (std::vector < CStudentData >::const_iterator it = students.begin(); it != students.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreStudents->append());
				
				row[comboColumns.id] = it->id;
				row[comboColumns.text] = Glib::ustring::compose("%3 %1 %2 (%4 класс)", it->name, it->middle, it->surname, it->level);
			}			
		}
		
		void set_schools (std::vector < CSchoolData > _schools)
		{
			schools = _schools;
		}	
				
		COlympData get_data ()
		{
			return data;
		}
};
