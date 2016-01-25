<?php
/**
 * GoToWebinar OAuth method; Get Access Token.
 */

$method = array(
  'url' => url(GOTOWEBINAR_OAUTH_URL . 'access_token', array(
    'query' => array(
      'grant_type' => 'authorization_code',
      'code' => $_GET['code'],
      'client_id' => $settings['api_key'],
    ),
  )),
  'method' => 'get',
  'response_type' => 'json',
);
