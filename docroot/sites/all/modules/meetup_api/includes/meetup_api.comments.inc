<?php

/**
 * @file
 * Wrapper for the MeetupAPIComments class
 */

function meetup_api_comments($request, $options = NULL) {
  require_once(MUAPILOC . '/MeetupAPIComments.php');
  $mug = new MeetupAPIComments(MUAPIKEY);
  return meetup_api_request($request, $mug, $options);
}

