
CONTENTS OF THIS FILE
---------------------

 * Overview
 * Installation
 * Usage

OVERVIEW
--------

The purpose of this module is to provide a way to move site content data
between Drupal instances.

Generally a Drupal site design and structure will be built by web developers.
When the site is being used as a live site content will be added by site
visitors and/or site editors.  This content data is usually in the form of
nodes but could be taxonomy terms. Also, the list of site users can be built
up over time as the site is used and this can be thought of as part of the
content data.

So, we can think of a Drupal site as made up of two broad parts; structure and
content.

It would be extremely useful to be able to export the content data to files
which can be re-imported into other Drupal instances.

One important usage would be in deploying a new version of a web site.  The
new site could be built up as a beta site and can have a very different
structure from the original live site.  When the beta site is ready to be
deployed the content data can be exported to data files which are then
imported into the beta site which becomes the new live site.  This way a new
site can be deployed which contains content data from the original site.

There will be other uses for this module in terms of testing, backup sites,
data deployment etc.

This module will enable the export and import of the following standard
content data:

 * Nodes
 * Taxonomy terms
 * Users

As Drupal can be extended and customised there may be other site content which
needs to be exported/imported.  This module will implement hooks to allow
other modules to extend this module to export/import any other content data.

Points to note:

 * This module will use the Druapl API to read data and produce rich data
   files which will contain all data needed to reproduce a particular
   dataset. It will also use the Drupal API to recreate data.

 * The module will carry out checks when importing datasets.  For example,
   when node data is being imported it will be necessary to check that the
   receiving site has the correct content type with the correct fields etc.

 * The receiving instance will end up with content data which exactly matches
   the data which was exported.  This means that ID numbers will match and
   that existing data on the receiving site may be deleted if it is not part
   of the imported dataset.

INSTALLATION
------------

 * Put the module in your Drupal modules directory and enable it in
   admin/build/modules.

 * Go to admin/user/permissions and grant permission to any roles that
   need to be able to export or import datasets.

USAGE
-----

 * Configure and use the module at admin/config/system/data_export_import.

 * The dataset files are exported to and imported from
   {files}/data_export_import.  Various methods can be used to manage the
   files and to move the files between Drupal instances. Although not tested,
   the module at: http://drupal.org/project/webfm looks suitable for file
   management.

 * The user running the module needs to have permissions to be able to delete
   nodes when importing pages.  It is expected that data exporting and
   importing will be carried out by an admin user.

 * If nodes are associated with taxonomy terms then it is best to import the
   taxonomy terms first to make sure they are available for the nodes.

 * Similarly, it would be best to import the users before importing nodes to
   make sure they are available if nodes link to them.

 * All nodes, taxonomy terms and users are imported to have exactly the same
   ID numbers as when they were exported.  This means that existing data may
   be deleted.  The import acts in the same way as rsync when the --delete
   flag has been set.  The idea is that the site which imports the data ends
   up with an exact copy of the data as was held on the site which exported
   the data.

NOTES
-----

 * This module is not compatible with any site which has localisation
   enabled. Checks have been put in place to prevent data being output if
   localisation has been installed and enabled using the i18n module.  If this
   is a requirement then please get in touch with the maintainer.
