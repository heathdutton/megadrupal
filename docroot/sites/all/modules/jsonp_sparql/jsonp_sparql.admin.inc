<?php

/**
 * @file
 * Build a form to administer JSONP SPARQL settings.
 *
 * These functions provide a user interface for administering the blocks,
 * and adding new blocks of the JSONP SPARQL type.
 */

/**
 * Administration interface for the JSONP SPARQL module.
 */
function jsonp_sparql_admin_settings($form, &$form_state) {
  $path = libraries_get_path('jquery.jsonp');
  if (!file_exists($path . '/jquery-jsonp.min.js') || file_exists($path . '/jquery-jsonp.js')) {
    drupal_set_message(t('The JQuery JSONP plugin must be installed in order to do any client side queries. Please go to <a href="@page">the status report</a> for instructions.', array('@page' => url('admin/reports/status'))), 'warning', FALSE);
  }
  $form['jsonp_sparql_blocks_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of blocks'),
    '#default_value' => variable_get('jsonp_sparql_blocks_number', '1'),
    '#required' => TRUE,
    '#description' => t('Enter the number of blocks you want control over. The settings are controlled per block on the !block_page', array('!block_page' => l(t('block admin page'), 'admin/structure/block'))),
    '#element_validate' => array('element_validate_number'),
  );
  return system_settings_form($form);
}
