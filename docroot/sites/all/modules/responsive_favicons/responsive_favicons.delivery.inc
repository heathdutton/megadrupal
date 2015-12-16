<?php

/**
 * @file
 *
 * Responsible for sending icons for common paths automatically requested by
 * certain browsers (even though the HTML does not mention them).
 */

/**
 * Creates a file object for the requested icon path.
 *
 * @param string $icon
 *   the icon filename.
 * @return object
 *   a file object.
 */
function responsive_favicons_get_file($file_path) {
  $uri = 'public://' . variable_get('responsive_favicons_path', 'favicons') . '/' . $file_path;

  $file = new stdClass();
  $file->uri = $uri;
  $file->filemime = file_get_mimetype($uri);
  $file->filesize = @filesize($uri);

  return $file;
}

/**
 * Attempts to send the raw file back in the response.
 *
 * @param $file
 *   a file object respresenting the icon.
 * @see http://cgit.drupalcode.org/favicon/tree/src/DrupalFavicon.php#n145
 */
function responsive_favicons_deliver_file($file) {
  if (is_int($file)) {
    drupal_deliver_html_page($file);
    return;
  }
  elseif (!is_object($file) || !is_file($file->uri) || !is_readable($file->uri)) {
    drupal_deliver_html_page(MENU_NOT_FOUND);
    return;
  }

  $headers = array(
    'Content-Type' => mime_header_encode($file->filemime),
    'Content-Disposition' => 'inline',
    'Content-Length' => $file->filesize,
  );

  // Let other modules know the file is being downloaded.
  module_invoke_all('file_transfer', $file->uri, $headers);

  foreach ($headers as $name => $value) {
    drupal_add_http_header($name, $value);
  }

  $fd = fopen($file->uri, 'rb');
  if ($fd !== FALSE) {
    while (!feof($fd)) {
      print fread($fd, DRUPAL_KILOBYTE);
    }
    fclose($fd);
  }
  else {
    watchdog('responsive_favicons', 'Unable to open @uri for reading.', array('@uri' => $file->uri));
    drupal_deliver_html_page(MENU_NOT_FOUND);
    return;
  }

  drupal_page_footer();
}
