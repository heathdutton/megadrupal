<?php
/**
 * @file
 * Migration functions for the PM Permission module.
 */

define('PMPERMISSION_OBSOLETE', '0');
define('PMPERMISSION_UNCHANGED', '1');
define('PMPERMISSION_CANNOT_BE_PORTED', '2');

/**
 * Retrieves array of legacy to new permission mappings (keyed by sub-module).
 */
function pmpermission_migrate_get_legacy_permission_map() {
  $map = array(
    'pmexpense' => array(
      'Project Management expense: access'                                => PMPERMISSION_UNCHANGED,
      'Project Management expense: add'                                   => 'PM permission pmexpense: create',
      'Project Management expense: delete all'                            => 'PM permission pmexpense: delete all',
      'Project Management expense: delete of user organization'           => 'PM permission pmexpense: delete own Organization',
      'Project Management expense: delete own'                            => 'PM permission pmexpense: delete own',
      'Project Management expense: edit all'                              => 'PM permission pmexpense: update all',
      'Project Management expense: edit of user organization'             => 'PM permission pmexpense: update own Organization',
      'Project Management expense: edit own'                              => 'PM permission pmexpense: update own',
      'Project Management expense: view all'                              => 'PM permission pmexpense: view all',
      'Project Management expense: view of user organization'             => 'PM permission pmexpense: view own Organization',
      'Project Management expense: view own'                              => 'PM permission pmexpense: view own',
    ),
    'pminvoice' => array(
      'Project Management invoice: access'                                => PMPERMISSION_UNCHANGED,
      'Project Management invoice: add'                                   => 'PM permission pminvoice: create',
      'Project Management invoice: delete all'                            => 'PM permission pminvoice: delete all',
      'Project Management invoice: delete of user organization'           => 'PM permission pminvoice: delete own Organization',
      'Project Management invoice: delete own'                            => 'PM permission pminvoice: delete own',
      'Project Management invoice: edit all'                              => 'PM permission pminvoice: update all',
      'Project Management invoice: edit of user organization'             => 'PM permission pminvoice: update own Organization',
      'Project Management invoice: edit own'                              => 'PM permission pminvoice: update own',
      'Project Management invoice: view all'                              => 'PM permission pminvoice: view all',
      'Project Management invoice: view of user organization'             => 'PM permission pminvoice: view own Organization',
      'Project Management invoice: view own'                              => 'PM permission pminvoice: view own',
    ),
    'pmnote' => array(
      'Project Management note: access'                                   => PMPERMISSION_UNCHANGED,
      'Project Management note: add'                                      => 'PM permission pmnote: create',
      'Project Management note: delete all'                               => 'PM permission pmnote: delete all',
      'Project Management note: delete of user organization'              => 'PM permission pmnote: delete own Organization',
      'Project Management note: delete own'                               => 'PM permission pmnote: delete own',
      'Project Management note: edit all'                                 => 'PM permission pmnote: update all',
      'Project Management note: edit of user organization'                => 'PM permission pmnote: update own Organization',
      'Project Management note: edit own'                                 => 'PM permission pmnote: update own',
      'Project Management note: view all'                                 => 'PM permission pmnote: view all',
      'Project Management note: view of user organization'                => 'PM permission pmnote: view own Organization',
      'Project Management note: view own'                                 => 'PM permission pmnote: view own',
    ),
    'pmorganization' => array(
      'Project Management Organization: access'                           => PMPERMISSION_UNCHANGED,
      'Project Management Organization: add'                              => 'PM permission pmorganization: create',
      'Project Management Organization: delete all'                       => 'PM permission pmorganization: delete all',
      'Project Management Organization: delete own'                       => 'PM permission pmorganization: delete own',
      'Project Management Organization: edit all'                         => 'PM permission pmorganization: update all',
      'Project Management Organization: edit belonged'                    => 'PM permission pmorganization: update own Organization',
      'Project Management Organization: edit own'                         => 'PM permission pmorganization: update own',
      'Project Management Organization: view all'                         => 'PM permission pmorganization: view all',
      'Project Management Organization: view belonged'                    => 'PM permission pmorganization: view own Organization',
      'Project Management Organization: view own'                         => 'PM permission pmorganization: view own',
    ),
    'pmperson' => array(
      'Project Management Person: access'                                 => PMPERMISSION_UNCHANGED,
      'Project Management Person: add'                                    => PMPERMISSION_OBSOLETE,
      'Project Management Person: delete all'                             => PMPERMISSION_OBSOLETE,
      'Project Management Person: delete of user organization'            => PMPERMISSION_OBSOLETE,
      'Project Management Person: delete own'                             => PMPERMISSION_OBSOLETE,
      'Project Management Person: delete when linked to own user account' => PMPERMISSION_OBSOLETE,
      'Project Management Person: edit all'                               => PMPERMISSION_OBSOLETE,
      'Project Management Person: edit of user organization'              => PMPERMISSION_OBSOLETE,
      'Project Management Person: edit own'                               => PMPERMISSION_OBSOLETE,
      'Project Management Person: edit when linked to own user account'   => PMPERMISSION_OBSOLETE,
      'Project Management Person: view all'                               => PMPERMISSION_OBSOLETE,
      'Project Management Person: view of user organization'              => PMPERMISSION_OBSOLETE,
      'Project Management Person: view own'                               => PMPERMISSION_OBSOLETE,
      'Project Management Person: view when linked to own user account'   => PMPERMISSION_OBSOLETE,
    ),
    'pmproject' => array(
      'Project Management Project: access'                                => PMPERMISSION_UNCHANGED,
      'Project Management Project: add'                                   => 'PM permission pmproject: create',
      'Project Management Project: delete all'                            => 'PM permission pmproject: delete all',
      'Project Management Project: delete if assigned to project'         => 'PM permission pmproject: delete if Assigned',
      'Project Management Project: delete if project manager'             => 'PM permission pmproject: delete if Project Manager',
      'Project Management Project: delete of user organization'           => 'PM permission pmproject: delete own Organization',
      'Project Management Project: delete own'                            => 'PM permission pmproject: delete own',
      'Project Management Project: edit all'                              => 'PM permission pmproject: update all',
      'Project Management Project: edit if assigned to project'           => 'PM permission pmproject: update if Assigned',
      'Project Management Project: edit if project manager'               => 'PM permission pmproject: update if Project Manager',
      'Project Management Project: edit of user organization'             => 'PM permission pmproject: update own Organization',
      'Project Management Project: edit own'                              => 'PM permission pmproject: update own',
      'Project Management Project: view all'                              => 'PM permission pmproject: view all',
      'Project Management Project: view if assigned to project'           => 'PM permission pmproject: view if Assigned',
      'Project Management Project: view if project manager'               => 'PM permission pmproject: view if Project Manager',
      'Project Management Project: view of user organization'             => 'PM permission pmproject: view own Organization',
      'Project Management Project: view own'                              => 'PM permission pmproject: view own',
    ),
    'pmtask' => array(
      'Project Management Task: access'                                   => PMPERMISSION_UNCHANGED,
      'Project Management Task: add'                                      => 'PM permission pmtask: create',
      'Project Management Task: delete all'                               => 'PM permission pmtask: delete all',
      'Project Management Task: delete if assigned to task'               => 'PM permission pmtask: delete if Assigned',
      'Project Management Task: delete of user organization'              => 'PM permission pmtask: delete own Organization',
      'Project Management Task: delete own'                               => 'PM permission pmtask: delete own',
      'Project Management Task: edit all'                                 => 'PM permission pmtask: update all',
      'Project Management Task: edit if assigned to task'                 => 'PM permission pmtask: update if Assigned',
      'Project Management Task: edit own'                                 => 'PM permission pmtask: update own',
      'Project Management Task: view all'                                 => 'PM permission pmtask: view all',
      'Project Management Task: view if assigned to task'                 => 'PM permission pmtask: view if Assigned',
      'Project Management Task: view of user organization'                => 'PM permission pmtask: view own Organization',
      'Project Management Task: view own'                                 => 'PM permission pmtask: view own',
    ),
    'pmteam' => array(
      'Project Management Team: access'                                   => PMPERMISSION_UNCHANGED,
      'Project Management Team: add'                                      => 'PM permission pmteam: create',
      'Project Management Team: delete all'                               => 'PM permission pmteam: delete all',
      'Project Management Team: delete belonged'                          => 'PM permission pmteam: delete belonged',
      'Project Management Team: delete own'                               => 'PM permission pmteam: delete own',
      'Project Management Team: edit all'                                 => 'PM permission pmteam: update all',
      'Project Management Team: edit belonged'                            => 'PM permission pmteam: edit belonged',
      'Project Management Team: edit own'                                 => 'PM permission pmteam: update own',
      'Project Management Team: view all'                                 => 'PM permission pmteam: view all',
      'Project Management Team: view belonged'                            => 'PM permission pmteam: view belonged',
      'Project Management Team: view own'                                 => 'PM permission pmteam: view own',
    ),
    'pmticket' => array(
      'Project Management Ticket: access'                                 => PMPERMISSION_UNCHANGED,
      'Project Management Ticket: add'                                    => 'PM permission pmticket: create',
      'Project Management Ticket: delete all'                             => 'PM permission pmticket: delete all',
      'Project Management Ticket: delete if assigned to ticket'           => 'PM permission pmticket: delete if Assigned',
      'Project Management Ticket: delete of user organization'            => 'PM permission pmticket: delete own Organization',
      'Project Management Ticket: delete own'                             => 'PM permission pmticket: delete own',
      'Project Management Ticket: edit all'                               => 'PM permission pmticket: update all',
      'Project Management Ticket: edit if assigned to ticket'             => 'PM permission pmticket: edit if Assigned',
      'Project Management Ticket: edit of user organization'              => 'PM permission pmticket: update own Organization',
      'Project Management Ticket: edit own'                               => 'PM permission pmticket: update own',
      'Project Management Ticket: view all'                               => 'PM permission pmticket: view all',
      'Project Management Ticket: view if assigned to ticket'             => 'PM permission pmticket: view if Assigned',
      'Project Management Ticket: view of user organization'              => 'PM permission pmticket: view own Organization',
      'Project Management Ticket: view own'                               => 'PM permission pmticket: view own',
    ),
    'pmtimetracking' => array(
      'Project Management Timetracking: access'                           => PMPERMISSION_UNCHANGED,
      'Project Management Timetracking: add'                              => 'PM permission pmtimetracking: create',
      'Project Management Timetracking: delete all'                       => 'PM permission pmtimetracking: delete all',
      'Project Management Timetracking: delete of user organization'      => 'PM permission pmtimetracking: delete own Organization',
      'Project Management Timetracking: delete own'                       => 'PM permission pmtimetracking: delete own',
      'Project Management Timetracking: edit all'                         => 'PM permission pmtimetracking: update all',
      'Project Management Timetracking: edit of user organization'        => 'PM permission pmtimetracking: update own Organization',
      'Project Management Timetracking: edit own'                         => 'PM permission pmtimetracking: update own',
      'Project Management Timetracking: view all'                         => 'PM permission pmtimetracking: view all',
      'Project Management Timetracking: view of user organization'        => 'PM permission pmtimetracking: view own Organization',
      'Project Management Timetracking: view own'                         => 'PM permission pmtimetracking: view own',
    ),
    'pm' => array(
      'Project Management: access administration pages'                   => PMPERMISSION_UNCHANGED,
      'Project Management: access dashboard'                              => PMPERMISSION_UNCHANGED,
    ),
  );
  return $map;
}

/**
 * Helper function to migrate permission defined by submodules.
 */
function pmpermission_migrate_execute($module_name) {
  // Enable Permission to be defined by pmpermission.
  pmpermission_migrate_enable_permissions($module_name);
  // Handover legacy permission to pmpermission based implementations.
  pmpermission_migrate_legacy_permissions($module_name);
}

/**
 * Sets site variables to tell pmpermission to handle submodule's permissions.
 */
function pmpermission_migrate_enable_permissions($type) {
  switch ($type) {
    case 'pmteam':
    case 'pmorganization':
    case 'pmproject':
    case 'pmtask':
    case 'pmticket':
    case 'pmtimetracking':
    case 'pmnote':
    case 'pminvoice':
    case 'pmexpense':
      variable_set("pmpermission_node_{$type}_enabled", TRUE);
      break;

    default:
      break;
  }
}

/**
 * Handles migration of legacy permissions for a given sub-module.
 */
function pmpermission_migrate_legacy_permissions($module_name) {
  // Enable Permission to be defined by pmpermission.
  // Handover legacy permission to pmpermission based implementations.
  $map = pmpermission_migrate_get_legacy_permission_map();
  $permissions = $map[$module_name];
  foreach ($permissions as $old => $new) {
    switch ($new) {
      case PMPERMISSION_OBSOLETE:
        drupal_set_message(t('Permission "@name" is obsolete and won\'t be migrated.', array('@name' => $old)), 'warning');
        break;

      case PMPERMISSION_CANNOT_BE_PORTED:
        drupal_set_message(t('Permission "@name" cannot be ported.', array('@name' => $old)), 'error');
        break;

      case PMPERMISSION_UNCHANGED:
        break;

      default:
        $rids = _pmpermission_migrate_get_all_roles_having_permission($old);
        $permission = array(
          // Revoke old permission (deletes the record) by setting to 0.
          $old => 0,
          $new => TRUE,
        );
        foreach ($rids as $rid => $role_name) {
          user_role_change_permissions($rid, $permission);
        }
        // All permission adjustment used by pmpermission module requires node
        // access to be rebuilt.
        node_access_needs_rebuild(TRUE);
        break;
    }
  }
}

/**
 * Gets a list of all roles which have been given a particular permission.
 */
function _pmpermission_migrate_get_all_roles_having_permission($name) {
  $rids = array();
  $roles = user_roles();
  $permissions = user_role_permissions($roles);
  foreach ($roles as $rid => $role_name) {
    if (isset($permissions[$rid][$name])) {
      $rids[$rid] = $role_name;
    }
  }
  return $rids;
}
