<?php

/**
 * List Flickr albums
 */
function flickrgallery_wrapper_albums() {
  $build = array();

  // Create Flickr object.
  $f = flickrapi_phpFlickr();

  // Check for private pictures
  $token = variable_get('flickrgallery_token', NULL);
  $private = variable_get('flickrgallery_private_pictures', 0);

  if (!empty($token) && $private == 1) {
    $f->setToken($token);
  }

  // Get Flickr User info and User ID.
  $flickr_user = $f->people_getInfo(variable_get('flickrgallery_userID', NULL));
  $flickr_uid = $flickr_user['id'];

  // Get Flickr sets.
  if (variable_get('flickrgallery_displaysets_bool') == 1) {
    // If true, then select the flickrgallery_displaysets_values
    $array_set = explode("\n", trim(variable_get('flickrgallery_displaysets_values')));
    foreach ($array_set as $key => $set) {
      $sets['photoset'][$key] = $f->photosets_getInfo($set);
    }
  }
  else {
    $sets = $f->photosets_getList($flickr_uid);
  }

  if (!empty($sets)) {
    $albums = array();
    $flickr_path = variable_get('flickrgallery_path', 'flickr');

    foreach ($sets['photoset'] as $set) {
      if (variable_get('flickrgallery_display_type') == 1 && module_exists('image') && module_exists('imagecache_external')) {
        $img = array(
          '#theme' => 'imagecache_external',
          '#style_name' => variable_get('flickrgallery_albums_imagestyle', 'thumbnail'),
          '#path' => "https://farm" . $set['farm'] . ".static.flickr.com/" . $set['server'] . "/" . $set['primary'] . "_" . $set['secret'] . "_b.jpg",
        );
      }
      else {
        $img = array(
          '#theme' => 'image',
          '#path' => "https://farm" . $set['farm'] . ".staticflickr.com/" . $set['server'] . "/" . $set['primary'] . "_" . $set['secret'] . "_" . variable_get('flickrgallery_albums', 's') . ".jpg",
        );
      }

      // Add default attributes
      $img += array(
        '#title' => $set['title']['_content'],
        '#alt' => $set['title']['_content'],
        '#attributes' => array('class' => 'flickrgallery-set-image'),
      );

      $image_link = array(
        '#theme' => 'link',
        '#path' => $flickr_path . "/set/" . $set['id'],
        '#text' => drupal_render($img),
        '#options' => array(
          'html' => TRUE,
          'attributes' => array(
            'class' => array('flickrgallery'),
            'title' => $set['title']['_content'],
          ),
        ),
      );
      $title_link = array(
        '#theme' => 'link',
        '#path' => $flickr_path . "/set/" . $set['id'],
        '#text' => $set['title']['_content'],
        '#options' => array(
          'html' => TRUE,
          'attributes' => array(
            'class' => array('flickrgallery-title'),
            'title' => $set['title']['_content'],
          ),
        ),
      );

      $albums[] = array(
        'info' => $set,
        'total' => $set['photos'],
        'image_link' => drupal_render($image_link),
        'title_link' => drupal_render($title_link),
      );
    }

    $build = array(
      '#theme' => 'flickrgallery_albums',
      '#description' => t(variable_get('flickrgallery_description', NULL)),
      '#albums' => $albums,
    );
  }
  else {
    drupal_set_message(t('No picture albums available'), 'error');
  }

  // Return the output.
  return drupal_render($build);
}

function flickrgallery_set($set_id) {
  $build = array();

  // Create Flickr object.
  $f = flickrapi_phpFlickr();

  // Check for private pictures
  $token = variable_get('flickrgallery_token', NULL);
  $private = variable_get('flickrgallery_private_pictures', 0);

  if (!empty($token) && $private == 1) {
    $f->setToken($token);
  }

  // Get Flickr set info.
  $set_info = $f->photosets_getInfo($set_id);

  // Get total photo count
  $count = $set_info['count_photos'];

  // Load # images per page
  $per_page = variable_get('flickrgallery_per_page', '500');

  // Get page number from query params
  $page = 0;
  $params = drupal_get_query_parameters();
  if (isset($params['page'])) {
    $page = $params['page'];
  }

  // Get Flickr photos for this set.
  $photos = $f->photosets_getPhotos($set_id, NULL, NULL, $per_page, ($page + 1));

  // Limit the number of pictures?
  if (variable_get('flickrgallery_display_itemsets_bool', 0) == 1) {
    $number_sets = variable_get('flickrgallery_display_itemsets_values');
    array_splice($photos['photoset']['photo'], ($number_sets - 1), -1);
  }

  // If there aren't any photo's, display message.
  if (empty($set_id) || empty($photos)) {
    drupal_set_message(t('This set doesn\'t exists or there aren\'t any pictures available for this set.'), 'error');
    return;
  }

  // Declare variables.
  $photoset = array();
  $image_meta = array();

  foreach ($photos['photoset']['photo'] as $photo) {
    if (variable_get('flickrgallery_display_type') == 1 && module_exists('image') && module_exists('imagecache_external')) {
      $img = array(
        '#theme' => 'imagecache_external',
        '#style_name' => variable_get('flickrgallery_thumb_imagestyle', 'large'),
        '#path' => $f->buildPhotoURL($photo, 'large'),
      );

      $url_external = imagecache_external_generate_path($original, variable_get('flickrgallery_large_imagestyle', 'large'));
      $url = image_style_url(variable_get('flickrgallery_large_imagestyle', 'large'), $url_external);
    }
    else {
      $img = array(
        '#theme' => 'image',
        '#path' => $f->buildPhotoURL($photo, variable_get('flickrgallery_thumb', 'square')),
      );

      $url = $f->buildPhotoURL($photo, variable_get('flickrgallery_large', 'large'));
    }

    // Add default attributes
    $img += array(
      '#title' => $photo['title'],
      '#alt' => $photo['title'],
      '#attributes' => array('class' => 'flickrgallery-set-image'),
    );

    // Get META data for this image, only if flickrgallery_override is set to TRUE in the admin screen
    // This will lead to slower performance
    if (variable_get('flickrgallery_override') == TRUE) {
      $image_meta = $f->photos_getInfo($photo['id']);
    }

    // Get the type for Lightbox.
    $type = variable_get('flickrgallery_lightbox_type', 'lighbox2');

    $image = array(
      '#theme' => 'link',
      '#path' => $url,
      '#text' => drupal_render($img),
      '#options' => array(
        'html' => TRUE,
        'attributes' => array(
          'class' => array('flickrgallery-image', $type),
          'rel' => $type . '[flickrgallery]',
          'title' => $photo['title'],
        ),
      ),
    );

    $photoset_photo = array(
      '#theme' => 'flickrgallery_photo',
      '#image' => array(
        'info' => $photo,
        'image' => drupal_render($image),
      ),
      '#image_meta' => $image_meta,
    );

    $photoset[] = drupal_render($photoset_photo);
  }

  // Build photoset
  $build['flickrgallery_photoset'] = array(
    '#theme' => 'flickrgallery_photoset',
    '#photoset' => $photoset,
    '#meta' => $set_info,
  );

  // Build pager
  $current_page = pager_default_initialize($count, $per_page);
  $build['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => $count,
  );

  // Return the output.
  return drupal_render($build);
}
