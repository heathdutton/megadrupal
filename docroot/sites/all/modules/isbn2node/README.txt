ISBN2node 1.2 for Drupal 7.x
------------------------------

This is a Drupal 7 module that will create nodes in a book archive based on
ISBN's. ISBN's or International Standard Book Numbers are unique identifiers for
books, and based on ISBN you can mostly gather quite a bit of information about
a book as well as get a hold of its cover. The module will allow users to easily
build archives of physical books based on these book's ISBN's.

The module provides
------------------------------

- a basic ISBN-based book content type (not to confuse with Drupal's own book
    content type).
- a mechanism to fetch the book information from ISBNdb or Open Library through
    their API's. They have information on millions of titles.
- a way to fetch images of the book's covers from OpenLibrary, which has about
    1 million covers available.
- a mechanism to add many books at once through a list of ISBN's.
- a simple form to set up the module, access keys etc.

The content type for this module is called an ISBN2node-book (isbn2node_book) to
separate it from Drupal's built-in book content type. If you want a less nerdy
name for the content type, you can rename it under
admin -> structure -> types -> manage -> isbn2node-book if you please.

The content type contains these fields:

- Title
- Subtitle
- Cover image
- ISBN
- Author
- Publisher
- Edition
- Physical description
- Publishing date

The fields will be matched with fields in the online-databases as best possible.
The databases do not follow a common standard, but have almost similar
information.

Once the module is installed, you can of course extend the content type with your
own fields, add taxonomies, manipulate its display settings, use its fields in
views etc.

Manual addition
------------------------------

You add a book manually by going to node -> add -> isbn2node_book and enter
information as for any node.

ISBN-based addition
------------------------------

You add a book based on ISBN by selecting
node -> add -> isbn2node_book/isbn2node and enter an ISBN. The module will then
try to fetch the information on the book and upon success populate a node form
for the book, which you can then edit (or leave as it is) and save. The book
won't be saved automatically when using the single ISBN form.

The module will check

- whether the book can be found based on ISBN
- whether the book is already in the system based on its ISBN

Failing in either situation will spawn an error and allow you to take action.

Covers will be fetched and shown in the editing form. If you want you can
deselect this cover and add your own with normal file upload.
Bulk import

You can bulk import books by going to node -> add -> isbn2node_book/bulk and
enter a list of ISBN's, one on each line. The module will then create an
ISBN-book for each number without a previous editing option, but skip ISBN's
that give no results and books already in the system based on ISBN.
When bulk importing books you have an option to include cover images in the
same run.

This is the absolutely first instance of this module, and still just a crude
sketch. I have used it intensively to create a register of my own books, and it
basically seems to work as intended.


Title-based addition
------------------------------

The module now supports title search (from version 1.2 and on), and will look
for whole and partial titles and return the list of books found matching a given
title. The first 10 will be shown with ISBN's, authors and a sample of the cover
if possible, and you can then select one and continue to add it as in an
ISBN-search.

This is the absolutely first instance of this module, and still just a crude
sketch. I have used it intensively to create a register of my own books,
and it basically seems to work as intended.


Future
------------------------------

But there are still some caveats and to-do's:

- Support other ISBN-databases
Right now you can choose between ISBNdb and Open Library. I would like to
include more libraries, especially non-US ones. The ideal way to do this would
be through some kind of plugin-architecture. For now the external databases a
re hardcoded into the module, and you can choose either. Plugins would open
for third party development of database support, and allow updates in import
methods without updating the module itself. The plugins should ideally cover
both book data an covers, since there are several sources for both.

- Support several ISBN-databases simultaneously
Now you choose one or the other, but the optimal would be to be able to choose
and prioritize several. That would allow the module to search one, and upon
failure continue to the next. Many books exist in one database and not the
other, and this would potentially give more successes.

- Batch process bulk imports
The bulk import is a one stop process, and will potentially run out of time when
the list of ISBN's imported gets long enough. The immediate solution is to limit
the number of ISBN's allowed per import, but the elegant one would be to use
the batch API to handle large imports.

- No D6 version
There are no plans of a Drupal 6 version of this module. It's time to move on!

The module does not use the already existing CCK-field ISBN. This is available
for Drupal 6 only, and will at least need to be developed into Drupal 7 before
it will work with this D7-only module. Upon a further development of the module,
I may consider supporting it, but since its main task is to validate ISBN's on
input, it's not really needed, but could certainly be nice to have.