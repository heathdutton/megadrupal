<?php
/**
 * @file
 * Theme file for Projekktor
 */

/**
 * Projekktor template preprocess function.
 */
function template_preprocess_projekktor_container(&$vars) {
  $settings = &$vars['settings'];

  // Each Projekktor instance gets a unique id.
  $projekktor_id = &drupal_static('projekktor_id', 0);
  $vars['id'] = ++$projekktor_id;
  $id = $vars['id'];

  // Prepare elements.
  $items = $vars['items'];
  $vars['items'] = array();
  $playlist_items = array();
  foreach ($items as $delta => $item) {
    if (!empty($item['uri'])) {
      $playlist_item[0]['src'] = file_create_url($item['uri']);
      $playlist_item[0]['type'] = $item['filemime'];
    }
    else {
      foreach ($item as $key => $value) {
        if (is_int($key)) {
          $playlist_item[$key]['src'] = file_create_url($item[$key]['uri']);
          $playlist_item[$key]['type'] = $item[$key]['filemime'];
        }
      }
    }

    // Sets filename as title if needed.
    if ((!isset($item['title'])) && (isset($item['filename']))) {
      $item['title'] = $item['filename'];
    }
    if (isset($item['image_uri'])) {
      if (!empty($settings['projekktor_image_style'])) {
        $item['poster'] = image_style_url($settings['projekktor_image_style'], $item['image_uri']);
      }
      else {
        $item['poster'] = file_create_url($item['image_uri']);
      }
    }

    // Add our title and poster to the playlist config array.
    if (array_key_exists('title', $item)) {
      $playlist_item['config']['title'] = $item['title'];
    }
    if (array_key_exists('poster', $item)) {
      $playlist_item['config']['poster'] = $item['poster'];
    }

    $playlist_items[$delta] = $playlist_item;

  }

  $playlist = $playlist_items;

  $vars['projekktor'] = array(
    '#prefix' => '<div class="projekktor-screen container-inner">',
    '#markup' => '<video id="projekktor-' . $id . '" class="projekktor clearfix" controls></video>',
    '#suffix' => '</div>',
  );

  // Convert $id to human friendly form for projekktor_add API.
  $player_id = 'projekktor-' . $id;

  // Convert $settings to human friendly form for projekktor_add API.
  $preset_name = $settings['projekktor_preset'];
  $theme_name = $settings['projekktor_theme'];

  // Attach Projekktor JavaScript.
  projekktor_add($player_id, $preset_name, $theme_name, $playlist);

  // Add jCarousel.
  if (!empty($settings['projekktor_jcarousel'])) {
    $carousel_items = array();
    foreach ($items as $delta => $item) {
      // Sets filename as title if needed.
      if ((!isset($item['title'])) && (isset($item['filename']))) {
        $item['title'] = $item['filename'];
      }
      $title = (!empty($item['title'])) ? $item['title'] : NULL;

      // @TODO: fix all this ugly markup in the array below.
      // This needs to be in a prerender function.
      $carousel_items[$delta] = array(
        'data' => "<div class='carousel-item-wrapper'><div class='carousel-image'>" . theme('image_style', array(
          'path' => $item['image_uri'],
          'alt' => $title,
          'style_name' => $settings['projekktor_thumbnail_style'],
          'title' => $title,
        )),
      );
      $carousel_items[$delta]['data'] .= "</div><div class='carousel-item-title'>" . $title . "</div>";
      $carousel_items[$delta]['data'] .= "<div id='image-background-holder-" . $delta . "' class='image-background-holder'></div></div>";
    }

    $vars['position'] = $settings['projekktor_jcarousel_position'];

    if ($vars['position'] == 'top' || $vars['position'] == 'left') {
      $vars['carousel_first'] = TRUE;
    }
    else $vars['carousel_second'] = TRUE;

    $vars['jcarousel'] = array(
      '#theme' => 'item_list',
      '#prefix' => '<div class="projekktor-jcarousel">',
      '#items' => $carousel_items,
      '#suffix' => '</div>',
      '#attributes' => array(
        'id' => 'projekktor-jcarousel-' . $id,
        'class' => array(
          'jcarousel-skin-' . $settings['projekktor_jcarousel_theme'],
        ),
      ),
    );

    // Attach jCarousel javascript.
    projekktor_jcarousel_add($id, $settings);
  }
}

/**
 * Views template preprocess function.
 *
 * @TODO: Integrate the carousel with views.
 */
function theme_views_view_projekktor($vars) {
  $items = array();

  $view = $vars['view'];
  $file_field_name = $vars['file_field_name'];
  $image_field_name = $vars['image_field_name'];
  foreach ($vars['rows'] as $row) {
    $lang = $row->_field_data[$view->base_field]['entity']->language;
    // Omit rows without file field.
    if (!isset($row->_field_data[$view->base_field]['entity']->{$file_field_name})) {
      continue;
    }
    $item = $row->_field_data[$view->base_field]['entity']->{$file_field_name}[$lang][0];
    // Adds a views image field to our item array.
    // First look to use the youtube thumb if it's a media youtube video.
    if ($item['filemime'] == 'video/youtube') {
      $wrapper = file_stream_wrapper_get_instance_by_uri($item['uri']);
      $item['image_uri'] = $wrapper->getLocalThumbnailPath();
    }
    // Otherwise look for another image in our views output.
    elseif (isset($row->_field_data[$view->base_field]['entity']->{$image_field_name})) {
      $item['image_uri'] = $row->_field_data[$view->base_field]['entity']->{$image_field_name}[$lang][0]['uri'];
    }
    $items[] = $item;
  }

  return theme('projekktor_container', array(
    'items' => $items,
    'settings' => $vars['options'],
  ));
}
