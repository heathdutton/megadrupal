<?php
/**
 * @file
 * Provides the general page callbacks.
 */

/**
 * Menu callback; Prints the translation server information.
 */
function l10n_update_bundled_server() {
  drupal_add_http_header('Content-Type', 'text/xml; charset=utf-8');

  echo '<?xml version="1.0" encoding="UTF-8"?>';
  echo '<l10n_server>';

  // Basis server information.
  echo '<name>' . variable_get('site_name', 'Drupal') . '</name>';
  echo '<link>' . _l10n_update_bundle_url() . '</link>';
  echo '<update_url>' . _l10n_update_bundle_url('system/l10n/%key/po/%project/%language') . '</update_url>';

  // Enabled languages.
  $languages = language_list('enabled');
  $languages = $languages[1];

  echo '<languages>';

  foreach ($languages as $language) {
    echo '<language>';
    echo '<name>' . check_plain($language->name) . '</name>';
    echo '<native>' . check_plain($language->native) . '</native>';
    echo '<code>' . $language->language . '</code>';
    echo '</language>';
  }

  echo '</languages>';

  echo '</l10n_server>';

  drupal_exit();
}

/**
 * Menu callback; Transfer the po file of a module or theme.
 *
 * @param $name
 *   The module or theme name.
 * @param $language
 *   Serve the po file of this language.
 */
function l10n_update_bundled_po($name, $language) {
  // Build the path to the translation file.
  $file = _l10n_update_bundled_path($name) . '/translations/' . $language . '.po';

  // Make sure the po file exists.
  if (is_file($file)) {
    if ($fp = fopen($file, 'rb')) {
      // Get the last modified timestamp.
      $modified = _l10n_update_bundled_po_timestamp($file, $fp);

      // Start the file transmission.
      drupal_add_http_header('Content-Type', 'text/x-po');
      drupal_add_http_header('Content-Length', filesize($file));
      drupal_add_http_header('Last-Modified', gmdate('D, d M Y H:i:s', $modified) . ' GMT');
      drupal_send_headers();

      if (ob_get_level()) {
        ob_end_clean();
      }

      while (!feof($fp)) {
        print fread($fp, 1024);
      }

      fclose($fp);
      drupal_exit();
    }
  }

  // Something went wrong.
  drupal_not_found();
  drupal_exit();
}
