-- SUMMARY --

The Book Rules module provides additional integration between the Book module 
in Drupal core and the Rules module, exposing the ability to loop over every 
node within a Book from a Rule.

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/book_rules


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.


-- CONFIGURATION --

None.


-- USAGE --

After enabling this module, a new Rule Action is available, "Get all the pages 
in a book".
This takes as a parameter the Book from which to retrieve pages.
The action returns as a provided variable a list of nodes which belong to the 
book.


-- EXAMPLE --

The following Rule, when triggered, will loop through all pages in a book and 
set the author of each page to the same value as the author of the book.

+ Get all the pages in a book
	Parameter: Book: [node]
	Provides variables: A list of pages in the book (book_pages)
+ Loop
	Parameter: List: [book-pages]
	List item: Current book page (book_page)
	+ Set a data value
		Parameter: Data: [book-page:author], Value: [node:author]
	+ Save entity
		Parameter: Entity: [book-page], Force saving immediately: true


-- CUSTOMIZATION --

None.


-- CONTACT --

Current maintainers:
* Benjamin Bradley (benjaminbradley) - https://www.drupal.org/user/395294

This project has been sponsored by:
* Autodesk University Workshop
  The AU Workshop helps designers & engineers learn new skills and do better 
  design work using Autodesk tools.
  Visit http://auworkshop.autodesk.com/ for more information.

* Polycot Associates
  Mission-driven technology mavens.
  Visit http://polycotassociates.com for more information.
