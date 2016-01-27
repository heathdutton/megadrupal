<?php

/**
 * @file
 * This is the Node Access Keys admin include which provides an interface for
 * adding/deleting/modifying the site's Access Keys as well as it's settings.
 */

/**
 * Form constructor for the settings form.
 *
 * @see nodeaccesskeys_settings_validate()
 * @ingroup forms
 */
function nodeaccesskeys_settings($form, &$form_state) {
  $node_types = array_map('check_plain', node_type_get_names());
  $form['nodeaccesskeys_default_node_type'] = array(
    '#type' => 'select',
    '#options' => $node_types,
    '#title' => t('Default Content Type'),
    '#default_value' => variable_get('nodeaccesskeys_default_node_type', NULL),
    '#description' => t('Select a default content type to use when viewing pages that are not nodes or views. This is needed for system pages that display the Node Access Keys form.'),
  );
  $form['nodeaccesskeys_site_403'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#title' => t('Alternate 403 (Access Denied) Page'),
    '#default_value' => variable_get('nodeaccesskeys_site_403', NULL),
    '#description' => t('Optionally define a 403 page to use when the Node Access Keys form is not relevant, such as on the Administration pages.'),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  $form['#validate'][] = 'nodeaccesskeys_settings_validate';

  return system_settings_form($form);
}

/**
 * Validates the submitted settings form.
 *
 * @ingroup forms
 */
function nodeaccesskeys_settings_validate($form, &$form_state) {
  // Get the normal path of the Alternate 403 error page.
  if (!empty($form_state['values']['nodeaccesskeys_site_403'])) {
    form_set_value($form['nodeaccesskeys_site_403'], drupal_get_normal_path($form_state['values']['nodeaccesskeys_site_403']), $form_state);
  }
  // Validate Alternate 403 error path.
  if (!empty($form_state['values']['nodeaccesskeys_site_403']) && !drupal_valid_path($form_state['values']['nodeaccesskeys_site_403'])) {
    form_set_error('nodeaccesskeys_site_403', t("The path '%path' is either invalid or you do not have access to it.", array('%path' => $form_state['values']['nodeaccesskeys_site_403'])));
  }
}

/**
 * Function to generate the list of Access Keys.
 *
 * @see nodeaccesskeys_list_validate()
 * @see nodeaccesskeys_list_submit()
 * @ingroup forms
 */
function nodeaccesskeys_list($form, &$form_state) {
  // Initialize a description of the steps for the wizard.
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;
  }

  switch ($form_state['step']) {
    case 1:
      $header = array(
        'title' => array(
          'data' => t('Key'),
          'field' => 'a.accesskey',
          'sort' => 'desc',
        ),
        'type' => array(
          'data' => t('Content Types'),
          'field' => 'a.nodetypes',
        ),
        'roles' => array(
          'data' => t('Roles'),
          'field' => 'a.roles',
        ),
        'operations' => array(
          'data' => t('Operations'),
        ),
      );

      $query = db_select('nodeaccesskeys', 'a')
        ->extend('PagerDefault')
        ->extend('TableSort');

      $keys = $query
        ->fields('a')
        ->limit(50)
        ->orderByHeader($header)
        ->execute();

      // Prepare the list of Access Keys.
      $options = array();
      foreach ($keys as $key) {
        // Get the Content Type names from the serialized machine-names.
        $types = array_map('node_type_get_name', unserialize($key->nodetypes));
        $types = array_diff($types, array(FALSE));

        // Get the readable role names from the role IDs stored in the database.
        $role_ids = unserialize($key->roles);
        $roles = array();
        foreach ($role_ids as $id) {
          if ($temp = user_role_load($id)) {
            $roles[] = $temp->name;
          }
        }

        $types = array_map('check_plain', $types);
        $roles = array_map('check_plain', $roles);

        // Create the edit URI.
        $edit_href = 'admin/config/people/nodeaccesskeys/' . $key->aid . '/edit';

        // Create the delete URI.
        $delete_href = 'admin/config/people/nodeaccesskeys/' . $key->aid . '/delete';

        $options[$key->aid] = array(
          'title' => $key->accesskey,
          'type' => implode(', ', $types),
          'roles' => implode(', ', $roles),
          'operations' => array(
            'data' => array(
              '#theme' => 'links__operations',
              '#links' => array(
                'edit' => array(
                  'title' => t('edit'),
                  'href' => $edit_href,
                ),
                'delete' => array(
                  'title' => t('delete'),
                  'href' => $delete_href,
                ),
              ),
              '#attributes' => array('class' => array('links', 'inline')),
            ),
          ),
        );
      }

      $form['nodeaccesskeys'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#empty' => t('No keys available.'),
      );

      $form['pager'] = array('#markup' => theme('pager'));

      if (!empty($options)) {
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Delete Selected Keys'),
        );
      }
      break;

    case 2:
      $result = db_select('nodeaccesskeys', 'n')
        ->fields('n', array('accesskey'))
        ->condition('aid', $form_state['storage']['nodeaccesskeys'])
        ->execute();

      $list_args = array('items' => array(), 'type' => 'ul');
      foreach ($result as $record) {
        $list_args['items'][] = check_plain($record->accesskey);
      }
      $list = theme('item_list', $list_args);

      $form = confirm_form($form,
        t('Are you sure you want to delete these Access Keys?'),
        'admin/config/people/nodeaccesskeys',
        $list . '<p>' . t('This action cannot be undone.') . '</p>',
        t('Delete'),
        t('Cancel')
      );
      break;
  }

  return $form;
}

/**
 * Form validation handler for nodeaccesskeys_list().
 *
 * @see nodeaccesskeys_list_submit()
 * @ingroup forms
 */
function nodeaccesskeys_list_validate($form, &$form_state) {
  $current_step = &$form_state['step'];

  switch ($current_step) {
    case 1:
      $aids = array_diff($form_state['values']['nodeaccesskeys'], array(0));
      if (empty($aids)) {
        form_set_error('nodeaccesskeys', t('You must select at least one Access Key to be deleted.'));
      }
      break;
  }
}

/**
 * Form submission handler for nodeaccesskeys_list().
 *
 * @see nodeaccesskeys_list_validate()
 * @ingroup forms
 */
function nodeaccesskeys_list_submit($form, &$form_state) {
  $current_step = &$form_state['step'];

  switch ($current_step) {
    // Store the values and rebuild the form for confirmation.
    case 1:
      $aids = array_diff($form_state['values']['nodeaccesskeys'], array(0));
      $form_state['storage'] = array('nodeaccesskeys' => $aids);
      $form_state['rebuild'] = TRUE;
      break;

    // Action confirmed. Delete Access Keys and return to the overview.
    case 2:
      $deleted = db_delete('nodeaccesskeys')
      ->condition('aid', $form_state['storage']['nodeaccesskeys'])
      ->execute();

      node_access_rebuild();
      drupal_set_message(format_plural(
        count($form_state['storage']['nodeaccesskeys']),
        'Deleted 1 Access Key.',
        'Deleted @count Access Keys.'));

      unset($form_state['step']);
      break;
  }

  $current_step++;
}

/**
 * Form constructor for the Add Access Key form.
 *
 * @see nodeaccesskeys_add_submit()
 * @ingroup forms
 */
function nodeaccesskeys_add($form, &$form_state) {
  return nodeaccesskeys_edit($form, $form_state);
}

/**
 * Form constructor for the Edit Access Key form.
 *
 * @param array $form
 *   The form structure where field elements are attached to. This might be a
 *   full form structure, or a sub-element of a larger form.
 * @param array $form_state
 *   The form state.
 * @param int $aid
 *   The access key ID to edit.
 *
 * @see nodeaccesskeys_edit_submit()
 * @ingroup forms
 */
function nodeaccesskeys_edit($form, &$form_state, $aid = NULL) {

  // Build the form without default values.
  $form['accesskey'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Key'),
    '#size' => 60,
    '#maxlength' => 256,
  );
  $node_types = array_map('check_plain', node_type_get_names());
  $form['nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Types'),
    '#title_display' => 'before',
    '#options' => $node_types,
  );
  $roles = array_map('check_plain', user_roles());
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User Roles'),
    '#title_display' => 'before',
    '#options' => $roles,
  );
  $form['submit'] = array('#type' => 'submit');

  // If there is an Access Key ID then look it up and provide default values.
  if (is_numeric($aid)) {
    $result = db_query(
      "SELECT aid, accesskey, nodetypes, roles
        FROM {nodeaccesskeys}
        WHERE aid = :aid",
      array(
        ':aid' => $aid,
      ));

    if ($record = $result->fetchObject()) {
      // Pass the Access Key ID.
      $form_state['aid'] = $record->aid;

      // Assign the default value for the Access Key.
      $form['accesskey']['#default_value'] = $record->accesskey;

      // Assign the default value for the Node Types.
      if (!empty($record->nodetypes)) {
        $nodetypes = unserialize($record->nodetypes);
        $form['nodetypes']['#default_value'] = $nodetypes;
      }

      // Assign the default value for the Roles.
      if (!empty($record->roles)) {
        $roles = unserialize($record->roles);
        $form['roles']['#default_value'] = $roles;
      }
    }

    $form['submit']['#value'] = t('Save Key');
  }
  else {
    $form['submit']['#value'] = t('Add Key');
  }

  return $form;
}

/**
 * Form submission handler for nodeaccesskeys_add().
 *
 * @ingroup forms
 */
function nodeaccesskeys_add_submit($form, &$form_state) {
  $node_types = $roles = array();

  // Reduce the checkbox values from a 2-dimensional array to a 1-dimensional
  // array while filtering out the unchecked values for both types and roles.
  foreach ($form_state['values']['nodetypes'] as $key => $value) {
    if (!empty($value)) {
      $node_types[] = $key;
    }
  }
  foreach ($form_state['values']['roles'] as $key => $value) {
    if (!empty($value)) {
      $roles[] = $key;
    }
  }

  nodeaccesskeys_add_key($form_state['values']['accesskey'], $node_types, $roles);
  drupal_set_message(t('New Access Key has been saved.'));
}

/**
 * Form submission handler for nodeaccesskeys_edit().
 *
 * @ingroup forms
 */
function nodeaccesskeys_edit_submit($form, &$form_state) {
  // An associative array is too excessive for storage in the database.
  $types = array_values($form_state['values']['nodetypes']);
  $roles = array_values($form_state['values']['roles']);

  // Remove the array elements that were not checked.
  $types = array_diff($types, array(0));
  $roles = array_diff($roles, array(0));

  db_update('nodeaccesskeys')
  ->fields(array(
    'accesskey' => $form_state['values']['accesskey'],
    'nodetypes' => serialize($types),
    'roles' => serialize($roles),
  ))
  ->condition('aid', (int) $form_state['aid'])
  ->execute();

  $form_state['redirect'][] = 'admin/config/people/nodeaccesskeys';
  node_access_rebuild();
  drupal_set_message(t('Access Key saved.'));
}

/**
 * Form constructor for the Access Key delete form.
 *
 * @param array $form
 *   The form structure where field elements are attached to. This might be a
 *   full form structure, or a sub-element of a larger form.
 * @param array $form_state
 *   The form state.
 * @param int $aid
 *   The Access Key ID to delete.
 *
 * @see nodeaccesskeys_delete_submit()
 * @ingroup forms
 */
function nodeaccesskeys_delete($form, &$form_state, $aid) {
  if (is_numeric($aid)) {
    $result = db_query(
      "SELECT accesskey FROM {nodeaccesskeys} WHERE aid = :aid",
      array(':aid' => $aid)
    );
    $record = $result->fetchObject();
    $form_state['delete_aid'] = $aid;
    return confirm_form($form,
      t(
        'Are you sure you want to delete the Access Key %accesskey?',
        array('%accesskey' => $record->accesskey)
      ),
      'admin/config/people/nodeaccesskeys',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
}

/**
 * Form submission handler for nodeaccesskeys_delete().
 *
 * @ingroup forms
 */
function nodeaccesskeys_delete_submit($form, &$form_state) {
  if (isset($form_state['delete_aid'])) {
    $deleted = db_delete('nodeaccesskeys')
      ->condition('aid', $form_state['delete_aid'])
      ->execute();

    $form_state['redirect'][] = 'admin/config/people/nodeaccesskeys';
    node_access_rebuild();
    drupal_set_message(t('Deleted Access Key.'));
  }
}
