<?php

/**
 * @file
 *
 * This is the task handler plugin to handle page redirecting based on context.
 */

// Plugin definition
$plugin = array(
  // is a 'context' handler type, meaning it supports the API of the
  // context handlers provided by ctools context plugins.
  'handler type' => 'context',
  'visible' => TRUE, // may be added up front.

  // Administrative fields.
  'title' => t('Redirect'),
  'admin summary' => 'page_manager_redirect_redirect_context_admin_summary',
  'admin title' => 'page_manager_redirect_redirect_context_title',
  'admin css' => array(
    page_manager_redirect_get_path('css/page_manager_redirect_admin.css'),
  ),
  'operations' => array(
    'settings' => array(
      'title' => t('Settings'),
      'description' => t('Change general settings about this variant.'),
      'form' => 'page_manager_redirect_redirect_context_edit_settings',
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
    'preview' => array(
      'title' => t('Preview'),
      'description' => t('Get a preview of what this variant will look like.'),
      'form' => 'page_manager_redirect_redirect_context_edit_preview',
      'ajax' => FALSE,
      'silent' => TRUE,
      'form info' => array('finish text' => t('Preview')),
    ),
  ),

  // Callback to render the data.
  'render' => 'page_manager_redirect_redirect_context_render',

  // Various callbacks for operations performed on the handler to ensure
  // related data is updated properly. We don't have any additional operations
  // to do (like panels does, with it's displays), except for removing legacy
  // displays on exporting.
  //'save' => 'page_manager_redirect_redirect_context_save',
  //'delete' => 'page_manager_redirect_redirect_context_delete',
  'export' => 'page_manager_redirect_redirect_context_export',
  //'clone' => 'page_manager_redirect_redirect_context_clone',

  'add features' => array(
    'criteria' => t('Selection rules'),
    'context' => t('Contexts'),
  ),
  // Where to go when finished.
  'add finish' => 'settings',

  'required forms' => array(
    'settings' => t('Configure settings'),
  ),

  'edit forms' => array(
    'criteria' => t('Selection rules'),
    'settings' => t('Settings'),
    'context' => t('Contexts'),
  ),
  'forms' => array(
    'settings' => array(
      'form id' => 'page_manager_redirect_redirect_context_edit_settings',
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
    'title' => t('Redirect'),
    'contexts' => array(),
    'relationships' => array(),
    'path' => '',
    'query' => '',
    'fragment' => '',
  ),
);

/**
 * Converts task handlers to a configuration array.
 */
function page_manager_redirect_redirect_context_get_redirection($handler) {
  return array(
    'path' => $handler->conf['path'],
    'query' => $handler->conf['query'],
    'fragment' => $handler->conf['fragment'],
  );
}

/**
 * Check selection rules and, if passed, render the contexts.
 *
 * We must first check to ensure the node is of a type we're allowed
 * to render. If not, decline to render it by returning NULL.
 */
function page_manager_redirect_redirect_context_render($handler, $base_contexts, $args, $test = TRUE) {
  // Go through arguments and see if they match.
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

  $info = array(
    'content' => page_manager_redirect_redirect_context_render_actual($handler, $contexts),
  );

  return $info;
}

/**
 * Special handling for exporting a panel task handler.
 *
 * When a panel is exported, we need to export the display separately
 * rather than just letting its object be unpacked, which does not work
 * very well.
 */
function page_manager_redirect_redirect_context_export(&$handler, $indent) {
  // Left to remove legacy code when exporting.
  // TODO: Take this out, and do it to any old displays in upgrade path instead.
  foreach (array('display', 'did') as $item) {
    if (isset($handler->conf[$item])) {
      unset($handler->conf[$item]);
    }
  }
}

/**
 * Set up a title for the panel based upon the selection rules.
 */
function page_manager_redirect_redirect_context_title($handler, $task, $subtask) {
  if (isset($handler->conf['title'])) {
    return check_plain($handler->conf['title']);
  }
  else {
    return t('Redirect');
  }
}

/**
 * Provide a nice little summary of what's in a panel.
 *
 * The task handler manager provides a summary of a given handler in a
 * collapsible div. This callback provides that. For Redirects, we provide a
 * summary of the redirect on one side, and a summary of the contexts in use on
 * the other.
 */
function page_manager_redirect_redirect_context_admin_summary($handler, $task, $subtask, $page, $show_title = TRUE) {
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
    array('class' => t('page-summary-label'), 'data' => t('Storage')),
    array('class' => t('page-summary-data'), 'data' => $type),
    array('class' => t('page-summary-operation'), 'data' => ''),
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
    array('class' => t('page-summary-label'), 'data' => t('Status')),
    array('class' => t('page-summary-data'), 'data' => $text),
    array('class' => t('page-summary-operation'), 'data' => $link),
  );

  $link = l(t('Edit'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'criteria')));
  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Selection rule')),
    array('class' => t('page-summary-data'), 'data' => $access),
    array('class' => t('page-summary-operation'), 'data' => $link),
  );

  $link = l(t('Edit'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'settings')));

  if (is_array($handler->conf)) {
    foreach ($handler->conf as $part => $value) {
      // Remove spaces - this will allow spaces to be used to delimit the context 'tokens' that need to be replaced!
      $handler->conf[$part] = str_replace(' ', '', $value);
    }
  }

  // Strip leading # from fragment if it's there.
  if (strpos($handler->conf['fragment'], '#') === 0) {
    $handler->conf['fragment'] = substr($handler->conf['fragment'], 1);
  }

  // Convert the query from a string to an array that can be used with url().
  $query = page_manager_redirect_redirect_context_build_query_array($handler->conf['query']);
  $options = array('query' => $query, 'fragment' => $handler->conf['fragment'], 'absolute' => TRUE);

  // Build the URL to show and to link to. This won't be a valid link if it's
  // got context keywords in it, since we have no context to substitute here.
  $url = '';
  if (!empty($handler->conf['path'])) {
    // Handle <front>
    $url .= $handler->conf['path'] == '<front>' ? '' : $handler->conf['path'];
  }
  if (count($query)) {
    $url .= '?' . drupal_http_build_query($query);
  }
  if (!empty($handler->conf['fragment'])) {
    $url .= '#' . $handler->conf['fragment'];
  }

  // Prepend the base URL of the site, just because it looks better.
  global $base_url;
  $url_linked = l($base_url . '/' . $url, $handler->conf['path'], $options);

  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Redirect URL')),
    array('class' => t('page-summary-data'), 'data' => $url_linked),
    array('class' => t('page-summary-operation'), 'data' => $link),
  );

  $info = theme('table', array('rows' => $rows, 'attributes' => array('class' => 'page-manager-handler-summary')));

  $title = $handler->conf['title'];
  if ($title != t('Redirect')) {
    $title = t('Redirect: @title', array('@title' => $title));
  }

  $output .= '<div class="clear-block">';
  if ($show_title) {
    $output .= '<div class="handler-title clear-block">';
    $output .= '<div class="actions handler-actions">' . $rendered_operations['actions'] . '</div>';
    $output .= '<span class="title-label">' . $title . '</span>';
  }

  $output .= '</div>';
  $output .= $info;
  $output .= '</div>';

  return $output;
}

// --------------------------------------------------------------------------
// Forms

/**
 * General notes about forms: The handler is automatically cached by the form
 * wizard, so anything we store on $form_state['handler'] anywhere will get
 * saved and appear on the next form. The cache is a 'working' cache and
 * if the user hits cancel on any page of the multi-page wizard, all
 * changes since the last 'update/finish' click will be flushed away.
 */

/**
 * General settings for the panel
 */
function page_manager_redirect_redirect_context_edit_settings($form, &$form_state) {
  $conf = $form_state['handler']->conf;
  $form['conf']['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
    '#title' => t('Administrative title'),
    '#description' => t('Administrative title of this variant.'),
  );

  $form['conf']['contexts'] = array(
    '#type' => 'markup',
    '#value' => '<p>You may use context keywords in the fields below. Refer to the Contexts tab for details of these. <br />You may surround keywords with spaces to delimit them. For example: <em>filters=tid:<strong> %term:tid </strong>type:news</em>.</p>'
  );

  $form['conf']['path'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['path'],
    '#title' => t('URL path to redirect to'),
    '#description' => t('A Drupal path (for example: <em>node/12</em>, <em>&lt;front&gt;</em>) or a full URL that the user will be redirected to if this variant is selected. Include the protocol (e.g. <em>http://</em>) for full URLs.'),
  );

  $form['conf']['query'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['query'],
    '#title' => t('Query string'),
    '#description' => t('A query string component for the redirection, if any, without any URL-encoding. (for example: <em>redirect=1&color=red</em>)<br />This will be validated, so the actual query string used may not reflect exactly what is entered here if it is invalid. See the <a href="summary">summary</a> or <a href="preview">preview</a> tabs to see what will actually be used. For example, parameters must all follow the form key=value (any without the equals sign will be changed to value=value), and each of these key/value pairs must be separated with a &. Use | in place of any & characters that are intended as part of a key or value.'),
  );

  $form['conf']['fragment'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['fragment'],
    '#title' => t('Anchor name'),
    '#description' => t('A destination fragment identifier for the redirection. This may mark a place on a page to skip to (for example: <em>#section3</em>). You may include or omit the <em>#</em>.'),
  );

  return $form;
}

/**
 * Submit handler for general settings form.
 */
function page_manager_redirect_redirect_context_edit_settings_submit(&$form, &$form_state) {
  $form_state['handler']->conf['title'] = $form_state['values']['title'];
  $form_state['handler']->conf['path'] = $form_state['values']['path'];
  $form_state['handler']->conf['query'] = $form_state['values']['query'];
  $form_state['handler']->conf['fragment'] = $form_state['values']['fragment'];
}

/**
 * Form to show a nice preview.
 */
function page_manager_redirect_redirect_context_edit_preview($form, &$form_state) {
  ctools_include('context');
  ctools_include('context-task-handler');

  $contexts = ctools_context_handler_get_all_contexts($form_state['task'], $form_state['subtask'], $form_state['handler']);
  $form['preview'] = array();
  ctools_context_replace_form($form['preview'], $contexts);

  // automatically preview if there are no argument placeholders.
  if (empty($form['preview'])) {
    $output = page_manager_redirect_redirect_context_render_actual($form_state['handler'], $contexts, TRUE);
    if (isset($form['buttons'])) {
      unset($form['buttons']);
    }
  }
  else {
    $form['preview']['#tree'] = TRUE;
    $form_state['contexts'] = $contexts;
  }

  if (!empty($output)) {
    $form['output'] = array(
      '#markup' => $output,
    );
  }

  $form_state['do not cache'] = TRUE;

  return $form;
}

/**
 * Display a preview upon submit if arguments were needed.
 */
function page_manager_redirect_redirect_context_edit_preview_submit(&$form, &$form_state) {
  $context = ctools_context_replace_placeholders($form_state['contexts'], $form_state['values']['preview']);

  $form_state['content'] = page_manager_redirect_redirect_context_render_actual($form_state['handler'], $context, TRUE); // Instead of panels_render_display().
  $form_state['redirect'] = FALSE;
  $form_state['rerender'] = TRUE;
}

/**
 * 'Render' a redirect display by simply redirecting.
 *
 * if $incoming_content is NULL, default content will be applied. Use
 * an empty string to indicate no content.
 * @render
 * @ingroup hook_invocations
 */
function page_manager_redirect_redirect_context_render_actual(&$handler, $context, $preview = FALSE) {
  ctools_include('context');

  // We only care about a few of the keys in the conf array.
  $conf = page_manager_redirect_redirect_context_get_redirection($handler);

  if (is_array($conf)) {
    foreach ($conf as $part => $value) {
      $replaced = ctools_context_keyword_substitute($value, array(), $context);
      // Remove spaces - this will allow spaces to be used to delimit the context 'tokens' that need to be replaced!
      $conf[$part] = str_replace(' ', '', $replaced);
    }

    // Strip a leading '#' from the fragment if present, for use in
    // drupal_goto() or url(). For the case where a leading '#' is required,
    // simply add an additional '#' to the fragment in the UI. (e.g. ##fragment)
    if (isset($conf['fragment'])) {
      if (strpos($conf['fragment'], '#') === 0) {
        $conf['fragment'] = substr($conf['fragment'], 1);
      }
    }
  }

  // Convert the query from a string to an array that can be used with url().
  $query = page_manager_redirect_redirect_context_build_query_array($conf['query']);

  if ($preview) {
    $options = array('query' => $query, 'fragment' => $conf['fragment'], 'absolute' => TRUE);
    $url = url($conf['path'], $options);
    $url_linked = l(check_plain($url), $conf['path'], $options);

    $output = '<p>The user will be redirected to:</p>';
    $output .= '<p>' . $url_linked . '</p>';

    return $output;
  }
  else {
    // We're assuming drupal_goto can handle any unsafe things passed to it.
    // This may turn out to be a false assuption, but looks correct to me.
    drupal_goto($conf['path'], array('query' => $query, 'fragment' => $conf['fragment']));
  }
}

/**
 * Transforms query strings entered by the user into arrays that can be used
 * with url(), l() and drupal_goto().
 */
function page_manager_redirect_redirect_context_build_query_array($string = NULL) {
  $array = array();

  if (!is_string($string) || $string == '') {
    return $array;
  }

  // Split the string up by '&' into key/value pairs.
  $pairs = explode('&', $string);
  foreach ($pairs as $pos => $pair) {
    // Handle blank strings
    if ($pair == '') {
      continue;
    }

    // Replace the & replacement character.
    $pair = str_replace('|', '&', $pair);

    if (strpos($pair, '=') == FALSE) {
      // If there is no '=', map key => value.
      $value = $key = $pair;
    }
    else {
      // Split $pair up around any '='
      list($key, $value) = explode('=', $pair);
    }
    $array[$key] = $value;
  }

  return $array;
}
