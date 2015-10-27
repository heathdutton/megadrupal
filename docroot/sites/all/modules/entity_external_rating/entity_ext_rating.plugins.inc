<?php
/**
 * @file
 *  Contains the rating plugins definition that are implemented by this module.
 */

/**
 * Defines the plugins.
 *
 * @return array
 *   An array with the plugins definition, in a format required by
 *   the ctools module. The default rating plugins that are implemented are the
 *   Twitter and Facebook.
 */
function _entity_ext_rating_entity_ext_rating_plugins() {
  $path = drupal_get_path('module', 'entity_ext_rating') . '/plugins';

  $info = array();
  $info['Twitter'] = array(
    'handler' => array(
      'class' => 'TwitterRating',
      'file' => 'TwitterRating.inc',
      'path' => $path,
    ),
  );
  $info['Facebook'] = array(
    'handler' => array(
      'class' => 'FacebookRating',
      'file' => 'FacebookRating.inc',
      'path' => $path,
    ),
  );
  return $info;
}