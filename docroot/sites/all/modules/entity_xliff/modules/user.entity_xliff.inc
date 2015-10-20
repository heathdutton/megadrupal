<?php

/**
 * @file
 * Hooks implemented on behalf of the User module to integrate with
 * Entity Xliff.
 */

use EntityXliff\Drupal\Factories\EntityTranslatableFactory as Paradigm;

if (!function_exists('user_entity_xliff_translatable_info')) {

  /**
   * Implements hook_entity_xliff_translatable_info().
   */
  function user_entity_xliff_translatable_info() {
    return array(
      'user' => array(
        'classes' => array(
          Paradigm::ENTITYFIELD => 'EntityXliff\Drupal\Translatable\Entity\UserTranslatable',
        ),
      ),
    );
  }

}
