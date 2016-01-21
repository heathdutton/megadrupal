<?php

/**
 * @file
 * Hooks provided by the Potx Exportables module.
 */


/**
 * Hook used to define the location where to find the po files.
 * A po file has a name of the form "name.ln.po" where ln
 * is the language code.
 *
 * @return
 *   An associative array with a key and a path.
 */
function hook_potx_file_location_info() {
  return array(
    'single_blog' => array(
      'path' => drupal_get_path('module', 'single_blog') . '/potx',
    ),
  );
}

/**
 * Act on module implementations of the previously described hook.
 *
 * @param array $info
 *   Implementations of hook_potx_exportable_file_location_info().
 */
function hook_potx_file_location_info_alter(&$info) {

}
