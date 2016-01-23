<?php


/**
 * Implements hook_preprocess_html().
 */

function led_preprocess_html(&$vars) {
}


/**
 * Implements hook_css_alter().
 */

function led_css_alter(&$css) {
}


/**
 * Implements hook_form_alter().
 */

function led_form_alter(&$form, &$form_state, $form_id) {
}


/**
 * Implements hook_status_messages().
 */

function led_status_messages($variables) {
  return theme_status_messages($variables);
}


/**
 * Implements theme_menu_link().
 */
function led_menu_link($link) {
  if ($link['element']['#href'] == 'user') {
    $link['element']['#title'] = t('Profile');
    $link['element']['#localized_options']['attributes']['class'] = array('link-user');
    if (arg(0) == 'user' && arg(1) == $GLOBALS['user']->uid) {
      $link['element']['#localized_options']['attributes']['class'][]= 'link-user-active';
      $link['element']['#localized_options']['attributes']['class'][]= 'active';
    }
  }
  return theme_menu_link($link);
}


