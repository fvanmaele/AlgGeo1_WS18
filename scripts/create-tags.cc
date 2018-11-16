// Create a Gerby tags file from LaTeX labels.
//
// Format:
// * <Chapter>[1..9]<Theorem>[001..999],<Label> for a theorem label;
// * <Chapter>[1..9]S<Section>[01..99],<Label> for a section label;
// * <Chapter>[1..9]T<Subsection[01..99],<Label> for a subsection label;
// * <Chapter>[1..9]U<Subsubsection[01..99],<Label> for a subsubsection label.
//
// Assumptions (.tex file):
// * definitions, theorems, etc. are labeled by chapter;
// * the document consists of a single part;
// * labels are put on separate lines;
// * the document contains no \include or \input lines.
//
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

bool grep_str(const std::string &line, const std::string &pat) {
  return line.find(pat) != std::string::npos;
}

void print_tag(std::string prefix, std::string label,
               int counter, char f = '0') {
  // available characters for counter
  int len = 4 - prefix.length();

  if (len < 1) {
    throw std::out_of_range("tag can only contain 4 characters");
  }
  if (counter > std::pow(10, len)-1) {
    throw std::out_of_range("counter too large for tag");
  }

  std::cout << prefix  << std::setfill(f) << std::setw(len)
            << counter << ","
            << label   << std::endl;
}

int main (int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: create-tags <file.tex>" << std::endl;
    std::exit(1);
  }

  std::fstream fs{};
  fs.open(argv[1], std::fstream::in);

  if (fs.bad()) {
    std::cerr << ("file open: read/write error") << std::endl;
    std::exit(1);
  }
  if (fs.fail()) {
    std::cerr << ("file open: logic error") << std::endl;
    std::exit(1);
  }

  std::string line{};
  bool wait_for_label{};
  bool wait_for_part_label{};
  bool wait_for_chapter_label{};
  bool wait_for_section_label{};
  bool wait_for_subsection_label{};
  bool wait_for_subsubsection_label{};

  int part_counter{};
  int chapter_counter{};
  int section_counter{};
  int subsection_counter{};
  int subsubsection_counter{};
  int block_counter{};

  // read file with whitespace skipping
  while (fs >> std::ws && std::getline(fs, line)) {
    // check for begin or end of latex block
    if (grep_str(line, "\\begin{thm}")
        || grep_str(line, "\\begin{example}")
        || grep_str(line, "\\begin{defn}")
        || grep_str(line, "\\begin{prop}")
        || grep_str(line, "\\begin{cor}")
        || grep_str(line, "\\begin{lem}")
        || grep_str(line, "\\begin{rem}"))
      {
        if (wait_for_section_label) {
          // if a section has been defined, require a label first
          throw std::logic_error("\\begin: label undefined (\\section)");
        }
        wait_for_label = true;
        block_counter++;
      }
    else if (grep_str(line, "\\end")) {
      if (wait_for_label) {
        // block label was not defined
        throw std::logic_error("\\end: label undefined (\\begin)");
      }
    }
    else if (grep_str(line, "\\part{")) {
      part_counter++;
      if (part_counter > 1) {
	throw std::logic_error("function not yet implemented");
      }
      wait_for_part_label = true;
      chapter_counter = 0;
      section_counter = 0;
      subsection_counter = 0;
      subsubsection_counter = 0;
    }
    else if (grep_str(line, "\\chapter{")) {
      if (wait_for_part_label) {
	throw std::logic_error("\\chapter: label undefined (\\part)");
      }
      wait_for_chapter_label = true;
      chapter_counter++;
      section_counter = 0;
      subsection_counter = 0;
      subsubsection_counter = 0;
    }
    else if (grep_str(line, "\\section{")) {
      if (wait_for_chapter_label) {
	throw std::logic_error("\\section: label undefined (\\chapter)");
      }
      wait_for_section_label = true;
      section_counter++;
      subsection_counter = 0;
      subsubsection_counter = 0;
    }
    else if (grep_str(line, "\\subsection{")) {
      if (wait_for_section_label) {
        throw std::logic_error("\\subsection: label undefined (\\section)");
      }
      wait_for_subsection_label = true;
      subsection_counter++;
      subsubsection_counter = 0;
    }
    else if (grep_str(line, "\\subsubsection{")) {
      if (wait_for_section_label) {
        throw std::logic_error("\\subsubsection: label undefined (\\section)");
      }
      if (wait_for_subsection_label) {
        throw std::logic_error("\\subsubsection: label undefined (\\subsection)");
      }
      wait_for_subsubsection_label = true;
      subsubsection_counter++;
    }
    else if (grep_str(line, "\\label{")) {
      // start after "{", continue until "}"
      std::string label = line.substr(7, line.size()-8);
      std::string chapter_prefix = std::to_string(chapter_counter);

      if (wait_for_label) {
        wait_for_label = false;
        print_tag(chapter_prefix, label, block_counter);
      }
      if (wait_for_chapter_label) {
	wait_for_chapter_label = false;
	print_tag(chapter_prefix, label, 0);
      }
      if (wait_for_section_label) {
        wait_for_section_label = false;
        print_tag(chapter_prefix + "S", label, section_counter);
      }
      if (wait_for_subsection_label) {
        wait_for_subsection_label = false;
        print_tag(chapter_prefix + "T", label, subsection_counter);
      }
      if (wait_for_subsubsection_label) {
        wait_for_subsubsection_label = false;
        print_tag(chapter_prefix + "U", label, subsubsection_counter);
      }
    }
  }
  return 0;
}
