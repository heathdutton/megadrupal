<?php
/**
 * @file
 * Wrappers Delight Drush command
 */

/**
 * Implements hook_drush_command().
 */
function entity_formatter_drush_command() {
  $items = array();

  $items['entity-formatter-make'] = array(
    'description' => "Makes a formatter class for an entity.",
    'arguments' => array(
      'entity_type' => 'Entity type to wrap.',

    ),
    'options' => array(
      'bundles' => array(
        'description' => 'Entity bundles',
        'example-value' => 'page,article',
      ),
      'module' => array(
        'description' => 'Output module name',
        'example-value' => 'entity_formatter_classes',
      ),
      'destination' => array(
        'description' => 'Destination parent directory',
        'example-value' => 'profiles/yourapp/modules/custom'
      ),
      'force' => array(
        'description' => 'Force updating all class methods',
      ),
    ),
    'examples' => array(
      'drush efm node --bundles=page,article --module=entity_formatter_classes' => 'Generate formatter classes for page and article node types and place them in the entity_formatter_classes module (generating module if not exists).',
    ),
    'aliases' => array('efm'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function entity_formatter_drush_help($section) {
  switch ($section) {
    case 'drush:entity-formatter-make':
      return dt("This command will create or update a formatter class for an entity.");
  }
}

/**
 * Implements drush_hook_COMMAND().
 *
 * Create wrapper classes for an entity type.
 *
 * @param $entity_type
 *   Entity type to create wrappers for
 *
 * @see drush_invoke()
 * @see drush.api.php
 */
function drush_entity_formatter_make($entity_type) {

  $entity_info = entity_get_info($entity_type);
  if (empty($entity_info)) {
    drush_set_error('ENTITY_TYPE_NOT_EXISTS', dt('Entity type @type does not exist.', array('@type' => $entity_type)));
    return;
  }

  $bundles = NULL;
  if ($bundle_string = drush_get_option('bundles')) {
    $bundles = explode(',', $bundle_string);
  }
  $module_name = drush_get_option('module');
  $destination = drush_get_option('destination');
  $force = drush_get_option('force');

  entity_formatter_make_formatters($entity_type, $bundles, $module_name, $destination, $force);
}



















