<?php

/**
 * Parses raw POST data into key => value array.
 * @param  [type] $post [description]
 * @return [type]       [description]
 */
function parse_raw_post($post = NULL) {
  $data = array();

  if (is_null($post)) {
    $post = file_get_contents('php://input');
  }
  foreach (explode('&', $post) as $pair) {
    $vals = explode('=', $pair);

    if (sizeof($vals) == 2) {
      $data[$vals[0]] = urldecode($vals[1]);
    }
  }

  return $data;
}