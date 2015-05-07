#!/bin/sh

bookname=book

HTML_DIR='../__web'
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

web_maker()
{
	re_build_catalog
	cp -f layouts/_layout.html.erb layouts/layout.html.erb

	review-compile -a --stylesheet=stylesheet.cs --target=html
	cp -f stylesheet.css $HTML_DIR

 	mv -f *.html $HTML_DIR

	cp -rf images "${HTML_DIR}/images"
}

web_watch_maker()
{
	changed_file=$1
	if [ ! -d "${TEMP_DIR}" ];then
				mkdir "${TEMP_DIR}"
				cp -rf book/* $TEMP_DIR
	fi

	string_filename=${changed_file##*/}
	string_filename=${string_filename##*\\}
	string_filename_without_extension=${string_filename%.*}
	string_extension=${changed_file##*.}
	
	if [ "${string_extension}" == "md" -o  "${string_extension}" == "re" ];then
		re_file=${string_filename_without_extension}.re
		orig_file=${re_file}.orig
		cp -f $changed_file "${TEMP_DIR}/${orig_file}"
		cd $TEMP_DIR
		if [ "${string_extension}" == "md" ];then
			md2review $string_filename > $orig_file
		fi
		review-preproc -inencoding=UTF-8 $orig_file > $re_file
	elif [[ "${changed_file}" == unityprojects* ]];then
		cp  --parents -f "${changed_file}" $TEMP_DIR
		replace_region_to_range "${TEMP_DIR}/${changed_file}"
		cd $TEMP_DIR
	else
	   	echo "other files"
		cp -f "../${changed_file}" ${changed_file/book/temp}
		cd $TEMP_DIR
	fi
	preproc
	review-compile -a --stylesheet=stylesheet.cs --target=html
	mv -f *.html $HTML_DIR
	exit
}

change_language()
{
	ruby -e 'Dir.glob("*.re").each { |re| File.write(re, File.open(re).read.gsub( /\/\/emlist\[([^\]]*)?\](\[#{Regexp.quote(ARGV[0])}\])/) { |w| w.gsub($2, "[#{ARGV[1]}]")}) }' $1 $2
}

re_build_catalog()
{
	CATALOG=catalog.yml
	cp -f ${CATALOG}.template $CATALOG

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
	refresh_temp
	re_build_catalog
	pdf_maker ${bookname}macro 'a5paper,14pt,oneside' ${bookname}.pdf
	refresh_temp
	re_build_catalog
	pdf_maker ${bookname}macro-bookbinding 'a5paper,14pt' ${bookname}-bookbinding.pdf
}

build()
{
	pdf_maker ${bookname}macro 'a5paper,14pt,oneside' ${bookname}.pdf
}

build_release()
{
	cp -f catalog-release.yml catalog.yml
	epub_maker
	refresh_temp
	cp -f catalog-release.yml catalog.yml
	pdf_maker ${bookname}macro 'a5paper,14pt,oneside' ${bookname}.pdf
	refresh_temp
	cp -f catalog-release.yml catalog.yml
	pdf_maker ${bookname}macro-bookbinding 'a5paper,14pt' ${bookname}-bookbinding.pdf
}

preproc()
{
	for file in $(find . -name "*.re");do
		cp -rf $file ${file}.orig
		review-preproc -inencoding=UTF-8 ${file}.orig > $file
	done
}

create_temp()
{
	refreshDirectory $TEMP_DIR
	cp -rf book/* $TEMP_DIR
	cd $TEMP_DIR
	md2re
	copy_unityproject_sources
	preproc
}

remove_temp()
{
	cd ../
	rm -rf $TEMP_DIR
}

refresh_temp()
{
	remove_temp
	create_temp
}

copy_unityproject_sources()
{
	cd ../
	find unityprojects -name "*.cs" | while read file;do
		cp  --parents -f "${file}" $TEMP_DIR
		replace_region_to_range "${TEMP_DIR}/${file}"
	done
	cd $TEMP_DIR
	
}

replace_region_to_range()
{
	path=$1
	sed -ie 's/#region\s\(.*\)/\/\/#@@range_begin(\1)/gm' "${path}"
	sed -ie  's/#endregion\s\(.*\)/\/\/#@@range_end(\1)/gm'  "${path}"
}

case $1 in
	"web_watch") web_watch_maker $2;;
esac

refreshDirectory $TEMP_DIR
create_temp

case $1 in
	"web") web_maker;;
	"pdf") build;;
	"epub") epub_maker;;
	"jenkins") build_jenkins;;
	"release") build_release;;
	*) build;;
esac