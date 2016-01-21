<?php
/**
 * @file
 */

class ms_files_username_handler extends views_handler_field {
  function render($values) {
    $uid = $values->ms_files_downloads_uid;
    $account = user_load($uid);
    return $account->name;
  }
}
