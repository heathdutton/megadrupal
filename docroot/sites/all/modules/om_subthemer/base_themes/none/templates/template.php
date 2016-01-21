<?php
/**
 * Generates IE CSS links for LTR and RTL languages.
 *
 * <!--[if IE  ]><?php print om_get_ie_styles('ie'); ?><![endif]-->
 * <!--[if IE 6]><?php print om_get_ie_styles('ie6'); ?><![endif]-->
 * <!--[if IE 7]><?php print om_get_ie_styles('ie7'); ?><![endif]-->
 * <!--[if IE 8]><?php print om_get_ie_styles('ie8'); ?><![endif]-->
 * <!--[if IE 9]><?php print om_get_ie_styles('ie9'); ?><![endif]-->
 */

function phptemplate_get_ie_styles($ie = NULL) {
  global $language;
  global $theme_path;
  
  $iecss = '';
  
  if (empty($ie)) {
    // depends on files
    $ies = array('ie', 'ie6', 'ie7', 'ie8', 'ie9');
  
    foreach ($ies as $key => $ie) {
      if (file_exists($theme_path . '/css/' . $ie . '.css')) {
        switch ($ie) {
          case  'ie': $num = ''; break;
          case 'ie6': $num =  6; break;
          case 'ie7': $num =  7; break;
          case 'ie8': $num =  8; break;
          case 'ie9': $num =  9; break;  
             default: $num = ''; break;                     
        }
        $iecss .= '<!--[if IE ' . $num . ']><link type="text/css" rel="stylesheet" media="all" href="' . base_path() . $theme_path . '/css/' . $ie . '.css" /><![endif]-->' . "\n";
      }
    }
  }
  else {
    // depends on head declaration
    $iecss = '<link type="text/css" rel="stylesheet" media="all" href="' . base_path() . $theme_path . '/css/' . $ie . '.css" />';
  }  
  if ($language->direction == LANGUAGE_RTL) $iecss .= '<style type="text/css" media="all">@import "' . base_path() . $theme_path . '/css/ie-rtl.css";</style>';  

  return $iecss;
}
 


