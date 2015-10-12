<?php

/**
 * @file
 * Yandex XML administration interface.
 */

/**
 * Settings form builder.
 */
function yandex_xml_settings_form($form, &$form_state) {
  $search_url = variable_get('yandex_xml_search_url');
  $form['yandex_xml_search_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Search URL'),
    '#description' => yandex_xml_help_message(),
    '#required' => TRUE,
    '#size' => 90,
    '#attributes' => array('required' => TRUE, 'placeholder' => 'http://xmlsearch.yandex.<domain>/xmlsearch?user=<username>&key=<key>'),
    '#default_value' => $search_url,
  );
  $form['yandex_xml_grpoups_on_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Items per page'),
    '#description' => t('The maximum number of posts displayed on overview search results pages.'),
    '#size' => 30,
    '#default_value' => variable_get('yandex_xml_grpoups_on_page', 10),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['yandex_xml_site_autodetect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autodetect site'),
    '#description' => t('If checked Drupal base url will be used.'),
    '#size' => 30,
    '#default_value' => variable_get('yandex_xml_site_autodetect'),
  );

  $form['yandex_xml_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Site'),
    '#description' => t("Leave empty if you won't restrict the search to the web site only."),
    '#size' => 30,
    '#default_value' => variable_get('yandex_xml_site_url'),
    '#attributes' => array('placeholder' => 'http://example.com/directory'),
    '#states' => array(
      'invisible' => array(':input[name="yandex_xml_site_autodetect"]' => array('checked' => TRUE)),
    ),
  );

  return system_settings_form($form);
}

/**
 * Settings form validation.
 */
function yandex_xml_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!valid_url($values['yandex_xml_search_url'], TRUE)) {
    form_set_error('yandex_xml_search_url', t('The search URL is not valid.'));
  }
  else {

    try {
      $search = new YandexXml($values['yandex_xml_search_url']);
      $search->query('test');
      $search->send();
    }
    catch (Exception $e) {
      form_set_error('yandex_xml_search_url', t('Search request failed: %error_message', array('%error_message' => $e->getMessage())));
    }

    if ($search->error) {
      require_once 'yandex_xml.pages.inc';
      $error_message = yandex_xml_error_messages($search->error);
      form_set_error('yandex_xml_search_url', t('Search request failed: %error_message', array('%error_message' => $error_message)));
    }
  }

  if ($values['yandex_xml_site_url'] && !valid_url($values['yandex_xml_site_url'], TRUE)) {
    form_set_error('yandex_xml_site_url', t('The site is not valid.'));
  }
  if ($values['yandex_xml_grpoups_on_page'] > 100) {
    form_set_error('yandex_xml_grpoups_on_page', t('Items per page must be less or equal to 100.'));
  }
}

/**
 * Return help text about Yandex XML registration.
 */
function yandex_xml_help_message() {
  global $language;
  $registration_link = l(t('register'), YandexXml::getRegistrationHelpUrl($language->language));
  return t('To set up and start using Yandex.XML you should !link the IP address that you plan to send search requests from.', array('!link' => $registration_link));
}
