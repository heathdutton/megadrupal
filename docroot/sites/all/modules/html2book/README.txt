HTML2BOOK README.txt
====================


CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Use
* Example
* Configuration
* Maintainers


INTRODUCTION
------------

The HTML2Book module allows you to create multi-page books by
splitting the body text of a book node into multiple nodes upon save.
The split points is based upon HTML heading tags.

This makes it possible to create a book in a word processing program,
export the result to HTML, and then use this HTML to create a
multipage Drupal book in a single step.


INSTALLATION
------------

1. Extract the HTML2Body project directory into the directory where
   you keep contributed modules (e.g. sites/all/modules/).

2. Enable the HTML2Body module on the Modules list page.

3. Navigate to the Admin » People » Permissions page and give the
   roles you will grant access to create multi-page books by means of
   this module the permission "Use HTML2Book".


USE
---

The module adds a field group named "HTML2Book Splitter" below book
page "Body" field.

Expanding this field group allows the author to create a new book page
for each HTML heading in the body text.

Each new book page will have the same author, categories, settings,
and other characteristics of the original page.

All text before the first HTML heading will be retained as the body of
the original (book root) page. Subsequent pages will be added as
children of that page, using the heading as their title and all text
from that point to the next heading as their body. Child pages will be
nested based on the subheading number, provided the subheadings are
logically organized.

You can choose which heading levels will be used to create new pages.
You may, for example, create new pages only when a <h1> heading is
encountered, or make a new page at every header.

If Organic Groups is used and the original page has been assigned to
one or more groups, all child book pages will belong to the same
groups.

If the source is a word processing document, be sure it has been saved
as html rather than as a word processing document so the HTML2Book
module can locate the headings in the text. Microsoft Word documents
pasted into the body should come from text first saved as 'Filtered
HTML' for best results. Badly structured documents will yield
unpredictable results, clean and simple html will work the best.

For best results, combine with Html Corrector and HTML Tidy modules.
When setting up HTML Tidy, choose the option to clean up Microsoft
Word text if the source is Microsoft Word.


EXAMPLE
-------

Node 1 Title: My Book

Here is my page.  This text is the introduction.

<h1>Page 1</h1>

<p>Here is the first line of my text for page 1.</p>
<p>Here is the second line of my text for page 1.</p>

<h2>Page 1a</h2>

<p>This is page 1a.</p>

<h2>Page 1b</h2>

<p>This is page 1b.</p>

<h1>Page 2</h1>

<p>This is page 2 and the last line of text.</p>

Will create the following book pages:

Node 1 Title: My Book
Node 1 Body: <div>Here is my page.</div>
Node 1 Parent: <top level>
Node 1 Weight: -15

    Node 2 Title: Page 1
    Node 2 Body: <p>Here is my text for page 1.</p>
    Node 2 Parent: Node 1
    Node 2 Weight: -15

       Node 3 Title: Page 1a
       Node 3 Body: <p>This is page 1a.</p>
       Node 3 Parent: Node 2
       Node 3 Weight: -15

       Node 4 Title: Page 1b
       Node 4 Body: <p>This is page 1b.</p>
       Node 4 Parent: Node 2
       Node 4 Weight: -14

    Node 5 Title: Page 2
    Node 5 Body: <p>This is page 2.</p>
    Node 5 Parent: Node 1
    Node 5 Weight: -14


CONFIGURATION
-------------

The module has no menu or modifiable settings.  There is no
configuration.  When enabled, the module will add the field group
named "HTML2Book Splitter" below book page "Body" field.  To removed
this field group, disable the module and clear caches.


CREDITS
-------

KarenS (https://drupal.org/user/45874) is the original author.
Gisle Hannemyr <gisle@hannemyr.no> is the current maintainer.

Any help with development (patches, reviews, comments) are welcome.

This project has been sponsored by Hannemyr Nye Medier AS
Visit: http://hannemyr.com/hnm/ for more information.
