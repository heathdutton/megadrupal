<?php
/**
 * @file
 * Contains forms and form related methods.
 */

/**
 * Admin settings form.
 */
function qui_admin_settings_form($form, &$form_state) {
  $form['reports'] = array(
    '#type' => 'fieldset',
    '#title' => t('Query Settings'),
  );
  $form['reports']['qui_preview_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Report Preview Limit'),
    '#size' => 3,
    '#description' => t('The number of rows to display during query previews.'),
    '#default_value' => variable_get('qui_preview_limit', 5),
  );
  $form['reports']['qui_show_revision_tables'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show revision tables'),
    '#description' => t('Revision tables can be selected and joined upon.'),
    '#default_value' => variable_get('qui_show_revision_tables', FALSE),
  );
  $form['reports']['qui_show_cache_tables'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show cache tables'),
    '#description' => t('Cache tables can be selected and joined upon.'),
    '#default_value' => variable_get('qui_show_cache_tables', FALSE),
  );
  $form['reports']['qui_autocomplete_input'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use autocomplete text input for picking tables/columns'),
    '#description' => t('The autocomplete text widget makes choosing tables/columns easier than the built-in select
    list.'),
    '#default_value' => variable_get('qui_autocomplete_input', TRUE),
  );
  $form['reports']['qui_show_query'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the executed query in previews by default.'),
    '#description' => t('The query can be viewed normally by clicking on "Show Query". This setting keeps the query visible at all times.'),
    '#default_value' => variable_get('qui_show_query', FALSE),
  );
  $form['reports']['qui_show_braces'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the curly braces around executed query in previews.'),
    '#description' => t('By default, the query will be shown with the curly braces replaced by any table prefixes you may be using. Enable this to keep the braces in place instead.'),
    '#default_value' => variable_get('qui_show_braces', FALSE),
  );

  $form['formats'] = array(
    '#type' => 'fieldset',
    '#title' => t('Format Settings'),
  );
  $form['formats']['qui_gzip_format_output'] = array(
    '#type' => 'checkbox',
    '#title' => t('GZip formatter output'),
    '#default_value' => variable_get('qui_gzip_format_output', TRUE),
  );

  return system_settings_form($form);
}

/**
 * Get reports for the listing and apply filters.
 *
 * @param array $filters
 *   Report filters.
 *
 * @return array
 *   Array of DQUI Reports.
 */
function _qui_report_listing(array $filters = array()) {
  if (empty($filters)) {
    return _qui_get_all_reports();
  }
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'qui_report');
  if ($filters['name']) {
    $query->propertyCondition('name', "%{$filters['name']}%", 'LIKE');
  }
  if ($filters['uid']) {
    $query->propertyCondition('author_uid', $filters['uid']);
  }
  if ($filters['tag']) {
    $query->propertyCondition('tags', "%{$filters['tag']}%", 'LIKE');
  }
  $query->propertyOrderBy('created', 'DESC');
  $results = $query->execute();
  return qui_report_load_multiple(array_keys($results['qui_report']));
}

/**
 * Report listing.
 */
function qui_report_list_form($form, &$form_state) {

  $form['#attached']['js'][] = drupal_get_path('module', 'qui') . '/qui.js';

  $filters = isset($form_state['values']['filters']) ? $form_state['values']['filters'] : array();
  $reports = _qui_report_listing($filters);

  // Check for "new" report unsaved changes.
  if (isset($_COOKIE['Drupal_visitor_qui_form_cache'])) {
    drupal_set_message(t('You have a new report with unsaved changes. !link', array(
      '!link' => l(t('View now'), 'admin/reports/qui/add'),
    )), 'warning');
  }

  // Check for unsaved report changes and display messages.
  $cached_forms = FALSE;
  foreach ($reports as $r) {
    if (isset($_COOKIE['Drupal_visitor_qui_form_cache_' . $r->identifier()])) {
      $cached_forms = TRUE;
    }
  }
  if ($cached_forms) {
    drupal_set_message(t('You have reports with unsaved changes (highlighted in red).'), 'warning');
  }

  $form['#id'] = 'qui-report-list-form';

  // Only need filters if we have reports.
  if (isset($form_state['values']['filters']) || count($reports) != 0) {
    $form['filters'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filters'),
      '#collapsed' => empty($form_state['values']['filters']) ? TRUE : FALSE,
      '#collapsible' => TRUE,
      '#tree' => TRUE,
    );
    $form['filters']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#default_value' => isset($form_state['values']['filters']['name']) ? $form_state['values']['filters']['name'] : '',
      '#size' => 20,
    );
    $form['filters']['uid'] = array(
      '#type' => 'textfield',
      '#title' => t('UID'),
      '#default_value' => isset($form_state['values']['filters']['uid']) ? $form_state['values']['filters']['uid'] : '',
      '#size' => 4,
    );
    $form['filters']['tag'] = array(
      '#type' => 'textfield',
      '#title' => t('Tag'),
      '#default_value' => isset($form_state['values']['filters']['tag']) ? $form_state['values']['filters']['tag'] : '',
      '#size' => 20,
    );
    $form['filters']['apply'] = array(
      '#prefix' => '<br />',
      '#type' => 'button',
      '#value' => t('Apply'),
      '#ajax' => array(
        'callback' => 'qui_refresh_ajax_form',
        'wrapper' => 'qui-report-list-form',
        'progress' => array('message' => t('loading...'), 'type' => 'throbber'),
      ),
    );
    if (isset($form_state['values']['filters'])) {
      $form['filters']['clear'] = array(
        '#markup' => l(t('Clear Filters'), 'admin/reports/qui'),
      );
    }
  }

  $form['list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reports'),
  );

  // No reports created yet.
  if (!isset($form_state['values']['filters']) && count($reports) == 0) {

    if (user_access('create new qui reports')) {
      $form['list']['empty'] = array(
        '#markup' => t("You haven't created any reports yet. !link.", array(
          '!link' => l(t('Create a report now'), 'admin/reports/qui/add'),
        )),
      );
    }
    else {
      $form['list']['empty'] = array(
        '#markup' => t("You haven't created any reports yet."),
      );
    }

    return $form;
  }

  $form['list']['table'] = array(
    '#prefix' => '<table>',
    '#suffix' => '</table',
  );

  $header = array(
    '<th width=25>ID</th>',
    '<th>Name</th>',
    '<th>Machine Name</th>',
    '<th width=100>Description</th>',
    '<th width=120>Tags</th>',
    '<th width=150>Allowed Roles</th>',
    '<th width=110>Formats</th>',
    '<th width=225>Operations</th>',
  );
  $form['list']['table']['header'] = array(
    '#markup' => implode('', $header),
  );
  $form['list']['table']['rows'] = array(
    '#tree' => TRUE,
  );

  $count = 0;
  foreach ($reports as $r) {

    if (!qui_report_access('view', $r)) {
      // User cannot access this report.
      continue;
    }

    $id = $r->internalIdentifier();

    $row_class = ($count % 2 == 0) ? 'odd' : 'even';
    $unsaved_changes = FALSE;

    // Check if the report has unsaved changes.
    if (isset($_COOKIE['Drupal_visitor_qui_form_cache_' . $r->identifier()])) {
      $unsaved_changes = TRUE;
      $row_class .= ' unsaved-changes';
    }

    $form['list']['table']['rows'][$id] = array(
      '#prefix' => '<tr class="' . $row_class . '">',
      '#suffix' => '</tr>',
    );
    $form['list']['table']['rows'][$id]['rid'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => $r->id,
    );
    $form['list']['table']['rows'][$id]['name'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => $r->name . '<br />Status: ' . _qui_report_status($r),
    );
    $form['list']['table']['rows'][$id]['machine_name'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => $r->machine_name,
    );
    if ($unsaved_changes) {
      $form['list']['table']['rows'][$id]['name']['#markup'] .= ' ' . l(t(' * '), $r->editPath(), array(
          'attributes' => array('title' => t('Report has unsaved changes. Click to view.')),
        ));
    }

    $form['list']['table']['rows'][$id]['description'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => !empty($r->description) ? '<a href="#" data-description="' . htmlspecialchars(check_plain($r->description)) . '" title="' . t('Click to view the description') . '">View Description</a>' : '',
    );
    $form['list']['table']['rows'][$id]['tags'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => implode(', ', $r->tags),
    );
    $allowed_roles = array();
    if (isset($r->settings->allowed_roles)) {
      foreach ($r->settings->allowed_roles as $rid) {
        $allowed_roles[] = user_role_load($rid)->name;
      }
    }
    $form['list']['table']['rows'][$id]['allowed_roles'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => theme('item_list', array('items' => $allowed_roles)),
    );
    $form['list']['table']['rows'][$id]['formats']['view'] = array(
      '#prefix' => '<td>',
      '#type' => 'select',
      '#options' => isset($r->settings->allowed_formats) ? _qui_report_list_format_formats($r->settings->allowed_formats) : _qui_report_list_format_formats(array()),
      '#empty_option' => t('view as...'),
      '#redirect_form' => $id,
      '#attributes' => array('onchange' => 'jQuery("#edit-view-report").click();jQuery(this).val(0);'),
    );
    $form['list']['table']['rows'][$id]['formats']['download'] = array(
      '#suffix' => '</td>',
      '#type' => 'select',
      '#options' => isset($r->settings->allowed_formats) ? _qui_report_list_format_formats($r->settings->allowed_formats) : _qui_report_list_format_formats(array()),
      '#empty_option' => t('download as...'),
      '#redirect_form' => $r->id,
      '#attributes' => array('onchange' => 'jQuery("#edit-view-report").click();jQuery(this).val(0);'),
    );
    $form['list']['table']['rows'][$id]['operations'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => _qui_report_list_options($r),
    );
    $count++;
  }
  $form['list']['table']['view_report'] = array(
    '#type' => 'submit',
    '#value' => t('View'),
    '#submit' => array('qui_view_report_redirect'),
    '#attributes' => array('style' => 'display:none;'),
  );
  return $form;
}

/**
 * Handles form redirect based on format chosen from the dropdown.
 */
function qui_view_report_redirect(&$form, &$form_state) {
  foreach ($form_state['values']['rows'] as $id => $row) {
    if (!empty($row['formats']['download'])) {
      $form_state['redirect'] = "qui/reports/download/{$id}/{$row['formats']['download']}";
    }
    elseif (!empty($row['formats']['view'])) {
      $form_state['redirect'] = "qui/reports/view/{$id}/{$row['formats']['view']}";
    }
  }
}

/**
 * Returns a list of well-formatted options for the report listing table.
 */
function _qui_report_list_options($r) {
  $output = '';
  $id = $r->identifier();
  $output .= l(t('View'), "admin/reports/qui/manage/{$id}/view");
  if (drupal_valid_path("admin/reports/qui/manage/{$id}/edit")) {
  }
  $output .= ' | ' . l(t('Edit'), "admin/reports/qui/manage/{$id}/edit");
  if (drupal_valid_path("admin/reports/qui/manage/{$id}/clone")) {
  }
  $output .= ' | ' . l(t('Clone'), "admin/reports/qui/manage/{$id}/clone");
  if (drupal_valid_path("admin/reports/qui/manage/{$id}/export")) {
  }
  $output .= ' | ' . l(t('Export'), "admin/reports/qui/manage/{$id}/export");
  if (drupal_valid_path("admin/reports/qui/manage/{$id}/delete")) {
  }
  if ($r->hasStatus(ENTITY_OVERRIDDEN)) {
    $output .= ' | ' . l(t('Revert'), "admin/reports/qui/manage/{$id}/revert");
  }
  elseif ($r->hasStatus(ENTITY_CUSTOM) || $r->status == 0) {
    $output .= ' | ' . l(t('Delete'), "admin/reports/qui/manage/{$id}/delete");
  }
  return $output;
}

/**
 * Returns a well-formatted list of formats.
 */
function _qui_report_list_format_formats($formats) {
  $info = qui_get_formatters();
  $return = array();
  foreach ($formats as $format) {
    $return[$format] = $info[$format]['name'];
  }
  return $return;
}

/**
 * Theme function for form.
 */
function theme_qui_create_report_form($vars) {
  $form = $vars['form'];

  _qui_report_form_joins_table($form);
  _qui_report_form_conditions_table($form);
  _qui_report_form_expressions_table($form);
  _qui_report_form_columns_table($form);
  _qui_report_form_group_table($form);
  _qui_report_form_order_table($form);

  $output = drupal_render_children($form);
  return $output;
}

/**
 * Formats the report query form groups to a draggable table.
 */
function _qui_report_form_group_table(&$form) {
  $header = array(t('Column'), t('Operations'));
  _qui_report_form_table($form, 'group', array(
    'column',
    'delete_group',
  ), $header, 'extras', $tabledrag = FALSE);
}

/**
 * Formats the report query form orders to a draggable table.
 */
function _qui_report_form_order_table(&$form) {
  $header = array('', t('Type'), t('Column'), t('Direction'), t('Weight'), t('Operations'));
  _qui_report_form_table($form, 'order', array(
    'type',
    'column',
    'direction',
    'weight',
    'delete_order',
  ), $header, 'extras');
}

/**
 * Formats the report query form columns to a draggable table.
 */
function _qui_report_form_columns_table(&$form) {
  $header = array('', t('Alias'), t('Column'), t('Weight'), t('Operations'));
  _qui_report_form_table($form, 'columns', array(
    'alias',
    'column',
    'weight',
    'delete_column',
  ), $header);
}


/**
 * Formats the report query form expressions to a draggable table.
 */
function _qui_report_form_expressions_table(&$form) {
  $header = array('', t('Alias'), t('Expression'), t('Weight'), t('Operations'));
  _qui_report_form_table($form, 'expressions', array(
    'alias',
    'value',
    'weight',
    'delete_expression',
  ), $header);
}

/**
 * Formats the report query form conditions to a draggable table.
 */
function _qui_report_form_conditions_table(&$form) {
  $header = array('', t('Type'), t('Column'), t('Operator'), t('Value'), t('Weight'), t('Operations'));
  _qui_report_form_table($form, 'conditions', array(
    'type',
    'column',
    'operator',
    'value',
    'weight',
    'delete_condition',
  ), $header);
}

/**
 * Formats the report query form joins to a draggable table.
 */
function _qui_report_form_joins_table(&$form) {
  $header = array('', t('Type'), t('Table'), t('Left Column'), t('Right Column'), t('Weight'), t('Operations'));
  _qui_report_form_table($form, 'joins', array(
    'type',
    'table',
    'left_col',
    'right_col',
    'weight',
    'delete_join',
  ), $header);
}

/**
 * Generate a table drag for the query builder form tables.
 *
 * @param array $form
 *   Form for the table to be rendered.
 * @param string $type
 *   Type of table (join, conditions, expressions, etc)
 * @param array $elements
 *   Element keys to add to the table.
 * @param array $header
 */
function _qui_report_form_table(array &$form, $type, array $elements = array(), array $header = array(), $sub = '', $tabledrag = TRUE) {
  if ($sub) {
    $type_array = & $form['query'][$sub][$type . '-wrapper'][$type];
  }
  else {
    $type_array = & $form['query'][$type . '-wrapper'][$type];
  }
  $rows = array();
  $row = 0;
  if (empty($type_array)) {
    return;
  }
  foreach (element_children($type_array, $sort = TRUE) as $key) {
    if ($tabledrag) {
      $rows[$row]['data'][] = array();
    }
    foreach ($elements as $element) {
      $rows[$row]['data'][] = drupal_render($type_array[$key][$element]);
    }
    $rows[$row]['class'] = array('draggable');
    $row++;
  }
  if (count($rows) == 0) {
    return;
  }
  $type_array = array(
    '#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('id' => $type . '-table')
    )),
  );
  if ($tabledrag) {
    drupal_add_tabledrag($type . '-table', 'order', 'sibling', $type . '-weight');
  }
}

/**
 * Form for creating a new DQUI report.
 */
function qui_report_form($form, &$form_state, QuiReport $report = NULL, $action = 'new') {
  $form['#theme'] = 'qui_create_report_form';
  $form['#attached']['js'][] = drupal_get_path('module', 'qui') . '/qui.js';
  $js_settings = array(
    'qui' => array(
      'use_autocomplete' => variable_get('qui_autocomplete_input', TRUE),
      'show_query' => variable_get('qui_show_query', FALSE),
    ),
  );
  drupal_add_js($js_settings, 'setting');

  $form['#id'] = 'qui-create-report-form';
  $form_state['#report'] = $report ? : new QuiReport();
  $form_state['#action'] = $action;
  $controller = new Qui_Query_Builder_Controller($form, $form_state);

  // Check if there is a cached form to return.
  if ($controller->getHandler('qui_cache')->getCache()) {
    return $form;
  }

  $controller->preRender();
  $controller->render();
  $form_state['#controller'] = & $controller;

  return $form;
}

/**
 * Basic AJAX callback to return the entire form.
 */
function _qui_query_builder_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validate the report creation form.
 */
function qui_report_form_validate(&$form, &$form_state) {
  /** @var $controller Qui_Query_Builder_Controller */
  $controller = $form_state['#controller'];
  $controller->setVars($form, $form_state);
  $controller->preRender();
  $controller->validate();
}

/**
 * Submit handler for the create report form.
 */
function qui_report_form_submit(&$form, &$form_state) {
  /** @var $controller Qui_Query_Builder_Controller */
  $controller = $form_state['#controller'];
  $controller->form = $form;
  $controller->form_state = $form_state;
  $controller->submit();
}

/**
 * Returns the query results for preview in a table.
 *
 * Also returns the query itself and any associated error messages.
 *
 * @see Qui_Preview_Handler::generatePreview()
 */
function _qui_form_preview_results($form, &$form_state) {
  /** @var Qui_Query_Builder_Controller $controller */
  $controller = $form_state['#controller'];
  $controller->validate();
  return $controller->getHandler('qui_preview')->generatePreview();
}
