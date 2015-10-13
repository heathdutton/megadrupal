<?php

/**
 * @file
 *  Theming functions. 
 */

function theme_social_groups($vars) {
  $groups = $vars['groups'];
  $output = NULL;
  foreach ($groups as $group) {
    $output .= $group;
  }
  
  return $output;
}

function theme_social_group_facebook($vars) {
  $module_path = drupal_get_path('module', 'social_groups');
  $group = $vars['group'];
  $output = '<div class="social_group_item facebook">';
  $output .= '<div class="social_group_logo facebook">';
  $image = theme('image', array('path' =>  $module_path. '/themes/images/facebook.png'));
  $output .= l($image, $group['url'], array('html' => TRUE));
  $output .= '</div>';
  $output .= '<div class="social_group_stat facebook">';
  $output .= t('!count Likes', array('!count' => $group['count']));
  $output .= '</div>';
  $output .= '<div class="social_group_description facebook">';
  $output .= l(t('Like Us on Facebook'), $group['url']);
  $output .= '</div>';
  $output .= '</div>';
  
  return $output;
}

function theme_social_group_twitter($vars) {
  $module_path = drupal_get_path('module', 'social_groups');
  $group = $vars['group'];
  $output = '<div class="social_group_item twitter">';
  $output .= '<div class="social_group_logo twitter">';
  $image = theme('image', array('path' =>  $module_path. '/themes/images/twitter.png'));
  $output .= l($image, $group['url'], array('html' => TRUE));
  $output .= '</div>';
  $output .= '<div class="social_group_stat twitter">';
  $output .= t('!count Followers', array('!count' => $group['count']));
  $output .= '</div>';
  $output .= '<div class="social_group_description twitter">';
  $output .= l(t('Follow Us on Twitter'), $group['url']);
  $output .= '</div>';
  $output .= '</div>';
  
  return $output;
}

function theme_social_group_youtube($vars) {
  $module_path = drupal_get_path('module', 'social_groups');
  $group = $vars['group'];
  $output = '<div class="social_group_item youtube">';
  $output .= '<div class="social_group_logo youtube">';
  $image = theme('image', array('path' =>  $module_path. '/themes/images/youtube.png'));
  $output .= l($image, $group['url'], array('html' => TRUE));
  $output .= '</div>';
  $output .= '<div class="social_group_stat youtube">';
  $output .= t('!count Subscribers', array('!count' => $group['count']));
  $output .= '</div>';
  $output .= '<div class="social_group_description youtube">';
  $output .= l(t('Subscribe Us on Youtube'), $group['url']);
  $output .= '</div>';
  $output .= '</div>';
  
  return $output;
}