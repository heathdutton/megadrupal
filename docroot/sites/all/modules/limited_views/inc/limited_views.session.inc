<?php

/**
 * @file
 *  Contains Session tracking method
 * @todo
 *  This currently breaks user sessions, so not a good idea to use at the moment :/
 */

/**
 * Increments a user's view count by arbitrary figure
 *
 * @param int $nid
 *  Node ID of current node
 * @param int $add
 *  figure to add to the existing count
 * @return int $count
 *  Number of views after new addition
 */
function limited_views_add($nid, $add = 1) {
  static $progressive;
  if (is_null($progressive)) {
    $progressive = limited_views_get_view_count();
  }
  $progressive += $add;

  $session = unserialize(sess_read(LIMITED_VIEWS_COOKIE_NAME_COUNT));
  $session[$nid] = array(
    'timestamp' => time(),
    'count' => $add,
  );
  sess_write(LIMITED_VIEWS_COOKIE_NAME_COUNT, serialize($session));

  return $progressive;
}

/**
 * Returns the number of views a user has accrued on their cookie
 *
 * This should always be used to access the view count, as it sanitizes the user
 * provided data.
 *
 * To use this in a test, you will have to use === in your statement to force
 * type checking.
 *
 * @return int
 *  number of content views
 */
function limited_views_get_view_count() {
  $expiry_timestamp = time() - LIMITED_VIEWS_EXPIRY_TIME;
  $view_count = 0;
  $session = unserialize(sess_read(LIMITED_VIEWS_COOKIE_NAME_COUNT));
  $count_data = (is_array($session)) ? $session : array();
  foreach ($count_data as $nid => $data) {
    if ($data['timestamp'] > $expire_timestamp) {
      // only add the count if the expiry time is less than the expiry time
      $view_count += (int)$data['count'];
    }
  }

  return $view_count;
}

/**
 * @todo: come up with occurances where sessions wouldn't be valid
 */
function limited_views_check_method() {
  return TRUE;
}
