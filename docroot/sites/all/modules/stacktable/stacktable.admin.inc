<?php

/**
 * @file
 * Administrative pages of stacktable module
 */

function stacktable_admin_settings_form() {
  $form = array();

  $form['stacktable_forms'] = array(
    '#type' => 'textarea',
    '#title' => t('Forms'),
    '#description' => t('A list of form IDs. One form ID per line. Also, you can specify tables which should be handled (for example: commerce_cart_form_@.views-table or commerce_checkout_form_complete@table)'),
    '#default_value' => variable_get('stacktable_forms', ''),
  );

  $form['stacktable_show_form_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show form ids'),
    '#description' => t('Will show a list of form IDs present on the particular page. <strong>Be careful, because all users (including anonymous) will see a debug messages when this option is enabled.</strong>'),
    '#default_value' => variable_get('stacktable_show_form_ids', 0),
  );
  return system_settings_form($form);
}