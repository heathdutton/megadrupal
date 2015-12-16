<?php

/**
 * @file
 * Administrative callbacks and form builder functions for the Commerce surcharge module.
 */


/**
 * Form builder: Configure form.
 */
function _commerce_surcharge_settings_form() {
  $form = array();

  $form['commerce_surcharge'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => 'settings',
  );

  // Current settings status.
  //$form['commerce_surcharge']['status'] = array(
  //'#type' => 'fieldset',
  //'#weight' => -1,
  //'#title' => t('Status'),
  //'#group' => 'commerce_surcharge',
  //);

  // Settings.
  $form['commerce_surcharge']['settings'] = array(
    '#type' => 'fieldset',
    '#weight' => 0,
    '#title' => t('Settings'),
    '#group' => 'commerce_surcharge',
  );

  $form['commerce_surcharge']['settings']['commerce_surcharge_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable credit card surcharge'),
    '#description' => t('Warning! Before enable this feature, please confirm with your merchant in legislation.'),
    '#default_value' => variable_get('commerce_surcharge_enabled', 0),
  );

  $form['commerce_surcharge']['settings']['commerce_surcharge_mode'] = array(
    '#type' => 'select',
    '#title' => t('Select surcharge type'),
    '#description' => t('Choose a fixed fees such as 1 dollar per transaction or a variable rate such as 2%.'),
    '#options' => array('fixed fees', 'variable rate'),
    '#default_value' => variable_get('commerce_surcharge_mode', 1),
    '#states' => array(
      'visible' => array(':input[name="commerce_surcharge_enabled"]' => array('checked' => TRUE)),
    ),
  );

  $form['commerce_surcharge']['settings']['commerce_surcharge_base'] = array(
    '#type' => 'textfield',
    '#title' => t('Set a base rate or fee'),
    '#description' => t('Set a base fixed fees such as 1 dollar per transaction or a variable rate such as 2%.'),
    '#default_value' => variable_get('commerce_surcharge_base', ''),
    '#size' => 10,
    '#maxlength' => 10,
    '#states' => array(
      'visible' => array(':input[name="commerce_surcharge_enabled"]' => array('checked' => TRUE)),
    ),
  );

  // Card rate settings.
  $form['commerce_surcharge']['card'] = array(
    '#type' => 'fieldset',
    '#weight' => 1,
    '#title' => t('Card settings'),
    '#description' => t('Set a specific card fixed fees or a variable rate, please check current surcharge settings. '),
    '#group' => 'commerce_surcharge',
  );


  $form['commerce_surcharge']['card']['container'] = array(
    '#type' => 'markup',
    '#prefix' => '<table>',
    '#suffix' => '</table>',
  );

  $credit_types = commerce_surcharge_credit_card_types();
  foreach ($credit_types as $name => $card) {
    $form['commerce_surcharge']['card']['container']['commerce_surcharge_card_' . $name] = array(
      '#type' => 'checkbox',
      '#title' => t('Set a rate for @card (only affects credit card @card)', array(
        '@card' => $card,
      )),
      '#prefix' => '<tr><td>',
      '#suffix' => '</td>',
      '#default_value' => variable_get('commerce_surcharge_card_' . $name, 0),
    );
    $form['commerce_surcharge']['card']['container']['commerce_surcharge_card_' . $name . '_rate'] = array(
      '#type' => 'textfield',
      '#title' => t('Rate', array(
        '@card' => $card,
      )),
      '#size' => 50,
      '#maxlength' => 10,
      '#prefix' => '<td>',
      '#suffix' => '</td></tr>',
      '#default_value' => variable_get('commerce_surcharge_card_' . $name . '_rate', ''),
    );
  }

  //Save into variables.
  $form = system_settings_form($form);
  // Return form.
  return $form;
}
