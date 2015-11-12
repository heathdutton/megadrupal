<?php
/**
 * @file
 * Admin pages file for the Webform Zendesk module.
 */

/**
 * Form for Zendesk settings.
 */
function webform_zendesk_settings_form($form, &$form_state) {
  $form['webform_zendesk_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Zendesk url'),
    '#default_value' => variable_get('webform_zendesk_url', ''),
    '#description' => t("Your Zendesk backend URL. The URL is where you access your Zendesk backend. For example: https://mydomain.zendesk.com - please leave out trailing slash."),
  );
  $form['webform_zendesk_user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Zendesk username'),
    '#default_value' => variable_get('webform_zendesk_user_name', ''),
    '#description' => t('The username of your Zendesk account. Usernames are e-mails in Zendesk.'),
  );
  $form['webform_zendesk_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('webform_zendesk_api_key', ''),
    '#description' => t('You can get your API Key from https://mydomain.zendesk.com/agent/admin/api. You can read more about the details  !link', array('!link' => l(t('here'), 'https://support.zendesk.com/entries/21733618-api-key'))),
  );
  return system_settings_form($form);
}
