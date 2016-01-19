<?php
/**
 * Developers documentation for Cloudwords module.
 */

/**
 * Alter the map of the Drupal to Cloudwords languages.
 *
 * For example you want to add French (Switzerland) language. In this
 * case you create custom language in drupal with code 'fr-ch' and
 * map it to Cloudwords language 'fr-ch'. Languages got mapped after
 * hook_multilingual_settings_changed() fired. So if you can't see
 * your changes -- do some changes in your language settings (for example
 * add and remove some language).
 *
 * @param array $map
 */
function hook_cloudwords_languages_map_alter(&$map) {
  $map['fr-ch'] = 'fr-ch';
}