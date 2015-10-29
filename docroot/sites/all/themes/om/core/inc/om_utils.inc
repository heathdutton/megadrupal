<?php
// $Id$

/**
 * @file
 * Theme Functions - Utilities
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Body Classes
 *
 * - Browser classes
 * - Grid Classes
 *
 */
function om_body_classes(&$vars, $info = array(), $agent = NULL) {

  // general grid
  $grids_allowed = array(12, 16, 24);
  if (isset($info['settings']['grid']) && in_array($info['settings']['grid'], $grids_allowed)) $vars['classes_array'][] = ' grids-' . $info['settings']['grid'];    
  
  if (isset($info['settings']['browsers']['class']) && ($info['settings']['browsers']['class'] == 'on')) {
    // Default known browsers
    $known_browsers = explode(',', preg_replace('/ /', '', $info['settings']['browsers']['known']));
    $agent = strtolower($agent ? $agent : $_SERVER['HTTP_USER_AGENT']);
    $pattern = '#(?<browser>' . join('|', $known_browsers) . ')[/ ]+(?<version>[0-9]+(?:\.[0-9]+)?)#';
  
    if (!preg_match_all($pattern, $agent, $matches)) return array();
    $i = count($matches['browser'])-1;
  
    $browser = array();
    $browser['browser'] = isset($matches['browser'][$i]) ? 'browser-' . om_class_safe($matches['browser'][$i]): '';
    $browser['version'] = (isset($matches['browser'][$i]) && isset($matches['version'][$i])) ? 'browser-' . om_class_safe($matches['browser'][$i]) . '-' . om_class_safe($matches['version'][$i]): '';  

    $vars['classes_array'][] = $browser['browser'];
    $vars['classes_array'][] = $browser['version'];
  }
}


/**
 * Additional Meta from Info file
 *
 */
function om_meta_get(&$vars, $info) { 

  $meta = isset($info['head']['meta']) ? $info['head']['meta']: FALSE; 
  if ($meta) {
    foreach ($meta as $name => $contents) {
      if (is_array($contents)) {
        $content = '';
        $ctotal = count($contents);
        $ccount = 0;
        foreach ($contents as $ckey => $cval) {
          $ccount++;
          $csep = ($ctotal != $ccount) ? ', ': '';
          $content .= $ckey . '=' . $cval . $csep;   
        }
      }
      else {
        $content = $contents;
      }
      $diff_keys = array('cleartype', 'X-UA-Compatible');
      $name_key = (!in_array($name, $diff_keys)) ? 'name': 'http-equiv';
      $vars['head'] .= '<meta ' . $name_key . '="' . $name . '" content="' . $content . '"/>' . "\n";  
    }
  } 
}


/**
 * Converts a string to a suitable html ID attribute.
 *
 * http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
 * valid ID attribute in HTML. This function:
 *
 * - Ensure an ID starts with an alpha character by optionally adding an 'id'.
 * - Replaces any character except alphanumeric characters with dashes.
 * - Converts entire string to lowercase.
 *
 * @param $string
 *   The string
 * @return
 *   The converted string
 */
function om_class_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  return strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $string));
} 


/**
 * Generates IE CSS links for LTR and RTL languages.
 *
 * <!--[if IE  ]><?php print om_get_ie_styles('ie'); ?><![endif]-->
 * <!--[if IE 6]><?php print om_get_ie_styles('ie6'); ?><![endif]-->
 * <!--[if IE 7]><?php print om_get_ie_styles('ie7'); ?><![endif]-->
 * <!--[if IE 8]><?php print om_get_ie_styles('ie8'); ?><![endif]-->
 * <!--[if IE 9]><?php print om_get_ie_styles('ie9'); ?><![endif]-->
 */

function om_get_ie_styles($ie = NULL) {
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
 


