<?php
/**
 * @file
 *   Viddler API V2 functions.
 *
 * @link http://developers.viddler.com/documentation/api-v2/ Viddler API Version 2 documentation. @endlink
 *
 */

define('VIDDLER_API_V2_ENDPOINT', 'http://api.viddler.com/api/v2/');

/**
 * Initiates a new Viddler session.
 *
 * Uses the viddler.users.auth method to authenticate against Viddler.
 * No parameters, but it picks up the username, password, API key and SSL option from the config settings in the database.
 * If the SSL option is set, authentication takes place over https.
 *
 * @return string
 *   Returns a session id string on success and FALSE on failure.
 *
 */
function scald_viddler_authenticate() {
  $viddler_settings = variable_get('scald_viddler_authentication', array());
  $username = isset($viddler_settings['username']) ? $viddler_settings['username'] : '';
  $password = isset($viddler_settings['password']) ? $viddler_settings['password'] : '';
  $use_ssl = isset($viddler_settings['use_ssl']) ? $viddler_settings['use_ssl'] : '1';
  $api_key = isset($viddler_settings['api_key']) ? $viddler_settings['api_key'] : '';
  $options = array(
    'query' => array(
      'key' => $api_key,
      'user' => $username,
      'password' => $password,
      'get_record_token' => '1',
    ),
  );

  $endpoint = VIDDLER_API_V2_ENDPOINT;
  if ($use_ssl) {
    $endpoint = str_replace('http', 'https', $endpoint);
    $options['https'] = TRUE;
  }

  $url = url($endpoint . 'viddler.users.auth.json', $options);
  $response = drupal_http_request($url);

  if (isset($response->data)) {
    $response_data = drupal_json_decode($response->data);
  }
  else {
    $response_data = array();
  }

  if ($response->code != 200) {
    // Check what the API error code was.
    $code = isset($response_data['error']['code']) ? $response_data['error']['code'] : 'Unknown';
    $description = isset($response_data['error']['description']) ? $response_data['error']['description'] : 'Unknown';
    watchdog('scald_viddler', 'Authentication failed. Code: @code, Description: @description, URL: @url', array('@code' => $code, '@description' => $description, '@url' => $url), WATCHDOG_ERROR, NULL);
    return FALSE;
  }
  else {
    // Authorisation successful.
    // If we have a sessionid, return it.
    if (isset($response_data['auth']['sessionid']) && !empty($response_data['auth']['sessionid'])) {
      return $response_data['auth']['sessionid'];
    }
    // No sessionid set, so return false.
    return FALSE;
  }
}

/**
 * Executes an API call to Viddler.
 *
 * @param $method
 *   A string containing the API method to call.
 * @param $query
 *   (optional) An array of key => value pairs to pass as arguments to the API method.
 *   The API key and session id get added automatically, so they can be left out of the query array.
 *
 * @return
 *   Returns an array containing results of the API call on success and FALSE on failure.
 *
 */
function scald_viddler_call_method($method = NULL, $query = array()) {
  $session_id = scald_viddler_authenticate();

  // Quit if authentication failed.
  if (!$session_id) return FALSE;

  // Quit if no method is passed.
  if (!$method) return FALSE;

  // Quit if the method isn't valid
  $valid_methods = scald_viddler_valid_methods();
  if (!array_key_exists($method, $valid_methods)) return FALSE;

  // Grab the API key.
  $viddler_settings = variable_get('scald_viddler_authentication', array());
  $api_key = isset($viddler_settings['api_key']) ? $viddler_settings['api_key'] : '';

  // Add the extra info to the query array.
  $query['key'] = $api_key;
  if ($valid_methods[$method]) {
    // Only add the sessionid if the method requires it.
    $query['sessionid'] = $session_id;
  }

  // Call the API method.
  $url = url(VIDDLER_API_V2_ENDPOINT . $method . '.json', array('query' => $query));
  $response = drupal_http_request($url);
  $success = FALSE;

  // Decode the returned json data.
  if (isset($response->data)) {
    $response_data = drupal_json_decode($response->data);
  }
  else {
    $response_data = array();
  }

  if ($response->code == 200) {
    // API call succeeded.
    return $response_data;
  }
  else {
    // API call failed.
    $code = isset($response_data['error']['code']) ? $response_data['error']['code'] : 'Unknown';
    $description = isset($response_data['error']['description']) ? $response_data['error']['description'] : 'Unknown';
    watchdog('scald_viddler', 'API call to @method failed. Code: @code, Description: @description, URL: @url', array('@method' => $method, '@code' => $code, '@description' => $description, '@url' => $url), WATCHDOG_ERROR, NULL);
    return FALSE;
  }
}

/**
 * Generates an array of valid Viddler API methods.
 *
 * @return
 *   Returns an array of 'method name' => 'session id required?' pairs.
 *
 */
function scald_viddler_valid_methods() {
  return array(
    'viddler.api.echo' => FALSE,
    'viddler.api.getInfo' => FALSE,
    'viddler.encoding.getSettings' => TRUE,
    'viddler.encoding.getStatus' => TRUE,
    'viddler.encoding.getStatus2' => TRUE,
    'viddler.encoding.encode' => TRUE,
    'viddler.encoding.cancel' => TRUE,
    'viddler.encoding.setSettings' => TRUE,
    'viddler.playlists.addVideo' => TRUE,
    'viddler.playlists.create' => TRUE,
    'viddler.playlists.setDetails' => TRUE,
    'viddler.playlists.delete' => TRUE,
    'viddler.playlists.getDetails' => TRUE,
    'viddler.playlists.list' => TRUE,
    'viddler.playlists.getByUser' => TRUE,
    'viddler.playlists.moveVideo' => TRUE,
    'viddler.playlists.removeVideo' => TRUE,
    'viddler.resellers.getSubaccounts' => TRUE,
    'viddler.resellers.removeSubaccounts' => TRUE,
    'viddler.videos.addClosedCaptioning' => TRUE,
    'viddler.videos.comments.add' => TRUE,
    'viddler.videos.comments.get' => TRUE,
    'viddler.videos.comments.remove' => TRUE,
    'viddler.videos.delClosedCaptioning' => TRUE,
    'viddler.videos.getDetails' => TRUE,
    'viddler.videos.setClosedCaptioning' => TRUE,
    'viddler.videos.setDetails' => TRUE,
    'viddler.videos.delete' => TRUE,
    'viddler.videos.delFile' => TRUE,
    'viddler.users.getProfile' => TRUE,
    'viddler.users.logout' => TRUE,
    'viddler.users.setProfile' => TRUE,
    'viddler.users.register' => TRUE,
    'viddler.users.getSettings' => TRUE,
    'viddler.users.setSettings' => TRUE,
    'viddler.videos.getAdsStatus' => TRUE,
    'viddler.videos.disableAds' => TRUE,
    'viddler.videos.enableAds' => TRUE,
    'viddler.videos.getByUser' => TRUE,
    'viddler.videos.getEmbedCodeTypes' => TRUE,
    'viddler.videos.getEmbedCode' => TRUE,
    'viddler.videos.prepareUpload' => TRUE,
    'viddler.videos.uploadProgress' => TRUE,
    'viddler.videos.setPermalink' => TRUE,
    'viddler.videos.getRecordToken' => TRUE,
    // 'viddler.users.auth' => FALSE,
    'viddler.users.getPlayerBranding' => TRUE,
    'viddler.videos.search' => TRUE,
    'viddler.users.setPlayerBranding' => TRUE,
    'viddler.videos.setThumbnail' => TRUE,
  );
}
