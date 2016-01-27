<?php


/**
 * Implements hook_preprocess_html().
 */
function metro_preprocess_html(&$vars) {
  $theme_path = drupal_get_path('theme', 'metro');
  drupal_add_css("$theme_path/metro-style-ie-7.css", array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css("$theme_path/metro-style-ie-8.css", array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Implements hook_css_alter().
 */
function metro_css_alter(&$css) {
  $theme_path = drupal_get_path('theme', 'metro');
  if (isset($css["$theme_path/css/ie.css"]))  unset($css["$theme_path/css/ie.css"]);
  if (isset($css["$theme_path/css/ie6.css"])) unset($css["$theme_path/css/ie6.css"]);
}


/**
 * Implements hook_form_alter().
 */
function metro_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'user_login_block') {
    $form['actions']['#weight'] = 1;
    $form['links']['#weight'] = 4;
    $form['links']['#prefix'] = '<div class="links-wr">';
    $form['links']['#suffix'] = '</div>';
    $form['openid_links']['#weight'] = 5;
    $form['openid_links']['#prefix'] = '<div class="links-openid-wr">';
    $form['openid_links']['#suffix'] = '</div>';
    $form['clearfix_right'] = array('#markup' => '<div class="clearfix-left"></div>', '#weight' => 100);
  }
}

