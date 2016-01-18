Glossify.module
---------------

The Glossify module provides filters that scan and parse content and replace terms in the text with
links to their pages. The d7 version is a complete rewrite of the module to be as simple and 
performant as possible. It consists of 2 filters:

-Glossify with taxonomy - links taxonomy terms appearing in content to their taxonomy term page. You
 can select which taxonomy vocabularies to use as the source for the terms.
 
-Glossify with content - links titles of content appearing in other content to their page. You can 
 select which content types to use as the source for the terms.


Installation
------------
- download and install like any other contributed module. See https://drupal.org/node/895232
  for more detailed instructions.
- navigate to admin/config/content/formats.
- click 'configure' for a text format to which you would like to add a glossify filter.
- enable the desired filter in the "Enabled filters" section.
- check the desired settings in the "Filter settings" vertical tab.


Usage
-----
- nothing else to do, if a text field has this an input format with this filter enabled, the desired
  terms will be converted into links automatically.


Theming
-------
The module provides a theme function (theme_glossify_links), some basic css, and 2 icons to format 
the links. Override and customize as desired. See https://drupal.org/node/457740 for more detailed
instuctions.
