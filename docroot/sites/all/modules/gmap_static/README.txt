CONTENTS OF THIS FILE
---------------------

 * Description
 * Installation
 * Configuration
 * Using
 * Support

DESCRIPTION
-----------
Google Map Static module allows to switch easily between 
interactive google maps (iframe) and static image maps (img). 

Sometimes you scroll the page with the map and try to look what's under it but
all you can scroll is the map. We found a solution of this usability problem.
Our module allows you to replace the iframe with a static image map. User can
switch to the interactive map and back any time. The iframe map will be opened
either in the popup, in a new window or in the same map area depending on what
option you choose in the module settings.

Each option can be used only on mobile or desktop devices or both at the same
time.

INSTALLATION
------------
 * Copy the entire module directory the Drupal sites/all/modules directory.
 * Login as an administrator. Enable the module in the "Administration" »
  "Modules".

CONFIGURATION
-------------
Module Configures in Administration » Configuration » System » GoogleMap Static

 * Configurate view mode for each device:

   - Columns "%DEVICE% preferences".
     Device types that can be configure.

   - Rows view-mode.
     View modes that can be set for the specific device:
     - none: Do nothing.
     - Popup map: Change iframe to static map. Switchable to fullscreen popup.
     - Change condition: Change iframe to static map. Switchable back to iframe.
     - In new window : Change iframe to static map. Display Iframe in a new
       window.

 * Configure visibility settings:
   
   - Pages List.
     Specify pages by using their paths. Enter one path per line. The "*" character
     is a wildcard.

   - Option:
     - All pages except those listed - Don't change a map to static on pages from
       this list, and change it for all other pages.
     - Only the listed pages - Change a map to static only on pages from list,
       and don't change it for any other.

USING
-------------
Add the <iframe> element with Google Map Embed in a block, field, or node.
The module automatically finds all the Google Maps in the iframe and replaces it
with images. If settings allow you to change to a specific page with the current
device, the card will become static.

More about Google Map Embed https://support.google.com/maps/answer/3544418

SUPPORT
-------
Feel free to report bugs and propositions in our Issue Queue
http://drupal.org/project/issues/gmap_static