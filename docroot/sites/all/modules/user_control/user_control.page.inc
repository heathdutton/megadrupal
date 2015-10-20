<?php

/**
 * Page callback: assign roles to users.
*/
function _user_control_page() {
  $session = isset($_SESSION['user_control_filter']) ? $_SESSION['user_control_filter'] : array();
  $query = db_select('users', 'u');
  $query->leftjoin('users_roles', 'r', 'r.uid = u.uid');
  $query->addExpression('GROUP_CONCAT(DISTINCT r.rid)', 'roles_list');
  $query->groupBy('u.uid');
  $query->fields('r', array('rid'));
  $query->condition('u.uid', 0, '<>')
    ->fields('u', array('uid', 'name'))
    ->orderBy('name', 'ASC');
  
  if (isset($session['user_email']) && !empty($session['user_email'])) {
    $query->condition('mail', $session['user_email'], '=');
  }
  elseif (isset($session['user_name']) && !empty($session['user_name'])) {
    $query->condition('name', $session['user_name'] .'%', 'LIKE');
  }
  
  $result = $query->extend('PagerDefault')
    ->limit(25)
    ->execute();

  $rows = array();
  foreach ($result as $obj_account) {
    $rows[] = array(
      array(
        'data' => $obj_account->name
      ),
      array(
        'data' => _user_control_add_ajax_link(
          $obj_account->uid,
          $obj_account->roles_list
        )
      ),
    );
  }
  $output = array();
  $output['form'] = drupal_get_form('_user_control_filter_form');
  
  $output['table'] = array(
    'data' => array(
      '#header' => array(
        array('data' => t('User login')),
        array('data' => t('Roles'))
      ),
      '#rows' => $rows,
      '#theme' => 'table',
    ),
    'pager' => array(
      '#theme' => 'pager',
    ),
  );
  
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'jquery.form');
  drupal_add_css(drupal_get_path('module', 'user_control') .'/user_control.css');

  return $output;
}


/**
 * Add AJAX links for role control.
 */
function _user_control_add_ajax_link($uid, $roles = '') {
  $output = '';
  if (empty($roles)) {
    $query = db_select('users_roles', 'r');
    $roles = $query->condition('r.uid', $uid, '=')
      ->fields('r', array('rid'))
      ->execute()
      ->fetchAllKeyed(0, 0);
  }
  else {
    $roles = explode(',', $roles);
  }
  
  return theme('user_control_ajax_buttons', array(
    'uid' => $uid,
    'roles' => $roles
  ));
}


/**
 * Filter form.
 */
function _user_control_filter_form($form, &$form_state) {
  $form = array();
  
  $session = isset($_SESSION['user_control_filter']) ? $_SESSION['user_control_filter'] : array();
  $form['user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('User name'),
    '#default_value' => (isset($session['user_name']) ? $session['user_name'] : ''),
    '#prefix' => '<div class="role_controle_filter">',
    '#size' => 30,
    '#maxlength' => 32,
  );
  
  $form['user_email'] = array(
    '#type' => 'textfield',
    '#title' => t('User email'),
    '#default_value' => (isset($session['user_email']) ? $session['user_email'] : ''),
    '#size' => 30,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  
  $form['reset'] = array(
    '#type' => 'submit',
    '#suffix' => '</div><div class="clearfix"></div>',
    '#value' => t('Reset'),
  );
  
  $form['user_information'] = array(
    '#markup' => t('User name or somting first litter. Also we may enter e-mail.'),
    '#prefix' => '<div class="role_controle_filter_info">',
    '#suffix' => '</div>',
  );
  return $form;
}


/**
 * Save filter parameters to session.
 */
function _user_control_filter_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Reset')) {
    $_SESSION['user_control_filter'] = array();
  }
  elseif ($form_state['values']['op'] == t('Filter')) {
    $_SESSION['user_control_filter'] = array(
      'user_name' => $form_state['values']['user_name'],
      'user_email' => $form_state['values']['user_email'],
    );
  }
}


/**
 * Callback menu: AJAX proccess role.
 */
function _user_control_ajax($uid, $action, $role, $mode = NULL) {
  if ($mode != 'ajax') {
    drupal_set_message(t('Turn on Javascript'));
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }
  
  $user_roles = user_roles(TRUE);
  $account = user_load($uid);
  $new_roles = $account->roles;
  
  if ($action == 'add') {
    $new_roles[$role] = $user_roles[$role];
  }
  elseif ($action == 'del') {
    unset($new_roles[$role]);
  }
  if (user_access('assign roles')) {
    user_save($account, array('roles' => array_filter($new_roles)));
  }
  
  $commands = array();
  $commands[] = ajax_command_html('#role_controle_uid_'. $uid,
    _user_control_add_ajax_link($uid)
  );
  return array('#type' => 'ajax', '#commands' => $commands);
}


/**
 * Settings form.
 */
function _user_control_settings_form() {
  $form = array();
  $form['user_control_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Roles',
    '#description' => 'Choose a role that can be managed.',
    '#options' => user_roles(TRUE),
    '#required' => TRUE,
    '#default_value' => variable_get('user_control_roles', array()),
  );
  return system_settings_form($form);
}


/**
 * Theming AJAX links.
 */
function theme_user_control_ajax_buttons($variables) {
  $user_control = variable_get('user_control_roles', array());
  $user_roles = user_roles(TRUE);
  $buttons = '';
  foreach ($user_control as $rid => $status) {
    if (!$status) {
      continue;
    }
    if (in_array($rid, $variables['roles'])) {
      //Link Delete role
      $buttons .= '  <div class="rc_button rc_delete"><div class="rc_button_wrap">'.
        l($user_roles[$rid],
          'admin_users_js/'. $variables['uid'] .'/del/'. $rid .'/nojs',
            array(
              'attributes' => array(
                'class' => array('use-ajax'),
                'title' => $user_roles[$rid],
              )
            )
         ) .'</div></div>
';
    }
    else {
      //Link Add role
      $buttons .= '  <div class="rc_button rc_add"><div class="rc_button_wrap">'.
        l($user_roles[$rid],
          'admin_users_js/'. $variables['uid'] .'/add/'. $rid .'/nojs',
          array(
            'attributes' => array(
              'class' => array('use-ajax'),
              'title' => $user_roles[$rid],
            )
          )
        ) .'</div></div>
';
    }
  }
  return '<div id="role_controle_uid_'. $variables['uid']
    .'" class="role_controle_buttons"><div class="rc_buttons_wrap">
'. $buttons .'</div></div>
';
}

