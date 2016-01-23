<?php
/**
 * @file
 * Gathers all the theme functions
 */

/**
 * Theme the likes data
 */
function theme_drupagram_likes($variables) {
  $output = '';
  $count = $variables['count'];
  $items = theme('drupagram_likes_data', $variables);

  $output .= '<div class="drupagram-likes">';
  $output .= '  <div class="drupagram-likes-count">';
  $output .= '    <span class="drupagram-likes-count-label">' . t('Likes') . ': ' . '</span>';
  $output .= '    <span class="drupagram-likes-count-value">' . $count . '</span>';
  $output .= '  </div>';
  $output .= $items;
  $output .= '</div>';
  return $output;
}

/**
 * Theme the likes items only
 */
function theme_drupagram_likes_data($variables) {
  $output = '';
  $items = $variables['data'];
  if (is_array($items) && !empty($items)) {
    $output .= '<div class="drupagram-likes-data">';
    foreach ($items as $key => $item) {
      $output .= theme('drupagram_likes_data_item', $item);
    }
    $output .= '</div>';
  }
  return $output;
}

/**
 * Theme a specific like item.
 */
function theme_drupagram_likes_data_item($variables) {
  $output = '';

  if (isset($variables['username']) && !empty($variables['username'])) {
    $output .= '<div class="drupagram-likes-data-username">';
    $output .= '<span class="drupagram-likes-data-username-label label">';
    $output .= t('Username') . ': ';
    $output .= '</span>';
    $output .= '<span class="drupagram-likes-data-username-value value">';
    $output .= $variables['username'];
    $output .= '</span>';
    $output .= '</div>';
  }

  if (isset($variables['profile_picture']) && !empty($variables['profile_picture'])) {
    $output .= '<div class="drupagram-likes-data-profile-picture">';
    $output .= '<span class="drupagram-likes-data-profile-picture-label label">';
    $output .= t('Profile Picture') . ': ';
    $output .= '</span>';
    $output .= '<span class="drupagram-likes-data-profile-picture-value value">';
    $output .= theme('image', array('path' => $variables['profile_picture']));
    $output .= '</span>';
    $output .= '</div>';
  }

  //$variables['id'];

  if (isset($variables['full_name']) && !empty($variables['full_name'])) {
    $output .= '<div class="drupagram-likes-data-full-name">';
    $output .= '<span class="drupagram-likes-data-full-name-label label">';
    $output .= t('Full Name') . ': ';
    $output .= '</span>';
    $output .= '<span class="drupagram-likes-data-full-name-value value">';
    $output .= $variables['full_name'];
    $output .= '</span>';
    $output .= '</div>';
  }

  return $output;
}