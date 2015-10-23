<?php

/**
 * @file
 * Admin hooks and functions for the Project Source module.
 */


/**
 * Admin form for the Project Source module.
 */
function project_src_settings($form_state) {
  $form = array();

  // Get all projects and reduce all API versions to a single list of projects.
  $all_projects = project_src_get_projects(NULL, NULL, TRUE);
  $projects = array();
  foreach ($all_projects as $api_version) {
    $projects += $api_version;
  }

  if (!empty($projects)) {
    $delta = 0;
    $settings = project_src_get_project_settings();
    ksort($projects);

    $form['settings_table'] = array(
      '#theme' => 'project_src_settings_table',
    );

    foreach ($projects as $name => $project) {
      $project_settings = isset($settings[$name]) ? $settings[$name] : array();
      $form['settings_table'][++$delta] = project_src_settings_row($project, $project_settings);
    }

    // Add a submit handler to save our master variable.
    $form['#submit'][] = 'project_src_settings_table_submit';
  }
  else {
    $link = 'https://drupal.org/project/project_src';
    $empty = t('No projects were found! Please install and configure a !integration_module to get started.', array(
      '!integration_module' => l(t('Project Source integration module'), $link),
    ));
    $form['empty'] = array(
      '#type' => 'html_tag',
      '#tag' => 'p',
      '#value' => $empty,
    );
  }

  return system_settings_form($form);
}

/**
 * Submit handler for the project table configuration.
 */
function project_src_settings_table_submit($form, &$form_state) {
  project_src_set_project_settings(NULL, NULL, $form_state['input']['project_src_project_settings']);
}


/**
 * Builds a project settings row of form elements.
 *
 * @param array $project
 *   The definition of the given project.
 * @param array $settings
 *   The existing settings for the given project.
 *
 * @return array
 *   A form API element renderable by our settings table theme function.
 */
function project_src_settings_row($project, $settings) {
  // This isn't a setting; just a way to indicate the project.
  $form['project_title'] = array(
    '#type' => 'html_tag',
    '#tag' => 'strong',
    '#value' => check_plain($project['title']),
  );

  // Load project settings, sort them, then iterate through and build the form.
  foreach (project_src_get_project_settings_definitions('project') as $id => $info) {
    // This is a table, so we don't want to display a title or description.
    unset($info['element']['#title'], $info['element']['#description']);

    // Allow other modules to alter this setting based on project specifics.
    $context = array('project' => $project, 'settings' => $settings);
    drupal_alter('project_src_settings', $info, $context, $id);

    // Set a form element keyed by the ID with the provided config element.
    $form[$id] = $info['element'];

    // Rename the setting config's form name so that it's properly saved.
    $form[$id]['#name'] = 'project_src_project_settings[' . $project['short_name'] . '][' . $id . ']';

    // If the setting config provided a default, be sure to use it.
    $default = isset($info['element']['#default_value']) ? $info['element']['#default_value'] : NULL;
    $form[$id]['#default_value'] = isset($settings[$id]) ? $settings[$id] : $default;
  }

  // Iterate through all API versions, rendering API version scope settings.
  $collapsible_lib = drupal_get_library('system', 'drupal.collapse');
  foreach (project_src_get_projects(NULL, NULL, TRUE) as $api => $list) {
    if (isset($list[$project['short_name']])) {
      // Supply proper project when altering runtime settings definitions.
      $project = $list[$project['short_name']];

      // Add a fieldset to house all API version-scoped settings.
      $form[$api] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($api),
        '#attributes' => array('class' => array('collapsible', 'collapsed')),
        '#attached' => array(
          'library' => $collapsible_lib['dependencies'],
          'js' => $collapsible_lib['js'],
          'css' => $collapsible_lib['css'],
        ),
      );

      // Iterate through API version-scoped settings and add them to the form.
      foreach (project_src_get_project_settings_definitions('api_version') as $id => $info) {
        $context = array('project' => $project, 'settings' => $settings);
        drupal_alter('project_src_settings', $info, $context, $id);
        $form[$api][$id] = $info['element'];
        $form[$api][$id]['#name'] = 'project_src_project_settings[' . $project['short_name'] . '][' . $api . '][' . $id . ']';
        $default = isset($info['element']['#default_value']) ? $info['element']['#default_value'] : NULL;
        $form[$api][$id]['#default_value'] = isset($settings[$api][$id]) ? $settings[$api][$id] : $default;
      }
    }
    else {
      $form[$api] = array();
    }
  }

  return $form;
}


/**
 * Themes Project Source custom settings tables.
 */
function theme_project_src_settings_table($variables) {
  $form = $variables['form'];
  $project_settings = project_src_get_project_settings_definitions('project');
  $projects = project_src_get_projects(NULL, NULL, TRUE);
  ksort($projects);

  // Build out the table headers by iterating through settings elements.
  $headers = array(t('Project'));
  foreach ($project_settings as $info) {
    $headers[] = isset($info['element']['#title']) ? $info['element']['#title'] : '';
  }
  foreach ($projects as $api_version => $list) {
    $headers[] = $api_version;
  }

  // Iterate through all table rows.
  foreach (element_children($form) as $key) {
    $rows[$key]['data'] = array();

    // Always render the project_title element first.
    $rows[$key]['data'][] = drupal_render($form[$key]['project_title']);

    // Iterate through all project settings and render them.
    foreach ($project_settings as $id => $info) {
      $rows[$key]['data'][] = drupal_render($form[$key][$id]);
    }

    // Iterate through all API version settings and render them.
    foreach ($projects as $api => $info) {
      $rows[$key]['data'][] = drupal_render($form[$key][$api]);
    }
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}


/**
 * Loads Project Source project settings definitions.
 *
 * @param string $scope
 *   (Optional) The project setting scope for which settings definitions are
 *   desired. Valid scopes include...
 *   - project: Settings that apply to single projects globally.
 *   - api_version: Settings that apply to individual API versions of projects.
 *
 * @return array
 *   An array of Project Source setting definitions keyed by setting ID and
 *   optionally filtered by setting scope.
 *
 * @see hook_project_src_settings()
 */
function project_src_get_project_settings_definitions($scope = NULL) {
  $settings_info = module_invoke_all('project_src_settings');
  uasort($settings_info, 'element_sort');

  // Filter down to just a given scope, if provided with a scope.
  if ($scope) {
    $filter = 'project_src_filter_settings_scope_' . $scope;
    if (function_exists($filter)) {
      $settings_info = array_filter($settings_info, $filter);
    }
  }

  return $settings_info;
}


/**
 * Filters project source settings definitions down to project scope.
 */
function project_src_filter_settings_scope_project($setting) {
  return isset($setting['scope']) && $setting['scope'] === 'project';
}

/**
 * Filters project source settings definitions down to API version scope.
 */
function project_src_filter_settings_scope_api_version($setting) {
  return isset($setting['scope']) && $setting['scope'] === 'api_version';
}
