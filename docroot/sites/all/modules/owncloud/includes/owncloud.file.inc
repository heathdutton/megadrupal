<?php

function owncloud_file($owncloud_id) {
  $path = 'owncloud://' . $owncloud_id;
  $file = owncloud_file_load_by_remote_id($path);
  if ($file) {
    $image = image_load($path);
    if ($image) {
      file_transfer($image->source, array(
        'Content-Type' => $image->info['mime_type'],
        'Content-Length' => $file->filesize
      ));
    }
  }
  print '';
  exit;
}
