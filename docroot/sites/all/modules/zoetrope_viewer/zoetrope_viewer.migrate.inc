<?php

/**
 * @file
 * Defines a migration destination for zoetrope_image fields.
 */

/**
 * Implements hook_migrate_api().
 */
function zoetrope_viewer_migrate_api() {
  $api = array(
    'api' => 2,
    'field handlers' => array('ZoetropeViewerUUIDFieldHandler'),
  );
  return $api;
}


/**
 * Zoetrope Field handler for the Migrate module.
 */
class ZoetropeViewerUUIDFieldHandler extends MigrateFieldHandler {

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    // Tell migrate what fields we can handle.
    $this->registerTypes(array('zoetrope_uuid_field'));
  }

  /**
   * {@inheritdoc}
   */
  public function arguments($zoetrope_uuid = NULL, $start_position = NULL) {
    $arguments = array();
    if (!is_null($zoetrope_uuid)) {
      $arguments['zoetrope_uuid'] = $zoetrope_uuid;
    }
    if (!is_null($start_position)) {
      $arguments['start_position'] = $start_position;
    }
    return $arguments;
  }

  /**
   * {@inheritdoc}
   */
  public function fields($type, $instance, $migration = NULL) {
    $fields = array();
    if ($type == 'zoetrope_uuid_field') {
      $fields['start_position'] = t('Subfield: the starting position');
      $fields['gallery_image_positions'] = t('Subfield: the gallery image positions');
    }
    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }

    $migration = Migration::currentMigration();

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $item = array();

      // Zoetrope UUID population.
      $item['zoetrope_uuid'] = $value;

      // Starting position population.
      if (isset($arguments['start_position'])) {
        if (is_array($arguments['start_position'])) {
          $item['start_position'] = intval($arguments['start_position'][$delta]);
        }
        else {
          $item['start_position'] = intval($arguments['start_position']);
        }
      }

      // Gallery image population.
      if (isset($arguments['gallery_image_positions']) && is_array($arguments['gallery_image_positions'])) {

        if (is_array(reset(reset($arguments['gallery_image_positions'])))) {
          $item['gallery_image_positions'] = _zoetrope_viewer_gallery_image_helper($arguments['gallery_image_positions'][$delta]);
        }
        else {
          $item['gallery_image_positions'] = _zoetrope_viewer_gallery_image_helper($arguments['gallery_image_positions']);
        }
      }

      $return[LANGUAGE_NONE][] = $item;
      $delta++;
    }
    return isset($return) ? $return : NULL;
  }
}

/**
 * Clean the gallery image formatter to drop any additional payload.
 *
 * This a security function which prevents any malformed or malicous code being
 * inserted via the `gallery_image` object.
 *
 * @param array $list
 *   an array containing the imported JSON.
 *
 * @return array
 *   The sanitised list.
 */
function _zoetrope_viewer_gallery_image_helper(array $list) {
  $ret = array();
  foreach ($list as $struct) {
    if (is_array($struct) && isset($struct['position']) && is_numeric($struct['position'])) {
      $ret[] = array(
        'position' => intval($struct['position']),
        'weight' => (isset($struct['weight']) ? floatval($struct['weight']) : 0),
      );
    }
  }
  return $ret;
}
