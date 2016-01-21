<?php
/**
 * @file
 * WhiteJazz D7.x template.php
 */

/**
 * Implemts hook_preprocess_html()
 */
function whitejazz_preprocess_html(&$variables) {

  drupal_add_css(path_to_theme() . '/style.css'); // is not setted in info file because it's loades to late

  if (theme_get_setting('whitejazz_uselocalcontent')) {
    $local_content = path_to_theme() . '/' . theme_get_setting('whitejazz_localcontentfile');
    if (file_exists($local_content)) {
      drupal_add_css($local_content);
    }
  }

  drupal_add_css
  (
    path_to_theme() . '/css/ie.css',
    array
    (
      'group' => CSS_THEME,
      'browsers' => array
      (
      'IE' => 'IE',
      '!IE' => FALSE
      ),
    'preprocess' => FALSE
    )
  );
}

/**
  * Implements hook_preprocess_block()
  */
function whitejazz_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'header' || $variables['block']->region == 'suckerfish') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
  $variables['title_attributes_array']['class'][] = 'title';
}

/**
 * Implements hook_process_html()
 * returns side calculatings for WhiteJazz fillet in $variables['styles']
 * and $variables['scripts']
 */
function whitejazz_process_html(&$variables) {
  // calculating sidbar width and content width
  $variables['styles'] .= '<script type="text/javascript">'/* Needed to avoid Flash of Unstyle Content in IE */ . '</script>';

  if (theme_get_setting('whitejazz_width')) {
    $variables['styles'] .= '<style type="text/css">
    #page {
      width : ' . theme_get_setting('whitejazz_width') . ';
    }
    </style>';
  }
  else {
    $variables['styles'] .= '<style type="text/css">
    #page {
      width: 95%;
    }
    </style>';
  }

  if ($left_sidebar_width = theme_get_setting('whitejazz_leftsidebarwidth')) {
    $variables['styles'] .= '<style type="text/css">
    body.sidebar-first #main { margin-left: -' . $left_sidebar_width . 'px; }
    body.two-sidebars #main { margin-left: -' . $left_sidebar_width . 'px; }
    body.sidebar-first #squeeze { margin-left: ' . $left_sidebar_width . 'px; }
    body.two-sidebars #squeeze { margin-left: ' . $left_sidebar_width . 'px; }
    #sidebar-left { width: ' . $left_sidebar_width . 'px; }
    </style>';
  }

  if ($right_sidebar_width = theme_get_setting('whitejazz_rightsidebarwidth')) {
    $variables['styles'] .= '<style type="text/css">
    body.sidebar-second #main { margin-right: -' . $right_sidebar_width . 'px; }
    body.two-sidebars #main { margin-right: -' . $right_sidebar_width . 'px; }
    body.sidebar-second #squeeze { margin-right: ' . $right_sidebar_width . 'px; }
    body.two-sidebars #squeeze { margin-right: ' . $right_sidebar_width . 'px; }
    #sidebar-right { width: ' . $right_sidebar_width . 'px; }
    </style>';
  }

  if (theme_get_setting('whitejazz_fontfamily') != 'Custom') {
    $variables['styles'] .= '<style type="text/css">
    body {
      font-family : ' . theme_get_setting('whitejazz_fontfamily') . ';
    }
    </style>';
  }
  elseif (theme_get_setting('whitejazz_customfont')) {
    $variables['styles'] .= '<style type="text/css">
    body {
      font-family : ' . theme_get_setting('whitejazz_customfont') . ';
    }
    </style>';
  }

  if (theme_get_setting('whitejazz_usecustomlogosize')) {
    $variables['styles'] .= '<style type="text/css">
    img#logo {
      width : ' . theme_get_setting('whitejazz_logowidth') . 'px;
      height: ' . theme_get_setting('whitejazz_logoheight') . 'px;
    }
    </style>';
  }

  $variables['scripts'] .= '<!--[if lte IE 6]>
  <script type="text/javascript" src="' . $GLOBALS['base_url'] . '/' . path_to_theme() . '/js/suckerfish.js"></script>
  <![endif]-->';
}
