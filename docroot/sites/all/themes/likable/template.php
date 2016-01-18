<?php
/**
 * @file
 * Overriding theme functions.
 */

/**
 * Implements hook_preprocess_maintenance_page().
 */
function likable_clean_theme_preprocess_maintenance_page(&$variables) {
  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
  drupal_add_css(drupal_get_path('theme', 'likable_clean_theme') . '/css/maintenance-page.css');
}

/**
 * Override or insert variables into the maintenance page template.
 */
function likable_clean_theme_process_maintenance_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty,
    // so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
}

/**
 * Insert themed search block.
 */
function likable_clean_theme_preprocess_search_block_form(&$vars) {
  $vars['form']['search_block_form']['#value'] = t('Search this site...');
  $vars['form']['search_block_form']['#attributes'] = array(
    'onblur' => "if (this.value == '') {this.value = '" . $vars['form']['search_block_form']['#value'] . "';} ;",
    'onfocus' => "if (this.value == '" . $vars['form']['search_block_form']['#value'] . "') {this.value = '';} ;",
  );
  unset($vars['form']['search_block_form']['#printed']);
  $vars['search']['search_block_form'] = drupal_render($vars['form']['search_block_form']);
  $vars['search_form'] = implode($vars['search']);
}

/**
 * Insert themed breadcrumb page navigation at top of the node content.
 */
function likable_clean_theme_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $breadcrumb[] = drupal_get_title();
    $output .= implode(' » ', $breadcrumb);
    return $output;
  }
}

/**
 * Override or insert variables into the page template.
 */
function likable_clean_theme_preprocess_page(&$vars) {
  if (isset($vars['main_menu'])) {
    $vars['main_menu'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'main-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }
  else {
    $vars['main_menu'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'secondary-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }
  else {
    $vars['secondary_menu'] = FALSE;
  }
}
