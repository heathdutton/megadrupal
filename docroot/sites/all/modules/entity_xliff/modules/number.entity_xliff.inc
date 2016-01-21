<?php

/**
 * @file
 * Hooks implemented on behalf of the List module to integrate with Entity
 * Xliff.
 */

if (!function_exists('list_entity_xliff_field_handler_info')) {

  /**
   * Implements hook_entity_xliff_field_handler_info().
   */
  function list_entity_xliff_field_handler_info() {
    return array(
      'boolean' => array(
        'class' => 'EntityXliff\Drupal\FieldHandlers\DefaultFieldHandler',
      ),
    );
  }

}
