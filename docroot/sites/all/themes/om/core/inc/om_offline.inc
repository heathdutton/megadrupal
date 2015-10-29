<?php
// $Id$

/**
 * @file
 * Theme Functions - Set site to offline mode
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Additional Meta from Info file
 *
 */
function om_offline(&$vars, $info) { 
  global $user;
  $om_path = OM_BASE_THEME_PATH;
  
  if (isset($info['settings']['offline']['switch']) && ($info['settings']['offline']['switch'] == 'on') && ($user->uid == 0)) {

    om_offline_time($vars, $info);    

    if ($vars['om_offline']) {
      $vars['footer_region'] = om_offline_content($vars, $info);

      $countdown = '';
      $countdown .= 'jQuery(document).ready(function ($) {';
      $countdown .= '	 var onlineDay = new Date(' . $vars['settings']['year'] . ', ' . $vars['settings']['month'] . '-1, ' . $vars['settings']['days'] . ', ' . $vars['settings']['hours'] . ', ' . $vars['settings']['minutes'] . ', ' . $vars['settings']['seconds'] . ');'; 

      $countdown .= '	 $("#om-launch-content #om-launch-counter").countdown({until: onlineDay, onExpiry: launch, alwaysExpire: true});';
      $countdown .= '	 $("body").prepend($("#om-launch-content"));';
      $countdown .= '	 $(".wrapper-outer").remove();';
      $countdown .= '	 function launch() { document.location.href = ""; };';
      $countdown .= '});';
      drupal_add_js($om_path . '/js/jquery.countdown.js'); 
      drupal_add_js($countdown, "inline"); 
      drupal_add_css($om_path . '/css/om_offline.css', array('group' => CSS_THEME));       
    } 
  }
}


/**
 * OM Offline Screen
 * 
 * - Logo
 * - Site Name
 * - Message
 * - Login
 *
 */
function om_offline_content(&$vars, $info) { 
  global $theme_path;
  $login_form = drupal_get_form('user_login');
  $out = '<div id="om-launch-content">';
  $out .= '<div id="logo"><a href="' . $vars['front_page'] . '" title="' . t('Home') . '" rel="home"><img src="' . $theme_path . '/logo.png" alt="' . t('Home') . '" /></a></div>';
  $out .= '<h1 id="site-name"><a href="' . $vars['front_page'] . '" title="' . t('Home') . '" rel="home">' . $vars['site_name'] . '</a></h1>';
  $out .= '<p>' . $info['settings']['offline']['message'] . ' ' . $vars['feed_icons'] . '</p>'; 
  $out .= '<div id="om-launch-counter"></div>';
  $out .= drupal_render($login_form);
  $out .= '</div>';
  
  return $out;
} 


/**
 * Calculates total time in seconds
 * for both settings and current
 *
 */
function om_offline_time(&$vars, $info) { 

  // leap years
  $leapyears = array(1 => 2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040, 2044, 2048, 2052, 2056, 2060, 2064, 2068, 2072, 2076, 2080, 2084, 2088, 2092, 2096, 2104, 2108);

  // standard days in months
  $months = array(1 => 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);  

  // conversion to seconds
  $seconds_year = 365 * 24 * 60 * 60;
  $seconds_day  = 24 * 60 * 60;
  $seconds_hour = 60 * 60;      
  $seconds      = 60;      
  
  // current
  $today   = getdate();
  $months_to_days = 0;
  for ($i = 1; $i <= $today['mon']; $i++) {
    $months[2] = (in_array($today['year'], $leapyears)) ? 29: 28;
    $months_to_days += $months[$i];
  }   
                  
  // hour, minute, second, month, day, year
  $now = 0;
  $now += $today['year']    * $seconds_year;
  $now += $months_to_days   * $seconds_day;
  $now += $today['mday']    * $seconds_day;
  $now += $today['hours']   * $seconds_hour;
  $now += $today['minutes'] * $seconds;
  $now += $today['seconds'];  
  
  // settings
  $vars['settings']['year']    = isset($info['settings']['countdown']['year']) ? $info['settings']['countdown']['year']: $today['year'];
  $vars['settings']['month']   = isset($info['settings']['countdown']['month']) ? $info['settings']['countdown']['month']: 0;
  $vars['settings']['days']    = isset($info['settings']['countdown']['days']) ? $info['settings']['countdown']['days']: 0;      
  $vars['settings']['hours']   = isset($info['settings']['countdown']['hours']) ? $info['settings']['countdown']['hours']: 0;      
  $vars['settings']['minutes'] = isset($info['settings']['countdown']['minutes']) ? $info['settings']['countdown']['minutes']: 0;      
  $vars['settings']['seconds'] = isset($info['settings']['countdown']['seconds']) ? $info['settings']['countdown']['seconds']: 0;      

  $settings_months_to_days = 0;
  for ($i = 1; $i <= $vars['settings']['month']; $i++) {
    $months[2] = (in_array($vars['settings']['year'], $leapyears)) ? 29: 28;
    $settings_months_to_days += $months[$i];
  }   
         
  $settings = 0;
  $settings += $vars['settings']['year']    * $seconds_year;
  $settings += $settings_months_to_days     * $seconds_day;
  $settings += $vars['settings']['days']    * $seconds_day;
  $settings += $vars['settings']['hours']   * $seconds_hour;
  $settings += $vars['settings']['minutes'] * $seconds;
  $settings += $vars['settings']['seconds'];

  $vars['om_offline'] = ($now < $settings) ? TRUE: FALSE;
}




 
