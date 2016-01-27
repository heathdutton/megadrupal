<?php

/**
 * @file
 * Provides the theming functions for the ecard module.
 */

/**
 * Implements hook_theme().
 */
function ecard_theme() {
  $path = drupal_get_path('module', 'ecard') . '/theme';

  $templates['ecard_formatter_view'] = array(
    'render element' => 'ecard_field',
    'template' => 'ecard_field',
    'path' => $path,
  );

  $templates['ecard_image_formatter'] = array(
    'variables' => array(
      'item' => NULL,
      'image_style' => NULL,
      'hash' => NULL,
      'args' => NULL,
      'ecard_settings' => NULL,
    ),
  );

  $templates['ecard_image'] = array(
    'template' => 'ecard_image',
    'path' => $path,
  );

  $templates['ecard_letter'] = array(
    'template' => 'ecard_letter',
    'path' => $path,
    'variables' => array(
      'message' => '',
    ),
  );

  $templates['ecard_copy'] = array(
    'template' => 'ecard_copy',
    'path' => $path,
    'variables' => array(
      'message' => '',
    ),
  );

  $templates['ecard_notice'] = array(
    'template' => 'ecard_notice',
    'path' => $path,
    'variables' => array(
      'message' => '',
    ),
  );

  return $templates;
}

/**
 * Returns HTML for an ecard image field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: An array of image data.
 *   - image_style: An optional image style.
 *   - path: An array containing the link 'path' and link 'options'.
 *
 * @ingroup themeable
 */
function theme_ecard_image_formatter($variables) {

  // Let the original image formatter do it's work and give us image markup.
  $variables['image'] = theme('image_formatter', $variables);

  $ecards = ecard_read($variables['hash']);
  foreach ($ecards as $ecard) {
    $variables['ecard'][] = $ecard;
  }

  $variables['id'] = '';
  $variables['class'] = ' class="ecard-if"';
  $variables['ecard_css'] = '';
  $variables['text'] = '';

  foreach ($variables['ecard_settings'] as $setting => $value) {
    if ($value) {
      switch ($setting) {
        case 'id':
          $variables['id'] = ' id="' . check_plain($value) . '"';
          break;
        case 'class':
          $variables['class'] = ' class="ecard-if ' . check_plain($value) . '"';
          break;
        case 'top':
          $variables['ecard_css'] .= ' top: ' . $value . 'px;';
          break;
        case 'left':
          $variables['ecard_css'] .= ' left: ' . $value . 'px;';
          break;
        case 'width':
          $variables['ecard_css'] .= ' width: ' . $value . 'px;';
          break;
        case 'color':
          $variables['ecard_css'] .= ' color: #' . $value . ';';
          break;
        case 'font':
          $variables['ecard_css'] .= ' font-family: ' . $value . ';';
          break;
        case 'font_size':
          $variables['ecard_css'] .= ' font-size: ' . $value . 'px;';
          break;
      }
    }
  }

  if (!empty($variables['ecard_css'])) {
    $variables['ecard_css'] = filter_xss_admin($variables['ecard_css']);
    $variables['ecard_css'] = 'style="' . $variables['ecard_css'] . '"';
  }

  if (!empty($variables['ecard'][0]->text)) {
    $variables['text'] = filter_xss_admin($variables['ecard'][0]->text);
  }

  // Add stylesheet for the formatter.
  drupal_add_css(drupal_get_path('module', 'ecard') . '/css/ecard-image-formatter.css');

  // Add our markup through our template.
  $output = theme('ecard_image', $variables);

  return $output;
}
