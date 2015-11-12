
======================================================================

CONTENTS
--------

 * Overview
 * Features
 * Requirements
 * Installation
 * Hooks
 * Usage
 * More Information


OVERVIEW
--------

The 'Fieldset helper' module saves the state of a collapsible fieldset
in a browser cookie. Being able to save the state of a collapsible fieldset
improves the usability of a website's forms and documents because users
are able to better customize what information is being presented to them.

For site administrator on the 'Modules' page, this module will save the
state of all the collasible fieldsets and add 'expand all' and 'collapse all'
links to the top page. These two tweaks make it easier for administrators to
enable and disable their modules.


FEATURES
--------

- Save the fieldset states on node add (node/add/*) and node edit (node/*/edit) as one page (node/form).
- Default fieldsets to be collapsible on specified or all pages.
- Default fieldsets to be collapsed on specified or all pages.
- Add 'Expand all | Collapse all' links on specified or all pages.
- Set the minimum number of collapsible fieldsets required to show 'Expand all | Collapse all' links.
- Allows specified collapsible fieldset's state to be saved across multiple pages by page or element id.
- Set the fieldset_helper_state_manager cookie duration.



INSTALLATION
------------

1. Copy/upload the fieldset_helper.module to the sites/all/modules directory
   of your Drupal installation.

2. Enable the fieldset_helper.module in Drupal (Modules).

3. Set the 'save fieldset state' user permissions for the 'Fieldset helper' module. (People > Permissions)


HOOKS
-----

 - HOOK_fieldset_alter(&$element);
 - HOOK_fieldset_helper_path_alter(&$path);
 - HOOK_fieldset_helper_element_path_alter($element_id, &$path);

USAGE:
------

  The below code snippet can used to add a 'hand-coded' collapsible fieldset to a node or a block.

  The code snippet is based on 'Adding a collapsible fieldset to your nodes'
  (http://drupal.org/node/118343).

  Notes:

    - Make sure your input filter does not use the 'Line break converter'.
      This will break your fieldset's formatting.

    - Required javascript file will be dynamically added if the any region on
      the page has a collapsible fieldset.

  Code Snippet:

    <fieldset id="static-xhtml-fieldset-example" class="collapsible form-wrapper">
      <legend><span class="fieldset-legend">Static XHTML collapsible fieldset example.</span></legend>
      <div class="fieldset-wrapper">This is an example of a static XHTML fieldset that is collapsible.</div>
    </fieldset>

    <fieldset id="static-xhtml-fieldset-example-collapsed" class="collapsible collapsed form-wrapper">
      <legend><span class="fieldset-legend">Static XHTML collapsed fieldset example</span></legend>
      <div class="fieldset-wrapper">This is an example of a static XHTML fieldset that is collapsed.</div>
    </fieldset>


MORE INFORMATION
----------------

 - Homepage
   http://thebigbluehouse.com

 - Download
   http://drupal.org/project/fieldset_helper


 - Changelog:
   http://cvs.drupal.org/viewvc.py/drupal/contributions/modules/fieldset_helper/CHANGELOG.txt?view=markup&pathrev=DRUPAL-7--2

 - CVS tree:
   http://cvs.drupal.org/viewvc.py/drupal/contributions/modules/fieldset_helper/?pathrev=DRUPAL-7--2

 - Demo site:
   http://drupal.bigbluedrop.com/admin/settings/fieldset_helper/test


AUTHOR/MAINTAINER
-----------------

 - Jacob Rockowitz
   http://drupal.org/user/371407
