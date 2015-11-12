INTRODUCTION
------------
The queryloader module integrates jQuery queryloader with Drupal 7 allowing
pages to be preloaded using AJAX.

 Features include

 * Adjustable script settings from the Admin configuration page including
    - Background color
    - Bar color
    - Bar height
    - Animation type
    - Percentage visualizing
    - Callback functions

 * Visibility settings (similar to blocks)
    - On specific pages
    - On specific content types

 * The option to call the QueryLoader manually (with the settings set in the admin) using
   Drupal.behaviors.queryloader.run($JqueryObject)
   
For more information about the original jQuery plugin please visit
http://www.gayadesign.com/diy/queryloader2-preload-your-images-with-ease/


INSTALLATION
------------
1. Download queryloader from https://github.com/Gaya/QueryLoader2 and unzip
into /sites/all/libraries/queryloader2.

2. Visit admin/config/user-interface/queryloader and configure which pages
queryloader should be displayed on.


COMPATIBILITY NOTES
-------------------
- The ability to disable deep searching is currently unavailable as it's incompatible 
  with the current Drupal jQuery release.
- Applying the QueryLoader to all AJAX requests can cause issues with drupal_get_form
  requests that use AJAX or AJAX fragments that could be destroyed before they are
  fully loaded.


AUTHORS/MAINTAINERS
-------------------

Drupal Module development:
Marton Bodonyi (username: codesidekick)
http://www.codesidekick.com

jQuery plugin development
Gaya Kessler
http://www.gayadesign.com/
