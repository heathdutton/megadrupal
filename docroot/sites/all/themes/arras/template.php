<?php

function arras_preprocess_html(&$vars) {
  // Add body classes
  $vars['classes_array'][] = theme_get_setting('logo') ? 'with-site-logo' : 'no-site-logo';
  $vars['classes_array'][] = (theme_get_setting('toggle_name') && filter_xss_admin(variable_get('site_name', 'Drupal'))) ? 'with-site-name' : 'no-site-name';
  $vars['classes_array'][] = (theme_get_setting('toggle_slogan') && filter_xss_admin(variable_get('site_slogan', ''))) ? 'with-site-slogan' : 'no-site-slogan';
  
  // Add css for colour schemes
  $colour = theme_get_setting('colour');
  $path = drupal_get_path('theme', 'arras') . '/arras-' . $colour . '.css';
  if (file_exists($path)) {
    drupal_add_css($path, array('group' => CSS_THEME));
  }

  // Add conditional CSS
  drupal_add_css(path_to_theme() . '/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

function arras_preprocess_page(&$vars) {
  // Suckerfish menus
  $vars['main_sf_menu'] = FALSE;
  if (theme_get_setting('toggle_main_menu') && !empty($vars['main_menu'])) {
    $tree = menu_tree_all_data(variable_get('menu_main_links_source', 'main-menu'));
    $data = menu_tree_output($tree);

    // Disable the 'ul' wrappers so we can add our own
    $data['#theme_wrappers'] = array();
    $vars['main_sf_menu'] = '<ul class="sf-menu clearfix">' . drupal_render($data) . '</ul>';
  }

  $vars['secondary_sf_menu'] = FALSE;
  if (theme_get_setting('toggle_secondary_menu') && !empty($vars['secondary_menu'])) {
    $tree = menu_tree_all_data(variable_get('menu_secondary_links_source', 'user-menu'));
    $data = menu_tree_output($tree);

    // Disable the 'ul' wrappers so we can add our own
    $data['#theme_wrappers'] = array();
    $vars['secondary_sf_menu'] = '<ul class="sf-menu clearfix">' . drupal_render($data) . '</ul>';
  }

  if (empty($vars['postamble'])) {
    // Linkback: Please do not remove this as a courtesy to the effort we have put into this theme. 
    global $base_path;
    $text = drupal_get_title();
    if (drupal_is_front_page() || (ord($text) % 2) == 0) {
      $title = 'Theme by ProsePoint Express';
      $site = 'http://www.prosepoint.net';
    }
    else {
      $title = 'Theme by ProsePoint';
      $site = 'http://www.prosepoint.org';
    }
    $text = arras_linkback_text();
    $vars['postamble'] = '<div style="font-size: 12px; line-height: 20px; text-align: right;"><a href="' . $site . '" title="' . $title . '"><img src="' . $base_path . drupal_get_path('theme', 'arras') . '/linkback.png" style="vertical-align: middle;" alt="' . $text . '" title="' . $title . '" /></a></div>';
  }
}

function arras_linkback_text() {
  $options = array(
    'Online newspaper and magazine cms software', 
    'Online newspaper software',
    'Magazine and newspaper software', 
    'Content management system for newspapers and magazines', 
    'News publishing content management system', 
    'Newspaper content management system', 
    'Magazine content management system',
    'CMS for newspaper', 
    'Newspaper cms', 
    'Magazine cms', 
    'News cms',
    'Publishing software', 
    'News website publishing software', 
    'Publish your own newspaper', 
    'Make your own magazine', 
    'Publishing online newspapers',
    'Software for hosted news websites', 
    'Software to publish news website', 
    'Software to publish a newspaper',
    'Newspaper websites and web design', 
    'Newspaper web design', 
    'Magazine web design', 
    'Newspaper template for websites',
  );
  $text = drupal_get_title();
  if (empty($text)) {
    $text = variable_get('site_name', 'Drupal');
  }
  return $options[(strlen($text) + 6) % count($options)];
}

function arras_preprocess_block(&$vars) {
  $vars['title_attributes_array']['class'] = 'block-title';
}

function arras_preprocess_node(&$vars) {
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
