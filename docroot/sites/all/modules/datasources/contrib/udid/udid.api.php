<?php

/**
 * Defines one or more datasources.
 *
 * Note: The ids should be valid PHP identifiers of max 32 characters.
 *
 * @return array
 *   An associative array of datasources, keyed by a unique
 *   identifier and containing associative arrays with the following keys:
 *   - name: The datasource translated name.
 *   - description [optional]: A translated string for description.
 *   - branches [optional]: array of branches (see hook_udid_databranches())
 *     You can provide a branch keyed 'main' for the main branch. If not it will be created.
 *   - branch_default [optional]: the key of the default databranch, default is main.
 *   - id_label [optional]: the translated label of the id field.
 *   - version_label [optional]: the translated label of the version field.
 *   - version_default [optional]: a callback to generate version field if empty see udid_defaut_version().
 *   - version_view [optional]: show version, default is FALSE or TRUE if a callback is given.
 *   - branch_view [optional]: show branch, default is FALSE or TRUE if more than 1 branch.
 *   - version_edit [optional]: edit version, default is FALSE.
 *   - branch_edit [optional]: edit branch, default is FALSE or TRUE if more than 1 branch.
 *   - compare [optional]: compare udid, default is udid_default_compare().
 *   
 * @see udid_datasources_registry()
 */
function hook_udid_datasources() {
  $sources['example_some_datasource'] = array(
    'name' => t('Some Datasource'),
    'description' => t('Description of some Datasource.'),
    'branches' => array(
      'example_some_databranch' => array(
      	'name' => t('Some Databranch'),
      	'description' => t('Description of some Databranch.'),
       ),
    ),
    'branch_default' => 'example_some_databranch',
  );
  $sources['example_other_datasource'] = array(
    'name' => t('Other Datasource'),
    'description' => t('Description of another Datasource.'),
  );

  return $sources;
}

/**
 * Allow module to alter the datasource registry.
 * 
 * @param array &$sources
 * 
 * @see udid_datasources_registry()
 */
function hook_udid_datasources_alter(array &$sources) {
  $sources['example_some_datasource']['version_edit'] = TRUE;
}

/**
 * Add one or more databranches.
 * This will be added to the 'branches' param.
 *
 * Note: The ids should be valid PHP identifiers of max 32 characters.
 *
 * @return array
 *   An associative array of databranches, keyed by databranch and by a unique
 *   identifier in a datasource and containing associative arrays with the following keys:
 *   - name: The databranch translated name.
 *   - description [optional]: A translated string for description.
 *   - weight [optional]: to alter the default branch order.
 *   
 * @see udid_datasources_registry()
 */
function hook_udid_databranches() {
  $branches['example_some_datasource']['example_some_databranch'] = array(
    'name' => t('Some Databranch'),
    'description' => t('Description of some Databranch.'),
    'weight' => 10,
  );
  $branches['example_some_datasource']['example_other_databranch'] = array(
    'name' => t('Other Databranch'),
    'description' => t('Description of another Databranch.'),
  );

  return $branches;
}