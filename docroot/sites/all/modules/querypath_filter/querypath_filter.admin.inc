<?php
/**
 * @file
 * Provides administration tasks for QueryPath filter module.
 *
 * Contains all forms and lists to manage the QueryPath rules.
 */

/**
 * Form for editing all the QueryPath rules at once.
 *
 * Manage rules weight and status.
 *
 * @see querypath_filter_overview_form_submit()
 * @ingroup forms
 */
function querypath_filter_overview_form($form, &$form_state) {
  $form = array();
  $form['#tree'] = TRUE;

  // Get all rules.
  foreach (querypath_filter_get_rules() as $rule) {
    $form['rules'][$rule->rid]['#item'] = $rule;
    $form['rules'][$rule->rid]['status-' . $rule->rid] = array(
      '#type' => 'checkbox',
      '#default_value' => $rule->status,
    );
    $form['rules'][$rule->rid]['weight-' . $rule->rid] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $rule->weight,
      '#delta' => 100,
    );
  }
  if (!empty($form)) {
    $form['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array(),
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['#empty_text'] = t('There are no QueryPath rules yet. <a href="@link">Add rule</a>.', array('@link' => url('admin/config/content/querypath_filter/rule/add')));
  }
  return $form;
}

/**
 * Submit handler for the QueryPath filter overview form.
 *
 * @see querypath_filter_overview_form()
 */
function querypath_filter_overview_form_submit($form, &$form_state) {
  $updated_items = array();

  foreach (element_children($form['rules']) as $rid) {
    $rule = $form['rules'][$rid]['#item'];

    // Check status.
    if ($rule->status != $form_state['values']['rules'][$rid]['status-' . $rid]) {
      $rule->status = $form_state['values']['rules'][$rid]['status-' . $rid];
      $updated_items[$rid] = $rule;
    }
    // Check weight.
    if ($rule->weight != $form_state['values']['rules'][$rid]['weight-' . $rid]) {
      $rule->weight = $form_state['values']['rules'][$rid]['weight-' . $rid];
      $updated_items[$rid] = $rule;
    }
  }
  // Save all our changed items to the database.
  foreach ($updated_items as $item) {
    querypath_filter_rule_save($item);
  }
  drupal_set_message(t('Your configuration has been saved.'));
}

/**
 * Form for editing an entire menu tree at once.
 *
 * Shows for one menu the menu links accessible to the current user and
 * relevant operations.
 *
 * @see querypath_filter_admin_rule_form_validate()
 * @see querypath_filter_admin_rule_form_submit()
 * @ingroup forms
 */
function querypath_filter_admin_rule_form($form, &$form_state, $rule = NULL) {
  $form = array();

  // Add css and js files for this form.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'querypath_filter') . '/querypath_filter.css',
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'querypath_filter') . '/querypath_filter.js',
  );


  // If this a new rule, initialize rule object.
  if (is_null($rule)) {
    $rule = _querypath_filter_rule_init();
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => $rule->title,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#maxlength' => 255,
    '#default_value' => $rule->description,
  );
  $form['qp_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Selector'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => $rule->qp_selector,
  );
  $form['qp_action'] = array(
    '#type' => 'select',
    '#title' => t('Action'),
    '#options' => _querypath_filter_get_actions(),
    '#required' => TRUE,
    '#default_value' => $rule->qp_action,
  );
  $form['qp_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#maxlength' => 255,
    '#default_value' => $rule->qp_value,
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => $rule->status,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 100,
    '#default_value' => is_null($rule->weight) ? 0 : $rule->weight,
  );

  $form['preview'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="querypath-filter-preview" class="qp-filter-preview"><strong>' . t('Preview') . ':</strong>',
    '#markup' => '<span>' . htmlentities(querypath_filter_format_rule($rule)) . '</span>',
    '#suffix' => '</div>',
    '#attributes' => array(
      'class' => array('qp-filter-preview'),
    ),
  );

  $form['rule'] = array(
    '#type' => 'value',
    '#value' => $rule,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#access' => $rule,
    '#submit' => array('querypath_filter_item_delete_submit'),
    '#value' => t('Delete'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/config/content/querypath_filter'),
  );
  return $form;
}

/**
 * Submit handler for the QueryPath filter rule form.
 *
 * @see querypath_filter_admin_rule_form()
 */
function querypath_filter_admin_rule_form_submit($form, &$form_state) {
  $rule = $form_state['values']['rule'];

  // Assign new values and save object.
  $rule->title = $form_state['values']['title'];
  $rule->description = $form_state['values']['description'];
  $rule->qp_selector = $form_state['values']['qp_selector'];
  $rule->qp_action = $form_state['values']['qp_action'];
  $rule->qp_value = $form_state['values']['qp_value'];
  $rule->status = $form_state['values']['status'];
  $rule->weight = $form_state['values']['weight'];
  if (querypath_filter_rule_save($rule)) {
    drupal_set_message(t('Rule saved.'));
  }
  else {
    drupal_set_message(t('Error while saving rule. Please contact site administrators'), 'error');
  }
  drupal_goto('admin/config/content/querypath_filter');
}

/**
 * Submit function for the delete button on the QueryPath rule editing form.
 */
function querypath_filter_item_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/content/querypath_filter/rule/' . $form_state['values']['rule']->rid . '/delete';
}

/**
 * Build a confirm form for deletion of a single QueryPath rule.
 */
function querypath_filter_item_delete_form($form, &$form_state, $item) {
  $form['#item'] = $item;
  return confirm_form($form, t('Are you sure you want to delete the QueryPath rule %item?', array('%item' => $item->title)), 'admin/config/content/querypath_filter');
}

/**
 * Process QueryPath rule delete form submissions.
 */
function querypath_filter_item_delete_form_submit($form, &$form_state) {
  $item = $form['#item'];
  $t_args = array('%title' => $item->title);
  if (querypath_filter_rule_delete($item)) {
    drupal_set_message(t('The QueryPath rule %title has been deleted.', $t_args));
    watchdog('querypath_filter', 'Deleted QueryPath rule %title.', $t_args, WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('The QueryPath rule %title could not be deleted.', $t_args));
    watchdog('querypath_filter', 'Could not delete QueryPath rule %title.', $t_args, WATCHDOG_ERROR, l(t('view'), 'admin/config/content/querypath_filter/rule/' . $item->id . '/edit'));
  }
  $form_state['redirect'] = 'admin/config/content/querypath_filter';
}

/**
 * Returns the title to be used on forms.
 *
 * @param object $rule
 *   Rule object
 * @param string $type
 *   Form type edit|delete
 *
 * @return string
 *   Form title
 */
function _querypath_filter_form_title($rule, $type) {
  switch ($type) {
    case 'edit':
      return t('Edit rule %rule', array('%rule' => $rule->title));
      break;

    case 'delete':
      return t('Delete rule %rule', array('%rule' => $rule->title));
      break;
  }
  return t('Rule %rule', array('%rule' => $rule->title));
}

/**
 * Returns HTML for the querypath_filter overview form into a table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_querypath_filter_overview_form($variables) {
  $output = '';
  $form = $variables['form'];

  drupal_add_tabledrag('qp-filter-rules', 'order', 'self', 'qp-filter-rule-weight', NULL, NULL, TRUE);

  $headers = array(
    t('Rule'),
    t('Rule details'),
    t('Enabled'),
    t('Weight'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );

  $rows = array();
  foreach (element_children($form['rules']) as $rid) {
    $rule = $form['rules'][$rid]['#item'];

    // Add special classes to be used for tabledrag.js.
    $form['rules'][$rid]['weight-' . $rid]['#attributes']['class'] = array('qp-filter-rule-weight');

    $row = array(
      'data' => array(
        check_plain($rule->title) . '<br /><em>' . check_plain($rule->description) . '</em>',
        check_plain(querypath_filter_format_rule($rule)),
        drupal_render($form['rules'][$rid]['status-' . $rid]),
        drupal_render($form['rules'][$rid]['weight-' . $rid]),
        l(t('edit'), 'admin/config/content/querypath_filter/rule/' . $rule->rid . '/edit'),
        l(t('delete'), 'admin/config/content/querypath_filter/rule/' . $rule->rid . '/delete'),
      ),
      'class' => array('querypath-filter-rule', 'draggable'),
    );
    $rows[] = $row;
  }

  $output .= theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'qp-filter-rules')));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Returns a formatted QueryPath rule, similar to jQuery.
 *
 * Be sure to use text filtering where necessary.
 * (check_plain()|check_markup()|filter_xss())
 *
 * @param object $rule
 *   Rule object
 *
 * @return string
 *   Rule snippet
 */
function querypath_filter_format_rule($rule) {
  $output = '';
  $output .= "$('" . $rule->qp_selector . "')";
  if (!empty($rule->qp_action)) {
    $output .= '.' . $rule->qp_action . '(';
    empty($rule->qp_value) ? '' : $output .= "'" . $rule->qp_value . "'";
    $output .= ')';
  }
  $output .= ';';
  return $output;
}
