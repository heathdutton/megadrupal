<?php
/**
 * @file
 * Pages implemented by UserVoice module.
 */

/**
 * Replace site-wide contact form with embedded UserVoice classic widget.
 *
 * @see uservoice_page_build()
 * @see https://pixelscrapper.uservoice.com/admin/settings/widgets/classic-widget-inline
 */
function uservoice_contact_page() {
  // Include the UserVoice JavaScript SDK. Only needed once per page, so don't
  // add it if the widget is already being added to this page (in which case
  // the SDK is added during hook_page_build()).
  if (!_uservoice_widget_visible()) {
    $uservoice_sdk = uservoice_get_sdk_script();
    $options = array(
      'type' => 'inline',
      'scope' => 'footer',
      'group' => JS_DEFAULT,
      'every_page' => FALSE,
      'weight' => 0,
      'defer' => FALSE,
      'cache' => TRUE,
      'preprocess' => FALSE,
    );
    drupal_add_js($uservoice_sdk, $options);
  }

  $mode = variable_get('uservoice_classic_widget_mode', USERVOICE_CLASSIC_WIDGET_DEFAULT_MODE);
  $primary_color = variable_get('uservoice_classic_widget_primary_color', USERVOICE_CLASSIC_WIDGET_DEFAULT_PRIMARY_COLOR);
  $link_color = variable_get('uservoice_classic_widget_link_color', USERVOICE_CLASSIC_WIDGET_DEFAULT_LINK_COLOR);
  $default_mode = variable_get('uservoice_classic_widget_default_mode', USERVOICE_CLASSIC_WIDGET_DEFAULT_TAB);
  $widget = '<div data-uv-inline="classic_widget" data-uv-mode="' . $mode . '" data-uv-primary-color="' . $primary_color . '" data-uv-link-color="' . $link_color . '" data-uv-default-mode="' . $default_mode . '" data-uv-width="100%" data-uv-height="550px"></div>';
  return array('#markup' => $widget);
}

/**
 * User profile gadget.
 */
function uservoice_user_profile_gadget() {
  // Check query parameters.
  $parameters = drupal_get_query_parameters();
  $mail = $parameters['email'];
  $given_sso_key = $parameters['sso'];
  $actual_sso_key = variable_get('uservoice_sso_key', '');
  if (empty($given_sso_key) || empty($mail) || $given_sso_key != $actual_sso_key) {
    watchdog('uservoice', USERVOICE_USER_PROFILE_GADGET_PATH . ' accessed without proper SSO key; access denied.', array(), WATCHDOG_NOTICE);
    print 'Acceess denied.';
    drupal_exit();
  }

  // Printing output here, rather than returning it, sends the output to the
  // page without any of the regular page theming.
  // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/
  // drupal_deliver_html_page/7
  // @see http://drupal.stackexchange.com/questions/21339/
  // blank-page-template-for-ajax-requests
  print theme('uservoice_user_profile_gadget', array('mail' => $mail));
}

/**
 * Order history gadget.
 */
function uservoice_order_history_gadget() {
  // Check query parameters.
  $parameters = drupal_get_query_parameters();
  $mail = $parameters['email'];
  $given_sso_key = $parameters['sso'];
  $actual_sso_key = variable_get('uservoice_sso_key', '');
  if (empty($given_sso_key) || empty($mail) || $given_sso_key != $actual_sso_key) {
    watchdog('uservoice', USERVOICE_ORDER_HISTORY_GADGET_PATH . ' accessed without proper SSO key; access denied.', array(), WATCHDOG_NOTICE);
    print 'Acceess denied.';
    drupal_exit();
  }

  // Printing output here, rather than returning it, sends the output to the
  // page without any of the regular page theming.
  // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/
  // drupal_deliver_html_page/7
  // @see http://drupal.stackexchange.com/questions/21339/
  // blank-page-template-for-ajax-requests
  print theme('uservoice_order_history_gadget', array('mail' => $mail));
}

/**
 * Process a webhook post from UserVoice.
 *
 * @see http://feedback.uservoice.com/knowledgebase/articles/49266-set-up-custom-web-service-hooks
 * @see mailchimp_process_webhook()
 */
function uservoice_process_webhook() {
  $json_data = $_POST['data'];
  $signature = $_POST['signature'];
  $event = $_POST['event'];

  // Check signature against uservoice sso key using hash_hmac.
  // See http://feedback.uservoice.com/knowledgebase/articles/
  // 49266-set-up-custom-web-service-hooks
  $sso_key = variable_get('uservoice_sso_key', '');
  if ($sso_key && $signature != hash_hmac('sha256', $json_data, $sso_key)) {
    watchdog('uservoice', 'uservoce_process_webhook accessed without proper signature; aborting. $_POST data follows: <pre>' . print_r($_POST, TRUE) . '</pre>', array(), WATCHDOG_NOTICE);
    return;
  }

  // Convert json object to php object.
  $php_data = json_decode($json_data);

  // Get email address to associate with this event (e.g. the user who submitted
  // the ticket).
  // @todo: double check this for all possible notification types.
  $mail = '';
  if ($php_data->ticket && $php_data->ticket->contact && $php_data->ticket->contact->email) {
    $mail = $php_data->ticket->contact->email;
  }

  // Load Drupal user based on email address.
  $uid = 0;
  if ($mail) {
    $account = user_load_by_mail($mail);
    if ($account) {
      $uid = $account->uid;
    }
  }

  // Save webhook data to webhook table.
  db_insert('uservoice_webhook')
  ->fields(array(
    'event' => $event,
    'data' => $json_data,
    'uid' => $uid,
    'mail' => $mail,
    'timestamp' => REQUEST_TIME,
  ))
  ->execute();

  // Log event.
  watchdog('uservoice', 'Webhook event @type has been processed.', array('@type' => $event), WATCHDOG_INFO);
}

/**
 * Redirect to a given destination after getting the current user's SSO token.
 *
 * Useful when providing links from Drupal to the UserVoice web portal, when
 * you want to send users with their single sign-on token. Instead of
 * customizing each link individually, you can simply create a link to, for
 * example: uservoice/sso-redirect?destination=http://mysubdomain.uservoice.com/
 * which will then send users to http://mysubdomain.uservoice.com/?sso=theirsso
 * so that they will be automatically signed into UserVoice.
 */
function uservoice_sso_redirect() {
  $parameters = drupal_get_query_parameters();
  if (!empty($parameters['destination'])) {
    global $user;
    $sso_token = uservoice_get_sso_token($user);
    drupal_goto($parameters['destination'], array('query' => array('sso' => $sso_token)));
  }
  else {
    drupal_set_message(t('Attempting to redirect with UserVoice sso key, but \'destination\' parameter is not set.'), 'error');
    drupal_goto('<front>');
  }
}
