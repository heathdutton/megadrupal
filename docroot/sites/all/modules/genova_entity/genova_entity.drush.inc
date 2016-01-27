<?php
/**
 * @file
 *   Drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function genova_entity_drush_command() {
  $items = array();

  $items['gen-entity-module-create'] = array(
    'callback' => 'genova_drush_entity_module_create',
    'description' => "Creates basic Drupal entity module structure.",
    'aliases' => array('gen-emc'),
    'arguments' => array(
      'name' => 'The name of the entity module to create',
    ),
//    'options' => array(
//      'components' => 'Components that will be present in the module.',
//    ),
    'examples' => array(
      'drush gen-entity-module-create somemodule' =>
      'Will create a entity module named "somemodule".',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
  );

  return $items;
}

/**
 * Command callback for gen-entity-module-create.
 *
 * @param string $module_name
 *   Module name.
 *
 * @return bool
 *   Command result.
 *
 * @see genova_drush_module_create().
 */
function genova_drush_entity_module_create($module_name = NULL) {

  if (empty($module_name)) {
    $arguments = drush_get_arguments();
    $module_name = $arguments[1];
  }

  if (module_exists($module_name)) {
    drush_print(strtr('Module @name already exists. Skipping process.', array('@name' => $module_name)));
    return FALSE;
  }

  $messenger = new GenDrushMessenger();
  genova_set_messenger($messenger);

  try {
    $module_path = genova_get_module_path($module_name);

    // If module directory was just created then we need to create some subdirs
    // inside of it.
    if (!file_exists($module_path . '/views')) {
      mkdir($module_path . '/views');
      mkdir($module_path . '/views/handlers');
    }
    if (!file_exists($module_path . '/includes')) {
      mkdir($module_path . '/includes');
    }

    $module_component = new GenEntityModule();
    $module_component->drushInit($module_name, $module_path);
    $module_component->validateInputs();
    $module_component->buildComponent();
    $module_component->onSuccess();
  }
  catch (GenException $e) {
    return drush_set_error('ERROR', $e->getMessage());
  }

  return TRUE;
}
