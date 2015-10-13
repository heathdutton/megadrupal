<?php

/**
 * @file
 * Hooks implemented on behalf of the Image module to integrate with Entity
 * Xliff.
 */

if (!function_exists('image_entity_xliff_field_handler_info')) {

  /**
   * Implements hook_entity_xliff_field_handler_info().
   */
  function image_entity_xliff_field_handler_info() {
    return array(
      'field_item_image' => array(
        'class' => 'EntityXliff\Drupal\FieldHandlers\ImageFieldHandler',
      ),
    );
  }

}
