<?php

/**
 * @file
 * Custom oauth2 class to connect to wunderlist.
 */

/**
 * {@inheritdoc}
 */
class Wunderlist extends OAuth2\Client {

  /**
   * {@inheritdoc}
   */
  protected function getTokenServerSide() {
    if (!isset($_GET['code'])) {
      drupal_goto($this->getAuthenticationUrl());
    }
    else {
      return $this->getToken(array(
          'grant_type' => 'authorization_code',
          'code' => $_GET['code'],
          'client_id' => $this->params['client_id'],
          'client_secret' => $this->params['client_secret'],
          'redirect_uri' => $this->params['redirect_uri'],
        ));
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function getToken($data) {

    if (isset($data['scope']) and $data['scope'] == NULL) {
      unset($data['scope']);
    }

    $client_id = $this->params['client_id'];
    $client_secret = $this->params['client_secret'];
    $token_endpoint = $this->params['token_endpoint'];

    $data_string = drupal_http_build_query($data);

    $options = array(
      'method' => 'POST',
      'data' => $data_string,
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded',
        'Authorization' => 'Basic ' . base64_encode("$client_id:$client_secret"),
      ),
    );

    $result = drupal_http_request($token_endpoint, $options);

    if ($result->code != 200) {
      throw new \Exception(
        t("Failed to get an access token of grant_type @grant_type.\nError: @result_error",
          array(
            '@grant_type' => $data['grant_type'],
            '@result_error' => $result->error,
          ))
      );
    }
    return (Array) json_decode($result->data);
  }

  /**
   * {@inheritdoc}
   */
  public function getAccessToken() {
    // Check wheather the existing token has expired.
    // We take the expiration time to be shorter by 10 sec
    // in order to account for any delays during the request.
    // Usually a token is valid for 1 hour, so making
    // the expiration time shorter by 10 sec is insignificant.
    // However it should be kept in mind during the tests,
    // where the expiration time is much shorter.
    $expiration_time = $this->token['expiration_time'];
    if ($expiration_time > (time() + 10)) {
      // The existing token can still be used.
      return $this->token['access_token'];
    }

    try {
      // Try to use refresh_token.
      $token = $this->getTokenRefreshToken();
    }
    catch (\Exception $e) {
      // Get a token.
      switch ($this->params['auth_flow']) {
        case 'client-credentials':
          $token = $this->getToken(array(
                     'grant_type' => 'client_credentials',
                     'scope' => $this->params['scope'],
                   ));
          break;

        case 'user-password':
          $token = $this->getToken(array(
                     'grant_type' => 'password',
                     'username' => $this->params['username'],
                     'password' => $this->params['password'],
                     'scope' => $this->params['scope'],
                   ));
          break;

        case 'server-side':
          $token = $this->getTokenServerSide();

          break;

        default:
          $token = NULL;
          break;
      }
    }

    if (is_null($token)) {
      throw new \Exception(t('Unknown authorization flow "!auth_flow". Suported values for auth_flow are: client-credentials, user-password, server-side.',
              array('!auth_flow' => $this->params['auth_flow'])));
    }

    $token['expiration_time'] = REQUEST_TIME + 3600;

    // Store the token (on session as well).
    $this->token = $token;
    $_SESSION['oauth2_client']['token'][$this->id] = $token;

    // Redirect to the original path (if this is a redirection
    // from the server-side flow).
    self::redirect();

    // Return the token.
    return $token['access_token'];
  }

  /**
   * Create a webhook for a list.
   *
   * Webhooks are triggered when a list is changed.
   *
   * @param int $list_id
   *   The list id to update.
   *
   * @return int
   *   The webhook id.
   */
  public function createWebhook($list_id) {
    global $base_url;

    $access_token = variable_get('wunderlist_token');
    $client_id = variable_get('wunderlist_client_id');
    $data = array(
      'list_id' => $list_id,
      'url' => $base_url . '/wunderlist/list/update/' . $list_id,
      'processor_type' => 'generic',
      'configuration' => '',
    );
    $url = variable_get('wunderlist_api_url', '') . '/webhooks';
    $headers = array(
      'X-Access-Token' => $access_token,
      'X-Client-ID' => $client_id,
    );
    $params = drupal_http_build_query($data);
    $response = drupal_http_request($url,
      array('headers' => $headers, 'method' => 'POST', 'data' => $params)
    );

    if ($response->code == 201) {
      $data = drupal_json_decode($response->data);
      return $data['id'];
    }
    else {
      $error = isset($response->error) ? $response->error : '';
      watchdog('wunderlist', 'Could not create webhook due to "%error"',
        array('%error' => $response->code . ' ' . $error));
    }
    return 0;
  }

  /**
   * Get webhooks for a list id.
   *
   * @param int $list_id
   *   The list id.
   *
   * @return array
   *   array with list ids.
   */
  public function getWebhooks($list_id) {
    $access_token = variable_get('wunderlist_token');
    $client_id = variable_get('wunderlist_client_id');
    $url = variable_get('wunderlist_api_url', '') . '/webhooks?list_id=' . $list_id;
    $headers = array(
      'X-Access-Token' => $access_token,
      'X-Client-ID' => $client_id,
    );
    $response = drupal_http_request($url,
      array('headers' => $headers, 'method' => 'GET')
    );
    if ($response->code == 200) {
      return drupal_json_decode($response->data);
    }
    return array();
  }

  /**
   * Deletes a webhook.
   *
   * @param int $revision_id
   *   The webhook id to delete.
   */
  public function deleteWebhook($revision_id) {
    $access_token = variable_get('wunderlist_token');
    $client_id = variable_get('wunderlist_client_id');
    $url = variable_get('wunderlist_api_url', '') . '/webhooks/' . $revision_id;
    $headers = array(
      'X-Access-Token' => $access_token,
      'X-Client-ID' => $client_id,
    );
    $response = drupal_http_request($url,
      array('headers' => $headers, 'method' => 'DELETE')
    );

    if ($response->code != 204) {
      $error = isset($response->error) ? $response->error : '';
      watchdog('wunderlist', 'Could not delete webhook due to "%error"',
        array('%error' => $response->code . ' ' . $error));
    }
  }

  /**
   * Returns a list from Wunderlist.
   *
   * @return array
   *   list array
   */
  public function getList() {
    $access_token = variable_get('wunderlist_token');
    $client_id = variable_get('wunderlist_client_id');
    $url = variable_get('wunderlist_api_url', '') . '/lists';
    $headers = array(
      'X-Access-Token' => $access_token,
      'X-Client-ID' => $client_id,
    );
    $response = drupal_http_request($url, array('headers' => $headers));

    if ($response->code == 200) {
      $data = drupal_json_decode($response->data);
    }
    else {
      $data = array();
      $error = isset($response->error) ? $response->error : '';
      watchdog('wunderlist', 'Could not fetch list due to "%error"',
        array('%error' => $response->code . ' ' . $error));
    }

    return $data;
  }

  /**
   * Return the task from one list item.
   *
   * @param int $id
   *   List id.
   *
   * @return array
   *   List array
   */
  public function getTasks($id) {
    $access_token = variable_get('wunderlist_token');
    $client_id = variable_get('wunderlist_client_id');
    $params = drupal_http_build_query(array('list_id' => $id));

    $url = variable_get('wunderlist_api_url', '') . '/tasks?' . $params;

    $headers = array(
      'X-Access-Token' => $access_token,
      'X-Client-ID' => $client_id,
      'Content-Type' => 'application/json',
    );

    $response = drupal_http_request($url, array('headers' => $headers));

    if ($response->code == 200) {
      $data = drupal_json_decode($response->data);
      foreach ($data as &$task) {
        $avatar = $this->getAvatar($task['created_by_id']);
        $task['avatar'] = $avatar;
      }
    }
    else {
      $data = array();
      watchdog('wunderlist', 'Could not fetch task due to "%error"',
        array(
          '%error' => $response->code . ' ' . $response->error . ': ' . $response->data,
        ));
    }

    return $data;
  }

  /**
   * Get the avatar of the user.
   *
   * @param int $user_id
   *   The id from the user.
   * @param int $size
   *   The size of the image.
   *
   * @return string
   *   The base64 encoded image.
   */
  public function getAvatar($user_id = NULL, $size = 64) {
    $access_token = variable_get('wunderlist_token');
    $client_id = variable_get('wunderlist_client_id');

    if (empty($user_id)) {
      $user = $this->getUser();
      $user_id = $user['id'];
    }

    $params = drupal_http_build_query(array('user_id' => $user_id, 'size' => $size));
    $url = variable_get('wunderlist_api_url', '') . '/avatar?' . $params;

    $headers = array(
      'X-Access-Token' => $access_token,
      'X-Client-ID' => $client_id,
    );

    $response = drupal_http_request($url, array(
      'headers' => $headers,
      'max_redirects' => 3,
    ));

    if ($response->code == 200) {
      $image = base64_encode($response->data);
    }
    else {
      $image = '';
      watchdog('wunderlist', 'Could not fetch image due to "%error"',
        array('%error' => $response->code . ' ' . $response->error));
    }

    return $image;
  }

  /**
   * Get authenticated Wunderlist user.
   *
   * @return array
   *   user array
   */
  public function getUser() {
    $access_token = variable_get('wunderlist_token');
    $client_id = variable_get('wunderlist_client_id');

    $url = variable_get('wunderlist_api_url', '') . '/user';

    $headers = array(
      'X-Access-Token' => $access_token,
      'X-Client-ID' => $client_id,
    );

    $response = drupal_http_request($url, array('headers' => $headers));

    if ($response->code == 200) {
      $data = drupal_json_decode($response->data);
      variable_set('wunderlist_user', $data);
    }
    else {
      $data = NULL;
      watchdog('wunderlist', 'Could not fetch image due to "%error"',
        array('%error' => $response->code . ' ' . $response->error));
    }

    return $data;
  }

}
