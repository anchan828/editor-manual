require 'fileutils'

info_path = "./info.txt"

File.open(info_path).readlines.each { |line|
  names = line.gsub("\n", "").split(",")
  book_name = "#{names[0]}_unity-editor-extension"
  system("./Darwin-cpdf -add-text '#{names[0]} #{names[1]}' -color '0.8 0.8 0.8' -bottom 10pt ../archives/book.pdf 2-end -o '#{book_name}.pdf'")

  move_to = "./books_in_dropbox/#{names[0]}"
  system("mkdir '#{move_to}'")
  system("mv -f '#{book_name}.pdf' '#{move_to}'")


  system("mkdir epub")
  system("cd epub;unzip '../../archives/book.epub'")



  Dir.glob("epub/**/*.opf").each{ |file|
    text = File.open(file).read.gsub("</dc:title>"," (#{names[0]} #{names[1]})</dc:title>")
    File.write file, text
  }
  system("cd epub;zip -0 -X '../books_in_dropbox/#{names[0]}/#{book_name}.epub' mimetype")
  system("cd epub;zip -r '../books_in_dropbox/#{names[0]}/#{book_name}.epub' * -x mimetype")
  system("rm -rf epub")

  system("cd '#{move_to}';zip '#{book_name}.zip' '#{book_name}'.*")

}
