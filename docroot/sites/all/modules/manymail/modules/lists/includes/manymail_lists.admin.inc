<?php
/**
 * @file
 * Admin page callbacks for recipient configuration.
 */

/**
 * Form function for selecting available recipient lists.
 *
 * @ingroup forms
 */
function manymail_lists_targets_lists_form($form, &$form_state) {
  $options = array();
  $lists = _manymail_lists_valid_lists();

  $header = array(
    t('Name'),
    t('Description'),
    t('Recipients'),
    // @todo: Follow up on http://drupal.org/node/365554.
    // Uncomment next line when it is committed.
    // array('data' => t('Actions'), 'colspan' => 2),
    t('Actions'),
  );

  foreach ($lists as $name => $list) {
    $options[$name] = array(
      check_plain($list->display_name),
      check_plain($list->description),
      $list->amount,
      // @todo: Follow up on http://drupal.org/node/365554.
      // Remove concatenation and add comma after it is committed.
      l(t('edit'), "admin/config/manymail/targets/lists/{$list->name}/edit") . ' / ' .
      l(t('delete'), "admin/config/manymail/targets/lists/{$list->name}/delete"),
    );
  }

  $form['allowed_lists'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allowed recipient lists'),
    '#description' => t('Select which lists can be used as a recipient list.'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['allowed_lists']['manymail_lists_allowed_lists'] = array(
    '#type' => 'tableselect',
    '#default_value' => variable_get('manymail_lists_allowed_lists', array()),
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No lists available.'),
    '#js_select' => FALSE,
  );

  $form['allowed_lists']['manymail_lists_allowed_lists_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Certain users may ignore this restriction'),
    '#default_value' => variable_get('manymail_lists_allowed_lists_override', 1),
    '#description' => t('When checked, users with the :permission permission can target any !what regardless of what has been configured above.', array(
      ':permission' => 'Override preset recipient lists',
      '!what' => 'list',
    )),
  );

  return system_settings_form($form);
}

/**
 * Form function for showing the recipient list management page.
 *
 * @ingroup forms
 */
function manymail_lists_manage_form($form, &$form_state) {
  $options = array();
  $lists = _manymail_lists_all_lists();

  $header = array(
    t('Name'),
    t('Description'),
    array('data' => t('Actions'), 'colspan' => 2),
  );

  foreach ($lists as $name => $list) {
    $options[] = array(
      check_plain($list->display_name),
      check_plain($list->description),
      l(t('edit'), "admin/config/manymail/targets/lists/manage/{$list->name}/edit"),
      l(t('delete'), "admin/config/manymail/targets/lists/manage/{$list->name}/delete"),
    );
  }

  $form['lists'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No lists available.'),
  );

  return $form;
}

/**
 * Form function for creating or editing a recipient list.
 *
 * @ingroup forms
 */
function manymail_lists_modify_form($form, &$form_state, $list = NULL) {
  // Make the first $roles checkbox a select-all.
  $js_setting['ManyMailCheckAll'] = array(
    'list' => '#edit-list-roles',
    'key' => '#edit-list-roles-' . DRUPAL_AUTHENTICATED_RID,
  );
  drupal_add_js($js_setting, 'setting');
  drupal_add_js(drupal_get_path('module', 'manymail') . '/misc/checkall.js');

  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('List details'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['details']['display_name'] = array(
    '#type' => 'textfield',
    '#title' => t('List name'),
    '#description' => t('The name under which this list will appear in the selection table.'),
    '#size' => 32,
    '#required' => TRUE,
  );

  $form['details']['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'manymail_lists_list_exists',
      'source' => array('details', 'display_name'),
    ),
    '#description' => t('A unique machine-readable name for this list. It must only contain lowercase letters, numbers, and underscores.'),
    '#disabled' => FALSE,
  );

  $form['details']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t("Describe who's in this list. This description will show up in the list selection table."),
    '#required' => TRUE,
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('List roles'),
    '#description' => t('Add certain user roles to the recipient list.'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Select all but anonymous role.
  $roles = user_roles(TRUE);
  array_walk($roles, 'check_plain');

  $form['roles']['list_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('List roles'),
    '#title_display' => 'invisible',
    '#options' => $roles,
  );

  $form['custom'] = array(
    '#attributes' => array(
      'id' => 'add-custom-recipients',
    ),
    '#type' => 'fieldset',
    '#title' => t('Custom recipients'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['custom']['add_recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Add non-user recipients'),
    '#description' => t('Each recipient should be on a separate line, following the pattern: mail|name (e.g.: jd@example.com|John Doe).<br /><strong>Note: </strong>Any e-mail address entered here that already exists within the user records will be ignored.'),
    '#weight' => 4,
  );

  $form['custom']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#description' => t('Select how you want to treat conflicting lines.'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['custom']['advanced']['option_incorrect'] = array(
    '#type' => 'radios',
    '#title' => t('Incorrectly formatted lines'),
    '#options' => array(
      'warn' => t('Warn'),
      'discard' => t('Discard silently'),
    ),
    '#default_value' => 'warn',
  );

  $form['custom']['advanced']['option_duplicate'] = array(
    '#type' => 'radios',
    '#title' => t('Duplicate entries'),
    '#options' => array(
      'warn' => t('Warn'),
      'discard' => t('Discard silently (use first occurence)'),
    ),
    '#default_value' => 'warn',
  );

  $form['custom']['advanced']['option_list_duplicate'] = array(
    '#type' => 'radios',
    '#title' => t('Recipients that were already in this list'),
    '#options' => array(
      'warn' => t('Warn'),
      'discard' => t('Discard silently'),
      'update' => t('Update with entered data'),
    ),
    '#default_value' => 'warn',
  );

  $form['custom']['advanced']['option_data_duplicate'] = array(
    '#type' => 'radios',
    '#title' => t('Recipients that were already in another list'),
    '#options' => array(
      'warn' => t('Warn'),
      'add' => t('Add to this list with the data from the other list'),
      'add_update' => t('Add to this list and update the other list with the entered data'),
    ),
    '#default_value' => 'warn',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save list'),
    '#weight' => 5,
  );

  if (!empty($list)) {
    // Save the list name for the submission handler.
    $form['manymail_list'] = array(
      '#type' => 'value',
      '#value' => $list,
    );

    // Retrieve the metadata for this list.
    $lists = _manymail_lists_all_lists();
    $data = $lists[$list];

    // Retrieve the selected roles for this list.
    $query = db_select('manymail_lists_roles', 'm');
    $query->addField('m', 'rid');
    $query->condition('m.list', $list);
    $roles = $query->execute()->fetchAllKeyed(0, 0);

    // Retrieve the custom recipients for this list.
    $query = db_select('manymail_lists_mail', 'ma')->extend('PagerDefault')->extend('TableSort');
    $query->join('manymail_lists_mail_meta', 'mt', 'mt.mail=ma.mail');
    $query->addField('ma', 'mail');
    $query->addField('mt', 'name');
    $query->condition('ma.list', $list);

    // Add the sortable header to the table.
    $header = array(
      'mail' => array(
        'data' => t('E-mail address'),
        'field' => 'ma.mail',
      ),
      'name' => array(
        'data' => t('Name'),
        'field' => 'mt.name',
        'sort' => 'asc',
      ),
    );
    $query->limit(10);
    $query->orderByHeader($header);

    $options = $query->execute()->fetchAllAssoc('mail', PDO::FETCH_ASSOC);

    // Set the default values.
    $form['details']['display_name']['#default_value'] = $data->display_name;
    $form['details']['name']['#default_value'] = $data->name;
    $form['details']['name']['#disabled'] = TRUE;
    $form['details']['description']['#default_value'] = $data->description;
    $form['roles']['list_roles']['#default_value'] = $roles;
  }

  // Show the table if there are custom recipients.
  if (!empty($options)) {
    $form['custom']['custom_recipients'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#js_select' => TRUE,
      '#weight' => 1,
    );

    $form['custom']['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 2,
    );

    $form['custom']['delete_recipients'] = array(
      '#type' => 'button',
      '#value' => t('Delete selected'),
      '#validate' => array('manymail_lists_delete_recipients_button'),
      '#limit_validation_errors' => array(),
      '#weight' => 3,
    );
  }

  return $form;
}

/**
 * Implements submission from the Form API.
 *
 * @see manymail_lists_modify_form()
 */
function manymail_lists_modify_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['manymail_list']) && $list = $form_state['values']['manymail_list']) {
    // Update name and description.
    db_update('manymail_lists')
      ->fields(array(
        'display_name' => $form_state['values']['display_name'],
        'description' => $form_state['values']['description'],
      ))
      ->condition('name', $list)
      ->execute();

    // Delete all previously associated roles.
    db_delete('manymail_lists_roles')
      ->condition('list', $list)
      ->execute();
  }
  else {
    // We already know the list name is valid, so insert it.
    db_insert('manymail_lists')
      ->fields(array(
        'name' => $form_state['values']['name'],
        'display_name' => $form_state['values']['display_name'],
        'description' => $form_state['values']['description'],
      ))
      ->execute();
  }

  // Save the role IDs to the database.
  // Because this is a many-many table: skip if no roles.
  $roles = array_filter($form_state['values']['list_roles']);

  if ($roles) {
    $query = db_insert('manymail_lists_roles')
      ->fields(array('list', 'rid'));

    foreach ($roles as $role) {
      $query->values(array(
        'list' => $form_state['values']['name'],
        'rid' => $role,
      ));

      // If authenticated user, the mail function of Lists will
      // include all users anyway, so stop the loop right here.
      if ($role == DRUPAL_AUTHENTICATED_RID) {
        break;
      }
    }

    $query->execute();
  }

  // Add all validated custom recipients to this list.
  if (!empty($form_state['manymail_lists_recipients'])) {
    $query = db_insert('manymail_lists_mail')
      ->fields(array('list', 'mail'));

    foreach ($form_state['manymail_lists_recipients'] as $mail) {
      $query->values(array(
        'list' => $form_state['values']['name'],
        'mail' => $mail,
      ));
    }

    $query->execute();
  }

  // Update the recipient metadata table.
  //
  // Delete existing and insert as a whole since db_merge()
  // would execute WAY too many queries.
  if (!empty($form_state['manymail_lists_recip_meta'])) {
    $recipient_meta = $form_state['manymail_lists_recip_meta'];

    db_delete('manymail_lists_mail_meta')
      ->condition('mail', array_keys($recipient_meta), 'IN')
      ->execute();

    $query = db_insert('manymail_lists_mail_meta')
      ->fields(array('mail', 'name'));

    foreach ($recipient_meta as $mail => $meta) {
      $query->values(array('mail' => $mail) + $meta);
    }

    $query->execute();
  }

  // Finally, redirect to the list management page.
  drupal_goto('admin/config/manymail/targets/lists/manage');
}

/**
 * Implements validation from the Form API.
 *
 * @see manymail_lists_modify_form()
 */
function manymail_lists_delete_recipients_button($form, &$form_state) {
  if (!empty($form_state['values']['manymail_list']) && $list = $form_state['values']['manymail_list']) {
    $to_delete = array_filter($form_state['values']['custom_recipients']);

    if ($to_delete) {
      manymail_lists_detach_custom_recipients($list, $to_delete);
    }
  }
}

/**
 * Implements validation from the Form API.
 *
 * @see manymail_lists_modify_form()
 */
function manymail_lists_modify_form_validate($form, &$form_state) {
  // Make line endings uniform for the explode() call.
  $recipients = preg_replace("/(\r\n|\r)+/", "\n", $form_state['values']['add_recipients']);
  $recipients = explode("\n", $recipients);

  // Retrieve the advanced options.
  $opt_incorrect = $form_state['values']['option_incorrect'];
  $opt_duplicate = $form_state['values']['option_duplicate'];
  $opt_list_duplicate = $form_state['values']['option_list_duplicate'];
  $opt_data_duplicate = $form_state['values']['option_data_duplicate'];

  // Used to store the validated data.
  $valid_mail = array();
  $valid_meta = array();

  // Used to store incorrectly formatted lines.
  $incorrect = array();

  // Used to store duplicate entries for the textarea,
  // the current list or the recipient metadata table.
  $lookup = array();
  $duplicates = array();
  $list_duplicates = array();
  $data_duplicates = array();

  // Retrieve e-mail addresses that are already in this list.
  if (!empty($form_state['values']['manymail_list']) && $list = $form_state['values']['manymail_list']) {
    $query = db_select('manymail_lists_mail', 'ma');
    $query->addField('ma', 'mail');
    $query->condition('ma.list', $list);
    $list_recipients = $query->execute()->fetchCol();

    // ManyMail Lists can store e-mail addresses in upper case,
    // so we convert them to lower case to check for duplicates.
    array_map('strtolower', $list_recipients);
  }

  // Retrieve e-mail addresses from the metadata table.
  $query = db_select('manymail_lists_mail_meta', 'mt');
  $query->addField('mt', 'mail');
  $data_recipients = $query->execute()->fetchCol();

  // ManyMail Lists can store e-mail addresses in upper case,
  // so we convert them to lower case to check for duplicates.
  array_map('strtolower', $data_recipients);

  foreach ($recipients as $recipient) {
    // Skip entirely if the string is empty;
    if (empty($recipient)) {
      continue;
    }

    // Skip if pipe character was not found.
    if (strpos($recipient, '|') === FALSE) {
      $incorrect[] = $recipient;
      continue;
    }

    $recipient_data = explode('|', $recipient);

    // Skip if more than one pipe character was found.
    if (count($recipient_data) > 2) {
      $incorrect[] = $recipient;
      continue;
    }

    // Make sure the provided e-mail address is valid.
    $mail = check_plain(filter_var($recipient_data[0], FILTER_VALIDATE_EMAIL));
    $name = check_plain($recipient_data[1]);

    // Lowercase the e-mail address to check for duplicates.
    $mail_lc = strtolower($mail);

    if ($mail_lc) {
      // Check for duplicates in the textarea.
      if (in_array($mail_lc, $lookup)) {
        $duplicates[] = $mail;
        continue;
      }
      $lookup[] = $mail_lc;

      // Check for duplicates in this list.
      if (!empty($list_recipients) && in_array($mail_lc, $list_recipients)) {
        $list_duplicates[] = $mail;

        if ($opt_list_duplicate == 'update') {
          $valid_meta[$mail_lc] = array('name' => $name);
        }

        continue;
      }

      // Check for duplicates in the recipient metadata table.
      if (in_array($mail_lc, $data_recipients)) {
        $data_duplicates[] = $mail;

        if ($opt_data_duplicate == 'add_update' || $opt_data_duplicate == 'add') {
          $valid_mail[] = $mail;
        }

        if ($opt_data_duplicate == 'add_update') {
          $valid_meta[$mail_lc] = array('name' => $name);
        }

        continue;
      }

      $valid_mail[] = $mail;
      $valid_meta[$mail_lc] = array('name' => $name);
    }
    else {
      // E-mail address was invalid.
      $incorrect[] = $recipient;
    }
  }

  // Throw a warning for incorrectly formatted lines.
  if ($incorrect && $opt_incorrect == 'warn') {
    $bad_lines = '';
    foreach ($incorrect as $line) {
      $bad_lines .= '<li>' . check_plain($line) . '</li>';
    }

    form_set_error('add_recipients', t(
      'The following custom recipient lines were incorrectly formatted: !lines',
      array('!lines' => "<ul>$bad_lines</ul>")
    ));
  }

  // Throw a warning for duplicates in the textarea.
  elseif ($duplicates && $opt_duplicate == 'warn') {
    $duplicates = array_unique($duplicates);
    form_set_error('add_recipients', t(
      'The following e-mail addresses were entered multiple times: @duplicates.',
      array('@duplicates' => implode(', ', $duplicates))
    ));
  }

  // Throw a warning for duplicates in this list.
  elseif ($list_duplicates && $opt_list_duplicate == 'warn') {
    form_set_error('add_recipients', t(
      'The following e-mail addresses were already in this list: @duplicates.',
      array('@duplicates' => implode(', ', $list_duplicates))
    ));
  }

  // Throw a warning for duplicates in the recipient metadata table.
  elseif ($data_duplicates && $opt_data_duplicate == 'warn') {
    form_set_error('add_recipients', t(
      'The following e-mail addresses were already in another list: @duplicates.',
      array('@duplicates' => implode(', ', $data_duplicates))
    ));
  }

  // Pass the recipient data to the submission handler.
  else {
    $form_state['manymail_lists_recipients'] = $valid_mail;
    $form_state['manymail_lists_recip_meta'] = $valid_meta;
  }
}

/**
 * Form function for deleting a recipient list.
 *
 * @param array $form
 *   As passed in drupal_get_form().
 * @param array $form_state
 *   As passed in drupal_get_form().
 * @param string $list
 *   The unique machine name of the list.
 *
 * @ingroup forms
 */
function manymail_lists_delete_form($form, &$form_state, $list) {
  if (isset($form_state['values']['op']) && $form_state['values']['op'] == $form_state['values']['cancel']) {
    drupal_goto('admin/config/manymail/targets/lists/manage');
  }

  // Save the list name for the submission handler.
  $form['manymail_list'] = array(
    '#type' => 'value',
    '#value' => $list,
  );

  // Retrieve the data for this list.
  $lists = _manymail_lists_all_lists();
  $data = $lists[$list];

  $form['message'] = array(
    '#type' => 'item',
    '#title' => t('Confirm deletion'),
    '#description' => t('Are you sure you want to delete the following list: <strong>@list</strong>?<br />Custom recipients that are no longer in use will be deleted along with it.', array('@list' => $data->display_name)),
  );

  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

/**
 * Implements submission from the Form API.
 *
 * @see manymail_lists_targets_lists_manage_delete_form()
 */
function manymail_lists_delete_form_submit($form, &$form_state) {
  $list = $form_state['values']['manymail_list'];

  db_delete('manymail_lists_roles')
    ->condition('list', $list)
    ->execute();

  db_delete('manymail_lists_mail')
    ->condition('list', $list)
    ->execute();

  db_delete('manymail_lists')
    ->condition('name', $list)
    ->execute();

  manymail_lists_garbage_collect_recipients();
  drupal_goto('admin/config/manymail/targets/lists/manage');
}
