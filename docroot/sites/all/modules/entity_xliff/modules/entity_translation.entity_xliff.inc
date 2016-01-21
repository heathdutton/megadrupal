<?php

/**
 * @file
 * Hooks implemented on behalf of the Entity Translation module to integrate
 * with Entity Xliff.
 */

use EntityXliff\Drupal\Factories\EntityTranslatableFactory as Paradigm;

if (!function_exists('entity_translation_entity_xliff_translatable_info')) {

  /**
   * Implements hook_entity_xliff_translatable_info().
   */
  function entity_translation_entity_xliff_translatable_info() {
    $info = array(
      'user' => array(
        'classes' => array(
          Paradigm::ENTITYFIELD => 'EntityXliff\Drupal\Translatable\Entity\UserTranslatable',
        ),
      ),
    );

    if (module_exists('comment')) {
      $info['comment'] = array(
        'classes' => array(
          Paradigm::ENTITYFIELD => 'EntityXliff\Drupal\Translatable\Entity\CommentTranslatable',
        ),
      );
    }

    if (module_exists('node')) {
      $info['node'] = array(
        'classes' => array(
          Paradigm::ENTITYFIELD => 'EntityXliff\Drupal\Translatable\Entity\NodeTranslatable',
        ),
      );
    }

    if (module_exists('taxonomy')) {
      $info['taxonomy_term'] = array(
        'classes' => array(
          Paradigm::ENTITYFIELD => 'EntityXliff\Drupal\Translatable\Entity\TaxonomyTermTranslatable',
        ),
      );
    }

    return $info;
  }

}
