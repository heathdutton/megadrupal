<?php

/*
* Implementation of hook_rules_action_info()
*/

function referral_rules_action_info() {
  return array(
    'flag_referral' => array(
	  'label' => t('Flag referral'),
	  'group' => t('user referral'),
	  'base' => 'rules_action_flag_referral',
	  'parameter' => array(
	    'user' => array('type' => 'user', 'label' => t('The user whose referral record you want to flag')),
	  )
	),
	'unflag_referral' => array(
	  'label' => t('Unflag referral'),
	  'group' => t('user referral'),
	  'base' => 'rules_action_unflag_referral',
	  'parameter' => array(
	    'user' => array('type' => 'user', 'label' => t('The user whose referral record you want to unflag')),
	  )
	),
	'change_referral' => array(
	  'label' => t('Change referral for user'),
	  'group' => t('user referral'),
	  'base' => 'rules_action_change_referral',
	  'parameter' => array(
	    'target_user' => array('type' => 'user', 'label' => t('Target user'), 'description' => t('The user you want to change the referral record for')),
		'referring_user' => array('type' => 'user', 'label' => t('New referring user'), 'description' => t('The user you want to say referred the target user')),
	  )
	),
	'new_referral' => array(
	  'label' => t('Set new referral for user'),
	  'group' => t('user referral'),
	  'base' => 'rules_action_new_referral',
	  'parameter' => array(
	    'target_user' => array('type' => 'user', 'label' => t('Target user'), 'description' => t('The user you want to set the new referral record for')),
		'referring_user' => array('type' => 'user', 'label' => t('New referring user'), 'description' => t('The user you want to say referred the target user')),
		'host' => array('type' => 'text', 'label' => t('Host'), 'description' => t('The host you to say the new user came from'), 'optional' => TRUE),
		'uri' => array('type' => 'text', 'label' => t('URI'), 'description' => t('The URI you want to say the new user came from'), 'optional' => TRUE),
	  )
	),
  );
}

/*
* Implementation of hook_rules_condition_info()
*/

function referral_rules_condition_info() {
  return array(
    'referral_is_flagged' => array(
	  'label' => t('Referral is flagged'),
	  'group' => t('user referral'),
	  'base' => 'rules_condition_referral_is_flagged',
	  'parameter' => array(
	    'user' => array('type' => 'user', 'label' => t('The user you want to see if their referral record has been flagged')),
	  ),
	),
	'user_referred' => array(
	  'label' => t('User was referred by another user.'),
	  'group' => t('user referral'),
	  'base' => 'rules_condition_user_referred',
	  'parameter' => array(
	    'user' => array('type' => 'user', 'label' => t('The user you want to check if they were referred by another user')),
	  ),
	),
  );
}

/*
* Implementation of hook_rules_event_info()
*/

function referral_rules_event_info() {
  $defaults = array(
    'group' => t('user referral'),
    'module' => 'referral',
    'access callback' => 'user_referral_rules_access',
  );
  return array(
    'referral_recorded' => $defaults + array(
	  'label' => t('After a new referral is recorded'),
	  'variables' => array(
	    'referring_user' => array('type' => 'user', 'label' => t('The user who owned the referral link')),
		'new_user' => array('type' => 'user', 'label' => t('The new user who visted the site through the referral link')),
	  ),
	),  
  );
}

function rules_condition_referral_is_flagged($user) {
  $result = db_query_range('SELECT flag FROM {referral} WHERE uid = :uid', 0, 1, array('uid' => $user->uid))->fetchField();
  return $result;
}

function rules_condition_user_referred($user) {
  $result = db_query_range('SELECT referral_uid FROM {referral} WHERE uid = :uid', 0, 1, array('uid' => $user->uid))->fetchField();
  return $result;
}

function rules_action_flag_referral($user) {
  db_query('UPDATE {referral}
    SET flag = 1, flag_timestamp = :time
    WHERE uid = :ruid', array('time' => time(), 'ruid' => $user->uid));
}

function rules_action_unflag_referral($user) {
  db_query('UPDATE {referral}
    SET flag = 0, flag_timestamp = :time
    WHERE uid = :ruid', array('time' => 0, 'ruid' => $user->uid));
}

function rules_action_change_referral($target_user, $ref_user) {
  db_query('UPDATE {referral}
    SET referral_uid = :ref_uid, created = :time
    WHERE uid = :target_uid', array('ref_uid' => $ref_user->uid, 'target_uid' => $target_user->uid, 'time' => time()));
}

function rules_action_new_referral($target_user, $ref_user, $host = 'system', $uri = 'system') {
  $result = db_query_range('SELECT referral_uid FROM {referral} WHERE uid = :uid', 0, 1, array('uid' => $user->uid))->fetchField();
  if (!$result) {
    $query = db_insert('referral')
	  ->fields(array(
	    'uid' => $target_user->uid,
		'referral_uid' => $ref_user->uid,
		'created' => time(),
		'host' => $host,
		'http_referer' => $uri,
	  ))
	  ->execute();
	  
	if (!$query) {
      watchdog('rules', 'INSERT of referral data failed.', array(), WATCHDOG_ERROR);
      return FALSE;
    }
	
  } else {
    return FALSE;
  }
}

function user_referral_rules_access() {
  return user_access('administer referral');
}