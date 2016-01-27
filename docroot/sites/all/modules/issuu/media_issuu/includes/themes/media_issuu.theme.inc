<?php
/**
 * @file
 * media_issuu/includes/themes/media_issuu.theme.inc
 *
 * Theme and preprocess functions for Media: Issuu.
 */

/**
 * Preprocess function for theme('media_issuu_document').
 * @todo This function needs to be completed to add all the parameters used
 * in the media-issuu-document.tpl file.
 */
function media_issuu_preprocess_media_issuu_document(&$variables) {
    //add embed javascript
  drupal_add_js('//e.issuu.com/embed.js', 'external');

  $variables['width'] = isset($variables['width']) ? $variables['width'] : media_issuu_variable_get('width');
  $variables['height'] = isset($variables['height']) ? $variables['height'] : media_issuu_variable_get('height');
  $variables['fullscreen'] = isset($variables['fullscreen']) ? $variables['fullscreen'] : media_issuu_variable_get('fullscreen');
  $variables['fullscreen'] = $variables['fullscreen'] ? 'true' : 'false';
  $variables['url'] = "http://issuu.com/" . substr($variables['uri'], 8);
}
/*

function media_vimeo_preprocess_media_vimeo_video(&$variables) {
  // Build the URL for display.
  $uri = $variables['uri'];
  $wrapper = file_stream_wrapper_get_instance_by_uri($uri);
  $parts = $wrapper->get_parameters();
  $variables['video_id'] = check_plain($parts['v']);

  $variables['width'] = isset($variables['width']) ? $variables['width'] : media_vimeo_variable_get('width');
  $variables['height'] = isset($variables['height']) ? $variables['height'] : media_vimeo_variable_get('height');
  $variables['autoplay'] = isset($variables['autoplay']) ? $variables['autoplay'] : media_vimeo_variable_get('autoplay');
  $variables['fullscreen'] = isset($variables['fullscreen']) ? $variables['fullscreen'] : media_vimeo_variable_get('fullscreen');
  $variables['autoplay'] = $variables['autoplay'] ? 1 : 0;
  $variables['fullscreen'] = $variables['fullscreen'] ? 'true' : 'false';

  $variables['wrapper_id'] = 'media_vimeo_' . $variables['video_id'] . '_' . $variables['id'];

  // For users with JavaScript, these object and embed tags will be replaced
  // by an iframe, so that we can support users without Flash.
  $variables['output'] = <<<OUTPUT
    <object width="{$variables['width']}" height="{$variables['height']}">
      <param name="movie" value="http://vimeo.com/moogaloop.swf?clip_id={$variables['video_id']}">
      <param name="allowFullScreen" value="{$variables['fullscreen']}"></param>
      <param name="wmode" value="transparent" />
      <embed src="http://vimeo.com/moogaloop.swf?clip_id={$variables['video_id']}" type="application/x-shockwave-flash" width="{$variables['width']}" height="{$variables['height']}" allowfullscreen="{$variables['fullscreen']}"></embed>
    </object>
OUTPUT;

  // Pass the settings to replace the object tag with an iframe.
  $settings = array(
    'media_vimeo' => array(
      $variables['wrapper_id'] => array(
        'width' => $variables['width'],
        'height' => $variables['height'],
        'video_id' => $variables['video_id'],
        'fullscreen' => $variables['fullscreen'],
        'id' => $variables['wrapper_id'] .'_iframe',
      ),
    ),
  );
  if ($variables['autoplay']) {
    $settings['media_vimeo'][$variables['wrapper_id']]['options'] = array(
      'autoplay' => $variables['autoplay'],
    );
  }
  drupal_add_js($settings, 'setting');
  drupal_add_js(drupal_get_path('module', 'media_vimeo') . '/js/media_vimeo.js');
  drupal_add_css(drupal_get_path('module', 'media_vimeo') . '/css/media_vimeo.css');
  drupal_add_js(drupal_get_path('module', 'media_vimeo') . '/js/flash_detect_min.js');
}
 */