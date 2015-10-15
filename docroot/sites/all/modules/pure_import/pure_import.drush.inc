<?php
/**
 * @file
 * Drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function pure_import_drush_command() {
  $items = array();

  // Import runner command.
  $items['pure-import-migrate'] = array(
    'description' => 'Pure Import migration runner.',
    'arguments' => array(
      'class' => 'Runner class of migration (implementing IPureImportRunnable).',
    ),
    'options' => array(
      'limit' => array(
        'description' => 'Limit of the migrated items.',
        'example-value' => '100',
      ),
    ),
    'examples' => array(
      'drush pim PureImportBlogPost --limit=10' => 'Import 10 items using the PureImportBlogPost class.',
    ),
    'aliases' => array('pim'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  return $items;
}

/**
 * Validation of the import runner drush command.
 *
 * @param $class
 *  Class name of the import.
 * @return bool
 */
function drush_pure_import_migrate_validate($class) {
  if (!$class || !class_exists($class)) {
    return drush_set_error('Pure Import', dt('Missing class: @class.', array('@class' => $class)));
  }
}

/**
 * Callback for the import runner command.
 *
 * @param $class
 *  Class name of the import.
 */
function drush_pure_import_migrate($class) {
  $limit = (int) drush_get_option('limit');

  pure_import_start_migration_with_source($class, $limit);
}
