
-- SUMMARY --

The Drupal module LinearCase allows ClosedQuestions and other documents to be
ordered in a "case study". The user is not allowed to navigate further than the
last question he has not yet answered correctly, but is allowed to navigate
backwards freely.


-- REQUIREMENTS --

LinearCase depends on the core module Book and the contrib module ClosedQuestion.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* None needed


-- USAGE --

A linearcase is a book that limits a students navigation. A student can progress
through the book freely, untill he reaches a page with a question. The student
then has to answer the question correctly before he can go to the pages beyond
that question.

To convert a Book to a Linear Case the top-node of the book has to be replaced by
a node of type LinearCase. The easiest way to do that is:

1. Create a node of type LinearCase, give it a clear name, and add it to the book.
2. Go to Outline -> "reorder an entire book" -> "edit order and titles" of the
   concerning book.
3. Drag all nodes so they are children of the LinearCase node.
4. Save the new outline.
5. Go to "edit" page of the LinearCase node.
6. Go to the section "Book outline" and choose <create a new book>
   This will make the LinearCase node the top-node of a new book, with all it's
   child pages in it.
7. Remove the old top-node, or if it has content, move it to be the first child
   of the LinearCase node.
8. Add the block "Case Navigation" to your preferred space for navigation.

It's important that students have no other way of navigating to individual pages
in the case, so you should probably not show the "Book Navigation" block on
"All Pages" but "only on Book pages", otherwise students can use that block to
jump directly to any page in the case. I hope to fix that in a later version.


-- TODO --

 * Make a TODO list.


-- CONTACT --

Current maintainers:
* Hylke van der Schaaf - hylke.vanderschaaf@wur.nl
