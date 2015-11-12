
Module description
==================

This is a module that allows a user with the specific admin permissions to
search for taxonomy terms across defined taxonomy vocabularies via a simple
text box. For sites with vocabularies with a large number of terms (1000s)
this makes it easier to find a taxonomy term and edit the term than using
the standard Drupal core taxonomy term browsing pages at /admin/structure/taxonomy/{$vocabulary_machine_name}

Installation and Configuration
==============================

1) Copy the Taxonomy Find module files to your Drupal modules 
   directory (e.g. /sites/all/modules)

2) To install, enable the Taxonomy Find module on the Drupal
   modules page /admin/build/modules

3) Grant the 'administer taxonomy find' permission to the
   roles desired on the Drupal user permissions page
   /admin/people/permissions. Users with this permission can 
   change basic settings of the Taxonomy Find module.

4) Grant the 'access taxonomy find' permission to the
   roles desired on the Drupal user permissions page
   /admin/people/permissions. Users with this permission can 
   search for taxonomy terms at the path /admin/structure/taxonomy/search

5) Once the Taxonomy Find module has been installed you can configure
   the settings at /admin/config/content/taxonomy_find

  a) You can set the number of results per page to show when showing 
     taxonomy terms that match the search string

  b) If you want to show the number of nodes that have that taxonomy
     terms in the results, select the checkbox for that option

  c) If you wish to exclude any taxonomy vocabularies from being search
     you can select them here.

6) Go to /admin/structure/taxonomy/search to search for taxonomy terms. You
   can restrict the search for a single taxonomy vocabulary or search across
   all vocabularies (with any vocabularies to always be excluded set in 
   the module configuration page).


Project Owner
Jeff Warrington http://drupal.org/user/46257
