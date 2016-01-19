<?php
/**
 * @file
 * Helper functions for odir_acl control module.
 */

/**
 * Page callback returning a form for editing permissions.
 */
function odir_acl_edit_form_callback($path, $roleid) {
  $rs = odir_acl_load_hirarchic($roleid, $path, FALSE);
  if ($rs->rowCount() == 0) {
    $r = new stdClass();
    $r->odir_path = $path;
    $r->group_id = $roleid;
    $r->allow = "";
    $r->deny = "";
  }
  else {
    $r = $rs->fetchObject();
  }

  odir_acl_rule_evaluation($r, $r->allow, $r->deny);
  return drupal_get_form('odir_acl_edit_form', $r);
}

/**
 * Page callback returning a string representing the users permissions.
 */
function odir_acl_ajax_get_permission_string($path, $role_id) {
  echo odir_acl_get_permission_string(array($role_id => $role_id), $path);
}

/**
 * Returns a string representing the users permissions.
 */
function odir_acl_get_permission_string($role_ids, $path, $roles = NULL) {
  $path = odir_decode($path);
  $access_rules = odir_acl_load_hirarchic($role_ids, $path, TRUE)->fetchAll();
  $perms = NULL;
  // The following row is needed when no rules defined.
  odir_acl_rule_evaluation($perms);
  foreach ($access_rules as $row) {
    odir_acl_rule_evaluation($perms, $row->allow, $row->deny);
  }
  if ($roles) {
    $role = $roles[$role_ids[0]];
  }
  else {
    $role = "";
  }
  $p = array(
    'allow' => odir_get_active_permissions($perms),
    'role' => $role);

  return theme('odir_acl_view_path_permissions_summary', $p);
}
