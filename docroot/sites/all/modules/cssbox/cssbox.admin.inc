<?php

/**
 * @file
 * Administrative page callbacks for the CssBox module.
 */

/**
 * General configuration form for controlling the CssBox behaviour.
 */
function cssbox_admin_settings() {

  $form['cssbox_features'] = array(
    '#type' => 'fieldset',
    '#title' => t('CssBox features'),
  );
  $form['cssbox_features']['cssbox_target_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Target ID for closing popup.'),
    '#default_value' => variable_get('cssbox_target_id', 'main'),
    '#size' => 30,
    '#description' => t('After clicking Close button, this is a target anchor to point. Hash # will be added automatically.'),
  );
  $form['cssbox_features']['cssbox_show_nextprev'] = array(
    '#type' => 'radios',
    '#title' => t('Show next, previous buttons'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
    '#default_value' => variable_get('cssbox_show_nextprev', 'yes'),
  );
  $form['cssbox_features']['cssbox_show_caption'] = array(
    '#type' => 'radios',
    '#title' => t('Show image caption'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
    '#default_value' => variable_get('cssbox_show_caption', 'yes'),
  );

  $form['cssbox_buttons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Buttons'),
  );
  $form['cssbox_buttons']['cssbox_text_previous'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous'),
    '#default_value' => variable_get('cssbox_text_previous', '« Prev'),
    '#size' => 30,
    '#description' => t('Text for the previous button within a group.'),
  );
  $form['cssbox_buttons']['cssbox_text_next'] = array(
    '#type' => 'textfield',
    '#title' => t('Next'),
    '#default_value' => variable_get('cssbox_text_next', 'Next »'),
    '#size' => 30,
    '#description' => t('Text for the next button within a group.'),
  );
  $form['cssbox_buttons']['cssbox_text_close'] = array(
    '#type' => 'textfield',
    '#title' => t('Close'),
    '#default_value' => variable_get('cssbox_text_close', 'Close'),
    '#size' => 30,
    '#description' => t('Text for the close button.'),
  );

  return system_settings_form($form);
}
