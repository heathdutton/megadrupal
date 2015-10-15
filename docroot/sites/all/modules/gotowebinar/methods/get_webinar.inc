<?php
/**
 * GoToWebinar API method; Get Webinars.
 *
 * Retrieve information on a specific webinar. If the webinar recurs, a sequence
 * of future times will be provided. Recurring webinars are treated the same as
 * normal webinars - each session in the recurring webinar series has a
 * different webinarKey. If an organizer cancels a webinar, then a request to
 * get that webinar would return a "404 Not Found" error.
 */

$method = array(
  'url' => url(GOTOWEBINAR_API_URL . "{$settings['organizer_key']}/webinars/{$options['params']['webinar_key']}", array(
    'query' => array(
      'oauth_token' => $settings['access_token'],
    ),
  )),
  'method' => 'get',
  'response_type' => 'json',
);
