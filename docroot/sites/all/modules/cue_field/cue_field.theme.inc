<?php

/**
 * @file
 * Theme functions used by the cue field module.
 */

/**
 * Theme function for cue_field_jplayer.
 */
function theme_cue_field_jplayer($variables) {
  $item = $variables['element'];
  $title = format_string('!title !start</span>', array(
    '!title' => $item['#title'],
    '!start' => render($item['#start']),
  ));
  $item['#uri']['options']['html'] = TRUE;
  $output = l($title, $item['#uri']['path'], $item['#uri']['options']);
  $output = '<div class"title">' . $output . '</div>';
  $links_output = array();
  foreach ($item['#links'] as $link) {
    $links_output[] = l($link['#text'], $link['#path'], $link['#options']);
  }

  if (count($links_output) > 0) {
    $output .= '<div class="actions">(' . implode(' | ', $links_output) . ')</div>';
  }

  return $output;
}
