<?php
/**
 * @file
 *
 * @author Dennis Brücke (blackice2999) | TWD - team:Werbedesign UG
 * @see http://drupal.org/user/413429
 * @see http://team-wd.de
 */

function theme_bootstrap_api_icon($variables) {
  $type = '';

  if (is_bool($variables['type']) && $$variables['type'] === TRUE) {
    $type = 'icon-white';
  }

  return '<i class="'. check_plain($variables['icon']) .' '. check_plain($type) .'"></i> ' . $variables['content'];
}
