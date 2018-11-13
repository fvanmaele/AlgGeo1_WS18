.PHONY: clean
all: create-tags split-by-bookmarks flatex

create-tags: create-tags.cc
	$(CXX) -o create-tags create-tags.cc

split-by-bookmarks: split-by-bookmarks.cc
	$(CXX) -o split-by-bookmarks split-by-bookmarks.cc

flatex: flatex.c
	$(CC)  -o flatex flatex.c

clean:
	@rm -v create-tags split-by-bookmarks flatex
