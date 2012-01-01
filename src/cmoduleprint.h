#pragma once

#include "Poco/Zip/Decompress.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Zip/Compress.h"
#include "Poco/Path.h"

#include <iostream>
#include <fstream>
#include <map>

#include "cmodulebase.h"
#include "mytypes.h"

class CModulePrint : public CModuleBase
{
	private:
		void process_file (std::string filename, std::map < std::string , std::string > &replaces)
		{
			std::ifstream in(filename.c_str());
			std::string s;

			for (char ch; in >> std::noskipws >> ch;)
				s.push_back(ch);
				
			in.close();
			
			for (std::map < std::string , std::string >::const_iterator it = replaces.begin(); it != replaces.end(); ++it)
			{
				std::cout << "replace from '" << it->first << "' to '" << it->second << "'." << std::endl;
				
				my_replace(s, it->first, it->second);
			}
			
			std::ofstream out(filename.c_str());
			
			out << s;
			
			out.close();
		}
		
	public:
		~CModulePrint ()
		{
		}
		
		std::string get_name () const
		{
			return "Тестовый модуль печати дипломов";
		}
		
		std::string get_description () const
		{
			return "Пробный модуль";
		}
		
		void process ()
		{
			std::string filename("test.ods");

			for (std::vector < int >::const_iterator it = selectedOlympsID.begin(); it != selectedOlympsID.end(); ++it)
			{
				std::ifstream fileIn(filename.c_str());
				Poco::TemporaryFile temp;
					
				if (!fileIn.is_open())
				{
					std::cout << "Файл \"" << filename << "\" не открыт!" << std::endl;
					continue;
				}

				CStudentData student;
				CSubjectData subject;
				CSchoolData school;

				get_information(*it, student, subject, school);
				
				std::string filenameOut = Glib::ustring::compose("process/Диплом %1 %2 %3 (%4, %5) [%6].ods", student.name, student.middle, student.surname, school.name, school.city, subject.name).raw();
				
				std::ofstream fileOut(filenameOut.c_str());
				Poco::Zip::Decompress archiveOut(fileIn, temp.path());
				
				archiveOut.decompressAllFiles();				

				std::map < std::string , std::string > replaces;

				replaces["##STUDENT_NAME##"] = student.name;
				replaces["##STUDENT_MIDDLE##"] = student.middle;
				replaces["##STUDENT_SURNAME##"] = student.surname;
				replaces["##STUDENT_LEVEL##"] = Glib::ustring::compose("%1", student.level).raw();
				replaces["##SUBJECT_NAME##"] = subject.format;
				replaces["##SUBJECT_DAY##"] = Glib::ustring::compose("%1", subject.day).raw();
				replaces["##SUBJECT_MONTH##"] = get_month(subject.month);
				replaces["##SUBJECT_YEAR##"] = Glib::ustring::compose("%1", subject.year % 100).raw();
				
				for (int i = 1; i <= 5; ++i)
					replaces[Glib::ustring::compose("##SCHOOL_NAME_%1##", i).raw()] = "";
				
				std::string s, format = school.format + "$";
				
				for (int i = 0, count = 0; i < format.length(); ++i)
					if (format[i] == '$')
					{
						replaces[Glib::ustring::compose("##SCHOOL_NAME_%1##", ++count).raw()] = s;
						s.clear();
					}
					else
						s.push_back(format[i]);
				
				process_file(temp.path() + "/content.xml", replaces);
				
				Poco::Zip::Compress archiveIn(fileOut, true);

				archiveIn.addRecursive(temp.path());
				archiveIn.close();
			}		
		}
		
		void clear ()
		{
			CModuleBase::clear();
		}	
};
