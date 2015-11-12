<?php
// $Id: template.php,v 1.2 2010/10/14 06:03:29 crashtest Exp $

///**
// * Return a themed breadcrumb trail.
// *
// * @param $breadcrumb
// *   An array containing the breadcrumb links.
// * @return a string containing the breadcrumb output.
// */
//function pockett_breadcrumb($variables) {
//  if (!empty($variables['breadcrumb'])) {
//    return '<div class="breadcrumb">' . implode(' › ', $variables['breadcrumb']) . '</div>';
//  }
//}

///**
// * Allow themable wrapping of all comments.
// */
//function pockett_comment_wrapper($content, $node) {
//  if (!$content || $node->type == 'forum') {
//    return '<div id="comments">' . $content . '</div>';
//  }
//  else {
//    return '<div id="comments"><h2 class="comments">' . t('Comments') . '</h2>' . $content . '</div>';
//  }
//}

/**
 * Override or insert variables into the page template.
 */
function pockett_preprocess_page(&$variables) {
  $variables['tabs2'] = menu_secondary_local_tasks();
  $variables['primary_nav'] = isset($variables['main_menu']) ? theme('links', $variables['main_menu'], array('class' => 'links main-menu')) : FALSE;
  $variables['secondary_nav'] = isset($variables['secondary_menu']) ? theme('links', $variables['secondary_menu'], array('class' => 'links secondary-menu')) : FALSE;
  $variables['ie_styles'] = pockett_get_ie_styles();

  // Prepare header
  $site_fields = array();
  if (!empty($variables['site_name'])) {
    $site_fields[] = check_plain($variables['site_name']);
  }
  if (!empty($variables['site_slogan'])) {
    $site_fields[] = check_plain($variables['site_slogan']);
  }
  $variables['site_title'] = implode(' ', $site_fields);
  if (!empty($site_fields)) {
    $site_fields[0] = '<span>'. $site_fields[0] .'</span>';
  }
  $variables['site_html'] = implode(' ', $site_fields);

  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs. Overridden to split the secondary tasks.
 */
function pockett_menu_local_tasks() {
  return menu_primary_local_tasks();
}

/**
 * Format the "Submitted by username on date/time" for each comment.
 */
function phptemplate_comment_submitted($comment) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $comment),
      '!datetime' => format_date($comment->timestamp)
    ));
}

/**
 * Format the "Submitted by username on date/time" for each node.
 */
function pockett_node_submitted($node) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $node),
      '!datetime' => format_date($node->created),
    ));
}

/**
 * Generates IE CSS links for LTR and RTL languages.
 */
function pockett_get_ie_styles() {
  global $language;

  $ie_styles = '<link type="text/css" rel="stylesheet" media="all" href="' . base_path() . path_to_theme() . '/fix-ie.css" />'. "\n";
  if ($language->direction == LANGUAGE_RTL) {
    $ie_styles .= '      <style type="text/css" media="all">@import "' . base_path() . path_to_theme() . '/fix-ie-rtl.css";</style>'. "\n";
  }

  return $ie_styles;
}
