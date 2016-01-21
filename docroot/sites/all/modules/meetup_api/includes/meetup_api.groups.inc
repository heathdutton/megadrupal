<?php

/**
 * @file
 * Wrapper for the MeetupAPIGroups class
 */

function meetup_api_groups($request, $options = NULL) {
  require_once(MUAPILOC . '/MeetupAPIGroups.php');
  $mug = new MeetupAPIGroups(MUAPIKEY);
  return meetup_api_request($request, $mug, $options);
}

