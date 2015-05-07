#!/bin/sh

bookname=book

HTML_DIR='../__html'
ARCHIVE_DIR='../archives'
TEMP_DIR='temp'

refreshDirectory()
{
	if [ -d $1 ];then
		rm -rf $1
	fi

	mkdir $1
}

pdf_maker()
{
	change_language  "cs" "[Sharp\\]C"
	cp -f config.yml.template config.yml

	echo "texstyle: $1" >> config.yml
	echo "texdocumentclass: [\"jsbook\", \"$2\"]" >> config.yml

	review-pdfmaker --ignore-errors config.yml

	if [ -e ${bookname}.pdf ];then

		if [ ! -d "${ARCHIVE_DIR}" ];then
			mkdir "${ARCHIVE_DIR}"
		fi

		mv ${bookname}.pdf "${ARCHIVE_DIR}/"$3
	fi
}

epub_maker()
{
	change_language "cs" "c#"

	review-epubmaker config.yml.template
	
	if [ -e ${bookname}.epub ];then

		if [ ! -d "${ARCHIVE_DIR}" ];then
			mkdir "${ARCHIVE_DIR}"
		fi

		mv ${bookname}.epub "${ARCHIVE_DIR}"
	fi
}

change_language()
{
	ruby -e 'Dir.glob("*.re").each { |re| File.write(re, File.open(re).read.gsub( /\/\/emlist\[([^\]]*)?\](\[#{Regexp.quote(ARGV[0])}\])/) { |w| w.gsub($2, "[#{ARGV[1]}]")}) }' $1 $2
}

re_build_catalog()
{
	backup_catalog

	echo "CHAPS:" >> catalog.yml

	for file in $(find . -name "*.re");do
		conti=1
		for c in $(cat catalog.yml);do
			if [ "${file#*\./}" == $c ];then
				conti=0
			fi
		done

		if [ $conti == 0 ];then
			continue
		fi

		echo " - ${file#*\./}" >> catalog.yml
	done
}

backup_catalog()
{
	CATALOG='catalog.yml'

	if [ -e $CATALOG ];then
		cp -f $CATALOG $CATALOG.bak
	fi

	cp $CATALOG.template $CATALOG
}

undo_catalog()
{
	mv "${CATALOG}.bak" $CATALOG
}

md2re()
{
	for file in $(find . -name "*.md");do
		md2review $file > ${file%.*}.re
	done
}

build_jenkins()
{

	re_build_catalog
	epub_maker
	pdf_maker ${bookname}macro 'a5paper,14pt,oneside' ${bookname}.pdf
	pdf_maker ${bookname}macro-bookbinding 'a5paper,14pt' ${bookname}-bookbinding.pdf

	undo_catalog
}

build()
{
	pdf_maker ${bookname}macro 'a5paper,14pt,oneside' ${bookname}.pdf
}

build_release()
{
	cp -f catalog-release.yml catalog.yml
	epub_maker
	pdf_maker ${bookname}macro 'a5paper,14pt,oneside' ${bookname}.pdf
	pdf_maker ${bookname}macro-bookbinding 'a5paper,14pt' ${bookname}-bookbinding.pdf
}

build_html()
{
	refreshDirectory $HTML_DIR
	
	re_build_catalog
	cp -f layouts/_layout.html.erb layouts/layout.html.erb

	review-compile -a --stylesheet=stylesheet.cs --target=html
	cp -f stylesheet.css $HTML_DIR

	for file in $(find . -name "*.html");do

		mv ${file} $HTML_DIR

	done

	cp -rf images "${HTML_DIR}/images"
}


refreshDirectory $TEMP_DIR

cp -rf book/* $TEMP_DIR

cd $TEMP_DIR

md2re

case $1 in
	"html") build_html;;
	"pdf") build;;
	"epub") epub_maker;;
	"jenkins") build_jenkins;;
	"release") build_release;;
	*) build;;
esac

cd ../

rm -rf $TEMP_DIR