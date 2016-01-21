<?php

/**
 * @file
 * Hooks implemented on behalf of the core Translation module to integrate with
 * Entity Xliff.
 */

use EntityXliff\Drupal\Factories\EntityTranslatableFactory as Paradigm;

if (!function_exists('translation_entity_xliff_translatable_info')) {

  /**
   * Implements hook_entity_xliff_translatable_info().
   */
  function translation_entity_xliff_translatable_info() {
    $info = array();

    if (module_exists('node')) {
      $info['node']['classes'] = array(
        Paradigm::CONTENT => 'EntityXliff\Drupal\Translatable\Content\NodeTranslatable',
      );
    }

    if (module_exists('field_collection')) {
      $info['field_collection_item']['classes'] = array(
        Paradigm::CONTENT => 'EntityXliff\Drupal\Translatable\Content\FieldCollectionTranslatable',
      );
    }

    return $info;
  }

}
