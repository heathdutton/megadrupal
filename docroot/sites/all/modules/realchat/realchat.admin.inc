<?php
/**
 * @file
 * RealChat settings page
 */

/**
 * Form builder function for admin settings.
 */
function realchat_settings() {
  $form['realchat_chat_url_tf'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat URL'),
    '#default_value' => variable_get('realchat_chat_url', ''),
    '#description' => t('Chat URL gotten from the administration URL: Client->Code Generator'),
  );
  $form['realchat_auth_key_tf'] = array(
    '#type' => 'textfield',
    '#title' => t('Authentication key'),
    '#default_value' => variable_get('realchat_auth_key', ''),
    '#description' => t('Leave empty for no authentication.  Else Must match the Authentication Key specified in the RealChat Control Center, Server Settings.'),
  );
  $form['realchat_close_url_tf'] = array(
    '#type' => 'textfield',
    '#title' => t('Close URL'),
    '#default_value' => variable_get('realchat_close_url', ''),
    '#description' => t('The url to be opened after the client exits the chat session.'),
  );
  $form['#submit'][] = 'realchat_settings_submit';
  return system_settings_form($form);
}

/**
 * Set 'realchat_chatURL', 'realchat_authKey', & 'realchat_closeURL'
 * based on the three textfields.
 */
function realchat_settings_submit($form, &$form_state) {

  if (isset($form_state['values']['realchat_chat_url_tf'])) {
    $tf = $form_state['values']['realchat_chat_url_tf'];
    variable_set('realchat_chat_url', $tf);
  }
  else {
    variable_del('realchat_chat_url');
    drupal_set_message('<p>' . t('No chat URL given, no chatting possible.') . '</p>');
  }

  if (isset($form_state['values']['realchat_auth_key_tf'])) {
    $tf = $form_state['values']['realchat_auth_key_tf'];
    variable_set('realchat_auth_key', $tf);
  }
  else {
    variable_del('realchat_auth_key');
    drupal_set_message('<p>' . t('No authorization key given,  authenticated chatting not possible .') . '</p>');
  }

  if (isset($form_state['values']['realchat_close_url_tf'])) {
    $tf = $form_state['values']['realchat_close_url_tf'];
    variable_set('realchat_close_url', $tf);
  }
  else {
    variable_del('realchat_close_url');
    drupal_set_message('<p>' . t('No close URL given, users will return to the site\'s front page.') . '</p>');
  }
}
