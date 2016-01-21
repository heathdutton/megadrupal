<?php
// $Id: userpoints_nodeaccess.rules.inc,v 1.1 2011/01/10 19:40:22 berdir Exp $

/**
 * @file
 * Rules integration for userpoints_nodeaccess.module
 */

/**
 * Implements hook_rules_action_info().
 */
function userpoints_nodeaccess_rules_action_info() {
  return array(
    'userpoints_nodeaccess_grant_access' => array(
      'label' => t('Grant access to a node'),
      'named parameter' => TRUE,
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user that will gain access.'),
        ),
        'nid' => array(
          'type' => 'integer',
          'label' => t('Node ID'),
          'description' => t('Define to which node access shall be granted.'),
        ),
      ),
      'group' => t('!Points', userpoints_translation()),
    ),
    'userpoints_nodeaccess_revoke_access' => array(
      'label' => t('Revoke access to a node'),
      'named parameter' => TRUE,
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user that will have access revoked.'),
        ),
        'nid' => array(
          'type' => 'integer',
          'label' => t('Node ID'),
          'description' => t('Define to which node access shall be revoked.'),
        ),
      ),
      'group' => t('!Points', userpoints_translation()),
    ),
  );
}

/**
 * Rules action; grant a user access to a node without using points.
 */
function userpoints_nodeaccess_grant_access($params) {
  db_merge('userpoints_nodeaccess')
  ->key(array(
    'nid' => $params['nid'],
    'uid' => is_object($params['user']->uid) ? $params['user']->getIdentifier() : $params['user']->uid,
  ))
  ->fields(array(
    'timestamp' => REQUEST_TIME,
  ))
  ->execute();
}

/**
 * Rules action; Revoke access of a user to a node without using points.
 */
function userpoints_nodeaccess_revoke_access($params) {
  db_delete('userpoints_nodeaccess')
    ->condition('nid', $params['nid'])
    ->condition('uid', is_object($params['user']->uid) ? $params['user']->getIdentifier() : $params['user']->uid)
    ->execute();
}
