<?php

/**
 * @file
 * Hooks and API functions provided by geoPlugin module.
 *
 */

/**
 * Example of hook_geoplugin_settings_alter().
 */
function hook_geoplugin_settings_alter(&$geoplugin) {
  // The default currency is USD.
  // If we wanted to change the base currency, we would use the following line
  $geoplugin->currency = 'EUR';
}

/**
 * Example of hook_geoplugin_info_alter().
 */
function hook_geoplugin_info_alter(&$geoplugin) {
  // If we want somehow alter the resulted info of geoplugin, we can do it here.
  if ($geoplugin->countryCode == 'RU') $geoplugin->country = 'Russia';
}
