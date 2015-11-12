<?php
/**
 * GoToWebinar API method; Registrant Fields.
 *
 * Retrieve GoToWebinar registration fields for a registrant.
 */

$method = array(
  'url' => url(GOTOWEBINAR_API_URL . "{$settings['organizer_key']}/webinars/{$options['params']['webinar_key']}/registrants/fields", array(
    'query' => array(
      'oauth_token' => $settings['access_token'],
    ),
  )),
  'method' => 'get',
  'response_type' => 'json',
);
