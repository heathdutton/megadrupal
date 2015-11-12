<?php
/**
 * @file includes/go.hooks.inc
 */

require_once dirname(__FILE__) . '/go.hooks.form.inc';

/**
 * Implements hook_init().
 */
function go_init() {
  go_google_analytics_code_insert();
  go_node_to_frontpage();
}

/**
 * Implements of hook_element_info_alter().
 */
function go_element_info_alter(&$type) {
  global $conf;

  if (!empty($conf['go_text_formats'])) {
    // Our process callback must run immediately after filter_process_format().
    $filter_process_format_location = array_search('filter_process_format', $type['text_format']['#process']);
    $replacement = array('filter_process_format', 'go_filter_process_format');
    array_splice($type['text_format']['#process'], $filter_process_format_location, 1, $replacement);
  }
}

/**
 * Implements hook_go_api()
 */
function go_go_api() {
  return array('api' => '1.0');
}

/**
 * Implements hook_preprocess_page().
 * Simple 403/404 handlers
 */
function go_preprocess_page() {
  unset($_GET['destination']);
  go_preprocess_page__handle_40x();
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Remove the current password field from the user_profile_form form (user/%/edit).
 */
function nocurrent_pass_form_user_profile_form_alter(&$form, &$form_state) {
  if (defined('GO_NO_CURRENT_PASSWORD')) {
    // searches the #validate array for the current_pass validation function, and removes it
    $key = array_search('user_validate_current_pass', $form['#validate']);
    if ($key !== FALSE) {
      unset($form['#validate'][$key]);
    }
    // hide the current password fields
    $form['account']['current_pass_required_value']['#access'] = FALSE;
    $form['account']['current_pass']['#access'] = FALSE;
  }
}

/**
 * Implements hook_view_pre_render().
 */
function go_views_pre_render(&$view) {
  $view_id = "{$view->name}__{$view->current_display}";
  $slideshow_id = "go_slideshow__views__{$view_id}";
  $slideshow_config = variable_get($slideshow_id);
  if (!is_null($slideshow_config)) {
    module_load_include('inc', 'go', 'includes/go.slideshow');
    go_views_pre_render__slideshow($view->name, $view->current_display, $slideshow_config);
  }
}
