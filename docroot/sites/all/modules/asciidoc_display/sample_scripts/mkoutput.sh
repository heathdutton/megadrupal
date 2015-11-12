# This sample Linux shell script takes the Core Docs project's documentation and
# makes it into a special form of bare XHTML output that this project's module
# can read and display. See the README.txt one directory up for more information
# about this project's Drupal module and how to install the required packages.
#
# You will probably need to modify the script slightly so it can find the
# AsciiDoc input you want to display. In addition, if you are making more than
# one book, you will need to have a separate output directory for each book.

# Make the output directory if it does not exist. Copy this line for each book
# you want to display, putting each in its own output directory.
mkdir -p ../../my_output_directory

# Run the preprocessor that puts file names into the files under each header.
# This is necessary for the source file editing functionality. Copy this line
# for each book you want to display.
php addnames._php ../../my_input_directory ../../my_output_directory

# Run the AsciiDoc processor to convert to DocBook format. The syntax is:
#   asciidoc -d book -b docbook -f [config file] -o [output file] [input file]
# Here, [input_file] is the overall file that includes the rest of the book
# files, and [output_file] is a .docbook file. Copy this line for each book
# you want to display.
asciidoc -d book -b docbook -f std.conf -o ../../my_output_directory/my_book_name.docbook ../../my_output_directory/my_book_name.txt

# Run the xmlto processor to convert from DocBook to bare XHTML, using a custom
# style sheet that makes output this module can recognize.  The syntax is:
#   xmlto -m bare.xsl xhtml -o [output dir] [input docbook file]
#  Copy this line for each book you want to display.
xmlto -m bare.xsl xhtml -o ../../my_output_directory ../../my_output_directory/my_book_name.docbook
