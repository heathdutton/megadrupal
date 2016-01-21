@file
README.txt file for Weblinks.


README.txt file
===============
Web Links provides a comprehensive way to post links on your site. All links are nodes, which can be put into taxonomies/categories and administered. The most common options are set by default so the module should work well right out of the box. However, there is a great deal of customization that may also be done to make the installation fit your needs. Additionally, a variety of blocks may be enabled (see below).

The Link Validity Checker feature will check links during a Cron run to see if they are still valid. You may have the module update any links that are marked as moved and unpublish links that have returned an error twice in a row.

A separate filter module (distributed with the package) may be used to easily insert links into other content.


Dependencies
============

The core taxonomy module must be enabled.

Incompatibilities
=================

Category module appears to be incompatible with this module.

The Links module's database is incompatible and will not work with Weblinks.

Installation and Settings
=========================

The most up-to-date information on the module can be found at:
http://drupal.org/node/273907

Recent Changes
==============

It was brought to our attention that there really was no way to control who has access to the main
links page. We have added a new permission "access web links" to address this oversight. You will
need to set this for any roles you wish to access Web Links.

Upgrade from Weblinks 6.x to 7.x
================================

Weblinks 6.x used the node sticky attribute to store an encoded weight value for
ordering links. This feature has been removed in 7.x as it corrupted the sticky
value for non-weblinks nodes. The update function weblinks_update_7002() fixes
the incorrectly stored sticky values.
See issue http://www.drupal.org/node/2030765 for more details.

To show which nodes will be corrected, you may run the following Drupal code from a php window:

$query = db_select('node', 'n')->fields('n', array('nid', 'type', 'sticky', 'title'));
$query->condition('n.sticky', 0, '<>')->condition('n.sticky', 1, '<>')->orderBy('nid', 'ASC');
foreach ($query->execute() as $sticky) {
  drupal_set_message(print_r((array)$sticky, TRUE));
}

For better control of link order you can use the Weight module available at http://www.drupal.org/project/weight

Images in 7.x
=============

In 7.x weblinks uses the built-in core image functionality to display images.
To use this, enable the Image module and add one (or more) image fields to the
Web Links content type. To display an image in a Weblinks block turn on the
option in the block's configuration page and select an image style. We provide
a default style of 150x150 but you can override this or add more styles to use
on different blocks. If you have more than one image field attached to the
Weblinks content type then the block will use the first field with 'block' in
the field name, or failing that, the first with 'block' in the field label. If 
you have multiple images attached to that field only the first will be displayed.

Alexa and Google Pagerank information removed from 7.x
======================================================
The 'Pralexa' sub-module has been removed from Weblinks 7.x
See https://www.drupal.org/node/2182163 for more details.

Taxonomy Image
==============

The Taxonomy Image module http://www.drupal.org/project/taxonomy_image will not
be supported in Weblinks 7.x. For details see http://www.drupal.org/node/2415145
