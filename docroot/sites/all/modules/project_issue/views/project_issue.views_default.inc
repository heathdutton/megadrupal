<?php


/**
 * @file
 * Provides default views on behalf of the project_issue module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function project_issue_views_default_views() {
  // Search the "default_views" directory for files ending in .view.php.
  $files = file_scan_directory(drupal_get_path('module', 'project_issue'). '/views/default_views', '/view\.php$/');
  foreach ($files as $absolute => $file) {
    // This is not require_once because it's possible that
    // hook_views_default_views() gets called more than once.
    require $absolute;
    if (isset($view)) {
      // $file->name has the ".php" stripped off, but still has the ".view".
      $view_name = substr($file->name, 0, strrpos($file->name, '.'));
      $views[$view_name] = $view;
    }
  }
  return $views;
}

/**
 * Add exposed filters for every taxonomy vocabulary used for issues.
 *
 * This is a helper function used by a few of the default view definitions.
 */
function _project_issue_views_add_taxonomy_filters(&$handler) {
  if (!module_exists('taxonomy')) {
    return;
  }
  $issue_term_references = _project_issue_get_issue_term_reference_fields();
  foreach ($issue_term_references as $field_name => $info) {
    $vocab = taxonomy_vocabulary_machine_name_load($info['vocabulary']);
    if (!empty($vocab) && ($issue_queue_widget = variable_get('project_issue_taxonomy_vocabulary_issue_queue_' . $vocab->vid, 0))) {
      // Since core is inconsistent about the field names for migrated
      // taxonomy term references vs. newly created ones, we don't want to
      // try to use the field name itself as the filter ID. Instead, we use
      // the vocabulary machine name.
      $filter_id = $vocab->machine_name;
      $handler->display->display_options['filters'][$filter_id]['id'] = $filter_id;
      $handler->display->display_options['filters'][$filter_id]['table'] = 'field_data_' . $field_name;
      $handler->display->display_options['filters'][$filter_id]['field'] = $field_name . '_tid';
      $handler->display->display_options['filters'][$filter_id]['value'] = '';
      $handler->display->display_options['filters'][$filter_id]['exposed'] = TRUE;
      $handler->display->display_options['filters'][$filter_id]['expose']['operator_id'] = $filter_id . '_op';
      $handler->display->display_options['filters'][$filter_id]['expose']['label'] = check_plain($vocab->name);
      $handler->display->display_options['filters'][$filter_id]['expose']['use_operator'] = TRUE;
      $handler->display->display_options['filters'][$filter_id]['expose']['operator'] = $filter_id . '_op';
      $handler->display->display_options['filters'][$filter_id]['expose']['identifier'] = $filter_id;
      $handler->display->display_options['filters'][$filter_id]['expose']['remember_roles'] = array(
  2 => '2',
);
      $handler->display->display_options['filters'][$filter_id]['reduce_duplicates'] = TRUE;
      $handler->display->display_options['filters'][$filter_id]['vocabulary'] = $vocab->machine_name;
      if ($issue_queue_widget == 'select') {
        $handler->display->display_options['filters'][$filter_id]['type'] = 'select';
        $handler->display->display_options['filters'][$filter_id]['expose']['multiple'] = FALSE;
      }
      else {
        $handler->display->display_options['filters'][$filter_id]['expose']['multiple'] = TRUE;
      }
    }
  }
}
