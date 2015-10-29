<?php

/**
 * @file
 * UI functionality for project issue e-mail notifications.
 *
 * @see mail.inc
 */

/**
 * Returns project issue notification levels.
 */
function _project_issue_notification_levels() {
  $levels = array(
    PROJECT_ISSUE_NOTIFICATION_NONE => t('None'),
    PROJECT_ISSUE_NOTIFICATION_OWN => t('Own issues'),
    PROJECT_ISSUE_NOTIFICATION_ALL => t('All issues'),
  );
  $flags = project_issue_get_follow_flags();
  if (!empty($flags)) {
    $levels[PROJECT_ISSUE_NOTIFICATION_OWN] = t('Issues you follow');
  }
  return $levels;
}

/**
 * Form constructor for global project issue notification settings for a user.
 */
function project_issue_notification_user_form($form, &$form_state, $account) {
  // Global notification level.
  project_issue_notification_user_settings_load($account);

  $options = _project_issue_notification_levels();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );
  $form['#tree'] = TRUE;

  $form['issue_notification_header'] = array(
    '#markup' => '<h3>' . t('Issue e-mail notifications') . '</h3>',
  );

  $token = drupal_get_token('project-issue-manage-notifications');

  // Per-project notification levels.
  $form['projects'] = array(
    '#theme' => 'project_issue_notification_projects_table',
    '#header' => array(t('Project'), t('Send e-mail'), t('Operations')),
  );

  $form['projects']['default'] = array(
    'title' => array(
      '#type' => 'item',
      '#markup' => t('Default notification'),
      '#description' => t('Customize for individual projects below.'),
    ),
    'level' => array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $account->project_issue_notification['level'],
      '#parents' => array('projects', 'default'),
    ),
    // We don't need any operations for this one.
    'operations' => array(),
  );

  $query = db_select('node', 'n')
    ->addTag('node_access')
    ->condition('n.status', 1);
  $query->innerJoin('project_issue_notification_project', 'pisp', 'n.nid = pisp.nid');
  $result = $query
    ->condition('pisp.uid', $account->uid)
    ->orderBy('n.title')
    ->fields('pisp', array('nid', 'level'))
    ->fields('n', array('title'))
    ->execute();
  foreach ($result as $project) {
    $form['projects'][$project->nid] = array(
      'title' => array(
        '#markup' => l($project->title, "node/$project->nid"),
      ),
      'level' => array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $project->level,
        // Adjust #parents to get a simple $project->nid => $level mapping in
        // the submit handler. Without adjustment, the value would be
        // array('level' => $level).
        '#parents' => array('projects', $project->nid),
      ),
      'operations' => array(
        '#markup' => l(t('delete'), "user/$account->uid/project-issue/delete/$project->nid", array('query' => array('token' => $token))),
      ),
    );
  }

  $form['projects']['new'] = array(
    'title' => array(
      '#type' => 'textfield',
      '#size' => 30,
      '#autocomplete_path' => 'project/autocomplete/issue/project',
      '#attributes' => array('placeholder' => t('Enter a project title')),
    ),
    'level' => array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => PROJECT_ISSUE_NOTIFICATION_ALL,
    ),
    // We don't need any operations for this one.
    'operations' => array(),
  );

  $form['notify_own_updates'] = array(
    '#type' => 'checkbox',
    '#title' => t('I want to be notified of changes that I make myself'),
    '#default_value' => $account->project_issue_notification['notify_own_updates'],
  );

  $options = array(
    'mail_subject_project' => t('Project name'),
    'mail_subject_category' => t('Issue category'),
  );
  $defaults = array_keys(array_filter(array_intersect_key($account->project_issue_notification, $options)));
  $form['project_issue_mail_subject'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Subject includes'),
    '#options' => $options,
    '#default_value' => $defaults,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  return $form;
}

/**
 * Form validation handler for project_issue_notification_user_form().
 *
 * If the user is trying to add notification preferences for a new project, we
 * validate the project title. If we can't find the project, we set a form
 * error. If we find the project, we save the nid into $form_state so we can
 * use that in the submit handler.
 */
function project_issue_notification_user_form_validate($form, &$form_state) {
  $new_project = $form_state['values']['projects']['new'];
  if (!empty($new_project['title'])) {
    $nid = db_query('SELECT nid FROM {node} WHERE title = :title AND type IN (:types)', array(':title' => $new_project['title'], ':types' => project_project_node_types()))->fetchField();
    $node = node_load($nid);
    if (!$node) {
      form_set_error('projects][new][title', t('The name you entered (%title) is not a valid project.', array('%title' => $new_project['title'])));
    }
    elseif (!node_access('view', $node)) {
      form_set_error('projects][new][title', t('You do not have access to %title.', array('%title' => $new_project['title'])));
    }
    elseif (!empty($form_state['values']['projects'][$nid])) {
      form_set_error('projects][new][title', t('You have already defined issue notification settings for the %title project.', array('%title' => $new_project['title'])));
    }
    else {
      $form_state['values']['projects']['new']['nid'] = $nid;
    }
  }
}

/**
 * Form submission handler for project_issue_notification_user_form().
 */
function project_issue_notification_user_form_submit($form, &$form_state) {
  // Update the global issue notification settings.
  $account = $form_state['values']['account'];

  // First, save the default setting for this user.
  $account->project_issue_notification = array(
    'level' => (int) $form_state['values']['projects']['default'],
    'notify_own_updates' => (int) !empty($form_state['values']['notify_own_updates']),
    'mail_subject_project' => (int) !empty($form_state['values']['project_issue_mail_subject']['mail_subject_project']),
    'mail_subject_category' => (int) !empty($form_state['values']['project_issue_mail_subject']['mail_subject_category']),
  );
  project_issue_notification_user_settings_save($account);

  unset($form_state['values']['projects']['default']);

  // Now, see if they're trying to add notifications for a new project.
  $new_project = $form_state['values']['projects']['new'];
  if (!empty($new_project['nid'])) {
    project_issue_notification_project_setting_save($account->uid, $new_project['nid'], $new_project['level']);
  }
  // Either way, we're now done with this row.
  unset($form_state['values']['projects']['new']);

  // Finally, save any changes to the per-project settings.
  if (!empty($form_state['values']['projects'])) {
    foreach ($form_state['values']['projects'] as $nid => $level) {
      project_issue_notification_project_setting_save($account->uid, $nid, $level);
    }
  }

  drupal_set_message(t('Your notification settings have been updated.'));
}

/**
 * Returns HTML for the table of per-project notification settings.
 *
 * @see project_issue_user_notification_form()
 */
function theme_project_issue_notification_projects_table($variables) {
  $element = $variables['element'];
  $output = '';
  $rows = array();
  foreach (element_children($element) as $nid) {
    $row = array();
    $row[] = drupal_render($element[$nid]['title']);
    $row[] = drupal_render($element[$nid]['level']);
    $row[] = drupal_render($element[$nid]['operations']);
    $rows[] = $row;
  }
  if (!empty($rows)) {
    $output .= theme('table', array('header' => $element['#header'], 'rows' => $rows));
  }

  $output .= drupal_render_children($element);
  return $output;
}

/**
 * Form constructor for per-project issue notification settings.
 */
function project_issue_notification_project_form($form, &$form_state, $project) {
  global $user;

  // Ensure the account has a valid e-mail address; may not be the case for
  // accounts created by external authentication providers.
  if (!valid_email_address($user->mail)) {
    drupal_set_message(t('You need to provide a valid e-mail address to receive issue notification e-mails. Please edit your user information.'), 'error');
    drupal_goto('user/' . $user->uid . '/edit');
  }

  project_project_set_breadcrumb($project, TRUE);
  drupal_set_title(t('Manage e-mail notifications for @project issues', array('@project' => $project->title)));

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $project->nid,
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );

  $level = project_issue_notification_project_setting_load($user->uid, $project->nid);
  $form['level'] = array(
    '#type' => 'radios',
    '#title' => t('Send e-mail'),
    '#options' => _project_issue_notification_levels(),
    '#default_value' => $level,
    '#description' => t('These settings only apply to the %project project. Manage notifications for all projects in your <a href="@account-url">user account</a>.', array(
      '%project' => $project->title,
      '@account-url' => url('user/' . $user->uid . '/project-issue'),
    )),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Form submission handler for project_issue_project_notification_form().
 */
function project_issue_notification_project_form_submit($form, &$form_state) {
  project_issue_notification_project_setting_save($form_state['values']['uid'], $form_state['values']['nid'], $form_state['values']['level']);

  drupal_set_message(t('Your notification settings have been updated.'));
}

function project_issue_notification_user_delete($account, $project) {
  $token = $_GET['token'];
  if (drupal_valid_token($token, 'project-issue-manage-notifications')) {
    project_issue_notification_project_setting_delete($account->uid, $project->nid);
    drupal_set_message(t('Deleted notification settings for %title.', array('%title' => $project->title)));
  }
  else {
    drupal_set_message(t('Invalid token, not deleting anything.'), 'error');
  }
  return drupal_goto("user/$account->uid/project-issue");
}
