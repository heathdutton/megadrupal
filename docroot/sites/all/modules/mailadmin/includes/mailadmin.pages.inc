<?php
/**
 * @file
 * Pages and forms to manage mail adresses for the mailadmin module.
 */

/**
 * Main overview page for mailadmin.
 */
function mailadmin_overview_page() {
  global $user;
  $build = array();

  $implementers = module_implements('mailadmin_domains');
  if (empty($implementers)) {
    drupal_set_message(t('The mailadmin module depends on other modules to provide a list of domains to provide e-mail mailboxes for. You need to enable one of these modules for this page to be useful.'), 'error');
  }

  $domains = array();
  foreach (module_invoke_all('mailadmin_domains', $user) as $domain) {
    $domains[$domain] = 0;
  }

  if (empty($domains)) {
    $build[] = array(
      '#markup' => t('You have no domains to manage e-mail addresses for.'),
    );
  }
  else {
    $counts = db_query("SELECT COUNT(mailbox_id) AS mailboxes, domain_name FROM {mailadmin_mailboxes} WHERE domain_name IN (:domain_names) GROUP BY domain_name", array(':domain_names' => array_keys($domains)))->fetchAllAssoc('domain_name');
    $aliases = db_query("SELECT source_domain_name, target_domain_name FROM {mailadmin_domain_aliases} WHERE source_domain_name IN (:domain_names)", array(':domain_names' => array_keys($domains)))->fetchAll();
    $alias_counts = $domains;
    foreach ($aliases as $alias) {
      if (isset($domains[$alias->source_domain_name])) {
        unset($domains[$alias->source_domain_name]);
      }
      if (isset($alias_counts[$alias->target_domain_name])) {
        $alias_counts[$alias->target_domain_name] += 1;
      }
    }
    $header = array(t('Domain name'), t('Number of mailboxes'), t('Number of aliases'));
    $rows = array();
    foreach ($domains as $domain => $value) {
      $rows[] = array(
        l($domain, 'mailadmin/domain/' . $domain),
        isset($counts[$domain]) ? $counts[$domain]->mailboxes : 0,
        isset($alias_counts[$domain]) ? $alias_counts[$domain] : 0,
      );
    }
    if ($rows) {
      $build['domains'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array('class' => array('domains')),
        '#prefix' => '<h2>' . t('Select a domain to configure e-mail for') . '</h2>',
      );
    }
  }

  $build['aliases'] = array(
    '#prefix' => '<div class="alias"><h2>' . t('Domain aliases') . '</h2>',
    '#suffix' => '</div>',
    'create_alias' => array(
      '#markup' => l('Add new alias', 'mailadmin/alias/add'),
    ),
    'aliases' => array(
      '#markup' => mailadmin_embed_view('mailadmin_alias_table', 'master'),
    ),
  );

  return $build;
}

/**
 * Domain overview page
 *
 * List all configured e-mail addresses for a domain.
 */
function mailadmin_domain_page($domain_name) {
  global $user;

  // If domain is not in the user's list of domain, return a not found.
  if (!in_array($domain_name, module_invoke_all('mailadmin_domains', $user))) {
    return drupal_not_found();
  }

  drupal_set_title(t('E-mail addresses for “@name”', array('@name' => $domain_name)));

  // Build the final render arrays.
  $build = array();

  $build['create_mailbox'] = array(
    '#markup' => l('Add new mailbox', 'mailadmin/mailbox/add', array(
      'query' => array('domain' => $domain_name),
    )),
  );

  $build['mailboxes'] = array(
    '#markup' => mailadmin_embed_view('mailadmin_mailbox_table', 'master', array($domain_name)),
  );

  $build['create_forward'] = array(
    '#markup' => l('Add new forwarding-only address', 'mailadmin/forward/add', array(
      'query' => array('domain' => $domain_name),
    )),
  );

    $build['forwards'] = array(
      '#markup' => mailadmin_embed_view('mailadmin_forward_table', 'master', array($domain_name)),
    );

  return $build;
}

/**
 * Display a table of mailboxes.
 */
function mailadmin_mailbox_page() {
  $build = array();

  $build['create_mailbox'] = array(
    '#markup' => l('Add new mailbox', 'mailadmin/mailbox/add'),
  );

  $build['mailboxes'] = array(
    '#markup' => mailadmin_embed_view('mailadmin_mailbox_table', 'master'),
  );

  return $build;
}

/**
 * Display a table of forwards.
 */
function mailadmin_forward_page() {
  $build = array();

  $build['create_mailbox'] = array(
    '#markup' => l('Add new mailbox', 'mailadmin/forward/add'),
  );

  $build['mailboxes'] = array(
    '#markup' => mailadmin_embed_view('mailadmin_forward_table', 'master'),
  );

  return $build;
}


/**
 * Editing form for mailboxs.
 */
function mailadmin_mailbox_edit_form($form, &$form_state, $mailbox = NULL) {
  $form['#attached']['css'][] = drupal_get_path('module', 'mailadmin') . '/css/mailadmin.admin.css';
  global $user;

  if (is_null($mailbox)) {
    $mailbox = mailadmin_mailbox_new();
    $mailbox->domain_name = (isset($_GET['domain'])) ? $_GET['domain'] : '';
  }

  $form['mailbox'] = array(
    '#type' => 'value',
    '#value' => $mailbox,
  );

  $form['address'] = array(
    '#prefix' => '<div class="address clearfix">',
    '#suffix' => '</div>',
  );

  $form['address']['local_part'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#description' => t('The first part of the e-mail address, before the @-sign.'),
    '#required' => TRUE,
    '#default_value' => $mailbox->local_part,
    '#suffix' => '<div class="form-item at">@</div>',
  );

  $form['address']['domain_name'] = array(
    '#type' => 'select',
    '#title' => t('Domain name'),
    '#options' => drupal_map_assoc(module_invoke_all('mailadmin_domains', $user)),
    '#required' => TRUE,
    '#default_value' => $mailbox->domain_name,
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => TRUE,
  );

  $form['real_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Real name'),
    '#default_value' => $mailbox->real_name,
  );

  $form['buttons'] = array();

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );
  
  if (!empty($mailbox->is_new)) {
    $form['buttons']['save']['#value'] = t('Create mailbox');
  }
  else {
    $form['buttons']['delete'] = array(
      '#markup' => l(t('Delete this mailbox'), 'mailadmin/mailbox/' . $mailbox->mailbox_id . '/delete'),
    );

    // When editing an existing entry, the semantics of the password changes.
    $form['password']['#title'] = t('New password');
    $form['password']['#description'] = t('To change the user’s password, enter a new one here.');
    $form['password']['#required'] = FALSE;
  }

  return $form;
}

/**
 * Submit handler saving a mailbox, new or existing.
 */
function mailadmin_mailbox_edit_form_submit($form, &$form_state) {
  $mailbox = $form_state['values']['mailbox'];

  $mailbox->local_part = $form_state['values']['local_part'];
  $mailbox->domain_name = $form_state['values']['domain_name'];
  $mailbox->real_name = $form_state['values']['real_name'];

  $address = $mailbox->local_part . '@' . $mailbox->domain_name;

  if (isset($form_state['values']['password']) && !empty($form_state['values']['password'])) {
    $mailbox->password = mailadmin_hash_password($form_state['values']['password']);
    $password_message = t('Password for “%address” set to “%pass”. This password is not stored in plain text and cannot be retrieved later.', array('%address' => $address, '%pass' => $form_state['values']['password']));
  }

  $saved = mailadmin_mailbox_save($mailbox);

  if ($saved === SAVED_NEW) {
    drupal_set_message($password_message);
    drupal_set_message(t('Mailbox “%name” was created.', array('%name' => $address)));
    watchdog('mailadmin', 'Mailbox “%name” was created.', array('%name' => $address));
  }
  elseif ($saved === SAVED_UPDATED) {
    if (!empty($password_message)) {
      drupal_set_message($password_message);
    }
    drupal_set_message(t('Mailbox “%name” was updated.', array('%name' => $address)));
    watchdog('mailadmin', 'Mailbox “%name” was updated.', array('%name' => $address));
  }
  else {
    drupal_set_message(t('Failed to save mailbox “%name”.', array('%name' => $address)), 'error');
  }

  $form_state['redirect'] = 'mailadmin/domain/' . $mailbox->domain_name;
}

/**
 * Confirmation form to delete mailboxes.
 */
function mailadmin_mailbox_delete_form($form, &$form_state, $mailbox) {
  $form['mailbox'] = array(
    '#type' => 'value',
    '#value' => $mailbox,
  );
  $question = t('Are you sure you want to delete the mailbox?');
  $description = t('This action cannot be undone.');
  $path = 'mailadmin/mailbox/' . $mailbox->mailbox_id;
  $yes = t('Delete');
  $no = t('Cancel');
  return confirm_form($form, $question, $path, "<p>$question</p><p>$description</p>", $yes, $no, 'confirm');
}

/**
 * Submit handler for mailadmin_mailbox_delete_form().
 */
function mailadmin_mailbox_delete_form_submit(&$form, &$form_state) {
  $mailbox = $form_state['values']['mailbox'];
  mailadmin_mailbox_delete($mailbox->mailbox_id);

  $form_state['redirect'] = 'mailadmin/domain/' . $mailbox->domain_name;

  $address = $mailbox->local_part . '@' . $mailbox->domain_name;
  watchdog('mailadmin', 'Mailbox “%name” was deleted.', array('%name' => $address));
}

/**
 * Form for creating and editing mail forwards.
 */
function mailadmin_forward_form($form, &$form_state, $forward = NULL) {
  $form['#attached']['css'][] = drupal_get_path('module', 'mailadmin') . '/css/mailadmin.admin.css';
  global $user;

  if (is_null($forward)) {
    $forward = mailadmin_forward_new();
    $forward->domain_name = (isset($_GET['domain'])) ? $_GET['domain'] : '';
  }

  $form['forward'] = array(
    '#type' => 'value',
    '#value' => $forward,
  );

  $form['address'] = array(
    '#prefix' => '<div class="address clearfix">',
    '#suffix' => '</div>',
  );

  $form['address']['local_part'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#description' => t('The first part of the e-mail address, before the @-sign.'),
    '#required' => TRUE,
    '#default_value' => $forward->local_part,
    '#suffix' => '<div class="form-item at">@</div>',
  );

  $form['address']['domain_name'] = array(
    '#type' => 'select',
    '#title' => t('Domain name'),
    '#options' => drupal_map_assoc(module_invoke_all('mailadmin_domains', $user)),
    '#required' => TRUE,
    '#default_value' => $forward->domain_name,
  );

  $form['destination'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination'),
    '#description' => t('The destination for the mail forward.'),
    '#maxlength' => 254,
    '#default_value' => $forward->destination,
  );

  $form['buttons'] = array();

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  if (!empty($forward->is_new)) {
    $form['buttons']['save']['#value'] = t('Create');
  }
  else {
    $form['buttons']['delete'] = array(
      '#markup' => l(t('Delete this forwarding-only address'), 'mailadmin/forward/' . $forward->forward_id . '/delete'),
    );
  }

  return $form;
}

/**
 * Submit handler for mailadmin_forward_form().
 */
function mailadmin_forward_form_submit(&$form, &$form_state) {
  $forward = $form_state['values']['forward'];

  $forward->local_part = $form_state['values']['local_part'];
  $forward->domain_name = $form_state['values']['domain_name'];
  $forward->destination = $form_state['values']['destination'];

  $address = $forward->local_part . '@' . $forward->domain_name;

  $saved = mailadmin_forward_save($forward);

  if ($saved === SAVED_NEW) {
    drupal_set_message(t('Forwarding-only address “%name” was created.', array('%name' => $address)));
    watchdog('mailadmin', 'Forwarding-only address “%name” was created.', array('%name' => $address));
  }
  elseif ($saved === SAVED_UPDATED) {
    drupal_set_message(t('Forwarding-only address “%name” was updated.', array('%name' => $address)));
    watchdog('mailadmin', 'Forwarding-only address “%name” was updated.', array('%name' => $address));
  }
  else {
    drupal_set_message(t('Failed to save forwarding-only address “%name”.', array('%name' => $address)), 'error');
  }

  $form_state['redirect'] = 'mailadmin/domain/' . $forward->domain_name;
}

/**
 * Confirmation form to delete forwards.
 */
function mailadmin_forward_delete_form($form, &$form_state, $forward) {
  $form['forward'] = array(
    '#type' => 'value',
    '#value' => $forward,
  );
  $question = t('Are you sure you want to delete the forwarding-only address?');
  $description = t('This action cannot be undone.');
  $path = 'mailadmin/forward/' . $forward->forward_id;
  $yes = t('Delete');
  $no = t('Cancel');
  return confirm_form($form, $question, $path, "<p>$question</p><p>$description</p>", $yes, $no, 'confirm');
}

/**
 * Submit handler for mailadmin_forward_delete_form().
 */
function mailadmin_forward_delete_form_submit(&$form, &$form_state) {
  $forward = $form_state['values']['forward'];
  mailadmin_forward_delete($forward->forward_id);

  $form_state['redirect'] = 'mailadmin/domain/' . $forward->domain_name;

  $address = $forward->local_part . '@' . $forward->domain_name;
  watchdog('mailadmin', 'Forwarding-only address “%name” was deleted.', array('%name' => $address));
}

/**
 * Form for creating and editing aliases.
 */
function mailadmin_alias_form($form, &$form_state, $alias = NULL) {
  global $user;

  if (empty($alias)) {
    $alias = mailadmin_alias_new();
  }

  $form['source'] = array(
    '#type' => 'select',
    '#title' => t('Source domain name'),
    '#options' => drupal_map_assoc(module_invoke_all('mailadmin_domains', $user)),
    '#required' => TRUE,
    '#default_value' => $alias->source_domain_name,
    '#description' => t('The source of the domain alias, the domain receiving emails to be forwarded to another domain'),
  );

  $form['target'] = array(
    '#type' => 'select',
    '#title' => t('Target domain name'),
    '#options' => drupal_map_assoc(module_invoke_all('mailadmin_domains', $user)),
    '#required' => TRUE,
    '#default_value' => $alias->target_domain_name,
    '#description' => t('The target of the domain alias, the domain to receive the emails'),
  );

  $form['buttons'] = array();

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  if (!empty($alias->is_new)) {
    $form['buttons']['save']['#value'] = t('Create');
  }
  else {
    $form['buttons']['delete'] = array(
      '#markup' => l(t('Delete this domain alias'), 'mailadmin/alias/' . $alias->alias_id . '/delete'),
    );
  }

  $form_state['alias'] = $alias;

  return $form;
}

/**
 * Validate handler for mailadmin_alias_form().
 */
function mailadmin_alias_form_validate(&$form, &$form_state) {
  // Make sure the domains are different.
  if ($form_state['values']['source'] == $form_state['values']['target']) {
    form_set_error('target', t('Target must be different from source'));
  }
  // Make sure target domain is not an alias.
  $target_query = db_select('mailadmin_domain_aliases')
    ->condition('source_domain_name', $form_state['values']['target']);
  if ($form_state['alias']->alias_id) {
    $target_query->condition('alias_id', $form_state['alias']->alias_id, '<>');
  }
  $target_condition = $target_query->countQuery()
    ->execute()
    ->fetch();
  if ($target_condition->expression > 0) {
    form_set_error('target', t('Target is already being used as an alias source. Alias sources are not allowed to be an alias target.'));
  }
  // Make sure the source is not in use already in another alias.
  $source_query = db_select('mailadmin_domain_aliases')
    ->condition('source_domain_name', $form_state['values']['source']);
  if ($form_state['alias']->alias_id) {
    $source_query->condition('alias_id', $form_state['alias']->alias_id, '<>');
  }
  $source_condition = $source_query->countQuery()
    ->execute()
    ->fetch();
  if ($target_condition->expression > 0) {
    form_set_error('source', t('You have already create an alias for this domain, edit that one instead.'));
  }
  // Make sure no mailboxes are on the source.
  $mailbox_condition = db_select('mailadmin_mailboxes')
    ->condition('domain_name', $form_state['values']['source'])
    ->countQuery()
    ->execute()
    ->fetch();
  if ($mailbox_condition->expression > 1) {
    form_set_error('source', t('You have mailboxes created for the source domain name. You will have to delete these first before you can create a domain alias with this domain name as the source.'));
  }
}

/**
 * Submit handler for mailadmin_alias_form().
 */
function mailadmin_alias_form_submit(&$form, &$form_state) {
  $alias = $form_state['alias'];
  $alias->source_domain_name = $form_state['values']['source'];
  $alias->target_domain_name = $form_state['values']['target'];
  $saved = mailadmin_alias_save($alias);
  if ($saved === SAVED_NEW || $saved === SAVED_UPDATED) {
    drupal_set_message(t('E-mail domain alias from “%source” to “%target” was created.',
      array('%source' => $form_state['values']['source'], '%target' => $form_state['values']['target'])
    ));
    watchdog('mailadmin', 'E-mail domain alias from “%source” to “%target” was created.',
      array('%source' => $form_state['values']['source'], '%target' => $form_state['values']['target'])
    );
  }
  else {
    drupal_set_message(t('Failed to save E-mail domain alias from “%source” to “%target”.',
      array('%source' => $form_state['values']['source'], '%target' => $form_state['values']['target'])
    ), 'error');
  }
  $form_state['redirect'] = 'mailadmin';
}

/**
 * Confirmation form to delete forwards.
 */
function mailadmin_alias_delete_form($form, &$form_state, $alias) {
  $form_state['alias'] = $alias;
  $question = t('Are you sure you want to delete the domain alias?');
  $description = t('This action cannot be undone.');
  $path = 'mailadmin/alias/' . $alias->alias_id;
  $yes = t('Delete');
  $no = t('Cancel');
  return confirm_form($form, $question, $path, "<p>$question</p><p>$description</p>", $yes, $no, 'confirm');
}

/**
 * Submit handler for mailadmin_alias_delete_form().
 */
function mailadmin_alias_delete_form_submit(&$form, &$form_state) {
  $alias = $form_state['alias'];
  mailadmin_alias_delete($alias->alias_id);

  $form_state['redirect'] = 'mailadmin';

  watchdog('mailadmin', 'E-mail domain alias from “%source” to “%target” was deleted.',
    array('%source' => $alias->source_domain_name, '%target' => $alias->target_domain_name)
  );
}
