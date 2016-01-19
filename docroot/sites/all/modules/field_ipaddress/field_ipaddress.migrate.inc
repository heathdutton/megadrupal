<?php

/**
 * @file
 * Adding support for the Migrate module.
 */

/**
 * Provides a ipaddress FieldHandler for the Migrate module.
 */
class MigrateFieldIpaddressFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('field_ipaddress'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $delta = 0;
    foreach ($values as $value) {
      if (!empty($value['start']) && !empty($value['end'])) {
        $return[LANGUAGE_NONE][$delta]['ipaddress']['start'] = $value['start'];
        $return[LANGUAGE_NONE][$delta]['ipaddress']['end']   = $value['end'];
        $delta++;
      }
    }
    return isset($return) ? $return : NULL;
  }
}
