<?php
// $Id$

/**
 * @file
 *
 */

function admin_alert_settings_form() {
  $roles = user_roles(TRUE);
  unset($roles[2]);
  $form['admin_alert_roles_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Administrator Roles'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Users with these roles will receive administrative
      alerts by email and on site.') . '<br/><span style="color:red;"><em>' .
      t('WARNING! Generally, only roles that can configure the site and
        administer users should be added here; otherwise, you may be spamming
        your users.') . '</em></span><br/>' . t('All roles specified here will
        automatically be given the permission "view admin alerts".'),
    'admin_alert_roles' => array(
      '#type' => 'checkboxes',
      '#title' => '',
      '#options' => $roles,
      '#default_value' => variable_get('admin_alert_roles', array()),
    ),
    'admin_alert_exclude_user_1' => array(
      '#type' => 'checkbox',
      '#title' => t('Exclude user 1 from receiving admin alerts'),
      '#description' => t('This option is useful if user 1 is reserved only for administrative purposes.'),
      '#default_value' => variable_get('admin_alert_exclude_user_1', FALSE),
    ),
  );

  global $base_root;
  $form['admin_alert_email_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Emails'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Change the email address that receives site emails.'),
    'admin_alert_alter_email' => array(
      '#type' => 'checkbox',
      '#title' => t('Send administrative email to all admins'),
      '#description' => t('Alters outgoing mail so that all items intended for the
        default site email address, %email, are sent to all administrators (users
        with the above roles) instead.  Most notably this means notifications of
        new accounts, but it also includes default implementations of the contact
        form and update notifications.',
        array('%email' => variable_get('site_email', ini_get('sendmail_from')))),
      '#default_value' => variable_get('admin_alert_alter_email', FALSE),
    ),
    'admin_alert_replace_site_email' => array(
      '#type' => 'checkbox',
      '#title' => t('Exclude site email address from administrative emails'),
      '#description' => t('This option is useful, for example, if your site email
        address is unmonitored, such as %email.', array('%email' => 'no-reply@' . $base_root)),
      '#default_value' => variable_get('admin_alert_replace_site_email', FALSE),
      '#states' => array(
        'enabled' => array(
          ':input[name="admin_alert_alter_email"]' => array('checked' => TRUE)
        )
      ),
    ),
  );
  $form['admin_alert_use_admin_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use admin CSS'),
    '#description' => t('If you put the block on administrative pages, you might like the way this css makes it look.'),
    '#default_value' => variable_get('admin_alert_use_admin_css', TRUE),
  );
  $form = system_settings_form($form);
  $form['#submit'][] = 'admin_alert_settings_form_submit';
  return $form;
}

function admin_alert_settings_form_submit(&$form, &$form_state) {
  $roles = array_keys(array_filter($form_state['values']['admin_alert_roles']));
  variable_set('admin_alert_roles', $roles);
  foreach ($roles as $rid) {
    user_role_grant_permissions($rid, array('view admin alerts'));
  }
}

function admin_alert_report($delta = NULL) {
    
  // query admin_alert and admin_alert_users tables
  // filter on current user, unread
  global $user;
  if ($delta == 'all') {
    $alerts = admin_alert_load_multiple(FALSE, array('uid' => $user->uid, 'closed' => 0));
  }
  else {
    $alerts = admin_alert_load_multiple(FALSE, array('uid' => $user->uid, 'user_read' => 0, 'closed' => 0));
  }

  $output = t('No alerts were found.');
  
  if (!empty($alerts)) {
    foreach ($alerts as $alert) {
      if ($alert->path && stripos(request_uri(), $alert->path) !== FALSE) {
        $output = drupal_render(admin_alert_view($alert));
        $skip_list = TRUE;
        break;
      }
      $items[] = array(
        'data' => drupal_render(admin_alert_view($alert, 'list')),
        'class' => array('admin-alert', $alert->type),
      );
    }
    if (!$skip_list) {
      $output = theme('item_list', array(
        'items' => $items,
        'type' => 'ul',
        'attributes' => array('class' => array('admin-alert'))
      ));
    }
  }
  drupal_add_css(drupal_get_path('module', 'admin_alert') . '/admin_alert.css');
  drupal_add_js(drupal_get_path('module', 'admin_alert') . '/admin_alert.js');
  return $output;

}

function admin_alert_admin($type) {
  $alerts = admin_alert_load_multiple(FALSE, array('type' => $type, 'count' => TRUE));
  $output = t('No alerts were found.');
  if (!empty($alerts)) {
    foreach ($alerts as $alert) {
      $items[] = array(
        'data' => drupal_render(admin_alert_view($alert, 'list')),
        'class' => array('admin-alert', $alert->type),
      );
    }
    $output = theme('item_list', array(
      'items' => $items,
      'type' => 'ul',
      'attributes' => array('class' => array('admin-alert'))
    ));
  }
  drupal_add_css(drupal_get_path('module', 'admin_alert') . '/admin_alert.css');
  drupal_add_js(drupal_get_path('module', 'admin_alert') . '/admin_alert.js');
  return $output;

}

function admin_alert_mark($alert, $op, $token) {
  $return = "alert-$alert->aaid-$op-$token";
  if (drupal_valid_token($token, "alert-$op-$alert->aaid")) {
    switch ($op) {
      case 'close':
        $alert->closed = TRUE;
        if (!admin_alert_save($alert)) {
          $return = 'error';
        }
        break;
      case 'delete':
        if (!entity_get_controller('admin_alert')->delete($alert)) {
          $return = 'error';
        }
        break;
      case 'read':
        global $user;
        if (!db_update('admin_alert_users')
          ->condition('uid', $user->uid)
          ->condition('aaid', $alert->aaid)
          ->fields(array('user_read' => time()))
          ->execute()){
          $return = 'error';
        }
        cache_clear_all("admin_alert:block:user_$user->uid", 'cache_block');
        break;
    }
    if ($_POST['js'] == 1) {
      drupal_json($return);
      exit();
    }
    $return = drupal_get_destination();
    drupal_goto($return['destination']);
  }
  drupal_access_denied();
}

function admin_alert_add($type) {
  $alert = entity_get_controller('admin_alert')->create($type);
  return drupal_get_form('admin_alert_add_form', $alert);
}

function admin_alert_add_form($form, &$form_state, $alert) {
  $form['send'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send this alert to administrators.'),
    '#default_value' => $alert->send,
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('Drupal path to which the alert points.'),
    '#default_value' => $alert->path,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#description' => t('Machine name for this alert.  Defaults to a PHP uniqid.'),
    '#default_value' => $alert->name,
  );
  foreach(array('aaid', 'type', 'translate', 'replacements', 'closed', 'created') as $field) {
    $form[$field] = array(
      '#type' => 'value',
      '#value' => $alert->$field
    );
  }
  $form['#admin_alert'] = $alert;
  $form_state['admin_alert'] = $alert;
  field_attach_form('admin_alert', $alert, $form, $form_state);
  unset($form['path']['#element_validate']);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  return $form;
}

function admin_alert_add_form_validate($form, &$form_state) {
  field_attach_form_validate('admin_alert', $form_state['admin_alert'], $form, $form_state);
}

function admin_alert_add_form_submit($form, &$form_state) {
  $alert = $form_state['values']['admin_alert'];
  foreach (array('path', 'send', 'name', 'aaid', 'type', 'translate', 'replacements', 'closed', 'created') as $field) {
    $alert->$field = $form_state['values'][$field];
  }
  field_attach_submit('admin_alert', $alert, $form, $form_state);
  $alert = admin_alert_save($alert);
  $form_state['redirect'] = 'admin-alert/' . $alert->aaid;
}
