<?php

/**
 * Set up a title for the panel based upon the selection rules.
 */
function brick_keystone_title($handler, $task, $subtask) {
  if (isset($handler->conf['title'])) {
    return check_plain($handler->conf['title']);
  }
  else {
    return t('Keystone');
  }
}

/**
 * Admin summary appears in a tab of page manager config for operation.
 */
function brick_keystone_admin_summary($handler, $task, $subtask, $page, $show_title = TRUE) {
  $task_name = page_manager_make_task_name($task['name'], $subtask['name']);
  $output = '';

  ctools_include('context');
  ctools_include('context-task-handler');
  ctools_include('content');

  // Get the operations
  $operations = page_manager_get_operations($page);

  // Get operations for just this handler.
  $operations = $operations['handlers']['children'][$handler->name]['children']['actions']['children'];
  $args = array('handlers', $handler->name, 'actions');
  $rendered_operations = page_manager_render_operations($page, $operations, array(), array('class' => array('actions')), 'actions', $args);

  $plugin = page_manager_get_task_handler($handler->handler);

  $object = ctools_context_handler_get_task_object($task, $subtask, $handler);
  $context = ctools_context_load_contexts($object, TRUE);

  $access = ctools_access_group_summary(!empty($handler->conf['access']) ? $handler->conf['access'] : array(), $context);
  if ($access) {
    $access = t('This panel will be selected if @conditions.', array('@conditions' => $access));
  }
  else {
    $access = t('This panel will always be selected.');
  }

  $rows = array();

  $type = $handler->type == t('Default') ? t('In code') : $handler->type;
  $rows[] = array(
    array('class' => array('page-summary-label'), 'data' => t('Storage')),
    array('class' => array('page-summary-data'), 'data' => $type),
    array('class' => array('page-summary-operation'), 'data' => ''),
  );

  if (!empty($handler->disabled)) {
    $link = l(t('Enable'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'actions', 'enable')));
    $text = t('Disabled');
  }
  else {
    $link = l(t('Disable'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'actions', 'disable')));
    $text = t('Enabled');
  }

  $rows[] = array(
    array('class' => array('page-summary-label'), 'data' => t('Status')),
    array('class' => array('page-summary-data'), 'data' => $text),
    array('class' => array('page-summary-operation'), 'data' => $link),
  );

  $link = l(t('Edit'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'criteria')));
  $rows[] = array(
    array('class' => array('page-summary-label'), 'data' => t('Selection rule')),
    array('class' => array('page-summary-data'), 'data' => $access),
    array('class' => array('page-summary-operation'), 'data' => $link),
  );

  $link = l(t('Edit'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'settings')));
  $title = ctools_content_admin_title($handler->conf['content_type'], $handler->conf['subtype'], $handler->conf['conf'], $context);
  $rows[] = array(
    array('class' => array('page-summary-label'), 'data' => t('Content type')),
    array('class' => array('page-summary-data'), 'data' => $title),
    array('class' => array('page-summary-operation'), 'data' => $link),
  );

  // TODO: Make a preview tab rather than shoving everything in the summary.
  $block = ctools_content_admin_info($handler->conf['content_type'], $handler->conf['subtype'], $handler->conf['conf'], $context);
  if (!$block->title) {
    $block->title = t('No title');
  }
  $admin_info = '<div class="pane-title">' . $block->title . '</div>';
  $admin_info .= '<div class="pane-content">' . filter_xss_admin(render($block->content)) . '</div>';

  $rows[] = array(
    array('class' => array('page-summary-label'), 'data' => t('Info')),
    array('class' => array('page-summary-data'), 'data' => $admin_info),
    array('class' => array('page-summary-operation'), 'data' => ''),
  );

  $info = theme('table', array('header' => array(), 'rows' => $rows, 'attributes' => array('class' => array('page-manager-handler-summary'))));

  $title = $handler->conf['title'];
  if ($title != t('Panel')) {
    $title = t('Panel: @title', array('@title' => $title));
  }

  $output .= '<div class="clearfix">';
  if ($show_title) {
  $output .= '<div class="handler-title clearfix">';
    $output .= '<div class="actions handler-actions">' . $rendered_operations['actions'] . '</div>';
    $output .= '<span class="title-label">' . $title . '</span>';
  }

  $output .= '</div>';
  $output .= $info;
  $output .= '</div>';

  return $output;
}

/**
 * Content settings for the panel
 */
function brick_keystone_edit_settings($form, &$form_state) {
  $conf = $form_state['handler']->conf;
  $form['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
    '#title' => t('Administrative title'),
    '#description' => t('Administrative title of this variant.'),
  );

  $form['no_blocks'] = array(
    '#type' => 'checkbox',
    '#default_value' => $conf['no_blocks'],
    '#title' => t('Disable Drupal blocks/regions'),
    '#description' => t('Check this to have the page disable all regions displayed in the theme. Note that some themes support this setting better than others. If in doubt, try with stock themes to see.'),
  );

  ctools_include('content');
  ctools_include('context');
  ctools_include('context-task-handler');

  $contexts = ctools_context_handler_get_all_contexts($form_state['task'], $form_state['subtask'], $form_state['handler']);

  $available_types = ctools_content_get_available_types($contexts);
  $options = array();
  foreach ($available_types as $plugin => $subtypes) {
    foreach ($subtypes as $subtype_id => $subtype) {

      // For some reason, the category might be an array. This happens with
      // views content panes.
      if (is_array($subtype['category'])) {
        $options[array_shift($subtype['category'])][$plugin .'-'. $subtype_id] = $subtype['title'];
      } else {
        $options[$subtype['category']][$plugin .'-'. $subtype_id] = $subtype['title'];
      }
    }
  }

  $form['content_type'] = array(
    '#type' => 'select',
    '#title' => 'Content type',
    '#default_value' => $conf['content_type'] .'-'. $conf['subtype'],
    '#options' => $options,
  );

  return $form;
}

/**
 * Submit handler writes content type and subtype to handler configuration.
 */
function brick_keystone_edit_settings_submit($form, &$form_state) {
  $form_state['handler']->conf['title'] = $form_state['values']['title'];
  $form_state['handler']->conf['no_blocks'] = $form_state['values']['no_blocks'];

  list($content_type, $subtype_name) = explode('-', $form_state['values']['content_type'], 2);

  $plugin = ctools_get_content_type($content_type);
  $subtype = ctools_content_get_subtype($plugin, $subtype_name);

  // When handler conf does not match form values, the content type has changed
  // and new defaults must be set for the content type configuration.
  if ($form_state['handler']->conf['content_type'] != $content_type && $form_state['handler']->conf['subtype'] != $subtype_name) {
    $form_state['handler']->conf['conf'] = ctools_content_get_defaults($plugin, $subtype);
  }

  $form_state['handler']->conf['content_type'] = $content_type;
  $form_state['handler']->conf['subtype'] = $subtype_name;
}

/**
 * Add ctools content config form state values to page manager settings.
 *
 * Content type config forms expect form state to be populated with plugins
 * and contexts.
 *
 * @see brick_ui::edit_execute_form().
 * @see brick_page_manager_variant_operations_alter().
 */
function brick_keystone_content_operation_wrapper($form, &$form_state) {
  ctools_include('content');
  ctools_include('context-task-handler');

  $handler = $form_state['handler'];

  // TODO: Some plugins won't work if op is only ever 'edit.'
  $form_state['op'] = 'edit';
  $form_state['plugin'] = ctools_get_content_type($handler->conf['content_type']);
  $form_state['subtype'] = ctools_content_get_subtype($handler->conf['content_type'], $handler->conf['subtype']);
  $form_state['subtype_name'] = $handler->conf['subtype'];
  $form_state['conf'] = $handler->conf['conf'];
  $form_state['contexts'] = ctools_context_handler_get_all_contexts($form_state['task'], $form_state['subtask'], $form_state['handler']);

  $form = ctools_content_configure_form_defaults($form, $form_state);
  $form['#submit'][] = 'brick_keystone_content_operation_submit';

  return $form;
}

/**
 * Copy configuration from form values to handler configuration.
 *
 * The valid configuration is copied to form state 'conf' by the content type
 * config form's submit handler.
 */
function brick_keystone_content_operation_submit(&$form, &$form_state) {
  // Copies user's form submissions to handler's stored configuration.
  $form_state['handler']->conf['conf'] = $form_state['conf'];
}

/**
 * Implement hook_page_manager_variant_operations_alter().
 *
 * This hook is only used when editing a task this task handler is attached to.
 * The keystone plugin's forms and operations include this file.
 *
 * @see brick_ui::edit_execute_form().
 * @see brick_keystone_content_operation_wrapper().
 */
function brick_page_manager_variant_operations_alter(&$operation, $handler) {
  if ($handler->handler == 'keystone') {

    ctools_include('context');
    ctools_include('content');

    // Figure out a way to support "Add."
    $op = 'edit';
    $content_form_info = array();
    $plugin = ctools_get_content_type($handler->conf['content_type']);
    $subtype = ctools_content_get_subtype($handler->conf['content_type'], $handler->conf['subtype']);
    $subtype_name = $handler->conf['subtype'];
    $conf = $handler->conf['conf'];

    // Use the edit form.
    if (!empty($subtype['edit form'])) {
      _ctools_content_create_form_info($content_form_info, $subtype['edit form'], $subtype, $subtype, $op);
    }
    else if (!empty($plugin['edit form'])) {
      _ctools_content_create_form_info($content_form_info, $plugin['edit form'], $plugin, $subtype, $op);
    }

    if (!empty($content_form_info['forms'])) {
      foreach ($content_form_info['forms'] as &$info) {
        $info['include'] = drupal_get_path('module', 'brick') . '/plugins/task_handlers/keystone.admin.inc';
        $info['wrapper'] = 'brick_keystone_content_operation_wrapper';
      }
      $operation['children']['settings']['form'] = array_merge_recursive($operation['children']['settings']['form'], $content_form_info);
      $operation['children']['settings']['form info']['show trail'] = TRUE;
    }
  }
}
