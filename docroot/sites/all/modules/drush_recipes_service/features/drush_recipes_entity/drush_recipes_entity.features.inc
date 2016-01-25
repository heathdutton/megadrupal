<?php
/**
 * @file
 * drush_recipes_entity.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drush_recipes_entity_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_eck_bundle_info().
 */
function drush_recipes_entity_eck_bundle_info() {
  $items = array(
    'drush_recipes_drecipe' => array(
      'machine_name' => 'drush_recipes_drecipe',
      'entity_type' => 'drush_recipes',
      'name' => 'drecipe',
      'label' => 'drecipe',
    ),
    'drush_recipes_recipe' => array(
      'machine_name' => 'drush_recipes_recipe',
      'entity_type' => 'drush_recipes',
      'name' => 'recipe',
      'label' => 'Recipe',
    ),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function drush_recipes_entity_eck_entity_type_info() {
  $items = array(
    'drush_recipes' => array(
      'name' => 'drush_recipes',
      'label' => 'Drush Recipes',
      'properties' => array(
        'uid' => array(
          'label' => 'Author',
          'type' => 'integer',
          'behavior' => 'author',
        ),
        'created' => array(
          'label' => 'Created',
          'type' => 'integer',
          'behavior' => 'created',
        ),
        'name' => array(
          'label' => 'Name',
          'type' => 'text',
          'behavior' => 'title',
        ),
      ),
    ),
  );
  return $items;
}
