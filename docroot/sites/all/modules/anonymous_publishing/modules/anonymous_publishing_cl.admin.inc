<?php

/**
 * @file
 * Menu callbacks for the CL tabs on the module admin page.
 *
 * Also the functions for sending verification email.
 */

/**
 * Menu callback: Provide module settings page.
 *
 * @return array
 *   Form.
 */
function anonymous_publishing_cl_admin_settings() {
  $anony = drupal_anonymous_user();
  $ctypes = variable_get('anonymous_publishing_cl_types', array());
  foreach ($ctypes as $ctype => $act) {
    if ('comment' == $ctype && $act) {
      $ua = user_access('post comments', $anony);
      if ($act && !$ua) {
        drupal_set_message(t('The module is set to to manage comments, but anonymous users are not allowed to post comments.'), 'warning');
      }
    }
    else {
      $ua = user_access('create ' . $ctype . ' content', $anony);
      if ($act && !$ua) {
        drupal_set_message(t('The module is set to to manage !ctypes, but anonymous users are not allowed to create !ctypes.', array('!ctype' => $ctype)), 'warning');
      }
    }
  }

  $admin = user_load(1);
  $ctypes = node_type_get_names();
  if (module_exists('comment')) {
    $ctypes['comment'] = 'Comment';
    $ctext = '(+ Comment)';
  }
  else {
    $ctext = '';
  }

  $form['anonymous_publishing_cl_types'] = array(
    '#type' => 'checkboxes',
    '#multiple' => TRUE,
    '#title' => t('Content types !comments where anonymous publishing is managed by this submodule:', array('!comments' => $ctext)),
    '#default_value' => variable_get('anonymous_publishing_cl_types', array()),
    '#options' => $ctypes,
    '#description' => t('Note: You also need to use node permissions to enable anonymous publishing for the anonymous user role if you want this role to be able to create content.'),
  );

  $form['anonymous_publishing_cl_options'] = array(
    '#type' => 'checkboxes',
    '#multiple' => TRUE,
    '#title' => t('Options:'),
    '#default_value' => variable_get('anonymous_publishing_cl_options', array(
       'sactivate' => 'sactivate',
       'sactstick' => 'sactstick',
       'modmail' => 0,
       'blockip' => 0,
       'atalias' => 0,
       'aregist' => 0,
    )),
    '#options' => array(
      'sactivate' => t('Allow self-activation.'),
      'sactstick' => t('Make self-activation sticky.'),
      'sactcomm' => t('&#8220;Skip comment approval&#8221; (set on <a title="Set permissions for anonymous." href="@url">Administration » People » Permissions</a>).',
        array('@url' => url('admin/people/permissions', array('fragment' => 'module-node')))),
      'modmail' => t('Send e-mail to administrator when anonymous content is created.'),
      'blockip' => t('Use IP-address for blocking.'),
      'atalias' => t('Associate a generated alias with contents published anonymously.'),
      'aregist' => t('Allow registered e-mails to be used for anonymous posts.'),
    ),
    '#description' => t('Check the options you want to enable.'),
  );
  
  $form['anonymous_publishing_cl_options']['sactcomm'] = array(
    '#disabled' => TRUE,
    '#default_value' => user_access('skip comment approval', $anony),
  );

  // Period is set on privacy tab, -1 = Delete ASAP.
  $period = variable_get('anonymous_publishing_cl_period', -1);
  if ($period != -1) {
    $form['anonymous_publishing_cl_options']['atalias'] = array(
      '#disabled' => TRUE,
    );
  }
  $form['anonymous_publishing_moderator'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator&#39;s e-mail address:'),
    '#size' => 60,
    '#maxlength' => EMAIL_MAX_LENGTH,
    '#default_value' => variable_get('anonymous_publishing_moderator', $admin->mail),
    '#description' => t('Address to use when the') . " '" . t('Send e-mail to administrator') . "&hellip;' " . t('option is checked.'),
  );

  $form['anonymous_publishing_emailweight'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification e-mail address field weight:'),
    '#size' => 3,
    '#maxlength' => 3,
    '#default_value' => variable_get('anonymous_publishing_emailweight', 0),
    '#element_validate' => array('element_validate_integer'),
    '#description' => t('Weight of verification e-mail address field on create content form.'),
  );

  $form['anonymous_publishing_autodelhours'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of hours to retain unverified anonymous posts before auto-deletions removes them:'),
    '#size' => 3,
    '#maxlength' => 3,
    '#default_value' => variable_get('anonymous_publishing_autodelhours', 999),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Non-verified content will be automatically deleted after this time. Type &#8220;999&#8221; for no limit.'),
  );

  $form['anonymous_publishing_flood'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of anonymous posts allowed from a single user e-mail/ip allowed within an hour:'),
    '#size' => 3,
    '#maxlength' => 2,
    '#default_value' => variable_get('anonymous_publishing_flood', 5),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Type &#8220;99&#8221; for no limit.'),
  );

  return system_settings_form($form);
}

/**
 * Menu callback: Form to change content of email message.
 *
 * @return array
 *   Form.
 */
function anonymous_publishing_cl_admin_email() {
  $form = array();

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'anonymous_publishing') . '/anonymous_publishing_cl_mail.css',
  );

  $form['anonymous_publishing_usersec'] = array(
    '#markup' => t('<p>You may edit the following fields to customize the e-mail message sent to non-authenticated users when they create content. One of the first two fields are used for the subject field, the rest may go in the body.</p>'),
  );

  // We do not santize these textfields and textareas, because the
  // form API does this. If santizes, the results is:
  // "quoted" => &quot;quoted&quot; =>  &amp;quot;quoted&amp;quot;.
  $form['anonymous_publishing_emailsubjectact'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject (activate content):'),
    '#size' => 72,
    '#maxlength' => 180,
    '#default_value' => variable_get('anonymous_publishing_emailsubject_act', _anonymous_publishing_email_text('subjectact')),
  );

  $form['anonymous_publishing_emailsubjectver'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject (verify email):'),
    '#size' => 72,
    '#maxlength' => 180,
    '#default_value' => variable_get('anonymous_publishing_emailsubject_ver', _anonymous_publishing_email_text('subjectver')),
  );

  $form['anonymous_publishing_emailaction'] = array(
    '#type' => 'textarea',
    '#title' => t('Introduction:'),
    '#default_value' => variable_get('anonymous_publishing_emailaction', _anonymous_publishing_email_text('action')),
    '#cols' => 60,
    '#rows' => 4,
    '#resizable' => FALSE,
  );

  $form['anonymous_publishing_emailactivate'] = array(
    '#type' => 'textarea',
    '#title' => t('Text to include if auto-deletion is enabled:'),
    '#default_value' => variable_get('anonymous_publishing_emailactivate', _anonymous_publishing_email_text('activate')),
    '#cols' => 60,
    '#rows' => 1,
    '#resizable' => FALSE,
  );

  $form['anonymous_publishing_emailverify'] = array(
    '#type' => 'textarea',
    '#title' => t('Text to include when administrator approval is  mandatory:'),
    '#default_value' => variable_get('anonymous_publishing_emailverify', _anonymous_publishing_email_text('verify')),
    '#cols' => 60,
    '#rows' => 2,
    '#resizable' => FALSE,
  );

  $form['anonymous_publishing_modsec'] = array(
    '#markup' => t('<p>You may edit the following fields to customize the e-mail message sent to the administrator when non-authenticated users create content. The first field is the subject, the second is the body.</p>'),
  );

  $form['anonymous_publishing_emailnsubject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject (admin):'),
    '#default_value' => variable_get('anonymous_publishing_emailnsubject', _anonymous_publishing_email_text('nsubject')),
    '#size' => 60,
    '#maxlength' => 180,
  );

  $form['anonymous_publishing_emailnbody'] = array(
    '#type' => 'textarea',
    '#title' => t('Body (admin):'),
    '#default_value' => variable_get('anonymous_publishing_emailnbody', _anonymous_publishing_email_text('nbody')),
    '#cols' => 60,
    '#rows' => 2,
    '#resizable' => FALSE,
  );

  $form['anonymous_publishing_vars'] = array(
    '#markup' => t('<p>You may use the following tokens in the texts above: <code>!action, !autodelhours, !email, !site, !title, !verification_uri.</code></p>'),
  );

  return system_settings_form($form);
}

/**
 * Submit for the anonymous_publishing_admin_moderation form.
 */
function anonymous_publishing_cl_admin_moderation_submit($form, &$form_state) {
  if (!array_key_exists('ap_row', $form_state['values'])) {
    drupal_set_message(t('No nodes.'), 'error');
    return;
  }

  foreach ($form_state['values']['ap_row'] as $settings) {
    if ($settings['cid']) {
      $comm = comment_load($settings['cid']);
      if ($comm) {
        $comm->status = $settings['status'];
        comment_save($comm);
      }
    }
    else {
      $node = node_load($settings['nid']);
      if ($node) {
        $node->status = $settings['status'];
        node_save($node);
      }
    }
  }
  drupal_set_message(t('Revised published status saved.'));
}


/**
 * Menu callback: Form to work with the moderation queue.
 *
 * @return array
 *   Form.
 */
function anonymous_publishing_cl_admin_moderation($form, &$form_state) {

  // Fetch all nodes that has been verified.
  $sql = "SELECT a.apid, a.nid, a.cid, a.verified, a.email, n.title, n.status FROM {anonymous_publishing} a JOIN {node} n ON a.nid = n.nid WHERE a.verified > 0 ORDER BY a.nid DESC";
  $result = db_query($sql);

  $form = array();
  $form['#tree'] = TRUE;
  $form['apm_info'] = array(
    '#markup' => t('<p>The following table shows all nodes that have been verified by e-mail. You may publish or unpublish by adding or removing a mark in the &#8220;Published&#8221; column. Then press &#8220;Execute&#8221; to execute the changes.</p>'),
  );

  $form['ap_row'] = array();
  while ($row = $result->fetchAssoc()) {
    if ($row['cid']) {
      $comment = db_query("SELECT subject, status FROM {comment} WHERE cid = :cid", array(':cid' => $row['cid']))->fetchAssoc();
      if ($comment) {
        $title = $comment ? $comment['subject'] : t('-deleted-');
        $status = $comment['status'];
      }
      else {
        $title = t('-deleted-');
        $status = 0;
      }
    }
    else {
      $title = $row['title'];
      $status = $row['status'];
    }
    $form['ap_row'][$row['apid']] = array();

    $form['ap_row'][$row['apid']]['nidmu'] = array(
      '#markup' => $row['nid'],
    );
    $form['ap_row'][$row['apid']]['cidmu'] = array(
      '#markup' => $row['cid'],
    );
    $form['ap_row'][$row['apid']]['title'] = array(
      '#markup' => check_plain($title),
    );
    $form['ap_row'][$row['apid']]['email'] = array(
      '#markup' => $row['email'] ? check_plain($row['email']) : t('(redacted)'),
    );
    $form['ap_row'][$row['apid']]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $status,
    );
    $form['ap_row'][$row['apid']]['nid'] = array(
      '#type' => 'hidden',
      '#value' => $row['nid'],
    );
    $form['ap_row'][$row['apid']]['cid'] = array(
      '#type' => 'hidden',
      '#value' => $row['cid'],
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Execute'),
  );

  return $form;
}

/**
 * Theme function to theme the moderation form in a table format.
 */
function theme_anonymous_publishing_cl_admin_moderation($variables) {
  $form = $variables['form'];
  $output = drupal_render($form['apm_info']);
  $header = array(
    t('nid'),
    t('cid'),
    t('title'),
    t('e-mail'),
    t('published'),
  );

  $rows = array();
  foreach (element_children($form['ap_row']) as $nid) {
    $row = array();
    foreach (element_children($form['ap_row'][$nid]) as $entry_key) {
      unset($form['ap_row'][$nid][$entry_key]['#title']);
      if (!isset($form['ap_row'][$nid][$entry_key]['#type']) || 'hidden' != $form['ap_row'][$nid][$entry_key]['#type']) {
        $row[] = drupal_render($form['ap_row'][$nid][$entry_key]);
      }
    }
    $rows[] = $row;
  }

  if (!$rows) {
    $rows[] = array(array(
      'data' => t('There is no verified content to moderate.'),
      'colspan' => 5,
      ));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit for the notify_admin_blocked form.
 */
function anonymous_publishing_cl_admin_blocked_submit($form, &$form_state) {
  if (!array_key_exists('users', $form_state['values'])) {
    drupal_set_message(t('No users.'), 'error');
    return;
  }

  foreach ($form_state['values']['users'] as $auid => $settings) {
    db_update('anonymous_publishing_emails')
    ->fields(array(
      'blocked' => $settings['blocked'],
    ))
    ->condition('auid', $auid)
    ->execute();
  }
  drupal_set_message(t('Blocking status saved.'));
}

/**
 * Menu callback: Form to manage blocking of anonymous users.
 *
 * @return array
 *   $form
 */
function anonymous_publishing_cl_admin_blocked($form, &$form_state) {
  $form = array();

  $form['#tree'] = TRUE;
  $form['apu_info'] = array(
    '#markup' => t('<p>The table below shows the e-mail address used to verify, IP-address, generated alias, blocked status for all <em>verified</em> e-mail addresses. Toggle the checkbox in the &#8220;Blocked&#8221; column to block or unblock. Then press &#8220;Execute&#8221; to execute the changes.</p><p>Note than an e-mail address is not listed here until it has been verified.  For yet unverified addresses, see the <em>unverified</em> tab.</p>'),
  );

  // Fetch all emails.
  $rows = db_query("SELECT auid, email, alias, ipaddress, blocked FROM {anonymous_publishing_emails} ORDER BY auid DESC")->fetchAll(PDO::FETCH_ASSOC);
  $form['users'] = array();
  foreach ($rows as $row) {
    $form['users'][$row['auid']] = array();
    $form['users'][$row['auid']]['auid'] = array(
      '#markup' => $row['auid'],
    );
    $form['users'][$row['auid']]['email'] = array(
      '#markup' => $row['email'],
    );
    $form['users'][$row['auid']]['ip-address'] = array(
      '#markup' => $row['ipaddress'],
    );
    $form['users'][$row['auid']]['alias'] = array(
      '#markup' => check_plain($row['alias']),
    );
    $form['users'][$row['auid']]['blocked'] = array(
      '#type' => 'checkbox',
      '#default_value' => $row['blocked'],
    );
  }

  if ($form['users']) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Execute'),
    );
  }

  return $form;
}


/**
 * Theme function to theme the blocked user form in a table format.
 */
function theme_anonymous_publishing_cl_admin_blocked($variables) {
  $form = $variables['form'];
  $output = drupal_render($form['apu_info']);
  $header = array(
    t('auid'),
    t('verification e-mail'),
    t('IP-address'),
    t('alias'),
    t('Blocked'),
  );

  $rows = array();
  foreach (element_children($form['users']) as $auid) {
    $row = array();
    foreach (element_children($form['users'][$auid]) as $entry_key) {
      unset($form['users'][$auid][$entry_key]['#title']);

      $row[] = drupal_render($form['users'][$auid][$entry_key]);
    }
    $rows[] = $row;
  }

  if (!$rows) {
    $rows[] = array(array(
        'data' => t('There is no e-mail addresses for anonymous publishers on file.'),
        'colspan' => 3,
      ));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit for the notify_admin_spam form.
 */
function anonymous_publishing_cl_admin_spam_submit($form, &$form_state) {
  if (!array_key_exists('bots', $form_state['values'])) {
    drupal_set_message(t('No bots.'), 'error');
    return;
  }

  $moved = 0;
  foreach ($form_state['values']['bots'] as $settings) {
    if ($settings['block']) {
      $existp = db_query("SELECT ip FROM {blocked_ips} WHERE ip = :ip", array(':ip' => $settings['ip2']))->fetchAssoc();
      if (FALSE == $existp) {
        $res = db_insert('blocked_ips')
          ->fields(array(
          'ip' => $settings['ip2'],
        ))
        ->execute();
      }
      else {
        $res = TRUE;
      }
      if ($res) {
        $res = db_delete('anonymous_publishing_bots')
          ->condition('ip', $settings['ip2'])
          ->execute();
        $moved++;
      }
    }
  }
  if ($moved) {
    drupal_set_message(t('IP-adresses moved to <code>{blocked_ips}</code>.'));
  }
}


/**
 * Menu callback: Form to manage spambot report.
 *
 * @return array
 *   $form
 */
function anonymous_publishing_cl_admin_spam($form, &$form_state) {
  $form = array();

  $form['#tree'] = TRUE;
  $form['apu_info'] = array(
    '#markup' => t("<p>The following table shows the IP-addresses and the average hits per day generated by the ten most aggressive spambots hitting the site. To move the bot's IP-address to Drupal's <code>{blocked_ips}</code> table, check the box in the &#8220;ban IP&#8221; column. Then press &#8220;Execute&#8221; to execute the move.</p><p>As an alternative to the Drupal <code>{blocked_ips}</code> table you may instead deny access to unwanted IP-addresses using the appropriate command in the web server access file.</p>"),
  );

  // Fetch first 10 bot reports.
  $rows = db_query("SELECT id, ip, visits, first, last FROM {anonymous_publishing_bots} ORDER BY visits DESC LIMIT 10")->fetchAll(PDO::FETCH_ASSOC);
  $form['bots'] = array();
  foreach ($rows as $row) {
    $freq = $row['visits'] / ((REQUEST_TIME - $row['first']) / 86400);
    $freq = min(array($freq, $row['visits']));
    $form['bots'][$row['id']] = array();
    $form['bots'][$row['id']]['id'] = array(
      '#markup' => $row['id'],
    );
    $form['bots'][$row['id']]['ip'] = array(
      '#markup' => $row['ip'],
    );
    $form['bots'][$row['id']]['first'] = array(
      '#markup' => format_interval(REQUEST_TIME - $row['first'], 1) . ' ' . t('ago'),
    );
    $form['bots'][$row['id']]['last'] = array(
      '#markup' => format_interval(REQUEST_TIME - $row['last'], 1) . ' ' . t('ago'),
    );
    $form['bots'][$row['id']]['visits'] = array(
      '#markup' => $row['visits'],
    );
    $form['bots'][$row['id']]['freq'] = array(
      '#markup' => round($freq),
    );
    $form['bots'][$row['id']]['block'] = array(
      '#type' => 'checkbox',
      '#default_value' => FALSE,
    );
    $form['bots'][$row['id']]['ip2'] = array(
      '#type' => 'hidden',
      '#value' => $row['ip'],
    );
  }

  if ($form['bots']) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Execute'),
    );
  }
  return $form;
}

/**
 * Theme function to theme the spambot report in a table format.
 */
function theme_anonymous_publishing_cl_admin_spam($variables) {
  $form = $variables['form'];
  $output = drupal_render($form['apu_info']);
  $header = array(
    t('id'),
    t('ip-address'),
    t('first seen'),
    t('last seen'),
    t('total hits'),
    t('daily hits'),
    t('ban IP'),
  );

  $rows = array();
  foreach (element_children($form['bots']) as $id) {
    $row = array();
    foreach (element_children($form['bots'][$id]) as $entry_key) {
      unset($form['bots'][$id][$entry_key]['#title']);

      if (isset($form['bots'][$id][$entry_key]['#type']) && 'hidden' == $form['bots'][$id][$entry_key]['#type']) {
        // Hidden field.
      }
      else {
        $row[] = drupal_render($form['bots'][$id][$entry_key]);
      }
    }
    $rows[] = $row;
  }

  if (!$rows) {
    $rows[] = array(array(
        'data' => t('There are no IP-addresses for spambots on file.'),
        'colspan' => 3,
      ));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit for the notify_admin_unverified form.
 */
function anonymous_publishing_cl_admin_unverified_submit($form, &$form_state) {
  if (!array_key_exists('unverified', $form_state['values'])) {
    drupal_set_message(t('No unverified posts?'), 'error');
    return;
  }

  $deleted = $moved = 0;
  $ownip = ip_address();
  foreach ($form_state['values']['unverified'] as $id => $settings) {
    // Don't block self.
    if ($ownip == $settings['ip2']) {
      drupal_set_message(t("You've tried to ban your own IP (request is ignored)."));
      continue;
    }
    if ($settings['block']) {
      if (!empty($settings['ip2'])) {
        $existp = db_query("SELECT ip FROM {blocked_ips} WHERE ip = :ip", array(':ip' => $settings['ip2']))->fetchAssoc();
        if (FALSE == $existp) {
          $res = db_insert('blocked_ips')
            ->fields(array(
            'ip' => $settings['ip2'],
          ))
          ->execute();
        }
        else {
          $res = TRUE;
        }
        if ($res) {
          $res = db_delete('anonymous_publishing')
            ->condition('apid', $id)
            ->execute();
          $moved++;
        }
      }
      if ($settings['cid']) {
        comment_delete($settings['cid']);
        $deleted++;
      }
      elseif ($settings['nid']) {
        node_delete($settings['nid']);
        $deleted++;
      }
    }
  }
  if ($moved) {
    $msg1 = t('IP-address moved to <code>{blocked_ips}</code>.');
    $msg1 .= ' ';
  }
  else {
    $msg1 = '';
  }
  if ($deleted) {
    $msg2 = t('Spam deleted.');
  }
  else {
    $msg2 = t('No spam could be identified.');
  }
  drupal_set_message($msg1 . $msg2);
}


/**
 * Menu callback: Form to manage spambot report.
 *
 * @return array
 *   $form
 */
function anonymous_publishing_cl_admin_unverified($form, &$form_state) {
  $form = array();

  $form['#tree'] = TRUE;
  $form['apu_info'] = array(
    '#markup' => t("<p>The following table shows the IP-addresses, verification e-mail address used, date posted and title of still <em>unverified</em> anonymous posts. To delete the contents and ban the IP-address by moving to Drupal's <code>{blocked_ips}</code> table, check the box in the &#8220;delete+ban IP&#8221; column. Then press &#8220;Execute&#8221;.</p><p>As an alternative to the Drupal <code>{blocked_ips}</code> table you may instead deny access to unwanted IP-addresses using the appropriate command in the web server access file.</p>"),
  );

  // Fetch all unverified posts.
  $rows = db_query("SELECT apid, nid, cid, verified, email, ip FROM {anonymous_publishing} WHERE (verified = 0) ORDER BY apid ASC LIMIT 100")->fetchAll(PDO::FETCH_ASSOC);
  $form['unverified'] = array();
  foreach ($rows as $row) {
    if ($row['cid']) {
      $commentfields = db_query("SELECT created, subject FROM {comment} WHERE cid = :cid", array(':cid' => $row['cid']))->fetchAssoc();
      $datefield = $commentfields['created'];
      $titlefield = $commentfields['subject'];
      if (empty($titlefield)) {
        $titlefield = '- empty -';
      }
    }
    else {
      $nodefields = db_query("SELECT created, title FROM {node} WHERE (:nid = nid)", array(':nid' => $row['nid']))->fetchAssoc();
      $datefield = $nodefields['created'];
      $titlefield = $nodefields['title'];
    }
    $datefield = ($datefield) ? format_interval(REQUEST_TIME - $datefield, 1) . ' ' . t('ago') : '-NULL-';
    $form['unverified'][$row['apid']] = array();
    $form['unverified'][$row['apid']]['apid'] = array(
      '#markup' => $row['apid'],
    );
    $form['unverified'][$row['apid']]['ip'] = array(
      '#markup' => $row['ip'],
    );
    $form['unverified'][$row['apid']]['email'] = array(
      '#markup' => check_plain($row['email']),
    );
    $form['unverified'][$row['apid']]['date'] = array(
      '#markup' => $datefield,
    );
    $form['unverified'][$row['apid']]['node/comment'] = array(
      '#markup' => check_plain($titlefield),
    );
    $form['unverified'][$row['apid']]['block'] = array(
      '#type' => 'checkbox',
      '#default_value' => FALSE,
    );
    $form['unverified'][$row['apid']]['ip2'] = array(
      '#type' => 'hidden',
      '#value' => $row['ip'],
    );
    $form['unverified'][$row['apid']]['nid'] = array(
      '#type' => 'hidden',
      '#value' => $row['nid'],
    );
    $form['unverified'][$row['apid']]['cid'] = array(
      '#type' => 'hidden',
      '#value' => $row['cid'],
    );
  }

  if ($form['unverified']) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Execute'),
    );
  }
  return $form;
}

/**
 * Theme function to theme the spambot report in a table format.
 */
function theme_anonymous_publishing_cl_admin_unverified($variables) {
  $form = $variables['form'];
  $output = drupal_render($form['apu_info']);
  $header = array(
    t('apid'),
    t('ip-address'),
    t('verification e-mail'),
    t('when'),
    t('title'),
    t('delete+ban IP'),
  );

  $rows = array();
  foreach (element_children($form['unverified']) as $id) {
    $row = array();
    foreach (element_children($form['unverified'][$id]) as $entry_key) {
      unset($form['unverified'][$id][$entry_key]['#title']);

      if (isset($form['unverified'][$id][$entry_key]['#type']) && 'hidden' == $form['unverified'][$id][$entry_key]['#type']) {
        // Hidden field.
      }
      else {
        $row[] = drupal_render($form['unverified'][$id][$entry_key]);
      }
    }
    $rows[] = $row;
  }

  if (!$rows) {
    $rows[] = array(array(
        'data' => t('There is no unverified posts on file.'),
        'colspan' => 3,
      ));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit for the anonymous_publishing_admin_privacy form.
 */
function anonymous_publishing_cl_admin_privacy_submit($form, &$form_state) {
  if (!array_key_exists('op', $form_state['values'])) {
    drupal_set_message(t('No operation?'), 'error');
    return;
  }
  if ('Purge now' == $form_state['values']['op']) {

    // First delete completly all that are published.
    db_delete('anonymous_publishing')
      ->condition('verified', 1)
      ->execute();

    // For the rest, delete the IP (we need e-mail for whitelist).
    db_update('anonymous_publishing')
      ->fields(array(
        'ip' => '',
      ))
      ->execute();

    drupal_set_message(t('All information linking identifiers to published content have been purged.'));
  }
  elseif ('Save setting' == $form_state['values']['op']) {
    variable_set('anonymous_publishing_cl_period', $form_state['values']['anonymous_publishing_cl_period']);
    drupal_set_message(t('Rentention period updated.'));
  }
  else {
    drupal_set_message(t('Unknown operation.', 'error'));
    return;
  }
}


/**
 * Menu callback: Form to change privacy settings and flush queue.
 *
 * @return array
 *   Form.
 */
function anonymous_publishing_cl_admin_privacy($form, &$form_state) {

  // Count the number of used aliases on file.
  $sql = "SELECT a.nid, a.email, m.alias FROM  {anonymous_publishing} a JOIN {anonymous_publishing_emails} m ON a.email = m.email WHERE m.alias <> '' ORDER BY a.nid DESC";
  $aliases = db_query($sql)->rowCount();

  $period = array(
    0 => t('Delete ASAP'),
    3600 => format_interval(3600),
    21600 => format_interval(21600),
    43200 => format_interval(43200),
    86400 => format_interval(86400),
    259200 => format_interval(259200),
    604800 => format_interval(604800),
    2592000 => format_interval(2592000),
    -1 => t('Indefinitely'),
  );

  $form = array();
  $options = variable_get('anonymous_publishing_cl_options', array());
  if (isset($options['atalias']) && $options['atalias']) {
    $disablep = TRUE;
    $warn = '<br/><strong>' . t('Note:') . '</strong> ' . t('Purging is incompatible with having a generated alias (main settings).  To purge, you need to turn this setting off.');
  }
  elseif (!$aliases) {
    $warn = '';
    $disablep = FALSE;
  }
  else {
    $warn = '<br/>' . t('You have !count linking verification e-mail to content. !these will be deleted when these links are purged.',
      array(
        '!count' => format_plural($aliases, '1 record', '@count records'),
        '!these' => format_plural($aliases, 'This', 'These'),
      )
    );
    $disablep = FALSE;
  }

  $form['anonymous_publishing_privacy'] = array(
    '#markup' => '<p>' . t('For enhanced privacy, you can set a limited retention period for identifying information, or purge this information instantly.') . ' ' . $warn . '</p>',
  );

  $form['apperiod'] = array(
    '#type' => 'fieldset',
    '#title' => t('Retention period'),
    '#collapsible' => FALSE,
  );
  $form['apperiod']['anonymous_publishing_cl_period'] = array(
    '#type' => 'select',
    '#title' => t('Maximum period to retain record that links verification e-mails, ip-addresses and generated aliases to specific content:'),
    '#default_value' => variable_get('anonymous_publishing_cl_period', array(-1)),
    '#options' => $period,
    '#description' => t('Select &#8220;Indefinitely&#8221; to make the records linking verification e-mail to content persistent.  This is the <em>only</em> setting compatible with a persistent alias as byline.'),
  );
  $form['apperiod']['submit'] = array(
    '#type' => 'submit',
    '#disabled' => $disablep,
    '#value' => t('Save setting'),
  );

  $form['appurge'] = array(
    '#type' => 'fieldset',
    '#title' => t('Purge'),
    '#collapsible' => FALSE,
  );
  $form['appurge']['info'] = array(
    '#markup' => t('<p>Press button below to immediately purge all information linking e-mails, ip-addresses and generated aliases to anonymously published content.  This operation can not be reversed.</p>'),
  );
  $form['appurge']['submit'] = array(
    '#type' => 'submit',
    '#disabled' => $disablep,
    '#value' => t('Purge now'),
  );

  return $form;
}

/**
 * Implements hook_theme().
 *
 * Register the form data theme into a table for the tabs listed.
 */
function anonymous_publishing_cl_theme() {
  return array(
    'anonymous_publishing_cl_admin_moderation' => array(
      'render element' => 'form',
    ),
    'anonymous_publishing_cl_admin_blocked' => array(
      'render element' => 'form',
    ),
    'anonymous_publishing_cl_admin_spam' => array(
      'render element' => 'form',
    ),
    'anonymous_publishing_cl_admin_unverified' => array(
      'render element' => 'form',
    ),
  );
}

/**
 * Implements hook_menu().
 */
function anonymous_publishing_cl_menu() {

  $items = array();

  $items['admin/config/people/anonymous_publishing_cl'] = array(
    'title' => 'Anonymous publishing CL',
    'description' => "Administrator settings for the Anonymous publishing CL (Craig's List model) submodule.",
    'page callback' => 'drupal_get_form',
    'page arguments' => array('anonymous_publishing_cl_admin_settings'),
    'access callback' => 'user_access',
    'access arguments' => array('administer anonymous_publishing'),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 10,
  );
  $items['admin/config/people/anonymous_publishing_cl/settings'] = array(
    'title' => 'Main settings',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );

  $items['admin/config/people/anonymous_publishing_cl/email'] = array(
    'title' => 'Message templates',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('anonymous_publishing_cl_admin_email'),
    'access callback' => 'user_access',
    'access arguments' => array('administer anonymous_publishing'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -9,
  );

  $items['admin/config/people/anonymous_publishing_cl/moderation'] = array(
    'title' => 'Moderation',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('anonymous_publishing_cl_admin_moderation'),
    'access callback' => 'user_access',
    'access arguments' => array('administer anonymous_publishing'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -8,
  );

  $items['admin/config/people/anonymous_publishing_cl/users'] = array(
    'title' => 'Verified',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('anonymous_publishing_cl_admin_blocked'),
    'access callback' => 'user_access',
    'access arguments' => array('administer anonymous_publishing'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -7,
  );

  $items['admin/config/people/anonymous_publishing_cl/unverified'] = array(
    'title' => 'Unverified',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('anonymous_publishing_cl_admin_unverified'),
    'access callback' => 'user_access',
    'access arguments' => array('administer anonymous_publishing'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -6,
  );

  $items['admin/config/people/anonymous_publishing_cl/spam'] = array(
    'title' => 'Spambots',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('anonymous_publishing_cl_admin_spam'),
    'access callback' => 'user_access',
    'access arguments' => array('administer anonymous_publishing'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -5,
  );

  $items['admin/config/people/anonymous_publishing_cl/privacy'] = array(
    'title' => 'Privacy',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('anonymous_publishing_cl_admin_privacy'),
    'access callback' => 'user_access',
    'access arguments' => array('administer anonymous_publishing'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -4,
  );

  $items['node/%node/verify'] = array(
    'title' => 'Anonymous publishing',
    'description' => 'Process url.',
    'page callback' => 'anonymous_publishing_cl_verify',
    'page arguments' => array(1),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'weight' => 10,
  );

  $items['comment/%comment/verify'] = array(
    'title' => 'Anonymous publishing',
    'description' => 'Process url.',
    'page callback' => 'anonymous_publishing_cl_verify',
    'page arguments' => array(1),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'weight' => 10,
  );

  return $items;
}

/**
 * Anonymous publishing mail handling.
 */

/**
 * Helper function; return email subject and body.
 *
 * If admin settings are set, return the overrided settings,
 * else return defaults.
 *
 * @param string $id
 *   The message ID.
 * @param array $variables
 *   An array of substitutions.
 *
 * @return text
 *   The text with the substitution applied.
 */
function _anonymous_publishing_email_text($id, array $variables = array()) {
  // Check if an admin setting overrides the default string.
  if ($admin_setting = variable_get('anonymous_publishing_email' . $id, FALSE)) {
    return strtr($admin_setting, $variables);
  }
  // No override, return with default strings.
  else {
    switch ($id) {
      case 'subjectact':
        return t('Please activate your content on !site', $variables);

      case 'subjectver':
        return t('Please verify the e-mail address for your content submission to !site', $variables);

      case 'action':
        return t('Somebody has created content with title "!title" on !site.  You receive this e-mail because your e-mail address (!email) was given as the verification e-mail address. To !action, click or copy this link into the browser: !verification_uri. If this was not you, please ignore this message.', $variables);

      case 'activate':
        return t('If you do not !action within !autodelhours hours, the content will be automatically deleted.', $variables);

      case 'verify':
        return t("After you've verified your e-mail address, your content will be placed in the moderation queue. It will be published when it has been approved by an administrator.");

      case 'nsubject':
        return t('Notification about anonymously published content on !site', $variables);

      case 'nbody':
        return t('Anonymous content with title "!title" has been published on !site.  The verification e-mail address was "!email".', $variables);
    }
  }
}

/**
 * Send verification email.
 *
 * @param object $node
 *   The node object.
 * @param string $akey
 *   Activation key.
 */
function _anonymous_publishing_send_email($node, $akey) {
  $options = variable_get('anonymous_publishing_cl_options', array());
  if (isset($node->title)) {
    $title = $node->title;
    $vfurl = url('node/' . $node->nid . '/verify', array('query' => array('akey' => $akey), 'absolute' => TRUE));
    $modp = !$options['sactivate'];
  }
  else {
    $title = $node->subject;
    if (empty($title)) {
      $title = '';
    }
    $vfurl = url('comment/' . $node->cid . '/verify', array('query' => array('akey' => $akey), 'absolute' => TRUE));
    $modp = !$options['sactivate'];
  }
  $autodelhours = variable_get('anonymous_publishing_autodelhours', '48');

  $from = variable_get('site_mail', ini_get('sendmail_from'));
  $variables = array(
    '!action' => $modp ? t('verify') : t('activate'),
    '!autodelhours' => $autodelhours,
    '!email' => $node->anonymous_publishing['email'],
    '!site' => variable_get('site_name', 'Drupal'),
    '!title' => check_plain($title),
    '!verification_uri' => $vfurl,
  );

  $to = $node->anonymous_publishing['email'];
  $subject = _anonymous_publishing_email_text($modp ? 'subjectver' : 'subjectact', $variables);
  $b1 = _anonymous_publishing_email_text('action', $variables);
  $b2 = 999 > $autodelhours ? _anonymous_publishing_email_text('activate', $variables) : '';
  $b3 = $modp ? _anonymous_publishing_email_text('verify', $variables) : '';
  $body = array($b1, $b2, $b3);

  // 'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
  $message = array(
    'id' => 'anonymous_publishing_verify',
    'to' => $to,
    'subject' => $subject,
    'body' => $body,
    'headers' => array(
      'From' => $from,
      'Sender' => $from,
      'Return-Path' => $from,
      'MIME-Version' => '1.0',
      'Content-Type' => 'text/plain; charset=UTF-8',
      'Content-Transfer-Encoding' => '8bit',
      'X-Mailer' => 'Drupal',
    ),
  );
  $system = drupal_mail_system('anonymous_publishing', 'verify');
  $message = $system->format($message);

  $result = $system->mail($message);
  if ($result) {
    drupal_set_message(t('A link and further instructions have been sent to your e-mail address.'));
    watchdog('anonymous_publishing', 'Verification mail sent to @to, from @from.', array('@to' => $to, '@from' => $from));
  }
  else {
    watchdog('anonymous_publishing', 'Error mailing activation/verification link.');
    drupal_set_message(t('Unable to send mail. Please contact the site admin.'), 'error');
  }

  if ($options['modmail']) {
    $admin = user_load(1);
    $subject = _anonymous_publishing_email_text('nsubject', $variables);
    $body = _anonymous_publishing_email_text('nbody', $variables);
    $body = wordwrap($body);
    $body = str_replace("\n", "\r\n", $body);

    $message = array(
      'id' => 'anonymous_publishing_notify',
      'to' => variable_get('anonymous_publishing_moderator', $admin->mail),
      'subject' => $subject,
      'body' => $body,
      'headers' => array(
        'From' => $from,
        'Sender' => $from,
        'Return-Path' => $from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
        'Content-Transfer-Encoding' => '8bit',
        'X-Mailer' => 'Drupal',
      ),
    );
    $system = drupal_mail_system('anonymous_publishing', 'notify');
    if (!$system->mail($message)) {
      watchdog('anonymous_publishing', 'Error notifying admin.');
    }
  }
}
