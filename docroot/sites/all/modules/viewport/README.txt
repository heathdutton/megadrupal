
CONTENTS OF THIS FILE
---------------------

 * INTRODUCTION
 * INSTALLATION
 * USAGE
 * ROADMAP


INTRODUCTION
------------

Current Maintainer: Salvador Molina <salvador.molinamoreno@codeenigma.com>

The Viewport module is a simple module that allows administrators, or users
with the "Administer Viewport Settings" permission, to set a Viewport HTML
metatag with the desired properties for one or several pages that can be
configured from the Settings page of the module.

The aim of the module is to provide an easy way to debug or test websites or
apps, as well as responsive designs on smartphones and tablets. This is *NOT* a
complete suite of tools to work with when delivering this kind of design / apps,
but a small utility to help with one specific aspect of responsive design.

Sometimes, one may need to set specific viewport values for a specific page on
the site (e.g when embedding a game for smartphones / tablets). This tool
helps to chase easily situations like that.

INSTALLATION
------------

To install the Viewport module:

 1. Place its entire folder into the "sites/all/contrib/modules" folder of your
    drupal installation.

 2. In your Drupal site, navigate to "admin/modules", search the "Viewport"
    module, and enable it by clicking on the checkbox located next to it.

 3. Click on "Save configuration".

 4. Enjoy.

USAGE
-----

After installing the module:

  1. Navigate to "admin/people/permissions" and assign
     the "Administer Viewport Settings" permission to the desired roles.

  2. Navigate to "admin/config/user-interface/viewport", and set the desired
     values for the different viewport properties.

  3. In the textarea provided, enter the paths (oner per line) for which you
     want the viewport tag to appear.

  4. For more information on the Viewport properties and their meanings,
     navigate to "admin/help/viewport".


ROADMAP
-------

In its actual state, the Viewport module only allows to set the same viewport
values for one or several pages, but not different values for different pages.
This is a feature that is not planned to be added to the core module at the time
being.

  Possible features that *MAY* be implemented after the release of the module,
  is integration with panels (http://drupal.org/project/panels/) and context
  (http://drupal.org/project/context) modules, in order to provide the feature
  mentioned in the above paragraph.
