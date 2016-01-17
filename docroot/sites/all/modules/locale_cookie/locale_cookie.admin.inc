<?php

/**
 * The URL language provider configuration form.
 */
function locale_cookie_language_providers_cookie_form($form, &$form_state) {
  $form['locale_cookie_language_negotiation_cookie_param'] = array(
    '#title' => t('Request/cookie parameter'),
    '#type' => 'textfield',
    '#default_value' => variable_get('locale_cookie_language_negotiation_cookie_param', 'language'),
    '#description' => t('Name of the request/cookie parameter used to determine the desired language.'),
  );

  $form['locale_cookie_language_negotiation_cookie_expire'] = array(
    '#title' => t('Expire'),
    '#type' => 'textfield',
    '#default_value' => variable_get('locale_cookie_language_negotiation_cookie_expire', 7),
    '#description' => t('The time (in days) in which the cookie should expire.'),
  );

  $form_state['redirect'] = 'admin/config/regional/language/configure';

  return system_settings_form($form);
}