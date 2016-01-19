/** What this module does **/

This module is meant to provide the INITIAL installation for a site's
wysiwyg - to save you a couple of hours of checking checkboxes and thinking what
on earth you did last time.

1.  It forces you to install some modules you might otherwise forget (imce, 
    imce mkdir). You can remove the dependencies from the wysiwyg_setup.info 
    file if you don't want them.
2.  It sets up the "html filter" and "html full" text formats, including buttons
    configuration and wysiwyg_filter configuration.
3.  This works with ckEditor and drupal 7.


/** Important - BEFORE ENABLING THE MODULE **/

1. Wysiwyg module version

As of the creation of this module (Jan 9, 2012), you need to use the dev version
of the wysiwyg module, or apply the patch from here:
http://drupal.org/node/624018
That enables us to export wysiwyg configs info features.

2. Having CKeditor

Download ckEditor and place it in sites/all/libraries before enabling the 
module.


/** Setup **/

After enabling the module:

1.  Configure imce (user roles).
2.  Edit wysiwyg.css - so that it has the styles you use on your site. This will 
    let the user see the content like it appears on the site when editing.



/** files **/

- wysiwyg_styles.js
  Overrides the styles dropdown in ckEditor, mostly in order to set rtl and ltr.
  This gets called from the module file.

- wysiwyg.css
  A stylesheet for ckEditor.
  This should be edited to match the site's styles.
