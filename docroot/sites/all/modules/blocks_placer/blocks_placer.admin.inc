<?php

/**
 * @file
 * Contains the administrative functions of the blocks_placer module.
 */

/**
 * URL to the jQuery UI Multiselect library website.
 */
define('BLOCKS_PLACER_JUMLIB_URL', 'https://github.com/crdeutsch/multiselect');


/**
 * Menu callback for the admin settings page.
 */
function blocks_placer_admin_settings() {

  // Add message concerning removed dependency.
  if (module_exists('jquery_ui_multiselect')) {
    drupal_set_message(t('The jQuery UI Multiselect module is no longer required by the Blocks Placer module and can be uninstalled along with its dependencies if not used elsewhere. The Javascript library is still required and its path must be specified under "Configuration".'));
  }

  $form['conf'] = array(
    '#title' => 'Configuration',
    '#type'  => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['conf']['blocks_placer_jumlib_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to jQuery UI Multiselect library'),
    '#description' => t('Path to where the /js and /css directories for the <a href="!url">jQuery UI Multiselect library</a> can be found. Without leading or trailing slash.', array('!url' => BLOCKS_PLACER_JUMLIB_URL)),
    '#default_value' => variable_get('blocks_placer_jumlib_path', 'sites/all/libraries/jquery_ui_multiselect'),
  );

  $form['settings'] = array(
    '#title' => 'Settings',
    '#type'  => 'fieldset',
    '#collapsible' => FALSE,
  );

  // Region selection.
  $theme = variable_get('theme_default', 'bartik');
  $regions_system = system_region_list($theme, REGIONS_VISIBLE);

  $form['settings']['blocks_placer_restricted_regions'] = array(
    '#type'  => 'select',
    '#title' => t('Disable regions in @active_theme', array('@active_theme' => $theme)),
    '#description' => t('Selected regions will <strong>not</strong> be available in the @module.', array('@module' => BLOCKS_PLACER_NAME_FRIENDLY)),
    '#default_value' => variable_get('blocks_placer_restricted_regions', array()),
    '#options' => $regions_system,
    '#size' => count($regions_system),
    '#multiple' => TRUE,
  );

  // Blocks to exclude (always on top).
  $query = db_select('block', 'b')->condition('theme', $theme);
  $query->addExpression("CONCAT(module, '_', delta)", 'delta');
  $result = $query->execute()->fetchCol();
  $blocks = array_combine($result, $result);

  $form['settings']['blocks_placer_topmost'] = array(
    '#type'  => 'select',
    '#title' => t('Blocks to keep topmost'),
    '#description' => t('Selected blocks will always be rendered first in regions.'),
    '#default_value' => variable_get('blocks_placer_topmost', array()),
    '#options' => $blocks,
    '#size' => 5,
    '#multiple' => TRUE,
  );

  $form['settings']['blocks_placer_block_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Block weights'),
    '#description' => 'A low weight places blocks before global blocks, for example "-500". A high weight places blocks after global blocks, for example "500". Use extreme values to be sure of placement.',
    '#default_value' => variable_get('blocks_placer_block_weight', -500),
    '#size' => 5,
  );

  // Set chosen container to full width.
  drupal_add_css('#edit_blocks_placer_topmost_chosen { width: 100% !important; }', 'inline');

  return system_settings_form($form);
}


/**
 * Menu callback for the region demo page.
 */
function blocks_placer_region_demo($theme = NULL) {
  drupal_add_css(drupal_get_path('module', 'blocks_placer') . '/css/blocks-placer.css');

  drupal_set_message(t('This page illustrates regions available for block placement. Using the @module-name you can place blocks in any of the regions outlined on this page. Note that the block layout is determined by the theme and can vary between regions.', array(
    '@module-name' => BLOCKS_PLACER_NAME_FRIENDLY,
  )));

  return '';
}
