<?php

/**
 * @file
 * Support for processing entity reference fields in Migrate.
 */

/**
 * Implement hook_migrate_api().
 */
function er_plus_migrate_api() {
  return array(
    'api' => 2,
    'field_handlers' => array('MigrateERPlusFieldHandler'),
  );
}

class MigrateERPlusFieldHandler extends MigrateEntityReferenceFieldHandler {
  public function __construct() {
    parent::__construct(array(
      'value_key' => 'target_id',
      'skip_empty' => TRUE,
    ));
    $this->registerTypes(array('entityreference'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $parent_return = parent::prepare($entity, $field_info, $instance, $values);
    if (isset($values['arguments']['class']) && $values['arguments']['class'] == 'MigrateEntityReferenceFieldHandler') {
      return $parent_return;
    }
    $items = array();
    foreach ($values as $delta => $item_id) {
      if (is_int($delta) && is_array($item_id)) {
        $items[$delta]['target_id'] = $values[$delta]['destid1'];
        $items[$delta]['view_mode'] = $values['arguments']['view_mode'][$delta];
        $items[$delta]['header'] = isset($values['arguments']['header'][$delta]) ? $values['arguments']['header'][$delta] : '';
      }
      if ($delta === 'destid1') {
        $items[$delta]['target_id'] = $values['destid1'];
        $items[$delta]['view_mode'] = $values['arguments']['view_mode'][0];
        $items[$delta]['header'] = isset($values['arguments']['header'][0]) ? $values['arguments']['header'][0] : '';
      }

    }

    foreach ($items as $item) {
      $return[LANGUAGE_NONE][] = $item;
    }

    return isset($return) ? $return : NULL;
  }

  /**
   * Implementation of MigrateFieldHandler::fields().
   *
   * @param $type
   *  The field type.
   * @param $instance
   *  Instance info for the field.
   * @param Migration $migration
   *  The migration context for the parent field. We can look at the mappings
   *  and determine which subfields are relevant.
   * @return array
   */
  public function fields($type, $instance, $migration = NULL) {
    $fields = array();
    $fields['view_mode'] = t('Subfield: View mode for the field');
    $fields['header'] = t('Subfield: Header for the field');
    $fields['class'] = t('Subfield: Class to use. Currently, only MigrateEntityReferenceFieldHandler is supported as an option.');
    return $fields;
  }

}
