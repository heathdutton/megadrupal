<?php
/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by user themes.
 */

/**
 * Implements html_head_alter().
 *
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function da_vinci_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
  unset($head_elements['metatag_content-language_0']);
}

/**
 * Implements da_vinci_status_messages().
 */
function da_vinci_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );
  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= "<div class=\"messages $type\">\n";
    $output .= "<div class=\"container\">\n";
    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>\n";
    }
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= '<span>' . $messages[0] . '</span>';
    }
    $output .= "</div>\n";
    $output .= "</div>\n";
  }
  return $output;
}

/**
 * Implements da_vinci_css_alter().
 */
function da_vinci_css_alter(&$css) {
  $data = array();

  if (!$cache = cache_get('da_vinci::excludes:css')) {
    // Get the da_vinci css to exclude and set in cache:
    $css_skip = (array) theme_get_setting('css_exclude', 'da_vinci');
    foreach ($css_skip as $value) {
      $data[$value] = $value;
    }
    cache_set('da_vinci::excludes:css', $data, 'cache', CACHE_TEMPORARY);
  }
  else {
    $data = $cache->data;
  }

  // Unset our skipped CSS:
  $css = array_diff_key($css, $data);

}

/**
 * Implements da_vinci_js_alter().
 */
function da_vinci_js_alter(&$js) {
  $data = array();

  if (!$cache = cache_get('da_vinci::excludes:js')) {
    // Get the da_vinci js to exclude and set in cache:
    $js_skip = (array) theme_get_setting('js_exclude', 'da_vinci');
    foreach ($js_skip as $value) {
      $data[$value] = $value;
    }
    cache_set('da_vinci::excludes:js', $data, 'cache', CACHE_TEMPORARY);
  }
  else {
    $data = $cache->data;
  }

  // Unset our skipped JS:
  $js = array_diff_key($js, $data);

}


/**
 * Insert themed breadcrumb page navigation at top of the node content.
 */
function da_vinci_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Use CSS to hide titile .element-invisible.
    // $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // comment below line to hide current page to breadcrumb.
    $breadcrumb[] = drupal_get_title();
    $output = '<nav class="breadcrumb">' . implode('<span class="breadcrumb_next"> » </span>', $breadcrumb) . '</nav>';
    return $output;
  }
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function da_vinci_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    if (arg(0) == 'user') {
      $variables['secondary'][0]['#link']['title'] = t('My account');
      $variables['secondary']['#prefix'] .= '<ul class="nav nav-tabs">';
    }
    else {
      $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    }
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}

/**
 * Insert class menu and structure.
 */
function da_vinci_menu_tree(&$variables) {
  return '<ul class="menu">' . $variables['tree'] . '</ul>';
}

/**
 * Insert viewport.
 */
function da_vinci_page_alter($page) {
  /* <meta name="viewport" content="width=device-width,
   * initial-scale=1, maximum-scale=1"/>
   */
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, user-scalable=no',
    ),
  );
  drupal_add_html_head($viewport, 'viewport');
}

/**
 * Implements hook_theme().
 */
function da_vinci_theme() {
  $themes = array();

  // Theme wrapper which wrapper an element with a span tag.
  $themes['span_container'] = array(
    'render element' => 'element',
  );

  return $themes;
}

/**
 * Implements theme_THEME().
 */
function da_vinci_span_container(&$variables) {
  $element = $variables['element'];

  $output = "<span class='{$variables['classes']}'>";
  $output .= $element['#children'];
  $output .= '</span>';

  return $output;
}

// Preprocess.
require_once "preprocess/template.preprocess.html.php";
require_once "preprocess/template.preprocess.page.php";
require_once "preprocess/template.preprocess.node.php";
require_once "preprocess/template.preprocess.view.php";
require_once "preprocess/template.preprocess.block.php";
require_once "preprocess/template.preprocess.comment.php";
require_once "preprocess/template.preprocess.region.php";
require_once "preprocess/template.preprocess.user-profile.php";

// Process.
require_once "process/template.process.php";
