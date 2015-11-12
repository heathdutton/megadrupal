<?php

/**
 * @file
 * Hook documentation for the Entity Xliff module.
 *
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup entity_xliff Entity Xliff module integrations.
 *
 * Module integrations with the Entity Xliff module.
 */


/**
 * @defgroup entity_xliff_hooks Entity Xliff's hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend the Entity
 * Xliff module.
 */

/**
 * Return info about entities and the way in which they integrate with the
 * Entity Xliff module.
 *
 * @return array
 *   Returns an associative array of Entity Xliff translatable details, keyed by
 *   entity type and translation paradigm. At a bare minimum Entity Xliff
 *   translatable info must include the following:
 *   - classes: An associative array of classes, keyed by translation paradigm,
 *     that implement EntityXliff\Drupal\Interfaces\EntityTranslatableInterface,
 *     describing how entities of this type can be imported/exported. If you are
 *     providing a class, you may wish to study or extend from the
 *     EntityXliff\Drupal\Translatable\EntityTranslatableBase abstract class.
 *     Also note that translation paradigms exist as static constants on the
 *     EntityXliff\Drupal\Factories\EntityTranslatableFactory class.
 */
function hook_entity_xliff_translatable_info() {
  $translatables['node'] = array(
    'classes' => array(
      'content_translation' => 'NodeContentTranslatable',
    ),
  );

  // Example showing a namespace'd PHP class.
  $translatables['my_entity'] = array(
    'classes' => array(
      'entity_translation' => 'NameSpace\Of\MyEntityTranslatable',
    ),
  );

  return $translatables;
}

/**
 * Alter Entity Xliff translatable info before it's applied to an Entity's info.
 *
 * @param array $translatables
 *   An associative array of Entity Xliff translatable details exactly as spec'd
 *   in hook_entity_xliff_translatable_info().
 */
function hook_entity_xliff_translatable_info_alter(&$translatables) {
  $translatables['node']['classes']['content_translation'] = 'MyCustomNodeTranslatable';
}

/**
 * Return info about entity fields and the way in which they integrate with the
 * Entity Xliff module.
 *
 * @return array
 *   Returns an associative array of Entity Xliff field handler details, keyed
 *   by entity type. At a bare minimum Entity Xliff field handler info must
 *   include the following:
 *   - class: Class that implements EntityXliff\Interfaces\FieldHandlerInterface,
 *     describing how entity metadata properties of this type can be get/set.
 */
function hook_entity_xliff_field_handler_info() {
  $handlers['addressfield'] = array(
    'class' => 'AddressFieldHandler',
  );

  // Example showing a namespace'd PHP class.
  $handlers['my_field_type'] = array(
    'class' => 'NameSpace\Of\MyFieldTypeHandler',
  );

  return $handlers;
}

/**
 * Alter Entity Xliff field handler info before it's provided for use by the
 * Entity Xliff field handling / mediation process.
 *
 * @param array $handlers
 *   An associative array of Entity Xliff translatable details exactly as spec'd
 *   in hook_entity_xliff_translatable_info().
 */
function hook_entity_xliff_field_handler_info_alter(&$handlers) {
  $handlers['addressfield']['class'] = 'MyCustomAddressFieldHandler';
}

/**
 * @}
 */
