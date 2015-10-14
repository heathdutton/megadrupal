<?php

/**
 * @file
 * API documentation for CKEditor Skins.
 */

/**
 * Register list of skins to CKEditor.
 *
 * Each item may have the following properties
 * - title: The name of skin.
 * - path: The path to skin, no slash at the end.
 *
 * @param string $version
 *   CKEditor Version.
 *
 * @return array
 *   List of skins.
 *
 * @see hook_ckeditor_skin_alter()
 * @see ckeditor_get_version()
 */
function hook_ckeditor_skin($version = NULL) {
  $skins = array();

  if (empty($version)) {
    return $skins;
  }

  $skin_path = drupal_get_path('module', 'ckeditor_skin') . '/skins';

  if (version_compare($version, '4.1') >= 0) {
    $skins['moono'] = array(
      'title' => 'Moono',
      'path'  => $skin_path . '/v4/moono',
    );

    $skins['moonocolor'] = array(
      'title' => 'Moono Color',
      'path'  => $skin_path . '/v4/moonocolor',
    );

    $skins['kama'] = array(
      'title' => 'Kama',
      'path'  => $skin_path . '/v4/kama',
    );
  }
  elseif (version_compare($version, '3.1') >= 0) {
    $skins['grappelli'] = array(
      'title' => 'Grappelli',
      'path'  => $skin_path . '/v3/grappelli',
    );
  }

  return $skins;
}

/**
 * Change the list of registered skins.
 *
 * @param array $skins
 *   List of skins.
 *
 * @param string $version
 *   CKEditor Version.
 *
 * @see hook_ckeditor_skin()
 * @see ckeditor_get_version()
 */
function hook_ckeditor_skin_alter(&$skins, $version = NULL) {
  unset($skins['kama']);
}
