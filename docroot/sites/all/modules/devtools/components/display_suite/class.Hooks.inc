<?php
namespace devtools\components\display_suite;

class Hooks extends \devtools\Hooks {
  protected static $_ds;

  function __construct() {
    self::$_ds = \devtools\components\DisplaySuite::instance();
  }

  /**
   * Extends hook_theme().
   */
  public function hook_theme($existing, $type, $theme, $path) {
    $data = array();

    foreach (self::$_ds->getFields() as $entity_type => $fields) {
      foreach ($fields as $field_name => $field_data) {
        $data[$field_name] = $field_data;
      }
    }

    return $data; 
  }

  /**
   * Extends hook_ds_fields_info().
   */
  function hook_ds_fields_info($entity_type) {
    $data = array();

    $fields = self::$_ds->getFields();

    if (isset($fields[$entity_type])) {
      foreach ($fields[$entity_type] as $field) {
        $data[$entity_type][$field['key']] = array(
          'title' => $field['name'],
          'field_type' => DS_FIELD_TYPE_FUNCTION,
          'function' => 'ds_field_access',
          'properties' => array(
            'default' => array('wrapper' => 'div', 'link' => 0),
            'formatters' => array(
              $field['key'] => $field['name'],
            ),
          )
        );

        if (isset($field['settings'])) {
          $data[$entity_type][$field['key']]['properties']['settings'] = $field['settings'];
          $data[$entity_type][$field['key']]['properties']['default'] = $field['default'];
        }

        if (isset($field['access_callback'])) {
          $data[$entity_type][$field['key']]['properties']['access_callback'] = $field['access_callback'];
        }

        if (isset($field['ui_limit'])) {
          $data[$entity_type][$field['key']]['ui_limit'] = $field['ui_limit'];
        }
      }
    }

    return $data;
  }
}