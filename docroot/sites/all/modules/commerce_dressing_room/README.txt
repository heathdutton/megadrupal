Commerce dressing room
======================

Description
-----------

Commerce dressing room lets you use your webcam and hand gestures
to try on clothes.

Built with Media Capture, Streams and HTML5
(http://dev.w3.org/2011/webrtc/editor/getusermedia.html)

- resize product with you hand (Left: - / Right: +)

- Buy specific variation of a product thanks to "Buy this product" button


Requirements
------------

- Recent WebKit Browser (Google Chrome)

- A HTML5 Theme


Dependencies
------------

- Drupal Commerce and all of its dependencies
https://drupal.org/project/commerce

- Jquery update (require 1.8 version)
https://drupal.org/project/jquery_update

- Commerce Product URLs
https://drupal.org/project/commerce_product_urls


Installation
------------

- Please check that you have a commerce product type
(admin/commerce/products/types) and
  a product display
(node type with product reference field - admin/structure/types)
- Enable "Commerce dressing room" module and all dependencies
from modules pages
  Home > Administration > Modules (admin/modules)


Configuration
-------------

1 - Please change Jquery version on jquery update config page: 1.8
(admin/config/development/jquery_update)

2 - To see "Open virtual dressing room" button on your products display (node),
you have to add 2 or more images thanks to "Commerce dressing room image" field
(on commerce product).

Example:
- Add a Product display (node): Drupal T-shirt
- Add 2 product variations (commerce product)
-- Green T-shirt
   Commerce dressing room image field:
   select commerce_dressing_room/img/tshirt-drupal-vert.png
-- Purple T-shirt
   Commerce dressing room image field:
   select commerce_dressing_room/img/tshirt-drupal-violet.png
- Go to your Drupal t-shirt (front) and click on "Open virtual dressing room"
- Use virtual dressing room


Credits
-------

The original module is written by Florian LE BRENN - http://florianlebrenn.fr
(florian.lebrenn [at] gmail [dot] com).

This module has been inspired by this project:
http://www.soundstep.com/blog/experiments/jsdetection/

Currently, this module use http://www.basic-slider.com/

Current maintainers:
  Florian LE BRENN - http://florianlebrenn.fr
