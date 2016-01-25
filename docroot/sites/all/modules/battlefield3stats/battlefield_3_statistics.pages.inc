<?php

/**
 * @file
 */

function battlefield_3_statistics_view_soldier($account) {
  $userdata = battlefield_3_statistics_userdata($account);
  $data = _get_bf3_stats(array(
    'players' => array($userdata['username']),
    'platform' => $userdata['platform']
  ));
  $output = '';
  if (isset($data['list']) && !empty($data['list'])) {
    foreach ($data['list'] as $player) {
      $output .= theme('battlefield_3_statistics_player', $player);
      $output .= theme('battlefield_3_statistics_player_extended', $player);
    }
  }
  return $output;
}
