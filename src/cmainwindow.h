#pragma once

#include <gtkmm.h>

#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Common.h"

#include "ceditstudentdialog.h"
#include "ceditschooldialog.h"

using Poco::Data::into;
using Poco::Data::use;
using Poco::Data::now;

class CModelColumnsOnlyID : public Gtk::TreeModel::ColumnRecord
{
	public:
		Gtk::TreeModelColumn < int > id;
		
		CModelColumnsOnlyID ()
		{
			add(id);
		}
};

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
		
		Poco::Data::Session* dbSession;

		Glib::RefPtr < Gtk::ListStore > refListStoreStudents;
		Glib::RefPtr < Gtk::ListStore > refListStoreSchools;

		std::vector < int > studentsID;
		std::vector < int > schoolsID;

		CModelColumnsOnlyID tableColumns;

	public:
		CMainWindow ():
			buttonNewStudent("Добавить"),
			buttonEditStudent("Редактировать"),
			buttonDeleteStudent("Удалить"),
			buttonNewSchool("Добавить"),
			buttonEditSchool("Редактировать"),
			buttonDeleteSchool("Удалить"),
			labelSearchStudents("Критерий отсеивания: "),
			labelSearchSchools("Критерий отсеивания: "),
			buttonBoxStudents(Gtk::BUTTONBOX_END, 10),
			buttonBoxSchools(Gtk::BUTTONBOX_END, 10)
		{
			Poco::Data::SQLite::Connector::registerConnector();		
			dbSession = new Poco::Data::Session("SQLite", "db.sqlite");

			set_title("Таракашка");
			resize(800, 700);
			
			add(box);
			box.pack_start(notebook);
		
			initSchoolsPage();
			initStudentsPage();
			show_all_children();
		}
		
		~CMainWindow ()
		{
			Poco::Data::SQLite::Connector::unregisterConnector();
		}
		
	private:
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
			
			entrySearchSchools.signal_changed().connect(sigc::mem_fun(*this, &CMainWindow::entrySearchSchools_changed));
			treeViewSchools.signal_row_activated().connect(sigc::mem_fun(*this, &CMainWindow::treeViewSchools_row_activated));
			buttonNewSchool.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonNewSchool_clicked));
			buttonEditSchool.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonEditSchool_clicked));
			buttonDeleteSchool.signal_clicked().connect(sigc::mem_fun(*this, &CMainWindow::buttonDeleteSchool_clicked));
				
			update_schoolsID();		
		}
		
		void treeViewStudents_row_activated (const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
		{
			process_student(studentsID[path[0]]);
		}
		
		void treeViewSchools_row_activated (const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
		{
			process_school(schoolsID[path[0]]);
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
			Glib::ustring monthName[13] = {"", "января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};

			int id = (*it)[tableColumns.id];
			int day, month, year;
		
			*dbSession << "SELECT birthday, birthmonth, birthyear FROM students WHERE id=:id", into(day), into(month), into(year), use(id), now;

			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 %2 %3", day, monthName[month], year));
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
		
			*dbSession << "SELECT format FROM schools WHERE id=:id", into(format), use(id), now;
			
			cr->set_property<bool>("active", !format.empty());
		}
};
