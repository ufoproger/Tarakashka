#pragma once

#include "Poco/Data/Common.h"
#include "Poco/Data/SQLite/Connector.h"

#include <gtkmm.h>

class CModelColumns : public Gtk::TreeModel::ColumnRecord
{
public:
	CModelColumns()
	{
		add(columnZ);
		add(columnX);
		add(columnY);
	}
	
	Gtk::TreeModelColumn < bool > columnZ;
	Gtk::TreeModelColumn < Glib::ustring > columnX;
	Gtk::TreeModelColumn < double > columnY;
};

class CBirthday
{
	private:
		int day;
		int month;
		int year;
		
	public:
		CBirthday ()
		{
			day = 0;
			month = 0;
			year = 0;
		}
		
		CBirthday (int _day, int _month, int _year)
		{
			day = _day;
			month = _month;
			year = _year;
		}
		
		Glib::ustring toString ()
		{
			Glib::ustring monthName[13] = {"", "января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
			
			return Glib::ustring::compose("%1 %2 %3", day, monthName[month], year);
		}
};

class CStudentName
{
	private:
		Glib::ustring name;
		Glib::ustring middle;
		Glib::ustring surname;
		
	public:
		CStudentName ()
		{
		}
		
		CStudentName (const CStudentName &copy)
		{
			name = copy.name;
			middle = copy.middle;
			surname = copy.surname;
		}
		
		CStudentName (Glib::ustring _name, Glib::ustring _middle, Glib::ustring _surname)
		{
			name = _name;
			middle = _middle;
			surname = _surname;
		}
		
		CStudentName (const Glib::ustring &s)
		{
			name = s;
		}
		
		Glib::ustring toString ()
		{
			return Glib::ustring::compose("%3 %1 %2", name, middle, surname);
		}
		
		void wooof ()
		{
			//name = Glib::ustring::compose("%1!", name);
		}
};
/*******************************************/
/*
#include <iostream>

using namespace Poco::Data;

void init()
{
	SQLite::Connector::registerConnector();
}

void shutdown()
{
	SQLite::Connector::unregisterConnector();
}

void incItem (Session &ses, std::string word, int mark_good, int mark_bad, int mark_new)
{
	std::string query_word;
	int query_mark_good, query_mark_bad, query_mark_new;

	ses << "SELECT word, mark_good, mark_bad, mark_new FROM rating WHERE word=:word", into(query_word), into(query_mark_good), into(query_mark_bad), into(query_mark_new), use(word), now;
	
//	std::cout << "data: " << query_word << ", " << query_mark_good << ", " << query_mark_bad << ", " << query_mark_new << ";" << std::endl;
	
	if (query_word.empty())
		ses << "INSERT INTO rating VALUES (:word, :mark_good, :mark_bad, :mark_new)", use(word), use(mark_good), use(mark_bad), use(mark_new), now;
	else
		ses << "UPDATE rating SET mark_good=:mark_good, mark_bad=:mark_bad, mark_new=:mark_new WHERE word=:word", use(mark_good + query_mark_good), use(mark_bad + query_mark_bad), use(mark_new + query_mark_new), use(word), now;
}

int main(int argc, char* argv[])
{
	init();
	
	Session *ses = new Session("SQLite", "test.db");
	

		std::string query_word;
	int query_mark_good, query_mark_bad, query_mark_new;

	*ses << "SELECT word, mark_good, mark_bad, mark_new FROM rating WHERE word=:word", into(query_word), into(query_mark_good), into(query_mark_bad), into(query_mark_new), use(query_word), now;
	
//	ses << "SELECT COUNT(*) FROM rating", into(count), now;





	//std::cout << "People in DB " << count << std::endl;
	
//	std::string s;
//	int ia, ib, ic;
//	int mark_good;
//	std::string aName("mihas");
	
//	ses << "SELECT word, mark_good FROM rating WHERE word=\"mias\"", into(s), into(ia), now;
//	std::cout << i << std::endl;}
//		std::cout << "ss: " << s << " " << ia << std::endl;
	
//	incItem(ses, "mihas", 1, 2, 3);
	shutdown();

	return 0;
}*/
/*******************************************/

class CModelColumnsStudent : public Gtk::TreeModel::ColumnRecord
{
	public:
		Gtk::TreeModelColumn < int > id;
		Gtk::TreeModelColumn < bool > here;
		Gtk::TreeModelColumn < CStudentName > student;
		Gtk::TreeModelColumn < CBirthday > birthday;
		Gtk::TreeModelColumn < Glib::ustring > school;
		Gtk::TreeModelColumn < int > level;
		Gtk::TreeModelColumn < Glib::ustring > city;
		
		CModelColumnsStudent ()
		{
			add(id);
			add(here);
			add(student);
			add(city);
			add(level);
			add(school);
			add(birthday);
		}
};
class CMainWindow : public Gtk::Window
{
	static void foo2 (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
	{
		CModelColumnsStudent studentColumns;
		CStudentName studentName((*it)[studentColumns.student]);
		
		cr->set_property<Glib::ustring>("text", studentName.toString());	
	}
	
	void foo (Gtk::CellRenderer* cr, const Gtk::TreeModel::iterator& it)
	{
		CModelColumnsStudent studentColumns;
		CBirthday birthday((*it)[studentColumns.birthday]);
		
		int id = (*it)[studentColumns.id];
		
		std::string name;
		
		*dbSession << "SELECT name FROM students WHERE id=:id", Poco::Data::into(name), Poco::Data::use(id), Poco::Data::now;
		
		cr->set_property<Glib::ustring>("text", name);
	}

	private:
		Gtk::Statusbar statusbar;
		Gtk::VBox verticalBox;
		Gtk::VBox vBoxStudents;
		Gtk::Button button1, button2;
		Gtk::Notebook notebook;
		Gtk::TreeView treeView;
		CModelColumns columns;
		CModelColumnsStudent studentColumns;
		Gtk::TreeView treeViewStudents;
		Glib::RefPtr < Gtk::ListStore > refListStoreStudents;
		
		Poco::Data::Session* dbSession;
		
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
		
	public:
		CMainWindow ():
			button2("Test sdfdfsd")
		{
			Poco::Data::SQLite::Connector::registerConnector();
		
			dbSession = new Poco::Data::Session("SQLite", "db.sqlite");

			set_title("Таракашка");
			resize(600, 700);
			
			add(verticalBox);
			
			refListStoreStudents = Gtk::ListStore::create(studentColumns);
			verticalBox.pack_start(notebook);
			verticalBox.pack_start(statusbar, Gtk::PACK_SHRINK);
			
			notebook.append_page(vBoxStudents, "Ученики");
			notebook.append_page(button1, "Школы");
			
			vBoxStudents.pack_start(treeViewStudents);
			vBoxStudents.pack_start(button2, Gtk::PACK_SHRINK);
			
			treeViewStudents.set_model(refListStoreStudents);
//			treeViewStudents.append_column_editable("ФИО", studentColumns.student);

			treeViewStudents.set_events(Gdk::BUTTON_PRESS_MASK);
			
			Gtk::TreeModel::Row row = *(refListStoreStudents->append());
			row[studentColumns.id] = 0;
			row[studentColumns.student] = CStudentName("Иван", "Сергеевич", "Перекотий");
			row[studentColumns.birthday] = CBirthday(6, 6, 1992);
			row[studentColumns.here] = false;
			row[studentColumns.city] = "Абакан";
			row[studentColumns.level] = 11;
			
			row = *(refListStoreStudents->append());
			row[studentColumns.id] = 1;
			row[studentColumns.student] = CStudentName("Константин", "Юрьевич", "Лапаузов");
			row[studentColumns.birthday] = CBirthday(24, 11, 1993);
			row[studentColumns.here] = true;
			row[studentColumns.city] = "Красноярск, Советский район";
			row[studentColumns.level] = 9;
			
			treeViewStudents.insert_column_with_data_func(-1, "ФИО", *Gtk::manage(new Gtk::CellRendererText), sigc::ptr_fun(&foo2));
			treeViewStudents.append_column("Город", studentColumns.city);
			treeViewStudents.append_column("Класс", studentColumns.level);
						
			treeViewStudents.insert_column_with_data_func(-1, "Дата рождения", *Gtk::manage(new Gtk::CellRendererText), sigc::mem_fun(*this, &CMainWindow::foo));
			
			Gtk::CellRendererToggle* cellToggle = Gtk::manage(new Gtk::CellRendererToggle);
  
			treeViewStudents.insert_column("Явился", *cellToggle, 0);
			treeViewStudents.get_column(0)->add_attribute(cellToggle->property_active(), studentColumns.here);
	
			treeViewStudents.insert_column("ID", studentColumns.id, 0);
			
			treeViewStudents.signal_button_release_event().connect(sigc::mem_fun(*this, &CMainWindow::foo3));






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
};
