<?php

/**
 * @file
 * Theme callbacks for the Nivo Lightbox module.
 */

/**
 * Returns HTML for a Nivo Lightbox image field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: Associative array of image or file data, which may include "uri",
 *     "alt", "width", "height", "title" and "attributes".
 *   - id: A unique CSS ID that identifies the current image.
 *   - gallery: A boolean indicating whether the image is part of a gallery.
 *   - group: A unique CSS ID that identifies the current images's group.
 *   - hidpi: A boolean indicating whether the image has a HiDPI version.
 *   - hidpi_image_style: An optional image style for the HiDPI image.
 *   - target_image_style: An optional image style for the target image.
 *   - lightbox_image_style: An optional image style for the lightbox image.
 *   - lightbox_type: A string indicating the type of the lightbox.
 *
 * @ingroup themeable
 */
function theme_nivo_lightbox_image_formatter($variables) {
  $item = $variables['item'];
  $image = array(
    'path' => $item['uri'],
  );

  if (array_key_exists('alt', $item)) {
    $image['alt'] = $item['alt'];
  }

  if (isset($item['attributes'])) {
    $image['attributes'] = $item['attributes'];
  }

  if (isset($item['width']) && isset($item['height'])) {
    $image['width'] = $item['width'];
    $image['height'] = $item['height'];
  }

  // Do not output an empty 'title' attribute.
  if (isset($item['title']) && drupal_strlen($item['title']) > 0) {
    $image['title'] = $item['title'];
  }

  if ($variables['target_image_style']) {
    $image['style_name'] = $variables['target_image_style'];
    $target_image = theme('image_style', $image);
  }
  else {
    $target_image = theme('image', $image);
  }

  // The link path and link options are both optional, but for the options to be
  // processed, the link path must at least be an empty string.
  $options = array(
    'attributes' => array(
      'id' => $variables['id'],
    ),
  );
  // When displaying an image inside a link, the html option must be TRUE.
  $options['html'] = TRUE;

  // Automatically group images into galleries by field instance when more than
  // one image is available.
  if ($variables['gallery']) {
    if ($variables['group']) {
      $options['attributes']['data-lightbox-gallery'] = $variables['group'];
    }
  }

  if (module_exists('retina_images')) {
    if ($variables['hidpi']) {
      $options['attributes']['data-lightbox-hidpi'] = image_style_url($variables['hidpi_image_style'], $image['path']);
    }

    if ($variables['lightbox_type']) {
      $options['attributes']['data-lightbox-type'] = $variables['lightbox_type'];
    }
  }

  // Do not output an empty 'title' attribute.
  if (isset($item['title']) && drupal_strlen($item['title']) > 0) {
    $options['attributes']['title'] = $item['title'];
  }

  if ($variables['lightbox_image_style']) {
    $output = l($target_image, image_style_url($variables['lightbox_image_style'], $image['path']), $options);
  }
  else {
    $output = l($target_image, file_create_url($image['path']), $options);
  }

  return $output;
}

/**
 * Returns HTML for a Nivo Lightbox file field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: Associative array of image or file data, which may include "uri",
 *     "filename", "uid", "filemime", "status", "timestamp" and "type".
 *   - id: A unique CSS ID that identifies the current file.
 *   - gallery: A boolean indicating whether the file is part of a gallery.
 *   - group: A unique CSS ID that identifies the current file's group.
 *   - target_view_mode: An optional view mode for the target file.
 *   - html_lightbox: A boolean indicating whether the lightbox contains inline
 *     HTML.
 *   - lightbox_view_mode: An optional view mode for the lightbox file.
 *   - lightbox_type: A string indicating the type of the lightbox.
 *
 * @ingroup themeable
 */
function theme_nivo_lightbox_file_formatter($variables) {
  $item = $variables['item'];
  $file = file_load($item['fid']);

  $render = file_view_file($file, $variables['target_view_mode']);
  $target_file = drupal_render($render);

  // The link path and link options are both optional, but for the options to be
  // processed, the link path must at least be an empty string.
  $options = array(
    'attributes' => array(
      'id' => $variables['id'],
    ),
  );
  // When displaying a file inside a link, the html option must be TRUE.
  $options['html'] = TRUE;

  // Automatically group files into galleries by field instance when more than
  // one file is available.
  if ($variables['gallery']) {
    if ($variables['group']) {
      $options['attributes']['data-lightbox-gallery'] = $variables['group'];
    }
  }

  if ($variables['lightbox_type']) {
    $options['attributes']['data-lightbox-type'] = 'ajax';
  }

  if ($variables['html_lightbox']) {
    $render = file_view_file($file, $variables['lightbox_view_mode']);
    $lightbox_file = drupal_render($render);

    $options['fragment'] = $variables['id'];
    $options['external'] = TRUE;
    $output = l($target_file, NULL, $options);
    $output .= '<div id="' . $variables['id'] . '" style="display:none;">' . $lightbox_file . '</div>';
  }
  else {
    $output = l($target_file, file_create_url($file->uri), $options);
  }

  return $output;
}
