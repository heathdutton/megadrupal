<?php

/**
 * Override or insert variables into the html template.
 */
function aperture_preprocess_html(&$vars) {
  // Add css for colour schemes
  $colour = theme_get_setting('colour');
  $path = drupal_get_path('theme', 'aperture') . '/aperture-' . $colour . '.css';
  if (file_exists($path)) {
    drupal_add_css($path, array('group' => CSS_THEME));
  }

  // Add conditional CSS for IE6 and IE7.
  drupal_add_css(path_to_theme() . '/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
}

function aperture_preprocess_page(&$vars) {
  if (theme_get_setting('toggle_main_menu') && !empty($vars['main_menu'])) {
    $tree = menu_tree_all_data(variable_get('menu_main_links_source', 'main-menu'));
    $vars['primary'] = '<div id="primary-menu">' . drupal_render(menu_tree_output($tree)) . '</div>';
  }

  if (empty($vars['postamble'])) {
    // Linkback: Please do not remove this as a courtesy to the effort we have put into this theme. 
    global $base_path;
    $text = aperture_linkback_text();
    $title = 'Theme by ProsePoint Express: ' . $text;
    $vars['postamble'] = '<div style="font-size: 12px; line-height: 20px; text-align: right;"><a href="http://www.prosepoint.net" title="' . $title . '"><img src="' . $base_path . drupal_get_path('theme', 'aperture') . '/linkback.png" style="vertical-align: middle;" alt="ProsePoint Express: ' . $text . '" title="' . $title . '" /></a></div>';
  }
  if (empty($vars['page']['footer']['#suffix'])) {
    $vars['page']['footer']['#suffix'] = '';
  }
  $vars['page']['footer']['#suffix'] .= $vars['postamble'];

  if (empty($vars['logo_alt'])) {
    $vars['logo_alt'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
}

function aperture_linkback_text() {
  $options = array(
    'Online newspaper and magazine cms software', 
    'Content management system for newspapers and magazines', 
    'Newspaper cms', 
    'Magazine and newspaper software', 
    'News publishing content management system', 
    'Newpaper websites and web design', 
    'Websites and web design for magazines', 
    'Publishing software', 
    'News website publishing software', 
    'Software for hosted news websites', 
    'Software for publishing news', 
  );
  return $options[strlen(variable_get('site_name', 'Drupal')) % count($options)];
}

function aperture_preprocess_node(&$vars) {
  $vars['title_attributes_array']['class'] = 'node-title';

  if ($vars['teaser']) {
    $size_suffix = 'teaser';
  }
  else {
    $size_suffix = 'full';
    $vars['classes_array'][] = 'node-' . $size_suffix;
  }
  $vars['classes_array'][] = drupal_html_class('node-' . $vars['node']->type . '-' . $size_suffix);
  
  // Hide taxonomy field for teasers
  if ($vars['teaser']) {
    hide($vars['content']['field_tags']);
  }
}

function aperture_preprocess_block(&$vars) {
  $vars['title_attributes_array']['class'] = 'block-title';
}

function aperture_textfield(&$vars) {
  // Reduce width of textfields because we have a small space
  if ($vars['element']['#size'] > 32) {
    $vars['element']['#size'] = 32;
  }
  return theme_textfield($vars);
}
