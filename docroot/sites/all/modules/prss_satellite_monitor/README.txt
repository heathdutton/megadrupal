====== PRSS Satellite Monitor ======
===== Overview =====
A module for the integrating PRSS satellite monitoring into a Drupal site.

This module creates a page that displays the same graphs available on the
Webmin interface of PRSS Satellite Receivers.

This is pretty much a glorified HTML scraper.

===== Dependencies =====
This module depends on PHP 5.2 or greater as it utilizes json_encode and
json_decode functions.

This module also depends upon the cURL library, so it may not work
out-of-the-box with many Windows web servers.
