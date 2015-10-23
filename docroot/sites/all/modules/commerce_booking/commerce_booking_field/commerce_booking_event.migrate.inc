<?php
/**
 * @file
 * Migrate Handler for the commerce booking event field.
 */

/**
 * Primary value passed to this field must be the entity_id of the event entity.
 *
 * Arguments are used to specify all the other values:
 *   'entity_type' - The entity type of the event being reference.
 */
class MigrateCommerceBookingEventFieldHandler extends MigrateFieldHandler {

  /**
   * Register this handler to the commerce_booking_event field type.
   */
  public function __construct() {
    $this->registerTypes(array('commerce_booking_event'));
  }

  /**
   * Overrides MigrateFieldHandler::prepare().
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();
    if (isset($values['arguments'])) {
      // Would love to use array_filter() but we need to remove NULLs and
      // we don't really want to define a callback function just for this.
      foreach ($values['arguments'] as $key => $value) {
        if (isset($value)) {
          $arguments[$key] = $value;
        }
      }
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $return[$language][$delta] = array('entity_id' => $value) + array_intersect_key($arguments, $field_info['columns']);
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}
