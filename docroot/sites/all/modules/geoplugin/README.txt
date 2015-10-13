geoPlugin
=========

geoPlugin is a free resource to provide geolocation technology to your site.

This module provides an API for using the geoPlugin service on your Drupal site.


Installation
============

Just install the module in your usual way. If you want to use the module via
javascript call, go to admin/config/services/geoplugin page and enable the
according settings.

If you want to use this module via PHP call, no additional setup is required.


Usage
=====

You can use this module in two ways: via JavaScript call or via PHP call.

1. JavaScript example:

var geoplugin = jQuery.cookie('geoplugin');
if (geoplugin && geoplugin.hasOwnProperty('geoplugin_city')) {
  console.log('You are in ' + geoplugin.geoplugin_city);
}

2. PHP example:

if (module_exists('geoplugin')) {
  $geoplugin = geoplugin_locate();
  if (isset($geoplugin) && $geoplugin->city != '') {
    echo "You are in " . $geoplugin->city;
  }
}

Note: Please, remember, that the external API request is executed only once per visitor,
and the response is stored in a cookie or PHP session. If for the testing purposes you
want to update the results, you should run $.removeCookie('filter', null); on Javascript
or $_SESSION['geoplugin'] = NULL; on PHP before the call to the module's API.


Data structure
==============

Here is a full structure of implemented geoplugin objects:

1. Javascript:
Object {
  geoplugin_areaCode: "0",
  geoplugin_city: "London",
  geoplugin_continentCode: "EU",
  geoplugin_countryCode: "GB",
  geoplugin_countryName: "United Kingdom",
  geoplugin_credit: "Some of the returned data includes GeoLite data created by MaxMind, available from <a href=\'http://www.maxmind.com\'>http://www.maxmind.com</a>.",
  geoplugin_currencyCode: "GBP",
  geoplugin_currencyConverter: 0.6714,
  geoplugin_currencySymbol: "£",
  geoplugin_currencySymbol_UTF8: "£",
  geoplugin_dmaCode: "0",
  geoplugin_latitude: "51.469501",
  geoplugin_longitude: "-0.0558",
  geoplugin_region: "London",
  geoplugin_regionCode: "84",
  geoplugin_regionName: "London",
  geoplugin_request: "92.20.44.88",
  geoplugin_status: 200
}

2. Php:

geoPlugin Object
(
    [host] => http://www.geoplugin.net/php.gp?ip={IP}&base_currency={CURRENCY}
    [currency] => USD
    [ip] => 92.20.44.88
    [city] => London
    [region] => London
    [areaCode] => 0
    [dmaCode] => 0
    [countryCode] => GB
    [countryName] => United Kingdom
    [continentCode] => EU
    [latitude] => 51.469501
    [longitude] => -0.0558
    [currencyCode] => GBP
    [currencySymbol] => £
    [currencyConverter] => 0.6714
)


Authors
=======

Pere Orga <pere@orga.cat>
Nikita Petrov <nikita.petrov.drupal@gmail.com>
