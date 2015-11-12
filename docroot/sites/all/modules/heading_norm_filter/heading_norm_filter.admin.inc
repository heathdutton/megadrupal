<?php

/**
 * @file
 * Provides admin form for Heading Normalizer
 */

/**
 * Form constructor to configre Heading Normalize settings
 *
 * @return
 *   array
 */
function heading_norm_filter_settings() {
  $form = array();

  if (!_heading_norm_filter_is_enabled()) {
    drupal_set_message(t('Heading Normalize is not yet enabled for at least one <a href="@input-formats-admin-path">input formats</a>.', array('@input-formats-admin-path' => url('admin/config/content/formats'))), 'error');
  }

  $form['heading_norm_filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Heading Normalize Settings'),
    '#collapsible' => TRUE,
    '#description' => t('General Settings'),
  );
  $form['heading_norm_filter'][HEADING_NORM_FILTER_VAR_NODE_START_LEVEL] = array(
    '#type' => 'select',
    '#title' => t('Set the Highest H-tag that should be allowed in Nodes'),
    '#description' => t('In HTML5, nodes should typically begin at H1.'),
    '#default_value' => variable_get(HEADING_NORM_FILTER_VAR_NODE_START_LEVEL, HEADING_NORM_FILTER_VAR_NODE_START_LEVEL_DEFAULT),
    '#options' => array(
      '1' => t('<h1>'),
      '2' => t('<h2>'),
      '3' => t('<h3>'),
      '4' => t('<h4>'),
      '5' => t('<h5>'),
      '6' => t('<h6>'),
    ),
  );
  $form['heading_norm_filter'][HEADING_NORM_FILTER_VAR_VIEW_START_LEVEL] = array(
    '#type' => 'select',
    '#title' => t('Set the Highest H-tag that should be allowed in Views'),
    '#description' => t('In HTML5, views should typically begin at H3.'),
    '#default_value' => variable_get(HEADING_NORM_FILTER_VAR_VIEW_START_LEVEL, HEADING_NORM_FILTER_VAR_VIEW_START_LEVEL_DEFAULT),
    '#options' => array(
      '1' => t('<h1>'),
      '2' => t('<h2>'),
      '3' => t('<h3>'),
      '4' => t('<h4>'),
      '5' => t('<h5>'),
      '6' => t('<h6>'),
    ),
  );
  return system_settings_form($form);
}
