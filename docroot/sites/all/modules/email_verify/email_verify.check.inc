<?php
/**
 * @file
 * User email check menu callback file for email_verify module.
 */

/**
 * Menu callback; look though the whole user base for invalid emails.
 *
 * Can be very long when hosts timeout.
 */
function email_verify_checkall() {
  $header = array('User Id', 'Name', 'Email');
  $rows = array();

  $results = db_select('users', 'u')
    ->fields('u', array('uid', 'name', 'mail'))
    ->execute();
  foreach ($results as $row) {
    if (email_verify_check($row->mail)) {
      $link = l($row->name, 'user/' . $row->uid);
      $rows[] = array(
        $row->uid,
        $link,
        $row->mail
      );
    }
  }

  return theme('table', array('header' => $header, 'rows'=> $rows));
}
