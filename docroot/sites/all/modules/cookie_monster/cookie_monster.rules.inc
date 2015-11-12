<?php
/**
 * @file
 * Rules definitions for cookie monster.
 */


function cookie_monster_rules_event_info() {
  $events['cookie_monster_rules_cookie_set'] = array(
    'label' => t('Cookie Monster Cookie Set'),
    'group' => 'Cookie Monster',
    'help' => t('Event fires after a cookie monster cookie has been set'),
    'variables' => array(
      'cookie_id' => array(
        'label' => 'Cookie ID',
        'type' => 'text',
        'description' => t('The ID of the cookie'),
      ),
      'cookie_data' => array(
        'label' => 'Cookie Data',
        'type' => 'text',
        'description' => t('The data in the cookie'),
        'handler' => 'cookie_monster_rules_load_data',
      ),
    ),
  );
  $events['cookie_monster_rules_cookie_expired'] = array(
    'label' => t('Cookie Monster Cookie Expire'),
    'group' => 'Cookie Monster',
    'help' => t('Event fires after a cookie monster cookie has been expired by the server'),
    'variables' => array(
      'cookie_id' => array(
        'label' => 'Cookie ID',
        'type' => 'text',
        'description' => t('The ID of the cookie'),
      ),
    ),
  );
  return $events;
}

function cookie_monster_rules_condition_info() {
  $conditions['cookie_monster_rules_cookie_exists'] = array(
    'label' => t('Check if a cookie exists'),
    'parameter' => array(
      'cookie_id' => array(
        'label' => t('Cookie ID'),
        'description' => t('The ID of the cookie to look for'),
        'type' => 'text',
      ),
    ),
    'group' => 'Cookie Monster',
  );
  return $conditions;
}


function cookie_monster_rules_action_info() {
  $actions['cookie_monster_rules_load_cookie_data'] = array(
    'label' => t('Load a cookie monster cookie data'),
    'group' => 'Cookie Monster',
    'parameter' => array(
      'cookie_id' => array(
        'label' => t('Cookie ID'),
        'description' => t('The ID of the cookie to load'),
        'type' => 'text',
      ),
    ),
    'provides' => array(
      'cookie_data' => array(
        'label' => t('Cookie Data'),
        'type' => 'text',
      ),
    ),
  );
  $actions['cookie_monster_rules_set_cookie'] = array(
    'label' => t('Set a cookie monster cookie'),
    'group' => 'Cookie Monster',
    'parameter' => array(
  	  'cookie_id' => array(
        'label' => t('Cookie ID'),
        'description' => t('The ID of the cookie to set'),
        'type' => 'text',
      ),
      'cookie_data' => array(
        'label' => t('Data'),
        'description' => t('Data to set in the cookie.'),
        'type' => 'text',
      ),
    ),
  );
  $actions['cookie_monster_rules_expire_cookie'] = array(
    'label' => t('Expire a cookie monster cookie'),
    'group' => 'Cookie Monster',
    'parameter' => array(
  	  'cookie_id' => array(
        'label' => t('Cookie ID'),
        'description' => t('The ID of the cookie to expire'),
        'type' => 'text',
      ),
    ),
  );
  return $actions;
}

function cookie_monster_rules_load_data($arguments) {
  return cookie_monster_rules_load_cookie_data($arguments['cookie_id']);
}

function cookie_monster_rules_cookie_exists($cookie_id) {
  return cookie_monster_cookie_exists($cookie_id);
}


function cookie_monster_rules_load_cookie_data($cookie_id) {
  if (!$data = cookie_monster_get_cookie($cookie_id)) {
    return array();
  }
  return array('cookie_data' => $data['data']);
}


function cookie_monster_rules_set_cookie($cookie_id, $cookie_data) {
  $data['id'] = $cookie_id;
  $data['data'] = $cookie_data;
  if (!cookie_monster_set_cookie($data)) {
    watchdog('Cookie Monster Rules', t('Could not set a cookie'), 'error');
    return FALSE;
  }
}


function cookie_monster_rules_expire_cookie($cookie_id) {
  if (!cookie_monster_expire_cookie($cookie_id)) {
    watchdog('Cookie Monster Rules', t('Could not expire a cookie in rules'));
    return FALSE;
  }
}