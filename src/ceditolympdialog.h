#pragma once

#include <gtkmm/treemodel.h>
#include <gtkmm/dialog.h>
#include <gtkmm/combobox.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>

#include "Poco/Data/Common.h"

#include "ceditschooldialog.h"
#include "ceditstudentdialog.h"
#include "ceditsubjectdialog.h"
#include "mytypes.h"

class CEditOlympDialog : public Gtk::Dialog
{
	private:
		Gtk::ComboBox	comboSubject;
		Gtk::ComboBox	comboStudent;
		Gtk::ComboBox	comboSchool;
		Gtk::Label		labelSubject;
		Gtk::Label		labelStudent;
		Gtk::Label		labelSchool;
		Gtk::HBox		boxSubject;
		Gtk::HBox		boxStudent;
		Gtk::HBox		boxSchool;
		Gtk::VBox 		box;
		
		Glib::RefPtr < Gtk::ListStore > refListStoreSubjects;
		Glib::RefPtr < Gtk::ListStore > refListStoreSchools;
		Glib::RefPtr < Gtk::ListStore > refListStoreStudents;

		CModelColumnsComboBox comboColumns;
		COlympData data;

	public:
		CEditOlympDialog ():
			labelSubject("Предмет: "),
			labelStudent("Ученик: "),
			labelSchool("Школа: ")
		{
			set_title("Редактирование данных олимпиады");
			
			refListStoreSubjects = Gtk::ListStore::create(comboColumns);
			refListStoreStudents = Gtk::ListStore::create(comboColumns);
			refListStoreSchools = Gtk::ListStore::create(comboColumns);
			
			get_vbox()->pack_start(box);
			box.pack_start(boxSubject, Gtk::PACK_SHRINK);
			box.pack_start(boxStudent, Gtk::PACK_SHRINK);
			box.pack_start(boxSchool, Gtk::PACK_SHRINK);
			boxSubject.pack_start(labelSubject, Gtk::PACK_SHRINK);
			boxSubject.pack_start(comboSubject);
			boxStudent.pack_start(labelStudent, Gtk::PACK_SHRINK);
			boxStudent.pack_start(comboStudent);
			boxSchool.pack_start(labelSchool, Gtk::PACK_SHRINK);
			boxSchool.pack_start(comboSchool);

			comboSubject.pack_start(comboColumns.text);
			comboStudent.pack_start(comboColumns.text);
			comboSchool.pack_start(comboColumns.text);
			
			box.set_border_width(10);
			box.set_spacing(5);
			boxSubject.set_spacing(5);
			boxStudent.set_spacing(5);
			boxSchool.set_spacing(5);
			
			comboSubject.set_model(refListStoreSubjects);
			comboStudent.set_model(refListStoreStudents);
			comboSchool.set_model(refListStoreSchools);

			add_button("Готово", Gtk::RESPONSE_OK);
			add_button("Отменить", Gtk::RESPONSE_CANCEL);

			show_all_children();
		}
		
		~CEditOlympDialog ()
		{
		}

		void on_response (int responseID)
		{
			data.student = (*comboStudent.get_active())[comboColumns.id];
			data.subject = (*comboSubject.get_active())[comboColumns.id];
			data.school = (*comboSchool.get_active())[comboColumns.id];
		}
		
		void set_data (COlympData _data)
		{
			data = _data;
		}
	

		void set_subjects (std::vector < CSubjectData > subjects)
		{
			refListStoreSubjects->clear();
			
			for (std::vector < CSubjectData >::const_iterator it = subjects.begin(); it != subjects.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreSubjects->append());
				
				row[comboColumns.text] = Glib::ustring::compose("%1 (%2.%3.%4)", it->name, it->day, it->month, it->year);
			}			
		}

		void set_students (std::vector < CStudentData > students)
		{
			refListStoreStudents->clear();
			
			for (std::vector < CStudentData >::const_iterator it = students.begin(); it != students.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreStudents->append());
				
				row[comboColumns.text] = Glib::ustring::compose("%3 %1 %2 (%4 класс)", it->name, it->middle, it->surname, it->level);
			}			
		}

		void set_schools (std::vector < CSchoolData > schools)
		{
			refListStoreSchools->clear();
			
			for (std::vector < CSchoolData >::const_iterator it = schools.begin(); it != schools.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreSchools->append());
				
				row[comboColumns.id] = it->id;
				row[comboColumns.text] = Glib::ustring::compose("%1 (%2)", it->name, it->city);
			}			
		}
				
		COlympData get_data ()
		{
			return data;
		}
};
