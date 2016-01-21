<?php

/**
 * @file
 * Hooks implemented on behalf of the Number module to integrate with Entity
 * Xliff.
 */

if (!function_exists('number_entity_xliff_field_handler_info')) {

  /**
   * Implements hook_entity_xliff_field_handler_info().
   */
  function number_entity_xliff_field_handler_info() {
    return array(
      'integer' => array(
        'class' => 'EntityXliff\Drupal\FieldHandlers\IntegerFieldHandler',
      ),
    );
  }

}
