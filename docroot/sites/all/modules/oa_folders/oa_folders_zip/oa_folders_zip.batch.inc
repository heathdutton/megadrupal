<?php


/**
 * First batch function, setting up the zip file.
 */
function oa_folders_zip_batch_prepare($arguments, &$context) {
  $context['results'] = $arguments;
  $context['results']['temp_file'] = drupal_tempnam(file_directory_temp(), "oa_folder_");
}

/**
 * Second batch function, adding files to the zip file.
 */
function oa_folders_zip_batch_add_file($files, &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_file'] = 0;
    $context['sandbox']['max'] = count($files);
  }

  $zip = new ZipArchive;
  if ($zip->open($context['results']['temp_file'], ZipArchive::CREATE) === TRUE) {
    $uri = $files[$context['sandbox']['progress']]['uri'];
    $path = $files[$context['sandbox']['progress']]['path'];
    $zip->addFile($uri, $path);
    $zip->close();
  } else {
      // WTF!!!!
  }
  $context['sandbox']['progress']++;

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Finish callback sending the zip file to the browser.
 */
function oa_folders_zip_batch_finish($success, $results, $operations) {

  // send the zip to the browser now.
  $filename = $results['temp_file'];
  $headers = array(
    'Content-Type'              => 'application/octet-stream',
    'Content-Disposition'       => 'attachment; filename="' . $results['filename'] . '"',
    'Content-Length'            => filesize($filename),
    'Content-Transfer-Encoding' => 'binary',
    'Pragma'                    => 'no-cache',
    'Cache-Control'             => 'must-revalidate, post-check=0, pre-check=0',
    'Expires'                   => '0',
    'Accept-Ranges'             => 'bytes'
  );

  foreach ($headers as $name => $value) {
    drupal_add_http_header($name, $value);
  }

  if ($results['parent_nid']) {
    $url = 'node/'. $results['parent_nid'];
  }
  else {
    $url = $results['http_referer'];
  }
  // Redirect the user to the parent folder or where he came from.
  header( "refresh:1;url=" . $url );

  print readfile($filename);
  exit;

}


