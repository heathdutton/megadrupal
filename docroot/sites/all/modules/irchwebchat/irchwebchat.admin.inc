<?php

/**
 * @file
 * Irchwebchat admin functions
 */

/**
 * Admin form
 */
function irchwebchat_admin_page($form, &$form_state) {
  $form = array();
  $form['irchwebchat_common_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('IRC-Hispano Chat common settings'),
    '#weight' => -4,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['irchwebchat_common_settings']['irchwebchat_default_channel'] = array(
    '#type' => 'textfield',
    '#title' => t('Default channel'),
    '#default_value' => variable_get('irchwebchat_default_channel', '#irc-hispano'),
    '#size' => 40,
    '#maxlength' => 150,
    '#description' => t('Default channel to enter on connect.'),
  );

  $form['irchwebchat_page_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('IRC-Hispano Chat page settings'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['irchwebchat_page_settings']['irchwebchat_chat_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat width'),
    '#default_value' => variable_get('irchwebchat_chat_width', '590'),
    '#size' => 5,
    '#maxlength' => 4,
    '#description' => t('In pixels.'),
  );
  $form['irchwebchat_page_settings']['irchwebchat_chat_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat height'),
    '#default_value' => variable_get('irchwebchat_chat_height', '400'),
    '#size' => 5,
    '#maxlength' => 4,
    '#description' => t('In pixels.'),
  );

  $form['irchwebchat_block_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('IRC-Hispano Chat block settings'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['irchwebchat_block_settings']['irchwebchat_locked_channel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Forced channel'),
    '#default_value' => variable_get('irchwebchat_locked_channel', 0),
    '#description' => t("Users won't be able to change the default channel."),
  );
  $form['irchwebchat_block_settings']['irchwebchat_local_channel'] = array(
    '#type' => 'radios',
    '#title' => t('Local channel'),
    '#default_value' => variable_get('irchwebchat_local_channel', '1'),
    '#options' => array(
      '0' => t("Checkbox to enter local channel unchecked"),
      '1' => t('Checkbox to enter local channel checked (default)'),
      '2' => t("No checkbox to enter local channel and the user won't enter a local channel"),
    ),
    '#description' => t("Determines if the user will enter a local channel or if will be able to choose to enter or not."),
  );

  return system_settings_form($form);
} // function irchwebchat_admin_page()
