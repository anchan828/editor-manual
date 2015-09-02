require 'fileutils'

info_path = "./info.txt"

File.open(info_path).readlines.each { |line|
  names = line.gsub("\n", "").split(",")
  book_name = "#{names[0]}_unity-editor-extension.pdf"
  system("./Darwin-cpdf -add-text '#{names[0]} #{names[1]}' -color '0.8 0.8 0.8' -bottom 10pt ../archives/book.pdf 2-end -o '#{book_name}'")

  move_to = "./books_in_dropbox/#{names[0]}"
  system("mkdir '#{move_to}'")
  system("mv -f '#{book_name}' '#{move_to}'")
}