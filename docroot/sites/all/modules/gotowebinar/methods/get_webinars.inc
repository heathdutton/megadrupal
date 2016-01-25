<?php
/**
 * GoToWebinar API method; Get Webinars.
 *
 * Retrieve details on all of an organizer's scheduled webinars that are set for
 * a future date. If the organizer does not have any scheduled webinars, the
 * response will be empty.
 */

$method = array(
  'url' => url(GOTOWEBINAR_API_URL . "{$settings['organizer_key']}/webinars", array(
    'query' => array(
      'oauth_token' => $settings['access_token'],
    ),
  )),
  'method' => 'get',
  'response_type' => 'json',
);
