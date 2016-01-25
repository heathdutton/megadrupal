<?php
/**
 * @file
 */

class ms_files_filename_handler extends views_handler_field {
  function render($values) {
    $p_fid = $values->ms_files_profiles_fid;
    $d_fid = $values->ms_files_downloads_fid;
    if ($p_fid) {
      $file = ms_files_load_file($p_fid);
    }
    if ($d_fid) {
      $file = ms_files_load_file($d_fid);
    }
    return basename($file->filename);
  }
}
