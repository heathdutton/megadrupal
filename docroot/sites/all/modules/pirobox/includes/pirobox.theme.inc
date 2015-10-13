<?php
/**
 * @file
 * Theme functions for the Pirobox module.
 */

/**
 * Returns HTML for an Pirobox image field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *     - item: An array of image data.
 *     - entity_type: The entity type. E.g. node, comment or taxonomy_term
 *     - entity: The full entity object.
 *     - field: The array contains field values.
 *     - display_settings: The array contains formatter setting values.
 *     - cover_class: The string contains the CSS class to use
 *                    the gallery covering.
 *     - random: Boolean value to use image random.
 *     - entity_style_extra: Boolean value to use different content image style.
 *     - image_style_extra: Boolean value to use different Pirobox image style.
 *
 * @ingroup themeable
 */
function theme_pirobox_formatter($variables) {
  $settings = $variables['display_settings'];
  $gallery_id = '';
  $unique = FALSE;

  switch ($variables['entity_type']) {
    case 'node':
      // Build the gallery id.
      $nid = !empty($variables['entity']->nid) ? $variables['entity']->nid : 'nid';

      switch ($settings['pirobox_gallery_grouping']) {
        case 'post':
          $gallery_id = 'gallery-' . $nid . $variables['cover_class'];
          break;
        case 'page':
          $gallery_id = 'gallery-all' . $variables['cover_class'];
          break;
        case 'field_post':
          $gallery_id = 'gallery-' . $nid . '-' . check_plain($variables['field']['field_name']) . $variables['cover_class'];
          break;
        case 'field_page':
          $gallery_id = 'gallery-' . check_plain($variables['field']['field_name']) . $variables['cover_class'];
          break;
        case 'post_user':
          $gallery_id = 'gallery-' . $nid . '-user-' . $variables['entity']->uid . $variables['cover_class'];
          break;
        case 'page_user':
          $gallery_id = 'gallery-user-' . $variables['entity']->uid . '-all' . $variables['cover_class'];
          break;
        case 'custom':
          $gallery_id = check_plain($settings['pirobox_gallery_custom']) . $variables['cover_class'];
          break;
        case 'none':
          $gallery_id = '';
          $unique = TRUE;
      }

      switch ($settings['pirobox_caption']) {
        case 'auto':
          // If the title is empty use alt or the entity title in that order.
          if (!empty($variables['item']['title'])) {
            $caption = $variables['item']['title'];
          }
          elseif (!empty($variables['item']['alt'])) {
            $caption = $variables['item']['alt'];
          }
          elseif (!empty($variables['entity']->title)) {
            $caption = $variables['entity']->title;
          }
          else {
            $caption = ($variables['item']['filename']);
          }
          break;

        case 'title':
          $caption = !empty($variables['item']['title']) ? $variables['item']['title'] : $variables['entity']->title;
          break;
        case 'alt':
          $caption = !empty($variables['item']['alt']) ? $variables['item']['alt'] : $variables['entity']->title;
          break;
        case 'entity_title':
          $caption = $variables['entity']->title;
          break;
        default:
          $caption = $variables['item']['filename'];
          break;
      }
      break;

    case 'comment':
      // Build the gallery id.
      $cid = !empty($variables['entity']->cid) ? $variables['entity']->cid : 'cid';

      switch ($settings['pirobox_gallery_grouping']) {
        case 'comment':
          $gallery_id = 'gallery-comment-' . $cid . $variables['cover_class'];
          break;
        case 'page':
          $gallery_id = 'gallery-all' . $variables['cover_class'];
          break;
        case 'custom':
          $gallery_id = check_plain($settings['pirobox_gallery_custom']) . $variables['cover_class'];
          break;
        case 'none':
          $gallery_id = '';
          $unique = TRUE;
      }

      switch ($settings['pirobox_caption']) {
        case 'auto':
          // If the title is empty use alt or the entity title in that order.
          if (!empty($variables['item']['title'])) {
            $caption = $variables['item']['title'];
          }
          elseif (!empty($variables['item']['alt'])) {
            $caption = $variables['item']['alt'];
          }
          elseif (!empty($variables['entity']->subject)) {
            $caption = $variables['entity']->subject;
          }
          else {
            $caption = $variables['item']['filename'];
          }
          break;

        case 'title':
          $caption = !empty($variables['item']['title']) ? $variables['item']['title'] : $variables['entity']->subject;
          break;
        case 'alt':
          $caption = !empty($variables['item']['alt']) ? $variables['item']['alt'] : $variables['entity']->subject;
          break;
        case 'entity_title':
          $caption = $variables['entity']->subject;
          break;
        default:
          $caption = $variables['item']['filename'];
      }
      break;

    case 'taxonomy_term':
      // Build the gallery id.
      $tid = !empty($variables['entity']->tid) ? $variables['entity']->tid : 'term';

      switch ($settings['pirobox_gallery_grouping']) {
        case 'term':
          $gallery_id = 'gallery-term-' . $tid . $variables['cover_class'];
          break;
        case 'page':
          $gallery_id = 'gallery-all' . $variables['cover_class'];
          break;
        case 'custom':
          $gallery_id = check_plain($settings['pirobox_gallery_custom']) . $variables['cover_class'];
          break;
        case 'none':
          $gallery_id = '';
          $unique = TRUE;
      }

      switch ($settings['pirobox_caption']) {
        case 'auto':
          // If the title is empty use alt or the entity title in that order.
          if (!empty($variables['item']['title'])) {
            $caption = $variables['item']['title'];
          }
          elseif (!empty($variables['item']['alt'])) {
            $caption = $variables['item']['alt'];
          }
          elseif (!empty($variables['entity']->name)) {
            $caption = $variables['entity']->name;
          }
          else {
            $caption = $variables['item']['filename'];
          }
          break;

        case 'title':
          $caption = !empty($variables['item']['title']) ? $variables['item']['title'] : $variables['entity']->name;
          break;
        case 'alt':
          $caption = !empty($variables['item']['alt']) ? $variables['item']['alt'] : $variables['entity']->name;
          break;
        case 'entity_title':
          $caption = $variables['entity']->name;
          break;
        default:
          $caption = $variables['item']['filename'];
      }
      break;
  }

  $item_uri = $variables['item']['uri'];

  // Allow other modules to alter the item URI.
  drupal_alter('pirobox_image_path', $item_uri);

  /**
   * Media module supports no image title and image alt. *
   *
   * @todo Check Media module new versions for added alt and title.
   */
  $image = array(
    'path' => $item_uri,
    'alt' => isset($variables['item']['alt']) ? $variables['item']['alt'] : $variables['item']['filename'], // *
    'title' => isset($variables['item']['title']) ? $variables['item']['title'] : $variables['item']['filename'], // *
    'longdesc' => FALSE,
    'style_name' => $settings['pirobox_entity_style'],
    'filename' => $variables['item']['filename']
  );

  if (isset($variables['item']['width']) && isset($variables['item']['height'])) {
    $image['width'] = $variables['item']['width'];
    $image['height'] = $variables['item']['height'];
  }

  // Shorten the caption for the example styles or when caption shortening is active.
  $trim_length = variable_get('pirobox_caption_trim_length', 75);
  if ((variable_get('pirobox_caption_trim', 0)) && (drupal_strlen($caption) > $trim_length)) {
    $caption = drupal_substr($caption, 0, $trim_length - 5) . '...';
  }

  $title = $caption;

  // Allow other modules to alter the caption text.
  drupal_alter('pirobox_formatter_caption', $caption, $variables);

  // We use the longdesc attribute in the a tag to build the caption
  // in the Pirobox.
  if ($caption != $title) {
    $image['longdesc'] = filter_xss_admin($caption);
  }

  if ($style_name = $settings['pirobox_image_style']) {
    $path = image_style_url($style_name, $image['path']);
  }
  else {
    $path = file_create_url($image['path']);
  }

  return theme('pirobox_imagefield', array('file' => (object) $variables['item'], 'image' => $image, 'path' => $path, 'title' => $title, 'gid' => $gallery_id, 'unique' => $unique));
}

/**
 * Returns HTML for an image using a specific Pirobox image style.
 *
 * @param $variables
 *   An associative array containing:
 *     - file: The full file object.
 *     - image: Image item as array.
 *     - path: The path of the image that should be displayed in the Pirobox.
 *     - title: The title text that will be used as a caption in the Pirobox.
 *     - gid: The string contains the gallery id for Pirobox image grouping.
 *     - unique: Bolean value to display
 *               single images (no Pirobox image grouping)
 *
 * @ingroup themeable
 */
function theme_pirobox_imagefield($variables) {
  $image = '';

  if ($variables['unique'] == FALSE) {
    $class = array('pirobox-' . $variables['gid']);
  }
  if ($variables['unique'] == TRUE) {
    $class = array('pirobox-' . rand());
  }

  $class[] = 'pirobox-item';

  // The last letters sbs stand for 'side by side'.
  if (variable_get('pirobox_images_sbs', 0) == 1) {
    $class[] = 'pirobox-item-sbs';
  }

  $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => $variables['title'],
      'longdesc' => isset($variables['image']['longdesc']) ? $variables['image']['longdesc'] : '',
      'class' => implode(' ', $class)
    )
  );

  if ($variables['image']['style_name'] == 'hide') {
    $class[] = 'js-hide';
  }
  elseif ($variables['image']['style_name'] == 'link') {
    return theme('pirobox_link', array('file' => $variables['file'], 'path' => $variables['path'], 'options' => $options));
  }
  elseif (!empty($variables['image']['style_name'])) {
    $image = theme('image_style', $variables['image']);
  }
  else {
    $image = theme('image', $variables['image']);
  }

  return l($image, $variables['path'], $options);
}

/**
 * Returns HTML if content image displayed as link.
 *
 * @param $variables
 *   An associative array containing:
 *   - file: A file object to which the link icon will be created.
 *   - path: The path of the image that should be displayed in the Pirobox.
 *   - options: An array containing link parameter such as class.
 *
 * @ingroup themeable
 */
function theme_pirobox_link($variables) {
  $icon = theme('file_icon', array('file' => $variables['file']));

  // Options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples

  return '<div class="content"><span class="file">' . $icon . ' ' . l($variables['file']->filename, $variables['path'], $variables['options']) . '</span></div>';
}
