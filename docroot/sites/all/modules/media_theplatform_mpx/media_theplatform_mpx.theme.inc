<?php

/**
 * @file
 * Theme integration for the media_theplatform_mpx module.
 */

/**
 * Returns HTML for an mpx video player embedded in an iframe tag.
 *
 * @param array $variables
 *   An associative array containing:
 *   - file: The mpx video file object.
 *   - guid: The mpx video guid string.
 *   - account_id: The mpx account ID if multiple videos share the same guid.
 *   - attributes: (optional) An array of additional attributes to use.
 *
 * @return string
 *   The rendered HTML.
 */
function theme_media_theplatform_mpx_player_iframe(array $variables) {
  $url = media_theplatform_mpx_get_player_iframe_url($variables);
  if (!$url) {
    return '';
  }

  // Construct the iframe tag element.
  $element = array(
    '#theme' => 'html_tag',
    '#tag' => 'iframe',
    '#attributes' => drupal_array_merge_deep(
      array(
        'src' => array(
          'path' => $url,
          'options' => array(
            'query' => array('form' => 'html'),
            'external' => TRUE,
          ),
        ),
        'frameborder' => 0,
        'allowfullscreen' => '',
        'id' => drupal_html_id('pdk-player'),
      ),
      $variables['attributes']
    ),
    '#value' => t('Your browser does not support iframes.'),
  );

  // Allow other modules to alter the iframe tag elements and attributes.
  drupal_alter('media_theplatform_mpx_player_iframe', $element, $variables);

  // Build the final iframe src attribute.
  $element['#attributes']['src'] = url($element['#attributes']['src']['path'], $element['#attributes']['src']['options']);

  // Return the rendered element.
  return drupal_render($element);
}
