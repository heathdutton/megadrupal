<?php

/**
 * Menu callback; mojeID admin settings form.
 */
function mojeid_registration_admin_settings() {

  if (!variable_get('https', FALSE)) {
    drupal_set_message(t("HTTPS isn't enabled. Please <a href=\"@url\">configure your web server to allow SSL connection</a> and place the following code at the end of site's settings.php file: <code>\$conf['https'] = TRUE;</code>", array('@url' => 'http://drupal.org/https-information')), 'warning');
  }

  $form['mojeid_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable registration motivation programme'),
    '#description' => t('Enable motivation program. See this link for details: <a href="@url">Incentive programme for web service providers</a>.', array('@url' => 'http://www.mojeid.cz/page/877/')),
    '#default_value' => variable_get('mojeid_registration', TRUE),
  );

  $form['mojeid_registration_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration page'),
  );

  $form['mojeid_registration_page']['mojeid_registration_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => _mojeid_registration_page_get('title'),
  );

  $form['mojeid_registration_page']['mojeid_registration_page_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Body'),
    '#rows' => 10,
    '#default_value' => _mojeid_registration_page_get('body'),
    '#format' => _mojeid_registration_page_get('body_format'),
  );

  $form['mojeid_registration_page']['mojeid_registration_page_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => _mojeid_registration_page_get('path'),
  );

  $form['#submit'][] = 'mojeid_registration_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Validation handler for the mojeID admin settings form.
 *
 * @see mojeid_registration_admin_settings()
 */
function mojeid_registration_admin_settings_validate($form, &$form_state) {
  $values =& $form_state['values'];

  if (!empty($values['mojeid_registration'])) {
    if (empty($values['mojeid_registration_page_title'])) {
      form_set_error('mojeid_registration_page_title', t("When motivation programme is enabled, page title is required."));
    }
    if (empty($values['mojeid_registration_page_path'])) {
      form_set_error('mojeid_registration_page_path', t("When motivation programme is enabled, page path is required."));
    }
  }
}

/**
 * Submit handler for the mojeID admin settings form.
 *
 * @see mojeid_registration_admin_settings()
 */
function mojeid_registration_admin_settings_submit($form, &$form_state) {
  $values =& $form_state['values'];

  if (isset($values['mojeid_registration_page_body']['value'])) {
    $body = $values['mojeid_registration_page_body'];
    $values['mojeid_registration_page_body'] = $body['value'];
    $values['mojeid_registration_page_body_format'] = $body['format'];
  }

  // Rebuild menu when some important setting has been changed.
  if ($values['mojeid_registration'] != variable_get('mojeid_registration', TRUE)
      || $values['mojeid_registration_page_title'] != _mojeid_registration_page_get('title')
      || $values['mojeid_registration_page_path'] != _mojeid_registration_page_get('path')
  ) {
    // We have to change settings before we rebuild the menu.
    variable_set('mojeid_registration', $values['mojeid_registration']);
    variable_set('mojeid_registration_page_title', $values['mojeid_registration_page_title']);
    variable_set('mojeid_registration_page_path', $values['mojeid_registration_page_path']);
    menu_rebuild();
  }
}
