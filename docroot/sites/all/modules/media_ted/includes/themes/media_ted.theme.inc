<?php

/**
 * @file media_ted/includes/themes/media_ted.theme.inc
 *
 * Theme and preprocess functions for Media: Ted.
 */

/**
 * Preprocess function for theme('media_ted_video').
 */
function media_ted_preprocess_media_ted_video(&$variables) {
  // Build the URL for display.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();
  $variables['video_id'] = check_plain($parts['v']);

  foreach (array('width', 'height', 'fullscreen') as $option) {
    // Set the option, either from the options array, or from the default value.
    $variables[$option] = isset($variables[$option]) ? $variables[$option] : (isset($variables['options'][$option]) ? $variables['options'][$option] : media_ted_variable_get($option));
  }

  $variables['fullscreen'] = $variables['fullscreen'] ? 'true' : 'false';
  $variables['wrapper_id'] = 'media_ted_' . $variables['video_id'] . '_' . $variables['id'];

  if (is_numeric($variables['video_id'])) {
    $variables['flash_embed'] = media_ted_prepare_flash($variables);
  }

  $variables['url'] = 'http://embed.ted.com/'. $variables['video_id'];

  // Do something useful with the overridden attributes from the file
  // object. We ignore alt and style for now.
  // @see media_youtube_preprocess_media_youtube_video
  if (isset($variables['options']['attributes']['class'])) {
    if (is_array($variables['options']['attributes']['class'])) {
      $variables['classes_array'] = array_merge($variables['classes_array'], $variables['options']['attributes']['class']);
    }
    else {
      // Provide nominal support for Media 1.x.
      $variables['classes_array'][] = $variables['options']['attributes']['class'];
    }
  }
}

function theme_media_ted_field_formatter_styles($variables) {
  $element = $variables['element'];
  $style = $variables['style'];
  $variables['file'] = $element['#item'];
  $variables['uri'] = $variables['file']['uri'];
  $variables['style_name'] = $style['name'];
  return theme('media_ted_embed', $variables);
}

/**
 * Preview for Styles UI.
 */
function theme_media_ted_preview_style($variables) {
  $variables['uri'] = media_ted_variable_get('preview_uri');
  $variables['field_type'] = 'file';
  $variables['object'] = file_uri_to_object($variables['uri']);

  return theme('styles', $variables);
}

/**
 * NOTE: Deprecated with Styles version 2.
 */
function theme_media_ted_styles($variables) {
  $style = $variables['style'];
  $variables['file'] = $variables['object'];
  $variables['uri'] = $variables['object']->uri;
  $variables['style_name'] = $style['name'];
  return theme('media_ted_embed', $variables);
}

/**
 * @todo: get this working
 *
 * This code is for embedding videos in WYSIYWG areas, not currently working.
 * NOTE: Deprecated with Styles version 2.
 */
function theme_media_ted_embed($variables) {
  $preset_name = $variables['preset_name'];
  $preset = styles_containers_available_styles('file', 'media_ted', $preset_name);
  $output = '';
  if (!empty($preset)) {
    // Build the URL for display.
    $uri = $variables['uri'];
    $wrapper = file_stream_wrapper_get_instance_by_uri($uri);
    $parts = $wrapper->get_parameters();

    $fullscreen_value = $autoplay = 'false';
    $in_browser = $thumbnail = FALSE;

    foreach ($preset['effects'] as $effect) {
      switch ($effect['name']) {
        case 'autoplay':
          $autoplay = $effect['data']['autoplay'] ? 'true' : 'false';
          break;
        case 'resize':
          $width = $effect['data']['width'];
          $height = $effect['data']['height'];
          break;
        case 'fullscreen':
          $fullscreen_value = $effect['data']['fullscreen'] ? 'true' : 'false';
          break;
        case 'thumbnail':
          $thumbnail = $effect['data']['thumbnail'];
      }
    }
    if (isset($variables['object']->override)) {
      $override = $variables['object']->override;
      if (isset($override['width'])) {
        $width = $override['width'];
      }
      if (isset($override['height'])) {
        $height = $override['height'];
      }
      if (isset($override['wysiwyg'])) {
        $thumbnail = TRUE;
      }
      if (isset($override['browser']) && $override['browser']) {
        $in_browser = TRUE;
        $thumbnail = TRUE;
      }
    }
    $width = isset($width) ? $width : media_ted_variable_get('width');
    $height = isset($height) ? $height : media_ted_variable_get('height');
    $video_id = check_plain($parts['v']);
    if ($thumbnail) {
      // @todo Clean this up.
      $image_variables = array(
        'path' => $wrapper->getOriginalThumbnailPath(),
        'alt' => $variables['alt'],
        'title' => $variables['title'],
        'getsize' => FALSE,
      );
      if (isset($preset['image_style'])) {
        $image_variables['path'] = $wrapper->getLocalThumbnailPath();
        $image_variables['style_name'] = $preset['image_style'];
        $output = theme('image_style', $image_variables);
      }
      else {
        // We need to add this style attribute here so that it doesn't get lost
        // If you resize a video in a node, save it, edit it, but don't adjust
        // the sizing of the video while editing, the size will revert to the
        // default.  Adding the specific size here retains the original resizing
        $WYSIWYG = isset($variables['object']->override['style']) ? $variables['object']->override['style'] : '';
        $image_variables['attributes'] = array('width' => $width, 'height' => $height, 'style' => $WYSIWYG);
        $output = theme('image', $image_variables);
      }
      if ($in_browser) {
        // Add an overlay that says 'Ted' to media library browser thumbnails.
        $output .= '<span />';
      }
    }
    else {
      $output = theme('media_ted_video', array('uri' => $uri, 'width' => $width, 'height' => $height, 'autoplay' => ($autoplay == 'true' ? TRUE : NULL), 'fullscreen' => ($fullscreen_value == 'true' ? TRUE : NULL)));
    }
  }
  return $output;
}

/**
 * Helper function for old flash-based embeds
 */
function media_ted_prepare_flash($variables) {
  $url = url('http://www.ted.com/talks/embed/id/' . $variables['video_id']);

  if (($response = drupal_http_request($url)) && $response->code != 200) {
    return;
  }

  preg_match('@name="flashvars" value="(.*?)"@i', $response->data, $matches);
  $flashvars = $matches[1];
  $w    = round($variables['width'] * 0.975);
  $h    = $variables['height'] - 85;
  $flashvars = preg_replace('/sz=([0-9]*)x([0-9]*)/', "sz={$w}x{$h}", $flashvars);
  $flashvars = preg_replace('/vw=[0-9]{2,4}/', "vw=$w", $flashvars);
  $flashvars = preg_replace('/vh=[0-9]{2,4}/', "vh=$h", $flashvars);

  $output = <<<EOD
  <div class="ted_media_player"><object width="{$variables['width']}" height="{$variables['height']}">
  <param name="movie" value="http://video.ted.com/assets/player/swf/EmbedPlayer.swf"></param>
  <param name="allowFullScreen" value="{$variables['fullscreen']}" />
  <param name="allowScriptAccess" value="always"/>
  <param name="wmode" value="transparent"></param>
  <param name="bgColor" value="#ffffff"></param>
  <param name="flashvars" value="{$flashvars}" />
  <embed src="http://video.ted.com/assets/player/swf/EmbedPlayer.swf" pluginspace="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" wmode="transparent" bgColor="#ffffff" width="{$variables['width']}" height="{$variables['height']}" allowFullScreen="{$variables['fullscreen']}" allowScriptAccess="always" flashvars="{$flashvars}"></embed></object></div>
EOD;

  return $output;
}
