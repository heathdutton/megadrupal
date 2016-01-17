<?php

/**
 * @file
 * Dashboardify admin settings.
 */

/**
 * Admin settings form for Dashboardify module.
 */
function dashboardify_settings_form() {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic dashboardify settings'),
  );
  $form['settings']['dashboardify_landing_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make dashboard default landing page after login.'),
    '#default_value' => variable_get('dashboardify_landing_page', FALSE),
  );
  $form['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout options'),
  );
  $form['layout']['dashboardify_columns'] = array(
    '#type' => 'select',
    '#title' => t('Number of columns'),
    '#default_value' => variable_get('dashboardify_columns', 2),
    '#options' => array(
      '1' => 1,
      '2' => 2,
      '3' => 3,
      '4' => 4,
      '5' => 5,
      '6' => 6,
      '7' => 7,
      '8' => 8,
      '9' => 9,
    ),
    '#description' => t('Set the number of columns you want to activate.'),
  );
  $form['layout']['dashboardify_widths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom column widths'),
    '#description' => t('Optionally set custom widths for each available region.
      If these values are omitted, the regions will have 50% width'
    ),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  for ($i = 1; $i <= 9; $i++) {
    $form['layout']['dashboardify_widths']['dashboardify_width_' . $i] = array(
      '#type' => 'textfield',
      '#title' => t('Column #@number width', array('@number' => $i)),
      '#default_value' => variable_get('dashboardify_width_' . $i, '50'),
      '#size' => 10,
      '#field_suffix' => '%',
    );
  }
  return system_settings_form($form);
}
