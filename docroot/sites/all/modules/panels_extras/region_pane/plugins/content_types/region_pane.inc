<?php
// $Id: region_pane.inc,v 1.0 2011/07/25 16:23:48  chriscalip Exp $

/**
 * @file
 * Plugin to handle the 'region_pane' content type which allows the contents
 * of a drupal regions to be included in a panel
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Region Based Pane'),
  'description' => t('Get the content of an entire region'),    
  'single' => TRUE,
  'content_types' => array('region_pane_content_type'),
  'render callback' => 'region_pane_content_type_render',
  'edit form' => 'region_pane_content_type_edit_form',
  'category' => t('Panels Extras'),
  'admin title' => 'region_pane_content_type_admin_title',
);
/**
 * Output function for the 'region_pane' content type.
 *
 * Outputs the breadcrumb for the current page.
 * @param unknown_type $subtype
 * @param unknown_type $conf
 * @param unknown_type $panel_args
 * @return object $block
 */
function region_pane_content_type_render($subtype, $conf, $panel_args) {
  $region = $conf['config_item_region_pane_selected_region'];
  $region_content = block_get_blocks_by_region($region);
  $block = new stdClass();
  $block->content = $region_content;
  return $block;
}


/**
 * 'Edit form' callback for the content type.
 *
 */
function region_pane_content_type_edit_form($form, &$form_state) {
  $settings = $form_state['conf'];
  $themes_list = list_themes();
  $regions_list = array();
  foreach ($themes_list as $theme) {
    $regions_list = array_merge($regions_list, system_region_list($theme->name));
  }
  $form['config_item_region_pane_selected_region'] = array(
    '#type' => 'select',
    '#default_value' => $settings['config_item_region_pane_selected_region'],
    '#title' => t('Region List'),
    '#options' => $regions_list,
  );
  return $form;
}

/**
 * Submit function, note anything in the formstate[conf] automatically gets saved 
 */
function region_pane_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['config_item_region_pane_selected_region'] = $form_state['values']['config_item_region_pane_selected_region'];
}

/**
 * Callback to provide the administrative title of region_pane content type.
 */
function region_pane_content_type_admin_title($subtype, $conf) {
  $output = t('Region Based Pane');
  if (!empty($conf['config_item_region_pane_selected_region'])) {
    $output .= ' : ' . t($conf['config_item_region_pane_selected_region']);
  }
  return $output;
}