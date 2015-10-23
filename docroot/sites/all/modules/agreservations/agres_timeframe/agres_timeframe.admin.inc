<?php

/**
 * @file
 * Implements the agres_timeframe content type. This module was created by Amazeelabs and
 * given to me as a contribution. Thanks amazee labs!
 */

/**
 * Implements hook_settings_form().
 */
function agres_timeframe_settings_form($form, &$form_state) {
  $form['agres_timeframes_global_overlap'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow overlaping of global agres_timeframes (the ones that are not associated with any other nodes.)'),
    '#default_value' => variable_get('agres_timeframes_global_overlap', NULL),
  );
  $form['agres_timeframes_nodes_overlap'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow overlaping of referenced agres_timeframes (the ones that are assigned to nodes.)'),
    '#default_value' => variable_get('agres_timeframes_nodes_overlap', NULL),
  );
  return system_settings_form($form);
}