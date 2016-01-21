<?php
/**
 * @file
 * Page callback: ActiveCampaign connection settings.
 *
 * @see ac_newsletter_menu()
 */

/**
 * Form constructor for the ac_newsletter admin connect form.
 *
 * @see ac_newsletter_api_test_connection_submit()
 *
 * @ingroup forms
 */
function ac_newsletter_connect_admin_form($form, &$form_state) {

  // Load icons.
  $icons = _ac_newsletter_icons();
  $logo = $icons['ac-logo-icon'];
  $conn = $icons['connect-ac-icon'];

  // Add CSS file.
  $form['#attached']['css'] = array(drupal_get_path('module', 'ac_newsletter') . '/css/ac_newsletter.css');

  // Add fieldset.
  $form['active_campaign'] = array(
    '#type' => 'fieldset',
    '#title' => t('ActiveCampaign API connection settings'),
  );

  // Display ActiveCampaign logo and Connect icon.
  $ac_newsletter_url = check_url(variable_get('ac_newsletter_api_url', AC_NEWSLETTER_SITE_URL));
  $form['active_campaign']['ac_newsletter_logo'] = array(
    '#markup' => t('<div class="ac-newsletter-logo">!logo_url</div>', array(
      '!logo_url' => l($logo, AC_NEWSLETTER_SITE_URL, array('attributes' => array('target' => '_blank'), 'html' => TRUE)))),
  );
  $form['active_campaign']['ac_newsletter_conf'] = array(
    '#markup' => t('<div class="ac-newsletter-comm">!conf_url</div>', array(
      '!conf_url' => l($conn, $ac_newsletter_url, array('attributes' => array('target' => '_blank'), 'html' => TRUE)))),
  );

  // Field to provide ActiveCampaign API url.
  $form['active_campaign']['ac_newsletter_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API URL'),
    '#default_value' => variable_get('ac_newsletter_api_url', ''),
    '#required' => TRUE,
    '#size' => 70,
    '#description' => t('Provide API url of your ActiveCampaign Account. ex: https://ACCOUNT.api-us1.com'),
    '#attributes' => array('placeholder' => t('Enter ActiveCampaign API url')),
  );

  // Field to provide ActiveCampaign API key.
  $form['active_campaign']['ac_newsletter_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API KEY'),
    '#default_value' => variable_get('ac_newsletter_api_key', ''),
    '#required' => TRUE,
    '#size' => 70,
    '#description' => t('Provide API key of your ActiveCampaign Account. you can find in the !api of your account.', array(
      '!api' => l(t('API tab'), drupal_get_path('module', 'ac_newsletter') . '/images/ac-api-tab.png', array(
        'attributes' => array('target' => '_blank'))))),
    '#attributes' => array('placeholder' => t('Enter ActiveCampaign API key')),
  );

  // Button to test the above credentials.
  $form['active_campaign']['ac_newsletter_api_test_connection'] = array(
    '#type' => 'button',
    '#value' => t('Test Connection'),
    '#executes_submit_callback' => TRUE,
    '#submit' => array('ac_newsletter_api_test_connection_submit'),
  );

  return system_settings_form($form);
}

/**
 * Form test submission handler for ac_newsletter_connect_admin_form().
 */
function ac_newsletter_api_test_connection_submit($form, &$form_state) {
  variable_set('ac_newsletter_api_url', trim($form_state['values']['ac_newsletter_api_url']));
  variable_set('ac_newsletter_api_key', trim($form_state['values']['ac_newsletter_api_key']));
  // Test the credentials.
  if (_ac_newsletter_test_api_credentials()) {
    drupal_set_message(t('Successfully connected'));
  }
  else {
    drupal_set_message(t('Access denied: Invalid credentials (URL and/or API key).'), 'error');
  }
}
