<?php
/**
 * @file
 * Photobox theme functions.
 */

/**
 * Returns HTML for a Photobox image field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: An array of image data.
 *   - image_style: An optional image style.
 *   - path: An array containing the link 'path' and link 'options'.
 *
 * @ingroup themeable
 */
function theme_photobox_image_formatter($variables) {
  $item = $variables['item'];
  $entity_type = $variables['entity_type'];
  $entity = $variables['entity'];
  $field = $variables['field'];
  $settings = $variables['display_settings'];

  $image = array(
    'path' => $item['uri'],
    'alt' => $item['alt'],
    'title' => $item['title'],
    'style_name' => $settings['photobox_content_image_style'],
  );

  if ($variables['delta'] == 0 && !empty($settings['photobox_content_image_style_first'])) {
    $image['style_name'] = $settings['photobox_content_image_style_first'];
  }

  if (isset($item['width']) && isset($item['height'])) {
    $image['width'] = $item['width'];
    $image['height'] = $item['height'];
  }

  $entity_title = entity_label($entity_type, $entity);

  switch ($settings['photobox_caption']) {
    case 'auto':
      // If the title is empty use alt or the entity title in that order.
      if (!empty($image['title'])) {
        $caption = $image['title'];
      }
      elseif (!empty($image['alt'])) {
        $caption = $image['alt'];
      }
      elseif (!empty($entity_title)) {
        $caption = $entity_title;
      }
      else {
        $caption = '';
      }
      break;
    case 'title':
      $caption = $image['title'];
      break;
    case 'alt':
      $caption = $image['alt'];
      break;
    case 'content_title':
      $caption = $entity_title;
      break;
    case 'custom':
      $caption = token_replace($settings['photobox_caption_custom'], array($entity_type => $entity, 'file' => (object) $item), array('clear' => TRUE));
      break;
    default:
      $caption = '';
  }
  $image['title'] = $caption;

  // Build the gallery id.
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $entity_id = !empty($id) ? $entity_type . '-' . $id : 'entity-id';
  switch ($settings['photobox_gallery']) {
    case 'post':
      $gallery_id = 'gallery-' . $entity_id;
      break;
    case 'page':
      $gallery_id = 'gallery-all';
      break;
    case 'field_post':
      $gallery_id = 'gallery-' . $entity_id . '-' . $field['field_name'];
      break;
    case 'field_page':
      $gallery_id = 'gallery-' . $field['field_name'];
      break;
    case 'custom':
      $gallery_id = $settings['photobox_gallery_custom'];
      break;
    default:
      $gallery_id = '';
  }

  if ($style_name = $settings['photobox_image_style']) {
    $path = image_style_url($style_name, $image['path']);
  }
  else {
    $path = file_create_url($image['path']);
  }

  return theme('photobox_imagefield', array('image' => $image, 'path' => $path, 'title' => $caption, 'gid' => $gallery_id));
}

/**
 * Returns HTML for an image using a specific Photobox image style.
 *
 * @param $variables
 *   An associative array containing:
 *   - image: image item as array.
 *   - path: The path of the image that should be displayed in the Photobox.
 *   - gid: Gallery id for Photobox image grouping.
 *
 * @ingroup themeable
 */
function theme_photobox_imagefield($variables) {
  $class = array('photobox');

  if ($variables['image']['style_name'] == 'hide') {
    $image = '';
    $class[] = 'js-hide';
  }
  elseif (!empty($variables['image']['style_name'])) {
    $image = theme('image_style', $variables['image']);
  }
  else {
    $image = theme('image', $variables['image']);
  }

  $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => $variables['title'],
      'class' => $class,
      'data-photobox-gallery' => $variables['gid'],
    )
  );

  return l($image, $variables['path'], $options);
}
