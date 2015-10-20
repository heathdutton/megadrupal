zeebox Everywhere
=================

This module provides several zeebox widgets as per the API information given on
<https://develop.zeebox.com> and has been sponsored by **zeebox**.

* TV Room
* TV Room Teaser
* Hot TV Rooms widget
* Synchronised Play-along Zone widget
* Follow button

The integration is handled by the Drupal [Bean module](https://drupal.org/project/bean)
which creates block entities that can be used to embed the widgets on your site.

## Requirements

* Drupal core 7.x
* [Bean module](https://drupal.org/project/bean) 7.x-1.x

## Installation

Install and enable the zeebox Everywhere module like any other Drupal module. Place
it uncompressed in your modules directory and enable on your `/admin/modules` page.
For detailed instructions on installing contributed modules see:
<http://drupal.org/documentation/install/modules-themes/modules-7>

## Usage

The module extends the Drupal [Bean module](https://drupal.org/project/bean) so you
will find the new Bean block-types on the `/block/add` page. Once some blocks are
created they can be enabled on the `admin/structure/block` page, where they can be
placed in your Drupal theme regions like normal core Drupal blocks.
For detailed instructions and helpful screencasts on using the Bean module see:
<https://drupal.org/node/1434622>

## Developers

* Global variables should be defined on the admin settings form zeebox_admin_settings()
  which should perhaps be moved to a more appropriate location in `/admin/config`
* There is no separate permission for setting the global variables perhaps there
  should be (if there are more)? For reference see:
  <https://api.drupal.org/api/drupal/modules!system!system.api.php/function/hook_permission/7>
* Shared variables should be placed in the `/zeebox/plugins/bean/shared` abstract
  classes - this should not/ cannot be used directly as plugins, only used by the
  plugins themselves
  * A good example of what could go here is the $message variable
* The abstract classes can be extended by the individual Bean block classes (use
  `parent::values()` to include the shared values) to include individual settings
  * A good example is a $width setting for the TV Room Teaser which is only present
    for this theme template
  * Or maybe the *data-zeebox-family-mode* setting for the TV Room widget (not included)
* When adding new settings these will automatically be available to the templates but
  if they need manipulating, ensure you place all code in the corresponding preprocess
  function and not in the template itself - the template should be for markup only
* We don't need to worry about Drupal adding the javascript file more than once as
  drupal_add_js() handles this internally using the URI as the key in an associative
  array; `$javascript[$options['data']] = $options;`

## Tasks

* [x] Write README file
* [x] Finish initial draft of module
* [x] Create templates for output
* [x] Move duplicated settings to parent (abstract) class
* [x] Clean up and test
* [x] Add comments/ doxygen tips
* [ ] Finish writing SimpleTests
* [x] Create Drupal project page
