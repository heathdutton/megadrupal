<?php

/**
 * @file
 * Hooks implemented on behalf of the Automatic Entity Label module to integrate
 * with Entity Xliff.
 */


if (!function_exists('auto_entitylabel_entity_xliff_translatable_fields_alter')) {

  /**
   * Implements hook_entity_xliff_translatable_fields_alter().
   */
  function auto_entitylabel_entity_xliff_translatable_fields_alter(&$fields, $wrapper) {
    // If this entity/bundle combo has Automatic Entity Labeling enabled...
    $ael_key = $wrapper->type() . '_' . $wrapper->getBundle();
    if (auto_entitylabel_get_setting($ael_key) == AUTO_ENTITYLABEL_ENABLED) {
      // Do not allow the label to be translatable.
      $key = array_search($wrapper->entityKey('label'), $fields);
      if ($key !== FALSE) {
        unset($fields[$key]);
      }
    }
  }

}
