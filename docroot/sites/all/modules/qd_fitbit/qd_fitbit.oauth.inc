<?php
/**
 * @file
 * OAuth functions.
 */

/**
 * Retrieves an access client with the correct access token for a user.
 *
 * @param int $uid
 *   The UID of the Drupal user to get an access client for
 *
 * @return object
 *   The client object. Returns FALSE if there is a failure.
 */
function qd_fitbit_get_access_client($uid) {
  $retval = FALSE;

  $result = db_query("SELECT tid FROM {oauth_common_token} WHERE uid=" . $uid);

  if ($tid = $result->fetchAssoc()) {
    $tid = $tid['tid'];

    $key = variable_get('qd_fitbit_key');
    $consumer = DrupalOAuthConsumer::load($key, FALSE);
    $access_token = DrupalOAuthToken::loadById($tid, FALSE);

    $sig_method = DrupalOAuthClient::signatureMethod();
    $auth = new HttpClientOAuth($consumer, $access_token, $sig_method, TRUE, TRUE);
    $formatter = new HttpClientBaseFormatter(HttpClientBaseFormatter::FORMAT_JSON);

    $client = new HttpClient($auth, $formatter);

    $retval = $client;
  }

  return $retval;
}


/**
 * Handles the action to request OAuth access to the Fitbit service.
 */
function qd_fitbit_connect_submit($form, &$form_state) {
  global $base_url;

  $key = variable_get('qd_fitbit_key');
  $secret = variable_get('qd_fitbit_secret');

  $consumer = DrupalOAuthConsumer::load($key, FALSE);
  if (!$consumer) {
    $consumer = new DrupalOAuthConsumer($key, $secret);
    $consumer->write();
  }

  $sig_method = DrupalOAuthClient::signatureMethod();
  $client = new DrupalOAuthClient($consumer, NULL, $sig_method);

  try {
    $request_token = $client->getRequestToken(QD_FITBIT_REQUESTTOKENURL, array(
      'callback' => $base_url . '/qd_fitbit/oauth',
    ));

    $request_token->write();

    $auth_url = $client->getAuthorizationUrl(QD_FITBIT_AUTHORIZEURL, array(
      'callback' => $base_url . '/qd_fitbit/oauth',
    ));

    drupal_goto($auth_url);
  }
  catch (Exception $e) {
    drupal_set_message(t('There was an error communicating with the Fitbit service.'), 'error');
    drupal_set_message($e->getMessage());
  }
}


/**
 * Handles the action to delete the OAuth information.
 */
function qd_fitbit_disconnect_submit($form, &$form_state) {
  global $user;
  $uid = $user->uid;

  $key = variable_get('qd_fitbit_key');
  $secret = variable_get('qd_fitbit_secret');

  if (isset($key) && isset($secret)) {

    // TODO: This is not the appropriate way to remove the key.
    $result = db_query("DELETE from {oauth_common_token} WHERE uid=" . $uid);

    drupal_set_message('You have disconnected your Fitbit account.');
  }
  drupal_goto('//user/' . $user->uid);
}


/**
 * OAuth callback for Fitbit to request.
 */
function qd_fitbit_oauth_callback($form, &$form_state) {
  if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
    global $user;
    global $base_url;

    $oauth_token = $_GET['oauth_token'];
    $oauth_verifier = $_GET['oauth_verifier'];

    $key = variable_get('qd_fitbit_key');

    $consumer = DrupalOAuthConsumer::load($key, FALSE);

    $request_token = DrupalOAuthToken::loadByKey($oauth_token, $consumer, OAUTH_COMMON_TOKEN_TYPE_REQUEST);
    $client = new DrupalOAuthClient($consumer, $request_token);
    $access_token = $client->getAccessToken(QD_FITBIT_ACCESSTOKENURL,
      array(
        'verifier' => $oauth_verifier,
        'get' => TRUE)
      );

    $access_token->uid = $user->uid;
    $access_token->authorized = 1;
    $access_token->write(TRUE);

    $request_token->delete();

    drupal_set_message(t('You have successfully authorized access to Fitbit'));

    $client = qd_fitbit_get_access_client($user->uid);
    $fitbit_profile = qd_fitbit_get_user_info($client);
    $edit = array(
      'data' => array(
        'qd_fitbit_profile' => $fitbit_profile,
      ),
    );
    $account = user_save($user, $edit);

    // Fill up the import queues for this user.
    qd_fitbit_check_queues($account);
    drupal_set_message(t('Your data will beging syncing momentarily. This could take some time to process your entire history.'));
  }
  else {
    drupal_set_message(t('Fitbit did not return a valid token'), 'error');
  }

  drupal_goto($base_url . '/user/' . $user->uid);
}
