<?php

/**
 * @file media_yandex/media_yandex.theme.inc
 *
 * Theme and preprocess functions for Media: Yandex.
 */

/**
 * Preprocess function for theme('media_yandex_video').
 */
function media_yandex_preprocess_media_yandex_video(&$variables) {
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();
  $variables['video_user'] = check_plain($parts['u']);
  $variables['video_id'] = check_plain($parts['v']);
  $variables['width'] = isset($variables['width']) ? $variables['width'] : variable_get('media_yandex_width', 450);
  $variables['height'] = isset($variables['height']) ? $variables['height'] : variable_get('media_yandex_height', 330);
}

/**
 * Return player code
 */
function theme_media_yandex_video($variables) {
  return '
    <div class="media-yandex-wrapper" style="width:' . $variables['width'] . 'px; height:' . $variables['height'] . 'px;">
      <object width="' . $variables['width'] . '" height="' . $variables['height'] . '">
        <param name="video" value="http://static.video.yandex.ru/lite/' . $variables['video_user'] . '/' . $variables['video_id'] . '/"/>
        <param name="allowFullScreen" value="true"/>
        <param name="scale" value="noscale"/>
        <embed
          src="http://static.video.yandex.ru/lite/' . $variables['video_user'] . '/' . $variables['video_id'] . '/"
          type="application/x-shockwave-flash"
          width="' . $variables['width'] . '"
          height="' . $variables['height'] . '"
          allowFullScreen="true"
          scale="noscale"
        > </embed>
      </object>
    </div>
  ';
}

