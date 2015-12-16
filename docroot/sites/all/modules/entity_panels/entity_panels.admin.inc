<?php
/**
 * @file
 * Admin functions.
 */

/**
 * Entity panels settings form.
 */
function entity_panels_settings_form($form, &$form_state) {
  $entity_options = array();

  foreach (entity_get_info() as $entity_type => $info) {
    if ($info['fieldable']) {
      $entity_options[$entity_type] = $info['label'];
    }
  }

  $form['entity_panels_entity_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Panelized entity types'),
    '#options' => $entity_options,
    '#default_value' => array_filter(variable_get('entity_panels_entity_types', array('node'))),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'entity_panels_rebuild_enabled';

  return $form;
}

/**
 * Page callback for the page listing "Panelized entities".
 */
function entity_panels_list_page() {
  module_load_include('inc', 'page_manager', 'page_manager.admin');

  $tasks = page_manager_get_tasks_by_type('entity_panels');
  $pages = array('operations' => array(), 'tasks' => array());

  entity_panels_get_pages($tasks, $pages);

  if (empty($pages['rows'])) {
    return array('#markup' => t('Acceptable entity types are not panelized.'));
  }

  global $user;

  ctools_include('object-cache');
  $locks = ctools_object_cache_test_objects('page_manager_page', $pages['tasks']);
  foreach ($locks as $task_name => $lock) {
    if ($lock->uid == $user->uid) {
      $pages['rows'][$task_name]['class'][] = ' page-manager-locked';
      $pages['rows'][$task_name]['name'] = t('This page is currently locked for editing by you. Nobody else may edit this page until these changes are saved or canceled.');
    }
    else {
      $pages['rows'][$task_name]['class'][] = ' page-manager-locked-other';
      $pages['rows'][$task_name]['name'] = t('This page is currently locked for editing by another user. You may not edit this page without breaking the lock.');
    }
  }

  $header = array(
    array('data' => t('Name'), 'class' => array('page-manager-page-name')),
    array('data' => t('Entity type'), 'class' => array('page-manager-page-entitytype')),
    array('data' => t('Bundle'), 'class' => array('page-manager-page-bundle')),
    array('data' => t('View mode'), 'class' => array('page-manager-page-viewmode')),
  );

  $header[] = array('data' => t('Operations'), 'class' => array('page-manager-page-operations'));
  $table = theme('table', array(
    'header' => $header,
    'rows' => $pages['rows'],
    'attributes' => array('id' => 'page-manager-list-pages'),
  ));

  $caption = '<h3>' . check_plain(t('Panelized entities')) . '</h3>';

  drupal_add_css(drupal_get_path('module', 'page_manager') . '/css/page-manager.css');

  return array('#markup' => $caption . $table);
}

/**
 * Collects table rows for "Panelized entities" table.
 */
function entity_panels_get_pages($tasks, &$pages, $task_id = NULL) {
  foreach ($tasks as $id => $task) {
    if (empty($task_id) && !empty($task['page operations'])) {
      $pages['operations'] = array_merge($pages['operations'], $task['page operations']);
    }

    if (!empty($task['subtasks'])) {
      entity_panels_get_pages(page_manager_get_task_subtasks($task), $pages, $task['name']);
      continue;
    }

    $task_name = $task['name'];
    $entity_type_label = '';
    $bundle_label = '';
    $view_mode_label = '';

    if (isset($task_id)) {
      $task_name = page_manager_make_task_name($task_id, $task['name']);
      list($entity_type, $bundle, $view_mode) = explode('-', $task['name'], 3);
      $info = entity_get_info($entity_type);
      $entity_type_label = isset($info['label']) ? $info['label'] : $entity_type;
      $bundle_label = isset($info['bundles'][$bundle]['label']) ? $info['bundles'][$bundle]['label'] : $bundle;
      $view_mode_label = isset($info['view modes'][$view_mode]['label']) ? $info['view modes'][$view_mode]['label'] : $view_mode;
    }

    $class = array('page-task-' . $id);
    if (isset($task['row class'])) {
      $class[] = $task['row class'];
    }

    if (!empty($task['disabled'])) {
      $class[] = 'page-manager-disabled';
    }

    $row = array('data' => array(), 'class' => $class, 'title' => strip_tags($task['admin description']));

    $type = isset($task['admin type']) ? $task['admin type'] : t('System');
    $pages['types'][$type] = $type;
    $row['data']['name'] = array('data' => $task_name, 'class' => array('page-manager-page-name'));

    $row['data']['entity_type'] = array('data' => $entity_type_label, 'class' => array('page-manager-page-entitytype'));
    $row['data']['bundle'] = array('data' => $bundle_label, 'class' => array('page-manager-page-bundle'));
    $row['data']['view_mode'] = array('data' => $view_mode_label, 'class' => array('page-manager-page-viewmode'));

    $operations = array(
      array(
        'title' => t('Edit'),
        'href' => page_manager_edit_url($task_name),
      ),
    );

    if (!empty($task['enable callback'])) {
      if (!empty($task['disabled'])) {
        array_unshift($operations, array(
          'title' => t('Enable'),
          'href' => 'admin/structure/pages/nojs/enable/' . $task_name,
          'query' => array('token' => drupal_get_token($task_name), 'destination' => 'admin/structure/pages/entity_panels'),
        ));
      }
      else {
        $operations[] = array(
          'title' => t('Disable'),
          'href' => 'admin/structure/pages/nojs/disable/' . $task_name,
          'query' => array('token' => drupal_get_token($task_name), 'destination' => 'admin/structure/pages/entity_panels'),
        );
      }
    }

    $ops = theme('links__ctools_dropbutton', array(
      'links' => $operations,
      'attributes' => array('class' => array('links', 'inline')),
    ));

    $row['data']['operations'] = array('data' => $ops, 'class' => array('page-manager-page-operations'));

    $pages['disabled'][$task_name] = !empty($task['disabled']);
    $pages['tasks'][] = $task_name;
    $pages['rows'][$task_name] = $row;
  }
}
