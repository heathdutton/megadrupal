<?php

/**
 * @file
 * Hooks implemented on behalf of the Link module to integrate with Entity
 * Xliff.
 */

if (!function_exists('link_entity_xliff_field_handler_info')) {

  /**
   * Implements hook_entity_xliff_field_handler_info().
   */
  function link_entity_xliff_field_handler_info() {
    return array(
      'field_item_link' => array(
        'class' => 'EntityXliff\Drupal\FieldHandlers\LinkFieldHandler',
      ),
    );
  }

}
