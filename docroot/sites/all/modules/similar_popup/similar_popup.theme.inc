<?php

/**
 * @file
 * Preprocessors and helper functions to make theming easier.
 */

/**
 * Returns HTML for a sound notification.
 */
function theme_similar_popup_sound_notification($variables) {
  global $base_url;
  $output = '<audio id="similar-popup-sound-notification">';
  $output .= '<source src="' . $base_url . '/' . drupal_get_path('module', 'similar_popup') . '/sounds/notify.ogg' . '" type="audio/ogg">';
  $output .= '<source src="' . $base_url . '/' . drupal_get_path('module', 'similar_popup') . '/sounds/notify.mp3' . '" type="audio/mpeg">';
  $output .= '</audio>';
  return $output;
}
