<?php

/*
 * largely copied from user_admin, which it replaces
 */
function user_admin_uid($callback_arg = '') {
  module_load_include('admin.inc', 'user');
  $op = isset($_POST['op']) ? $_POST['op'] : $callback_arg;

  switch ($op) {
    case t('Create new account'):
    case 'create':
      $build['user_register'] = drupal_get_form('user_register_form');
      break;
    default:
      if (!empty($_POST['accounts']) && isset($_POST['operation']) && ($_POST['operation'] == 'cancel')) {
        $build['user_multiple_cancel_confirm'] = drupal_get_form('user_multiple_cancel_confirm');
      }
      else {
        $build['user_filter_form'] = drupal_get_form('user_filter_form');
        $build['user_admin_account'] = drupal_get_form('user_admin_account_uid');//matslats
      }
  }
  return $build;
}



/*
 * implements HOOK_form_FORM_ID_alter
 * rebuilds the user login page to include uid support
 * has to completely replace the user_admin_account
 * because if we do a form alter a second pager is created and i can't see how to hack it
 */
function user_admin_account_uid($form, $form_state) {
  //add the uid column on the header, where it can be included in the sort
  $header = array(
    'uid' => array(
      'data' => t('User ID'),
      'field' => 'u.uid',
      'sort' => 'desc'
    ),
    'username' => array(
      'data' => t('Username'),
      'field' => 'u.name',
    ),
    'status' => array(
      'data' => t('Status'),
      'field' => 'u.status',
    ),
    'roles' => array(
      'data' => t('Roles')
    ),
    'member_for' => array(
      'data' => t('Member for'),
      'field' => 'u.created',
    ),
    'access' => array(
      'data' => t('Last access'),
      'field' => 'u.access',
    ),
    'operations' => array(
      'data' => t('Operations')
    ),
  );

  $query = db_select('users', 'u');
  $query->condition('u.uid', 0, '<>');
  user_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(u.uid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('u', array('uid', 'name', 'status', 'created', 'access'))
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();


  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $options = array();
  foreach (module_invoke_all('user_operations') as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'unblock',
  );
  $form['#validate'] = array('user_admin_account_validate');
  $form['#submit'] = array('user_admin_account_submit');
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $destination = drupal_get_destination();

  $status = array(t('blocked'), t('active'));
  $roles = array_map('check_plain', user_roles(TRUE));
  $options = array();
  foreach ($result as $account) {
    $users_roles = array();
    $roles_result = db_query('SELECT rid FROM {users_roles} WHERE uid = :uid', array(':uid' => $account->uid));
    foreach ($roles_result as $user_role) {
      $users_roles[] = $roles[$user_role->rid];
    }
    asort($users_roles);

    $options[$account->uid] = array(
      //add the uid column to the table
      'uid' => $account->uid,
      'username' => theme('username', array('account' => $account)),
      'status' =>  $status[$account->status],
      'roles' => theme('item_list', array('items' => $users_roles)),
      'member_for' => format_interval(REQUEST_TIME - $account->created),
      'access' =>  $account->access ? t('@time ago', array('@time' => format_interval(REQUEST_TIME - $account->access))) : t('never'),
      'operations' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('edit'),
          '#href' => "user/$account->uid/edit",//matslats
          '#options' => array('query' => $destination)
        )
      )
    );
  }

  $form['accounts'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No people available.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}


/*
 * implements uid_login hook_uid_renum
 */
function field_uid_renum($old, $new) {
  foreach (field_info_instances('user', 'user') as $instance) {
    db_update('field_data_'. $instance['field_name'])
      ->fields(array('entity_id' => $new))
      ->condition('entity_type', 'user')
      ->condition('entity_id', $old)
      ->execute();
    db_update('field_revision_'. $instance['field_name'])
      ->fields(array('entity_id' => $new))
      ->condition('entity_type', 'user')
      ->condition('entity_id', $old)
      ->execute();
  }
}
/*
 * implements uid_login hook_uid_renum for node table's uid field
 */
function node_uid_renum($old, $new) {
  db_update('node')->fields(array('uid' => $new))->condition('uid', $old)->execute();
}
/*
 * implements uid_login hook_uid_renum users table and users_roles table
 */
function user_uid_renum($old, $new) {
  db_update('users')->fields(array('uid' => $new))->condition('uid', $old)->execute();
  db_update('users_roles')->fields(array('uid' => $new))->condition('uid', $old)->execute();
}
