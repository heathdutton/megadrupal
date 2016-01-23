<?php
/**
 * @file
 * Access grants administrative UI for the access control kit module.
 */

/**
 * Menu page callback; the access grants overview page.
 */
function access_overview_grants($form, $form_state) {
  // Check for the delete confirmation form.
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return access_grant_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['grants']));
  }
  $form['filter'] = access_admin_grants_filter_form();
  $form['#submit'][] = 'access_admin_grants_filter_form_submit';
  $form['admin'] = access_admin_grants();

  return $form;
}

/**
 * Form constructor for the grant admin form.
 *
 * @see access_admin_grants_validate()
 * @see access_admin_grants_submit()
 */
function access_admin_grants() {
  // Build the delete button.
  $form['actions'] = array('#type' => 'actions', '#weight' => -10);
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete selected grants'),
    '#validate' => array('access_admin_grants_validate'),
    '#submit' => array('access_admin_grants_submit'),
  );

  // Build the sortable table header.
  $header = array(
    'username' => array(
      'data' => t('Username'),
      'field' => 'u.name',
    ),
    'role' => array(
      'data' => t('Role'),
      'field' => 'r.name',
    ),
    'realms' => t('Realms'),
    'scheme' => array(
      'data' => t('Scheme'),
      'field' => 's.name',
    ),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('access_grant', 'g')
    ->extend('PagerDefault')
    ->extend('TableSort');
  access_admin_grants_build_filter_query($query);

  $query->join('users', 'u', 'g.uid = u.uid');
  $query->join('role', 'r', 'g.rid = r.rid');
  $query->join('access_scheme', 's', 'g.scheme = s.scheme');
  $query->addField('g', 'gid', 'gid');
  $query->addField('r', 'name', 'role');
  $query->addField('s', 'name', 'scheme');
  $result = $query
    ->fields('u', array('uid', 'name', 'status', 'created', 'access'))
    ->limit(50)
    ->orderByHeader($header)
    ->execute()
    ->fetchAllAssoc('gid');
  $grants = access_grant_load_multiple(array_keys($result));

  // Prepare the list of grants.
  $destination = drupal_get_destination();
  $options = array();
  foreach ($result as $gid => $fields) {
    // Get the realms list, truncated for length.
    $realms = implode(', ', access_grant_get_realms($grants[$gid]));
    if (drupal_strlen($realms) > 30) {
      $realms = drupal_substr($realms, 0, 25) . '...';
    }

    // Add the main columns.
    $options[$gid] = array(
      'username' => theme('username', array('account' => $fields)),
      'role' => check_plain($fields->role),
      'realms' => $realms,
      'scheme' => check_plain($fields->scheme),
    );

    // Add the operations links.
    $url = 'admin/access/grant/' . $gid;
    $operations = array();
    $operations['view'] = array(
      'title' => t('view'),
      'href' => $url,
    );
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => $url . '/edit',
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => $url . '/delete',
      'query' => $destination,
    );
    $options[$gid]['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  // Format as a table with pager.
  $form['grants'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No access grants available.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

/**
 * Form validation handler for access_admin_grants().
 *
 * @see access_admin_grants_submit()
 */
function access_admin_grants_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['grants']) || !count(array_filter($form_state['values']['grants']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Form submission handler for access_admin_grants().
 *
 * @see access_admin_grants_validate()
 */
function access_admin_grants_submit($form, &$form_state) {
  // Filter out unchecked grants.
  $grants = array_filter($form_state['values']['grants']);

  if ($form_state['values']['op'] == t('Delete selected grants')) {
    $form_state['values']['operation'] = 'delete';
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Form constructor for the access grant administration filter form.
 */
function access_admin_grants_filter_form() {
  $session = isset($_SESSION['access_overview_grant_filter']) ? $_SESSION['access_overview_grant_filter'] : array();
  $filters = access_admin_grants_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only access grants where'),
    '#theme' => 'exposed_filters__access_grant',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if (isset($filters[$type]['options'])) {
      $value = $filters[$type]['options'][$value];
    }
    else {
      $value = check_plain($value);
    }
    $t_args = array(
      '%property' => $filters[$type]['title'],
      '%value' => $value,
    );
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    unset($filters[$type]);
  }

  $form['filters']['available'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['available']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    if (isset($filter['options'])) {
      $form['filters']['available']['filters'][$key] = array(
        '#type' => 'select',
        '#options' => $filter['options'],
        '#title' => $filter['title'],
        '#default_value' => '[any]',
      );
    }
    else {
      $form['filters']['available']['filters'][$key] = array(
        '#type' => 'textfield',
        '#size' => '32',
        '#title' => $filter['title'],
      );
    }
  }

  $form['filters']['available']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $available = element_children($form['filters']['available']['filters']);
  if (!empty($available)) {
    $form['filters']['available']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => count($session) ? t('Refine') : t('Filter'),
    );
  }
  if (count($session)) {
    $form['filters']['available']['actions']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
    );
    $form['filters']['available']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Form submission handler for access_admin_grants_filter_form().
 */
function access_admin_grants_filter_form_submit($form, &$form_state) {
  $filters = access_admin_grants_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter])) {
          if (!isset($filters[$filter]['options'])) {
            // Filters lacking an 'options' key are text input.
            $_SESSION['access_overview_grant_filter'][] = array($filter, $form_state['values'][$filter]);
          }
          elseif ($form_state['values'][$filter] != '[any]') {
            // Flatten the options array to accommodate hierarchical/nested options.
            $flat_options = form_options_flatten($filters[$filter]['options']);
            // Only accept valid selections offered on the dropdown, block bad input.
            if (isset($flat_options[$form_state['values'][$filter]])) {
              $_SESSION['access_overview_grant_filter'][] = array($filter, $form_state['values'][$filter]);
            }
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['access_overview_grant_filter']);
      break;
    case t('Reset'):
      $_SESSION['access_overview_grant_filter'] = array();
      break;
  }
}

/**
 * Lists access grant administration filters that can be applied.
 *
 * @return
 *   Returns a list of filter definitions for the grant administration form.
 *   Each key is filter identifier for access_admin_grants_build_filter_query(),
 *   and each value is an array with the following keys:
 *   - 'title': The translated title of the filter, to be shown on the form.
 *   - 'options': (optional) A list of options to populate the filter as a
 *     select field. If omitted, the filter will be rendered as a text field.
 */
function access_admin_grants_filters() {
  // By role.
  $filters['rid'] = array(
    'title' => t('role'),
    'options' => array('[any]' => t('any')) + access_realm_roles(),
  );

  // By scheme.
  $filters['scheme'] = array(
    'title' => t('scheme'),
    'options' => array('[any]' => t('any')) + access_scheme_get_names(),
  );

  // By username.
  $filters['username'] = array(
    'title' => t('username'),
  );

  return $filters;
}

/**
 * Apply filters for the access grant administration page based on session.
 *
 * @param $query
 *   A SelectQuery to which the filters should be applied.
 */
function access_admin_grants_build_filter_query(SelectQueryInterface $query) {
  $filter_data = isset($_SESSION['access_overview_grant_filter']) ? $_SESSION['access_overview_grant_filter'] : array();
  foreach ($filter_data as $index => $filter) {
    list($key, $value) = $filter;
    switch ($key) {
      case 'rid':
      case 'scheme':
        $query->condition('g.' . $key, $value);
        break;
      case 'username':
        $query->condition('u.name', $value);
        break;
    }
  }
}

/**
 * Menu page callback; the access grant add list page.
 */
function access_grant_add_list() {
  $schemes = access_scheme_get_schemes();
  $content = array();

  foreach ($schemes as $key => $scheme) {
    $content[$key] = array(
      'title' => $scheme->name,
      'description' => $scheme->description,
      'href' => 'admin/access/add/' . str_replace('_', '-', $scheme->scheme),
      'localized_options' => array(),
    );
  }

  if (empty($content)) {
    return t('You have not created any access schemes yet. Go to the <a href="@create-scheme">access scheme creation page</a> to add a new access scheme.', array('@create-scheme' => url('admin/structure/access/add')));
  }
  // Bypass this listing page if only one scheme is available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  return theme('admin_block_content', array('content' => $content));
}

/**
 * Menu page callback; add an access grant for a given scheme.
 *
 * @param $scheme
 *   The access scheme in which to create a new grant.
 */
function access_grant_add($scheme) {
  $grant = (object) array('scheme' => $scheme->scheme);
  return drupal_get_form('access_grant_form', $grant);
}

/**
 * Menu page callback; edit an access grant.
 *
 * @param $grant
 *   The access grant to edit.
 */
function access_grant_edit($grant) {
  $scheme = access_scheme_get_scheme($grant);
  drupal_set_title(t('<em>Edit @scheme for</em> @grant', array('@scheme' => $scheme->name, '@grant' => access_grant_label($grant))), PASS_THROUGH);
  return drupal_get_form('access_grant_form', $grant);
}

/**
 * Form constructor for the access grant add/edit form.
 *
 * @param $grant
 *   The access grant to edit.
 *
 * @see access_grant_form_validate()
 * @see access_grant_form_submit()
 */
function access_grant_form($form, &$form_state, $grant) {
  // During initial form build, add the grant entity to the form state for use
  // during form building and processing. During a rebuild, use what is in the
  // form state.
  if (!isset($form_state['grant'])) {
    $form_state['grant'] = $grant;
  }
  else {
    $grant = $form_state['grant'];
  }

  // Basic grant information.  Not user-configurable.
  foreach (array('gid', 'uid', 'rid', 'scheme') as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => isset($grant->$key) ? $grant->$key : NULL,
    );
  }
  $is_new = empty($grant->gid);

  // The ACK user reference field.
  $account = empty($grant->uid) ? FALSE : user_load($grant->uid);
  $form['user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#maxlength' => 60,
    '#weight' => -5,
    '#required' => $is_new,
    '#disabled' => !$is_new,
  );
  if ($is_new) {
    $form['user']['#autocomplete_path'] = 'user/autocomplete';
  }
  if (!empty($account)) {
    $form['user']['#default_value'] = check_plain($account->name);
  }

  // The ACK role reference field.
  $role = empty($grant->rid) ? FALSE : user_role_load($grant->rid);
  $form['role'] = array(
    '#type' => 'select',
    '#title' => t('Role'),
    '#weight' => -4,
    '#options' => access_realm_roles(),
    '#required' => $is_new,
    '#disabled' => !$is_new,
  );
  if (!empty($role)) {
    $form['role']['#default_value'] = $role->rid;
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );
  if (!empty($grant->gid)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 10,
    );
  }

  field_attach_form('access_grant', $grant, $form, $form_state);
  return $form;
}

/**
 * Form validation handler for access_grant_form().
 *
 * @see access_grant_form_submit()
 */
function access_grant_form_validate($form, &$form_state) {
  $grant = (object) $form_state['values'];

  // On new grants, we must validate that the user reference is valid, and that
  // the uid-rid-scheme combination is unique. This is not needed for editing
  // an existing grant, because the user and role fields cannot be changed.
  if (empty($grant->gid)) {
    $account = empty($grant->user) ? FALSE : user_load_by_name($grant->user);
    $role = empty($grant->role) ? FALSE : user_role_load($grant->role);

    // Validate the user reference.
    if (empty($account)) {
      form_set_error('user', t('The username %name does not exist.', array('%name' => $grant->user)));
    }

    // Validate the uniqueness of the user-role-scheme combination.
    if (isset($account->uid) && isset($role->rid)) {
      $existing = access_grant_load_by_condition(array('uid' => $account->uid, 'rid' => $role->rid, 'scheme' => $grant->scheme));
      if (!empty($existing)) {
        $scheme = access_scheme_load($grant->scheme);
        $existing = reset($existing);
        form_set_error('', t('%user has already been granted access as %role in @scheme. Duplicate access grants are not permitted. Please <a href="@url">edit the existing grant</a> instead.', array('%user' => $account->name, '%role' => $role->name, '@scheme' => $scheme->name, '@url' => url('admin/access/grant/' . $existing->gid . '/edit'))));
      }
    }
  }

  entity_form_field_validate('access_grant', $form, $form_state);
}

/**
 * Form submission handler for access_grant_form().
 *
 * @see access_grant_form_validate()
 */
function access_grant_form_submit($form, &$form_state) {
  // Check for the Delete button.
  if ($form_state['triggering_element']['#value'] == t('Delete')) {
    $form_state['redirect'] = 'admin/access/grant/' . $form_state['values']['gid'] . '/delete';
    return;
  }

  $grant = access_grant_form_submit_build_grant($form, $form_state);
  $status = access_grant_save($grant);

  $account = user_load($grant->uid);
  $role = user_role_load($grant->rid);
  $scheme = access_scheme_load($grant->scheme);
  $t_args = array(
    '%user' => $account->name,
    '%role' => $role->name,
    '%scheme' => $scheme->name,
  );
  switch ($status) {
    case SAVED_NEW:
      drupal_set_message(t('Granted %user access as %role in %scheme.', $t_args));
      watchdog('access', 'Granted %role to %user in %scheme.', $t_args, WATCHDOG_NOTICE, l(t('edit'), 'admin/access/grant/' . $grant->gid . '/edit'));
      break;
    case SAVED_UPDATED:
      drupal_set_message(t("Updated %user's access as %role in %scheme.", $t_args));
      watchdog('access', 'Updated %role realms for %user in %scheme.', $t_args, WATCHDOG_NOTICE, l(t('edit'), 'admin/access/grant/' . $grant->gid . '/edit'));
      break;
  }

  $form_state['values']['gid'] = $grant->gid;
}

/**
 * Updates the form state's grant entity by processing the submission values.
 */
function access_grant_form_submit_build_grant($form, &$form_state) {
  $grant = $form_state['grant'];
  entity_form_submit_build_entity('access_grant', $grant, $form, $form_state);

  if (empty($grant->gid)) {
    $account = user_load_by_name($form_state['values']['user']);
    $grant->uid = $account->uid;
    $grant->rid = $form_state['values']['role'];
  }

  return $grant;
}

/**
 * Form constructor for the access grant delete confirmation form.
 *
 * @see access_grant_delete_confirm_submit()
 */
function access_grant_delete_confirm($form, &$form_state, $grant) {
  $form_state['grant'] = $grant;

  $account = user_load($grant->uid);
  $role = user_role_load($grant->rid);
  $scheme = access_scheme_load($grant->scheme);
  $t_args = array(
    '%user' => $account->name,
    '%role' => $role->name,
    '%scheme' => $scheme->name,
  );

  $message = t("Are you sure you want to revoke %user's access as %role in %scheme?", $t_args);
  $caption = '<p>' . t('If this is the last grant giving %user access as %role, then %user will be removed from the role entirely.', $t_args) . '</p>';
  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/access/grant/' . $grant->gid, $caption, t('Delete'));
}

/**
 * Form submission handler for access_grant_delete_confirm().
 */
function access_grant_delete_confirm_submit($form, &$form_state) {
  $grant = $form_state['grant'];
  access_grant_delete($grant->gid);

  $account = user_load($grant->uid);
  $role = user_role_load($grant->rid);
  $scheme = access_scheme_load($grant->scheme);
  $t_args = array(
    '%user' => $account->name,
    '%role' => $role->name,
    '%scheme' => $scheme->name,
  );
  drupal_set_message(t('%user has been removed from the %role role in %scheme.', $t_args));
  watchdog('access', 'Revoked %role for %user in %scheme.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/access';
  return;
}

/**
 * Form constructor for the access grant multiple delete confirmation form.
 *
 * @see access_grant_multiple_delete_confirm_submit()
 */
function access_grant_multiple_delete_confirm($form, &$form_state, $selected) {
  $gids = array_keys($selected);
  $grants = access_grant_load_multiple($gids);
  $roles = user_roles();
  $schemes = access_scheme_get_names();
  $usernames = db_query('SELECT g.uid, u.name FROM {access_grant} g INNER JOIN {users} u ON g.uid = u.uid WHERE g.gid IN (:gids)', array(':gids' => $gids))->fetchAllKeyed();

  $form['grants'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );
  foreach ($grants as $grant) {
    $form['grants'][$grant->gid] = array(
      '#type' => 'hidden',
      '#value' => $grant->gid,
      '#prefix' => '<li>',
      '#suffix' => t('@user as @role in @scheme', array('@user' => $usernames[$grant->uid], '@role' => $roles[$grant->rid], '@scheme' => $schemes[$grant->scheme])) . "</li>\n",
    );
  }

  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => 'delete',
  );
  $form['#submit'][] = 'access_grant_multiple_delete_confirm_submit';
  $confirm_question = format_plural(count($grants), 'Are you sure you want to delete this access grant?', 'Are you sure you want to delete these access grants?');
  return confirm_form($form, $confirm_question, 'admin/access', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for access_grant_multiple_delete_confirm().
 */
function access_grant_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach (array_keys($form_state['values']['grants']) as $gid) {
      access_grant_delete($gid);
    }
    $count = count($form_state['values']['grants']);
    watchdog('access', 'Deleted @count access grants.', array('@count' => $count));
    drupal_set_message(format_plural($count, 'Deleted 1 access grant.', 'Deleted @count access grants.'));
  }
  $form_state['redirect'] = 'admin/access';
}

/**
 * Process variables for access-grant.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $access_grant
 *
 * @see access-grant.tpl.php
 */
function template_preprocess_access_grant(&$variables) {
  $grant = $variables['elements']['#access_grant'];
  $variables['access_grant'] = $grant;

  // Helpful $ack variable for templates.
  foreach (element_children($variables['elements']) as $key) {
    $variables['ack'][$key] = $variables['elements'][$key];
  }
  field_attach_preprocess('access_grant', $grant, $variables['elements'], $variables);

  // Add helper variables for the access realm field, since the name is dynamic.
  $variables['access_field'] = 'ack_' . $grant->scheme;
  $variables['ack_realms'] = $variables[$variables['access_field']];

  // Add a class to identify the scheme.
  $variables['classes_array'][] = drupal_html_class('access-grant-' . $grant->scheme);

  // Allow per scheme templates.
  $variables['theme_hook_suggestions'][] = 'access_grant__' . $grant->scheme;
}
