<?php

/**
 * @file
 * Wrapper for the MeetupAPIRsvps class
 */

function meetup_api_rsvps($request, $options = NULL) {
  require_once(MUAPILOC . '/MeetupAPIRsvps.php');
  $mug = new MeetupAPIRsvps(MUAPIKEY);
  return meetup_api_request($request, $mug, $options);
}

