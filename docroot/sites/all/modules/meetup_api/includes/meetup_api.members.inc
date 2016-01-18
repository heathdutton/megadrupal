<?php

/**
 * @file
 * Wrapper for the MeetupAPIMembers class
 */

function meetup_api_members($request, $options = NULL) {
  require_once(MUAPILOC . '/MeetupAPIMembers.php');
  $mug = new MeetupAPIMembers(MUAPIKEY);
  return meetup_api_request($request, $mug, $options);
}

