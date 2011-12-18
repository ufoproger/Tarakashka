#pragma once

#include "Poco/Data/Common.h"
#include "Poco/Data/SQLite/Connector.h"

#include <gtkmm.h>

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
		Gtk::Entry entrySearchStudents;
		Gtk::Label labelSearchStudents;
		Gtk::Statusbar statusbar;
		Gtk::VBox verticalBox;
		Gtk::HBox hBoxSearchStudents;
		Gtk::VBox vBoxStudents;
		Gtk::Button button1, button2;
		Gtk::Notebook notebook;
		Gtk::ScrolledWindow scrolledWindowStudents;
		Gtk::TreeView treeView;
		Gtk::HButtonBox buttonBoxStudents;
		Gtk::Button buttonNewStudent, buttonDeleteStudent, buttonEditStudent;
		CModelColumnsStudent studentColumns;
		Gtk::TreeView treeViewStudents;
//		Gtk::CheckButton radioHideAwayStudents;
		Glib::RefPtr < Gtk::ListStore > refListStoreStudents;
		std::vector < int > studentsID;							
		Poco::Data::Session* dbSession;
/*		
		bool foo3 (GdkEventButton* event)
		{
			std::cout << "foo3" << std::endl;
			
			Glib::RefPtr < Gtk::TreeView::Selection > refSelection = treeViewStudents.get_selection();

			if(refSelection)
			{
				Gtk::TreeModel::iterator it = refSelection->get_selected();

				if (it)
				{
					CStudentName studentName = (*it)[studentColumns.student];
					
					std::cout << studentName.toString() << std::endl;
					
					studentName.wooof();
	
	
					std::cout << ":" << studentName.toString() << std::endl;
					
					(*it)[studentColumns.student] = studentName;
				}
			}		
			
			return true;
		}
		*/
	public:
		CMainWindow ():
			buttonNewStudent("Добавить"),
			buttonEditStudent("Редактировать"),
			buttonDeleteStudent("Удалить"),
			labelSearchStudents("Критерии отсеивания: "),
			buttonBoxStudents(Gtk::BUTTONBOX_END, 10)
//			radioHideAwayStudents("Скрыть неявившихся")
		{
			Poco::Data::SQLite::Connector::registerConnector();
		
			dbSession = new Poco::Data::Session("SQLite", "db.sqlite");

			set_title("Таракашка");
			resize(800, 700);
			
			add(verticalBox);

			verticalBox.pack_start(notebook);
			verticalBox.pack_start(statusbar, Gtk::PACK_SHRINK);
			
			initStudentsPage();			

			notebook.append_page(button1, "Школы");
	
			
	
/*
	typedef Poco::Tuple < int , int , int, int > point_all;
	typedef std::vector < point_all > points_vector;
	points_vector pv;
	session << "SELECT * FROM example_db WHERE uid=?", use(40), into(pv), now;
	for (points_vector::const_iterator it = pv.begin(); it != pv.end(); ++it)
		std::cout << "item: id=" << it->get<0>() << ", x=" << it->get<1>() << ", y=" << it->get<2>() << ", uid=" << it->get<3>() << std::endl;
*/



/*
			treeViewStudents.append_column("Город", studentColumns.city);
			treeViewStudents.append_column("Класс", studentColumns.level);
						
	//		treeViewStudents.insert_column_with_data_func(-1, "Дата рождения", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::foo));
			
			Gtk::CellRendererToggle* cellToggle = Gtk::manage(new Gtk::CellRendererToggle);
  
//			treeViewStudents.insert_column("Явился", *cellToggle, 0);
			
//			treeViewStudents.get_column(0)->add_attribute(cellToggle->property_active(), studentColumns.here);
	
			
			treeViewStudents.signal_button_release_event().connect(sigc::mem_fun(*this, &CMainWindow::foo3));
*/





/*
			treeView.set_model(refListStore);
			treeView.append_column_editable("Ось абсцисс", columns.columnX);
			treeView.append_column_editable("Ось абсцsdисс", columns.columnY);
		//	treeView.set_fixed_height_mode(false);
			treeView.set_headers_clickable();
			statusbar.push("hello");
			
			Gtk::TreeModel::Row row = *(refListStore->append());

			row[columns.columnX] = Glib::ustring("hldfs sd sd asd asdf");
			row[columns.columnY] = 666;
			row[columns.columnZ] = true;
			row = *(refListStore->append());

			row[columns.columnX] = Glib::ustring("hldfs sd sd asd asdf");
			row[columns.columnY] = 666;
			row[columns.columnZ] = false;

  Gtk::CellRendererToggle* cell = Gtk::manage(new Gtk::CellRendererToggle);
  
  Glib::ustring aaaa("some");
  
  int cols_count = treeView.insert_column(aaaa, *cell, 0);
  Gtk::TreeViewColumn* pColumn = treeView.get_column(cols_count - 1);
  if(pColumn)
  {
    pColumn->add_attribute(cell->property_active(), columns.columnZ);
  }
			*/
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

			notebook.append_page(vBoxStudents, "Ученики");
			
			vBoxStudents.pack_start(hBoxSearchStudents, Gtk::PACK_SHRINK);
			vBoxStudents.pack_start(scrolledWindowStudents);
			vBoxStudents.pack_start(buttonBoxStudents, Gtk::PACK_SHRINK);
			
			buttonBoxStudents.pack_start(buttonNewStudent);
			buttonBoxStudents.pack_start(buttonEditStudent);
			buttonBoxStudents.pack_start(buttonDeleteStudent);
			
			buttonBoxStudents.set_border_width(10);
			
			scrolledWindowStudents.add(treeViewStudents);			

			treeViewStudents.property_enable_grid_lines().set_value(true);
//			treeViewStudents.property_level_indentation().set_value(false);
//			treeViewStudents.property_headers_clickable().set_value(true);
//			treeViewStudents.property_enable_search().set_value(true);
//			treeViewStudents.property_search_column().set_value(3);
//			treeViewStudents.set_search_equal_func(sigc::mem_fun(*this, &CMainWindow::on_search_equal));
			
			hBoxSearchStudents.set_border_width(5);
			
			hBoxSearchStudents.pack_start(labelSearchStudents, Gtk::PACK_SHRINK);
			hBoxSearchStudents.pack_start(entrySearchStudents);
//			hBoxSearchStudents.pack_start(radioHideAwayStudents, Gtk::PACK_SHRINK);
			
			entrySearchStudents.signal_changed().connect(sigc::mem_fun(*this, &CMainWindow::on_my_changed));
			
			treeViewStudents.set_model(refListStoreStudents);
			treeViewStudents.set_events(Gdk::BUTTON_PRESS_MASK);
			treeViewStudents.set_enable_tree_lines();

			treeViewStudents.insert_column_with_data_func(-1, "ФИО", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::student_cell_renderer_name));
//			treeViewStudents.insert_column_with_data_func(0, "Здесь", *Gtk::manage(new Gtk::CellRendererToggle), sigc::mem_fun(*this, &CMainWindow::student_cell_renderer_here));
			treeViewStudents.insert_column_with_data_func(-1, "Город", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::student_cell_renderer_city));
			treeViewStudents.insert_column_with_data_func(-1, "Класс", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::student_cell_renderer_level));
			treeViewStudents.insert_column_with_data_func(-1, "Школа", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::student_cell_renderer_school));
			treeViewStudents.insert_column_with_data_func(-1, "Дата рождения", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::student_cell_renderer_birthday));
			treeViewStudents.insert_column("ID", studentColumns.id, 0);
			
//			treeViewStudents.signal_button_press_event().connect(sigc::mem_fun(*this, &CMainWindow::on_my_button_press_event), false);
			treeViewStudents.signal_row_activated().connect(sigc::mem_fun(*this, &CMainWindow::on_my_row_activated));
				
			update_studentsID();
		}
	
		void on_my_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
		{
/*			if (column->get_title() == Glib::ustring("Здесь"))
			{
				int here, id = studentsID[path[0]];

				*dbSession << "SELECT here FROM students WHERE id=:id", into(here), use(id), now;
				*dbSession << "UPDATE students SET here = :here WHERE id = :id", use((here + 1) % 2), use(id), now;

				update_studentsID();
			}
			else
			{
				
			}*/
			
			std::cout << "row!" << std::endl;
		}
/*		
		bool on_my_button_press_event(GdkEventButton* event)
		{
			if (event->type == GDK_2BUTTON_PRESS && event->button == 1)
				std::cout << "button press event!" << std::endl;
			
			
			return false;
		}
*/	
		void on_my_changed()
		{
			update_studentsID();
		}
	
		bool on_search_equal(const Glib::RefPtr<Gtk::TreeModel>& model, int column, const Glib::ustring& key, const Gtk::TreeModel::iterator& iter)
		{
			std::cout << "search: " << column << ", id: " << (*iter)[studentColumns.id] << ", key: " << key.raw() << std::endl;
			
			return false;
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
	
		void student_cell_renderer_name (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			std::string name, middle, surname;
		
			*dbSession << "SELECT name, middle, surname FROM students WHERE id=:id", into(name), into(middle), into(surname), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 %2 %3", surname, name, middle));		
		}
/*
		void student_cell_renderer_here (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			int here;
		
			*dbSession << "SELECT here FROM students WHERE id=:id", into(here), use(id), now;
		
			cr->set_property<bool>("active", (bool)here);
		}
*/	  
		void student_cell_renderer_level (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			int level;
		
			*dbSession << "SELECT level FROM students WHERE id=:id", into(level), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", level));
		}

		void student_cell_renderer_school (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			std::string school;
		
			*dbSession << "SELECT schools.name FROM schools INNER JOIN students ON students.id = :id AND students.school_id = schools.id", into(school), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", school));
		}

		void student_cell_renderer_city (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			int id = (*it)[studentColumns.id];
			std::string city;
		
			*dbSession << "SELECT schools.city FROM schools INNER JOIN students ON students.id = :id AND students.school_id = schools.id", into(city), use(id), now;
		
			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1", city));
		}

		void student_cell_renderer_birthday (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
		{
			Glib::ustring monthName[13] = {"", "января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};

			int id = (*it)[studentColumns.id];
			int day, month, year;
		
			*dbSession << "SELECT birthday, birthmonth, birthyear FROM students WHERE id=:id", into(day), into(month), into(year), use(id), now;

			cr->set_property<Glib::ustring>("text", Glib::ustring::compose("%1 %2 %3", day, monthName[month], year));
		}
};
