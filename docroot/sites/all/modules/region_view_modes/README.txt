Region View Modes
=================

A simple way to place fields in regions.

Features
--------

Region View Modes is a very simple module that provides a surprising amount of
functionality thanks to Drupal's View Modes.

- Allows fields for any content type to be placed in sidebars, the header,
the footer, or any other theme region.
- Allows fields to be displayed in multiple regions. For example, an image
could be displayed using a large image style in the main content area and as a
thumbnail in one of the sidebars.
- Fields in regions can be reordered via drag and drop
- Field Groups can be used in regions.
- Region view modes are exportable into Features.

How to use
----------

- Download and enable this module (or test it out now!)
- Visit the Manage Display page for any content type (e.g. Article)
- Expand the Custom Display Settings section
- Check the view mode for one or more theme:region combinations. For example,
Bartik theme: Sidebar first region
- Click Save at the bottom of the page
- You'll now see the activated view mode(s) near the top of the page. Click on
one.
- Reorder, hide, or change the settings for any fields
- View a node of that content type

How it works
------------

Region View Modes creates view modes for the regions of active themes. It will
run the current node through each activated view mode and place the rendered
node into the corresponding theme region.

Template suggestions are provided so you can use a simplified node.tpl.php
template for nodes displayed in regions. This makes it easy to avoid the
inclusion of page titles, submitted by information, comments, and links.

Hiding regions from region view modes
-------------------------------------

You may optionally declare regions that you don't want available as region view
modes.

If you are using a custom theme, you can add a custom key to your .info file to
exclude a region from the view mode listing under Custom Display Settings.

For example, if your theme declares a region like so:

    regions[triptych_last] = Triptych last

You may hide it from the available region view modes by adding this:

    region_view_modes_hidden[] = triptych_last

To hide regions from other themes, you can implement hook_system_info_alter() in
a custom module to add the `region_view_modes_hidden` key. You can see an
example in region_view_modes_system_info_alter(), which is used to hide the
content region from any theme that has one.

Similar modules
---------------

Display Suite - A very powerful suite of modules that allow the creation of
custom view modes. It can even do something similar to region view modes when
using its region to block functionality.

Known issues
------------

- View modes appear in places where you wouldn't want users to see them, like
the Content row style in Views, and probably in some Entity Reference
configuration.
