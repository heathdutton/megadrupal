<?php

/**
 * @file
 *
 * This is the task handler plugin to render ctools content as page content.
 */

// Plugin definition
$plugin = array(
  // is a 'context' handler type, meaning it supports the API of the
  // context handlers provided by ctools context plugins.
  'handler type' => 'context',
  'visible' => TRUE, // may be added up front.

  // Administrative fields.
  'title' => t('Keystone'),
  'admin summary' => array(
    'file' => 'keystone.admin.inc',
    'path' => drupal_get_path('module', 'brick') .'/plugins/task_handlers',
    'function' => 'brick_keystone_admin_summary',
  ),
  'admin title' => array(
    'file' => 'keystone.admin.inc',
    'path' => drupal_get_path('module', 'brick') .'/plugins/task_handlers',
    'function' => 'brick_keystone_title',
  ),
  'tab operation' => 'brick_keystone_tab_operation',
  'operations' => array(
    'settings' => array(
      'title' => t('Content'),
      'description' => t('Change content settings for this variant.'),
      'form' => array(
        'order' => array(
          'content_type' => t('Content'),
        ),

        // The 'content_type' operation in the page manager task admin is still
        // very tricky. It makes a multistep form inside the operation tab, and
        // the steps in the form may change based on settings. The form must
        // have a distinctive key becuase the content type plugin's
        // configuration form is often keyed 'form'.
        'forms' => array(
          'content_type' => array(
            'include' => drupal_get_path('module', 'brick') . '/plugins/task_handlers/keystone.admin.inc',
            'form id' => 'brick_keystone_edit_settings',
          ),
        ),
      ),
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
  'render' => 'brick_keystone_render',

  'add features' => array(
    'criteria' => t('Selection rules'),
    'context' => t('Contexts'),
  ),

  // Where to go when finished.
  'add finish' => 'settings',

  'required forms' => array(
    'settings' => t('Content'),
  ),

  'edit forms' => array(
    'criteria' => t('Selection rules'),
    'settings' => t('Content'),
    'context' => t('Contexts'),
  ),
  'forms' => array(
    'settings' => array(
      'include' => drupal_get_path('module', 'brick') . '/plugins/task_handlers/keystone.admin.inc',
      'form id' => 'brick_keystone_edit_settings',
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
    'title' => t('Keystone'),
    'no_blocks' => FALSE,
    'contexts' => array(),
    'relationships' => array(),
    'content_type' => 'custom',
    'subtype' => 'custom',

    // The conf must be populated with defaults from the default content type.
    'conf' => array(
      'admin_title' => '',
      'title' => '',
      'body' => '',
      'format' => filter_fallback_format(),
      'substitute' => TRUE,
    ),
  ),
);

/**
 * Provide the operation trail for the 'Edit panel' link.
 *
 * When editing the panel, go directly to the content tab.
 */
function brick_keystone_tab_operation($handler, $contexts, $args) {
  return array('handlers', $handler->name, 'settings');
}

/**
 * Brick keystone task handler render function.
 */
function brick_keystone_render($handler, $base_contexts, $args, $test = TRUE) {

  ctools_include('content');
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

  // TODO: Handle page title consistently. Probably this switcheroo is
  // superfluous.
  $page_title = drupal_get_title();
  $conf = $handler->conf;
  $output = ctools_content_render($conf['content_type'], $conf['subtype'], $conf['conf'], array(), array(), $contexts);
  drupal_set_title($page_title, PASS_THROUGH);

  if (isset($output->content)) {
    return array(
      'content' => $output->content,
      'title' => $output->title,
      'no_blocks' => $handler->conf['no_blocks'],
    );
  }
  else {
    // Returning FALSE in a predefined task causes the original hook_menu page
    // callback to run. However, in the case of custom page tasks, there is no
    // original hook_menu page callback to run. Hmm...
    return FALSE;
  }
}
