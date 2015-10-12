Getty Images
=======================

https://www.drupal.org/sandbox/gettyimagesapi/2450655

INTRODUCTION
------------

Getty Images brings its award-winning content to the Drupal community.  Its
best-in-class photographers and imagery help customers produce inspiring work
which appears every day in the world’s most influential newspapers, magazines,
advertising campaigns, films, television programs, books and online media.

Within this module, you have 2 ways to access tons of photos, vectors and
illustrations:

- You can click Access Embeddable Images and choose from more than 55 million
  embeddable images. These images are available for non-commercial use and at
  no cost to you. For more information, read our Embed FAQ and Terms of Use.

- You can click Getty Images Customer and then sign in with Getty Images
  account.  From there, you can search for images specific to your Getty Images
  licensing agreement. You can download and preview comp images before you post
  as well as select from different image sizes before you publish. Licensed
  images are saved to your Media Library for future use.

* For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/gettyimagesapi/2450655

* To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2450655

REQUIREMENTS
------------
This module requires the following modules:

* Image (https://www.drupal.org/project/image)
* Libraries API (https://www.drupal.org/project/libraries)
* jQuery Update (https://www.drupal.org/project/jquery_update)
* FileField Sources (https://www.drupal.org/project/filefield_sources)

This module requires the following JS Libraries:

* backbone (http://backbonejs.org/backbone-min.js)
* underscore 1.6.0 (https://github.com/jashkenas/underscore/tree/1.6.0)
* spin.js (http://fgnass.github.io/spin.js)
* jQuery Cookie (https://github.com/carhartl/jquery-cookie)

INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7 for
  further information.

* The Getty Images module uses the libraries module to include the above
  JavaScript requirements. You must download and install these libraries to the
  following directories:

   - sites/all/libraries/backbone/backbone-min.js
   - sites/all/libraries/underscore/underscore-min.js
   - sites/all/libraries/spinjs/spin.min.js
   - sites/all/libraries/jquery-cookie/jquery.cookie.js

CONFIGURATION
-------------

* Configure the Text Format

  If you don’t have a licensing agreement with Getty Images, you can embed
  images for free on your blog or site as long as the photo is not used for
  commercial purposes (meaning not in an advertisement or to sell a product,
  raise money, or promote or endorse something). For more information about
  usage terms, see the Getty Images Embed page or FAQ.

  Follow these steps to setup Text Format:

   - In the Drupal admin area, click Configuration, and then click Text formats.
     (Note that this field is available only to SuperAdmin users.)

   - Select the Text Format where you’d like to embed an image,
     and then click Configure.

   - Under Enabled filters, select the Getty Images Embeds check box.

   - Click Save configuration.

   - Repeat for each Text Format in which you’d like to embed images.

* Embed a Photo

  After you configure the Text Format for Getty Images Embeds, follow these
  steps to embed a photo on the page:

   - On the editing screen, click the Getty Images tab in the lower-right
     corner.

   - Click Access Embeddable Images.

   - Search for a photo using keywords and optional filters,
     and then click Search.

   - Select the image you want to embed.

   - At the bottom of the Image Details panel, click Copy Embed Image HTML to
     clipboard. This copies the embed code into session memory,
     emulating your clipboard.

     Note: Web browsers will not allow the module to copy to your clipboard
     directly.

   - On the editing screen, place your cursor in the text field where you want
     to embed an image.

     Note: You can only embed within a text field, not an image field.

   - Press Ctrl+V (PC) or Cmd+V (Mac) to paste the embed code. The embed code
     should take the form of a Getty Images URL like http://gty.im/92895101.

     Note: Because the embed code is stored in session memory rather than your
     actual clipboard, you can only use the keyboard shortcut to paste in the
     URL within a text field. Other paste methods (like right-clicking and
     selecting Paste) do not work as expected.

   - Click Save. The gty.im URL should resolve to an image embed.

* Add Getty Images as a Source

  If you have a licensing agreement with Getty Images, you can add Getty images
  as a possible source for the image field to seamless access content
  within Drupal.

   - Under Structure, click Content Types, and then click manage fields.
     Note that you may need to add a new Image field.

   - Edit the image field, and click to expand File sources

   - Select the Attach file from Getty Images checkbox, and then click Save.
