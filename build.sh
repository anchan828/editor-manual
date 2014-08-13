cd book


build_dir='__build'

mkdir $build_dir


predef=$(find predef -name "*.re")
chaps=$(find chaps -name "*.re")
postdef=$(find postdef -name "*.re")

for file in ${predef} ${chaps} ${postdef};do

ln ${file} $build_dir/

done

rm -rf ${build_dir}/images
cp -r images ${build_dir}



if [ "$1" = "html" ];then

cd $build_dir
rm ./*.html
review2html -a
rm ./*.re
fi

if [ "$1" = "pdf" ];then

cp -r sty ${build_dir}
cp catalog.yml ${build_dir}/catalog.yml
cp config-pdf.yml ${build_dir}/config.yml

cd $build_dir

review-pdfmaker config.yml
mv book.pdf ../book.pdf
cd ../
rm -rf $build_dir
fi



