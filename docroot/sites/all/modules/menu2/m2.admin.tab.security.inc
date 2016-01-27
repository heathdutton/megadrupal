<?php

require_once 'm2.constants.inc';

/**
 * m2_admin_security_page
 * m2_admin_security_form
 */

function m2_admin_security_page() {
  drupal_set_title(t('Administer Menu2'));
  $output = m2_form_render('m2_admin_security_form');
  return theme('m2_content_wrapper', array('class' => array('m2-tab', 'm2-security-tab', 'clearfix'), 'markup' => $output));
}


function m2_admin_security_form($form, &$form_state) {
  $perms_info = m2_permission();
  $perms_id = array_reverse(array_keys($perms_info));
  $roles_id = array(); /* only roles that assigned to permissions */
  $perms_ws_roles = array();
  foreach ($perms_id as $c_perm_id) {
    $c_roles_info = _m2_sec_get_roles_by_permissions(array($c_perm_id));
    $roles_id = array_unique(array_merge($roles_id, array_keys($c_roles_info)));
    $perms_ws_roles[$c_perm_id] = $c_roles_info;
  }
/* prepare report header */
  $header = array(array('data' => t('Users that can:')));
  foreach ($perms_ws_roles as $c_perm_id => $c_roles_info) {
    $header[]= array('class' => array('permission'), 'data' => $perms_info[$c_perm_id]['title']);
  }
  $rows = array();
/* check if anonymous role have permission */
  if (in_array(DRUPAL_ANONYMOUS_RID, $roles_id)) {
    $c_row = array(array('class' => array('user'), 'data' => t('anonymous users')));
    foreach ($perms_ws_roles as $c_perm_id => $c_roles_info) {
      $is_have_perm = isset($c_roles_info[DRUPAL_ANONYMOUS_RID]);
      $is_warning = $perms_info[$c_perm_id]['security_warning_for_anonymous'];
      $c_row_class = $is_have_perm && $is_warning ? array('permission', 'permission-warning') : array('permission');
      $c_row[]= array('class' => $c_row_class, 'data' => $is_have_perm ? ($is_warning ? '+ '.t('(warning)') : '+') : '');
    }
    $rows[]= $c_row;
  }
/* check if authenticated role have permission */
  if (in_array(DRUPAL_AUTHENTICATED_RID, $roles_id)) {
    $c_row = array(array('class' => array('user'), 'data' => t('authenticated users')));
    foreach ($perms_ws_roles as $c_perm_id => $c_roles_info) {
      $is_have_perm = isset($c_roles_info[DRUPAL_AUTHENTICATED_RID]);
      $is_warning = $perms_info[$c_perm_id]['security_warning_for_authenticated'];
      $c_row_class = $is_have_perm && $is_warning ? array('permission', 'permission-warning') : array('permission');
      $c_row[]= array('class' => $c_row_class, 'data' => $is_have_perm ? ($is_warning ? '+ '.t('(warning)') : '+') : '');
    }
    $rows[]= $c_row;
  }
/* check if other roles have permission */
  $users = _m2_sec_get_users_by_roles($roles_id);
  foreach ($users as $uid => $name) {
    $c_user_html = user_access('access user profiles') ? l($name, "user/$uid", array('attributes' => array('target' => '_blank'))) : $name;
    $c_row = array(array('class' => array('user'), 'data' => $c_user_html));
    $c_user_roles = _m2_sec_get_single_user_roles($uid);
    foreach ($perms_ws_roles as $c_perm_id => $c_roles_info) {
      $c_intersect_roles = array_intersect_key($c_roles_info, drupal_map_assoc($c_user_roles));
      $c_row[]= array('class' => array('permission'), 'data' => $c_intersect_roles ? theme('m2_title', array('type' => 'span', 'title' => implode(', ', $c_intersect_roles), 'markup' => '+')) : '');
    }
    $rows[]= $c_row;
  }  
/* output security report */
  $form['report'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No Users found.'),
    '#attributes' => array('class' => array('report-security')),
  );
  return $form;
}


/**
 * _m2_sec_get_roles_by_permissions | return array(N) or array()
 * _m2_sec_get_users_by_roles       | return array(N) or array()
 * _m2_sec_get_single_user_roles    | return array(N) or array()
 */

function _m2_sec_get_roles_by_permissions($perms_id) {
  if (is_array($perms_id) && count($perms_id)) {
    $query = db_select('role', 'r')->distinct();
    $query->join('role_permission', 'p', 'p.rid = r.rid');
    $query->fields('p', array('rid'))->fields('r', array('name'));
    $query->condition('p.permission', $perms_id, 'in');
    $query_result = $query->execute()->fetchAllKeyed();
    return $query_result ? $query_result : array();
  } else {
    return array();
  }
}


function _m2_sec_get_users_by_roles($roles_id) {
  if (is_array($roles_id) && count($roles_id)) {
    $query = db_select('users', 'u')->distinct();
    $query->join('users_roles', 'r', 'r.uid = u.uid');
    $query->fields('r', array('uid'))->fields('u', array('name'));
    $query->condition('r.rid', $roles_id, 'in');
    $query->orderBy('name');
    $query_result = $query->execute()->fetchAllKeyed();
    return $query_result ? $query_result : array();
  } else {
    return array();
  }
}


function _m2_sec_get_single_user_roles($uid) {
  $query = db_select('users_roles', 'r');
  $query->fields('r', array('rid'));
  $query->condition('uid', $uid);
  return $query->execute()->fetchCol('rid');
}

