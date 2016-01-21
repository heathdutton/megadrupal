<?php

/**
 * @file
 * Wrapper for the MeetupAPIEvents class
 */

function meetup_api_events($request, $options = NULL) {
  require_once(MUAPILOC . '/MeetupAPIEvents.php');
  $mug = new MeetupAPIEvents(MUAPIKEY);
  return meetup_api_request($request, $mug, $options);
}

