<?php
/**
 * @file
 * Filter module integration.
 */

/**
 * Implements hook_filter_info().
 */
function deepzoom_filter_info() {
  $filters = array();

  $filters['deepzoom'] = array(
    'title' => t('Deep Zoom'),
    'description' => t('Provides the ability to create interactively viewable high-resolution images.'),
    'process callback' => 'deepzoom_filter_deepzoom_process',
    'settings callback' => 'deepzoom_filter_deepzoom_settings',
    'default settings' => array(
      'deepzoom_image_style' => 'large',
    ),
    'tips callback' => 'deepzoom_filter_deepzoom_tips',
  );

  return $filters;
}

/**
 * Deep Zoom Filter tips callback.
 */
function deepzoom_filter_deepzoom_tips($filter, $format, $long) {
  return t("Images with the 'deepzoom' class are converted to Deep Zoom images: &lt;img class='deepzoom' /&gt;");
}

/**
 * Deep Zoom Filter settings callback.
 */
function deepzoom_filter_deepzoom_settings($form, &$form_state, $filter, $format, $defaults, $filters) {
  $filter->settings += $defaults;

  $element = array();

  $element['deepzoom_image_style'] = array(
    '#title' => t('Fallback image style'),
    '#type' => 'select',
    '#default_value' => $filter->settings['deepzoom_image_style'],
    '#options' => array(),
    '#required' => TRUE,
  );
  foreach (array_keys(image_styles()) as $image_style) {
    $element['deepzoom_image_style']['#options'][$image_style] = $image_style;
  }

  return $element;
}

/**
 * Deep Zoom Filter process callback.
 */
function deepzoom_filter_deepzoom_process($text, $filter, $format, $langcode, $cache, $cache_id) {
  if (preg_match_all('/<img(?=[^>]*src=[\'"](.*?)[\'"])(?=[^>]*class=[\'"].*?(?<=[\s\'"])deepzoom(?=[\s\'"]+).*?[\'"]).*?\/>/', $text, $matches, PREG_SET_ORDER)) {
    global $base_url;
    foreach ($matches as $match) {
      $match[1] = str_replace(file_create_url('public://'), 'public://', $match[1]);
      $match[1] = str_replace(str_replace($base_url, '', file_create_url('public://')), 'public://', $match[1]);
      if (file_valid_uri($match[1]) && file_exists($match[1])) {
        $text = str_replace($match[0], render(theme('deepzoom', array('images' => $match[1]))), $text);
      }
    }
  }
  return $text;
}
