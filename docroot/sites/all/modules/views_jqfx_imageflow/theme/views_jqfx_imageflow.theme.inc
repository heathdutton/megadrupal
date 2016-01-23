<?php
/**
 *  @file
 *  Theme & preprocess functions for the Views jQFX: Imageflow module.
 */

/**
 *  We'll grab only the first image from each row.
 */
function template_preprocess_views_jqfx_imageflow(&$vars) {
  // Initialize our $images array.
  $vars['images'] = array();

  // Strip all images from the $rows created by the original view query.
  foreach ($vars['rows'] as $item) {
    if (preg_match('@(<\s*img\s+[^>]*>)@i', $item, $matches)) {
      $image = $matches[1];
      // We need to add a URL to 'longdesc', as required by the plugin.
      // If our image is in an anchor tag, use its URL.
      if (preg_match('@<\s*a\s+href\s*=\s*"\s*([^"]+)\s*"[^>]*>[^<]*' . preg_quote($image) . '[^<]*<\s*/a\s*>@i', $item, $urls)) {
        $url = $urls[1];
      }
      // Or look to see if we have any other links in our views items.
      elseif (preg_match('@href\s*=\s*"([^"]+)"@i', $item, $urls)) {
        $url = $urls[1];
      }
      // Otherwise link to the original image.
      elseif (preg_match('@src\s*=\s*"([^"]+)"@i', $image, $urls)) {
        $url = $urls[1];
      }
      else {$url = url('<front>');}

      // Add the URL to the image's longdesc tag.
      $image = preg_replace('@img\s+@i', 'img longdesc="' . $url . '" ', $image);

      // Remove the base url from the image path for the reflect script.
      $image = str_replace(array('http://' . $_SERVER['HTTP_HOST'], 'https://' . $_SERVER['HTTP_HOST']), '', $image);

      // Add the image to our image array to display.
      $vars['images'][] = $image;
    }
  }

  $options = $vars['options']['views_jqfx_imageflow'];

  // Pass our background and text colors to the template.
  if ($options['custom_styles']) {
    $vars['styles'] = 'style="background-color:' . $options['background_color'] . ';color:' . $options['text_color'] . ';"';
  }
  else $vars['styles'] = NULL;

  _views_jqfx_imageflow_add_js($options, 'views-jqfx-imageflow-images-' . $vars['id']);
}

function _views_jqfx_imageflow_add_js($options, $id) {

  // Find the path to our plugin.
  $path = 'sites/all/libraries';

  // Add the plugin JS and CSS.
  drupal_add_js($path . '/ImageFlow/imageflow.js');
  drupal_add_css($path . '/ImageFlow/imageflow.css');

  //Add the module JS and CSS
  $drupal_path = drupal_get_path('module', 'views_jqfx_imageflow');
  drupal_add_js($drupal_path . '/js/views_jqfx_imageflow.js');
  drupal_add_css($drupal_path . '/css/views_jqfx_imageflow.css');

  $settings = array(
    'ImageFlowID'       => $id,
    'animationSpeed'    => (int) $options['animation_speed'],
    'aspectRatio'       => (float) $options['aspect_ratio'],
    'buttons'           => (bool) $options['buttons'],
    'captions'          => (bool) $options['captions'],
    'circular'          => (bool) $options['circular'],
    'glideToStartID'    => (bool) $options['glide_to_start_id'],
    'imageCursor'       => $options['image_cursor'],
    'imageFocusM'       => (float) $options['image_focus_m'],
    'imageFocusMax'     => (int) $options['image_focus_max'],
    'imagePath'         => check_plain($options['image_path']),
    'imageScaling'      => (bool) $options['image_scaling'],
    'imagesHeight'      => (float) $options['images_height'],
    'imagesM'           => (float) $options['images_m'],
    'opacity'           => (bool) $options['opacity'],
    'percentLandscape'  => (int) $options['percent_landscape'],
    'percentOther'      => (int) $options['percent_other'],
    'preloadImages'     => (bool) $options['preload_images'],
    'reflections'       => (bool) $options['reflections'],
    'reflectionPNG'     => (bool) $options['reflection_png'],
    'reflectPath'       => check_plain($options['reflect_path']),
    'scrollbarP'        => (float) $options['scrollbar_p'],
    'slider'            => (bool) $options['slider'],
    'sliderCursor'      => $options['slider_cursor'],
    'sliderWidth'       => (int) $options['slider_width'],
    'slideshow'         => (bool) $options['slideshow'],
    'slideshowAutoplay' => (bool) $options['slideshow_autoplay'],
    'slideshowSpeed'    => (int) $options['slideshow_speed'],
    'startID'           => (int) $options['start_id'],
    'startAnimation'    => (bool) $options['start_animation'],
    'xStep'             => (int) $options['x_step'],
  );

  // These can break imageflow if passed empty.
  if ($options['opacity_array']) {
    $settings['opacityArray'] = check_plain($options['opacity_array']);
  }
  if ($options['reflection_get']) {
    $settings['reflectionGET'] = $options['reflection_get'];
  }
  if ($options['reflection_p']) {
    $settings['reflectionP'] = (float)$options['reflection_p'];
  }
  if ($options['on_click']) {
    $settings['onClick'] = $options['on_click'];
  }

  drupal_add_js(array('viewsJqfxImageFlow' => array($id => $settings)), 'setting');
}
