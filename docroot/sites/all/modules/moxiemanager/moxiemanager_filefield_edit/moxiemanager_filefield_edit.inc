<?php

/**
 * @file
 *
 * Include file for the AJAX callback responsible for updating edited files.
 * @todo: Allow files to be edited instead of only images.
 */

/**
 * AJAX page callback for menu item 'moxiemanager_filefield_edit_ajax'.
 * Catches a POST from JavaScript after editing a file using Moxiemanager
 * file editor.
 */
function moxiemanager_filefield_edit_ajax() {
  // Get the posted variables and strip their values from possible attacks.
  $post_keys = array(
    'url',
    'field_name',
    'entity_type',
    'bundle_name',
  );
  foreach ($post_keys as $key) {
    if (isset($_POST[$key])) {
      ${$key} = filter_xss($_POST[$key]);
    }
    else {
      // If a value was not posted, there's no use in proceeding any futher.
      return MENU_ACCESS_DENIED;
    }
  }

  // Get the current field and instance information.
  $field = field_info_field($field_name);
  $instance = field_info_instance($entity_type, $field_name, $bundle_name);

  // Get the preview image style.
  $preview_image_style = $instance['widget']['settings']['preview_image_style'];
  $image_style = $instance['display']['default']['settings']['image_style'];
  // Flush the preview image style.

  if (!empty($preview_image_style)) {
    image_style_flush(image_style_load($preview_image_style));
  }
  if (!empty($image_style)) {
    image_style_flush(image_style_load($image_style));
  }

  // Get the URI scheme (private:// or public://)
  $uri_scheme = $field['settings']['uri_scheme'];
  $file_stream_wrapper = file_stream_wrapper_get_instance_by_scheme($uri_scheme);
  $stream_wrapper_uri = $file_stream_wrapper->getUri();
  $directory_path = $file_stream_wrapper->getDirectoryPath();

  // Extract the relative filepath of the edited file (e.g.: news_images/holiday.png).
  $file_path = trim(@end(explode($directory_path, $url)), '/');

  // Assemble the stream URL (e.g.: public://news_images/holiday.png).
  $file_uri = $stream_wrapper_uri . $file_path;

  // Now try to find the file object of the edited file.
  if ($files = file_load_multiple(array(), array('uri' => $file_uri))) {
    $file = reset($files);
  }
  else {
    // Couldn't find the file.
    // Create a new file object and save it.
    $pathinfo = pathinfo($file_uri);
    $mimetype = mime_content_type($file_uri);
    $file = new stdClass();
    $file->uid = $GLOBALS['user']->uid;
    $file->status = FILE_STATUS_PERMANENT;
    $file->filename = $pathinfo['basename'];
    $file->uri = $file_uri;
    $file->filemime = $mimetype;
    file_save($file);

    // Record that a module is using this file.
    file_usage_add($file, 'filefield_filemanager_edit', 'file', $file->fid);
  }

  // Create a thumbnail image for the file.
  $file->mm_thumbnail_url = image_style_url($preview_image_style, $file->uri);
  // Add a query parameter with an unique value to make sure browsers won't load
  // cached images.
  $file->mm_thumbnail_url .= '&time=' . REQUEST_TIME;

  // G the widths and heights of the generated thumbnail image.
  $info = image_get_info($file->mm_thumbnail_url);
  $file->mm_thumbnail_width = $info['width'];
  $file->mm_thumbnail_height = $info['height'];

  // Output the file object.
  drupal_json_output($file);
}
