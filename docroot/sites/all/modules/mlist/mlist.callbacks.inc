<?php
/**
 * @file
 *
 * Function callbacks and helper functions.
 */

/**
 * Creates the form to allow users to subscribe or unsubscribe from lists.
 *
 * @param array $form
 *   The Drupal form array.
 * @param array $form_state
 *   The Drupal form state array.
 *
 * @return array
 *   A drupal renderable form array.
 */
function mlist_subscribe_form(array $form, array &$form_state) {
  global $user;

  $lists = mlist_get_lists();
  if (count($lists) > 0) {

    // Provide some instructions.
    $form['intro'] = array(
      '#markup' => t('Subscribe or unsubscribe to a mailing list by entering your email address and selecting the checkbox for the mailing list. You will receive an email with further instructions to complete the process.'),
    );

    // If the user is anonymous then provide a text field for the email.
    if ($user->uid == 0) {
      $form['email'] = array(
        '#type'      => 'textfield',
        '#title'     => t('Your Email address'),
        '#size'      => 20,
        '#required'  => TRUE,
        '#maxlength' => 255,
        '#element_validate' => array('mlist_validate_email'),
      );
    }
    // Else use the registered email address for the user.
    else {
      $form['email'] = array(
        '#type'      => 'value',
        '#value'     => check_plain($user->mail),
      );
      $form['your_email'] = array(
        '#markup'    => '<p><label>' . t('Your email address: %email', array('%email' => $user->mail)) . '</label></p>',
      );
    }

    // Iterate through the lists and create a checkbox field for each one.
    $options = array();
    foreach ($lists as $list_id => $info) {
      $options[$list_id] = check_plain($info['name'] . '. (' . $info['description'] . ') ');
    }
    $form['lists'] = array(
      '#type'  => 'checkboxes',
      '#title' => t('Please select one or more lists.'),
      '#required'  => TRUE,
      '#options' => $options,
    );

    // Add in the subscribe and unsubscribe buttons.
    $form['subscribe'] = array(
      '#type' => 'submit',
      '#value' => t('subscribe'),
      '#name' => 'subscribe',
    );
    $form['unsubscribe'] = array(
      '#type' => 'submit',
      '#value' => t('unsubscribe'),
      '#name' => 'unsubscribe',
    );
  }
  else {
    $form['intro'] = array(
      '#markup' => t('Sorry, there are no lists currently available for subscription.'),
    );
  }
  return $form;
}

/**
 * A function for validating an email address in a form.
 *
 * @param array $element
 *   The form $element to validate.
 * @param array $form_state
 *   The form state array.
 * @param array $form
 *   The form array.
 */
function mlist_validate_email(array $element, array &$form_state, array $form) {
  if (!valid_email_address($element['#value'])) {
    form_error($element, t('Please provide a valid email address.'));
  }
}
/**
 * Submit callback for mlist_subscribe_form.
 */
function mlist_subscribe_form_submit($form, &$form_state) {
  $from = trim($form_state['values']['email']);
  $params = array();
  $lists = mlist_get_lists();
  $selected_lists = $form_state['values']['lists'];

  // If the operation is to subscribe or unsubscribe set the action.
  if ($form_state['clicked_button']['#name'] == 'subscribe') {
    $action = 'subscribe';
  }
  if ($form_state['clicked_button']['#name'] == 'unsubscribe') {
    $action = 'unsubscribe';
  }

  // If the user supplied an email address then send the email
  // the Mailman list server.
  if ($from) {
    // Iterate through all of the available lists and for those that
    // the user selected send an email.
    foreach ($lists as $list_id => $info) {
      if (in_array($list_id, $selected_lists)) {
        $list_email = $info[$action];
        $params['subject'] = $action;
        $message = drupal_mail('mlist', $action, $list_email, language_default(), $params, $from);

        // Inform the user that an email has been sent.
        if ($message['result']) {
          drupal_set_message(
            t('An email has been sent to, %from, with details to complete the %action process to the %list_name mailing list. Please follow the instructions in the email to continue.',
              array(
                '%from' => $from,
                '%action' => $action,
                '%list_name' => $lists[$list_id]['name'],
              )
            )
          );
        }
      }
    }
  }
}

/**
 * Provides the form for adding and editing of a mailing list.
 *
 * @param array $form
 *   The Drupal form array.
 * @param array $form_state
 *   The Drupal form state array.
 * @param int $list_id
 *   The ID of the list to delete.
 *
 * @return array
 *   A drupal renderable form array.
 */
function mlist_form(array $form, array &$form_state, $list_id = NULL) {

  // Initialize the default values.
  $default_sync  = array();
  $default_roles = array();
  $default_name  = '';
  $default_desc  = '';
  $default_sub   = '';
  $default_usub  = '';
  if (array_key_exists('values', $form_state)) {
    $default_name  = array_key_exists('list_name', $form_state['values']) ? $form_state['values']['list_name'] : '';
    $default_desc  = array_key_exists('description', $form_state['values']) ? $form_state['values']['description'] : '';
    $default_sub   = array_key_exists('subscribe_email', $form_state['values']) ? $form_state['values']['subscribe_email'] : '';
    $default_usub  = array_key_exists('unsubscribe_email', $form_state['values']) ? $form_state['values']['unsubscribe_email'] : '';
    $default_sync  = array_key_exists('sync', $form_state['values']) ? $form_state['values']['sync'] : '';
    $default_roles = array_key_exists('default_roles', $form_state['values']) ? $form_state['values']['default_roles'] : '';
  }

  // If a list_id is provided to the form then this form should set the
  // defaults using the database fields.
  $action = t('Add');
  $btn_name = 'Add';
  if ($list_id) {
    $action = t('Update');
    $btn_name = 'Update';
    $list = db_select('mailing_lists', 'ml')
      ->fields('ml')
      ->condition('list_id', $list_id)
      ->execute()
      ->fetchObject();
    if (!$default_name) {
      $default_name = $list->list_name;
    }
    if (!$default_desc) {
      $default_desc = $list->description;
    }
    if (!$default_sub) {
      $default_sub = $list->subscribe_email;
    }
    if (!$default_usub) {
      $default_usub = $list->unsubscribe_email;
    }
    $default_sync = explode(',', $list->sync);
    $assigned_roles = db_select('mailing_lists_roles', 'mlr')
      ->fields('mlr', array('rid'))
      ->condition('list_id', $list_id)
      ->execute();
    foreach ($assigned_roles as $assigned_role) {
      $default_roles[] = $assigned_role->rid;
    }
  }
  // Get the list of available roles on the site.
  $allroles = db_select('role', 'r')
    ->fields('r')
    ->orderBy('name')
    ->execute();
  $roles = array();
  $sync_roles = array();
  foreach ($allroles as $role) {
    $roles[$role->rid] = $role->name;
    // We can't sync anonymous users so ignore that role.
    if ($role->name != 'anonymous user') {
      $sync_roles[$role->rid] = $role->name;
    }
  }

  $form['list_id'] = array(
    '#type'  => 'value',
    '#value' => $list_id,
  );
  $form['list_name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Mailing List Name'),
    '#description'   => t('Please enter the name of the MailMan mailing list.  This should be the name that appears in the MailMan configuration settings'),
    '#required'      => TRUE,
    '#default_value' => $default_name,
  );
  $form['description'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Description'),
    '#description'   => t('Please enter a description for this database that site visitors will read.'),
    '#default_value' => $default_desc,
    '#required'      => TRUE,
  );
  $form['subscribe_email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Subscription email'),
    '#description'   => t('Please enter the mailman subscription email address. The typical mailman subscribe email address for a list is: [list name]-subscribe@[domain].'),
    '#default_value' => $default_sub,
    '#required'      => TRUE,
    '#element_validate' => array('mlist_validate_email'),
  );
  $form['unsubscribe_email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Unsubscription email'),
    '#description'   => t('Please enter the mailman unsubscription email address. The typical mailman subscribe email address for a list is: [list name]-unsubscribe@[domain].'),
    '#default_value' => $default_usub,
    '#required'      => TRUE,
    '#element_validate' => array('mlist_validate_email'),
  );
  $form['roles'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Allowed Roles'),
    '#description'   => t('Please select the roles that should be able to subscribe/unsubscribe to the list.'),
    '#options'       => $roles,
    '#required'      => TRUE,
    '#default_value' => $default_roles,
  );
  $form['sync'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Sync mailing list members with the following roles'),
    '#description'   => t('Sync Mailman mailing list members for the selected roles. This option only works when the "drush mailman-sync" command can be executed on the same server where the Mailman mail server is installed.'),
    '#options'       => $sync_roles,
    '#default_value' => $default_sync,
  );
  $form['add'] = array(
    '#type'  => 'submit',
    '#value' => $action,
    '#name'  => $btn_name,
  );
  $form['#tree'] = TRUE;
  $form['#prefix'] = '<div id="mlist_edit_form_div">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Validate callback for mlist_form.
 */
function mlist_form_validate($form, &$form_state) {
  $list_name = trim($form_state['values']['list_name']);
  $list_id   = $form_state['values']['list_id'];

  // Make sure list_name only contains alpha-numeric, dashes and underscores.
  if (preg_match("/[^a-zA-Z0-9_-]/", $list_name)) {
    form_set_error('list_name', t('Please provide a list name with only alhpa-numeric, dashes or underscore characters.'));
  }

  // Check if the list name has changed and if so, make sure it is unique.
  if ($list_id) {
    $orig_list_name = db_select('mailing_lists', 'ml')
      ->fields('ml', array('list_name'))
      ->condition('list_id', $list_id)
      ->execute()
      ->fetchField();
    // Make sure the new name is not a duplicate.
    if ($orig_list_name != $list_name) {
      $new_list_name = db_select('mailing_lists', 'ml')
        ->fields('ml', array('list_name'))
        ->condition('list_name', $list_name)
        ->condition('list_id', $list_id, '<>')
        ->execute()
        ->fetchField();
      if ($new_list_name) {
        form_set_error('list_name', t('The list name already exists.'));
      }
    }
  }
  // We don't have a list id, so make sure no other list has the same name.
  else {
    $new_list_name = db_select('mailing_lists', 'ml')
      ->fields('ml', array('list_name'))
      ->condition('list_name', $list_name)
      ->execute()
      ->fetchField();
    if ($new_list_name) {
      form_set_error('list_name', t('The list name already exists.'));
    }
  }
}

/**
 * Submit callback for mlist_form.
 */
function mlist_form_submit($form, &$form_state) {
  $list_id           = $form_state['values']['list_id'];
  $list_name         = trim($form_state['values']['list_name']);
  $description       = trim($form_state['values']['description']);
  $subscribe_email   = trim($form_state['values']['subscribe_email']);
  $unsubscribe_email = trim($form_state['values']['unsubscribe_email']);
  $roles             = $form_state['values']['roles'];
  $sync              = $form_state['values']['sync'];
  $sync_rids         = '';

  $form_state['redirect'] = 'admin/config/system/mlist/show';

  // Iterate through the roles that the users requested to be synced and
  // construct a comma-delimited list of role IDs.
  foreach ($sync as $s_rid => $s_selected) {
    if ($s_selected) {
      $sync_rids .= $s_rid;
      $sync_rids .= ',';
    }
  }
  // Remove the trailing comma.
  $sync_rids = substr($sync_rids, 0, -1);

  // Now handle the response based on the button clicked.
  $clicked_button = $form_state['clicked_button']['#value'];
  $transaction = db_transaction();
  try {
    // If a list id is provided then we will handle an update or a delete.
    if (strcmp($clicked_button, 'Update') == 0) {
      db_update('mailing_lists')
        ->fields(array(
          'list_name'         => $list_name,
          'description'       => $description,
          'subscribe_email'   => $subscribe_email,
          'unsubscribe_email' => $unsubscribe_email,
          'sync'              => $sync_rids,
        ))
        ->condition('list_id', $list_id)
        ->execute();
    }
    if (strcmp($clicked_button, 'Add') == 0) {
      // Add the list.
      $list_id = db_insert('mailing_lists')
        ->fields(array(
          'list_name'         => $list_name,
          'description'       => $description,
          'subscribe_email'   => $subscribe_email,
          'unsubscribe_email' => $unsubscribe_email,
          'sync'              => $sync_rids,
        ))
        ->execute();
    }
    // Add the allowed roles for this list.  We want to first remove any
    // roles that exists and then add in the newly selected ones
    // first remove all existing roles for this list.
    db_delete('mailing_lists_roles')
      ->condition('list_id', $list_id)
      ->execute();
    // Now add the roles.
    foreach ($roles as $rid => $selected) {
      if ($selected) {
        db_insert('mailing_lists_roles')
          ->fields(array('list_id' => $list_id, 'rid' => $rid))
          ->execute();
      }
    }
  }
  catch (Exception $e) {
    watchdog('mlist', $e->getMessage(), array(), WATCHDOG_ERROR);
    drupal_set_message(t('Failed to add mailing list.'), 'error');
    $transaction->rollback();
    return;
  }
  drupal_set_message(t('Mailing list saved.'));
}

/**
 * Returns the form for deleting a list.
 *
 * @param array $form
 *   The Drupal form array.
 * @param array $form_state
 *   The Drupal form state array.
 * @param int $list_id
 *   The ID of the list to delete.
 *
 * @return array
 *   A drupal renderable form array.
 */
function mlist_delete_list_form(array $form, array &$form_state, $list_id) {
  // Set the breadcrumb.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t('Administration'), 'admin');
  $breadcrumb[] = l(t('Configuration'), 'admin/config');
  $breadcrumb[] = l(t('Mailing Lists'), 'admin/config/system/mlist');
  drupal_set_breadcrumb($breadcrumb);

  $list_name = db_select('mailing_lists', 'ml')
    ->fields('ml', array('list_name'))
    ->condition('list_id', $list_id)
    ->execute()
    ->fetchField();
  $form['list_id'] = array(
    '#type'  => 'value',
    '#value' => $list_id,
  );
  $form['extra'] = array(
    '#type' => 'item',
    '#markup' => t('This will remove the list from this site. Users will no longer be able to subscribe or unsubscribe using this site.'),
  );
  $form['description'] = array(
    '#type' => 'item',
    '#markup' => t('Are you sure you want to delete the list: %list_name?', array('%list_name' => $list_name)),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/system/mlist/show',
  );
  // By default, render the form using theme_confirm_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'confirm_form';
  }
  return $form;
}

/**
 * Implements hook_submit().
 */
function mlist_delete_list_form_submit($form, &$form_state) {
  $list_id = $form_state['values']['list_id'];
  $form_state['redirect'] = 'admin/config/system/mlist/show';

  // Delete the list and ancillary information.
  $transaction = db_transaction();
  try {
    db_delete('mailing_lists')
      ->condition('list_id', $list_id)
      ->execute();
    db_delete('mailing_lists_roles')
      ->condition('list_id', $list_id)
      ->execute();
  }
  catch (Exception $e) {
    watchdog('mlist', $e->getMessage(), array(), WATCHDOG_ERROR);
    drupal_set_message(t('Failed to delete mailing list.'), 'error');
    $transaction->rollback();
    return;
  }
  drupal_set_message(t('Mailing list deleted'));
}

/**
 * Provides an HTML table of the lists with links for editing and deleting.
 */
function mlist_show_lists() {
  $header = array('List Name', 'Description', 'Actions');
  $rows = array();

  // Get all of the lists and create a table row for each one.
  $lists = db_select('mailing_lists', 'ml')
    ->fields('ml')
    ->execute();
  foreach ($lists as $list) {
    $rows[] = array(
      check_plain($list->list_name),
      check_plain($list->description),
      l(t('Edit'), 'admin/config/system/mlist/edit/' . $list->list_id) . ' ' .
      l(t('Delete'), 'admin/config/system/mlist/delete/' . $list->list_id),
    );
  }
  $table = array(
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array(),
    '#sticky' => TRUE,
    '#caption' => '',
    '#colgroups' => array(),
    '#empty' => t('There are currently no mailing lists'),
    '#theme' => 'table',
  );
  return $table;
}
