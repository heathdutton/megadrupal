<?php

/**
 * Ctools custom content type plugin definition for personalizing Panels.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Personalization Pane'),
  'description' => t('Personalize two panes in this Panel.'),
  'category' => t('Personalize'),
  'defaults' => array(
    'title' => NULL,
    'osid' => NULL,
  ),
  'all contexts' => TRUE,
);

/**
 * Render all of the linked panes in a Lift json blob so Lift.
 */
function personalize_panels_personalize_pane_content_type_render($subtype, $conf, $args, $contexts) {
  ctools_include('content');
  $option_set = personalize_option_set_load($conf['osid']);
  if (!$option_set) {
    return;
  }

  $display = personalize_panels_load_panels_display($option_set->data['page_did'], $option_set->data['task_name']);
  $display->context = $contexts;
  $render_handler = panels_get_renderer_handler($display->renderer, $display);
  // We render every pane option we are showing and pass it along to the
  // browser as a json string in a script tag so the right option can be
  // loaded without an AJAX call on page load.
  $renderable = array();
  $renderable['#theme_wrappers'] = array('personalize_options_wrapper');
  $renderable['#personalize_option_set'] = $option_set;
  $renderable['panes'] = array();
  $renderable['#personalize_options'] = array();

  foreach ($option_set->options as $option) {
    foreach ($display->content as $pane) {
      if ($option['panels_pane_pid'] == $pane->uuid) {
        $render_pane = $render_handler->render_pane($pane);
        $renderable['panes'][]['#markup'] = $renderable['#personalize_options'][$option['option_id']]['#markup'] = $render_pane;
        break;
      }
    }
  }
  $renderable['#first_option'] = $renderable['panes'][0];
  personalize_element_with_option_set($renderable, $option_set, TRUE);

  $block = new stdClass();
  $block->title = NULL;
  $block->content = $renderable;
  return $block;
}

/**
 * Set the title in the Panels admin.
 */
function personalize_panels_personalize_pane_content_type_admin_title($subtype, $conf) {
  return t('Personalization: %title', array('%title' => $conf['title']));
}

/**
 * Set the pane preview in the Panels interface.
 */
function personalize_panels_personalize_pane_content_type_admin_info($subtype, $conf, $contexts) {
  $option_set = personalize_option_set_load($conf['osid']);
  if (!$option_set) {
    return;
  }
  $display = personalize_panels_load_panels_display($option_set->data['page_did'], $option_set->data['task_name']);
  $display->context = $contexts;

  $names = array();
  foreach ($option_set->options as $pane) {
    foreach ($display->content as $key => $value) {
      if ($pane['panels_pane_pid'] == $value->uuid) {
        $panel_names[$value->uuid] = ctools_content_admin_title($value->type, $value->subtype, $value->configuration, $display->context);
        $panel_names[$value->uuid] .= ' (' . $value->uuid .')';
        break;
      }
    }
  }

  $block = new stdClass();
  $block->title = t('Personalization details:');
  $panes = theme('item_list', array('items' => $panel_names));
  $block->content = t('Running test on panes:<br />!panes', array('!panes' => $panes));
  return $block;
}

/**
 * Admin form to let the user select the two panes in the test.
 */
function personalize_panels_personalize_pane_content_type_edit_form($form, &$form_state) {
  $display = $form_state['display'];
  $conf = $form_state['conf'];
  $option_set = personalize_option_set_load($conf['osid']);

  unset($form['override_title']);
  unset($form['override_title_text']);
  unset($form['override_title_markup']);
  // We need to include these file for our ajax add. During ajax requests drupal
  // serves the form from the cache so this file and ctools api files aren't 
  // included.
  form_load_include($form_state, 'inc', 'personalize_panels', 'plugins/content_types/personalize_pane');
  form_load_include($form_state, 'inc', 'ctools', 'includes/content');

  $form['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
    '#title' => t('Administrative title'),
    '#description' => t('This title is shown in the Panels editor and never shown when the page is rendered.'),
    '#required' => TRUE,
  );

  $agents = array();
  foreach (personalize_agent_load_multiple() as $agent) {
    $agents[$agent->machine_name] = $agent->label;
  }

  $form['agent'] = array(
    '#type' => 'select',
    '#title' => t('Campaign'),
    '#options' => $agents,
    '#default_value' => !empty($option_set->agent) ? $option_set->agent : NULL,
    '#required' => TRUE,
  );

  $no_did = FALSE;
  $panes = array();
  foreach ($display->content as $pane) {
    if (empty($pane->did)) {
      $no_did = TRUE;
      continue;
    }
    $admin_title = ctools_content_admin_title($pane->type, $pane->subtype, $pane->configuration, $display->context);
    $panes[$pane->uuid] = ctools_content_admin_title($pane->type, $pane->subtype, $pane->configuration, $display->context);
  }
  // $no_did indicates that the panel page has been overridden and is being 
  // served out of a features file. This is fine for display, but we can't save
  // all of the information that we need for our option set while it is in this
  // state.
  if ($no_did) {
    drupal_set_message(t('This panels page is currenty overridden in code and as such this option set is missing information that it needs to save properly. Please save the panels page and then try to add this option set again.'), 'warning');
  }
  $form['#tree'] = TRUE;
  $form['panes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select the panes in this Panel that should participate in the test.'),
    '#theme' => 'table',
    '#tree' => TRUE,
    '#header' => array(t('Option Label'), t('Panel')),
    '#rows' => array(),
    '#prefix' => '<div id="personalize-options-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  // Build the panes table with the proper number of options. We'll use 
  // $form_state['num_options'] to determine the number of rows to build.
  $num_options = !empty($option_set->options) ? count($option_set->options) : 2;
  $form_state['num_options'] = !empty($form_state['num_options']) ? $form_state['num_options'] : $num_options;
  for ($i = 0; $i < $form_state['num_options']; $i++) {
    // Build the fields for this row in the table. We'll be adding these to the 
    // form several times, so it's easier if they are individual variables 
    // rather than in an array.
    $label = array(
      '#type' => 'textfield',
      '#title' => t('Option @num Label', array('@num' => $i + 1)),
      '#required' => TRUE,
      '#default_value' => isset($option_set->options[$i]['option_label']) ? $option_set->options[$i]['option_label'] : t('Option @num', array('@num' => $i + 1)),
    );
    $pid = array(
      '#type' => 'select',
      '#title' => t('Option @num', array('@num' => $i + 1)),
      '#options' => $panes,
      '#required' => TRUE,
      '#default_value' => isset($option_set->options[$i]['panels_pane_pid']) ? $option_set->options[$i]['panels_pane_pid'] : NULL,
    );
    // Include the fields so they'll be rendered and named correctly, but 
    // they'll be ignored here when rendering as we're using theme_table. Note 
    // that we're using references to the variables, not just copying the values 
    // into the array.
    $form['panes'][$i] = array(
      "option_label" => &$label,
      'panels_pane_pid' => &$pid,
    );
    // Now add references to the fields to the rows that theme_table will use.
    // As we've used references, the table will use the very same field arrays 
    // as the FAPI used above.
    $form['panes']['#rows'][] = array(
      array('data' => &$label),
      array('data' => &$pid),
    );
    // Because we've used references we need to unset variables. If we don't 
    // then every iteration of the loop will just overwrite the variables we 
    // created the first time through leaving us with a form with 3 copies of 
    // the same fields.
    unset($label);
    unset($pid);
  }

  $form['pane_controls']['add_pane'] = array(
    '#type' => 'submit',
    '#value' => t('Add an option'),
    '#submit' => array('personalize_panels_personalize_pane_ajax_add_option'),
    '#ajax' => array(
      'callback' => 'personalize_panels_personalize_pane_ajax',
      'wrapper' => 'personalize-options-fieldset-wrapper',
    ),
    '#limit_validation_errors' => array(),
  );

  if ($form_state['num_options'] > 1) {
    $form['pane_controls']['remove_pane'] = array(
      '#type' => 'submit',
      '#value' => t('Remove an option'),
      '#submit' => array('personalize_panels_personalize_pane_ajax_remove_option'),
      '#ajax' => array(
        'callback' => 'personalize_panels_personalize_pane_ajax',
        'wrapper' => 'personalize-options-fieldset-wrapper',
      ),
      '#limit_validation_errors' => array(),
    );
  }

  $form['osid'] = array(
    '#type' => 'value',
    '#value' => $conf['osid'],
  );

  return $form;
}

/**
 *  AJAX submit add handler for a pane option.
 */
function personalize_panels_personalize_pane_ajax_add_option($form, &$form_state) {
  $form_state['num_options']++;
  $form_state['rebuild'] = TRUE;
}

/**
 *  AJAX submit remove handler for a pane option.
 */
function personalize_panels_personalize_pane_ajax_remove_option($form, &$form_state) {
  if ($form_state['num_options'] > 2) {
    $form_state['num_options']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 *  AJAX replace handler for the add/remove buttons in the edit form.
 */
function personalize_panels_personalize_pane_ajax($form, $form_state) {
  return $form['panes'];
}

/**
 * Save the user's data.
 */
function personalize_panels_personalize_pane_content_type_edit_form_validate($form, &$form_state) {
  $input = $form_state['input'];
  $panes = array();
  $error = FALSE;
  foreach ($input['panes'] as $key => $pane) {
    if (isset($panes[$pane['panels_pane_pid']])) {
      form_set_error("panes][$key");
      $error = TRUE;
    }
    $panes[$pane['panels_pane_pid']] = TRUE;
  }

  if ($error) {
    form_set_error('error', t('Each pane can only be used once.'));
  }
}

/**
 * Save the user's data.
 */
function personalize_panels_personalize_pane_content_type_edit_form_submit($form, &$form_state) {
  foreach ($form_state['plugin']['defaults'] as $key => $default_value) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
  $display = $form_state['display'];
  $conf = &$form_state['conf'];
  // The CTools custom content type plugin system provides very little 
  // to the edit form about the page context as a whole. The display, however
  // provides the task and subtask id names, so luckily we can grab page info.
  // We need this info later because we need to iterate over this pages panes
  // in order to render our new personalized content type.
  list($handler, $task_name, $subtask_id, $manager) = preg_split('/[:]+/', $display->cache_key);
  // Panelizer and Page Manager have different edit urls, so we'll store this
  // as part of the option set for use during hook_personalize_edit_link().
  $edit_url = "admin/structure/pages/nojs/operation/{$task_name}/handlers/{$subtask_id}/content";
  if ($handler == 'panelizer') {
    $edit_url = "{$task_name}/{$subtask_id}/{$handler}/{$manager}/content";
  }

  // Create the personalization set and save it to this pane instance so we can
  // call it during rendering.
  $ppane = new stdClass();
  $ppane->osid = empty($conf['osid']) ? NULL : $conf['osid'];
  $ppane->plugin = 'panel_pane';
  $ppane->agent = $form_state['values']['agent'];
  $ppane->label = $form_state['values']['title'];
  $ppane->data = array(
    'page_did' => $display->did,
    'task_name' => page_manager_make_task_name($task_name, $subtask_id),
    'handler_id' => $subtask_id,
    'edit_url' => $edit_url,
  );
  $ppane->options = array();
  foreach ($form_state['input']['panes'] as $form_pane) {
    $ppane->options[] = $form_pane;
  }

  $option_set = personalize_option_set_save($ppane);
  if (empty($conf['osid'])) {
    $conf['osid'] = $option_set->osid;
  }
}

/**
 * Loads a panels display.
 *
 * @param mixed $id
 *  A task name as set by page_manager_make_task_name() or a numeric display id.
 *
 * @return object
 *   A panels display object.
 */
function personalize_panels_load_panels_display($id, $task_name = NULL) {
  $page = page_manager_get_current_page();
  if (is_numeric($id)) {
    return panels_load_display($id);
  }
  list($task_name, $subtask_id, $handler_id) = explode('-', $task_name);
  $handlers = page_manager_load_sorted_handlers(page_manager_get_task($task_name), $subtask_id);
  $handler = $handlers[$handler_id];
  return panels_panel_context_get_display($handler);
}
