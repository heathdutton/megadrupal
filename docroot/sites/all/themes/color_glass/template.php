<?php
/**
 * @file
 * Theme template.php file.
 */

// Ensure that __DIR__ constant is defined.
if (!defined('__DIR__')) {
  define('__DIR__', dirname(__FILE__));
}

// Include all require files.
require_once __DIR__ . '/includes/helper.inc';
require_once __DIR__ . '/includes/bootstrap.inc';

drupal_static_reset('element_info');


/**
 * Implements hook_theme().
 *
 * Register theme hook implementations.
 *
 * The implementations declared by this hook have two purposes: either they
 * specify how a particular render array is to be rendered as HTML (this is
 * usually the case if the theme function is assigned to the render array's
 * #theme property), or they return the HTML that should be returned by an
 * invocation of theme().
 *
 * @see _bootstrap_theme()
 */
function color_glass_theme(&$existing, $type, $theme, $path) {
  return _bootstrap_theme($existing, $type, $theme, $path);
}

/**
 * Implements hook_css_alter().
 */
function color_glass_css_alter(&$css) {
  $theme_path = drupal_get_path('theme', 'color_glass');
  // Exclude specified CSS files from theme.
  $excludes = _bootstrap_get_theme_info(NULL, 'exclude][css');

  // Add Bootstrap Css.
  if ($bscdn_css = _bootstrap_setting('bscdn_css', "color_glass")) {
    $css[$bscdn_css] = array(
      'data' => $bscdn_css,
      'type' => 'external',
      'every_page' => TRUE,
      'media' => 'all',
      'preprocess' => FALSE,
      'group' => CSS_THEME,
      'browsers' => array('IE' => TRUE, '!IE' => TRUE),
      'weight' => -3.02,
    );
  }

  // Add fontawesome.
  if ($fontawesome = _bootstrap_setting('fontawesome', "color_glass")) {
    $css[$fontawesome] = array(
      'data' => $fontawesome,
      'type' => 'external',
      'every_page' => TRUE,
      'media' => 'all',
      'preprocess' => FALSE,
      'group' => CSS_THEME,
      'browsers' => array('IE' => TRUE, '!IE' => TRUE),
      'weight' => -3.01,
    );
  }
  // Add google font.
  if ($googlefont = _bootstrap_setting('googlefont', "color_glass")) {
    $css[$googlefont] = array(
      'data' => $googlefont,
      'type' => 'external',
      'every_page' => TRUE,
      'media' => 'all',
      'preprocess' => FALSE,
      'group' => CSS_THEME,
      'browsers' => array('IE' => TRUE, '!IE' => TRUE),
      'weight' => -3,
    );
  }

  // Add the overrides file.
  $override = $theme_path . '/css/overrides.css';
  $css[$override] = array(
    'data' => $override,
    'type' => 'file',
    'every_page' => TRUE,
    'media' => 'all',
    'preprocess' => TRUE,
    'group' => CSS_THEME,
    'browsers' => array('IE' => TRUE, '!IE' => TRUE),
    'weight' => -1,
  );

  // Add flatit.css
  if ($flatit = _bootstrap_setting('flatit', "color_glass")) {
    $flatit = $theme_path . '/css/flatit.css';
    $css[$flatit] = array(
      'data' => $flatit,
      'type' => 'file',
      'every_page' => TRUE,
      'media' => 'all',
      'preprocess' => TRUE,
      'group' => CSS_THEME,
      'browsers' => array('IE' => TRUE, '!IE' => TRUE),
      'weight' => -0.99,
    );
  }

  if (!empty($excludes)) {
    $css = array_diff_key($css, drupal_map_assoc($excludes));
  }
  // Replace the colored file.
  $colorcss = drupal_get_path('theme', 'color_glass') . '/css/color.css';
  $colleredcss = variable_get('color_color_glass_stylesheets', array());
  if (isset($css[$colorcss]) && !(empty($colleredcss))) {
    $uri = variable_get('color_color_glass_stylesheets', array());
    $wrapper = file_stream_wrapper_get_instance_by_uri($uri[0]);
    if ($wrapper) {
      $css[$colorcss]['data'] = $wrapper->getExternalUrl();
      $css[$colorcss]['type'] = 'external';
    }
  }
}

/**
 * Implements hook_js_alter().
 */
function color_glass_js_alter(&$js) {
  // Exclude specified JavaScript files from theme.
  $excludes = _bootstrap_get_theme_info(NULL, 'exclude][js');

  $theme_path = drupal_get_path('theme', 'color_glass');

  // Add or replace JavaScript files when matching paths are detected.
  // Replacement files must begin with '_', like '_node.js'.
  $files = _bootstrap_file_scan_directory($theme_path . '/js', '/\.js$/');
  foreach ($files as $file) {
    $path = str_replace($theme_path . '/js/', '', $file->uri);
    // Detect if this is a replacement file.
    $replace = FALSE;
    if (preg_match('/^[_]/', $file->filename)) {
      $replace = TRUE;
      $path = dirname($path) . '/' . preg_replace('/^[_]/', '', $file->filename);
    }
    $matches = array();
    if (preg_match('/^modules\/([^\/]*)/', $path, $matches)) {
      if (!module_exists($matches[1])) {
        continue;
      }
      else {
        $path = str_replace('modules/' . $matches[1], drupal_get_path('module', $matches[1]), $path);
      }
    }
    // Path should always exist to either add or replace JavaScript file.
    if (!empty($js[$path])) {
      // Replace file.
      if ($replace) {
        $js[$file->uri] = $js[$path];
        $js[$file->uri]['data'] = $file->uri;
        unset($js[$path]);
      }
      // Add file.
      else {
        $js[$file->uri] = drupal_js_defaults($file->uri);
        $js[$file->uri]['group'] = JS_THEME;
      }
    }
  }

  // Ensure jQuery Once is always loaded.
  // @see https://drupal.org/node/2149561
  if (empty($js['misc/jquery.once.js'])) {
    $jquery_once = drupal_get_library('system', 'jquery.once');
    $js['misc/jquery.once.js'] = $jquery_once['js']['misc/jquery.once.js'];
    $js['misc/jquery.once.js'] += drupal_js_defaults('misc/jquery.once.js');
  }

  // Always add bootstrap.js last.
  $bootstrap = $theme_path . '/js/bootstrap.js';
  $js[$bootstrap] = drupal_js_defaults($bootstrap);
  $js[$bootstrap]['group'] = JS_THEME;
  $js[$bootstrap]['scope'] = 'footer';

  if (!empty($excludes)) {
    $js = array_diff_key($js, drupal_map_assoc($excludes));
  }

  // Add Bootstrap settings.
  $js['settings']['data'][]['bootstrap'] = array(
    'anchorsFix' => _bootstrap_setting('anchors_fix'),
    'anchorsSmoothScrolling' => _bootstrap_setting('anchors_smooth_scrolling'),
    'formHasError' => (int) _bootstrap_setting('forms_has_error_value_toggle'),
    'popoverEnabled' => _bootstrap_setting('popover_enabled'),
    'popoverOptions' => array(
      'animation' => (int) _bootstrap_setting('popover_animation'),
      'html' => (int) _bootstrap_setting('popover_html'),
      'placement' => _bootstrap_setting('popover_placement'),
      'selector' => _bootstrap_setting('popover_selector'),
      'trigger' => implode(' ', array_filter(array_values((array) _bootstrap_setting('popover_trigger')))),
      'triggerAutoclose' => (int) _bootstrap_setting('popover_trigger_autoclose'),
      'title' => _bootstrap_setting('popover_title'),
      'content' => _bootstrap_setting('popover_content'),
      'delay' => (int) _bootstrap_setting('popover_delay'),
      'container' => _bootstrap_setting('popover_container'),
    ),
    'tooltipEnabled' => _bootstrap_setting('tooltip_enabled'),
    'tooltipOptions' => array(
      'animation' => (int) _bootstrap_setting('tooltip_animation'),
      'html' => (int) _bootstrap_setting('tooltip_html'),
      'placement' => _bootstrap_setting('tooltip_placement'),
      'selector' => _bootstrap_setting('tooltip_selector'),
      'trigger' => implode(' ', array_filter(array_values((array) _bootstrap_setting('tooltip_trigger')))),
      'delay' => (int) _bootstrap_setting('tooltip_delay'),
      'container' => _bootstrap_setting('tooltip_container'),
    ),
  );

  if ($bscdn_js = _bootstrap_setting('bscdn_js')) {
    $cdn_weight = -99.99;
    $js[$bscdn_js] = drupal_js_defaults($bscdn_js);
    $js[$bscdn_js]['type'] = 'external';
    $js[$bscdn_js]['every_page'] = TRUE;
    $js[$bscdn_js]['weight'] = $cdn_weight;
  }
}


/**
 * Implements hook_preprocess_page().
 */
function color_glass_preprocess_page(&$variables) {
  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-6"';
  }
  elseif (!empty($variables['page']['sidebar_first']) || !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-9"';
  }
  else {
    $variables['content_column_class'] = ' class="col-sm-12"';
  }

  if (_bootstrap_setting('fluid_container') === 1) {
    $variables['container_class'] = 'container-fluid';
  }
  else {
    $variables['container_class'] = 'container';
  }

  // Primary nav.
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Build links.
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    // Provide default theme wrapper function.
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  // Secondary nav.
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Build links.
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    // Provide default theme wrapper function.
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }

  $variables['navbar_classes_array'] = array('navbar');

  if (_bootstrap_setting('navbar_position') !== '') {
    $variables['navbar_classes_array'][] = 'navbar-' . _bootstrap_setting('navbar_position');
  }
  elseif (_bootstrap_setting('fluid_container') === 1) {
    $variables['navbar_classes_array'][] = 'container-fluid';
  }
  else {
    $variables['navbar_classes_array'][] = 'container';
  }
  if (_bootstrap_setting('navbar_inverse')) {
    $variables['navbar_classes_array'][] = 'navbar-inverse';
  }
  else {
    $variables['navbar_classes_array'][] = 'navbar-default';
  }
  // Add search box variable.
  $search_box = '';
  if (module_exists('search') && _bootstrap_setting('search_box')) {
    $search_form = drupal_get_form('search_form');
    $search_box = drupal_render($search_form);
    $variables['search_box'] = $search_box;
  }
  // Add Credits in the site footer.
  $variables['theme_credits'] = '';
  if (_bootstrap_setting('credits')) {
    $variables['theme_credits'] = t('Drupal theme created by <a href="@cmsbots" target = "blank" title = "Drupal theme generator" class = "clr-link-color">cmsbots.com</a> .', array(
      '@cmsbots' => 'http://cmsbots.com/drupal-theme-generator',
    ));
  }
}

/**
 * Implements hook_preprocess_html().
 */
function color_glass_preprocess_html(&$variables) {
  switch (_bootstrap_setting('navbar_position')) {
    case 'fixed-top':
      $variables['classes_array'][] = 'navbar-is-fixed-top';
      break;

    case 'fixed-bottom':
      $variables['classes_array'][] = 'navbar-is-fixed-bottom';
      break;

    case 'static-top':
      $variables['classes_array'][] = 'navbar-is-static-top';
      break;
  }
}

/**
 * Override or insert variables into the html template.
 */
function color_glass_process_html(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Implements hook_process_page().
 *
 * @see page.tpl.php
 */
function color_glass_process_page(&$variables) {
  $variables['navbar_classes'] = implode(' ', $variables['navbar_classes_array']);
}
