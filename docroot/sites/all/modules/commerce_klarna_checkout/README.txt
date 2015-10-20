DESCRIPTION
===========
Payment module for Drupal Commerce using Klarna Checkout
Read more at
https://klarna.com/sv/salj-med-klarna/vara-tjanster/klarna-checkout
(only in swedish).
API documentation is avalable at https://docs.klarna.com/en/getting-started

REQUIREMENTS
============
- Klarna Checkout PHP Library should be installed in libraries with
  the structure
    sites/all/libraries/klarna_checkout/Checkout.php etc.
    Klarna Checkout PHP is available here
      https://docs.klarna.com/en/downloads/php
    or at github
      https://github.com/klarna/kco_php
- You can download the library with drush, if you have it installed. Use the 
following command:
drush kco-php-dl
- cURL should be installed on server
- DESKTOP: Width of containing block shall be at least 750px
- MOBILE: Width of containing block shall be 100% of browser window
    (No padding or margin)
- The website needs a page with Terms and conditions.
    Specify the path to the page on the settings page.

RECOMMENDED
===========
- Mobile Detect Library can be installed in libraries with the structure
  sites/all/libraries/Mobile-Detect/Mobile_Detect.php etc.
  Mobile Detect is available at
    https://github.com/serbanghita/Mobile-Detect
