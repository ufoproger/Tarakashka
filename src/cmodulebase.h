#pragma once

#include <gtkmm/dialog.h>
#include <glibmm.h>

#include <string>
#include <vector>

#include "mytypes.h"

class CModuleBase
{
	protected:
		Gtk::Dialog *dialog;
		std::vector < CStudentData > students;
		std::vector < CSubjectData > subjects;
		std::vector < CSchoolData > schools;
		std::vector < COlympData > olymps;
		std::vector < int > selectedOlympsID;

	protected:
		virtual void process () = 0;
		
		bool get_information (int id, CStudentData &student, CSubjectData &subject, CSchoolData &school)
		{
			for (std::vector < COlympData >::const_iterator it = olymps.begin(); it != olymps.end(); ++it)
				if (it->id == id)
				{
					for (std::vector < CStudentData >::const_iterator it2 = students.begin(); it2 != students.end(); ++it2)
						if (it2->id == it->student)
						{
							student = *it2;
							
							break;
						}
						
					for (std::vector < CSubjectData >::const_iterator it2 = subjects.begin(); it2 != subjects.end(); ++it2)
						if (it2->id == it->subject)
						{
							subject = *it2;
							
							break;
						}
						
					break;
				}
				
			for (std::vector < CSchoolData >::const_iterator it = schools.begin(); it != schools.end(); ++it)
				if (it->id == student.schoolID)
				{
					school = *it;
					
					break;
				}
				
			return true;
		}
		
	public:
		CModuleBase ()
		{
			clear();
		}
		
		~CModuleBase ()
		{
		}
		
		virtual std::string get_name () const = 0;
		virtual std::string get_description () const = 0;		
		
		void clear ()
		{
			students.clear();
			subjects.clear();
			schools.clear();
			selectedOlympsID.clear();
			
			dialog = NULL;
		}
		
		void set_students (const std::vector < CStudentData > &_students)
		{
			students = _students;
		}

		void set_subjects (const std::vector < CSubjectData > &_subjects)
		{
			subjects = _subjects;
		}
		
		void set_schools (const std::vector < CSchoolData > &_schools)
		{
			schools = _schools;
		}
		
		void set_olymps (const std::vector < COlympData > &_olymps)
		{
			olymps = _olymps;
		}
		
		bool run (const std::vector < int > &_selectedOlympsID)
		{
			selectedOlympsID = _selectedOlympsID;
		
			if (selectedOlympsID.empty())
			{
				Gtk::MessageDialog dialogError("Пустой запрос!", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
				
				dialogError.run();
				
				return false;
			}
			
			if (dialog == NULL)
				dialog = new Gtk::MessageDialog(Glib::ustring::compose("Выполнить модуль \"%1\"?", get_name()), false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
				
			int response = dialog->run();
			
			delete dialog;
			
			if (response == Gtk::RESPONSE_YES || response == Gtk::RESPONSE_OK)
			{
				process();
				
				return true;
			}

			return false;
		}
};
