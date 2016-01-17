This module provides some additional features and the ability to set default
settings for content type options, such as 'Promoted to front page', 'Display
author and date information', etc.

Added features:
  Set default values for common content type settings (see below) - you can
    still override the defaults per content type
  'Save and New' button to add new nodes of the same type right after saving
  User permissions for content type duplicated on content type add/edit page

Set defaults for:
  Title field
  Preview button
  'Save and New' button
  'Save and New' button text
  Published
  Promoted to front page
  Sticky at top of lists
  Create new revision
  Display author and date information
  User permissions

This module is basically things that I find I always need to change (or would
like to have a default value set instead of Drupal's default) in content types
for my development. It is still rough and can use some improvements.

Submit any feature requests/bugs to the issue queue!

Installation
============
  Install as you would any other normal module.
  Navigate to admin/structure/types/defaults to setup your defaults (this
    step is not necessary if you like all of Drupal's default settings).
  When viewing a content type add/edit page you will have the additional
    options presented to you (listed in 'Added features' above) and your
    defaults will be applied (if you are creating a new content type and
    you have setup customized defaults).

Maintainer
==========
Jacob Neher (bocaj)
