<?php
/**
 * @file
 * Theme pre-process file for the zeebox Everywhere module.
 */

/**
 * Processes variables for zeebox-tv-room.tpl.php.
 */
function template_preprocess_zeebox_tv_room(&$variables) {
  // Must be in the footer/ after the hyperlink.
  drupal_add_js('http://static.zeebox.com/embed/1/zeebox.js', array(
    'scope' => 'footer',
    'external' => TRUE,
  ));
  $variables['region'] = variable_get('zeebox_region', 'us');
  $base_url = _zeebox_url_for_region($variables['region']);
  $variables['url'] = url($base_url . '/tv/rooms/' . $variables['settings']['tv_room_id'], array('external' => TRUE));
  $variables['message'] = t('Visit the Official Room on zeebox to join in the discussion!');
}

/**
 * Processes variables for zeebox-tv-room-teaser.tpl.php.
 */
function template_preprocess_zeebox_tv_room_teaser(&$variables) {
  // Must be in the footer/ after the hyperlink.
  drupal_add_js('http://static.zeebox.com/embed/1/zeebox.js', array(
    'scope' => 'footer',
    'external' => TRUE,
  ));
  $variables['region'] = variable_get('zeebox_region', 'us');
  $base_url = _zeebox_url_for_region($variables['region']);
  $variables['url'] = url($base_url . '/tv/rooms/' . $variables['settings']['tv_room_id'], array('external' => TRUE));
  $variables['message'] = t('Get zeebox and join in the discussion!');
  $variables['width'] = '300px';
}

/**
 * Processes variables for zeebox-hot-tv-rooms.tpl.php.
 */
function template_preprocess_zeebox_hot_tv_rooms(&$variables) {
  // Must be in the footer/ after the hyperlink.
  drupal_add_js('http://static.zeebox.com/embed/1/zeebox.js', array(
    'scope' => 'footer',
    'external' => TRUE,
  ));
  $variables['region'] = variable_get('zeebox_region', 'us');
  $base_url = _zeebox_url_for_region($variables['region']);
  // The API is <a href="http://zeebox.com/users/{zeebox-user-id}/rooms" data-zeebox-limit="{limit}"
  // data-zeebox-link="{link}" data-zeebox-type="hot-rooms">Check out our hottest zeebox TV Rooms</a>
  $variables['url'] = url($base_url . '/users/' . $variables['settings']['user_id'] . '/rooms', array('external' => TRUE));
  $variables['message'] = t('Check out our hottest zeebox TV Rooms!');
  $variables['width'] = '300px';
}

/**
 * Processes variables for zeebox-play-along.tpl.php.
 */
function template_preprocess_zeebox_play_along(&$variables) {
  // Must be in the footer/ after the hyperlink.
  drupal_add_js('http://static.zeebox.com/embed/1/zeebox.js', array(
    'scope' => 'footer',
    'external' => TRUE,
  ));
  $variables['region'] = variable_get('zeebox_region', 'us');
  $base_url = _zeebox_url_for_region($variables['region']);
  $variables['url'] = url($base_url . '/tv/brand/' . $variables['settings']['brand_id'], array('external' => TRUE));
  $variables['message'] = t('Have fun with zeebox!');
}

/**
 * Processes variables for zeebox-follow-button.tpl.php.
 */
function template_preprocess_zeebox_follow_button(&$variables) {
  $variables['region'] = variable_get('zeebox_region', 'us');
  // Currently the base_url isn't used as the widget is only available on the staging environment.
  $base_url = _zeebox_url_for_region($variables['region']);
  $variables['url'] = url('http://stage-develop.zeebox.com/buttons/follow-frame.html', array('query' => array('brand' => $variables['settings']['brand_id'], 'tvc' => $variables['region']), 'external' => TRUE));
  $variables['message'] = t('Have fun with zeebox!');
}
