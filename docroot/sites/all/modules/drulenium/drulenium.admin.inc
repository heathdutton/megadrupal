<?php

/**
 * @file drulenium_ctools_export_ui.inc
 * 
 * Settings form for global drulenium custimazation
 */
function drulenium_admin_settings_form($form, &$form_state) {
  $languages = language_list();

  $form = array();
  /*
  $form['drulenium_appid'] = array(
    '#title' => t('Base URL'), 
    '#type' => 'textfield', 
    '#description' => t('<p> Site Base URL if different from current site.</p>'), 
    '#default_value' => variable_get('drulenium_appid', '') 
  );
  */
  $form['drulenium_test_users'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test Users'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $user_types = user_roles(TRUE);
  foreach ($user_types as $rid => $role_name){
    $users = array();
    if($rid == 2){
      $result = db_query(
        'select u.uid, u.name from {users} u WHERE u.uid <> 0 AND u.status = 1 ORDER by u.name ASC');
    }else{
      $result = db_query(
        'select ur.uid, u.name from {users_roles} ur left join {users} u on u.uid = ur.uid WHERE ur.rid = :role_id AND u.status = 1 ORDER by u.name ASC',
        array(':role_id' => $rid)
      );
    }
    foreach ($result as $row) {
      $users[$row->uid] = $row->name;
    }
    $form['drulenium_test_users']['drulenium_role_'.$rid] = array(
       '#type' => 'select',
       '#title' => t($role_name),
       '#options' => $users,
       '#default_value' => variable_get('drulenium_role_'.$rid, ''),
      /* '#description' => print_r($value, TRUE), */
   );
  }

  $form = system_settings_form($form);
  return $form;
}

function _get_facebook_locale() {
  $fb_locale = array();
  $cache = cache_get('drulenium');
  if (empty($cache->data)) {
    $result = drupal_http_request("https://www.facebook.com/translations/FacebookLocales.xml");
    $xml = simplexml_load_string($result->data);
    foreach ( $xml->locale as $line ) {
      $representation = ( array ) $line->codes->code->standard->representation;
      $name = ( array ) $line->englishName;
      $key = $representation[0];
      $value = $name[0];
      $fb_locale[$key] = $value;
    }
    cache_set('drulenium', serialize($fb_locale), 'cache');
  }
  else {
    $fb_locale = unserialize($cache->data);
  }
  return $fb_locale;
}
