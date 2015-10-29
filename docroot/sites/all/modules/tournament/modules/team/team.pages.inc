<?php

/**
 * @file
 * Page callback file for the team module.
 */

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing teams.
 */
function team_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $result = db_select('teams')
      ->fields('teams', array('name'))
      ->condition('name', db_like($string) . '%', 'LIKE')
      ->condition('status', 1)
      ->range(0, 10)
      ->execute();

    foreach ($result as $team) {
      $matches[$team->name] = check_plain($team->name);
    }
  }

  drupal_json_output($matches);
}

/**
 * Menu callback; view a single team.
 */
function team_page_view($team) {
  // If there is a menu link to this team, the link becomes the last part
  // of the active trail, and the link name becomes the page title.
  // Thus, we must explicitly set the page title to be the team title.
  drupal_set_title($team->name);

  // For markup consistency with other pages, use team_view_multiple() rather than team_view().
  $teams = team_view_multiple(array($team->tid => $team), 'full');

  return $teams;
}

/**
 * Form builder; the team add form.
 *
 * @ingroup forms
 */
function team_create_form($form, &$form_state) {
  // Add default values for new teams
  team_object_prepare($team);
  $form_state['team'] = $team;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Team name'),
    '#required' => TRUE,
    '#maxlength' => variable_get('team_name_max', 64),
    '#description' => t('WARNING: You are unable to change your name after it is decided.'),
  );
  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 5,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create new team'),
  );
  $form['notice'] = array(
    '#markup' => t('After creating your team, you will be able to edit all other profile settings.'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Validation function for the team add form.
 */
function team_create_form_validate($form, &$form_state) {
  // Name availability validation
  if (isset($form_state['values']['name'])) {
    $query = db_select('teams')
      ->condition('name', trim($form_state['values']['name']))
      ->condition('status', 1)
      ->countQuery()
      ->execute()
      ->fetchField();

      if (!empty($query)) {
        form_set_error('name', t('The team name entered is already in use.'));
      }
  }
}

/**
 * Submit handler for the team add form.
 *
 * @see team_form()
 */
function team_create_form_submit($form, &$form_state) {
  $team = $form_state['team'];

  // Ensure trimmer team names
  if (isset($form_state['values']['name'])) {
    $form_state['values']['name'] = trim($form_state['values']['name']);
  }

  entity_form_submit_build_entity('team', $team, $form, $form_state);

  // The team object will gain the tid after calling team_save().
  team_save($team);

  drupal_set_message(t('Team created successfully.'));
  drupal_set_message(t('Team created successfully.'));
  $form_state['redirect'] = 'team/' . $team->tid;
}

/**
 * Form builder; the team edit form.
 *
 * @ingroup forms
 */
function team_form($form, &$form_state, $team = NULL) {
  drupal_set_title(t('Edit !title', array('!title' => $team->name)));

  // During initial form build, add the team entity to the form state for use
  // during form building and processing. During a rebuild, use what is in the
  // form state.
  if (!isset($form_state['team'])) {
    $form_state['team'] = $team;
  }
  else {
    $team = $form_state['team'];
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Team name'),
    '#default_value' => $team->name,
    '#disabled' => TRUE,
    '#description' => t('You are unable to change the name of this team.'),
  );

  if (variable_get('team_alias', 1)) {
    if ($alias = drupal_lookup_path('alias', 'team/' . $team->tid)) {
      $form['alias'] = array(
        '#type' => 'item',
        '#title' => t('Alias'),
        '#markup' => t('Your profile is now accessible at <a href="@url">@url</a>.', array('@url' => url($alias, array('absolute' => TRUE)))),
      );
    }
    else {
      $form['alias'] = array(
        '#type' => 'textfield',
        '#title' => t('Alias'),
        '#field_prefix' => 'team/',
        '#description' => t('An alias allows you to promote your presence on @site-name with a short URL. For example, an alias of "flowers" would appear as the URL <em>!domain/flowers</em>.<br />WARNING: You are unable to change your alias after it is decided.', array('!domain' => $_SERVER['HTTP_HOST'] . '/team', '@site-name' => variable_get('site_name', 'Drupal'))),
        '#element_validate' => array('team_alias_validate'),
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (team_access('cancel', $team)) {
    $form['actions']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel account'),
      '#submit' => array('team_form_cancel_submit'),
    );
  }
  if (team_access('delete', $team)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('team_form_delete_submit'),
    );
  }

  // Attach field widgets
  field_attach_form('team', $team, $form, $form_state);

  return $form;
}

/**
 * AJAX Callback for team_form().
 *
 * @return renderable array (the team joining fieldset element)
 */
function team_form_join_callback($form, $form_state) {
  return $form['joining'];
}

/**
 * Validation function for the team edit form.
 */
function team_form_validate($form, &$form_state) {
  entity_form_field_validate('team', $form, $form_state);
}

/**
 * Validate a given user alias.
 */
function team_alias_validate($element, &$form_state, $form) {
  if (!empty($element['#value'])) {
    $alias = trim($element['#value']);
    $path = 'team/' . $alias;

    // Check for bogus characters
    if (preg_match("/[^a-zA-Z0-9_\-]+/", $alias) > 0) {
      form_set_error('alias', t('You can only use letters, numbers, hyphens and underscores for aliases.'));
    }
    elseif (preg_match("/[a-zA-Z]+/", $alias) == 0) {
      form_set_error('alias', t('You must use one or more alphabetical characters for aliases.'));
    }
    elseif (drupal_get_normal_path($path) != $path) {
      form_set_error('alias', t('Alias not available. The alias entered is already in use.'));
    }
  }
}

/**
 * Submit handler for the team edit form.
 *
 * @see team_form()
 */
function team_form_submit($form, &$form_state) {
  $team = $form_state['team'];

  entity_form_submit_build_entity('team', $team, $form, $form_state);

  team_save($team);

  if (!empty($form_state['values']['alias'])) {
    // Add new drupal path for the team alias
    $path['source'] = 'team/' . $team->tid;
    $path['alias'] = 'team/' . trim($form_state['values']['alias']);
    path_save($path);
  }

  drupal_set_message(t('Team updated successfully.'));
  $form_state['redirect'] = 'team/' . $team->tid;
}

/**
 * Button submit function: handle the 'Cancel account' button on the team form.
 */
function team_form_cancel_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  $team = $form_state['team'];
  $form_state['redirect'] = array('team/' . $team->tid . '/cancel', array('query' => $destination));
}

/**
 * Button submit function: handle the 'Delete' button on the team form.
 */
function team_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  $team = $form_state['team'];
  $form_state['redirect'] = array('team/' . $team->tid . '/delete', array('query' => $destination));
}

/**
 * Form builder; the team settings form.
 *
 * @ingroup forms
 */
function team_settings_form($form, &$form_state, $team) {
  $form_state['team'] = $team;

  if (variable_get('team_joining', 1)) {
    // If the Team joining value has been changed on the form, use that instead
    $team_join = isset($form_state['values']['joining']) ? $form_state['values']['joining'] : $team->joining;

    $form['joining'] = array(
      '#title' => t('Joining'),
      '#type' => 'fieldset',
      '#prefix' => '<div id="team-joining-wrapper">',
      '#suffix' => '</div>',
      '#description' => $team->joining == TEAM_JOIN_PASS ? t('A password has already been set. To change the current password, enter the new password in both fields below.') : '',
    );
    $form['joining']['joining'] = array(
      '#type' => 'select',
      '#title' => t('Privacy'),
      '#options' => team_join_options(),
      '#default_value' => $team_join,
      '#ajax' => array(
        'callback' => 'team_form_join_callback',
        'wrapper' => 'team-joining-wrapper',
      ),
      '#description' => team_join_description($team_join),
    );
    // This field element handles both new passwords and password changes.
    // We don't need to be as strict as to have users enter the current password.
    // Field element must be added within an if statement as oppose to an #access
    // check otherwise the password_confirm JS breaks.
    if ($team_join == TEAM_JOIN_PASS) {
      $form['joining']['join_pass'] = array(
        '#type' => 'password_confirm',
        '#size' => 25,
        '#attributes' => array('autocomplete' => 'off'),
        '#required' => !isset($team->join_pass),
      );
    }
    $form['joining']['join_info'] = array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#description' => t('This message will be displayed to users on the Join Team form. It can be used to give further details about your team such as the rules and requirements for joining.'),
      '#default_value' => isset($team->join_info) ? filter_xss($team->join_info) : '',
      '#access' => $team_join != TEAM_JOIN_OFF,
    );
  }

  if (variable_get('team_challenging', 1)) {
    $form['challenging'] = array(
      '#title' => t('Challenging'),
      '#type' => 'fieldset',
      '#description' => t('Other teams are able to challenge your team by clicking the Challenge button on your team profile page.'),
    );
    $form['challenging']['challenge_info'] = array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#description' => t('This message will be displayed to users on the Challenge Team form. It should be used to give details such as team availability.'),
      '#default_value' => isset($team->challenge_info) ? $team->challenge_info : '',
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Submit handler for the team settings form.
 *
 * @see team_form()
 */
function team_settings_form_submit($form, &$form_state) {
  $team = $form_state['team'];

  if ($form_state['values']['joining'] != TEAM_JOIN_PASS) {
    $form_state['values']['join_pass'] = NULL;
  }

  entity_form_submit_build_entity('team', $team, $form, $form_state);

  team_save($team);

  drupal_set_message(t('Team settings updated successfully.'));
}

/**
 * Menu callback - ask for confirmation of team deletion
 */
function team_delete_confirm($form, &$form_state, $team) {
  $form_state['team'] = $team;;
  return confirm_form($form,
    t('Are you sure you want to delete %name?', array('%name' => $team->name)),
    'team/' . $team->tid . '/edit',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute team deletion
 */
function team_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $team = $form_state['team'];
    team_delete($team->tid);

    drupal_set_message(t('Team %name has been deleted.', array('%name' => $team->name)));
  }

  $form_state['redirect'] = '<front>';
}

/**
 * Menu callback - ask for confirmation of team cancellation
 */
function team_cancel_confirm($form, &$form_state, $team) {
  // Always provide entity id in the same form key as in the entity edit form.
  $form_state['team'] = $team;
  return confirm_form($form,
    t('Are you sure you want to cancel the account for %name?', array('%name' => $team->name)),
    'team/' . $team->tid . '/edit',
    t('This action cannot be undone.'),
    t('Confirm'),
    t('Cancel')
  );
}

/**
 * Execute team deletion
 */
function team_cancel_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $team = $form_state['team'];
    team_cancel($team);

    drupal_set_message(t('The team account for %name has been cancelled.', array('%name' => $team->name)));
  }

  $form_state['redirect'] = '<front>';
}

/**
 * Form builder; the team members form.
 *
 * @ingroup forms
 */
function team_members_form($form, &$form_state, $team) {
  global $user;

  if (empty($team->members)) {
    // Do not continue if there are no members
    $form['message'] = array(
      '#markup' => t('No team members found.'),
    );

    return $form;
  }

  drupal_set_title(t('Manage members'));

  // Store the team object for use in form handlers
  $form_state['team'] = $team;

  $roles = team_get_member_roles();

  // Admins should not be able to set member roles to this value
  unset($roles[TEAM_MEMBER_ROLE_INVITED]);

  $form['members'] = array(
    '#tree' => TRUE,
    '#theme' => 'team_members_form_members'
  );
  foreach ($team->members as $uid => $role) {
    $account = user_load($uid);

    $account_link = theme('username', array('account' => $account));

    if (variable_get('team_user_picture', 0)) {
      $name = theme('user_picture', array('account' => $account)) . '<div class="user-name">' . $account_link . '</div>';
    }

    $form['members'][$uid]['name'] = array(
      '#markup' => isset($name) ? $name : $account_link,
    );

    if ($role == TEAM_MEMBER_ROLE_INVITED) {
      $form['members'][$uid]['role'] = array(
        '#markup' => t('This member has not confirmed their invitation yet.'),
      );
    }
    else {
      $form['members'][$uid]['role'] = array(
        '#type' => 'select',
        '#options' => $roles,
        '#default_value' => $role,
        '#description' => t('Confirmation required'),
        '#disabled' => $uid == $user->uid,
      );

      if ($role != TEAM_MEMBER_ROLE_PENDING) {
        // Admins should not be able to set member roles to this value
        unset($form['members'][$uid]['role']['#options'][TEAM_MEMBER_ROLE_PENDING]);
        unset($form['members'][$uid]['role']['#description']);
      }
    }

    $form['members'][$uid]['remove'] = array(
      '#markup' => l(t('Remove'), 'team/' . $team->tid . '/edit/members/' . $uid . '/remove'),
      '#access' => $uid != $user->uid,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

/**
 * Submit handler for the team members form.
 *
 * @see team_members_form()
 */
function team_members_form_submit($form, &$form_state) {
  $team = $form_state['team'];

  if (isset($form_state['values']['members'])) {
    foreach ($form_state['values']['members'] as $uid => $member) {
      db_update('teams_members')
        ->fields(array(
          'role' => $member['role'],
        ))
        ->condition('tid', $team->tid)
        ->condition('uid', $uid)
        ->execute();

      if ($member['role'] > TEAM_MEMBER_ROLE_PENDING && $team->members[$uid] == TEAM_MEMBER_ROLE_PENDING) {
        // An admin accepted a team member
        $account = user_load($uid);
        module_invoke_all('team_member_insert', $team, $account);
      }
    }
  }

  drupal_set_message(t('Team members successfully updated.'));

  // Redirect to the manage members page
  $form_state['redirect'] = 'team/' . $team->tid . '/edit/members';
}

/**
 * Returns HTML for the team members form members form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An array representing the form element.
 *
 */
function theme_team_members_form_members($variables) {
  $members = $variables['element'];

  $rows = array();
  foreach (element_children($members) as $uid) {
    if (isset($members[$uid]['role'])) {
      $action = $members[$uid]['role'];
    }
    if (isset($members[$uid]['status'])) {
      $action = $members[$uid]['status'];
    }

    $rows[] = array(
      drupal_render($members[$uid]['name']),
      drupal_render($action),
      drupal_render($members[$uid]['remove']),
    );
   }

  $output = theme('table', array('rows' => $rows));

  return $output;
}

/**
 * Form to invite users to the team.
 */
function team_member_invite_form($form, &$form_state, $team) {
  drupal_set_title(t('Invite new member'));

  // Store the team object for use in form handlers
  $form_state['team'] = $team;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#autocomplete_path' => 'user/autocomplete',
    '#required' => TRUE,
    '#description' => t('Enter the user name of the user you wish to invite.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send Invite'),
  );

  return $form;
}

/**
 * Validate handler for the team member invite form.
 *
 * @see team_member_invite_form()
 */
function team_member_invite_form_validate($form, &$form_state) {
  $team = $form_state['team'];

  $account = user_load_by_name($form_state['values']['name']);

  if (!$account) {
    form_set_error('name', t('The username %name does not exist.', array('%name' => $form_state['values']['name'])));
  }
  elseif (isset($team->members[$account->uid])) {
    form_set_error('name', t('@name is already a member of @team. <a href="@manage">Manage team members</a> to view their status.', array('@name' => $account->name, '@team' => $team->name, '@manage' => url('team/' . $team->tid . '/edit/members'))));
  }
  else {
    // Store the user object for use in the form submit handler
    $form_state['account'] = $account;
  }
}

/**
 * Submit handler for the team member invite form.
 *
 * @see team_member_invite_form()
 */
function team_member_invite_form_submit($form, &$form_state) {
  $team = $form_state['team'];
  $account = $form_state['account'];

  team_member_insert($team, $account, TEAM_MEMBER_ROLE_INVITED);

  drupal_set_message(t('%name has successfully been invited to the team.', array('%name' => $account->name)));

  // Prepare parameters
  $params['account'] = $account;
  $params['team'] = $team;

  // Notify the invited user
  drupal_mail('team', 'invite', $account->mail, user_preferred_language($account), $params);

  // Redirect to the manage members page
  $form_state['redirect'] = 'team/' . $team->tid . '/edit/members';
}

/**
 * Form to add users to the team.
 */
function team_member_add_form($form, &$form_state, $team) {
  drupal_set_title(t('Add new member'));

  // Store the team object for use in form handlers
  $form_state['team'] = $team;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#autocomplete_path' => 'user/autocomplete',
    '#required' => TRUE,
    '#description' => t('Enter the user name of the user you wish to add.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add member'),
  );

  return $form;
}

/**
 * Validate handler for the team member add form.
 *
 * @see team_member_add_form()
 */
function team_member_add_form_validate($form, &$form_state) {
  $team = $form_state['team'];

  $account = user_load_by_name($form_state['values']['name']);

  if (!$account) {
    form_set_error('name', t('The username %name does not exist.', array('%name' => $form_state['values']['name'])));
  }
  elseif (isset($team->members[$account->uid])) {
    form_set_error('name', t('%name is already a member of $team. <a href="@manage">Manage team members</a> to view their status.', array('%name' => $account->name, '$team' => $team->name, '@manage' => url('team/' . $team->tid . '/edit/members'))));
  }
  else {
    // Store the user object for use in the form submit handler
    $form_state['account'] = $account;
  }
}

/**
 * Submit handler for the team member add form.
 *
 * @see team_member_add_form()
 */
function team_member_add_form_submit($form, &$form_state) {
  $team = $form_state['team'];
  $account = $form_state['account'];

  team_member_insert($team, $account, TEAM_MEMBER_ROLE_MEMBER);

  drupal_set_message(t('%name has successfully been added to the team.', array('%name' => $account->name)));

  // Notify other modules of the new member
  module_invoke_all('team_member_insert', $team, $account);

  // Redirect to the manage members page
  $form_state['redirect'] = 'team/' . $team->tid . '/edit/members';
}

/**
 * Form for users to join the team.
 */
function team_member_join_form($form, &$form_state, $team) {
  drupal_set_title(t('Join !name', array('!name' => $team->name)));

  global $user;

  if (!$user->uid) {
    drupal_set_message(t('You must first register a user account or login before you can join a team.'), 'warning');
    drupal_goto('user/register');
  }

  // Store the team object for use in form handlers
  $form_state['team'] = $team;

  $form['description'] = array(
    '#markup' => team_join_description($team->joining),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  if (drupal_strlen(trim($team->join_info)) > 0) {
    $form['join_info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Message from the admins'),
      '#description' => filter_xss($team->join_info),
    );
    $form['disclaimer'] = array(
      '#markup' => t('By clicking Join below, you confirm you have read the message set by the team admins above.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }

  if ($team->joining == TEAM_JOIN_PASS) {
    $form['pass'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#required' => TRUE,
      '#attributes' => array('autocomplete' => 'off'),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Join'),
  );
   $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'team/' . $team->tid,
  );

  return $form;
}

/**
 * Validate handler for the team member join form.
 *
 * @see team_member_join_form()
 */
function team_member_join_form_validate($form, &$form_state) {
  $team = $form_state['team'];

  if (isset($form_state['values']['pass'])) {
    // Validate password against the hashed team join pass
    if (!_team_check_password($team, trim($form_state['values']['pass']))) {
      form_set_error('pass', t('The password entered was incorrect.'));
    }
  }
}

/**
 * Submit handler for the team member join form.
 *
 * @see team_member_join_form()
 */
function team_member_join_form_submit($form, &$form_state) {
  global $user;

  $team = $form_state['team'];

  switch ($team->joining) {
    case TEAM_JOIN_CONFIRM:

      $role = TEAM_MEMBER_ROLE_PENDING;

      // Notify team admins
      team_mail_members($team, 'join');

      drupal_set_message(t('Your join request has been created successfully. The team administrator must confirm your request before you will become a member.'));
      break;

    case TEAM_JOIN_AUTO:
    case TEAM_JOIN_PASS:

      $role = TEAM_MEMBER_ROLE_MEMBER;

      // Notify all team members
      team_mail_members($team, 'member', $role);

      drupal_set_message(t('Congratulations, you have successfully joined this team!'));
      module_invoke_all('team_member_insert', $team, $user);
      break;
  }

  team_member_insert($team, $user, $role);

  // Redirect to the profile page
  $form_state['redirect'] = 'team/' . $team->tid;
}

/**
 * Menu callback - ask for confirmation before leaving a team
 */
function team_member_leave_confirm($form, &$form_state, $team) {
  global $user;

  $form_state['team'] = $team;

  if ($team->members[$user->uid] == TEAM_MEMBER_ROLE_ADMIN) {
    // If this member is an admin then check that at least 1 other admin exists
    $admins = array_keys($team->members, TEAM_MEMBER_ROLE_ADMIN);
    if (count($admins) < 2) {
      $form['error'] = array(
        '#markup' => t('You are the last administrator left in this team. Before leaving this team, you must set another administrator from the <a href="@url">Manage members</a> page.', array('@url' => url('team/' . $team->tid . '/edit/members'))),
      );
      return $form;
    }
  }

  return confirm_form($form,
    t('Are you sure you want to leave the team %team?', array('%team' => $team->name)),
    'team/' . $team->tid,
    t('This action cannot be undone.'),
    t('Leave'),
    t('Cancel')
  );
}

/**
 * Submit handler for the team member leave confirm form.
 *
 * @see team_member_leave_confirm()
 */
function team_member_leave_confirm_submit($form, &$form_state) {
  global $user;

  $team = $form_state['team'];

  if ($form_state['values']['confirm']) {
    team_member_delete($team, $user);

    drupal_set_message(t('You have successfully left team @team.', array('@team' => $team->name)));
  }

  // Redirect to the team profile page
  $form_state['redirect'] = 'team/' . $team->tid;
}

/**
 * Menu callback - team member invitation response form
 */
function team_member_invite_respond_form($form, &$form_state, $team) {
  global $user;

  drupal_set_title(t('Respond to invite'));

  $form_state['team'] = $team;
  $form_state['account'] = $user;

  $form['description'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('You have received an invite to join team @team.', array('@team' => $team->name)),
  );

  $form['actions']['accept'] = array(
    '#type' => 'submit',
    '#value' => t('Accept'),
    '#name' => 'accept',
  );
  $form['actions']['reject'] = array(
    '#type' => 'submit',
    '#value' => t('Reject'),
    '#name' => 'reject',
  );
  $form['actions']['reject'] = array(
    '#type' => 'submit',
    '#value' => t('Reject'),
    '#name' => 'reject',
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'team/' . $team->tid),
  );

  return $form;
}

/**
 * Submit handler for the team member invitation response form.
 *
 * @see team_member_invite_respond_form()
 */
function team_member_invite_respond_form_submit($form, &$form_state) {
  $team = $form_state['team'];
  $account = $form_state['account'];
  $op = $form_state['triggering_element']['#name'];

  if ($op == 'accept') {
    db_update('teams_members')
      ->fields(array(
        'role' => TEAM_MEMBER_ROLE_MEMBER,
      ))
      ->condition('tid', $team->tid)
      ->condition('uid', $account->uid)
      ->execute();

    // Notify all team members
    team_mail_members($team, 'member', TEAM_MEMBER_ROLE_MEMBER);

    drupal_set_message(t('Congratulations, you have successfully joined this team!'));

    module_invoke_all('team_member_insert', $team, $account);
  }
  else {
    team_member_delete($team, $account);
    drupal_set_message(t('You have rejected the team invitation.'));
  }

  // Redirect to the team page
  $form_state['redirect'] = 'team/' . $team->tid;
}

/**
 * Menu callback - ask for confirmation of team member removal
 */
function team_member_remove_confirm($form, &$form_state, $team, $uid) {
  $account = user_load($uid);

  $form_state['team'] = $team;
  $form_state['account'] = $account;

  return confirm_form($form,
    t('Are you sure you want to remove member %name from team %team?', array('%name' => $account->name, '%team' => $team->name)),
    'team/' . $team->tid . '/edit/members',
    t('This action cannot be undone.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Submit handler for the team member delete confirm form.
 *
 * @see team_member_remove_confirm()
 */
function team_member_remove_confirm_submit($form, &$form_state) {
  $team = $form_state['team'];
  $account = $form_state['account'];

  if ($form_state['values']['confirm']) {
    team_member_delete($team, $account);

    drupal_set_message(t('Member @name has been deleted from team @team.', array('@name' => $account->name, '@team' => $team->name)));
  }

  // Redirect to the manage members page
  $form_state['redirect'] = 'team/' . $team->tid . '/edit/members';
}

/**
 * Menu callback; Expand a teams member list
 */
function team_member_select_expand($node, $id, $op, $type) {
  if ($type == 'ajax') {
    $commands = array();

    // The table row we want to work with
    $selector = '.participants-wrapper li.participant-' . $id;

    switch ($op) {
      case 'show':
        $build = team_member_select_expand_list($node, $id);
        $html = drupal_render($build);

        $icon = theme('team_member_select_hide_icon');
        $link = l($icon, 'node/' . $node->nid . '/team_member_select/' . $id . '/hide/nojs', array('html' => TRUE, 'attributes' => array('class' => array('use-ajax', 'team-member-select-hide-icon'))));

        $commands[] = ajax_command_append($selector, $html);
        $commands[] = ajax_command_replace($selector . ' .team-member-select-show-icon', $link);
        break;

      case 'hide':
        $icon = theme('team_member_select_show_icon');
        $link = l($icon, 'node/' . $node->nid . '/team_member_select/' . $id . '/show/nojs', array('html' => TRUE, 'attributes' => array('class' => array('use-ajax', 'team-member-select-show-icon'))));

        $commands[] = ajax_command_replace($selector . ' .team-member-select-hide-icon', $link);
        $commands[] = ajax_command_remove($selector . ' table'); // Remove the previous members table
        break;
    }

    $build = array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );

    ajax_deliver($build);
  }
  else {
    drupal_goto();
  }
}

/**
 * Display an expanded list.
 */
function team_member_select_expand_list($node, $id) {
  $team = team_load($id);

  if (!empty($node->member_select)) {
    $members = team_tournaments_members($node, $id);
  }
  else {
    $members = _team_members_active($team);
  }

  $accounts = user_load_multiple(array_keys($members));

  $rows = array();
  foreach ($accounts as $uid => $account) {
    $name = theme('username', array('account' => $account));
    $rows[$uid] = array('name' => $name);
  }

  $path = 'node/' . $node->nid . '/edit/participants/' . $id . '/members';

  $build = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => array(array('data' => t('Name'), 'align' => 'left')),
    '#empty' => t('No members selected.'),
    '#attributes' => array(
      'class' => array('team-members'),
    ),
  );

  if (_team_tournament_participant_members_access($node, $team->tid)) {
    $build['#caption'] = l(t('Edit members'), $path);
  }

  // Allow other modules to alter the structure
  drupal_alter('team_member_select_expand', $build, $node, $team);

  return $build;
}

/**
 * Menu callback; Form builder for editing a teams tournament members
 */
function team_tournament_participant_members_form($form, &$form_state, $node, $id) {
  $form_state['node'] = $node;

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );

  $team = team_load($id);
  $form_state['team'] = $team;

  // Available team members
  $options = team_member_select_options($team);

  // Show all team tournament members (even unconfirmed)
  $members = team_tournaments_members($node, $id, 0);
  $form_state['storage']['members'] = $members;

  if ($unconfirmed = array_keys($members, 0)) {
    foreach ($unconfirmed as $uid) {
      $usernames[] = $options[$uid];
    }

    drupal_set_message(t('Members %members are awaiting confirmation from the tournament administrator before they appear on the tournament participant list.', array('%members' => implode(', ', $usernames))), 'warning');
  }

  $form['members'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Members'),
    '#options' => $options,
    '#default_value' => array_keys($members),
    '#description' => t('New members that are selected will require approval from a tournament admin before they become official.')
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'node/' . $node->nid),
  );

  return $form;
}

/**
 * Submit handler for editing a teams tournament members
 */
function team_tournament_participant_members_form_submit($form, &$form_state) {
  $node = $form_state['node'];
  $members = $form_state['storage']['members'];
  $id = $form_state['values']['id'];
  $team = $form_state['team'];

  foreach ($form_state['values']['members'] as $uid => $status) {
    // The member previous status
    $previous = array_key_exists($uid, $members);

    if (empty($status) && $previous) {
      // A member was unchecked
      db_delete('teams_tournaments_members')
        ->condition('nid', $node->nid)
        ->condition('tid', $id)
        ->condition('uid', $uid)
        ->execute();
    }
    elseif ($status && !$previous) {
      // A member was checked
      db_insert('teams_tournaments_members')
        ->fields(array(
          'nid' => $node->nid,
          'tid' => $id,
          'uid' => $uid,
          'status' => 0,
          'created' => REQUEST_TIME,
        ))
        ->execute();

      // Mail tournament admins
      $params = array(
        'team' => $team,
        'node' => $node,
        'uid' => $uid,
      );
      tournament_mail_admins($node, 'tournament_member_select', $params, 'team');
    }
  }

  drupal_set_message(t('Members updated successfully. Any new members that were selected require approval from a tournament admin before they become official.'), 'warning');
  $form_state['redirect'] = 'node/' . $node->nid;
}

/**
 * Menu callback; Expand a teams member list
 */
function team_member_select_update_form($form, &$form_state, $node, $id, $uid) {
  $team = team_load($id);
  $account = user_load($uid);

  if (empty($team) || empty($account)) {
    drupal_goto('node/' . $node->nid);
  }

  $form_state['node'] = $node;
  $form_state['account'] = $account;
  $form_state['team'] = $team;

  $form['description'] = array(
    '#markup' => t('!team requested to add member !account to their member list.', array('!team' => l($team->name, 'team/' . $team->tid), '!account' => theme('username', array('account' => $account)))),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['accept'] = array(
    '#type' => 'submit',
    '#value' => t('Accept'),
    '#submit' => array('team_member_select_update_form_submit_accept'),
  );
  $form['actions']['reject'] = array(
    '#type' => 'submit',
    '#value' => t('Reject'),
    '#submit' => array('team_member_select_update_form_submit_reject'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'node/' . $node->nid),
  );

  return $form;
}

function team_member_select_update_form_submit_accept($form, &$form_state) {
  $node = $form_state['node'];
  $team = $form_state['team'];
  $account = $form_state['account'];

  db_update('teams_tournaments_members')
    ->fields(array(
      'status' => 1,
    ))
    ->condition('nid', $node->nid)
    ->condition('tid', $team->tid)
    ->condition('uid', $account->uid)
    ->execute();

  drupal_set_message(t('Team members successfully updated for %name.', array('%name' => $team->name)));

  $form_state['redirect'] = 'node/' . $node->nid;
}

function team_member_select_update_form_submit_reject($form, &$form_state) {
  $node = $form_state['node'];
  $team = $form_state['team'];
  $account = $form_state['account'];

  db_delete('teams_tournaments_members')
    ->condition('nid', $node->nid)
    ->condition('tid', $team->tid)
    ->condition('uid', $account->uid)
    ->execute();

  drupal_set_message(t('Team members successfully updated for %name.', array('%name' => $team->name)));

  $form_state['redirect'] = 'node/' . $node->nid;
}