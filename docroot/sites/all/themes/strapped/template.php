<?php

/**
 * @file
 * form-element.func.php
 */

// Include theme functions
include_once "theme/adaptive.inc";
include_once "theme/checkboxes.inc";
include_once "theme/container.inc";
include_once "theme/date.inc";
include_once "theme/errors.inc";
include_once "theme/form.inc";
include_once "theme/images.inc";
include_once "theme/link.inc";
include_once "theme/multiple.inc";
include_once "theme/menu.inc";
include_once "theme/radios.inc";
include_once "theme/fieldset.inc";
include_once "theme/process.inc";
include_once "theme/text.inc";
include_once "theme/views.inc";
include_once "theme/webform.inc";
include_once "theme/webform_grid.inc";
include_once "theme/webform_matrix_table.inc";
include_once "theme/theme.inc";
include_once "theme/wrappers.inc";
include_once "theme/captcha.inc";
include_once "theme/range.inc";
include_once "theme/media.inc";
include_once "theme/table.inc";
include_once "theme/forum.inc";

/**
 * Implements hook_ctools_plugin_directory().
 */
function strapped_ctools_plugin_directory($module, $plugin) {
  if ($module == "panels" && in_array($plugin, array('layouts', 'styles'))) {
    return "plugins/$plugin";
  }
}


/**
 * @param $form
 * @param $form_state
 * @param $form_id
 */
function strapped_form_alter(&$form, &$form_state, $form_id) {


  // Undo what the bootstrap theme did in alter.inc to remove the wrapper around actions.
  unset($form['actions']['#theme_wrappers']);


  _forum_form_alter($form, $form_state, $form_id);

  // Comment overview form
  if ($form_id == 'comment_admin_overview') {


  }
}


/**
 * @param $variables
 * @param $hook
 */
function strapped_preprocess_page(&$variables, $hook) {

  // Create a variable to indicate that the current page is a panelised page.
  $variables['is_panelized_page'] = $variables['is_panel_page'] = FALSE;

  // Test for taxonomy term pages.
  $term = menu_get_object('taxonomy_term', 2);
  if (is_object($term) && isset($term->panelizer)) {
    $variables['is_panelized_page'] = TRUE;
  }

  // Test for panelized pages.
  if (isset($variables['node']) && isset($variables['node']->panelizer)) {
    $variables['is_panelized_page'] = TRUE;
  }

  if (page_manager_get_current_page() || !empty($vars['node']) && $vars['node']->type == 'panelized_page') {
    $variables['is_panel_page'] = TRUE;
  }


}

/**
 * @file
 * views.vars.php
 */

/**
 * Implements hook_preprocess_views_view_table().
 */
function strapped_preprocess_views_view_table(&$vars) {
  $vars['classes_array'][] = 'table-bordered';
}

