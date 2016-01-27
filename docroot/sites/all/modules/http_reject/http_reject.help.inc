<?php
/**
 * Helper fucntion for the help hook.
 */
function http_reject_admin_help() {
  $output  = t('<p>The HTTP Reject module allows you to reject HTTP requests based on a request method white-list and user-agent blacklist, with a configurable HTTP error status code.');
  $output .= t('<p>For a list of HTTP status codes and their meanings, see <a href="!link">!link</a>.', array('!link' => 'http://tools.ietf.org/html/rfc2616#section-10.4'));
  return $output;
}
