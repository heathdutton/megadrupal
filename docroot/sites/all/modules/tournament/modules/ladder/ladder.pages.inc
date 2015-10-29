<?php

/**
 * Returns HTML for a ladder level bar
 *
 * @param $variables
 *   An associative array containing:
 *   - level: Current level.
 *   - exp: Current EXP.
 *   - data: Level data returned from _ladder_level_data().
 *
 */
function theme_ladder_level_bar($variables) {
  drupal_add_css(drupal_get_path('module', 'ladder') . '/css/ladder-level-bar.css');

  $title = t('Current EXP: @exp/@exp_tnl', array('@exp' => $variables['exp'], '@exp_tnl' => $variables['data']['next_level']));

  $output = '<div class="ladder-level-bar-outer" title="' . $title . '">';
  $output .= '<div class="ladder-level-bar-inner" style="width:' . $variables['data']['percent'] . '%">';
  $output .= '<div class="ladder-level-bar-text">' . $variables['level'] . '</div>';
  $output .= '</div></div>';

  return $output;
}
