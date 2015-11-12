<?php

/**
 * @file
 * Adds UI for listing Url redirect path(s).
 */

/**
 * Implements listing form.
 */
function url_redirect_list_form($form, &$form_state) {
  global $base_url;
  $form = array();
  $form['goto_list'] = array(
    '#markup' => l(t('Add Url Redirect'), 'admin/config/url_redirect'),
  );
  $form['path'] = array(
    '#title' => t('Path'),
    '#type' => 'textfield',
    '#default_value' => isset($_GET['path']) ? $_GET['path'] : '',
  );
  $form['redirect_path'] = array(
    '#title' => t('Redirect Path'),
    '#type' => 'textfield',
    '#default_value' => isset($_GET['redirect_path']) ? $_GET['redirect_path'] : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => 'Reset',
  );
  $query = db_select('url_redirect', 'u')
      ->fields('u');
  if (!empty($_GET['path'])) {
    $query->condition('path', '%' . db_like($_GET['path']) . '%', 'LIKE');
  }
  if (!empty($_GET['redirect_path'])) {
    $query->condition('redirect_path', '%' . db_like($_GET['redirect_path']) . '%', 'LIKE');
  }
  $result = $query->execute()->fetchAll();
  // Header for the list of Redirects.
  $header = array(
    array('data' => t('Path')),
    array('data' => t('Redirect Path')),
    array('data' => t('Checked For')),
    array('data' => t('Roles')),
    array('data' => t('Users')),
    array('data' => t('Status')),
    array('data' => t('Display Message')),
    array('data' => t('Edit link')),
    array('data' => t('Delete link')),
  );
  $rows = array();
  $output = '';
  foreach ($result as $url) {
    // Edit link.
    $edit_link = $base_url . '/admin/config/url_redirect/edit?path=' . $url->path;
    // Delete link.
    $delete_link = $base_url . '/admin/config/url_redirect/delete?path=' . $url->path;
    // Get the list of all the Roles.
    if ($url->roles) {
      $roles_names = array_keys((array) json_decode($url->roles));
      $roles = '';
      foreach ($roles_names as $rid) {
        $rolename = db_select('role', 'r')
            ->fields('r', array('name'))
            ->condition('rid', $rid);
        $role_result = $rolename->execute()->fetchField();
        if ($role_result) {
          $roles .= $role_result . '(' . $rid . ')' . ', ';
        }
      }
      $list_of_roles = rtrim($roles, ', ');
    }
    else {
      $list_of_roles = 'N/A';
    }
    // Get the list of all the Users.
    if ($url->users) {
      $user_names = array_keys((array) json_decode($url->users));
      $names = '';
      foreach ($user_names as $uid) {
        $username = db_select('users', 'u')
            ->fields('u', array('name'))
            ->condition('uid', $uid);
        $user_result = $username->execute()->fetchField();
        if ($user_result) {
          $names .= $user_result . '(' . $uid . ')' . ', ';
        }
      }
      $list_of_users = rtrim($names, ', ');
    }
    else {
      $list_of_users = 'N/A';
    }
    // Get the status.
    if ($url->status == 1) {
      $status = 'Enabled';
    }
    else {
      $status = 'Disabled';
    }
    // Get the message.
    if ($url->message == 'Yes') {
      $message = 'Enabled';
    }
    else {
      $message = 'Disabled';
    }
    $rows[] = array(
      array('data' => $url->path),
      array('data' => $url->redirect_path),
      array('data' => $url->check_for),
      array('data' => $list_of_roles),
      array('data' => $list_of_users),
      array('data' => $status),
      array('data' => $message),
      array('data' => l(t('Edit'), $edit_link)),
      array('data' => l(t('Delete'), $delete_link)),
    );
  }
  if (count($rows) > 0) {
    $per_page = 25;
    $current_page = pager_default_initialize(count($rows), $per_page);
    $chunks = array_chunk($rows, $per_page, TRUE);
    $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
    $output .= theme('pager', array('quantity', count($rows)));
    $form['output'] = array(
      '#markup' => $output,
    );
  }
  else {
    $form['output'] = array(
      '#markup' => t('No Paths available.'),
    );
  }
  return $form;
}

/**
 * Implements hook_form_state().
 */
function url_redirect_list_form_submit($form, &$form_state) {
  // Goto current path if reset.
  if ($form_state['values']['op'] == 'Reset') {
    drupal_goto(current_path());
  }
  // Pass values to url.
  if ($form_state['values']['op'] == 'Filter') {
    $filter_path = $form_state['values']['path'];
    $filter_redirect_path = $form_state['values']['redirect_path'];
    $params['path'] = check_plain($filter_path);
    $params['redirect_path'] = check_plain($filter_redirect_path);
    drupal_goto('admin/config/url_redirect/list', array('query' => $params));
  }
}
