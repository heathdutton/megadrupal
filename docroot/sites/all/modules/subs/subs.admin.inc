<?php
/**
 * @file
 * Subs admin UI.
 */

/**
 * Form callback.
 * Return subs admin settings form.
 *
 * @see subs_menu()
 */
function subs_settings_form() {
  $form = array();

  $form['subs_workflow'] = array(
    '#type' => 'select',
    '#title' => t('Workflow'),
    '#description' => t('Decide how subscriptions relate to users.'),
    '#options' => array(
      SUBS_WORKFLOW_ONE => t('Only one active subscription allowed per user.'),
      SUBS_WORKFLOW_TYPE => t('Only one active subscription allowed per user and subscription type.'),
      SUBS_WORKFLOW_ANY => t('Allow any number of active subscriptions per user and subscription type.'),
    ),
    '#default_value' => variable_get('subs_workflow', SUBS_WORKFLOW_ONE),
  );

  $form['subs_dev'] = array(
    '#type' => 'checkbox',
    '#title' => t('Development mode'),
    '#description' => t('Development mode logs more watchdog entries such as debug statments whereas production will only log errors.'),
    '#default_value' => variable_get('subs_dev', TRUE),
  );

  return system_settings_form($form);
}

/**
 * Subscription Type UI Controller.
 */
class SubsTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage subscription types, including fields.';
    return $items;
  }

  /**
   * Builds the operation form.
   * This is used to display an additional warning when deleting subscription
   * types.
   * All other operations are passed through to the parent
   *
   * For the export operation a serialized string of the entity is directly
   * shown in the form (no submit function needed).
   */
  public function operationForm($form, &$form_state, $entity, $op) {
    if ($op === 'delete') {
      $confirm_question = t('Are you sure you want to delete the Subscription type %label?', array('%label' => $entity->getTranslation('label')));
      $description = t('All subscriptions of this type will be cancelled and deleted. This action cannot be undone.');
      return confirm_form($form, $confirm_question, $this->path, $description);
    }
    else {
      return parent::operationForm($form, $form_state, $entity, $op);
    }
  }

}

/**
 * Form callback.
 * Generates the subscription type edit form.
 */
function subs_type_form($form, &$form_state, $subscription_type, $op = 'edit') {
  // Store this for other modules wanting to alter this form and also for our own benefit.
  $form_state['subscription_type'] = $subscription_type;

  if ($op == 'clone') {
    $subscription_type->label .= ' (cloned)';
    $subscription_type->name = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $subscription_type->label,
    '#description' => t('The human-readable name of this subscription type.'),
    '#required' => TRUE,
    '#size' => 30,
    '#weight' => 0,
  );

  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($subscription_type->name) ? $subscription_type->name
        : '',
    '#maxlength' => 32,
    '#disabled' => $subscription_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'subs_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this subscription type. It must only contain lowercase letters, numbers, and underscores.'),
    '#weight' => 1,
  );

  $subscription_length = isset($subscription_type->length) ? $subscription_type->length : 0;
  $subscription_days = _subs_admin_secs_to_days($subscription_length);

  $form['length'] = array(
    '#title' => t('Length'),
    '#type' => 'textfield',
    '#description' => t('The length of this subscription type, in days.'),
    '#default_value' => $subscription_days,
    '#size' => 6,
    '#weight' => 2,
  );

  $grace_length = isset($subscription_type->grace) ? $subscription_type->grace : 0;
  $grace_days = _subs_admin_secs_to_days($grace_length);

  $form['grace'] = array(
    '#title' => t('Grace period length'),
    '#type' => 'textfield',
    '#description' => t("The length of this subscription type's grace period, in days."),
    '#default_value' => $grace_days,
    '#size' => 6,
    '#weight' => 3,
  );

  $form['default_status'] = array(
    '#title' => t('Default subscription status'),
    '#type' => 'select',
    '#options' => subs_status_options_list(),
    '#default_value' => isset($subscription_type->default_status) ? $subscription_type->default_status
        : SUBS_STATUS_PENDING,
    '#description' => t("The default status for new subscriptions of this type."),
    '#weight' => 4,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($subscription_type->description) ? $subscription_type->description
        : '',
    '#description' => t('A brief description of this subscription type.'),
    '#weight' => 6,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save subscription type'),
    '#weight' => 40,
  );

  if (!$subscription_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete subscription type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('subs_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Validation callback.
 * Handles validating subscription types.
 *
 * @see subs_type_form()
 */
function subs_type_form_validate(&$form, &$form_state) {
  // Length must be numeric.
  if (!is_numeric($form_state['values']['length'])) {
    form_set_error('length', t('Invalid subscription length.'));
  }
  // Length must be positive.
  elseif ($form_state['values']['length'] < 0) {
    form_set_error('length', t('Subscription length must be greater than or equal to zero.'));
  }
  // We're good, convert to seconds.
  else {
    form_set_value($form['length'], _subs_admin_days_to_secs($form_state['values']['length']), $form_state);
  }

  // Grace period must be numeric.
  if (!is_numeric($form_state['values']['grace'])) {
    form_set_error('grace', t('Invalid subscription grace period length.'));
  }
  // Grace period must be positive.
  elseif ($form_state['values']['grace'] < 0) {
    form_set_error('grace', t('Subscription grace period length must be greater than or equal to zero.'));
  }
  // We're good, convert to seconds.
  else {
    form_set_value($form['grace'], _subs_admin_days_to_secs($form_state['values']['grace']), $form_state);
  }

  // Default status must be valid.
  $options = subs_status_options_list();
  if (!array_key_exists($form_state['values']['default_status'], $options)) {
    form_set_error('default_status', t('Invalid default status.'));
  }
}

/**
 * Submit callback.
 * Handles saving subscription types.
 *
 * @see subs_type_form()
 */
function subs_type_form_submit(&$form, &$form_state) {
  $subscription_type = entity_ui_form_submit_build_entity($form, $form_state);
  if (subs_type_save($subscription_type)) {
    if ($form_state['op'] === 'edit') {
      drupal_set_message(t('Subscription type %label saved.', array('%label' => $subscription_type->getTranslation('label'))));
    }
    elseif ($form_state['op'] === 'add') {
      drupal_set_message(t('Subscription type %label created.', array('%label' => $subscription_type->getTranslation('label'))));
    }
  }
  else {
    drupal_set_message(t('Error saving Subscription type %label.', array('%label' => $subscription_type->getTranslation('label'))), 'error');
  }

  $form_state['redirect'] = 'admin/structure/subscriptions';
}

/**
 * Submit callback.
 * Handles deleting subscription types.
 *
 * @see subs_type_form()
 */
function subs_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/subscriptions/manage/' . $form_state['subs_type']->name . '/delete';
}

/**
 * Helper function.
 * Converts seconds to days.
 *
 * @param int
 *   The number of seconds to convert.
 * @return int
 *   The number of corresponding days.
 */
function _subs_admin_secs_to_days($seconds) {
  // 1 day = 86400 seconds.
  return floor($seconds / 86400);
}

/**
 * Helper function.
 * Converts days to seconds.
 *
 * @param int
 *   The number of days to convert.
 * @return int
 *   The number of corresponding seconds.
 */
function _subs_admin_days_to_secs($days) {
  // 1 day = 86400 seconds.
  return $days * 86400;
}

/**
 * Page callback.
 * Lists all subscriptions.
 *
 * @param $account
 *   Optinally, list subscriptions for a specific account.
 *
 * @see subs_menu()
 */
function subs_list($account = NULL) {
  drupal_add_css(drupal_get_path('module', 'subs') . '/subs.admin.css', array('GROUP' => CSS_SYSTEM));

  // Build the sortable table header.
  $header = array(
    'user' => array(
      'data' => 'User',
      'type' => 'property',
      'specifier' => 'uid',
    ),
    'type' => array(
      'data' => 'Type',
      'type' => 'entity',
      'specifier' => 'bundle',
    ),
    'status' => array(
      'data' => 'Status',
      'type' => 'property',
      'specifier' => 'status',
    ),
    'date_start' => array(
      'data' => 'Start Date',
      'type' => 'property',
      'specifier' => 'date_start',
    ),
    'date_end' => array(
      'data' => 'End Date',
      'type' => 'property',
      'specifier' => 'date_end',
    ),
    'changed' => array(
      'data' => 'Updated',
      'type' => 'property',
      'specifier' => 'changed',
      'sort' => 'desc',
    ),
    'operations' => array('data' => t('Operations')),
  );

  $filters = subs_get_filter();

  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'subs')
    ->tableSort($header)
    ->pager($filters['per_page']);

  if ($account) {
    $uid = is_object($account) ? $account->uid : $account;
    $query->propertyCondition('uid', $uid, '=');
    // No need to specify the obvious.
    unset($header['user']);
  }

  if (!empty($filters['type'])) {
    $query->entityCondition('bundle', $filters['type'], 'IN');
  }

  if (!empty($filters['status'])) {
    $query->propertyCondition('status', $filters['status'], 'IN');
  }

  $result = $query->execute();

  if (!empty($result)) {
    $subscriptions = subs_load_multiple(array_keys($result['subs']));
  }
  else {
    $subscriptions = array();
  }

  $destination = drupal_get_destination();
  $subscription_types = subs_get_types();
  $status_options = subs_status_options_list();

  $subs_content = array(
    '#theme' => 'tableselect',
    '#header' => $header,
    '#options' => array(),
    '#empty' => t('There are no subscriptions available.'),
    '#js_select' => TRUE,
    '#attributes' => array(),
  );

  foreach ($subscriptions as $subscription) {
    $subs_content[$subscription->sid] = array(
      '#type' => 'checkbox',
      '#title' => '',
    );

    $subs_content['#options'][$subscription->sid] = array();

    if (!$account) {
      $user = user_load($subscription->uid);
      $subs_content['#options'][$subscription->sid] += array(
        'user' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $user->name,
            '#href' => 'user/' . $subscription->uid,
          ),
        ),
      );
    }

    $subs_content['#options'][$subscription->sid] += array(
      'type' => $subscription_types[$subscription->type]->getTranslation('label'),
      'status' => $status_options[$subscription->status],
      'date_start' => $subscription->date_start > 0 ? format_date($subscription->date_start, 'subs')
          : t('None'),
      'date_end' => $subscription->date_end > 0 ? format_date($subscription->date_end, 'subs')
          : t('None'),
      'changed' => format_date($subscription->changed, 'short'),
    );

    // Build a list of operations.
    $operations = array();

    $operations['view'] = array(
      'title' => t('view'),
      'href' => 'subscription/' . $subscription->sid,
      'query' => $destination,
    );

    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'subscription/' . $subscription->sid . '/edit',
      'query' => $destination,
    );

    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => 'subscription/' . $subscription->sid . '/delete',
      'query' => $destination,
    );

    // Render an unordered list of operations links.
    $subs_content['#options'][$subscription->sid]['operations'] = array(
      'data' => array(
        '#theme' => 'links__subs_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  $output = array(
    'subs_filter_form' => drupal_get_form('subs_filter_form'),
    'subs_operations' => drupal_get_form('subs_operations_form', $subs_content),
    'pager' => array(
      '#theme' => 'pager',
    ),
  );

  return $output;
}

/**
 * Returns subscription filters.
 *
 * @return array
 *   An array keyed by filter name.
 *
 * @see subs_filter_form()
 */
function subs_get_filter() {
  // Pull any saved values from the session.
  $filters = isset($_SESSION['subs-filter']) ? $_SESSION['subs-filter'] : array();

  // Fill with defaults where necessary.
  if (!isset($filters['type'])) {
    $filters['type'] = array_keys(subs_get_types());
  }

  if (!isset($filters['status'])) {
    $filters['status'] = array_keys(subs_status_options_list());
  }

  if (!isset($filters['per_page'])) {
    $filters['per_page'] = 50;
  }

  return $filters;
}

/**
 * Sets subscription filters.
 *
 * @see subs_filter_submit()
 * @see subs_filter_form()
 */
function subs_set_filter($filter) {
  $_SESSION['subs-filter'] = $filter;
}

/**
 * Resets subscription filters.
 *
 * @see subs_filter_submit_reset()
 * @see subs_filter_form()
 */
function subs_reset_filter() {
  unset($_SESSION['subs-filter']);
}

/**
 * Form callback.
 * Displays filter form for listing subscriptions.
 *
 * @see subs_list()
 */
function subs_filter_form($form, &$form_state) {
  $filters = subs_get_filter();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => !isset($_SESSION['subs-filter']),
  );

  $type_options = array();
  foreach (subs_get_types() as $type) {
    $type_options[$type->name] = $type->getTranslation('label');
  }
  asort($type_options);

  $form['filters']['type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Type'),
    '#multiple' => TRUE,
    '#options' => $type_options,
    '#default_value' => $filters['type'],
  );

  $form['filters']['status'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Status'),
    '#multiple' => TRUE,
    '#options' => subs_status_options_list(),
    '#default_value' => $filters['status'],
  );

  $form['filters']['per_page'] = array(
    '#type' => 'select',
    '#title' => t('Items Per Page'),
    '#multiple' => FALSE,
    '#options' => array(20 => '20', 50 => '50', 100 => '100', 200 => '200', 500 => '500'),
    '#default_value' => $filters['per_page'],
  );

  $form['filters']['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('subs_filter_submit'),
  );
  $form['filters']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('subs_filter_submit_reset'),
  );

  return $form;
}

/**
 * Submit callback.
 * Applies filters.
 *
 * @see subs_filter_form()
 */
function subs_filter_submit($form, &$form_state) {
  $filters = array(
    'type' => $form_state['values']['filters']['type'],
    'status' => $form_state['values']['filters']['status'],
    'per_page' => $form_state['values']['filters']['per_page'],
  );

  subs_set_filter($filters);
  drupal_goto('admin/content/subscription');
}

/**
 * Submit callback.
 * Resets filters.
 *
 * @see subs_filter_form()
 */
function subs_filter_submit_reset($form, &$form_state) {
  subs_reset_filter();
  drupal_goto('admin/content/subscription');
}

/**
 * Form callback.
 * Displays buttons operations.
 *
 * @see subs_list()
 */
function subs_operations_form($form, &$form_state, $subs_content) {
  $form['operations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Operations'),
    '#description' => t('Execute operations on the checked subscriptions.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['operations']['action'] = array(
    '#type' => 'select',
    '#options' => array(
      '' => t('- Choose an operation -'),
      'pending' => t('Set pending'),
      'active' => t('Set active'),
      'grace' => t('Set to grace period'),
      'renew' => t('Renew'),
      'cancel' => t('Cancel'),
      'delete' => t('Delete'),
    ),
  );

  $form['operations']['execute'] = array(
    '#type' => 'submit',
    '#value' => t('Execute'),
    '#disabled' => (empty($subs_content['#options'])) ? TRUE : FALSE,
  );

  // Define a parent wrapper element.
  $form['subs_table'] = array(
    '#type' => 'item',
    '#prefix' => '<div class="subs-content-table">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  $form['subs_table']['subs_content'] = $subs_content;

  return $form;
}

/**
 * Validate callback.
 *
 * @see subs_operations_form()
 */
function subs_operations_form_validate(&$form, &$form_state) {
  $sids = array();

  foreach ($form_state['values']['subs_table']['subs_content'] as $sid => $check) {
    if ($check) {
      $sids[] = $sid;
    }
  }

  if (empty($sids)) {
    form_set_error('subs_table][subs_content', t('You must select at least one subscription.'));
    return;
  }

  $form_state['sids_selected'] = $sids;
}

/**
 * Submit callback.
 *
 * @see subs_operations_form()
 */
function subs_operations_form_submit(&$form, &$form_state) {
  $sids = $form_state['sids_selected'];

  // @TODO add a confirmation step.
  switch ($form_state['values']['action']) {
    case 'active':
      foreach ($sids as $sid) {
        if ($subscription = subs_load($sid)) {
          subs_set_active($subscription);
        }
      }
      break;
    case 'pending':
      foreach ($sids as $sid) {
        if ($subscription = subs_load($sid)) {
          subs_set_pending($subscription);
        }
      }
      break;
    case 'grace':
      foreach ($sids as $sid) {
        if ($subscription = subs_load($sid)) {
          subs_set_grace($subscription);
        }
      }
      break;
    case 'renew':
      foreach ($sids as $sid) {
        if ($subscription = subs_load($sid)) {
          subs_renew($subscription);
        }
      }
      break;
    case 'cancel':
      foreach ($sids as $sid) {
        if ($subscription = subs_load($sid)) {
          subs_set_cancelled($subscription);
        }
      }
      break;
    case 'delete':
      foreach ($sids as $sid) {
        if ($subscription = subs_load($sid)) {
          subs_delete($subscription);
        }
      }
      break;
  }

  drupal_set_message(t('Operation "@op" has been executed successfully.', array('@op' => $form_state['values']['action'])));
}