<?php
/**
 * @file
 * Theme override functions.
 */
/**
 * Add IE conditional stylesheets
 */
function waterloo_preprocess_html(&$variables) {
  drupal_add_css(
    path_to_theme() . '/css/ie7.css',
    array(
      'group' => CSS_THEME,
      'browsers' => array(
        'IE' => 'lte IE 7',
        '!IE' => FALSE,
      ),
      'every_page' => TRUE,
    )
  );
  drupal_add_css(
    path_to_theme() . '/css/ie8.css',
    array(
      'group' => CSS_THEME,
      'browsers' => array(
        'IE' => 'IE 8',
        '!IE' => FALSE,
      ),
      'every_page' => TRUE,
    )
  );
}

/**
 * Override or insert variables into the page template.
 */
function waterloo_process_page(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function waterloo_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

/**
 * Create new template for user login block
 */
function waterloo_theme(&$existing, $type, $theme, $path) {
  $hooks['user_login_block'] = array(
    'template' => 'templates/user-login-block',
    'render element' => 'form',
  );
  return $hooks;
}

/**
 * Process variables for user login block
 */
function waterloo_preprocess_user_login_block(&$vars) {
  $vars['name'] = render($vars['form']['name']);
  $vars['pass'] = render($vars['form']['pass']);
  $vars['submit'] = render($vars['form']['actions']['submit']);
  $vars['rendered'] = drupal_render_children($vars['form']);
}

/**
 * Change breadcrumb separator
 */
function waterloo_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb['breadcrumb'])) {
    return '<div id="breadcrumbs">' . implode(' : ', $breadcrumb['breadcrumb']) . '</div>';
  }
}
