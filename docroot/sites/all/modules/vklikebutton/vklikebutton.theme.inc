<?php

/**
 * @file
 * Theme function for vklikebutton.
 */

/**
 * Returns field content to display.
 */
function theme_vklikebutton($conf) {
  $width = $conf['width'];
  $height = $conf['height'];
  $type = $conf['type'];
  $verb = $conf['verb'];
  $appid = $conf['appid'];
  $title = $conf['title'];
  $description = $conf['description'];
  $text = $conf['text'];
  $url = $conf['url'];
  $image = $conf['image'];
  $page_id = $conf['page_id'];

  $element_id = 'vk_like_' . $page_id;

  $options = array();
  if ($width) {
    $options[] = 'width: "' . $width . '"';
  }
  if ($height) {
    $options[] = 'height: "' . $height . '"';
  }
  if ($type) {
    $options[] = 'type: "' . $type . '"';
  }
  if (!is_null($verb)) {
    $options[] = 'verb: "' . $verb . '"';
  }
  if ($title) {
    $options[] = 'pageTitle: "' . $title . '"';
  }
  if ($description) {
    $options[] = 'pageDescription: "' . $description . '"';
    $options[] = 'description: "' . $description . '"';
  }
  if ($url) {
    $options[] = 'pageUrl: "' . $url . '"';
  }
  $options[] = 'pageImage: "' . $image . '"';
  $options[] = 'image: "' . $image . '"';

  if ($text) {
    $options[] = 'text: "' . $text . '"';
  }

  $output = '<div id="' . $element_id . '" class="vk_like_button"></div>';
  drupal_add_js('http://vk.com/js/api/openapi.js', array(
    'type' => 'external',
    'weight' => '-20',
    'group' => JS_LIBRARY,
  ), 'vklikebutton_openapi');
  drupal_add_js('VK.init({apiId: ' . $appid . ', onlyWidgets: true});', array(
    'type' => 'inline',
    'scope' => 'header',
    'weight' => '0',
    'group' => JS_LIBRARY,
  ), 'vklikebutton_init');
  drupal_add_js('VK.Widgets.Like(' . json_encode($element_id) . ', {' . implode(",\n", $options) . '}, 16' . $page_id . ');', array(
    'type' => 'inline',
    'scope' => 'footer',
  ));
  return $output;
}
