<?php
/**
 * Form callback for commerce_userpoints_discount_menu().
 */
function commerce_userpoints_discount_settings_form($form, $form_state) {
  $form = array();
  $options = userpoints_get_categories();

  $form['commerce_userpoints_discount_default_termid'] = array(
    '#type' => 'select',
    '#title' => t('Set the default userpoint discount category.'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_userpoints_discount_default_termid', userpoints_get_default_tid()),
  );

  $form['commerce_userpoints_discount_total_price_components'] = array(
    '#type' => 'select',
    '#title' => t('Allowed price components'),
    '#default_value' => variable_get('commerce_userpoints_discount_total_price_components', array()),
    '#description' => t('Select the list of price component the credit can cover.'),
    '#options' => commerce_price_component_titles(),
    '#multiple' => TRUE,
    '#size' => min(12, count(commerce_price_component_titles())),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

