#!/bin/bash
src=split-by-bookmarks.cc
tmp=$(mktemp -d)
i=0

echo "building $src to $tmp..."
g++ -DBMARK_LEVEL=2 "$src" -o "$tmp"/converter

pdftk "$1" dump_data_utf8 | "$tmp"/converter | while read -r a b title; do
    title=${title// /-}
    title=${title//\//-}
    title=${title//ä/ae}
    title=${title//ö/oe}
    ttile=${title//ü/ue}
    title=${title:-preamble}

    echo "$2-${i}_$title".pdf
    pdftk "$1" cat $a-$b output "$2-${i}_$title".pdf
    let i++;
done

rm -rf "$tmp"
