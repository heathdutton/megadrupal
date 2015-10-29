<?php

/**
 * @file
 * Provide administration screens for ec_checkout module.
 */

/**
 * Form for determining the order of the checkout screens.
 *
 * @ingroup form
 */
function ec_checkout_admin_screen_form($form, &$form_state) {
  $form = array();

  $types = ec_checkout_get_types();

  $form['ec_checkout_weights'] = array(
    '#tree' => TRUE,
    '#theme' => 'ec_checkout_admin_screen_form',
  );

  foreach ($types as $type => $info) {
    $form['ec_checkout_weights'][$type] = array();
    $form['ec_checkout_weights'][$type]['name'] = array(
      '#markup' => $info->name,
    );
    $form['ec_checkout_weights'][$type]['description'] = array(
      '#markup' => $info->description,
    );
    $form['ec_checkout_weights'][$type]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => isset($info->weight) ? $info->weight : 0,
      '#delta' => count($types) > 10 ? count($types) + 1 : 10,
      '#attributes' => array('class' => array('screen-weight')),
    );
  }

  return system_settings_form($form);
}
