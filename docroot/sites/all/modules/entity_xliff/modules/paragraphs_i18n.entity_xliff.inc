<?php

/**
 * @file
 * Hooks implemented on behalf of the Paragraphs i18n module to integrate with
 * Entity Xliff.
 */

use EntityXliff\Drupal\Factories\EntityTranslatableFactory as Paradigm;

if (!function_exists('paragraphs_i18n_entity_xliff_translatable_info')) {

  /**
   * Implements hook_entity_xliff_translatable_info().
   */
  function paragraphs_i18n_entity_xliff_translatable_info() {
    return array(
      'paragraphs_item' => array(
        'classes' => array(
          Paradigm::CONTENT => 'EntityXliff\Drupal\Translatable\Content\ParagraphsTranslatable',
        ),
      ),
    );
  }

}

if (!function_exists('paragraphs_i18n_entity_xliff_translatable_fields_alter')) {

  /**
   * Implements hook_entity_xliff_translatable_fields_alter().
   *
   * Ensures that paragraphs items fields always appear last in XLIFF exports.
   * This works around an issue where paragraphs items rely on host entity
   * translation initialization.
   */
  function paragraphs_i18n_entity_xliff_translatable_fields_alter(&$fields, $wrapper) {
    $info = $wrapper->getPropertyInfo();
    $needle = 'paragraphs_item';

    for ($n = 0; $n < count($fields); $n++) {
      $field = $fields[$n];
      // If this field is translatable and it's a paragraphs item, move it to
      // the back of the translatable list.
      if (isset($info[$field]['type']) && strpos($info[$field]['type'], $needle)) {
        unset($fields[$n]);
        $fields[] = $field;
      }
    }
  }

}
