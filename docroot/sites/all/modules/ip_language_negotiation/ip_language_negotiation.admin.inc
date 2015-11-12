<?php

/**
 * @file
 * Admin settings for the IP Language Negotiation.
 */

/**
 * Admin form for the IP Language Negotiation settings.
 */
function ip_language_negotiation_form($form, &$form_state) {
  require_once DRUPAL_ROOT . '/includes/iso.inc';
  $countries = country_get_list();
  $languages = locale_language_list();
  $language_default = language_default();
  $settings = variable_get('ip_language_negotiation_countries', array());

  $form['intro'] = array(
    '#markup' => t('Use the interface below to select the default language per country. You only have to set the exceptions, because the default language will be used as fall-back.'),
  );

  // Remove the default language.
  unset($languages[$language_default->language]);

  $letter = '';
  $default_option = array(
    '' => t('Default (@default_language)', array(
      '@default_language' => $language_default->name,
    )),
  );
  foreach ($countries as $country_code => $country) {
    // Remove accents so we can sort countries correctly.
    $current_letter = iconv('UTF-8', 'ASCII//TRANSLIT', drupal_substr($country, 0, 1));

    if ($letter != $current_letter) {
      $letter = $current_letter;
      if (empty($form['ip_language_letter_' . $letter])) {
        $form['ip_language_letter_' . $letter] = array(
          '#type' => 'fieldset',
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#title' => t('Countries with the letter %letter', array('%letter' => $letter)),
        );
      }
    }
    $form['ip_language_letter_' . $letter][$country_code] = array(
      '#type' => 'radios',
      '#options' => array_merge($default_option, $languages),
      '#title' => $country,
      '#default_value' => '',
    );
    if (!empty($settings[$country_code])) {
      $form['ip_language_letter_' . $letter][$country_code]['#default_value'] = $settings[$country_code];
      $form['ip_language_letter_' . $letter]['#collapsed'] = FALSE;
    }
  }

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit handler for the settings form.
 */
function ip_language_negotiation_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  variable_set('ip_language_negotiation_countries', $form_state['values']);
}
