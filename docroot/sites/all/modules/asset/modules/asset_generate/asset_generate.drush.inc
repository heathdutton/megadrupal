<?php

/**
 * @file
 *  Generate content, taxonomy, menu, and users via drush framework.
 */

/**
 * Implementation of hook_drush_command().
 */
function asset_generate_drush_command() {
  $items['generate-assets'] = array(
    'description' => 'Create assets.',
    'arguments' => array(
      'num_assets' => 'Number of assets to generate.',
    ),
    'options' => array(
      'kill' => 'Delete all assets before generating new ones.',
      'types' => 'A comma delimited list of asset types to generate.',
    ),
    'aliases' => array('gena'),
  );
  return $items;
}

/**
 * Command callback. Generate a number of nodes and comments.
 */
function drush_asset_generate_generate_assets($num_assets = NULL) {
  if (drush_generate_is_number($num_assets) == FALSE) {
    return drush_set_error('DEVEL_GENERATE_INVALID_INPUT', dt('Invalid number of assets'));
  }

  $values['values']['kill_assets'] = drush_get_option('kill');
  $values['values']['title_length'] = 6;
  $values['values']['num_assets'] = $num_assets;
  $types = implode(',', array_keys(asset_type_get_names()));
  $values['values']['asset_types'] = drupal_map_assoc(explode(',', drush_get_option('types', $types)));
  $asset_types = array_filter($values['values']['asset_types']);
  if (!empty($values['values']['kill_content']) && empty($asset_types)) {
    return drush_set_error('DEVEL_GENERATE_INVALID_INPUT', dt('Please provide asset types (--types) in which you want to delete the assets.'));
  }
  drush_generate_include_devel();
  asset_generate_assets($values);
}
