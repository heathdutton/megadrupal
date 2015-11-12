<?php
/**
 * @file
 * Migrate Ooyala fields.
 */

class MigrateOoyalaFieldHandler extends MigrateFieldHandler {
  /**
   * Register our field handler with Migrate.
   */
  public function __construct() {
    $this->registerTypes(array('ooyala'));
  }

  /**
   * Override prepare to set up standard Field API saving of Ooyala fields.
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();

    if (isset($values['arguments'])) {
      $arguments = array_filter($values['arguments']);
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $return[$language][$delta] = array('value' => $value) + array_intersect_key($arguments, $field_info['columns']);
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}
