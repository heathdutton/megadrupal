<?php
/**
 * Hijacking theme_local_action().
 */
function jqueryui_vtcore_menu_local_action($variables) {
  global $theme_key;
  _jqueryui_selected_theme_init('load', $theme_key);
  _jqueryui_load_js(BASE_THEME, 'misc');
  _jqueryui_load_css(BASE_THEME, 'base');

  $link = $variables['element']['#link'];

  $attributes = array(
    'class' => array(
    	'ui-state-default',
    	'ui-corner-all'
  	),
  );

  $output = '<li ' . drupal_attributes($attributes) . '>';

  // Add our icon
  $output .= '<span class="ui-icon ui-icon-circle-plus"></span>';

  if (isset($link['href'])) {
    $output .= l($link['title'], $link['href'], isset($link['localized_options']) ? $link['localized_options'] : array());
  }
  elseif (!empty($link['localized_options']['html'])) {
    $output .= $link['title'];
  }
  else {
    $output .= check_plain($link['title']);
  }
  $output .= "</li>\n";

  return $output;
}