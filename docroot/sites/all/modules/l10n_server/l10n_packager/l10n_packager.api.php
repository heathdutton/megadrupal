<?php

/**
 * @file
 * Hooks provided by the Localization packager module.
 */

/**
 * Executes code after packaging was done for a file.
 *
 * @param $file
 *   File object. Only the 'uri' public property is required.
 */
function hook_l10n_packager_packaged($file) {
  // Purge the Fastly CDN cache for this file.
  if ($fastly_key = variable_get('drupalorg_fastly_key', FALSE)) {
    $download_url = variable_get('l10n_packager_update_url', file_create_url(l10n_packager_directory()));
    $request = drupal_http_request($download_url . preg_replace('%^' . preg_quote(l10n_packager_directory(), '%') . '%', '', $file->uri), array(
      'headers' => array('Fastly-Key' => $fastly_key),
      'method' => 'PURGE',
    ));
  }
}
