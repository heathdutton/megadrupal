Module: Sharerich

Description
===========
Customisable Share buttons for social media.

Dependencies
- ctools
- token
- libraries

Supports
- google_analytics_et (If the module is enabled, GA event tracking will be added to the share buttons)

Installation
============
Put the modules into your sites/module/contrib folder;

Make sure rrssb is in the libraries folder.
If you use drush and make files, it should clone it automatically into /libraries/rrssb

To run the drush command manually, go to the drupal installation folder and run:
$ drush make --no-core ./sites/all/modules/contrib/sharerich/sharerich.make

The command above will download rrssb into ./sites/all/libraries/rrsb folder.

Alternativelly, you can download rrssb library manually from
https://github.com/kni-labs/rrssb/releases/tag/v1.6.5 and place it inside ./sites/all/libraries/rrssb folder.
Sharerich currently supports rrssb version v1.6.5.

Now you can enable Sharerich.
Go to the permissions page and set them accordingly.

Configuration
=============
- Visit /admin/structure/sharerich and create your own button sets;
- Visit /admin/structure/sharerich/settings for general settings;
- Go to each content type, open the Sharerich tab and select how many button sets you want to create.
- Go to each display of the content type and choose which button set to use with each Sharerich field.

Alternativelly, Sharerich creates blocks for each button set (if you tick the box when creating button sets).
see /admin/structure/block

Notes
=====

  - Facebook share:

  It looks like Facebook is now ignoring any custom parameters on the share widget (https://developers.facebook.com/x/bugs/357750474364812/)
  Since facebook.inc service uses www.facebook.com/sharer/sharer.php, it will pull the information from the Open graph tags of the Url being shared.
  If you want to use custom information, you need to use the widget below. Please note that you will need to have a Facebook App Id and Site Url.

  <a href="https://www.facebook.com/dialog/feed?redirect_uri=[sharerich:fb_site_url]&display=popup&app_id=[sharerich:fb_app_id]&link=[sharerich:url]&name=[sharerich:title]&description=[sharerich:summary]" class="popup">
    <span class="icon">
        <svg version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" width="28px" height="28px" viewBox="0 0 28 28" enable-background="new 0 0 28 28" xml:space="preserve">
            <path d="M27.825,4.783c0-2.427-2.182-4.608-4.608-4.608H4.783c-2.422,0-4.608,2.182-4.608,4.608v18.434
                c0,2.427,2.181,4.608,4.608,4.608H14V17.379h-3.379v-4.608H14v-1.795c0-3.089,2.335-5.885,5.192-5.885h3.718v4.608h-3.726
                c-0.408,0-0.884,0.492-0.884,1.236v1.836h4.609v4.608h-4.609v10.446h4.916c2.422,0,4.608-2.188,4.608-4.608V4.783z"/>
        </svg>
    </span>
    <span class="text">facebook</span>
  </a>


  - To alter the buttons markup.

  hook_sharerich_buttons_alter(&$buttons) {

  }
