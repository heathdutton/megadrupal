<?php

/**
 * @file
 * Function to compute the auto-complete string used in the discussion title.
 */


/**
 * Autocomplete a forum topic discussion title.
 *
 * The function does not return if the auto-complete feature
 * worked. Otherwise, it returns NULL.
 *
 * NOTE :
 * The previous version included a $tid parameter. It was not
 * being used and it would not work properly if the user was
 * creating a new node (i.e. you cannot select a forum, then a
 * title, at least, not dynamically... and it was shown as being
 * like that before.)
 *
 * @param $string The user string so far
 */
function discussthis_autocomplete($string = '') {
  // Anything yet?
  if (!$string) {
    echo drupal_json_encode(array());
    exit();
  }

  // Current user has the right to do that?!
  if (!user_access('access content')) {
    drupal_access_denied();
    return;
  }

//   // Grab the first 10 forum nodes with titles like the string typed so far
//   $sql = "SELECT n.nid, n.title FROM {node} n"
//     . " WHERE n.type = 'forum' AND LOWER(n.title) LIKE LOWER";
//   if (variable_get('discussthis_autocomplete_contains', 0)) {
//     // a LIKE pattern that starts with % can be slow...
//     $sql .= "('%%%s%%')";
//   }
//   else {
//     $sql .= "('%s%%')";
//   }
//   // TODO Please convert this statement to the D7 database API syntax.
//   $result = db_query_range(db_rewrite_sql($sql), $string);

  
  $result = db_select('node', 'n')
  ->fields('n', array('nid', 'title'))
  ->condition('type', 'forum', '=')
  ->condition('title', db_like($string) . '%', 'LIKE')
  ->execute()
  ->fetchAssoc();
  
  $matches = array();
  $matches[$result['nid']] = check_plain($result['title']) . ' [nid:' . $result['nid'] . ']';

  echo drupal_json_encode($matches);
  exit();
}

// vim: ts=2 sw=2 et syntax=php
