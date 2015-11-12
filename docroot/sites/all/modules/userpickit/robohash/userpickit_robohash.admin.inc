<?php

/**
 * @file
 * Pages for User Pic Kit Robohash integration.
 */

/**
 * Global settings form for Robohash integration for User Pic Kit. 
 */
function userpickit_robohash_settings_form($form, $form_state) {
  $form['background'] = array(
    '#title' => t('Backgrounds'),
    '#description' => t('A background generated for a user.'),
    '#type' => 'select',
    '#options' => array(
      'transparent' => t('Transparent'),
      'background1' => t('Places'),
      'background2' => t('Geometric Shapes'),
    ),
    '#default_value' => variable_get(USERPICKIT_ROBOHASH_VARIABLE_BACKGROUND, 'transparent'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission callback
 *
 * @see userpickit_robohash_settings_form()
 */
function userpickit_robohash_settings_form_submit($form, &$form_state) {
  drupal_set_message('Configuration saved.');
  variable_set(USERPICKIT_ROBOHASH_VARIABLE_BACKGROUND, $form_state['values']['background']);
}