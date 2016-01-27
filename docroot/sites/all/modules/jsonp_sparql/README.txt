
============================
JSONP SPARQL Drupal module
============================


DESCRIPTION
-----------
The JSONP SPARQL module enables you to add blocks to your pages containing
information retrieved from a SPARQL endpoint. Its preferred response format
is JSONP, but it can also be used with regular JSON.


INSTALLATION
------------
1. Extract the JSONP SPARQL module tarball and place the entire directory 
  into your Drupal setup (e.g. in sites/all/modules).

2. Make sure you have the libraries module installed.

3. Enable this module as any other Drupal module by navigating to
  administer > site building > modules.
  
4. Download the jQuery JSONP plugin from
  http://code.google.com/p/jquery-jsonp/downloads/list and put it in the
  correct folder. Rename the files to exclude the version number (i.e
  jquery-jsonp.min.js), so that in your libraries folder you have
  'jquery.jsonp/jquery-jsonp.min.js' or 'jquery.jsonp/jquery-jsonp.js'. Or you
  can just navigate to the status report (admin/reports/status) to get
  instructions on how to download the plugin.


CONFIGURATION
-------------
1. The configuration of the module is found under Configuration > 
  Search and metadata > Settings for JSONP SPARQL, and on a per-block basis.
  
2. For an extensive documentation, consult the help section, or the project
  page. You can find the help section by enabling the (core) module "help",
  and navigate to Help > jsonp_sparql.


AUTHORS
-------
Module written by:
  Eirik S. Morland <eirik@morland.no>
