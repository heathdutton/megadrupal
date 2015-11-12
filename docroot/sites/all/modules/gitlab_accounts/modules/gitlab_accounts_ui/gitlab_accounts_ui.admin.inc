<?php

/**
 * @file
 * Administer pages for GitLab Accounts UI module.
 */

/**
 * Page callback for accounts overview.
 */
function gitlab_accounts_ui_accounts_admin_page() {
  // Load available accounts.
  $accounts = gitlab_accounts_load_accounts();

  if (!empty($accounts)) {
    $table = array(
      '#theme' => 'table',
      '#header' => array(
        array('data' => t('Remote Id'), 'width' => '6%'),
        array('data' => t('Account')),
        array('data' => t('Endpoint URL')),
        array('data' => t('Operations'), 'width' => '5%', 'colspan' => 2),
      ),
      '#rows' => array(),
      '#caption' => NULL,
      '#colgroups' => array(),
      '#sticky' => FALSE,
      '#empty' => t('No GitLab accounts have been added yet.'),
    );

    foreach ($accounts as $id => $account) {
      $table['#rows'][] = array(
        'data' => array(
          $account->remote_id,
          check_plain($account->name) . ' (' . check_plain($account->username) . ')',
          check_plain($account->endpoint_url),
          l(t('edit'), 'admin/config/services/gitlab-accounts/account/' . $id . '/edit'),
          l(t('delete'), 'admin/config/services/gitlab-accounts/account/' . $id . '/delete'),
        ),
      );
    }

    return drupal_render($table);
  }

  $message = array(
    '#markup' => '<p>' . t('No GitLab accounts have been added yet.') . '</p>',
  );

  return drupal_render($message);
}

/**
 * Page callback for add account form.
 */
function gitlab_accounts_ui_add_account_form($form, $form_state) {
  // Modify page title.
  drupal_set_title(t('Add account'));

  $form['endpoint_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint URL'),
    '#description' => t('The endpoint URL used to connect to the GitLab account.'),
    '#default_value' => 'https://gitlab.com/api/v3',
    '#required' => TRUE,
  );
  $form['private_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Private token'),
    '#description' => t("The GitLab account's private token."),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save account'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), GITLAB_ACCOUNTS_UI_ADMIN_PAGE_URL),
  );

  return $form;
}

/**
 * Validate callback for add account form.
 *
 * @see gitlab_accounts_ui_add_account_form()
 */
function gitlab_accounts_ui_add_account_form_validate($form, &$form_state) {
  // Try to load the GitLab account with the private token.
  $account = gitlab_users_get_current_user($form_state['values']['private_token'], array(
    'wsdata' => array(
      'endpoint' => $form_state['values']['endpoint_url'],
    ),
  ));

  if (empty($account['data'])) {
    form_set_error('not_existing_key', t('Cannot connect to the GitLab account with the provided settings.'));
  }
  else {
    $existing_account = gitlab_accounts_load_account(NULL, array(
      'remote_id' => $account['data']['id'],
      'endpoint_url' => $form_state['values']['endpoint_url'],
    ));
    // Check if the account already has been added.
    if (!empty($existing_account)) {
      form_set_error('not_existing_key', t('The GitLab account has been added already.'));
    }
    else {
      $form_state['new_account'] = $account['data'];
    }
  }
}

/**
 * Submit callback for add account form.
 *
 * @see gitlab_accounts_ui_add_account_form()
 */
function gitlab_accounts_ui_add_account_form_submit($form, &$form_state) {
  // Create a new Gitlab account entity.
  $new_account = entity_create('gitlab_account', array(
    'remote_id' => $form_state['new_account']['id'],
    'name' => $form_state['new_account']['name'],
    'username' => $form_state['new_account']['username'],
    'endpoint_url' => $form_state['values']['endpoint_url'],
  ));
  // Encrypt and set the private token.
  $new_account->setPrivateToken($form_state['values']['private_token']);
  // Save the new account entity.
  $new_account->save();

  drupal_set_message(t('The new account has been saved.'));

  // Redirect the user to the accounts overview page.
  $form_state['redirect'] = GITLAB_ACCOUNTS_UI_ADMIN_PAGE_URL;
}

/**
 * Page callback for edit account form.
 */
function gitlab_accounts_ui_edit_account_form($form, $form_state, $account) {
  // Save the account for later usage.
  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['remote_id'] = array(
    '#type' => 'item',
    '#title' => t('Remote ID'),
    '#markup' => $account->remote_id,
  );
  $form['name'] = array(
    '#type' => 'item',
    '#title' => t('Account'),
    '#markup' => check_plain($account->name . ' (' . $account->username . ')'),
  );
  $form['endpoint_url'] = array(
    '#type' => 'item',
    '#title' => t('Endpoint URL'),
    '#markup' => check_plain($account->endpoint_url),
  );
  $form['private_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Private token'),
    '#description' => t("The GitLab account's private token."),
    '#default_value' => $account->getPrivateToken(),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update account'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), GITLAB_ACCOUNTS_UI_ADMIN_PAGE_URL),
  );

  return $form;
}

/**
 * Validate callback for edit account form.
 *
 * @see gitlab_accounts_ui_edit_account_form()
 */
function gitlab_accounts_ui_edit_account_form_validate($form, &$form_state) {
  // Try to load the GitLab account with the provided private token.
  $account = gitlab_users_get_current_user($form_state['values']['private_token'], array(
    'wsdata' => array(
      'endpoint' => $form_state['values']['account']->endpoint_url,
    ),
  ));

  if (empty($account['data'])) {
    form_set_error('private_token', t('Cannot connect to the GitLab account with the provided private token.'));
  }
  // Check if the edited account's id is the same as the account id belongs to
  // the modified private token.
  elseif ($account['data']['id'] != $form_state['values']['account']->remote_id) {
    form_set_error('private_token', t('The private token related GitLab account different from the edited one.'));
  }
}

/**
 * Submit callback for edit account form.
 *
 * @see gitlab_accounts_ui_edit_account_form()
 */
function gitlab_accounts_ui_edit_account_form_submit($form, &$form_state) {
  $updated_account = $form_state['values']['account'];
  // Update the account's private token.
  $updated_account->setPrivateToken($form_state['values']['private_token']);
  // Save the modified account.
  $updated_account->save();

  drupal_set_message(t('The account has been updated.'));

  // Redirect the user to the accounts overview page.
  $form_state['redirect'] = GITLAB_ACCOUNTS_UI_ADMIN_PAGE_URL;
}

/**
 * Page callback for delete account form.
 */
function gitlab_accounts_ui_delete_account_form($form, $form_state, $account) {
  // Save the account for later usage.
  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );
  $form['remote_id'] = array(
    '#type' => 'item',
    '#title' => t('Remote ID'),
    '#markup' => $account->remote_id,
  );
  $form['name'] = array(
    '#type' => 'item',
    '#title' => t('Account'),
    '#markup' => check_plain($account->name . ' (' . $account->username . ')'),
  );
  $form['endpoint_url'] = array(
    '#type' => 'item',
    '#title' => t('Endpoint URL'),
    '#markup' => check_plain($account->endpoint_url),
  );

  return confirm_form($form, t('Are you sure you want to delete the following account?'), GITLAB_ACCOUNTS_UI_ADMIN_PAGE_URL, '', 'Delete');
}

/**
 * Submit callback for delete account form.
 *
 * @see gitlab_accounts_ui_delete_account_form()
 */
function gitlab_accounts_ui_delete_account_form_submit($form, &$form_state) {
  entity_delete('gitlab_account', $form_state['values']['account']->id);

  drupal_set_message(t('The account has been deleted.'));
  // Redirect the user to the accounts overview page.
  $form_state['redirect'] = GITLAB_ACCOUNTS_UI_ADMIN_PAGE_URL;
}
