------------------------------------------------------------------------------
  sitematcher library Readme
  http://drupal.org/project/sitematcher

  D7 version by David Herminghaus, http://drupal.org/user/833794
------------------------------------------------------------------------------

Contents:
=========
1. ABOUT
2. TECHNICAL
3. QUICK REFERENCE

NOTICE
======

This is not literally a Drupal module, but rather a handy API tool for
multisite installations. It is version independent as for so that you
may apply arbitrary configurations, and it is NOT installed the way modules
usually are. Read this manual and the inline documentation instead.

1. ABOUT
========

A common issue when dealing with Drupal multisite setups is the overhead
in configuring each host environment, maintaining individual sites.php and
settings.php files for any of your multisites, on any host. Inspired by this
http://netzaffe.de/book/export/html/707 nice and smart article, sitematcher
tries to automate there's suggestions (and a bit more).

The basic idea is to prepare global configuration snippets ("sections"),
each of which can be included (optionally based on regex conditions matched
against the actual host name from the request). The actually matching
sections are merged into configuration arrays which can then be used
on request, e.g. by including the sitematcher class in a central settings.php
and then extracting/merging its property arrays into the current variable scope.
Think of the workflow like something somehow similar to e.g. MS' GPO concept.

Sitematcher can actually deliver any setting you may need in settings.php,
sites.php and probably anywhere else. Just include the class file,
or one of the shorthand wrappers, put your individual optional data includes
in /sites/default/sitematcher/includes and go.

2. TECHNICAL
============

Site detection is done in two (three) steps.

1. Internal site name and folder detection
First, the tool tries to identify which site is actually being called.
To achieve this, the actual content of $_SERVER['HTTP_HOST'] is reassembled
just like Drupal does (i.e., optionally prepending any non-80 port like a
subdomain). Then it is matched against the host_regex pattern provided in your
sites.settings.inc (see below) and the first match (if any; your obligation!)
is stored as $sitematcherInstance->internals['site'].

2. Site specific settings
DRUPAL_ROOT/sites/default/sitematcher/includes is browsed for a 
"sites.settings.inc" file containing section definitions. The section keyed
with the site id detected in #1 is parsed into the basic configuration arrays:

	a. $sitematcherInstance->settings
	   Any key of this array should correspond with what you usually set
	   directly in settings.php, e.g. "databases", "cookie_comain" etc.
	b. $sitematcherInstance->internals
	   Any data you would rather not merge into settings.php (mostly
	   includes configuration, matching regex patterns etc.
	c. $sitematcherInstance->conf
	   Lately, more and more addons tend to allow for configuration via
	   settings.php, using the $conf array. This section can e.g. be merged
	   into $conf.

3. Additional settings includes.
By adding strings to the $section['internals']['includes'] array, you can
tell sitematcher to load, match and apply unlimited additional definitions
with regex filters, fallbacks and globals. As this even works recursively,
you will not need to define any settings/configurations detail more than once.
Also, you can easily manage even large multisite without repeating yourself
more than necessary (i.e. never).
Name pattern:

	$section['internals']['includes']['db']
	
will refer to a custom include named
	
	db.settings.inc
	
and so on.

3. QUICK REFERENCE
==================

1. Extract the "module" tarball into DRUPAL_ROOT/sites/all/libraries,
   so that sitematcher.class.inc is located at
   DRUPAL_ROOT/sites/all/libraries/sitematcher/sitematcher.class.inc
2. Use example.sites.settings.inc as a template for your sites configuration
   and save the result in
   DRUPAL_ROOT/sites/default/sitematcher/includes/sites.settings.inc
3. Optionally create custom includes based on example.settings.inc
   and save the result, following the naming convention from 2#3, in e.g.
   DRUPAL_ROOT/sites/default/sitematcher/includes/custom.settings.inc
   (which would be referred with $section['internals']['includes']['custom']).
4. Copy the code snippet from
   DRUPAL_ROOT/sites/all/libraries/sitematcher/sitematcher.settings.php.inc
   into your settings.php or simply include that wrapper file
   (see also inline documentation). I. e. if you want to use sitematcher
   in this context.
5. Copy the code snippet from
   DRUPAL_ROOT/sites/all/libraries/sitematcher/sitematcher.sites.php.inc
   into your sites.php (it may be combined with existing entries, but might
   overwrite one of them - which is desired!)
6. Use instances of DrupalSiteMatcher in whatever other context you may need it.
7. Use the example test.php as a starting point for debugging your own 
   extensions.
