<?php

/**
 * @file
 * tapestry template.php
 */
 
function get_tapestry_style() {
  $style = theme_get_setting('tapestry_style');
    if (!$style) {
      $style = 'gerberdaisy';
    }
  if (theme_get_setting('tapestry_pickstyle')) {
    if (isset($_COOKIE["tapestrystyle"])) {
      $style = $_COOKIE["tapestrystyle"];
    }
  }
  return $style;
}

/**
 * Implements hook_breadcrump()
 */
function tapestry_breadcrumb($variables) {
  // Add the bullets to breadcrumb
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    $bullet = base_path() . path_to_theme() . '/images/' . get_tapestry_style() . '/bullet-breadcrumb.png';
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= '<div class="breadcrumb">' . implode(' <image src="'. $bullet . '" /> ', $breadcrumb) . '</div>';
    return $output;
  }
}

/**
 * Implements hook_menu_link()
 */
function tapestry_menu_link($variables) {
//used to get the names from menu links  to set it as classes so we can use the icons.css 
  $element = $variables['element'];  
  $sub_menu = '';  
  $name_id = 'item-'. strtolower(str_replace(' ', '_', strip_tags($element['#title'])));
  $element['#attributes']['class'][] = 'menu-' . $element['#original_link']['mlid'] . ' '.$name_id;  
  if ($element['#below']) { 
    $sub_menu = drupal_render($element['#below']); 
  }  
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);  
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Implements hook_preprocess_page()
 */
function tapestry_preprocess_page(&$variables) {
  if (theme_get_setting('tapestry_themelogo')) {
    $variables['logo'] = base_path() . path_to_theme() . '/images/' . get_tapestry_style() . '/logo.png';
  }
}

/**
 * Implements hook_preprocess_block()
 */
function tapestry_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'suckerfish') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
    $variables['title_attributes_array']['class'][] = 'title';
}

/**
 * Implements hook_preprocess_html()
 */
function tapestry_preprocess_html(&$variables) {  
  // here are the calculating for content and sidebars width

  global $sidebar_mode;
  global $page_width;
  global $outsidebar_loc;
  global $left_sidebar_width;
  global $right_sidebar_width;
  global $outside_sidebar_width;
  global $suckerfish_align;
  global $main_content_side_margins;
  global $main_content_width;
  global $inside_content_width;
  global $drop_shadow_width;
  global $round_container_width;
  global $ie6suckerfish;

  $sidebar_mode = theme_get_setting('tapestry_sidebarmode');
  if (!$sidebar_mode) {
    $sidebar_mode = 'center';
  }

  $outsidebar_loc = theme_get_setting('tapestry_outsidebar');
  if (!$outsidebar_loc) {
    $outsidebar_loc = 'left';
  }

  $page_width = theme_get_setting('tapestry_fixedwidth');
  if (!$page_width) {
    $page_width = 900;
  }

  $sidebar_count = 0;
  if (!empty($variables['page']['sidebar_first'])) {
    $sidebar_count++;
    $left_sidebar_width = theme_get_setting('tapestry_leftsidebarwidth');
    if (!$left_sidebar_width) {
      $left_sidebar_width = 200;
    }
  } else {
    $left_sidebar_width = 0;
  }

  if (!empty($variables['page']['sidebar_second'])) {
  $sidebar_count++;
  $right_sidebar_width = theme_get_setting('tapestry_rightsidebarwidth');
  if (!$right_sidebar_width) {
    $right_sidebar_width = 200;
  }
  } else {
    $right_sidebar_width = 0;
  }
  
  if (!empty($variables['page']['sidebar_outside'])) {
  $sidebar_count++;
  $outside_sidebar_width = theme_get_setting('tapestry_outsidebarwidth');
    if (!$outside_sidebar_width) {
  $outside_sidebar_width = 200;
  }
  } else {
    $outside_sidebar_width = 0;
  }

  $suckerfish_align = theme_get_setting('tapestry_suckerfishalign');
  if (!$suckerfish_align) {
    $suckerfish_align = 'right';
  }

  $main_content_side_margins = 10;
  $main_content_width = $page_width - $left_sidebar_width - $right_sidebar_width - $outside_sidebar_width - ($main_content_side_margins * 2);

  $inside_content_width = $page_width - $outside_sidebar_width;

  $drop_shadow_width = $page_width + 24;
  $round_container_width = $page_width - 38;


  if (!empty($variables['page']['suckerfish'])) { 
    if (theme_get_setting('tapestry_suckerfishmenus')) {
      $ie6suckerfish = true;
    }
  }

  // here we add the styles css becaus if thes setit up in info file then therare loadet to late because the  coller css will lodet here to early
  drupal_add_css(path_to_theme() . '/style.css');
  drupal_add_css(path_to_theme() . '/css/' . get_tapestry_style() . '.css');
  drupal_add_css(path_to_theme() . '/css/suckerfish.css');

  $useicons = theme_get_setting('tapestry_useicons'); 
  if ($useicons) {
    if (isset($_COOKIE["tapestryicons"])) {
      $useicons = (($_COOKIE["tapestryicons"]) == "1");
    }
  }

  if ($useicons) {
    drupal_add_css(path_to_theme() . '/css/icons.css');
  }

  if (theme_get_setting('tapestry_uselocalcontent')) {
    //  Add  custome conten file
    $local_content = theme_get_setting('tapestry_localcontentfile');
    if (file_exists($local_content)) {
      drupal_add_css($local_content);
    }
  }

  drupal_add_css
  (
    path_to_theme() . '/css/ie6.css', 
    array
    (
      'group' => CSS_THEME, 
      'browsers' => array
      (
      'IE' => 'lte IE 6', 
      '!IE' => FALSE
      ), 
    'preprocess' => FALSE
    )
  );

  if (theme_get_setting('tapestry_useicons')) {
    drupal_add_js(path_to_theme() . '/js/pickicons.js');
  }

  if (theme_get_setting('tapestry_pickstyle')) {
    drupal_add_js(path_to_theme() . '/js/pickstyle.js');
  }
}

/**
 * Implements hook_process_html()
 */
function tapestry_process_html(&$variables) {  
  // now we get the calculated variables and implment they to the page header
  $variables['styles'] .= '<script type="text/javascript">' /* Needed to avoid Flash of Unstyle Content in IE */ . '</script>';

  global $sidebar_mode;
  global $page_width;
  global $outsidebar_loc;
  global $left_sidebar_width;
  global $right_sidebar_width;
  global $outside_sidebar_width;
  global $suckerfish_align;
  global $main_content_side_margins;
  global $main_content_width;
  global $inside_content_width;
  global $drop_shadow_width;
  global $round_container_width;
  global $ie6suckerfish;

  // Font settings
  if (theme_get_setting('tapestry_fontfamily') != 'Custom') {
    $variables['styles'] .= 
    '<style type="text/css">
      body {
        font-family :' . theme_get_setting('tapestry_fontfamily') .';
      }
    </style>';
  } else if (theme_get_setting('tapestry_customfont')) {
      $variables['styles'] .=
      '<style type="text/css">
        body {
          font-family :' . theme_get_setting('tapestry_customfont') .';
        }
      </style>';
  }

  // we add the styles for the content width after calculating
  $variables['styles'] .= '<style type="text/css">
  #banner, #container, #headercontainer, #header-region-container, #footer-region-container, #suckerfish-container {
    width:' . $page_width . 'px;
  }

  #page-right, #round-right {
    width:' . $drop_shadow_width . 'px;
  }

  #sidebar-left {
    width:' . $left_sidebar_width . 'px;
  }

  #sidebar-right {
    width:' . $right_sidebar_width . 'px;
  }

  #sidebar-outside {
    width:' . $outside_sidebar_width . 'px;
  }

  #inside-content {
    width:' . $inside_content_width . 'px;
  }

  #mainContent {
    width:' . $main_content_width . 'px;
    padding-left:' . $main_content_side_margins . 'px;
    padding-right:' . $main_content_side_margins . 'px;
  }

  #round-container {
    width:' . $round_container_width . 'px;
  }';

  if ($sidebar_mode == 'left') {
    $variables['styles'] .=  '#sidebar-right {
      float: left !important;
    }';
  }
  else if ($sidebar_mode == 'right') {
    $variables['styles'] .=  '#sidebar-left {
      float: right !important;
    }';
  }

  if ($outsidebar_loc == 'right') { $variables['styles'] .=
    '#sidebar-outside {
      float: right !important; clear: right !important;
    }

    #inside-content {
      float: left !important;
    }';
  }

  $variables['styles'] .= '</style>';

  if ($suckerfish_align == 'left') {
    $variables['styles'] .= 
      '<style type="text/css">
      #suckerfishmenu {
      float: left !important; }
      </style>';
  }
  elseif ($suckerfish_align == 'center') {
  $variables['styles'] .=
    '<style type="text/css">
    #suckerfish-container {
      display: table-cell !important;
    }

    #suckerfishmenu {
      margin: 0 auto !important;
      display: table !important;
      float: none !important;
    }
    </style>
    <!--[if lte IE 7]>
      <style type="text/css">
      #suckerfish-container {
        display: block;
        text-align: center;
      }

      #suckerfishmenu {
        display: inline;
        zoom: 1;
        float: none;
      }
      </style>
    <![endif]-->';
  }

  if ($ie6suckerfish) {
    $variables['scripts'] .=
    '<!--[if lte IE 6]>
      <script type="text/javascript" src="' . path_to_theme() . '/js/suckerfish.js"></script>
    <![endif]-->';
  }
}
