<?php

/**
 * Menu callback; The page for processing OAuth login transactions.
 *
 * @param $action
 *   The action being requested. Currently supports the following:
 *    - connect: Initiate Facebook connection and log a user in.
 *    - deauth: Remove the exisitng Facebook connection access.
 * @param Invite $invite
 */
function invite_fboauth_action_page($action, $invite) {
  module_load_include('pages.inc', 'invite', 'includes/invite');
  module_load_include('fboauth.inc', 'fboauth', 'includes/fboauth');

  $app_id = variable_get('fboauth_id', '');
  $app_secret = variable_get('fboauth_secret', '');

  if ($access_token = _invite_fboauth_access_token($_REQUEST['code'], $action['name'], $app_id, $app_secret)) {
    fboauth_action_invoke($action['name'], $app_id, $access_token);
    invite_accept($invite);
  }

  return 'Only Facebook will ever see this page - humans go away!';
}

/**
 * Copy of fboauth_access_token().
 *
 * @TODO: Delete it when #2311659 patch is committed.
 */
function _invite_fboauth_access_token($code, $action_name, $app_id = NULL, $app_secret = NULL) {
  // Use the default App ID and App Secret if not specified.
  $app_id = isset($app_id) ? $app_id : variable_get('fboauth_id', '');
  $app_secret = isset($app_secret) ? $app_secret : variable_get('fboauth_secret', '');

  // Note that the "code" provided by Facebook is a hash based on the client_id,
  // client_secret, and redirect_url. All of these things must be IDENTICAL to
  // the same values that were passed to Facebook in the approval request. See
  // the fboauth_link_properties function.
  $query = array(
    'client_id' => $app_id,
    'client_secret' => $app_secret,
    'redirect_uri' => fboauth_action_url($_GET['q'], array('absolute' => TRUE, 'query' => !empty($_GET['destination']) ? array('destination' => $_GET['destination']) : array())),
    'code' => $code,
  );
  $token_url = url('https://graph.facebook.com/oauth/access_token', array('absolute' => TRUE, 'query' => $query));
  $authentication_result = drupal_http_request($token_url);

  if ($authentication_result->code != 200) {
    $error = !empty($authentication_result->error) ? $authentication_result->error : t('(no error returned)');
    $data = !empty($authentication_result->data) ? print_r($authentication_result->data, TRUE) : t('(no data returned)');
    watchdog('fboauth', 'Facebook OAuth could not acquire an access token from Facebook. We queried the following URL: <code><pre>@url</pre></code>. Facebook\'s servers returned an error @error: <code><pre>@return</pre></code>', array('@url' => $token_url, '@error' => $error, '@return' => $data));
  }
  else {
    // The result from Facebook comes back in a query-string-like format,
    // key1=value1&key2=value2. Parse into an array.
    $authentication_strings = explode('&', $authentication_result->data);
    $authentication_values = array();
    foreach ($authentication_strings as $authentication_string) {
      list($authentication_key, $authentication_value) = explode('=', $authentication_string);
      $authentication_values[$authentication_key] = $authentication_value;
    }
  }

  return isset($authentication_values['access_token']) ? $authentication_values['access_token'] : NULL;
}
