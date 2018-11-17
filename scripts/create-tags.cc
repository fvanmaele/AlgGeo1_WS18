// Create a Gerby tags file from LaTeX labels.
//
// Format:
// * <Chapter>[1..9]<Theorem>[001..999],<Label> for a theorem label;
// * <Chapter>[1..9]S<Section>[01..99],<Label> for a section label;
//
// Assumptions (.tex file):
// * definitions, theorems, etc. are labeled by chapter;
// * the document consists of a single part;
// * labels are put on separate lines;
// * the document contains no \include or \input lines.
//
// Caveats:
// * subsections and subsubsections are numbered continously
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

bool find_str(const std::string &line, const std::string &pat) {
  return line.find(pat) != std::string::npos;
}

std::string write_tag(const std::string &prefix, int counter, char f = '0') {
  // available characters for counter
  int len = 4 - prefix.length();

  if (len < 1) {
    throw std::out_of_range("tag can only contain 4 characters");
  }
  if (counter > std::pow(10, len)-1) {
    throw std::out_of_range("counter too large for tag");
  }

  std::stringstream tag{};
  tag << prefix << std::setfill(f) << std::setw(len)
      << counter;

  return tag.str();
}

void print_tag(std::unordered_set<std::string> &tag_set,
	       const std::string &tag,
	       const std::string &label) {
  if (tag_set.find(tag) == tag_set.end()) {
    tag_set.insert(tag);
    std::cout << tag << "," << label << std::endl;
  } else {
    std::string err = "duplicate tag: " + tag + ", label: " + label;
    throw std::invalid_argument(err);
  }
}

void is_label_undefined(const std::string &pos, const std::string &prev,
			bool marker, const std::string &label) {
  if (marker) {
    std::string err = pos + ": label undefined (" + prev;

    if (label.size()) {
      err += ", last label " + label + ")";
    } else {
      err += ")";
    }
    throw std::invalid_argument(err);
  }
}

// https://stackoverflow.com/questions/1798112
std::string trim_right(std::string& s, const char* t = " \t\n\r\f\v") {
    s.erase(s.find_last_not_of(t) + 1);
    return s;
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
  std::string last_label{};
  std::unordered_set<std::string> tag_set;

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
    if (find_str(line, "\\begin{thm}")
	|| find_str(line, "\\begin{example}")
	|| find_str(line, "\\begin{defn}")
	|| find_str(line, "\\begin{prop}")
	|| find_str(line, "\\begin{cor}")
	|| find_str(line, "\\begin{lem}")
	|| find_str(line, "\\begin{rem}"))
      {
	// if a section has been defined, require a label first
	is_label_undefined("\\begin", "\\chapter",
			   wait_for_chapter_label, last_label);
	is_label_undefined("\\begin", "\\part",
			   wait_for_part_label, last_label);
	is_label_undefined("\\begin", "\\section",
			   wait_for_section_label, last_label);
	is_label_undefined("\\begin", "\\subsection",
			   wait_for_subsection_label, last_label);
	is_label_undefined("\\begin", "\\subsubsection",
			   wait_for_subsubsection_label, last_label);

	wait_for_label = true;
	block_counter++;
      }
    else if (find_str(line, "\\end")) {
      // block label was not defined
      is_label_undefined("\\end", "\\begin", wait_for_label, last_label);
    }
    else if (find_str(line, "\\part{")) {
      part_counter++;
      wait_for_part_label = true;
      //chapter_counter = 0;
      section_counter = 0;
      subsection_counter = 0;
      subsubsection_counter = 0;
    }
    else if (find_str(line, "\\chapter{")) {
      is_label_undefined("\\chapter", "\\part",
			 wait_for_part_label, last_label);

      wait_for_chapter_label = true;
      chapter_counter++;
      section_counter = 0;
      subsection_counter = 0;
      subsubsection_counter = 0;
    }
    else if (find_str(line, "\\section{")) {
      is_label_undefined("\\section", "\\part",
			 wait_for_part_label, last_label);
      is_label_undefined("\\section", "\\chapter",
			 wait_for_chapter_label, last_label);

      wait_for_section_label = true;
      section_counter++;
      //subsection_counter = 0;
      //subsubsection_counter = 0;
    }
    else if (find_str(line, "\\subsection{")) {
      is_label_undefined("\\subsection", "\\part",
			 wait_for_part_label, last_label);
      is_label_undefined("\\subsection", "\\chapter",
			 wait_for_chapter_label, last_label);
      is_label_undefined("\\subsection", "\\section",
			 wait_for_section_label, last_label);

      wait_for_subsection_label = true;
      subsection_counter++;
      //subsubsection_counter = 0;
    }
    else if (find_str(line, "\\subsubsection{")) {
      is_label_undefined("\\subsubsection", "\\part",
			 wait_for_part_label, last_label);
      is_label_undefined("\\subsubsection", "\\chapter",
			 wait_for_chapter_label, last_label);
      is_label_undefined("\\subsubsection", "\\section",
			 wait_for_section_label, last_label);
      is_label_undefined("\\subsubsection", "\\subsection",
			 wait_for_subsection_label, last_label);

      wait_for_subsubsection_label = true;
      subsubsection_counter++;
    }
    else if (find_str(line, "\\label{")) {
      // start after "{", continue until "}"}
      last_label = line.substr(7, trim_right(line).size() - 8);
      std::string chapter_prefix = std::to_string(chapter_counter);
      std::string new_tag{};

      if (wait_for_label) {
	new_tag = write_tag(chapter_prefix, block_counter);
	print_tag(tag_set, new_tag, last_label);
	wait_for_label = false;
      }
      if (wait_for_chapter_label) {
	new_tag = write_tag(chapter_prefix, 0);
	print_tag(tag_set, new_tag, last_label);
	wait_for_chapter_label = false;
      }
      if (wait_for_section_label) {
	new_tag = write_tag(chapter_prefix + "S", section_counter);
	print_tag(tag_set, new_tag, last_label);
	wait_for_section_label = false;
      }
      if (wait_for_subsection_label) {
	new_tag = write_tag(chapter_prefix + "T", subsection_counter);
	print_tag(tag_set, new_tag, last_label);
	wait_for_subsection_label = false;
      }
      if (wait_for_subsubsection_label) {
	new_tag = write_tag(chapter_prefix + "U", subsubsection_counter);
	print_tag(tag_set, new_tag, last_label);
	wait_for_subsubsection_label = false;
      }
    }
  }
  return 0;
}
