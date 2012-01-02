#pragma once

#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Common.h"

#include <gtkmm/messagedialog.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/notebook.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>

#include <utility>
#include <vector>

#include "cselectmoduledialog.h"
#include "ceditstudentdialog.h"
#include "ceditschooldialog.h"
#include "ceditsubjectdialog.h"
#include "ceditolympdialog.h"
#include "cmoduleprinttable.h"
#include "cmoduleprint.h"
#include "cmodulebase.h"
#include "mytypes.h"

using Poco::Data::into;
using Poco::Data::use;
using Poco::Data::now;

class CMainWindow : public Gtk::Window
{
	private:
		Gtk::ScrolledWindow		scrolledWindowStudents;
		Gtk::Button 			buttonNewStudent;
		Gtk::Button				buttonDeleteStudent;
		Gtk::Button				buttonEditStudent;
		Gtk::HButtonBox			buttonBoxStudents;
		Gtk::Entry				entrySearchStudents;
		Gtk::Label				labelSearchStudents;
		Gtk::TreeView			treeViewStudents;
		Gtk::VBox				box;
		Gtk::VBox				boxStudents;
		Gtk::HBox				boxSearchStudents;
		Gtk::Notebook			notebook;
		Gtk::ScrolledWindow		scrolledWindowSchools;
		Gtk::VBox				boxSchools;
		Gtk::HBox				boxSearchSchools;
		Gtk::Label				labelSearchSchools;
		Gtk::Entry				entrySearchSchools;
		Gtk::TreeView			treeViewSchools;
		Gtk::HButtonBox			buttonBoxSchools;
		Gtk::Button				buttonDeleteSchool;
		Gtk::Button				buttonEditSchool;
		Gtk::Button				buttonNewSchool;
		Gtk::VBox				boxSubjects;
		Gtk::HButtonBox			buttonBoxSubjects;
		Gtk::Button				buttonDeleteSubject;
		Gtk::Button				buttonEditSubject;
		Gtk::Button				buttonNewSubject;
		Gtk::ScrolledWindow		scrolledWindowSubjects;
		Gtk::TreeView			treeViewSubjects;
		Gtk::Button				buttonDeleteOlymp;
		Gtk::Button				buttonNewOlymp;
		Gtk::Button				buttonEditOlymp;
		Gtk::HButtonBox			buttonBoxOlymps;
		Gtk::VBox				boxOlymps;
		Gtk::HBox				boxSearchOlymps;
		Gtk::ScrolledWindow		scrolledWindowOlymps;
		Gtk::Label				labelSearchOlymps;
		Gtk::Entry				entrySearchOlymps;
		Gtk::TreeView			treeViewOlymps;
		Gtk::Label				labelSelectedOlympsCount;
		Gtk::Button				buttonUnselectOlymps;
		Gtk::Button				buttonDoSelectedOlymps;
		
		Poco::Data::Session* dbSession;

		Glib::RefPtr < Gtk::ListStore > refListStoreStudents;
		Glib::RefPtr < Gtk::ListStore > refListStoreSchools;
		Glib::RefPtr < Gtk::ListStore > refListStoreSubjects;
		Glib::RefPtr < Gtk::ListStore > refListStoreOlymps;
		
		std::vector < CModuleBase* > modules;
		std::vector < int > selectedOlympsID;
		std::vector < int > studentsID;
		std::vector < int > subjectsID;
		std::vector < int > schoolsID;
		std::vector < int > olympsID;
		
		CModelColumnsOnlyID tableColumns;

	public:
		CMainWindow ():
			buttonNewStudent("Добавить"),
			buttonEditStudent("Редактировать"),
			buttonDeleteStudent("Удалить"),
			buttonNewSchool("Добавить"),
			buttonEditSchool("Редактировать"),
			buttonDeleteSchool("Удалить"),
			buttonNewSubject("Добавить"),
			buttonEditSubject("Редактировать"),
			buttonDeleteSubject("Удалить"),
			buttonNewOlymp("Добавить"),
			buttonEditOlymp("Редактировать"),
			buttonDeleteOlymp("Удалить"),
			buttonUnselectOlymps("Снять отметки"),
			buttonDoSelectedOlymps("Обработать"),
			labelSearchStudents("Критерий отсеивания: "),
			labelSearchSchools("Критерий отсеивания: "),
			labelSearchOlymps("Критерий отсеивания: "),
			buttonBoxStudents(Gtk::BUTTONBOX_END, 10),
			buttonBoxSchools(Gtk::BUTTONBOX_END, 10),
			buttonBoxOlymps(Gtk::BUTTONBOX_END, 10),
			buttonBoxSubjects(Gtk::BUTTONBOX_END, 10)
		{
			Poco::Data::SQLite::Connector::registerConnector();		
			dbSession = new Poco::Data::Session("SQLite", "db.sqlite");

			set_title("Таракашка");
			resize(800, 700);
			
			add(box);
			box.pack_start(notebook);

			initOlympsPage();
			initSubjectsPage();
			initStudentsPage();
			initSchoolsPage();

			modules.push_back(new CModulePrint());
			modules.push_back(new CModulePrintTable());
			
			show_all_children();
		}
		
		~CMainWindow ()
		{
			Poco::Data::SQLite::Connector::unregisterConnector();
		}
		
	private:
		void initOlympsPage ()
		{
			refListStoreOlymps = Gtk::ListStore::create(tableColumns);

			notebook.append_page(boxOlymps, "Олимпиады");
			boxOlymps.pack_start(boxSearchOlymps, Gtk::PACK_SHRINK);
			boxOlymps.pack_start(scrolledWindowOlymps);
//			boxOlymps.pack_start(labelSelectedOlympsCount, Gtk::PACK_SHRINK);
			boxOlymps.pack_start(buttonBoxOlymps, Gtk::PACK_SHRINK);
			buttonBoxOlymps.pack_start(buttonUnselectOlymps);
			buttonBoxOlymps.pack_start(buttonDoSelectedOlymps);
			buttonBoxOlymps.pack_start(buttonNewOlymp);
			buttonBoxOlymps.pack_start(buttonEditOlymp);
			buttonBoxOlymps.pack_start(buttonDeleteOlymp);
			boxSearchOlymps.pack_start(labelSearchOlymps, Gtk::PACK_SHRINK);
			boxSearchOlymps.pack_start(entrySearchOlymps);
			scrolledWindowOlymps.add(treeViewOlymps);			
			
			buttonBoxOlymps.set_border_width(10);
			boxSearchOlymps.set_border_width(5);

			treeViewOlymps.property_enable_grid_lines().set_value(true);
			treeViewOlymps.set_model(refListStoreOlymps);
			treeViewOlymps.set_events(Gdk::BUTTON_PRESS_MASK);

			treeViewOlymps.insert_column("ID", tableColumns.id, 0);
			treeViewOlymps.insert_column_with_data_func(-1, "", *Gtk::manage(new Gtk::CellRendererToggle), sigc::mem_fun(*this, &CMainWindow::treeViewOlymps_cell_renderer_use));						
			treeViewOlymps.insert_column_with_data_func(-1, "Предмет", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewOlymps_cell_renderer_subject));
			treeViewOlymps.insert_column_with_data_func(-1, "Ученик", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewOlymps_cell_renderer_student));
			treeViewOlymps.insert_column_with_data_func(-1, "Школа", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewOlymps_cell_renderer_school));
			treeViewOlymps.insert_column_with_data_func(-1, "Здесь", *Gtk::manage(new Gtk::CellRendererToggle), sigc::mem_fun(*this, &CMainWindow::treeViewOlymps_cell_renderer_here));
		
			for (int i = 2; i < 6; ++i)
				treeViewOlymps.get_column(i)->set_resizable();
		
			entrySearchOlymps.signal_changed().connect(sigc::mem_fun(*this, &CMainWindow::entrySearchOlymps_changed));


			treeViewOlymps.signal_row_activated().connect(sigc::mem_fun(*this, &CMainWindow::treeViewOlymps_row_activated));
			buttonEditOlymp.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonEditOlymp_clicked));
			buttonDeleteOlymp.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonDeleteOlymp_clicked));
			buttonNewOlymp.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonNewOlymp_clicked));
			buttonDoSelectedOlymps.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonDoSelectedOlymps_clicked));
			
			update_olympsID();			
		}
		
		void initStudentsPage ()
		{
			refListStoreStudents = Gtk::ListStore::create(tableColumns);

			notebook.append_page(boxStudents, "Ученики");
			boxStudents.pack_start(boxSearchStudents, Gtk::PACK_SHRINK);
			boxStudents.pack_start(scrolledWindowStudents);
			boxStudents.pack_start(buttonBoxStudents, Gtk::PACK_SHRINK);
			buttonBoxStudents.pack_start(buttonNewStudent);
			buttonBoxStudents.pack_start(buttonEditStudent);
			buttonBoxStudents.pack_start(buttonDeleteStudent);
			boxSearchStudents.pack_start(labelSearchStudents, Gtk::PACK_SHRINK);
			boxSearchStudents.pack_start(entrySearchStudents);
			scrolledWindowStudents.add(treeViewStudents);			
			
			buttonBoxStudents.set_border_width(10);
			boxSearchStudents.set_border_width(5);			

			treeViewStudents.property_enable_grid_lines().set_value(true);
			
			treeViewStudents.set_model(refListStoreStudents);
			treeViewStudents.set_events(Gdk::BUTTON_PRESS_MASK);

			treeViewStudents.insert_column("ID", tableColumns.id, 0);
			treeViewStudents.insert_column_with_data_func(-1, "ФИО", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewStudents_cell_renderer_name));
			treeViewStudents.insert_column_with_data_func(-1, "Город", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewStudents_cell_renderer_city));
			treeViewStudents.insert_column_with_data_func(-1, "Класс", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewStudents_cell_renderer_level));
			treeViewStudents.insert_column_with_data_func(-1, "Школа", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewStudents_cell_renderer_school));
			treeViewStudents.insert_column_with_data_func(-1, "Дата рождения", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewStudents_cell_renderer_birthday));
			
			for (int i = 1; i < 6; ++i)
				treeViewStudents.get_column(i)->set_resizable();
				
			treeViewStudents.signal_row_activated().connect(sigc::mem_fun(*this, &CMainWindow::treeViewStudents_row_activated));
			entrySearchStudents.signal_changed().connect(sigc::mem_fun(*this, &CMainWindow::entrySearchStudents_changed));
			buttonNewStudent.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonNewStudent_clicked));
			buttonEditStudent.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonEditStudent_clicked));
			buttonDeleteStudent.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonDeleteStudent_clicked));
				
			update_studentsID();
		}
	
		void initSchoolsPage ()
		{
			refListStoreSchools = Gtk::ListStore::create(tableColumns);

			notebook.append_page(boxSchools, "Школы");
			boxSchools.pack_start(boxSearchSchools, Gtk::PACK_SHRINK);
			boxSchools.pack_start(scrolledWindowSchools);
			boxSchools.pack_start(buttonBoxSchools, Gtk::PACK_SHRINK);
			buttonBoxSchools.pack_start(buttonNewSchool);
			buttonBoxSchools.pack_start(buttonEditSchool);
			buttonBoxSchools.pack_start(buttonDeleteSchool);
			boxSearchSchools.pack_start(labelSearchSchools, Gtk::PACK_SHRINK);
			boxSearchSchools.pack_start(entrySearchSchools);
			scrolledWindowSchools.add(treeViewSchools);			
			
			buttonBoxSchools.set_border_width(10);
			boxSearchSchools.set_border_width(5);			

			treeViewSchools.property_enable_grid_lines().set_value(true);
			
			treeViewSchools.set_model(refListStoreSchools);
			treeViewSchools.set_events(Gdk::BUTTON_PRESS_MASK);

			treeViewSchools.insert_column("ID", tableColumns.id, 0);
			treeViewSchools.insert_column_with_data_func(-1, "Город", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewSchools_cell_renderer_city));
			treeViewSchools.insert_column_with_data_func(-1, "Название", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewSchools_cell_renderer_name));
			treeViewSchools.insert_column_with_data_func(-1, "Полное название", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewSchools_cell_renderer_long_name));
			treeViewSchools.insert_column_with_data_func(-1, "Формат", *Gtk::manage(new Gtk::CellRendererToggle), sigc::mem_fun(*this, &CMainWindow::treeViewSchools_cell_renderer_format));
			treeViewSchools.insert_column_with_data_func(-1, "Ученики", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewSchools_cell_renderer_count));
		
			for (int i = 1; i < 6; ++i)
				treeViewSchools.get_column(i)->set_resizable();
			
			entrySearchSchools.signal_changed().connect(sigc::mem_fun(*this, &CMainWindow::entrySearchSchools_changed));
			treeViewSchools.signal_row_activated().connect(sigc::mem_fun(*this, &CMainWindow::treeViewSchools_row_activated));
			buttonNewSchool.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonNewSchool_clicked));
			buttonEditSchool.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonEditSchool_clicked));
			buttonDeleteSchool.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonDeleteSchool_clicked));
				
			update_schoolsID();		
		}

		void initSubjectsPage ()
		{
			refListStoreSubjects = Gtk::ListStore::create(tableColumns);

			notebook.append_page(boxSubjects, "Предметы");
			boxSubjects.pack_start(scrolledWindowSubjects);
			boxSubjects.pack_start(buttonBoxSubjects, Gtk::PACK_SHRINK);
			buttonBoxSubjects.pack_start(buttonNewSubject);
			buttonBoxSubjects.pack_start(buttonEditSubject);
			buttonBoxSubjects.pack_start(buttonDeleteSubject);
			scrolledWindowSubjects.add(treeViewSubjects);			
			
			buttonBoxSubjects.set_border_width(10);

			treeViewSubjects.property_enable_grid_lines().set_value(true);
			
			treeViewSubjects.set_model(refListStoreSubjects);
			treeViewSubjects.set_events(Gdk::BUTTON_PRESS_MASK);

			treeViewSubjects.insert_column("ID", tableColumns.id, 0);
			treeViewSubjects.insert_column_with_data_func(-1, "Название", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewSubjects_cell_renderer_name));
			treeViewSubjects.insert_column_with_data_func(-1, "Дата", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewSubjects_cell_renderer_date));
			treeViewSubjects.insert_column_with_data_func(-1, "Формат", *Gtk::manage(new Gtk::CellRendererToggle), sigc::mem_fun(*this, &CMainWindow::treeViewSubjects_cell_renderer_format));
			treeViewSubjects.insert_column_with_data_func(-1, "Участники", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::treeViewSubjects_cell_renderer_count));

			for (int i = 1; i < 5; ++i)
				treeViewSubjects.get_column(i)->set_resizable();

			treeViewSubjects.signal_row_activated().connect(sigc::mem_fun(*this, &CMainWindow::treeViewSubjects_row_activated));
			buttonNewSubject.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonNewSubject_clicked));
			buttonEditSubject.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonEditSubject_clicked));
			buttonDeleteSubject.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonDeleteSubject_clicked));
			buttonUnselectOlymps.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonUnselectOlymps_clicked));
			update_subjectsID();		
		}
		
		void treeViewStudents_row_activated (const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
		{
			process_student(studentsID[path[0]]);
		}

		void treeViewOlymps_row_activated (const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
		{
			int id = olympsID[path[0]];

			if (column->get_title().empty())
			{
				std::vector < int >::iterator it = find(selectedOlympsID.begin(), selectedOlympsID.end(), id);
				
				if (it == selectedOlympsID.end())
					selectedOlympsID.push_back(id);
				else
					selectedOlympsID.erase(it);
				
				update_olympsID();
				return;
			}
			
			if (column->get_title() == "Здесь")
			{
				int here;
				
				*dbSession << "SELECT here FROM olymps WHERE id = :id", into(here), use(id), now;
				*dbSession << "UPDATE olymps SET here = :here WHERE id = :id", use((bool)(here == 0)), use(id), now;
				
				update_olympsID();
				return;
			}
			
			process_olymp(id);
		}
		
		void treeViewSchools_row_activated (const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
		{
			process_school(schoolsID[path[0]]);
		}

		void treeViewSubjects_row_activated (const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
		{
			process_subject(subjectsID[path[0]]);
		}
		
		void process_olymp (int id = -1)
		{
			CEditOlympDialog dialog;
			COlympData data;
			
			dialog.set_subjects(get_subjects_array());
			dialog.set_students(get_students_array());
			dialog.set_schools(get_schools_array());
			
			if (id != -1)
			{
				data.id = id;
			
				*dbSession << "SELECT subject_id, student_id, here FROM olymps WHERE id = :id", 
					into(data.subject),
					into(data.student),
					into(data.here),
					use(data.id),
					now;

				dialog.set_data(data);
			}

			if (dialog.run() == Gtk::RESPONSE_OK)
			{
				data = dialog.get_data();
				
				if (data.is_valid())
				{
					if (id == -1)
					{
						*dbSession << "INSERT INTO olymps (subject_id, student_id, here) " \
							"VALUES (:subject, :student, :here)",
							use(data.subject),
							use(data.student),
							use(data.here),
							now;
					}
					else
					{
						*dbSession << "UPDATE olymps SET subject_id = :subject, student_id = :student, here = :here WHERE id = :id",
							use(data.subject),
							use(data.student),
							use(data.here),
							use(id),
							now;
					}					
					
					update_olympsID();
				}
				else
				{
					Gtk::MessageDialog messageDialog("Некоторые поля не были заполнены. Изменения не внесены!", false, Gtk::MESSAGE_ERROR);
					
					messageDialog.run();
				}
			}		
		}
		
		std::vector < CSubjectData > get_subjects_array ()
		{
			std::vector < Poco::Tuple < int , std::string , std::string , int , int , int > > subjects;
			
			*dbSession << "SELECT id, name, format, day, month, year FROM subjects WHERE deleted = 0", into(subjects), now;
			
			std::vector < CSubjectData > result;
			
			for (std::vector < Poco::Tuple < int , std::string , std::string , int, int , int > >::const_iterator it = subjects.begin(); it != subjects.end(); ++it)
				result.push_back(CSubjectData(it->get<0>(), it->get<1>(), it->get<2>(), it->get<3>(), it->get<4>(), it->get<5>()));

			return result;
		}

		std::vector < CSchoolData > get_schools_array ()
		{
			std::vector < Poco::Tuple < int , std::string , std::string , std::string , std::string > > schools;
			std::vector < CSchoolData > result;
			
			*dbSession << "SELECT id, name, long_name, format, city FROM schools WHERE deleted = 0", into(schools), now;
			
			for (std::vector < Poco::Tuple < int , std::string , std::string , std::string , std::string > >::const_iterator it = schools.begin(); it != schools.end(); ++it)
				result.push_back(CSchoolData(it->get<0>(), it->get<1>(), it->get<2>(), it->get<3>(), it->get<4>()));

			return result;
		}

		std::vector < COlympData > get_olymps_array ()
		{
			std::vector < Poco::Tuple < int , int , int , int > > olymps;
			std::vector < COlympData > result;
			
			*dbSession << "SELECT id, subject_id, student_id, here FROM olymps WHERE deleted = 0", into(olymps), now;
			
			for (std::vector < Poco::Tuple < int , int , int , int > >::const_iterator it = olymps.begin(); it != olymps.end(); ++it)
				result.push_back(COlympData(it->get<0>(), it->get<1>(), it->get<2>(), it->get<3>()));

			return result;
		}

		std::vector < CStudentData > get_students_array ()
		{
			std::vector < Poco::Tuple < int , std::string , std::string , std::string , int , int , int , int , int > > students;
			std::vector < CStudentData > result;
			
			*dbSession << "SELECT id, name, middle, surname, level, school_id, birthday, birthmonth, birthyear FROM students WHERE deleted = 0", into(students), now;
			
			for (std::vector < Poco::Tuple < int , std::string , std::string , std::string , int , int , int , int , int > >::const_iterator it = students.begin(); it != students.end(); ++it)
				result.push_back(CStudentData(it->get<0>(), it->get<1>(), it->get<2>(), it->get<3>(), it->get<4>(), it->get<5>(), it->get<6>(), it->get<7>(), it->get<8>()));

			return result;
		}
				
		void process_subject (int id = -1)
		{
			CEditSubjectDialog dialog;
			CSubjectData data;
			
			if (id != -1)
			{
				data.id = id;
			
				*dbSession << "SELECT name, format, day, month, year FROM subjects WHERE id = :id", 
					into(data.name),
					into(data.format),
					into(data.day),
					into(data.month),
					into(data.year),
					use(data.id),
					now;

				dialog.set_data(data);
			}

			if (dialog.run() == Gtk::RESPONSE_OK)
			{
				data = dialog.get_data();
				
				if (data.is_valid())
				{
					if (id == -1)
					{
						*dbSession << "INSERT INTO subjects (name, format, day, month, year) " \
							"VALUES (:name, :format, :day, :month, :year)",
							use(data.name),
							use(data.format),
							use(data.day),
							use(data.month),
							use(data.year),
							now;
					}
					else
					{
						*dbSession << "UPDATE subjects SET name = :name, format = :format, day = :day, month = :month, year = :year WHERE id = :id",
							use(data.name),
							use(data.format),
							use(data.day),
							use(data.month),
							use(data.year),
							use(id),
							now;
					}					
					
					update_subjectsID();
				}
				else
				{
					Gtk::MessageDialog messageDialog("Некоторые поля не были заполнены. Изменения не внесены!", false, Gtk::MESSAGE_ERROR);
					
					messageDialog.run();
				}
			}			
		}
		
		void process_student (int id = -1)
		{
			CEditStudentDialog dialog;
			std::vector < Poco::Tuple < int , std::string , std::string > > schools;
			
			*dbSession << "SELECT id, name, city FROM schools WHERE deleted = 0", into(schools), now;
			
			dialog.set_schools(schools);

			CStudentData data;
			
			if (id != -1)
			{
				data.id = id;
			
				*dbSession << "SELECT name, middle, surname, birthday, birthmonth, birthyear, level, school_id FROM students WHERE id = :id", 
					into(data.name),
					into(data.middle),
					into(data.surname),
					into(data.birthday),
					into(data.birthmonth),
					into(data.birthyear),
					into(data.level),
					into(data.schoolID),
					use(data.id),
					now;

				dialog.set_data(data);
			}

			if (dialog.run() == Gtk::RESPONSE_OK)
			{
				data = dialog.get_data();
				
				if (data.is_valid())
				{
					if (id == -1)
					{
						*dbSession << "INSERT INTO students (name, middle, surname, birthday, birthmonth, birthyear, level, school_id) " \
							"VALUES (:name, :middle, :surname, :day, :month, :year, :level, :school)",
							use(data.name),
							use(data.middle),
							use(data.surname),
							use(data.birthday),
							use(data.birthmonth),
							use(data.birthyear),
							use(data.level),
							use(data.schoolID),
							now;							
					}
					else
					{
						*dbSession << "UPDATE students SET name = :name, middle = :middle, surname = :surname, birthday = :day, birthmonth = :month, birthyear = :year, level = :level, school_id = :school WHERE id = :id",
							use(data.name),
							use(data.middle),
							use(data.surname),
							use(data.birthday),
							use(data.birthmonth),
							use(data.birthyear),
							use(data.level),
							use(data.schoolID),
							use(id),
							now;
					}					
					
					update_studentsID();
				}
				else
				{
					Gtk::MessageDialog messageDialog("Некоторые поля не были заполнены. Изменения не внесены!", false, Gtk::MESSAGE_ERROR);
					
					messageDialog.run();
				}
			}			
		}

		void process_school (int id = -1)
		{
			CEditSchoolDialog dialog;
			CSchoolData data;
			
			if (id != -1)
			{
				data.id = id;
			
				*dbSession << "SELECT name, long_name, city, format FROM schools WHERE id = :id", 
					into(data.name),
					into(data.longName),
					into(data.city),
					into(data.format),
					use(data.id),
					now;

				dialog.set_data(data);
			}

			if (dialog.run() == Gtk::RESPONSE_OK)
			{
				data = dialog.get_data();
				
				if (data.is_valid())
				{
					if (id == -1)
					{
						*dbSession << "INSERT INTO schools (name, long_name, city, format) " \
							"VALUES (:name, :long, :city, :format)",
							use(data.name),
							use(data.longName),
							use(data.city),
							use(data.format),
							now;
							

					}
					else
					{
						*dbSession << "UPDATE schools SET name = :name, long_name = :long, city = :city, format = :format WHERE id = :id",
							use(data.name),
							use(data.longName),
							use(data.city),
							use(data.format),
							use(id),
							now;
					}					
					
					update_schoolsID();
				}
				else
				{
					Gtk::MessageDialog messageDialog("Некоторые поля не были заполнены. Изменения не внесены!", false, Gtk::MESSAGE_ERROR);
					
					messageDialog.run();
				}
			}			
		}

		void entrySearchStudents_changed()
		{
			update_studentsID();
		}

		void entrySearchOlymps_changed()
		{
			update_olympsID();
		}

		void entrySearchSchools_changed()
		{
			update_schoolsID();
		}
	
		void buttonNewStudent_clicked ()
		{
			process_student();
		}
		
		void buttonEditStudent_clicked ()
		{
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewStudents.get_selection();

			if (refSelection)
				if (Gtk::TreeModel::iterator it = refSelection->get_selected())
					process_student((*it)[tableColumns.id]);
		}

		void buttonUnselectOlymps_clicked ()
		{
			selectedOlympsID.clear();
			update_olympsID();
		}
		
		void buttonDoSelectedOlymps_clicked ()
		{
			if (selectedOlympsID.empty())
			{
				Gtk::MessageDialog dialogError("Пустой запрос!", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
				
				dialogError.run();
				
				return;
			}

			CSelectModuleDialog dialog;
			
			dialog.set_data(modules);
			
			if (dialog.run() == Gtk::RESPONSE_OK)
			{
				int index = dialog.get_data();
				
				modules[index]->set_students(get_students_array());
				modules[index]->set_subjects(get_subjects_array());
				modules[index]->set_schools(get_schools_array());
				modules[index]->set_olymps(get_olymps_array());
				modules[index]->run(selectedOlympsID);
			}
		}
		
		void buttonDeleteStudent_clicked ()
		{
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewStudents.get_selection();

			if (refSelection)
			{
				if (Gtk::TreeModel::iterator it = refSelection->get_selected())
				{
					Gtk::MessageDialog messageDialog("Действительно удалить ученика из базы?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
					
					if (messageDialog.run() == Gtk::RESPONSE_YES)
					{
						*dbSession << "UPDATE students SET deleted = 1 WHERE id = :id", use((*it)[tableColumns.id]), now;
						
						update_studentsID();
					}
				}
			}
		}

		void buttonNewSubject_clicked ()
		{
			process_subject();
		}
		
		void buttonEditSubject_clicked ()
		{
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewSubjects.get_selection();

			if (refSelection)
				if (Gtk::TreeModel::iterator it = refSelection->get_selected())
					process_subject((*it)[tableColumns.id]);
		}
		
		void buttonDeleteSubject_clicked ()
		{
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewSubjects.get_selection();

			if (refSelection)
			{
				if (Gtk::TreeModel::iterator it = refSelection->get_selected())
				{
					Gtk::MessageDialog messageDialog("Действительно удалить предмет из базы?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
					
					if (messageDialog.run() == Gtk::RESPONSE_YES)
					{
						*dbSession << "UPDATE subjects SET deleted = 1 WHERE id = :id", use((*it)[tableColumns.id]), now;
						
						update_subjectsID();
					}
				}
			}
		}

		void buttonNewOlymp_clicked ()
		{
			process_olymp();
		}

		void buttonEditOlymp_clicked ()
		{
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewOlymps.get_selection();

			if (refSelection)
				if (Gtk::TreeModel::iterator it = refSelection->get_selected())
					process_olymp((*it)[tableColumns.id]);
		}
		
		void buttonDeleteOlymp_clicked ()
		{
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewOlymps.get_selection();

			if (refSelection)
			{
				if (Gtk::TreeModel::iterator it = refSelection->get_selected())
				{
					Gtk::MessageDialog messageDialog("Действительно удалить участника олимпиады из базы?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
					
					if (messageDialog.run() == Gtk::RESPONSE_YES)
					{
						*dbSession << "UPDATE olymps SET deleted = 1 WHERE id = :id", use((*it)[tableColumns.id]), now;
						
						update_olympsID();
					}
				}
			}
		}
		
		void buttonNewSchool_clicked ()
		{
			process_school();
		}
		
		void buttonEditSchool_clicked ()
		{
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewSchools.get_selection();

			if (refSelection)
				if (Gtk::TreeModel::iterator it = refSelection->get_selected())
					process_school((*it)[tableColumns.id]);
		}
		
		void buttonDeleteSchool_clicked ()
		{
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewSchools.get_selection();

			if (refSelection)
			{
				if (Gtk::TreeModel::iterator it = refSelection->get_selected())
				{
					Gtk::MessageDialog messageDialog("Действительно удалить школу из базы?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
					
					if (messageDialog.run() == Gtk::RESPONSE_YES)
					{
						*dbSession << "UPDATE schools SET deleted = 1 WHERE id = :id", use((*it)[tableColumns.id]), now;
						
						update_schoolsID();
					}
				}
			}				
		}
		
		void update_olympsID ()
		{
			refListStoreOlymps->clear();
			olympsID.clear();
			
			std::string searchText = "%" + entrySearchOlymps.get_text().raw() + "%";

			*dbSession << "SELECT olymps.id FROM olymps INNER JOIN schools, students, subjects " \
				"ON olymps.student_id = students.id AND olymps.subject_id = subjects.id AND schools.id = students.school_id " \
				"WHERE (UPPER(students.name) LIKE UPPER(:text) " \
				"OR UPPER(students.middle) LIKE UPPER(:text) " \
				"OR UPPER(students.surname) LIKE UPPER(:text) " \
				"OR UPPER(students.level) LIKE UPPER(:text) " \
				"OR UPPER(schools.name) LIKE UPPER(:text) " \
				"OR UPPER(schools.city) LIKE UPPER(:text) " \
				"OR UPPER(schools.long_name) LIKE UPPER(:text) "
				"OR UPPER(subjects.name) LIKE UPPER(:text)) " \
				"AND olymps.deleted = 0", use(searchText), into(olympsID), now;

			for (std::vector < int >::const_iterator it = olympsID.begin(); it != olympsID.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreOlymps->append());

				row[tableColumns.id] = *it;
			}
			
			buttonUnselectOlymps.set_label(Glib::ustring::compose("Снять отметки (%1)", selectedOlympsID.size()));
			buttonUnselectOlymps.set_sensitive(!selectedOlympsID.empty());
			buttonDoSelectedOlymps.set_sensitive(!selectedOlympsID.empty());
		}
		
		void update_studentsID ()
		{
			refListStoreStudents->clear();
			studentsID.clear();
			
			std::string searchText = "%" + entrySearchStudents.get_text().raw() + "%";
	
			*dbSession << "SELECT students.id FROM students INNER JOIN schools ON students.school_id = schools.id " \
				"WHERE (UPPER(students.name) LIKE UPPER(:text) " \
				"OR UPPER(students.middle) LIKE UPPER(:text) " \
				"OR UPPER(students.surname) LIKE UPPER(:text) " \
				"OR UPPER(students.level) LIKE UPPER(:text) " \
				"OR UPPER(schools.name) LIKE UPPER(:text) " \
				"OR UPPER(schools.city) LIKE UPPER(:text) " \
				"OR UPPER(schools.long_name) LIKE UPPER(:text)) " \
				"AND students.deleted = 0 AND schools.deleted = 0", use(searchText), into(studentsID), now;

			for (std::vector < int >::const_iterator it = studentsID.begin(); it != studentsID.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreStudents->append());

				row[tableColumns.id] = *it;
			}		
		}
		
		void update_schoolsID ()
		{
			refListStoreSchools->clear();
			schoolsID.clear();
			
			std::string searchText = "%" + entrySearchSchools.get_text().raw() + "%";
			
			*dbSession << "SELECT id FROM schools WHERE deleted = 0 " \
				"AND (UPPER(name) LIKE UPPER(:text) " \
				"OR UPPER(city) LIKE UPPER(:text) " \
				"OR UPPER(long_name) LIKE UPPER(:text) " \
				"OR UPPER(format) LIKE UPPER(:text))", use(searchText), into(schoolsID), now;
			
			for (std::vector < int >::const_iterator it = schoolsID.begin(); it != schoolsID.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreSchools->append());
				
				row[tableColumns.id] = *it;
			}
		}

		void update_subjectsID ()
		{
			refListStoreSubjects->clear();
			subjectsID.clear();
				
			*dbSession << "SELECT id FROM subjects WHERE deleted = 0", into(subjectsID), now;

			for (std::vector < int >::const_iterator it = subjectsID.begin(); it != subjectsID.end(); ++it)
			{
				Gtk::TreeModel::Row row = *(refListStoreSubjects->append());

				row[tableColumns.id] = *it;
			}		
		}
	
		void treeViewStudents_cell_renderer_name (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string name, middle, surname;
		
			*dbSession << "SELECT name, middle, surname FROM students WHERE id=:id", into(name), into(middle), into(surname), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 %2 %3", surname, name, middle));		
		}

		void treeViewStudents_cell_renderer_level (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			int level;
		
			*dbSession << "SELECT level FROM students WHERE id=:id", into(level), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", level));
		}

		void treeViewStudents_cell_renderer_school (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string school;
		
			*dbSession << "SELECT schools.name FROM schools INNER JOIN students ON students.id = :id AND students.school_id = schools.id", into(school), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", school));
		}

		void treeViewStudents_cell_renderer_city (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string city;
		
			*dbSession << "SELECT schools.city FROM schools INNER JOIN students ON students.id = :id AND students.school_id = schools.id", into(city), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", city));
		}

		void treeViewStudents_cell_renderer_birthday (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{


			int id = (*it)[tableColumns.id];
			int day, month, year;
		
			*dbSession << "SELECT birthday, birthmonth, birthyear FROM students WHERE id=:id", into(day), into(month), into(year), use(id), now;

			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", print_date_full(day, month, year)));
		}
		
		void treeViewSchools_cell_renderer_name (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string name;
		
			*dbSession << "SELECT name FROM schools WHERE id=:id", into(name), use(id), now;
		
			cr->set_property<Glib::ustring>("text", name);		
		}

		void treeViewSchools_cell_renderer_city (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string city;
		
			*dbSession << "SELECT city FROM schools WHERE id=:id", into(city), use(id), now;
		
			cr->set_property<Glib::ustring>("text", city);		
		}

		void treeViewSchools_cell_renderer_long_name (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string longName;
		
			*dbSession << "SELECT long_name FROM schools WHERE id=:id", into(longName), use(id), now;
		
			cr->set_property<Glib::ustring>("text", longName);		
		}

		void treeViewSchools_cell_renderer_format (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string format;
		
			*dbSession << "SELECT format FROM schools WHERE id = :id", into(format), use(id), now;
			
			cr->set_property<bool>("active", !format.empty());
		}
		
		void treeViewSchools_cell_renderer_count (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			int count1, count2;
			
			*dbSession << "SELECT COUNT(*) FROM students WHERE school_id = :id AND deleted = 0", into(count1), use(id), now;
			
			*dbSession << "SELECT COUNT(*) FROM students WHERE school_id = :id AND deleted = 1", into(count2), use(id), now;
			
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 / %2", count1, count2));	
		}
		
		void treeViewSubjects_cell_renderer_count (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			int count1, count2;
			
			*dbSession << "SELECT COUNT(*) FROM olymps WHERE subject_id = :id AND deleted = 0", into(count1), use(id), now;
			
			*dbSession << "SELECT COUNT(*) FROM olymps WHERE subject_id = :id AND deleted = 1", into(count2), use(id), now;
			
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 / %2", count1, count2));	
		}		

		void treeViewSubjects_cell_renderer_name (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string name;
			
			*dbSession << "SELECT name FROM subjects WHERE id = :id", into(name), use(id), now;
			
			cr->set_property<Glib::ustring>("text", name);	
		}
		
		void treeViewSubjects_cell_renderer_date (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			int day, month, year;
		
			*dbSession << "SELECT day, month, year FROM subjects WHERE id=:id", into(day), into(month), into(year), use(id), now;

			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", print_date_full(day, month, year)));
		}		

		void treeViewSubjects_cell_renderer_format (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string format;
		
			*dbSession << "SELECT format FROM subjects WHERE id = :id", into(format), use(id), now;
			
			cr->set_property<bool>("active", !format.empty());
		}

		void treeViewOlymps_cell_renderer_here (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			int here;
		
			*dbSession << "SELECT here FROM olymps WHERE id = :id", into(here), use(id), now;
			
			cr->set_property<bool>("active", (bool)here);
		}
		
		void treeViewOlymps_cell_renderer_use (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];

			cr->set_property<bool>("active", (bool)(std::find(selectedOlympsID.begin(), selectedOlympsID.end(), id) != selectedOlympsID.end()));
		}		

		void treeViewOlymps_cell_renderer_student (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string name, middle, surname;
			int level;
			
			*dbSession << "SELECT students.name, students.middle, students.surname, students.level " \
				"FROM olymps INNER JOIN students ON olymps.student_id = students.id " \
				"WHERE olymps.id = :id", 
				into(name), into(middle), into(surname), into(level), use(id), now;
			
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%3 %1 %2 (%4 класс)", name, middle, surname, level));
		}

		void treeViewOlymps_cell_renderer_subject (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string name;
			int day, month, year;
			
			*dbSession << "SELECT subjects.name, subjects.day, subjects.month, subjects.year " \
				"FROM olymps INNER JOIN subjects  ON olymps.subject_id = subjects.id " \
				"WHERE olymps.id = :id", 
				into(name), into(day), into(month), into(year), use(id), now;
						
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 (%2)", name, print_date_short(day, month, year)));	
		}

		void treeViewOlymps_cell_renderer_school (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[tableColumns.id];
			std::string name, city;
			
			*dbSession << "SELECT schools.name, schools.city " \
				"FROM olymps INNER JOIN students, schools ON olymps.student_id = students.id AND students.school_id = schools.id " \
				"WHERE olymps.id = :id", 
				into(name), into(city), use(id), now;
						
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 (%2)", name, city));	
		}
};
