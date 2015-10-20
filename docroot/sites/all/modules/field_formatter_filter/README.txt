
Field Formatter Filter
======================

Extends text field formatter settings to allow you to choose a different 
"text format" or text filter at the same time as the normal choices of 
"full" or "summary".

Use case
--------

Use it to

* Create super-simple teasers by removing block elements and headings.

* Use special enhancement filters like toc, tabs, glossary or chunker
  only in very specific view modes or for specific content types - in a way
  that does not require the editor to manage the text formats.
  
Usage
-----

Once installed, additional settings can be found when editing an entity types
field settings, eg at /admin/structure/types/manage/page/display
If you choose "default", "trimmed", or "summary or trimmed", on a longtext
field, there will be additional options in the settings for that field.
 
Caveats
-------
Running two text filters one after the other can have unexpected side effects,
so it's recommended that the per-filter format be pretty simple.


Remainder display mode for text-with-summary fields
===================================================

Additionally, there is a new field_formatter for the text-with-summary 
field type. 'Remainder after trimming' can be used to display just the
leftovers of a body text area after a teaser summary has been removed.
It's the reciprocal of 'Summary or trimmed'

Use case
--------
The formatter may be used to create more elaborate layouts - eg with 
display_suite to show the body teaser in one region and the body remainder in 
another, possibly split by an image or for alignment.

Usage
-----
To make that work, you may use display_suite extras to create a 'dynamic field'
that duplicates the node body field, and lets you use teaser and anti-teaser 
as displays. 
You will need to ensure that the trim length in both cases is identical.
