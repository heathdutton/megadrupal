<?php

/**
 * @file
 * Bitbucket OAuth2 client.
 */

class OpenIDConnectBitbucketClient extends OpenIDConnectClientBase {

  /**
   * A mapping of OpenID Connect user claims to Bitbucket user properties.
   *
   * @see https://confluence.atlassian.com/display/BITBUCKET/users+Endpoint
   *
   * @var array
   */
  protected $userInfoMapping = array(
    'name' => 'display_name',
    'sub' => 'uuid',
    'preferred_username' => 'username',
    'website' => 'website',
  );

  /**
   * {@inheritdoc}
   */
  public function authorize($scope = 'openid email') {
    // Convert scopes for Bitbucket compatibility. Note that (at the time of
    // writing) Bitbucket's OAuth2 API does not support scopes.
    $mapping = array(
      'openid' => NULL,
      'email' => 'email',
      'profile' => 'account',
    );
    $scopes = explode(' ', $scope);
    foreach ($scopes as $key => $current) {
      if (!array_key_exists($current, $mapping)) {
        continue;
      }
      elseif ($mapping[$current] === NULL) {
        unset($scopes[$key]);
      }
      else {
        $scopes[$key] = $mapping[$current];
      }
    }
    $scope = implode(' ', $scopes);

    parent::authorize($scope);
  }

  /**
   * {@inheritdoc}
   */
  public function retrieveUserInfo($access_token) {
    $data = parent::retrieveUserInfo($access_token);
    if (!$data) {
      return FALSE;
    }

    $claims = array();
    foreach ($this->userInfoMapping as $claim => $key) {
      if (array_key_exists($key, $data)) {
        $claims[$claim] = $data[$key];
      }
    }

    // The avatar is inside the user resource links.
    if (!isset($claims['picture']) && isset($data['links']['avatar']['href'])) {
      $claims['picture'] = $data['links']['avatar']['href'];
    }

    // The email address is not provided in the /user resource. So we need to
    // make another request to find out the user's email address(es).
    if (empty($claims['email'])) {
      $endpoints = $this->getEndpoints();
      $request_options = array(
        'method' => 'GET',
        'timeout' => 15,
        'headers' => array(
          'Accept' => 'application/json',
          'Authorization' => 'Bearer ' . $access_token,
        ),
      );
      $url = str_replace(':uuid', $data['uuid'], $endpoints['emails']);
      $email_response = drupal_http_request($url, $request_options);
      if (isset($email_response->error) || $email_response->code != 200) {
        openid_connect_log_request_error(__FUNCTION__, $this->name, $email_response);

        return FALSE;
      }
      // See https://confluence.atlassian.com/display/BITBUCKET/emails+Resource
      $emails = drupal_json_decode($email_response->data);
      foreach ($emails as $email) {
        if (!empty($email['primary'])) {
          $claims['email'] = $email['email'];
          $claims['email_verified'] = $email['active'];
          break;
        }
      }

      // A Bitbucket user may not have a 'primary' address, if no email address
      // is verified yet.
      if (empty($claims['email']) && count($emails)) {
        $email = reset($emails);
        $claims['email'] = $email['email'];
        $claims['email_verified'] = $email['active'];
      }
    }

    return $claims;
  }

  /**
   * {@inheritdoc}
   */
  public function getEndpoints() {
    return array(
      'authorization' => 'https://bitbucket.org/site/oauth2/authorize',
      'token' => 'https://bitbucket.org/site/oauth2/access_token',
      'userinfo' => 'https://api.bitbucket.org/2.0/user',
      'emails' => 'https://api.bitbucket.org/1.0/users/:uuid/emails',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function decodeIdToken($id_token) {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function retrieveTokens($authorization_code) {
    // Exchange `code` for access token and ID token.
    $redirect_uri = OPENID_CONNECT_REDIRECT_PATH_BASE . '/' . $this->name;
    $post_data = array(
      'code' => $authorization_code,
      'grant_type' => 'authorization_code',
      'redirect_uri' => url($redirect_uri, array('absolute' => TRUE)),
    );
    $request_options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($post_data),
      'timeout' => 15,
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded',
        'Accept' => 'application/json',
        'Authorization' => 'Basic ' . base64_encode($this->getSetting('client_id') . ':' . $this->getSetting('client_secret')),
      ),
    );
    $endpoints = $this->getEndpoints();
    $response = drupal_http_request($endpoints['token'], $request_options);
    if (!isset($response->error) && $response->code == 200) {
      $response_data = (array) drupal_json_decode($response->data);

      return $response_data + array(
        // Default value for the access_token.
        'access_token' => NULL,
        // Fake the ID token.
        'id_token' => NULL,
      );
    }
    else {
      openid_connect_log_request_error(__FUNCTION__, $this->name, $response);
      return FALSE;
    }
  }

}
