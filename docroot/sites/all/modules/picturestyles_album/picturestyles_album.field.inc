<?php
/**
 * @file
 * Album picture style field formatter file
 *
 * Implements hook_field_formatter_info().
 * This allows the formatter to show up in the list of available
 * formatters and binds it to certain types of entities or fields.
 */

function picturestyles_album_field_formatter_info() {
  $formatters = array(
    'album' => array(
      'label' => t('Album Image'),
      'field types' => array('image'),
    ),
  );

  return $formatters;
}

/**
 * Implements hook_field_formatter_view().
 * This actually provides the code that applies the specific
 * field formatters to each image in the sequence. Right now,
 * images beyond the third in a sequence are ingnored.
 * @TODO: Check to make sure a field is set to allow
 * multiple images
 * @TODO: Allow expansion to lightbox to see full gallery.
 */
function picturestyles_album_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();
  $element['images'] = array();

  // Check if the formatter involves a link.
  if (isset($display['settings']['image_link']) && $display['settings']['image_link'] == 'content') {
    $uri = entity_uri($entity_type, $entity);
  }
  elseif (isset($display['settings']['image_link']) && $display['settings']['image_link'] == 'file') {
    $link_file = TRUE;
  }

  foreach ($items as $delta => $item) {
    if (isset($link_file)) {
      $uri = array(
        'path' => file_create_url($item['uri']),
        'options' => array(),
      );
    }

    switch ($delta) {
      case 0:
        $image_preset = 'picturestyles_album_large';
        break;
      case 1:
        $image_preset = 'picturestyles_album_small_top';
        break;
      case 2:
      default:
        $image_preset = 'picturestyles_album_small_btm';
        break;
    }

  //assemble the themeable elment and add it to the $elment array
    if ($delta < 3) {
      $element['#images'][$delta] = array(
        '#theme' => 'image_formatter',
        '#item' => $item,
        '#image_style' => $image_preset,
      );
      if (isset($uri)) {
        $element['#images'][$delta]['#path'] = $uri;
      }
    }
  }

  //set theme for element
  $element['#theme'] = 'album';

  return $element;
}