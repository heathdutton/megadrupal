<?php
/**
 * @file cloudmenu.theme.inc
 */

/**
 * Theme function that returns the menu tree items as url parameter for cumulus.swf
 */
function theme_cloudmenu_weighted($variables) {
  $links = $variables['links'];
  $settings = $variables['settings'];

  $output = '<tags>';
  foreach ($links as $link) {
    $font_size = (12 - (intval($link['depth']) * $settings['flash_font_size_interval'])) + ($settings['flash_font_size'] - $settings['flash_font_size_interval']);
    $output .= l($link['title'], $link['href'], array('absolute' => TRUE, 'attributes' => array('style' => '"font-size: '. $font_size .'px;"'))) ." \n";
  }
  $output .= '</tags>';
  return urlencode($output);
}

/**
 * Theme function that simply returns the menu tree items as links.
 */
function theme_cloudmenu_weighted_alt($variables) {
  $output = '';
  foreach($variables['links'] as $link) {
    if (isset($link['options']['attributes'])) {
      $options = array('attributes' => $link['options']['attributes']);
    }
    else {
      $options = array();
    }
    $output .= l($link['title'], $link['href'], $options);
  }
  return $output;
}
