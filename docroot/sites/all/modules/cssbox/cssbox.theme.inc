<?php

/**
 * @file
 * CssBox theme functions.
 */

/**
 * Returns HTML for an CssBox image field formatter.
 */
function theme_cssbox_image_formatter($variables) {
  // Default variables.
  $item = $variables['item'];
  $entity_type = $variables['entity_type'];
  $entity = $variables['entity'];
  $field = $variables['field'];
  $settings = $variables['display_settings'];
  $output_caption = variable_get('cssbox_show_caption', 'yes');
  $output_nextprev = variable_get('cssbox_show_nextprev', 'yes');

  // Image details.
  $image = array(
    'path' => $item['uri'],
    'alt' => $item['alt'],
    'title' => $item['title'],
    'style_name' => $settings['cssbox_content_style'],
  );
  if (isset($item['width']) && isset($item['height'])) {
    $image['width'] = $item['width'];
    $image['height'] = $item['height'];
  }

  // Set default caption.
  $entity_title = entity_label($entity_type, $entity);
  switch ($settings['cssbox_caption']) {
     case 'auto':
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
    case 'node_title':
      $caption = $entity_title;
      break;
    case 'custom':
      $caption = token_replace($settings['cssbox_caption_custom'], array($entity_type => $entity, 'file' => (object) $item), array('clear' => TRUE));
      break;
    default:
      $caption = '';
  }
  // Remove all tags in case someone added them to ALT tag.
  $caption = check_plain($caption);

  // Generate path to full image.
  if ($style_name = $settings['cssbox_popup_style']) {
    $path = image_style_url($style_name, $image['path']);
  }
  else {
    $path = file_create_url($image['path']);
  }

  // Unset some variables if they are set to hide.
  if ($output_nextprev == 'no') {
    $variables['next'] = NULL;
    $variables['prev'] = NULL;
  }
  if ($output_caption == 'no') {
    $caption = NULL;
  }

  return theme('cssbox_imagefield', array('image' => $image, 'path' => $path, 'caption' => $caption, 'output' => $settings['cssbox_output'], 'fid' => $item['fid'], 'next' => $variables['next'], 'prev' => $variables['prev']));
}

/**
 * Returns HTML for an image using a specific CssBox image style.
 */
function theme_cssbox_imagefield($variables) {
  // Theme image.
  if (!empty($variables['image']['style_name'])) {
    $image = theme('image_style', $variables['image']);
  }
  else {
    $image = theme('image', $variables['image']);
  }

  // Generate output for inclusion.
  $output = theme('cssbox_popup', array(
    'image' => $variables['path'],
    'caption' => $variables['caption'],
    'id' => 'image-' . $variables['fid'],
    'next' => $variables['next'],
    'prev' => $variables['prev'],
  ));
  // Add generated output using javascript at on the footer.
  if ($variables['output'] != 'html') {
    drupal_add_js('document.write(\'' . $output . '\');', array('type' => 'inline', 'scope' => 'footer'));
    $output = NULL;
  }

  $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => $variables['caption'],
    )
  );
  return '<a href="#image-' . $variables['fid'] . '"' . drupal_attributes($options['attributes']) . '>' . $image . '</a>' . $output;
}

/**
 * Returns HTML for popup.
 */
function theme_cssbox_popup($variables) {
  $target = variable_get('cssbox_target_id', 'main');
  $prev = variable_get('cssbox_text_previous', '« Prev');
  $next = variable_get('cssbox_text_next', 'Next »');
  $close = variable_get('cssbox_text_close', 'Close');

  $output = ''
  . '<div class="cssbox" id="' . $variables['id'] . '">'
    . '<img src="' . $variables['image'] . '" alt="' . $variables['caption'] . '">'
    . '<a href="#' . $target . '" class="cssbox-close">' . t($close) . '</a>'
    . (!empty($variables['prev']) ? '<a href="#image-' . $variables['prev']['fid'] . '" class="cssbox-prev">' . t($prev) . '</a>' : '')
    . (!empty($variables['next']) ? '<a href="#image-' . $variables['next']['fid'] . '" class="cssbox-next">' . t($next) . '</a>' : '')
    . (!empty($variables['caption']) ? '<div><p>' . $variables['caption'] . '</p></div>' : '')
  . '</div>';

  return $output;
}
