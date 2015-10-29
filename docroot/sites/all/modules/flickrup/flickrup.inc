<?php
/**
 * @file
 * This file contains the functions for work with Flickr's photos.
 */

/**
 * A list of possible photo sizes with description and label.
 *
 * @return array
 *   An array of photo sizes.
 */
function flickrup_photo_sizes() {
  $sizes = array(
    's' => array(
      'label' => 'Square',
      'description' => t('s: 75 px square'),
    ),
    't' => array(
      'label' => 'Thumbnail',
      'description' => t('t: 100 px on longest side'),
    ),
    'q' => array(
      'label' => 'Large Square',
      'description' => t('q: 150 px square'),
    ),
    'm' => array(
      'label' => 'Small',
      'description' => t('m: 240 px on longest side'),
    ),
    'n' => array(
      'label' => 'Small 320',
      'description' => t('n: 320 px on longest side'),
    ),
    '-' => array(
      'label' => 'Medium',
      'description' => t('-: 500 px on longest side'),
    ),
    'z' => array(
      'label' => 'Medium 640',
      'description' => t('z: 640 px on longest side'),
    ),
    'c' => array(
      'label' => 'Medium 800',
      'description' => t('c: 800 px on longest side'),
    ),
    'b' => array(
      'label' => 'Large',
      'description' => t('b: 1024 px on longest side'),
    ),
    'h' => array(
      'label' => 'Large 1600',
      'description' => t('h: 1600 px on longest side'),
    ),
    'k' => array(
      'label' => 'Large 2048',
      'description' => t('k: 2048 px on longest side'),
    ),
    'o' => array(
      'label' => 'Original',
      'description' => t('o: Original image'),
    ),
  );

  $library = libraries_detect('galleria');
  if ($library['installed']) {
    $sizes['g'] = array(
      'label' => 'Galleria',
      'description' => t('g: Galleria plugin'),
    );
  }

  return $sizes;
}

/**
 * A list of possible sizes for manage display Galleria plugin.
 *
 * @return array
 *   An array of photo sizes
 */
function flickrup_galleria_photo_sizes() {
  return array(
    'q' => t('150 px square'),
    'm' => t('240 px on longest side'),
    'n' => t('320 px on longest side'),
    '-' => t('500 px on longest side'),
    'z' => t('640 px on longest side'),
    'c' => t('800 px on longest side'),
    'b' => t('1024 px on longest side'),
    'h' => t('1600 px on longest side'),
  );
}

/**
 * A list of possible sizes for manage display lightbox plugin.
 *
 * @return array
 *   An array of photo sizes.
 */
function flickrup_lightbox_photo_sizes() {
  return array(
    'q' => t('150 px square'),
    'm' => t('240 px on longest side'),
    'n' => t('320 px on longest side'),
    '-' => t('500 px on longest side'),
    'z' => t('640 px on longest side'),
    'c' => t('800 px on longest side'),
    'b' => t('1024 px on longest side'),
    'h' => t('1600 px on longest side'),
    'k' => t('2048 px on longest side'),
    'o' => t('Original image'),
  );
}

/**
 * Returns the URL to $photo.
 *
 * @param string $photo_metadata
 *   Metadata of the photo.
 * @param string $size
 *   Photo size.
 *
 * @return string
 *   URL for $photo with the correct size and format.
 */
function flickrup_get_img_url($photo_metadata, $size) {
  $sizes = flickrup_photo_sizes();
  foreach ($photo_metadata['sizes'] as $image_size) {
    $img_static_url = $image_size['source'];
    if (isset($sizes[$size]) && $image_size['label'] == $sizes[$size]['label']) {
      break;
    }
    elseif (!isset($sizes[$size])) {
      break;
    }
  }

  return $img_static_url;
}

/**
 * Returns the photo attributes.
 *
 * @param string $photo_metadata
 *   Metadata of the photo.
 * @param string $size
 *   Photo size.
 *
 * @return array
 *   Height, width, title, description, alt of the photo.
 */
function flickrup_get_img_attributes($photo_metadata, $size) {
  $sizes = flickrup_photo_sizes();
  $title = check_plain($photo_metadata['title']);
  $description = check_plain($photo_metadata['description']);
  $height = 0;
  $width = 0;
  foreach ($photo_metadata['sizes'] as $image_size) {
    $height = $image_size['height'];
    $width = $image_size['width'];
    if (isset($sizes[$size]) && $image_size['label'] == $sizes[$size]['label']) {
      break;
    }
    elseif (!isset($sizes[$size])) {
      break;
    }
  }
  $attributes = array(
    'height' => $height,
    'width' => $width,
    'title' => $title,
    'description' => $description,
    'alt' => isset($photo_metadata['description']) ? $title . ' ' . $description : $title,
  );
  return $attributes;
}

/**
 * Returns the available sizes for a photo.
 *
 * @param string $photo_id
 *   ID of the photo to get the available sizes of.
 *
 * @return array
 *   Response from the flickrup method flickr.photos.getSizes.
 *   (https://www.flickr.com/services/api/flickr.photos.getSizes.html)
 */
function flickrup_photo_get_sizes($photo_id) {
  $flickr = flickrup_phpFlickr();
  $response = $flickr->photos_getSizes($photo_id);
  if ($response) {
    return $response;
  }
  else {
    flickrup_set_error($flickr->getErrorMsg(), $flickr->getErrorCode());
    return FALSE;
  }
}

/**
 * Returns the phpFlickr object.
 *
 * @return phpFlickr
 *   The phpFlickr object.
 */
function flickrup_phpFlickr($clear_cache = FALSE) {
  $api_key = variable_get('flickrup_api_key', '');
  $api_secret = variable_get('flickrup_api_secret', '');
  $auth_token = variable_get('flickrup_auth_token', '');

  if (empty($api_key) || empty($api_secret)) {
    flickrup_set_error(t('Flickr API Authorization keys not set. Please <a href="@configure-link">configure the FlickrUp module settings</a>', array(
      '@configure-link' => url('admin/config/media/flickrup'),
    )));
    return NULL;
  }

  $library = libraries_detect('phpflickr');
  if (!$library['installed']) {
    flickrup_set_error(t('You need download the <a href="@download-url">phpFlickr library</a> and extract the entire contents of the archive into the %path directory on your server.', array(
      '@download-url' => $library['download url'],
      '%path' => 'sites/all/libraries',
    )));

    return NULL;
  }
  else {
    $path_to_library = libraries_get_path('phpflickr');
    $php_flickr = $path_to_library . '/phpFlickr.php';
    include_once $php_flickr;

    $flickr = new phpFlickr($api_key, $api_secret);
    if ($auth_token) {
      $flickr->setToken($auth_token);
    }

    return $flickr;
  }

}

/**
 * Callback for checking authorization status.
 */
function _flickrup_is_authorized() {
  if (variable_get('flickrup_api_key', '') && variable_get('flickrup_api_secret', '') && variable_get('flickrup_auth_token', '')) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Displays an error message to flickrup admins and writes an error to log.
 *
 * @param string $error_message
 *   Message or error response to display.
 * @param string $error_code
 *   Error code.
 */
function flickrup_set_error($error_message, $error_code = NULL) {
  if (!empty($error_code)) {
    $error_message = t('FlickrUp: !error_message Error code: !error_code', array(
      '!error_message' => $error_message,
      '!error_code' => $error_code,
    ));
  }
  else {
    $error_message = t('FlickrUp: !error_message', array('!error_message' => $error_message));
  }
  if (!$error_message) {
    $error_message = t('FlickrUp: Occurred an unspecified error');
  }
  if (user_access('administer flickrup')) {
    drupal_set_message($error_message, 'error');
  }
  watchdog('flickrup', $error_message, array(), WATCHDOG_ERROR);
}
