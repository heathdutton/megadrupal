<?php

/**
 * @file
 * Preprocessors and helper functions to make theming easier.
 */

/**
 * Implements template_preprocess().
 *
 * Adds Tabs, Filter form elements and Rules table to variables.
 */
function rules_filter_preprocess_rules_filter_rules_form(&$variables) {
  module_load_include('inc', 'rules_filter', 'includes/pages');

  // Adds Filter form elements to variables.
  $is_rules_page = ($variables['form']['#form_id'] == 'rules_admin_reaction_overview');
  $variables['rules_filter_form'] = _rules_filter_filter_form($is_rules_page);

  // Adds Tabs and Rules table to variables.
  $components = _rules_filter_rules_table_components($is_rules_page);
  $variables['rules_filter_tabs'] = $components['tabs'];
  $variables['rules_filter_table'] = $components['table'];
}
