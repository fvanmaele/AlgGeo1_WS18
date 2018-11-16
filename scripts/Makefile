all: create-tags split-by-bookmarks

create-tags: create-tags.cc
	$(CXX) -o create-tags create-tags.cc

split-by-bookmarks: split-by-bookmarks.cc
	$(CXX) -o split-by-bookmarks -DBMARK_LEVEL=2 split-by-bookmarks.cc
