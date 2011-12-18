#pragma once

#include <gtkmm.h>

#include "Poco/Data/Common.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/SQLiteException.h"

#include "ceditstudentdialog.h"

using Poco::Data::into;
using Poco::Data::use;
using Poco::Data::now;

class CModelColumnsStudent : public Gtk::TreeModel::ColumnRecord
{
	public:
		Gtk::TreeModelColumn < int > id;
		
		CModelColumnsStudent ()
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
		
		Poco::Data::Session* dbSession;

		Glib::RefPtr < Gtk::ListStore > refListStoreStudents;

		std::vector < int > studentsID, schoolsID;

		CModelColumnsStudent studentColumns;

	public:
		CMainWindow ():
			buttonNewStudent("Добавить"),
			buttonEditStudent("Редактировать"),
			buttonDeleteStudent("Удалить"),
			labelSearchStudents("Критерии отсеивания: "),
			buttonBoxStudents(Gtk::BUTTONBOX_END, 10)
		{
			Poco::Data::SQLite::Connector::registerConnector();		
			dbSession = new Poco::Data::Session("SQLite", "db.sqlite");

			set_title("Таракашка");
			resize(800, 700);
			
			add(box);
			box.pack_start(notebook);

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
			refListStoreStudents = Gtk::ListStore::create(studentColumns);

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

			treeViewStudents.insert_column("ID", studentColumns.id, 0);
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
	
		void treeViewStudents_row_activated (const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
		{
			process_student(studentsID[path[0]]);
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

		void entrySearchStudents_changed()
		{
			update_studentsID();
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
					process_student((*it)[studentColumns.id]);
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
						*dbSession << "UPDATE students SET deleted = 1 WHERE id = :id", use((*it)[studentColumns.id]), now;
						update_studentsID();
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

				row[studentColumns.id] = *it;
			}		
		}
	
		void treeViewStudents_cell_renderer_name (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			std::string name, middle, surname;
		
			*dbSession << "SELECT name, middle, surname FROM students WHERE id=:id", into(name), into(middle), into(surname), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 %2 %3", surname, name, middle));		
		}

		void treeViewStudents_cell_renderer_level (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			int level;
		
			*dbSession << "SELECT level FROM students WHERE id=:id", into(level), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", level));
		}

		void treeViewStudents_cell_renderer_school (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			std::string school;
		
			*dbSession << "SELECT schools.name FROM schools INNER JOIN students ON students.id = :id AND students.school_id = schools.id", into(school), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", school));
		}

		void treeViewStudents_cell_renderer_city (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			std::string city;
		
			*dbSession << "SELECT schools.city FROM schools INNER JOIN students ON students.id = :id AND students.school_id = schools.id", into(city), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", city));
		}

		void treeViewStudents_cell_renderer_birthday (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			Glib::ustring monthName[13] = {"", "января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};

			int id = (*it)[studentColumns.id];
			int day, month, year;
		
			*dbSession << "SELECT birthday, birthmonth, birthyear FROM students WHERE id=:id", into(day), into(month), into(year), use(id), now;

			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 %2 %3", day, monthName[month], year));
		}
};
