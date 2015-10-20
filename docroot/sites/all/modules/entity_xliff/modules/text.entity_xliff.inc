<?php

/**
 * @file
 * Hooks implemented on behalf of the Text module to integrate with Entity
 * Xliff.
 */

if (!function_exists('text_entity_xliff_field_handler_info')) {

  /**
   * Implements hook_entity_xliff_field_handler_info().
   */
  function text_entity_xliff_field_handler_info() {
    return array(
      'text' => array(
        'class' => 'EntityXliff\Drupal\FieldHandlers\DefaultFieldHandler',
      ),
      'text_formatted' => array(
        'class' => 'EntityXliff\Drupal\FieldHandlers\FormattedFieldHandler',
      ),
      'field_item_textsummary' => array(
        'class' => 'EntityXliff\Drupal\FieldHandlers\SummarizedFieldHandler',
      ),
    );
  }

}
