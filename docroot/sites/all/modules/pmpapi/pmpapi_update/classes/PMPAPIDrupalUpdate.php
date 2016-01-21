<?php

/**
 * @file
 * Contains PMPAPIDrupalUpdates.
 */

/**
 * 
 */

class PMPAPIDrupalUpdate extends PMPAPIDrupal {

  /**
   * Initializes a PMPAPIDrupalUpdate object.
   */
  function __construct($topic_uri, $mode) {
    parent::__construct();
    $this->verify_token = hash('sha256', REQUEST_TIME);
    $this->topic_uri = $topic_uri;
    $this->mode = $mode;
  }

  /**
   * Sends an HTTP subscribe/unsubscribe request to the API.
   */
  function sendRequestToHub() {
    $token = $this->auth()->getToken()->access_token;
    // Hacky, but this will have to do until we have a more hypermedia-y
    // solution
    $base = variable_get('pmpapi_base_url');
    if ($base == 'https://api.pmp.io') {
      $uri = 'https://publish.pmp.io/notifications';
    }
    else {
      $uri = 'https://publish-sandbox.pmp.io/notifications';
    }

    $secret = sha1(sha1(variable_get('site_name') . REQUEST_TIME));
    variable_set('pmpapi_update_secret', $secret);
    $data = array(
      'hub.callback' => url('pmpapi_notifications', array('absolute' => TRUE)),
      'hub.mode' => $this->mode,
      'hub.topic' => $this->topic_uri ,
      'hub.verify' => 'sync',
      'hub.secret' => $secret,
      'hub.verify_token' => $this->verify_token,
    );
    $options['data'] = drupal_http_build_query($data);
    $options['method'] = 'POST';
    $options['headers'] = array(
      'Content-Type' => 'application/x-www-form-urlencoded',
      'Authorization' => 'Bearer ' . $token,
    );

    $this->response = drupal_http_request($uri, $options);
  }
}