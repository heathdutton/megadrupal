<?php

/**
 * @file
 * Contains the Field Tracker User Name.
 */

/**
 * Views field handler for the user name
 *
 * @ingroup views
 */
class track_field_changes_handler_field_user_name extends views_handler_field {

  function render($values) {
    $uid = $values->track_field_changes_audit_uid;
    if ($uid) {
      $user = user_load($uid);
      return $user->name;
    }
    else {
      return '';
    }
  }

}
