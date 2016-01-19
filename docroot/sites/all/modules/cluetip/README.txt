
Cluetip module:
-------------------------

D6:
Author - Chris Shattuck (www.impliedbydesign.com)
Co-Author - Alex McFadyen (www.openlyconnected.com)

D7:
Author - Steve Tweeddale (www.computerminds.co.uk)
Co-Author - Inder Singh (www.indersingh.com)

License - GPL

Overview:
-------------------------

The cluetip module is a thin wrapper for the jQuery [Cluetip][1] plugin, which
provides nice, configurable on-hover tooltips. It's most simple (default) usage
generates the tooltip title from an elements title attribute, and will load as
the tooltip body content from a link specified in the rel attribute (via AHAH).
A range of more complex uses and behaviours are available through it's
extensive options, many of which are helpfully demonstrated in the 'demo'
folder that's included in the plugin's [download][2], and on the projects
[demo page][3].

[1]: http://plugins.learningjquery.com/cluetip
[2]: http://plugins.jquery.com/project/cluetip
[3]: http://plugins.learningjquery.com/cluetip/demo/

Additionally, the [bgiframe][4] and [hoverIntent][5] jQuery plugins can be used
to enhance cluetip, and are in fact included with the cluetip distribution.
Currently, both will automatically be utilised by this module.

[4]: http://brandonaaron.net/code/bgiframe/docs
[5]: http://cherne.net/brian/resources/jquery.hoverIntent.html


Installation:
-------------------------

1. Download the Cluetip module and copy it into your 'modules' directory.
2. Download the Cluetip library from http://plugins.jquery.com/project/cluetip/
3. Unzip the library and place it one of the following folders:

    - sites/all/libraries
    - sites/YOURDOMAIN/libraries
    - profiles/YOURPROFILE/libraries

   ...replacing YOURDOMAIN or YOURPROFILE as appropriate.

   rename your cluetip library folder to "cluetip" if it is something else and
   make sure that "jquery.cluetip.js" file is as:
   "libraries/cluetip/jquery.cluetip.js"

4. Go to Administer >> Modules and enable the module.


Example usage:
-------------------------

By default the cluetip library is included on every page. but this setting
could be turned off from "admin/config/user-interface/cluetip". This is helpful
to turn it off if you are planning to add library on selected pages manually.

E.g you can add a library manually in following way:-

    drupal_add_library('cluetip', 'cluetip');
    or
    libraries_load('cluetip');

This could be done either in a module or your themes template.php. You then
need to instantiate cluetip with custom Javascript code (again, added via a
module or your theme's template.php). This has been left up to you due to the
vast array of options and methods it can be instantiated with. If you haven't
read the [demo page][3] yet, please do so.

Here's a very simple implementation of using the cluetip jquery plugin in Drupal
to show the title attribute of any element with the class "cluetip-title" in a
nice cluetip popup. For this to work the title has to be in the format
title="Header|Title text". It's applied using drupal behaviours, so it will run
on page load and any time content is added to your page (using Drupal's
methods):

    (function ($) {

    Drupal.behaviors.cluetip = {
      attach: function(context, settings) {
        $('.cluetip-title').cluetip({splitTitle: '|'});
      }
    }

    })(jQuery);

This modules will add default JS on pages so you can use class "cluetip-title"
for cluetip titles. Here is the default JS being used by module which is
included on every page.

$('.cluetip-title').cluetip({
    arrows: true,
    splitTitle: '|',
    cluetipClass: 'rounded'
  }
  );

