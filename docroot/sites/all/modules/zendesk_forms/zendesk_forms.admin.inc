<?php
/**
 * @file
 * Admin pages file for the Zendesk Forms module.
 */

/**
 * Form for Zendesk settings.
 *
 * @ingroup forms.
 */
function zendesk_forms_settings_form($form, &$form_state) {
  $form['zendesk_forms_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('zendesk_forms_api_key', ''),
    '#description' => t('If you are not sure what your key is take a look here: !link', array('!link' => l('https://support.zendesk.com/entries/21733618-api-key', 'https://support.zendesk.com/entries/21733618-api-key'))),
  );
  $form['zendesk_forms_user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Zendesk username'),
    '#default_value' => variable_get('zendesk_forms_user_name', ''),
    '#description' => t('The username of your Zendesk agent. Usernames are e-mails in Zendesk.'),
  );
  $form['zendesk_forms_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Zendesk url'),
    '#default_value' => variable_get('zendesk_forms_url', ''),
    '#description' => t("Your url with Zendesk. The url is where you access your Zendesk backend. For example: https://reload.zendesk.com - please leave out trailing slash."),
  );
  return system_settings_form($form);
}
