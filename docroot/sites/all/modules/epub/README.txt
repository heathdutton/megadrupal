
ePub
----

NOTE: Install the advanced_help module (http://drupal.org/project/advanced_help)
to access more help (writing still in progress...)

To install, place the entire ePub folder into your modules directory.
Go to Administer -> Site building -> Modules and enable the following modules:

- book.module
- content.module
- iso_639.module

Other than the module and its dependencies you will need to download
three php classes(EPub.php, EPubChapterSplitter.php and Zip.php) and put them
into sites/all/libraries/epub directory(that will result in a path like
/path/to/your/drupal/installation/sites/all/libraries/epub/EPub.php).
You can download a copy of the three needed files here:

- http://code.nois3lab.it/epub/epub.zip

Or fetch all EPub Classes and examples from the original website here:

- http://www.phpclasses.org/browse/package/6110.html

If everything went well, you should now be able to create a new ePub content:
try by clicking on 'Create content' in the navigation menu; you should see an
EPub type among other content types, if so it means you're ready to go.
The creation should be pretty straightforward, the only mandatory fields for an
EPub content are the title, the language (which defaults to english) and the name
of the book outline your ePub is referring to (which features an autocomplete
field for your convenience). If now you go to Home -> Administer -> Content management
you should be able to administer and download your ePub files.

As soon as an ePub content is created and linked to a book outline, it will
automatically become available as download in the ePub and in the book outline
pages thanks to a new tab in the menu reporting 'Download ePub'.

Notes: If you will append some other book pages to the first one you created before,
they will be available in the ePub as well. Keep in mind that you can create an ePub
only on book outlines.

Maintainers
-----------
The ePub was originally developed by:
Claudio Beatrice

Current maintainers:
Claudio Beatrice


Sponsored by
____________

nois3lab (cc) 2001
http://nois3lab.it