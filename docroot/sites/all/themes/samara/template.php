<?php

/**
 * Override or insert variables into the html template.
 */
function samara_preprocess_html(&$vars) {

  // Add font and layout styles.
  $css['font'] = 'html {font-size: ' . theme_get_setting('base_font_size') . ';}';
  $css['no-sidebars'] = '
  body.no-sidebars #navigation, body.no-sidebars #header, body.no-sidebars #main-columns {
    width: '     . theme_get_setting('layout_1_width')     . ';
    min-width: ' . theme_get_setting('layout_1_min_width') . ';
    max-width: ' . theme_get_setting('layout_1_max_width') . ';
  }';
  $css['one-sidebar'] = '
  body.one-sidebar #navigation, body.one-sidebar #header, body.one-sidebar #main-columns {
    width: '     . theme_get_setting('layout_2_width')     . ';
    min-width: ' . theme_get_setting('layout_2_min_width') . ';
    max-width: ' . theme_get_setting('layout_2_max_width') . ';
  }';
  $css['two-sidebars'] = '
  body.two-sidebars #navigation, body.two-sidebars #header, body.two-sidebars #main-columns {
    width: '     . theme_get_setting('layout_3_width')     . ';
    min-width: ' . theme_get_setting('layout_3_min_width') . ';
    max-width: ' . theme_get_setting('layout_3_max_width') . ';
  }';
  drupal_add_css(implode($css), array('type' => 'inline', 'group' => CSS_THEME));

  // Add color scheme CSS
  drupal_add_css(path_to_theme() . '/styles/sch-' . theme_get_setting('color_scheme') . '.css', array('group' => CSS_THEME));

  // Add conditional CSS for IE
  drupal_add_css(path_to_theme() . '/styles/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/styles/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/styles/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the page template.
 */
function samara_preprocess_page(&$vars) {

  $positions = array(
    'sidebar_first' => theme_get_setting('sidebar_first_weight'),
    'sidebar_second' => theme_get_setting('sidebar_second_weight'),
  );
  
  $vars['left_sidebars'] = $vars['right_sidebars'] = '';
  foreach ($positions as $sidebar => $position) {
    switch ($position) {
      case -2:
        $vars['left_sidebars'] = render($vars['page'][$sidebar]) . $vars['left_sidebars'];
        break;
      case -1:
        $vars['left_sidebars'] .= render($vars['page'][$sidebar]);
        break;
      case 1:
        $vars['right_sidebars'] = render($vars['page'][$sidebar]) . $vars['right_sidebars'];
        break;
      case 2:
        $vars['right_sidebars'] .= render($vars['page'][$sidebar]);
        break;
    }
  }

  // Add $primary_nav variable, unlike built-in $main_menu variable it supports drop-down menus
  if (!empty($vars['main_menu'])) {
    $menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    $vars['primary_nav'] = $menu_tree ? render($menu_tree) : FALSE;
  }

  if (!empty($vars['secondary_menu'])) {
    $vars['secondary_nav'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'secondary-menu'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['secondary_nav'] = FALSE;
  }
  
  // Add $copyright_information variable
  $vars['copyright_information'] = theme_get_setting('copyright_information');

}

/**
 * Override or insert variables into the region template.
 */
function samara_preprocess_region(&$vars) {
  // Remove default classes from sidebars regions
  if($vars['region'] == 'sidebar_first') {
    $vars['classes_array'] = array('clearfix');
  }
  if( $vars['region'] == 'sidebar_second') {
    $vars['classes_array'] = array('clearfix');
  }
}


/**
 * Override or insert variables into the block template.
 */
function samara_preprocess_block(&$vars) {
  // Remove default classes from some regions
  if (in_array($vars['block']->region, array('content', 'content_top', 'content_bottom', 'highlight'))) {
    $vars['classes_array'] = array();
  }
}

/**
 * Overrides theme_tablesort_indicator().
 */
function samara_tablesort_indicator($vars) {
  $attributes = array(
    'alt' => t('sort icon'),
  );
  if ($vars['style'] == 'asc') {
    $attributes['path']  = path_to_theme() . '/images/tablesort-ascending.png';
    $attributes['title'] = t('sort ascending');
  }
  else {
    $attributes['path']  = path_to_theme() . '/images/tablesort-descending.png';
    $attributes['title'] = t('sort descending');
  }
  return theme('image', $attributes);
}
