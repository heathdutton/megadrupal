<?php
/**
 * GoToWebinar API method; Get Upcoming Webinars.
 *
 * Returns webinars scheduled for the future.
 */

$method = array(
  'url' => url(GOTOWEBINAR_API_URL . "{$settings['organizer_key']}/upcomingWebinars", array(
    'query' => array(
      'oauth_token' => $settings['access_token'],
    ),
  )),
  'method' => 'get',
  'response_type' => 'json',
);
