#!/bin/sh

bookname=book

BOOK=book
BASE_PATH=$(cd $(dirname $0) && pwd)
BOOK_DIR="${BASE_PATH}/${BOOK}"
TEMP_DIR="${BASE_PATH}/temp"
ARCHIVE_DIR="${BASE_PATH}/archives"

pdf_maker()
{
	cd $TEMP_DIR
	change_language  "cs" "[Sharp\\]C"
	cp -f config.yml.template config.yml
	echo "texstyle: $1" >> config.yml
	echo "texdocumentclass: [\"jsbook\", \"$2\"]" >> config.yml
	review-pdfmaker --ignore-errors config.yml
	archive ${bookname}.pdf "${ARCHIVE_DIR}/"$3
	change_language  "[Sharp\\]C" "cs"
}

epub_maker()
{
	cd $TEMP_DIR
	change_language "cs" "c#"
	review-epubmaker config.yml.template
	archive ${bookname}.epub $ARCHIVE_DIR
	change_language "c#" "cs"
}

archive()
{
	from=$1
	to=$2

	if [ -e $from ];then
		mv $from $to
	fi
}

web_maker()
{
	cd $TEMP_DIR
	re_build_catalog
	cp -f layouts/_layout.html.erb layouts/layout.html.erb
	review-compile -a --stylesheet=stylesheet.css --target=html
	rm -f  layouts/layout.html.erb
}

web_watch_maker()
{
	changed_file=$1
	string_filename=${changed_file##*/}
	string_filename=${string_filename##*\\}
	string_filename_without_extension=${string_filename%.*}
	string_extension=${changed_file##*.}
	
	if [ "${string_extension}" == "md" -o  "${string_extension}" == "re" ];then
		re_file=${string_filename_without_extension}.re
		
		if [ "${string_extension}" == "md" ];then
			md2review $string_filename > "${TEMP_DIR}/${re_file}"
		else
			cp -f $changed_file  "${TEMP_DIR}/${re_file}"
		fi
		preproc_re_file "${TEMP_DIR}/${re_file}"
	fi
	
	web_maker
}

change_language()
{
	ruby -e 'Dir.glob("*.re").each { |re| File.write(re, File.open(re).read.gsub( /\/\/emlist\[([^\]]*)?\](\[#{Regexp.quote(ARGV[0])}\])/) { |w| w.gsub($2, "[#{ARGV[1]}]")}) }' $1 $2
}

re_build_catalog()
{
	cd $TEMP_DIR
	CATALOG=catalog.yml
	cp -f ${CATALOG}.template $CATALOG

	echo "\nCHAPS:" >> $CATALOG

	for file in $(find . -name "*.re");do
		conti=1
		for c in $(cat ${CATALOG});do
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

remove_md2re_files()
{
	for file in $(find . -name "*.md");do
		rm ${file%.*}.re
	done
}

build_release()
{
	cp -f catalog-release.yml catalog.yml
	build
}

build()
{
	epub_maker
	pdf_maker ${bookname}macro 'a5paper,14pt,oneside' ${bookname}.pdf
	pdf_maker ${bookname}macro-bookbinding 'a5paper,14pt' ${bookname}-bookbinding.pdf
}

# We still need this.
windows() { [[ -n "$WINDIR" ]]; }

# Cross-platform symlink function. With one parameter, it will check
# whether the parameter is a symlink. With two parameters, it will create
# a symlink to a file or directory, with syntax: link $linkname $target
link() {
    if [[ -z "$2" ]]; then
        # Link-checking mode.
        if windows; then
            fsutil reparsepoint query "$1" > /dev/null
        else
            [[ -h "$1" ]]
        fi
    else
        # Link-creation mode.
        if windows; then
            # Windows needs to be told if it's a directory or not. Infer that.
            # Also: note that we convert `/` to `\`. In this case it's necessary.
            if [[ -d "$2" ]]; then
                cmd <<< "mklink /d \"${1//\//\\}\" \"${2//\//\\}\"" > /dev/null
            else
                cmd <<< "mklink \"${1//\//\\}\" \"${2//\//\\}\"" > /dev/null
            fi
        else
            # You know what? I think ln's parameters are backwards
            ln -s "$2" "${1//\//}"
        fi
    fi
}

rmlink() {
    if windows; then
        # Again, Windows needs to be told if it's a file or directory.
        if [[ -d "$1" ]]; then
            rmdir "$1";
        else
            rm "$1"
        fi
    else
        rm "$1"
    fi
}

setUp()
{
	workspace=$1
	remove_temp_files
	symlink $workspace
	copy_files $workspace
	re_build_catalog
	preproc_re_files $workspace
}

symlink()
{
	workspace=$1
	
	if [ ! -d "${workspace}" ];then
		mkdir "${workspace}"
	fi
	cd $BOOK_DIR
	dirs=`ls -F | grep /`

	cd $workspace
	
	for dir in $dirs;do
		if [ ! -d "${dir}" ];then
			link "${dir}" "${BOOK_DIR}/${dir}"
		fi
	done
}

copy_files()
{
	workspace=$1
	cd $BOOK_DIR
	array=`ls -F | grep -v /$`

	for file in $array;do
		cp -f $file $workspace
	done
}
remove_temp_files()
{
	cd $TEMP_DIR
	array=`ls -F | grep -v /$`
	for file in $array;do
		rm -f $file
	done
}
preproc_re_files()
{
	cd $1
	md2re
	
	for file in $(find . -name "*.re");do
		preproc_re_file $file
	done
	
	remove_md2re_files
}

preproc_re_file()
{
	review-preproc -inencoding=UTF-8 --replace $1
}

if [ ! -d "${ARCHIVE_DIR}" ];then
			mkdir "${ARCHIVE_DIR}"
fi

setUp $TEMP_DIR
case $1 in
	"web") web_maker;;
	"web_watch") web_watch_maker;;
	"epub") epub_maker;;
	"jenkins") build;;
	"release") build_release;;
	*) pdf_maker ${bookname}macro 'a5paper,14pt,oneside' ${bookname}.pdf;;
esac