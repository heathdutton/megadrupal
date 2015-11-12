<?php

/**
 * @file
 * Relying party functions
 */


/**
 * Implements hook_entity_info().
 */
function openid_sso_provider_entity_info() {
  $return = array(
    'openid_sso_provider_rps' => array(
      'label' => t('Relying party'),
      'entity class' => 'OpenidSsoProviderRps',
      'controller class' => 'OpenidSsoProviderRpsController',
      'base table' => 'openid_sso_provider_rps',
      'fieldable' => TRUE,
      'exportable' => TRUE,
      'entity keys' => array(
        'id' => 'rpsid',
        'name' => 'machine_name',
        'label' => 'label',
        'trusted' => 'trusted',
        'enabled' => 'enabled',
      ),
      'bundles' => array(
        'openid_sso_provider_rps' => array(
          'label' => t('Relying parties bundle'),
          'admin' => array(
            'path' => 'admin/structure/relying-parties/manage',
            'access arguments' => array('administer openid_sso_provider_rps')
          ),
        ),
      ),
      'load hook' => 'openid_sso_provider_rps_load',
      'view modes' => array(
        'full' => array(
          'label' => t('Default'),
          'custom settings' => FALSE,
        ),
      ),
      'uri callback' => 'entity_class_uri',
      'module' => 'openid_sso_provider',
      'admin ui' => array(
        'path' => 'admin/structure/relying-parties',
        'file' => 'openid_sso_provider.admin.inc',
        'controller class' => 'OpenidSsoProviderRpsUIController',
      ),
      'access callback' => 'openid_sso_provider_rps_access',
    ),
  );
  return $return;
}

/**
 * Access callback for entity.
 */
function openid_sso_provider_rps_access($op, $entity = NULL) {
  return user_access('administer openid_sso_provider_rps');
}

/**
 * Load relying parties by rpsid.
 * @todo: use openid_sso_provider_get_relying_parties() instead.
 */
function openid_sso_provider_rps_load($rpsid, $reset = FALSE) {
  $items = openid_sso_provider_rps_load_multiple(array($rpsid), array(), $reset);
  field_attach_load('openid_sso_provider_rps', $items);
  return reset($items);
}

/**
 * Load one (unique) enabled relying party by realm
 */
function openid_sso_provider_rps_load_by_realm($realm, $reset = FALSE) {
  $query = new EntityFieldQuery();
  $entity_type = 'openid_sso_provider_rps';
  $query->entityCondition('entity_type', $entity_type)
    ->propertyCondition('realm', $realm)
    // only retrieve enabled relying parties
    ->propertyCondition('enabled', 1);
  $result = $query->execute();
  if (count($result)) {
    $rpsid = key($result[$entity_type]);
    $rp = openid_sso_provider_rps_load($rpsid, $reset);
    return $rp;
  }
  return FALSE;
}

/**
 * Load multiple relying parties based on certain conditions.
 */
function openid_sso_provider_rps_load_multiple($rpsids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('openid_sso_provider_rps', $rpsids, $conditions, $reset);
}

/**
 * Lists relying parties
 * @todo: use properly EntityFieldQuery
 */
function openid_sso_provider_get_relying_parties($type_name = 'all', $selection = array()) {
  $types = entity_load_multiple_by_name('openid_sso_provider_rps', ($type_name != 'all') ? array($type_name) : FALSE);

  if (isset($selection['enabled'])) {

    foreach ($types as $key => $type) {

      // Exclude disabled relying parties
      if ($selection['enabled'] == TRUE && $type->enabled != 1) {
        unset($types[$key]);
      }

      // Exclude enabled relying parties
      elseif ($selection['enabled'] == FALSE && $type->enabled == 1) {
        unset($types[$key]);
      }
    }
  }

  if (isset($selection['trusted'])) {

    foreach ($types as $key => $type) {

      // Exclude untrusted relying parties
      if ($selection['trusted'] == TRUE && $type->trusted != 1) {
        unset($types[$key]);
      }

      // Exclude trusted relying parties
      elseif ($selection['trusted'] == FALSE && $type->trusted == 1) {
        unset($types[$key]);
      }
    }
  }

  return ($type_name != 'all') ? reset($types) : $types;
}


/**
 * Save relying party.
 */
function openid_sso_provider_rps_save($entity) {
  entity_save('openid_sso_provider_rps', $entity);
}

/**
 * Delete single.
 */
function openid_sso_provider_rps_delete($entity) {
  entity_delete('openid_sso_provider_rps', entity_id('openid_sso_provider_rps' ,$entity));
}

/**
 * Delete multiple.
 */
function openid_sso_provider_rps_delete_multiple($entity_ids) {
  entity_delete_multiple('openid_sso_provider_rps', $entity_ids);
}

