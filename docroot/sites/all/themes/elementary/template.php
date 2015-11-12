<?php
/**
 * @file
 * Template.php provides theme functions & overrides
 */

/**
 * Implements hook_css_alter().
 */
function elementary_css_alter(&$css) {
  global $user;
  if (theme_get_setting('remove_all_css') && $user->uid == 0) {
    foreach ($css as $key => $val) {
      if (!strpos($key, "elementary")) {
        unset($css[$key]);
      }
    }
  }
}

/**
 * Implements hook_preprocess_html().
 */
function elementary_preprocess_html(&$vars) {
  $font_style = theme_get_setting('font_serif');
  if (isset($font_style) && $font_style == 'serif') {
    $vars['classes_array'][] = 'serif';
  }
}

/**
 * Implements hook_preprocess_page().
 */
function elementary_preprocess_page(&$vars) {
  // Theme the $main_menu links.
  if (isset($vars['main_menu'])) {
    $vars['page']['menu'] = theme('links', array('links' => $vars['main_menu']));
  }
  // Add some extra CSS for admin sections.
  if (user_access('access administration pages')) {
    $path = drupal_get_path('theme', 'elementary') . '/css/admin.css';
    drupal_add_css($path);
  }
  // Add in the attribution link - TODO: make this optional?
  $vars['attribution_link'] = l(t("Elementary - Designed By Thomas"), "http://designedbythomas.co.uk", array('attributes' => array('class' => 'attribution')));
}
/**
 * Implements hook_preprocess_node().
 */
function elementary_preprocess_node(&$vars) {
  // Format the new $date variable.
  $vars['date'] = format_date($vars['created'], 'custom', 'd M Y');
  // Add in template suggestion for teasers.
  if ($vars['view_mode'] == 'teaser') {
    $vars['theme_hook_suggestions'][] = 'node__teaser';
  }
}

/**
 * Implements hook_preprocess_comment().
 */
function elementary_preprocess_comment(&$vars) {
  // Format the new $date variable.
  $created = $vars['elements']['#comment']->created;
  $vars['date'] = format_date($created, 'custom', 'd M Y');
}

/**
 * Implements theme_textarea().
 */
function elementary_textarea(&$vars) {
  // Remove the grippie!
  $vars['element']['#resizable'] = FALSE;
  return theme_textarea($vars);
}

/**
 * Remove the comment filter tips.
 */
function elementary_filter_tips($tips, $long = FALSE, $extra = '') {
  return;
}

/**
 * Remove the comment filter's more information tips link.
 */
function elementary_filter_tips_more_info() {
  return;
}
