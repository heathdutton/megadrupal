<?php

/**
 * @file
 * LiteJazz template.php
 */
 
function get_litejazz_style() {
  $style = theme_get_setting('litejazz_style');
  if (!$style) {
    $style = 'blue';
  }
  if (theme_get_setting('litejazz_pickstyle')) {
    if (isset($_COOKIE["litejazzstyle"])) {
      $style = $_COOKIE["litejazzstyle"];
    }
  }
  return $style;
}

/**
 * Implements hook_preprocess_html()
 **/
function litejazz_preprocess_html(&$variables) {
  drupal_add_css(path_to_theme() . '/style.css'); //is not settet in info file because it's loades to late
  drupal_add_css(drupal_get_path('theme', 'litejazz') . '/css/' . get_litejazz_style() . '.css');

  if (theme_get_setting('litejazz_pickstyle')) {
    drupal_add_js(drupal_get_path('theme', 'litejazz') . '/js/pickstyle.js');
  }
  
  if (theme_get_setting('litejazz_suckerfish')) {
    drupal_add_css(drupal_get_path('theme', 'litejazz') . '/css/suckerfish_'  . get_litejazz_style() . '.css');
  }

  if (theme_get_setting('litejazz_uselocalcontent')) {
  // $local_content = drupal_get_path('theme', 'litejazz') . '/' . theme_get_setting('litejazz_localcontentfile');
    $local_content =  theme_get_setting('litejazz_localcontentfile');
    if (file_exists($local_content)) {
      drupal_add_css($local_content);
    }
  }
}

/**
 * Implements hook_preprocess_page()
 **/
function litejazz_preprocess_page(&$variables) {
  if (theme_get_setting('litejazz_themelogo')) {
    $variables['logo'] = base_path() . path_to_theme() . '/images/' . get_litejazz_style() . '/logo.png';
  }
}

/**
 * Implements hook_preprocess_block()
 **/
function litejazz_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if (theme_get_setting('litejazz_suckerfish') && ($variables['block']->region == 'suckerfish')) {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
  
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
  
  $variables['title_attributes_array']['class'][] = 'title';
}

/**
 * Implements hook_process_html()
 **/
function litejazz_process_html(&$variables) {
  // calculates side width and fonts
  $variables['styles'] .= '<style type="text/css">'/* Nedded to avoid */ .'</style>';

  if (theme_get_setting('litejazz_width')) {
  $variables['styles'] .= '<style type="text/css">
      #page {
        width: ' . theme_get_setting('litejazz_width') . ';
      }
        </style>';
  } else {
  $variables['styles'] .= '<style type="text/css">
      #page {
        width: 95%;
      }
        </style>';
  } 
   
  if ($left_sidebar_width = theme_get_setting('litejazz_leftsidebarwidth')) {
      $variables['styles'] .= '<style type="text/css">
        body.sidebar-first #main 
        {
          margin-left: -' . $left_sidebar_width . 'px;
        }
        body.two-sidebars #main 
        {
          margin-left: -' . $left_sidebar_width . 'px;
        }
        body.sidebar-first #squeeze 
        {
          margin-left: ' . $left_sidebar_width . 'px;
        }
        body.two-sidebars #squeeze 
        {
          margin-left: ' . $left_sidebar_width . 'px;
        }
        #sidebar-left 
        {
          width: ' . $left_sidebar_width . 'px;
        }
      </style>';
  } 

  if ($right_sidebar_width = theme_get_setting('litejazz_rightsidebarwidth')) {
      $variables['styles'] .= '<style type="text/css">
        body.sidebar-second #main 
        {
          margin-right: -' . $right_sidebar_width . 'px;
        }
        body.two-sidebars #main 
        {
          margin-right: -' . $right_sidebar_width . 'px;
        }
        body.sidebar-second #squeeze 
        {
          margin-right: ' . $right_sidebar_width . 'px;
        }
        body.two-sidebars #squeeze 
        {
          margin-right: ' . $right_sidebar_width . 'px;
        }
        #sidebar-right 
        {
          width: ' . $right_sidebar_width . 'px;
        }
      </style>';
  } 

  if (theme_get_setting('litejazz_fontfamily') != 'Custom') {

  $variables['styles'] .= '<style type="text/css">
              body {
                font-family : ' . theme_get_setting('litejazz_fontfamily') . ';
              }
           </style>';
  }
  elseif (theme_get_setting('litejazz_fontfamily')) {
  $variables['styles'] .= '<style type="text/css">
              body {
                font-family : ' . theme_get_setting('litejazz_customfont') . ';
              }
           </style>';
  } 

  if (theme_get_setting('litejazz_usecustomlogosize')) {
    $variables['styles'] .= '<style type="text/css">
          img#logo {
            width : ' . theme_get_setting('litejazz_logowidth') . 'px;
            height: ' . theme_get_setting('litejazz_logoheight') . 'px;
          }
        </style>';
  } 

  $variables['styles'] .= '<!--[if IE]>
  <style type="text/css" media="all">@import "' . base_path() . path_to_theme() . '/css/ie.css";</style>
  <![endif]-->';

  if (theme_get_setting('litejazz_suckerfish')) {
    $variables['styles'] .= '<style type="text/css">
        #suckerfishmenu div .contextual-links-wrapper {
           display:none;
        }
    </style>';
    $variables['scripts'] .= '<!--[if lte IE 6]>
      <script type="text/javascript" src="' . $GLOBALS['base_url'] . '/' . path_to_theme() . '/js/suckerfish.js"></script>
    <![endif]-->';
  }
}
