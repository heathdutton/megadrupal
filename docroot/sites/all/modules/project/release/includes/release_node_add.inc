<?php

/**
 * @file
 * Code related to node/add/project-release and friends.
 */

/**
 * Form builder for a simple form to select a project when creating a new
 * release (as the first "page", but this is not really a multi-page form).
 */
function project_release_pick_project_form($form, &$form_state, $node_type) {
  $form_state['node_type'] = $node_type;

  if (variable_get('project_selection_widget', 'select') === 'select') {
    // Fetch a list of all projects with releases.
    $projects = project_projects_select_options(array(array('field' => 'field_project_has_releases')));
    if (count($projects) === 0) {
      drupal_set_message(t('You do not have access to any projects.'), 'error');
    }
    $form['project_id'] = array(
      '#type' => 'select',
      '#title' => t('Project'),
      '#options' => $projects,
      '#required' => TRUE,
    );
  }
  else {
    $form['project_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Project'),
      '#required' => TRUE,
      '#autocomplete_path' => 'project/autocomplete/release/project',
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
  return $form;
}

function project_release_pick_project_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['project_id'])) {
    if (variable_get('project_selection_widget', 'select') === 'select') {
      $node = node_load($form_state['values']['project_id']);
    }
    else {
      $query = new EntityFieldQuery;
      $result = $query
        ->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', project_project_node_types())
        ->fieldCondition('field_project_has_releases', 'value', 1)
        ->propertyCondition('title', $form_state['values']['project_id'])
        ->range(0, 1)
        ->addTag('node_access')
        ->execute();
      if (isset($result['node'])) {
        $nids = array_keys($result['node']);
        $node = node_load(reset($nids));
      }
    }
    if (empty($node) || !project_node_is_project($node)) {
      form_set_error('project_id', t('Invalid project selected.'));
    }
    else {
      $form_state['project_machine_name'] = $node->field_project_machine_name[LANGUAGE_NONE][0]['value'];
    }
  }
}

function project_release_pick_project_form_submit($form, &$form_state) {
  $type_url_str = str_replace('_', '-', $form_state['node_type']);
  $form_state['redirect'] = 'node/add/' . $type_url_str . '/' . $form_state['project_machine_name'];
}

function project_release_autocomplete_project($string) {
  $matches = array();

  $query = new EntityFieldQuery;
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', project_project_node_types())
    ->fieldCondition('field_project_has_releases', 'value', 1)
    ->propertyCondition('title', db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->addTag('node_access')
    ->execute();

  $projects = isset($result['node']) ? $result['node'] : array();
  $projects = node_load_multiple(array_keys($projects));
  foreach ($projects as $project) {
    $title = $project->title;
    // Commas and quotes in terms are special cases, so encode 'em.
    if (strpos($title, ',') !== FALSE || strpos($title, '"') !== FALSE) {
      $title = '"'. str_replace('"', '""', $project->title) .'"';
    }
    $matches[$title] = check_plain($project->title);
  }

  drupal_json_output($matches);
}
