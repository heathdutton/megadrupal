<?php
/**
 * @file
 * Administrative forms and functions for the CMS Updater module.
 */

/**
 * Page callback: Configures administrative settings via system_settings_form().
 *
 * Define a form to set the settings for the CMS Updater.
 *
 * @see cms_updater_menu()
 * @see cms_updater_service_settings_form_validate()
 *
 * @return
 *   The general settings form code stored in the $form variable, before
 *   converted to HTML.
 */
function cms_updater_settings_form() {
  require_once 'conf/constants.php';

  // Defaults
  $domain = $GLOBALS['base_url'];
  $path = drupal_get_path('module', 'cms_updater');
  $key_check_result = FALSE;
  $renew = isset($_GET['renew']) ? TRUE : FALSE;
  $cmsu_payment_url = isset($_GET['cmsu_payment_url']) ? check_plain($_GET['cmsu_payment_url']) : FALSE;
  $current_path = $domain . '/?q=' . drupal_get_path_alias(current_path());

  // Add some css for the form
  drupal_add_css($path . '/css/cms_updater.css');
  // Attach js stuff
  $form['#attached']['js'] = array(drupal_get_path('module', 'cms_updater') . '/js/cms_updater.js');
  drupal_add_js(array('cms_updater' => array('service_url' => CMS_UPDATER_SERVICE_URL)), 'setting');

  // If a CMS Updater license key is set, check service if the key is valid
  $key = variable_get('cms_updater_key', '');
  if ($key && !isset($_SESSION['CMS_UPDATER_KEY_VALID'])) {
    $key_check_result = _cms_updater_request($key, 'check_key');
  }
  if (isset($_SESSION['CMS_UPDATER_KEY_VALID'])) {
    $key_check_result = TRUE;
    unset($_SESSION['CMS_UPDATER_KEY_VALID']);
  }

  // Create a css class indicator
  if ($key_check_result) {
    $check = 'approved';
  }
  else {
    $check = 'failed';
    $key = FALSE;
  }

  // Build the settings form
  $form['cms_updater_email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Email'),
    '#default_value' => variable_get('cms_updater_email', variable_get('site_mail')),
    '#description'   => t('This email address is used to receive reports and invoices from the CMS Updater service.'),
    '#required'      => TRUE,
  );

  $form['cms_updater_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Licence key'),
    '#default_value' => variable_get('cms_updater_key', ''),
    '#prefix'        => '<div class="cms-updater-key-wrapper">',
    '#required'      => TRUE,
    '#suffix'        => '<div class="' . $check . '"></div></div>',
  );

  // Show the date of expiry only when we have one
  if ($expiry = variable_get('cms_updater_expiry', '')) {
    $form['cms_updater_date_of_expiry'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Date of expiry'),
      '#default_value' => $expiry,
      '#description'   => t('Be sure to renew your subscription before the key is invalid.'),
      '#disabled'      => TRUE,
    );
    if (!$renew) {
      $link = l(t('Renew subscription'), $current_path, array('query'      => array('renew' => '1'),
                                                              'attributes' => array('class' => 'renew-link')
      ));
      $form['cms_updater_date_of_expiry']['#prefix'] = $link;
//      $form['cms_updater_date_of_expiry']['#suffix'] = $link.'</div>';
    }

  }

  // Show the iframe for the payment process of a license key, if the key is invalid or not set
  if (!$key || $renew || $cmsu_payment_url) {
    $form['cms_updater_service'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('CMS Updater - Buy licence key'),
      '#collapsible' => TRUE,
      '#collapsed'   => FALSE,
    );
    $basic_iframe_url = CMS_UPDATER_SERVICE_URL . '/buy/license?domain=' . $domain . '&path=' . $path . '&version=' . VERSION . '&major=' . CMS_UPDATER_CMS_ID . '&returnURL=' . $current_path . '&email=' . variable_get('cms_updater_email', variable_get('site_mail'));
    $iframe_url = $cmsu_payment_url ? CMS_UPDATER_SERVICE_URL . '/' . $cmsu_payment_url : $basic_iframe_url;

    $form['cms_updater_service']['iframe'] = array(
      '#markup' => '<iframe id="buy-cms-updater-service" src="' . $iframe_url . '" width="100%" ></iframe>',
    );
  }

  $form['cms_updater_key_validation'] = array(
    '#type'  => 'hidden',
    '#value' => $key_check_result,
  );

  // Add custom validation handler
  $form['#validate'][] = 'cms_updater_service_settings_form_validate';

  return system_settings_form($form);
}

/**
 * Validate the entered data of CMS Updater settings form
 *
 * @param $form
 * @param $form_state
 */
function cms_updater_service_settings_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  if (!valid_email_address($values['cms_updater_email'])) {
    form_set_error('cms_updater_email', t('No valid e-mail address.'));
  }
  // If key field was empty and is now filled OR is filled with an new value
  if ((!$form['cms_updater_key']['#default_value'] && $values['cms_updater_key']) ||
    ($form['cms_updater_key']['#default_value'] != $values['cms_updater_key'])
  ) {
    $key_check_result = _cms_updater_request($values['cms_updater_key'], 'check_key');
    if (!$key_check_result) {
      form_set_error('cms_updater_key', t('The CMS Updater license key is not valid or expired.'));
      $form['cms_updater_key']['#suffix'] = '<div class="failed"></div></div>';
    }
    else {
      // For performance reasons set this session variable
      $_SESSION['CMS_UPDATER_KEY_VALID'] = TRUE;
      // Save the expiry date for the given key
      $expiry = _cms_updater_request($values['cms_updater_key'], 'check_expiry');
      variable_set('cms_updater_expiry', $expiry);
      // Set the actual version of the CMS
      $expiry = _cms_updater_request($values['cms_updater_key'], 'set_version', FALSE);
    }
  }
  // Key was not changed an is valid
  if ($form['cms_updater_key']['#default_value'] == $values['cms_updater_key'] && $values['cms_updater_key_validation']) {
    if ($form['cms_updater_email']['#default_value'] != $values['cms_updater_email']) {
      $key_check_result = _cms_updater_request($values['cms_updater_key'], 'set_email', $values['cms_updater_email']);
      if ($key_check_result !== TRUE) {
        form_set_error('cms_updater_email', t('Sorry, could not send new e-mail address to CMS Updater service.'));
      }
    }

  }
  $form_state['redirect'] = 'admin/config/system/cms-updater';

}

/**
 * Helper function to send a request to the CMS Updater service and return the result
 *
 * @param $key
 * @param $endpoint
 * @param bool $mail
 * @return mixed
 */
function _cms_updater_request($key, $endpoint, $mail = FALSE) {
  require_once 'conf/constants.php';

  $domain = $GLOBALS['base_url'];
  $path = drupal_get_path('module', 'cms_updater');
  $check_url = CMS_UPDATER_SERVICE_URL . '/api/service/' . $endpoint . '.json';

  // Build data array
  $data = array(
    'path'   => $path,
    'domain' => $domain,
    'key'    => $key,
  );

  if($endpoint == 'set_version'){
    $data['cms'] = 1;
    $data['version'] = VERSION;
  }

  // Add email to data array if exist
  if ($mail) {
    $data['email'] = $mail;
  }

  // Make json
  $json_data = json_encode($data);

  // Prepare request.
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/json',
    ),
    'method'  => 'POST',
    'data'    => $json_data,
  );

  // Send request
  $result = drupal_http_request($check_url, $options);

  // Decode
  $key_check_result = json_decode($result->data);

  return $key_check_result[0];
}

