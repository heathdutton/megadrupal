<?php

/**
 * @file
 * Hooks provided by the Private Dynamic Stream module.
 */

/**
 * Allows modules to append content to the file's content that is downloaded by a user.
 *
 * @param $uri
 *   String specifying the file URI to transfer.
 *
 * @return
 *   An array of strings to be appended to the file's transferred content
 */
function hook_private_dynamic_stream_append($uri) {
  global $base_url;
  global $user;

  $data = array();

  $mimetype = file_get_mimetype($uri);
  switch ($mimetype) {
    case 'image/jpeg':
      $data[] = t('Downloaded from @url by user @uid on @timestamp', array(
        '@url' => $base_url,
        '@uid' => $user->uid,
        '@timestamp' => REQUEST_TIME,
      ));
      break;
  }

  return $data;
}

/**
 * Allows modules to alter the appended data of other modules.
 *
 * @param $data
 *   An array of strings to be appended to the file's transferred content
 *
 * @see hook_private_dynamic_stream_append()
 */
function hook_private_dynamic_stream_append_alter(&$data, $uri) {
  // allow only the first one
  if (!empty($data)) {
    $data = array_slice($data, 0, 1);
  }
}

/**
 * Allows module to alter the effective URL used to serve the file.
 *
 * @param $uri
 *   The original uri requested by the browser.
 */
function hook_private_dynamic_stream_uri(&$uri) {
  // Use a user specific file.
  global $user;
  $uri .= $user->uid . '.pdf';
}

/**
 * Allows modules to define a callback for a file, which will be called instead
 * of the default fopen passthrough.
 *
 * @param $uri
 *   The URI under consideration.
 *
 * @return
 *   A string with the function name that will serve the file. The lightest
 *   weight module returning a callback will be used. If no module returns a
 *   callback the default mechanism will be used.
 */
function hook_private_dynamic_stream_transfer_callback($uri) {
  if (substr($uri, -3) == '.id') {
    return 'example_private_dynamic_stream_transfer_callback';
  }
}

/**
 * Example file transfer callback.
 *
 * @param $uri
 *   The URI to be served.
 */
function example_private_dynamic_stream_transfer_callback($uri) {
  $file = file_get_contents($uri);
  str_replace('UNIQUEID', uniqid());
  print $file;
}
