<?php

/**
 * @file
 * Wrapper for the MeetupAPIProfilesv2 class
 */

function meetup_api_profilesv2($request, $options = NULL) {
  require_once(MUAPILOC . '/MeetupAPIProfilesv2.php');
  $mug = new MeetupAPIProfilesv2(MUAPIKEY);
  return meetup_api_request($request, $mug, $options);
}

