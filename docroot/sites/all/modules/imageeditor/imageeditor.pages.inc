<?php
/**
 * @file
 * Different pages for Image Editor module.
 */

/**
 * AJAX callback to save the image.
 */
function imageeditor_save($codename) {
  $plugin = imageeditor_info('editor', $codename);
  // Prepare temporary directory.
  $directory = imageeditor_temporary_directory();
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
    watchdog('imageeditor', 'Directory %directory could not be created.', array('%directory' => $directory), WATCHDOG_WARNING);
    drupal_json_output('There was an error saving your image. Please contact site administrator.');
    drupal_exit();
  }

  // Process image per service.
  if ($function = ctools_plugin_get_function($plugin, 'save_callback')) {
    $function();
  }

  return '';
}

/**
 * AJAX callback to close the editor.
 */
function imageeditor_close($codename) {
  drupal_add_js(drupal_get_path('module', 'imageeditor') . '/js/imageeditor.close.js');
  return '';
}

/**
 * AJAX callback to upload the image to external service.
 */
function imageeditor_upload($codename) {
  $plugin = imageeditor_info('uploader', $codename);

  global $base_url;
  // TODO: need to fix this somehow... @see imageeditor_inline_url_to_uri().
  $filepath = '@' . drupal_realpath(str_replace($base_url . '/', '', rawurldecode($_POST['url'])));
  $post = $filepath;
  if ($post_callback = ctools_plugin_get_function($plugin, 'post_callback')) {
    $post = $post_callback($filepath);
  }

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible;)");
  curl_setopt($ch, CURLOPT_TIMEOUT, 120);
  curl_setopt($ch, CURLOPT_POST, true);
  curl_setopt($ch, CURLOPT_URL, $plugin['upload_url']);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post);

  $response = curl_exec($ch);
  curl_close($ch);

  $output = '';
  if ($response_callback = ctools_plugin_get_function($plugin, 'response_callback')) {
    $output = $response_callback($response);
  }

  drupal_json_output($output);
  drupal_exit();
}
