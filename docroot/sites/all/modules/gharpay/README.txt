README
=====================
Gharpay


What is Gharpay
---------------------
This module integrates payment method provided by Gharpay.in with Drupal.
Gharpay has an Indian network of executives to reach customer's doorsteps and accept payments. They don't deliver any products so ecommerce companies can use them to do cash bookings before shipping their products.

About this module
---------------------
This module currently integrates with Ubercart Drupal module and provides a payment method to configure.
It is packaged into two modules: gharpay and uc_gharpay.
Common functions are available in gharpay module, including the configuration form. Idea is to add support for webform and independent integration over time.

Requirements
---------------------
Gharpay PHP Library

Install
---------------------
1. Download Gharpay PHP Library from https://github.com/downloads/gharpay/PHP-Library/PHP-Library_1_02.zip
2. Extract zip and place the content in sites/all/libraries.
3. Make sure GharpayAPI.php is available at sites/all/libraries/gharpay/GharpayAPI.php
4. Create an account on gharpay.in and obtain API key and password.
5. Edit config.php provided in the GharpayAPI PHP Library and add your API key as <username> and your password as <password>.
    define("USERNAME","your_api_key"); // add your username here;
    define("PASSWORD","your_password"); // add password here;
6. Go to admin/config/services/gharpay and configure.
7. Payment method would get automatically enabled for Ubercart once uc_gharpay module is enabled.


Roadmap
----------------------
1. Add support for independent integration.
2. Allow to track order status at Gharpay's end.


======================
This module is sponsored by gharpay.in