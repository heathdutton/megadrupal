<?php
/**
 * @file
 * admin function for opac authorities module.
 */

/**
 * Generates the authority type editing form.
 */
function opac_authority_type_form($form, &$form_state, $authority_type, $op = 'edit') {
  if ($op == 'clone') {
    $authority_type->label .= ' (cloned)';
    $authority_type->authtype = '';
  }

  $form['label'] = array(
    '#title' => t('Label', array(), array('context' => 'libraries')),
    '#type' => 'textfield',
    '#default_value' => $authority_type->label,
    '#description' => t('The human-readable name of this authority type.', array(), array('context' => 'libraries')),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['authtype'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($authority_type->authtype) ? $authority_type->authtype : '',
    '#maxlength' => 32,
    '#disabled' => $authority_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'opac_authority_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this authority type. It must only contain lowercase letters, numbers, and underscores.', array(), array('context' => 'libraries')),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($authority_type->description) ? $authority_type->description : '',
    '#description' => t('Description about the authority type.', array(), array('context' => 'libraries')),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save authority type', array(), array('context' => 'libraries')),
    '#weight' => 40,
  );

  if (!$authority_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete authority type', array(), array('context' => 'libraries')),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('opac_authority_type_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Submit handler for creating/editing authority type.
 */
function opac_authority_type_form_submit(&$form, &$form_state) {
  $authority_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  opac_authority_type_save($authority_type);

  // Redirect user back to list of authority types.
  $form_state['redirect'] = 'admin/structure/authority-types';
}

/**
 * Page to select authority type to add new authority.
 */
function opac_authority_admin_add_page() {
  $items = array();
  foreach (opac_authority_types() as $type_key => $authority_type) {
    $items[] = l(entity_label('opac_authority_type', $authority_type), 'opac_authority/add/' . $type_key);
  }

  if (!count($items)) {
    return t('No authority types yet.', array(), array('context' => 'libraries'));
  }
  return array(
    'list' => array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#title' => t('Select type of authority to create.', array(), array('context' => 'libraries')),
    ),
  );
}

/**
 * Add new authority page callback.
 */
function opac_authority_add($type) {
  $authority_type = opac_authority_types($type);

  $authority = entity_create('opac_authority', array('authtype' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('opac_authority_type', $authority_type))));

  $output = drupal_get_form('opac_authority_form', $authority);

  return $output;
}

/**
 * Authority Form.
 */
function opac_authority_form($form, &$form_state, $authority) {
  $form_state['authority'] = $authority;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#default_value' => $authority->title,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $authority->uid,
  );

  field_attach_form('opac_authority', $authority, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save authority', array(), array('context' => 'libraries')),
    '#submit' => $submit + array('opac_authority_form_submit'),
  );

  // Show Delete button if we edit authority.
  $authid = entity_id('opac_authority', $authority);
  if (!empty($authid) && user_access('edit opac_authority entities')) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('opac_authority_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function opac_authority_form_submit($form, &$form_state) {
  $authority = $form_state['authority'];

  entity_form_submit_build_entity('opac_authority', $authority, $form, $form_state);

  opac_authority_save($authority);

  $uri = entity_uri('opac_authority', $authority);

  $form_state['redirect'] = $uri['path'];

  drupal_set_message(t('Authority %title saved.', array('%title' => entity_label('opac_authority', $authority))));
}

/**
 * Submit handler.
 */
function opac_authority_form_submit_delete($form, &$form_state) {
  $authority = $form_state['authority'];
  $uri = entity_uri('opac_authority', $authority);
  $form_state['redirect'] = $uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function opac_authority_delete_form($form, &$form_state, $authority) {
  $form_state['authority'] = $authority;
  $form['authid'] = array(
    '#type' => 'value',
    '#value' => entity_id('opac_authority', $authority),
  );
  $uri = entity_uri('opac_authority', $authority);
  return confirm_form($form,
    t('Are you sure you want to delete authority %title?', array('%title' => entity_label('opac_authority', $authority))),
    $uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function opac_authority_delete_form_submit($form, &$form_state) {
  $authority = $form_state['authority'];
  opac_authority_delete($authority);

  drupal_set_message(t('Authority %title deleted.', array('%title' => entity_label('opac_authority', $authority))));

  $form_state['redirect'] = '<front>';
}

/**
 * Delete form submit handler.
 */
function opac_authorities_multiple_delete_confirm($form, &$form_state, $authorities) {
  $form['authorities'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );
  // Returns only elements with TRUE values.
  foreach ($authorities as $authid => $value) {
    $title = db_query('SELECT title FROM {opac_authority} WHERE authid = :authid', array(':authid' => $authid))->fetchField();
    $form['authorities'][$authid] = array(
      '#type' => 'hidden',
      '#value' => $authid,
      '#prefix' => '<li>',
      '#suffix' => check_plain($title) . "</li>\n",
    );
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'opac_authorities_multiple_delete_confirm_submit';
  $confirm_question = format_plural(count($authorities),
    'Are you sure you want to delete this item?',
    'Are you sure you want to delete these items?');
  return confirm_form($form,
    $confirm_question,
    'admin/authorities', t('This action cannot be undone.'),
    t('Delete'), t('Cancel'));
}

/**
 * Form submit for opac_authorities_multiple_delete_confirm.
 *
 * Process authorities deleting.
 */
function opac_authorities_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    opac_authority_delete_multiple(array_keys($form_state['values']['authorities']));
    $count = count($form_state['values']['authorities']);
    watchdog('opac_authorities', 'Deleted @count authorities.', array('@count' => $count));
    drupal_set_message(format_plural($count, 'Deleted 1 authority.', 'Deleted @count authorities.'));
  }
  $form_state['redirect'] = 'admin/authorities';
}

/**
 * Menu callback: authorities administration.
 */
function opac_authorities_admin($form, $form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return opac_authorities_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['authorities']));
  }
  $form['filter'] = opac_authorities_filter_form();
  $form['#submit'][] = 'opac_authorities_filter_form_submit';
  $form['admin'] = opac_authorities_admin_authorities();

  return $form;
}

/**
 * Form builder: Builds the authorities administration overview.
 */
function opac_authorities_admin_authorities() {
  $admin_access = user_access('administer opac_authority entities');

  // Build the 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
    '#access' => $admin_access,
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => array(
      'none' => t('Select an operation'),
      'delete' => t('delete selected items'),
    ),
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('opac_authorities_admin_authorities_validate'),
    '#submit' => array('opac_authorities_admin_authorities_submit'),
  );

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'a.title'),
    'type' => array('data' => t('Authority type'), 'field' => 'a.authtype'),
    'author' => t('Author'),
    'changed' => array(
      'data' => t('Updated'),
      'field' => 'a.changed',
      'sort' => 'desc',
    ),
  );
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('opac_authority', 'a')->extend('PagerDefault')->extend('TableSort');
  $filter_data = isset($_SESSION['opac_authorities_overview_filter']) ? $_SESSION['opac_authorities_overview_filter'] : array();
  foreach ($filter_data as $index => $filter) {
    list($key, $value) = $filter;
    $query->condition('a.' . $key, $value);
  }

  $authids = $query
    ->fields('a', array('authid'))
    ->limit(50)
    ->orderByHeader($header)
    ->execute()
    ->fetchCol();
  $authorities = opac_authority_load_multiple($authids);

  // Prepare the list of authorities.
  $destination = drupal_get_destination();
  $options = array();
  foreach ($authorities as $authority) {
    $options[$authority->authid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $authority->title,
          '#href' => 'opac_authority/' . $authority->authid,
          '#options' => array(),
        ),
      ),
      'type' => check_plain($authority->authtype),
      'author' => theme('username', array('account' => $authority)),
      'changed' => format_date($authority->changed, 'short'),
    );
    // Build a list of all the accessible operations for the current authority.
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'opac_authority/' . $authority->authid . '/edit',
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => 'opac_authority/' . $authority->authid . '/delete',
      'query' => $destination,
    );
    $options[$authority->authid]['operations'] = array(
      'data' => array(
        '#theme' => 'links__node_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  // Only use a tableselect when the current user is able to perform any
  // operations.
  if ($admin_access) {
    $form['authorities'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No authority available.'),
    );
  }
  // Otherwise, use a simple table.
  else {
    $form['authorities'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#empty' => t('No authority available.'),
    );
  }

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Validate opac_authorities_admin_authorities form submissions.
 *
 * Check if any authorities have been selected to perform the chosen
 * 'Update option' on.
 */
function opac_authorities_admin_authorities_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['authorities']) || !count(array_filter($form_state['values']['authorities']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Process opac_authorities_admin_authorities form submissions.
 *
 * Execute the chosen 'Update option' on the selected authorities.
 */
function opac_authorities_admin_authorities_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Return a filter form.
 */
function opac_authorities_filter_form() {
  $session = isset($_SESSION['opac_authorities_overview_filter']) ? $_SESSION['opac_authorities_overview_filter'] : array();
  $filters = opac_authorities_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    $value = $filters[$type]['options'][$value];
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
  }
  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => check_plain($filter['title']),
      '#default_value' => '[any]',
    );
  }
  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Process result from authorities administration filter form.
 */
function opac_authorities_filter_form_submit($form, &$form_state) {
  $filters = opac_authorities_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a
      // choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate
          // hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on
          // the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['opac_authorities_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;

    case t('Undo'):
      array_pop($_SESSION['opac_authorities_overview_filter']);
      break;

    case t('Reset'):
      $_SESSION['opac_authorities_overview_filter'] = array();
      break;

  }
}

/**
 * Return available filters.
 */
function opac_authorities_filters() {
  $opac_types = opac_authority_types();
  $options = array('[any]' => t('any'));
  foreach ($opac_types as $name => $type) {
    $options[$name] = $type->label;
  }
  return array(
    'authtype' => array(
      'title' => t('type'),
      'options' => $options,
    ),
  );
}
