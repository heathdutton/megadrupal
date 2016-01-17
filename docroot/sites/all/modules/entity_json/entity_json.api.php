<?php

/**
 * @file
 * API documentation for hooks invoked (provided) by the Enitiy JSON module.
 */

/**
 * Alter hook for the internal links at the top of issue node pages.
 *
 * @param array $return
 *   The array to be converted into JSON and returned.
 * @param array $context
 *   An array consisting of three parts. The 'entity' object, the 'author'
 *   user-object and the 'content' array loaded from the entity via
 *   field_attach_view.
 *
 * @see entity_json_json()
 * @see drupal_alter()
 */
function hook_entity_json_alter($return, $context) {
  $return['newData'] = $context['entity']->myModuleData;
}
