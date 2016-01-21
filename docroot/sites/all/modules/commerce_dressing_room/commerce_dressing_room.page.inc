<?php
/**
 * @file
 * Commerce dressing room front page.
 *
 * @ingroup commerce_dressing_room
 */

/**
 * Build front page with dressing room.
 *
 * @param object $node
 *   Node object.
 *
 * @return string
 *   Dressing room page.
 */
function commerce_dressing_room_page($node) {
  // Load CSS files.
  drupal_add_css(drupal_get_path('module', 'commerce_dressing_room') . '/css/commerce_dressing_room.css');

  // Load JS files here.
  drupal_add_js(drupal_get_path('module', 'commerce_dressing_room') . '/js/slideshow/bjqs-1.3.min.js', 'file');
  drupal_add_js(drupal_get_path('module', 'commerce_dressing_room') . '/js/cam.js', 'file');

  $images_without_background = commerce_dressing_room_get_image_without_background($node);

  return theme('commerce_dressing_room', array(
    'images_without_background' => $images_without_background,
    'description' => t('Reach buttons with your hands to use this virtual dressing room.'),
    'message' => t('Loading assets, please allow this website to use your webcam.'),
    'info1' => t('You are watching a video demo, follow the instructions below to enable your webcam.'),
    'info2' => t('Please view this page using !chrome_canary_url and enable the "Media Stream" flag in about:flags.', array(
      '!chrome_canary_url' => l(t('Google Chrome Canary'), 'http://tools.google.com/dlpage/chromesxs', array(
        'absolute' => TRUE,
        'external' => TRUE,
      )),
    )),
    'video_demo' => t("Your browser doesn't support the html 5 video tag."),
  ));
}
