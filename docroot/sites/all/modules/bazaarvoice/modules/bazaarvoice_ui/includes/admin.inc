<?php
/**
 * @file
 * Provides the Bazaarvoice administrative interface.
 */

/**
 * Admin settings form.
 */
function bazaarvoice_ui_admin_settings_form($form, &$form_state) {

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account Settings'),
    '#description' => t('Bazaarvoice account settings'),
    '#collapsible' => TRUE,
  );

  $form['account']['bazaarvoice_mode'] = array(
    '#type' => 'select',
    '#title' => t('Account Mode'),
    '#description' => t('Mode to use for connecting to Bazaarvoice. Use staging for pre-production development and testing.'),
    '#options' => array(
      'stg' => t('Staging'),
      'prod' => t('Production'),
    ),
    '#default_value' => variable_get('bazaarvoice_mode', 'stg'),
  );

  $form['account']['bazaarvoice_client_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Client code'),
    '#required' => TRUE,
    '#description' => t('The client code for the Bazaarvoice account.'),
    '#default_value' => variable_get('bazaarvoice_client_code', ''),
  );

  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Conversations API Settings'),
    '#description' => t('Settings for connecting to the Bazaarvoice Conversations API.'),
    '#collapsible' => TRUE,
  );

  $form['api']['bazaarvoice_pass_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Pass Key'),
    '#required' => TRUE,
    '#description' => t('API key is required to authenticate API user and check permission to access particular client data.  If you do not have an API key you can get one !url', array('!url' => l(t('here'), 'https://developer.bazaarvoice.com/apps/register'))),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => variable_get('bazaarvoice_pass_key'),
  );

  $form = system_settings_form($form);

  return $form;
}

/**
 * Locale settings form.
 */
function bazaarvoice_ui_admin_locale_form($form, &$form_state) {
  $form = array();
  // Build a table for configuring locales.
  $form['locales_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Language Locales'),
    '#description' => t('Configure locale codes for each language enabled on the site. Bazaarvoice expects the locale to be formatted as [language code]_[COUNTRY CODE] (example: en_US)'),
  );

  $form['locales_set']['locales'] = array(
    '#theme' => 'bazaarvoice_ui_admin_locale_form_locales',
    '#tree' => TRUE,
  );

  // Get site languages.
  $languages = bazaarvoice_get_site_languages();
  // Get site locales.
  $locales = bazaarvoice_get_locales();
  // Get default locale.
  $default_locale = '';
  // Get list of default locale options.
  $default_locales = array();
  // Loop through each language.
  foreach ($languages as $lang_code => $language) {
    $default_locales[$lang_code] = $language->name;
    $form['locales_set']['locales'][$lang_code] = array(
      '#title' => check_plain(t('!title locale', array('!title' => $language->name))),
      '#title_display' => 'invisible',
      '#type' => 'textfield',
      // If have the locale, print only the country code.
      '#field_prefix' => $lang_code . '_',
      '#description' => t('Provide the uppercase 2 letter country code for this locale.'),
      '#max_length' => 2,
    );

    if (isset($locales[$lang_code])) {
      $form['locales_set']['locales'][$lang_code]['#default_value'] = str_replace($lang_code . '_', '', $locales[$lang_code]['locale']);

      if ($locales[$lang_code]['is_default']) {
        $default_locale = $lang_code;
      }
    }
  }

  $form['locales_set']['locales']['locale_default'] = array(
    '#type' => 'radios',
    '#title' => t('Default locale'),
    '#title_display' => 'invisible',
    '#options' => $default_locales,
    '#default_value' => $default_locale,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );

  $form['#validate'][] = 'bazaarvoice_ui_admin_locale_form_validate';
  $form['#submit'][] = 'bazaarvoice_ui_admin_locale_form_submit';

  return $form;
}

/**
 * Validation callback for locale form.
 */
function bazaarvoice_ui_admin_locale_form_validate($form, &$form_state) {
  // Get site languages.
  $languages = bazaarvoice_get_site_languages();
  // Loop through the submitted locales.
  foreach ($form_state['values']['locales'] as $lang_code => $locale) {
    // Is there a locale value?
    if (isset($languages[$lang_code]) && $locale) {
      // Is it not two capital letters?
      if (!preg_match('/^[A-Z]{2}$/', $locale)) {
        form_set_error('locales][' . $lang_code, t('Invalid country code provided for @language'));
      }
    }
  }

  // Get default locale language and make sure a value was submitted.
  if (isset($form_state['values']['locales']['locale_default'])) {
    if (empty($form_state['values']['locales'][$form_state['values']['locales']['locale_default']])) {
      form_set_error('locales][locale_default', t('Default must have a valid locale code.'));
    }
  }
}

/**
 * Submit callback for locale form.
 */
function bazaarvoice_ui_admin_locale_form_submit($form, &$form_state) {
  $default_locale = $form_state['values']['locales']['locale_default'];
  // Get the enabled site languages.
  $languages = bazaarvoice_get_site_languages();
  // Array to hold locales to create/update.
  $locales = array();
  // Locales to delete.
  $delete = array();
  // Loop through each language.
  foreach ($languages as $lang_code => $language) {
    // Locale submitted?
    if (isset($form_state['values']['locales'][$lang_code]) && !empty($form_state['values']['locales'][$lang_code])) {
      // Add to the locales array.
      $locales[$lang_code] = array(
        'locale' => $lang_code . '_' . trim($form_state['values']['locales'][$lang_code]),
        'is_default' => ($default_locale == $lang_code) ? 1 : 0,
      );
    }
    else {
      $delete[] = $lang_code;
    }
  }

  // Save the locales.
  bazaarvoice_set_locales($locales);
  // Delete unset locales.
  bazaarvoice_delete_locales($delete);
  drupal_set_message(t('Locales have been saved.'));
}
