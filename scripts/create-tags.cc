// Create a Gerby tags file from LaTeX labels.
//
// Format:
// * <Part>[1..9]<Theorem>[001..999],<Label> for a theorem label;
// * <Part>[1..9]000,<Label> for a section label.
//
// Assumptions (.tex file):
// * definitions, theorems, etc. are labeled contiguously, not by section;
// * the document consists mainly of sections;
// * labels are put on separate lines;
// * \include or \input are not supported.
//
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

bool grep_str(const std::string &line, const std::string &pat)
{
  return line.find(pat) != std::string::npos;
}

int main (int argc, char* argv[]) {
  if (argc != 2)
    {
      std::cerr << "usage: create-tags <file.tex>" << std::endl;
      std::exit(1);
    }

  std::fstream fs{};
  fs.open(argv[1], std::fstream::in);

  if (fs.bad())
    {
      std::cerr << ("file open: read/write error") << std::endl;
      std::exit(1);
    }
  else if (fs.fail())
    {
      std::cerr << ("file open: logic error") << std::endl;
      std::exit(1);
    }

  std::string line{};
  bool in_block{};
  bool in_section{};
  bool wait_for_label{};
  bool wait_for_section_label{};

  int part_counter{};
  int section_counter{};
  int subsection_counter{};
  int subsubsection_counter{};
  int block_counter{};

  while (std::getline(fs, line))
    {
      // check for begin or end of latex block
      if (grep_str(line, "\\begin{thm}")
	  || grep_str(line, "\\begin{example}")
	  || grep_str(line, "\\begin{defn}")
	  || grep_str(line, "\\begin{prop}")
	  || grep_str(line, "\\begin{cor}")
	  || grep_str(line, "\\begin{lem}")
	  || grep_str(line, "\\begin{rem}"))
	{
	  if (wait_for_section_label)
	    { // if a section has been defined, require a label first
	      throw std::logic_error("\\section: label not defined");
	    }     

	  in_block = true;
	  wait_for_label = true;
	  block_counter++;
	}
      else if (grep_str(line, "\\end"))
	{
	  if (wait_for_label)
	    { // block label was not defined
	      throw std::logic_error("\\end: label not defined");
	    }

	  in_block = false;
	}
      else if (grep_str(line, "\\part{"))
	{
	  part_counter++;
	  section_counter = 0;
	  subsection_counter = 0;
	  subsubsection_counter = 0;
	}
      else if (grep_str(line, "\\section{"))
	{
	  in_section = true;
	  wait_for_section_label = true;

	  section_counter++;
	  subsection_counter = 0;
	  subsubsection_counter = 0;
	}
      else if (grep_str(line, "\\subsection{"))
	{
	  subsection_counter++;
	}
      else if (grep_str(line, "\\subsubsection{"))
	{
	  subsubsection_counter++;
	}
      else if (grep_str(line, "\\label{"))
	{ // start after "{", continue until "}"
	  std::string label = line.substr(7, line.size()-8);

	  if (wait_for_section_label)
	    {
	      wait_for_section_label = false;
	      std::cout << part_counter << "S"
			<< std::setfill('0') << std::setw(2)
			<< section_counter << ","
			<< label << std::endl; 
	    }	      
	  if (in_block)
	    {
	      wait_for_label = false;
	      std::cout << part_counter
			<< std::setfill('0') << std::setw(3)
			<< block_counter << ","
			<< label << std::endl;
	    }
	}
    }
  return 0;
}
