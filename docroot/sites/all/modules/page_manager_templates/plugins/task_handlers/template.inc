<?php

/**
 * @file
 *
 * This is the task handler plugin to allow the use of custom templates.
 */

// Plugin definition
$plugin = array(
  // is a 'context' handler type, meaning it supports the API of the
  // context handlers provided by ctools context plugins.
  'handler type' => 'context',
  'visible' => TRUE, // may be added up front.

  // Administrative fields.
  'title' => t('Template'),
  'admin summary' => 'page_manager_template_admin_summary',
  'admin title' => 'page_manager_template_title',
  'operations' => array(
    'settings' => array(
      'title' => t('General'),
      'description' => t('Change general settings for this variant.'),
      'form' => 'page_manager_template_edit_settings',
    ),
    'criteria' => array(
      'title' => t('Selection rules'),
      'description' => t('Control the criteria used to decide whether or not this variant is used.'),
      'ajax' => FALSE,
      'form' => array(
        'order' => array(
          'form' => t('Selection rules'),
        ),
        'forms' => array(
          'form' => array(
            'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
            'form id' => 'ctools_context_handler_edit_criteria',
          ),
        ),
      ),
    ),
    'context' => array(
      'title' => t('Contexts'),
      'ajax' => FALSE,
      'description' => t('Add additional context objects to this variant that can be used by the content.'),
      'form' => array(
        'order' => array(
          'form' => t('Context'),
        ),
        'forms' => array(
          'form' => array(
            'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
            'form id' => 'ctools_context_handler_edit_context',
          ),
        ),
      ),
    ),
  ),

  // Callback to render the data.
  'render' => 'page_manager_template_render',

  'add features' => array(
    'criteria' => t('Selection rules'),
    'context' => t('Contexts'),
  ),
  // Where to go when finished.
  'add finish' => 'settings',

  'required forms' => array(
    'settings' => t('Panel settings'),
  ),

  'edit forms' => array(
    'criteria' => t('Selection rules'),
    'settings' => t('General'),
    'context' => t('Contexts'),
  ),
  'forms' => array(
    'settings' => array(
      'form id' => 'page_manager_template_edit_settings',
    ),
    'context' => array(
      'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
      'form id' => 'ctools_context_handler_edit_context',
    ),
    'criteria' => array(
      'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
      'form id' => 'ctools_context_handler_edit_criteria',
    ),
  ),
  'default conf' => array(
    'title' => t('Template'),
    'contexts' => array(),
    'relationships' => array(),
    'module' => '',
    'theme' => '',
    'module_or_theme' => 'module',
    'template' => '',
    'template_path' => '',
    'hook_theme' => TRUE,
    'full_page' => FALSE,
  ),
);

function page_manager_template_admin_summary($handler, $task, $subtask, $page, $show_title = TRUE) {
  $task_name = page_manager_make_task_name($task['name'], $subtask['name']);
  $output = '';

  ctools_include('context');
  ctools_include('context-task-handler');

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
  $rows[] = array(
    array('class' => array('page-summary-label'), 'data' => t('Template')),
    array('class' => array('page-summary-data'), 'data' => $handler->conf['template']),
    array('class' => array('page-summary-operation'), 'data' => $link),
  );

  $rows[] = array(
    array('class' => array('page-summary-label'), 'data' => t('Preprocess function')),
    array('class' => array('page-summary-data'), 'data' => 'template_preprocess_' . $handler->conf['theme_function']),
    array('class' => array('page-summary-operation'), 'data' => t('You can use this preprocess function to prepare variables or add css or js for the page.')),
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
 * Set up a title for the panel based upon the selection rules.
 */
function page_manager_template_title($handler, $task, $subtask) {
  if (isset($handler->conf['title'])) {
    return check_plain($handler->conf['title']);
  }
  else {
    return t('Template');
  }
}

/**
 * General settings for the panel
 */
function page_manager_template_edit_settings($form, &$form_state) {
  $conf = $form_state['handler']->conf;
  $form['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
    '#title' => t('Administrative title'),
    '#description' => t('Administrative title of this variant.'),
  );

  $form['hook_theme'] = array(
    '#title' => t('Provide theme function'),
    '#type' => 'checkbox',
    '#description' => t('Page manager templates can create the hook_theme implementation to register your template file. Uncheck this if you will create your own hook_theme implementation for this theme function.'),
    '#default_value' => $conf['hook_theme'],
  );

  $form['template'] = array(
    '#title' => t('Template name'),
    '#type' => 'textfield',
    '#description' => t('The name of the template or theme function. If you will rely on Page Manager Templates for your hook_theme implementation, enter the name of the new template file excluding its the .tpl.php extension. If you are handling your own hook_theme implementation, enter the name of the theme function.'),
    '#default_value' => $conf['template'],
  );

  ctools_include('dependent');
  $form['module_or_theme'] = array(
    '#title' => t('Will a module or theme contain your template file?'),
    '#type' => 'select',
    '#options' => array('module' => 'module', 'theme' => 'theme'),
    '#default_value' => $conf['module_or_theme'],
    '#dependency' => array('edit-hook-theme' => array(TRUE)),
  );
  $result = db_query("SELECT name FROM {system} WHERE type = 'theme' ORDER BY name ASC");
  $themes = array();
  foreach ($result as $theme) {
    $themes[$theme->name] = $theme->name;
  }
  $form['theme'] = array(
    '#title' => t('Theme containing your template file'),
    '#type' => 'select',
    '#options' => $themes,
    '#default_value' => $conf['theme'],
    '#dependency' => array('edit-hook-theme' => array(TRUE), 'edit-module-or-theme' => array('theme')),
    '#dependency_count' => 2,
  );

  $form['module'] = array(
    '#title' => t('Module containing your template file'),
    '#type' => 'select',
    '#options' => module_list(),
    '#default_value' => $conf['module'],
    '#dependency' => array('edit-hook-theme' => array(TRUE), 'edit-module-or-theme' => array('module')),
    '#dependency_count' => 2,
  );

  $form['template_path'] = array(
    '#title' => t('Sub-directory'),
    '#type' => 'textfield',
    '#default_value' => $conf['template_path'],
    '#dependency' => array('edit-hook-theme' => array(TRUE)),
    '#description' => t('Optional path to the template within the chosen module or theme. e.g. templates'),
  );

  $form['full_page'] = array(
    '#title' => t('Override entire page'),
    '#type' => 'checkbox',
    '#description' => t('Rather than only controlling the content region of the page, you can opt to take over the entire HTML page output with your template.'),
    '#default_value' => $conf['full_page'],
  );

  $form['#submit'][] = 'drupal_theme_rebuild';

  return $form;
}

function page_manager_template_edit_settings_submit($form, &$form_state) {
  $form_state['handler']->conf['title'] = $form_state['values']['title'];
  $form_state['handler']->conf['module'] = $form_state['values']['module'];
  $form_state['handler']->conf['theme'] = $form_state['values']['theme'];
  $form_state['handler']->conf['module_or_theme'] = $form_state['values']['module_or_theme'];
  $form_state['handler']->conf['full_page'] = $form_state['values']['full_page'];

  $form_state['handler']->conf['template_path'] = $form_state['values']['template_path'];
  $form_state['handler']->conf['template'] = $form_state['values']['template'];
  $form_state['handler']->conf['hook_theme'] = $form_state['values']['hook_theme'];
  $form_state['handler']->conf['theme_function'] = $form_state['values']['template'];
  if ($form_state['values']['hook_theme']) {
    $form_state['handler']->conf['base'] = $form_state['values']['theme'];
    if ($form_state['values']['module_or_theme'] == 'module') {
      $form_state['handler']->conf['base'] = $form_state['values']['module'];
    }
    $form_state['handler']->conf['theme_function'] = $form_state['handler']->conf['base'] . '_template_' . str_replace("-", "_", $form_state['values']['template']);

    // Reset the page manager templates registry.
    cache_clear_all('page_manager_templates_registry', 'cache');
    _page_manager_templates_check_registry();
  }
}

function page_manager_template_render($handler, $base_contexts, $args, $test = TRUE) {
  ctools_include('context');
  ctools_include('context-task-handler');

  // Add my contexts
  $contexts = ctools_context_handler_get_handler_contexts($base_contexts, $handler);

  // Test.
  if ($test && !ctools_context_handler_select($handler, $contexts)) {
    return;
  }

  if (isset($handler->handler)) {
    ctools_context_handler_pre_render($handler, $contexts, $args);
  }

  if ($handler->conf['hook_theme']) {
    _page_manager_templates_check_registry();
  }

  $content = theme($handler->conf['theme_function'], $contexts);

  if ($handler->conf['full_page']) {
    print $content;
    return NULL;
  }

  $info = array(
    'content' => $content,
    'title' => check_plain($handler->conf['title']),
  );

  return $info;
}
