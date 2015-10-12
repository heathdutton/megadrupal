<?php

/**
 * Checks entity access for various operations.
 *
 * @param string $op
 *   The operation being performed. One of 'view', 'update', 'create', 'delete'
 *   or 'archive'.
 * @param entity $entity
 *   entity to check access for, or for the create operation, the entity bundle.
 *   If nothing is given access permissions for all entities of given type are
 *   returned.
 */
function _entityspice_entity_access($etype, $op, $entity = NULL, $user = NULL) {
  $account = $user ? $user : $GLOBALS['user'];
  $self = isset($entity->uid) ? ($entity->uid == $account->uid) : FALSE;

  if (($op == 'edit') && user_access("edit own $etype", $account) && $self) {
    return TRUE;
  }

  if (($op == 'view') && user_access("view own $etype", $account) && $self) {
    return TRUE;
  }

  if (user_access("administer $etype", $account)) {
    return TRUE;
  }

  if ($op == 'view' && user_access("access $etype", $account)) {
    return TRUE;
  }

  if (($op == 'edit' || $op == 'archive') && user_access("manage $etype", $account)) {
    return TRUE;
  }

  if (($op == 'create' || $op == 'delete') && user_access("manage $etype", $account)) {
    return TRUE;
  }

  if (isset($entity) && is_string($entity) && $op == 'create' && user_access("create $entity $etype", $account)) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Access callback for the entity API.
 */
function _entityspice_entity_bundle_access($etype, $op, $bundle = NULL, $account = NULL) {
  return user_access("administer $etype bundles", $account);
}

/**
 * Define permissions useful for entities.
 *
 * @param string $type
 *   Entity type (machine name).
 * @param unknown $label
 *   Entity label (human redable).
 * @return array
 *   Array of permissions related to entities.
 */
function _entityspice_entity_permissions($type, $label) {
  $perms = array(
    "administer $type bundles" => array(
      'title' => t("Administer $label Bundles"),
      'description' => t("Manage $label bundles and their structure."),
    ),
    "administer $type" => array(
      'title' => t("Administer $type"),
      'description' => t("Perform administration tasks for $label."),
    ),
    "manage $type" => array(
      'title' => t("Manage $label"),
      'description' => t("Create, update or delete $label."),
    ),
    "access $type" => array(
      'title' => t("Access $label"),
      'description' => t("View $label."),
    ),
    "view own $type" => array(
      'title' => t("Access Own $label"),
      'description' => t("View own $label information."),
    ),
    "edit own $type" => array(
      'title' => t("Edit Own $label"),
      'description' => t("Edit own $label information."),
    ),
  );

  return $perms;
}

