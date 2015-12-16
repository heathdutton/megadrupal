<?php
/**
 * @file
 * Functions:
 *
 *  - Helpers to determine entity translatability
 *  - Callback for field widget
 */

/**
 * Determines if an entity is translatable.
 *
 * @param string $entity_type
 *   The type of entity.
 * @param string $bundle
 *   A bundle of the entity.
 * @param object $entity
 *   The entity object.
 *
 * @return bool
 *   Whether an entity is translatable.
 */
function translation_field_diffuser_entity_is_translatable($entity_type, $bundle, $entity = null) {
  $translatable = FALSE;

  if ($entity_type == 'node') {
    $translatable = module_exists('translation') && translation_supported_type($bundle);
    $translatable |= module_exists('i18n_node') && i18n_node_type_enabled($bundle);
  }

  if (module_exists('i18n_taxonomy') && $entity_type == 'taxonomy_term'
    && is_object($entity)) {
    // @todo currently not working, set to FALSE
    $translatable = FALSE;
  }

  if (module_exists('i18n_taxonomy') && $entity_type == 'vocabulary') {
    $entity = !empty($entity) && is_object($entity) ? $entity : taxonomy_vocabulary_machine_name_load($bundle);
    if (property_exists($entity, 'i18n_mode')) {
      $translatable |= $entity->i18n_mode == I18N_MODE_TRANSLATE;
    }
    else {
      $translatable |= i18n_taxonomy_vocabulary_mode($entity) == I18N_MODE_TRANSLATE;
    }
  }

  if (module_exists('entity_translation')) {
    $translatable |= entity_translation_enabled($entity_type, $entity);
  }

  return $translatable;
}

/**
 * Determines if at least one field for an entity is translatable.
 *
 * @param string $type
 *   The machine name entity type.
 *
 * @return bool
 *   Whether at least one field for an entity is translatable.
 */
function translation_field_diffuser_entity_field_is_translatable($type) {
  $translatable = &drupal_static(__FUNCTION__ . '_' . $type, FALSE);

  if (module_exists('entity_translation') && !$translatable) {
    foreach (entity_get_info() as $entity_type => $info) {
      if ($type == $entity_type && $info['fieldable']
        && entity_translation_enabled($entity_type)) {
        $translatable = TRUE;
      }
    }
  }

  return $translatable;
}

/**
 * Callback function to process the field.
 *
 * Add a list of fields that can propagate their data from
 * the entity source to its translations.
 *
 * @param array $element
 *   The field widget form element.
 */
function translation_field_diffuser_field_element_add_process(array &$element, $number_items = 1) {
  $field_name = $element['#field_name'];

  if (translation_field_diffuser_field_is_propagable($element['#entity_type'], $element['#bundle'], $field_name)) {
    if (!property_exists($element['#entity'], 'propagable_fields')) {
      $element['#entity']->propagable_fields = array();
    }
    // Avoid duplicate with multi value element form.
    if (!array_key_exists($field_name, $element['#entity']->propagable_fields)) {
      $element['#entity']->propagable_fields[$field_name] = array(
        'number'     => $number_items,
        'field_name' => $field_name
      );
    }
    else {
      $element['#entity']->propagable_fields[$field_name]['number']++;
    }
  }
}

/**
 * Whether the field instance is propagable.
 *
 * @param string $entity_type
 * @param string $bundle
 * @param string $field_name
 *
 * @return bool
 */
function translation_field_diffuser_field_is_propagable($entity_type, $bundle, $field_name) {
  $propagable = FALSE;
  $variable_name = 'translation_field_diffuser_' . $entity_type;

  if (variable_get($variable_name . '_value', FALSE)) {
    $variable_name .= '_' . $bundle;
    if (variable_get($variable_name . '_value', FALSE)) {
      $variable_name .= '_' . $field_name;
      if (variable_get($variable_name . '_value', FALSE)) {
        $propagable = TRUE;
      }
    }
  }

  return $propagable;
}
