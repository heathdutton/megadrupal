<?php
/**
 * @file
 */

class ms_files_buynow_handler extends views_handler_field {
  function render($values) {
    global $user;
    $pfid = $values->pfid;
    $p_file = ms_files_load_file_plan($pfid);

    // Check if the user has the file already
    if (ms_files_user_has_file($user->uid, $p_file->pfid)) {
      $d_file = ms_files_load_file_download($p_file->pfid, $user->uid);
      $link = l(basename($p_file->name), 'file/download/' . $d_file->file_key);
      return theme('ms_files_download_link', array('link' => $link));
    }
    else {
      $button = ms_files_get_purchase_button($p_file->pfid, $p_file->name);
      return theme('ms_files_buynow_button', array('button' => $button));
    }
  }
}
