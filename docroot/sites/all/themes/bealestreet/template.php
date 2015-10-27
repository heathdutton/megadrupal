<?php
/**
 * @file
 * Beale Street template.php
 */
  
function get_bealestreet_style() {
// set the color CSS and if Style Picker JS Enabled then calls the cookies
  $style = theme_get_setting('bealestreet_style');
  if (!$style) {
    $style = 'blue';
  }
  if (theme_get_setting('bealestreet_pickstyle')) {
    if (isset($_COOKIE["bealestyle"])) {
      $style = $_COOKIE["bealestyle"];
    }
  }
  return $style;
}

/**
 * Implements hook_preprocess_html()
 */
function bealestreet_preprocess_html(&$variables) {
  $style =  get_bealestreet_style();

  drupal_add_css(path_to_theme() . '/style.css');
  drupal_add_css(path_to_theme() . '/css/' . $style . '.css');

  if (theme_get_setting('bealestreet_suckerfish')) {
    drupal_add_css(path_to_theme() . '/css/suckerfish_' . $style . '.css');
  }

  if (theme_get_setting('bealestreet_uselocalcontent')) {
    $local_content = path_to_theme() . '/' . theme_get_setting('bealestreet_localcontentfile');
    if (file_exists($local_content)) {
      drupal_add_css($local_content);
    }
  }

  if (theme_get_setting('bealestreet_pickstyle')) {
    drupal_add_js(path_to_theme() . '/js/pickstyle.js');
  }
}

/**
 * Implements hook_preprocess_block()
 */
function bealestreet_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if (theme_get_setting('bealestreet_suckerfish') && ($variables['block']->region == 'suckerfish_menu')) {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
  if ($variables['block']->region == 'header' || $variables['block']->region == 'search_box') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
  $variables['title_attributes_array']['class'][] = 'title';
}

/**
 * Implements hook_process_html()
 * calculates sidewidth for seidebars and main content
 */
function bealestreet_process_html(&$variables) {

  $variables['styles'] .= '<script type="text/javascript">' /* Needed to avoid Flash of Unstyle Content in IE */ . '</script>';

  if (theme_get_setting('bealestreet_width')) {
    $variables['styles'] .= '<style type="text/css">
    #page {
      width : ' . theme_get_setting('bealestreet_fixedwidth') . 'px;
    }
    .topBlock, .bottomBlock, .middleWrapper, #loginWrapper {
      width : ' . theme_get_setting('bealestreet_fixedwidth') + 30 . 'px;
    }
    </style>';
  }
  else {
    $variables['styles'] .= '<style type="text/css">
    #page {
      width: 95%;
    }
    .topBlock, .bottomBlock, .middleWrapper, #loginWrapper {
      width : 95%;
    }
    </style>';
  } 
  if (theme_get_setting('bealestreet_leftsidebarwidth')) {
    $variables['styles'] .= '<style type="text/css">
    #sidebar-left, .region-sidebar-first {
      width : ' . theme_get_setting('bealestreet_leftsidebarwidth') . 'px;
    }
    </style>';
  } 
  if (theme_get_setting('bealestreet_rightsidebarwidth')) {
    $variables['styles'] .= '<style type="text/css">
    #sidebar-right, .region-sidebar-second {
      width : ' . theme_get_setting('bealestreet_rightsidebarwidth') . 'px;
    }
    </style>';
  } 

  if (theme_get_setting('bealestreet_fontfamily') != 'Custom') {
    $variables['styles'] .= '<style type="text/css">
    body {
      font-family : ' . theme_get_setting('bealestreet_fontfamily') . ';
    }
    </style>';
  }
  elseif (theme_get_setting('bealestreet_customfont')) {
    $variables['styles'] .= '<style type="text/css">
    body {
      font-family : ' . theme_get_setting('bealestreet_customfont') . ';
    }
    </style>';
  }

  if (theme_get_setting('bealestreet_suckerfish')) {
    $variables['scripts'] .= '<!-- if lte IE6]>
    <script type="text/javascript">
      <script type="text/javascript" src="' . path_to_theme() . '/js/suckerfish.js"></script>
    <![endif]-->';
  }
}
