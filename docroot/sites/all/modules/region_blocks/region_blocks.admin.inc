<?php
/**
 * @file
 * Generate the module config page.
 */

/**
 * Set up the config page.
 */
function region_blocks_config() {
  $form = array();
  $regions = system_region_list(variable_get('theme_default', ''));
  $cache_options = array(
    DRUPAL_NO_CACHE => 'DRUPAL_NO_CACHE',
    DRUPAL_CACHE_CUSTOM => 'DRUPAL_CACHE_CUSTOM',
    DRUPAL_CACHE_PER_ROLE => 'DRUPAL_CACHE_PER_ROLE',
    DRUPAL_CACHE_PER_USER => 'DRUPAL_CACHE_PER_USER',
    DRUPAL_CACHE_PER_PAGE => 'DRUPAL_CACHE_PER_PAGE',
  );

  $form['region_blocks__blocks'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Create blocks for following regions:'),
    '#default_value' => variable_get('region_blocks__blocks', array(0)),
    '#options' => $regions,
  );

  $form['region_blocks__cache'] = array(
    '#type' => 'select',
    '#title' => t('Default block cache setting:'),
    '#default_value' => variable_get('region_blocks__cache', DRUPAL_NO_CACHE),
    '#options' => $cache_options,
  );

  return system_settings_form($form);
}
