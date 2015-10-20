<?php
/**
 * Theming functions for restaurant opentable module.
 */

/**
 * Implements template_preprocess_restaurant_opentable_reservation_form().
 */
function template_preprocess_restaurant_opentable_reservation_form(&$variables) {
  $restaurant_id = $variables['restaurant_id'];
  $mode = $variables['mode'];

  // Create a script url from this restaurant id.
  $script_url = url('https://secure.opentable.com/frontdoor/default.aspx', array(
    'query' => array(
      'rid' => $restaurant_id,
      'restref' => $restaurant_id,
      'bgcolor' => 'F6F6F3',
      'titlecolor' => '0F0F0F',
      'subtitlecolor' => '0F0F0F',
      'btnbgimage' => 'https://secure.opentable.com/frontdoor/img/ot_btn_red.png',
      'otlink' => 'FFFFFF',
      'icon' => 'dark',
      'mode' => $mode,
      'hover' => 1,
    ),
    'absolute' => TRUE,
  ));
  $variables['script_url'] = $script_url;


  // Do not render widget when in preview.
  if (drupal_match_path(current_path(), "panels/ajax/ipe/*")) {
    $variables['script_url'] = '';
  }

  // Add custom css.
  drupal_add_css(drupal_get_path('module', 'restaurant_opentable') . '/css/restaurant_opentable.css');
}

/**
 * Implements template_preprocess_restaurant_opentable_awards().
 */
function template_preprocess_restaurant_opentable_awards(&$variables) {
  drupal_add_css(drupal_get_path('module', 'restaurant_opentable') . '/css/restaurant_opentable.css');
}

/**
 * Implements template_preprocess_restaurant_opentable_ratings().
 */
function template_preprocess_restaurant_opentable_ratings(&$variables) {
  $ratings = $variables['ratings'];

  foreach ($ratings as $index => $rating) {
    if (isset($rating['rating']['value']) && isset($rating['rating']['range'])) {
      $ratings[$index]['#markup'] = '';
      $value = (float) $rating['rating']['value'];
      $range = (float) $rating['rating']['range'];

      for ($i = $range; $i >= 1; $i--) {
        $icon_class = 'star';

        if ($value == 0) {
          $icon_class = 'star-o';
        }
        else if ($i < $value || $value < 1) {
          $icon_class = 'star-half-o';
        }

        $ratings[$index]['#markup'] .= '<i class="fa fa-' . $icon_class . '"></i>';
        $value--;
      }

    }
    else {
      $ratings[$index]['#markup'] = $rating['rating']['text'];
    }
  }

  $variables['ratings'] = $ratings;
}