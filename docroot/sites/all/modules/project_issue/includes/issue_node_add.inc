<?php
/**
 * @file
 * Code required for the project_issue node form.
 */

/**
 * Project issue node add page callback.
 *
 * This is a modified version of node_add(), with the addition of injecting
 * the project field based on the URL.
 */
function project_issue_node_add($type, $project_entity) {
  global $user;

  // We need to ensure the user has permission to view the parent project, or
  // we'll disclose the project title (and the entityreference validation will
  // fail when they eventually try to submit the issue).
  if (!node_access('view', $project_entity)) {
    return drupal_access_denied();
  }

  $field = field_info_field('field_project');
  list($target_id) = entity_extract_ids($field['settings']['target_type'], $project_entity);

  $types = node_type_get_types();
  $node = (object) array(
    'uid' => $user->uid,
    'name' => (isset($user->name) ? $user->name : ''),
    'type' => $type,
    'language' => LANGUAGE_NONE,
  );
  $node->field_project[LANGUAGE_NONE][0]['target_id'] = $target_id;
  drupal_set_title(t('Create @name', array('@name' => $types[$type]->name)), PASS_THROUGH);

  // Since this callback does not live in node.pages.inc, we need to
  // define an include dependency manually, so ajax requests don't break.
  // This is a modified version of drupal_get_form().
  $form_state = array();
  $form_state['build_info']['args'][] = $node;
  form_load_include($form_state, 'inc', 'node', 'node.pages');

  return drupal_build_form($type . '_node_form', $form_state);
}

/**
 * A simple form to select a project when creating a new issue.
 *
 * This form will allow the user to select a project from a pull-down select
 * options, so the user can specify the project that the project_issue belongs
 * to.  This interface will only be suitable for sites with a small number of
 * projects.
 */
function project_issue_pick_project_form($form, &$form_state, $node_type) {
  $form = array();

  drupal_set_title(t('Submit @name', array('@name' => node_type_get_name($node_type))));

  $form_state['node_type'] = $node_type;

  if (variable_get('project_selection_widget', 'select') == 'select') {
    // Fetch a list of all projects.
    $machine_names = array();
    $projects = project_projects_select_options($machine_names);
    if (empty($projects)) {
      drupal_set_message(t('You do not have access to any projects.'), 'error');
    }

    $form['project'] = array(
      '#type' => 'select',
      '#title' => t('Project'),
      '#empty_option' => t('- Select a project -'),
      '#options' => $projects,
      '#required' => TRUE,
    );
  }
  else {
    // Set up an autocomplete instead.
    $form['project'] = array(
      '#title' => t('Project'),
      '#type' => 'textfield',
      '#size' => 30,
      '#required' => TRUE,
      '#autocomplete_path' => 'project/autocomplete/issue/project',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
  return $form;
}

function project_issue_pick_project_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['project'])) {
    // project_load() handles resolving both machine name and nid.
    $node = project_load($form_state['values']['project']);
    if (empty($node)) {
      // Try loading by title.
      $query = new EntityFieldQuery();
      $query
        ->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', project_project_node_types())
        ->fieldCondition('field_project_has_issue_queue', 'value', 1)
        ->propertyCondition('title', '%' . db_like($form_state['values']['project']) . '%', 'like');

      $result = $query->execute();
      if (!empty($result['node'])) {
        $nids = array_keys($result['node']);
        if (count($nids) != 1) {
          form_set_error('project', t('Multiple matches found for %title.', array('%title' => $form_state['values']['project'])));
        }
        else {
          $node = project_load($nids[0]);
        }
      }
    }
    if (empty($node)) {
      form_set_error('project', t('Invalid project selected.'));
    }
    else {
      $form_state['project_machine_name'] = $node->field_project_machine_name[LANGUAGE_NONE][0]['value'];
    }
  }
}

function project_issue_pick_project_form_submit($form, &$form_state) {
  $type_url_str = str_replace('_', '-', $form_state['node_type']);
  $form_state['redirect'] = 'node/add/' . $type_url_str . '/' . $form_state['project_machine_name'];
}
