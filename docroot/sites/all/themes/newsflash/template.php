<?php

/**
 * @file
 * NewsFlash template.php
 *
 */

/**
 * Gets NewsFlash defined style.
 * get setted color style and save it in a cookie
 */
function get_newsflash_style() {
  $style = theme_get_setting('newsflash_style');
  if (!$style) {
    $style = 'blue';
  }
  if (theme_get_setting('newsflash_pickstyle')) {
    if (isset($_COOKIE["newsflashstyle"])) {
      $style = $_COOKIE["newsflashstyle"];
    }
  }
  return $style;
}

/**
 * Implements hook_preprocess_html()
 *
 */
function newsflash_preprocess_html(&$variables) {
// set the color style themes
  $style = get_newsflash_style();
  drupal_add_css(path_to_theme() . '/style.css'); //is not settet in info file because it's loades to late
  drupal_add_css(path_to_theme() . '/css/' . $style . '.css');

  if (theme_get_setting('newsflash_suckerfish')) {
    drupal_add_css(path_to_theme() . '/css/suckerfish_' . $style . '.css');
  }
  else {
    drupal_add_css(path_to_theme() . '/css/nosuckerfish.css');
  }

  // set for custome css
  if (theme_get_setting('newsflash_uselocalcontent')) {
    $custom_css =  theme_get_setting('newsflash_localcontentfile');
    if (file_exists($custom_css)) {
      drupal_add_css($custom_css);
    }
  }

  // Start processing the inline CSS

  $ldir = $variables['language']->dir;

  $page_width = theme_get_setting('newsflash_width');
  if (!$page_width) {
    $page_width = '95%';
  }

  $css = "#page { width: $page_width; }\n";

  $left_sidebar_width = theme_get_setting('newsflash_leftsidebarwidth');
  if ($left_sidebar_width) {
    if ($ldir == 'rtl') {
      $css .= "body.sidebar-first #main, body.two-sidebars #main { margin-left: 0px; margin-right: -${left_sidebar_width}px !important; }\n";
      $css .= "body.sidebar-first #squeeze, body.two-sidebars #squeeze { margin-left: 0px; margin-right: ${left_sidebar_width}px !important;; }\n";
    }
    
    if ($ldir == 'ltr') {
      $css .= "body.sidebar-first #main, body.two-sidebars #main { margin-left: -${left_sidebar_width}px !important; margin-right: 0px;}\n";
      $css .= "body.sidebar-first #squeeze, body.two-sidebars #squeeze { margin-left: ${left_sidebar_width}px !important; margin-right: 0px; }\n";
    }
      $css .= "#sidebar-left { width: ${left_sidebar_width}px; }\n";
  }

  $right_sidebar_width = theme_get_setting('newsflash_rightsidebarwidth');
  if ($right_sidebar_width) {
    if ($ldir == 'rtl') {
      $css .= "body.sidebar-second #main, body.two-sidebars #main { margin-right: 0px; margin-left: -${right_sidebar_width}px !important; }\n";
      $css .= "body.sidebar-second #squeeze, body.two-sidebars #squeeze { margin-right: 0px; margin-left: ${right_sidebar_width}px !important; }\n";
    }
    
    if ($ldir == 'ltr') {
      $css .= "body.sidebar-second #main, body.two-sidebars #main { margin-right: -${right_sidebar_width}px !important; margin-left: 0px;}\n";
      $css .= "body.sidebar-second #squeeze, body.two-sidebars #squeeze { margin-right: ${right_sidebar_width}px !important; margin-left: 0px; }\n";
    }
    $css .= "#sidebar-right { width: ${right_sidebar_width}px; }\n";
  }

  if (theme_get_setting('newsflash_fontfamily')) {
    $css .= 'body { font-family: ' . (theme_get_setting('newsflash_fontfamily') != 'Custom' ? theme_get_setting('newsflash_fontfamily') : str_replace(array("&quot;", '\\', '/', '&#039;', '(', ')', ':', ';'), array('"', ''), check_plain(theme_get_setting('newsflash_customfont')))) . "; }\n";
  }

  if (theme_get_setting('newsflash_usecustomlogosize')) {
    $css .= 'img#logo { width: ' . theme_get_setting('newsflash_logowidth') . 'px; height: ' . theme_get_setting('newsflash_logoheight') . "px; }\n";
  }

  if (theme_get_setting('newsflash_suckerfish')) {
    $css .= '#suckerfishmenu div .contextual-links-wrapper { display: none; }';
  }

  // Add inline CSS
  drupal_add_css($css, array('type' => 'inline', 'preprocess' => FALSE));

  // Add conditional stylesheet for IE
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('!IE' => FALSE), 'preprocess' => FALSE));

  if (theme_get_setting('newsflash_pickstyle')) {
    drupal_add_js(path_to_theme() . '/js/pickstyle.js');
  }
}

/**
 * Implements hook_preprocess_page()
 */
function newsflash_preprocess_page(&$variables) {
  if (theme_get_setting('newsflash_themelogo') and theme_get_setting('logo')) {
    $variables['logo'] = base_path() . path_to_theme() . '/images/' . get_newsflash_style() . '/logo.png';
  }
  $variables['title_attributes_array']['class'][] = 'title';
}

/**
 * Implements hook_preprocess_node()
 * inject the class "title" in the node teaser
 */
function newsflash_preprocess_node(&$variables) {
  $variables['title_attributes_array']['class'][] = 'title';
/**
 * Test implementation for a title suffix like a blinking news
 */
/**
  if ($variables['created'] >= (TIME() - ( 2 * 24 * 60 *60 ))) { // ( 2 * 24 * 60 *60 ) = two Days
    $variables['title_attributes_array']['class'][] = 'new';
  }
 */
}

/**
 * Implements hook_preprocess_block()
 */
function newsflash_preprocess_block(&$variables) {
  // In the suckerfishmenu region visually hide block titles if use suckerfish enabled.
  if (theme_get_setting('newsflash_suckerfish') && $variables['block']->region == 'suckerfish') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }

  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
    //adds in blocks title the class .title
    $variables['title_attributes_array']['class'][] = 'title';
}

/**
 * Implements hook_process_html()
 */
function newsflash_process_html(&$variables) {

  if (theme_get_setting('newsflash_suckerfish')) {
      $variables['scripts'] .= '<!--[if lte IE 6]>
        <script type="text/javascript" src="' . path_to_theme() . '/js/suckerfish.js"></script>
      <![endif]-->';
  }
}
