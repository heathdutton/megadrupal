<?php
/**
 * GoToWebinar API method; Create Registrant.
 *
 * Register an attendee for a scheduled webinar. The response contains the
 * registrantKey and Join URL for the registrant. An email will be sent to the
 * registrant unless the organizer turns off the confirmation email setting from
 * the GoToWebinar website. Note: If your webinar includes additional required
 * fields, those fields will need to be specified during the create request. In
 * order to get a list of all fields required during registration make a call to
 * Get Registrant Fields.
 */

$method = array(
  'url' => url(GOTOWEBINAR_API_URL . "{$settings['organizer_key']}/webinars/{$options['params']['webinar_key']}/registrants", array(
    'query' => array(
      'oauth_token' => $settings['access_token'],
    ),
  )),
  'method' => 'post',
  'response_type' => 'json',
);
