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

class CModulePrintTable : public CModuleBase
{
	private:
		void process_file (std::string filename)
		{
			std::ifstream in(filename.c_str());
			std::string s;

			for (char ch; in >> std::noskipws >> ch;)
				s.push_back(ch);
				
			in.close();

			std::string table("table:table-row");
			std::string row, header, footer;
			size_t pos = s.find("##CURRENT_ROW##");
			
			if (pos != std::string::npos)
			{
				size_t posEndHeader = s.rfind(table, pos) - 1;
				size_t posBeginFooter = s.find(table, pos) + table.length() + 1;
				
				header = s.substr(0, posEndHeader);
				footer = s.substr(posBeginFooter);
				row = s.substr(posEndHeader, posBeginFooter - posEndHeader);
			}

			s.clear();

			my_replace(row, "##CURRENT_ROW##", "");
			
			int count = 0;
			
			for (std::vector < int >::const_iterator it = selectedOlympsID.begin(); it != selectedOlympsID.end(); ++it)
			{			
				++count;
				
				CStudentData student;
				CSubjectData subject;
				CSchoolData school;

				get_information(*it, student, subject, school);

				std::map < std::string , std::string > replaces;

				replaces["##ID##"] = Glib::ustring::compose("%1", count).raw();
				replaces["##STUDENT_NAME##"] = student.name;
				replaces["##STUDENT_MIDDLE##"] = student.middle;
				replaces["##STUDENT_SURNAME##"] = student.surname;
				replaces["##STUDENT_LEVEL##"] = Glib::ustring::compose("%1", student.level);			
				replaces["##STUDENT_BIRTHDAY##"] = print_date_short(student.birthday, student.birthmonth, student.birthyear);
				replaces["##SCHOOL_NAME##"] = school.name;
				replaces["##SCHOOL_LONG_NAME##"] = school.longName;
				replaces["##SCHOOL_FORMAT##"] = school.format;
				replaces["##SCHOOL_CITY##"] = school.city;
				
				std::string current_row = row;
				
				for (std::map < std::string , std::string >::const_iterator it2 = replaces.begin(); it2 != replaces.end(); ++it2)
					my_replace(current_row, it2->first, it2->second);
					
				s += current_row;
			}
			
			std::ofstream out(filename.c_str());

			out << header << s << footer;
			
			out.close();
		}
		
	public:
		~CModulePrintTable ()
		{
		}
		
		std::string get_name () const
		{
			return "Печать таблицы с данными участников";
		}
		
		std::string get_description () const
		{
			return "Плюшки-финтифлюшки.";
		}
		
		void process ()
		{
			std::string filename("test2.ods");
			std::ifstream fileIn(filename.c_str());
			Poco::TemporaryFile temp;
				
			if (!fileIn.is_open())
			{
				std::cout << "Файл \"" << filename << "\" не открыт!" << std::endl;
			}

			std::string filenameOut = "process/Таблица.ods";
			
			Poco::Zip::Decompress archiveOut(fileIn, temp.path());
			
			archiveOut.decompressAllFiles();				

			process_file(temp.path() + "/content.xml");
			
			std::ofstream fileOut(filenameOut.c_str());
			Poco::Zip::Compress archiveIn(fileOut, true);

			archiveIn.addRecursive(temp.path());
			archiveIn.close();
		}
		
		void clear ()
		{
			CModuleBase::clear();
		}	
};
