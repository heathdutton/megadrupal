<?php
/**
 * @file
 * Functions related to allowing users to alter the user settings and options.
 */

/**
 * Form constructor for the user add API form.
 *
 * @see eve_api_user_add_api_form_validate()
 * @see eve_api_user_add_api_form_submit()
 *
 * @ingroup forms
 */
function eve_api_user_add_api_form($form, &$form_state, $account) {
  if (!isset($form_state['user'])) {
    $form_state['user'] = $account;
  }
  else {
    $account = $form_state['user'];
  }

  $form['add_api']['info'] = array(
    '#type' => 'item',
    '#title' => t('Add API Key'),
    '#description' => t('Please enter your EVE API in order to add it to your existing account.'),
    '#weight' => 0,
  );

  $allow_expires = variable_get('eve_api_require_expires', FALSE) ? t('You may set an expiry, but the preferred method is to tick the box "No Expiry".') : t('Please tick the box "No Expiry".');
  $allow_type = variable_get('eve_api_require_type', TRUE) ? t('Please set this to "All".') : t('You may select your main character or "All".');

  $form['add_api']['help_character'] = array(
    '#type' => 'item',
    '#title' => t('Character:'),
    '#description' => $allow_type,
    '#weight' => 10,
  );

  $form['add_api']['help_type'] = array(
    '#type' => 'item',
    '#title' => t('Type:'),
    '#description' => t('Please set this to "Character".'),
    '#weight' => 20,
  );

  $form['add_api']['help_expire'] = array(
    '#type' => 'item',
    '#title' => t('Expires:'),
    '#description' => $allow_expires,
    '#weight' => 30,
  );

  $form['add_api']['help_mask'] = array(
    '#type' => 'item',
    '#title' => t('Access Mask:'),
    '#description' => t('Click <a href="@mask" target="_blank">here</a> to create a new key with the correct access mask pre-selected.', array('@mask' => 'http://community.eveonline.com/support/api-key/CreatePredefined?accessMask=' . variable_get('eve_api_access_mask', 268435455))),
    '#weight' => 40,
  );

  $form['add_api']['keyID'] = array(
    '#type' => 'textfield',
    '#title' => t('Key ID'),
    '#description' => t('Please enter your Key ID.'),
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 15,
    '#weight' => 50,
  );

  $form['add_api']['vCode'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification Code'),
    '#description' => t('Please enter your Verification Code.'),
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 64,
    '#weight' => 60,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Add API Key'),
  );

  $form['#validate'] = array('eve_api_user_add_api_form_validate');
  $form['#submit'] = array('eve_api_user_add_api_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_user_add_api_form().
 *
 * @see eve_api_user_add_api_form_submit()
 */
function eve_api_user_add_api_form_validate($form, &$form_state) {
  $key_id = (int) $form_state['values']['keyID'];
  $v_code = (string) $form_state['values']['vCode'];

  if (empty($key_id) || empty($v_code) || preg_match('/[^a-z0-9]/i', $v_code) || preg_match('/[^0-9]/', $key_id) || strlen($key_id) > 15 || strlen($v_code) > 64 || strlen($v_code) < 20) {
    form_set_error('keyID', t('Invalid input, please try again.'));
    form_set_error('vCode');
    return;
  }

  $result = db_query('SELECT apiID FROM {eve_api_keys} WHERE keyID = :keyID AND vCode =:vCode', array(
    ':keyID' => $key_id,
    ':vCode' => $v_code,
  ));

  if ($result->rowCount()) {
    form_set_error('keyID', t('API Key already exists!'));
    form_set_error('vCode');
    return;
  }

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if (isset($characters['error'])) {
    form_set_error('keyID', t('There was an error with the API.'));
    form_set_error('vCode');
  }
  else {
    $whitelist = array();

    if (!empty($characters)) {
      foreach ($characters['characters'] as $character) {
        $whitelist[] = (int) $character['characterID'];
      }
    }

    $result = db_query('SELECT characterID FROM {eve_api_whitelist} WHERE characterID IN (:characterIDs)', array(
      ':characterIDs' => $whitelist,
    ));

    $allow_expires = variable_get('eve_api_require_expires', FALSE) ? FALSE : !empty($characters['expires']);
    $allow_type = variable_get('eve_api_require_type', TRUE) ? $characters['type'] != 'Account' : FALSE;

    if ($result->rowCount()) {
      if ($allow_expires || ($characters['accessMask'] & 8388680) != 8388680) {
        form_set_error('keyID', t('Your account has been whitelisted, please ensure that the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked. Only (Public Information -> (Characterinfo and FacWarStats), (Private Information) -> (CharacterSheet)) are required.'));
        form_set_error('vCode');
      }
    }
    else {
      if ($allow_expires || $allow_type || ($characters['accessMask'] & variable_get('eve_api_access_mask', 268435455)) != variable_get('eve_api_access_mask', 268435455)) {
        form_set_error('keyID', t('Please ensure that all boxes are highlighted and selected for the API, the "Character" drop down box is set to "All", the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked.'));
        form_set_error('vCode');
      }
    }

    if ($chars = eve_api_characters_exist($characters)) {
      form_set_error('keyID', t('Characters on this key have already been registered. Characters registered: @chars', array('@chars' => implode(", ", $chars))));
      form_set_error('vCode');
    }
  }
}

/**
 * Form submission handler for eve_api_user_add_api_form().
 *
 * @see eve_api_user_add_api_form_validate()
 */
function eve_api_user_add_api_form_submit($form, &$form_state) {
  $account = $form_state['user'];
  $uid = (int) $account->uid;
  $key_id = (int) $form_state['values']['keyID'];
  $v_code = (string) $form_state['values']['vCode'];
  $form_state['redirect'] = 'user/' . $uid . '/eve_api';

  $character = eve_api_create_key($account, $key_id, $v_code);

  $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
  $queue->createItem(array(
    'uid' => $uid,
    'runs' => 1,
  ));

  if ($character === FALSE && !isset($character['not_found'])) {
    drupal_set_message(t('There was an error with the API.'), 'error');
  }
  else {
    drupal_set_message(t('API Key successfully added!'));
  }
}

/**
 * Form constructor for the user List API form.
 *
 * @param string $account
 *   The account information for the user page being viewed.
 *
 * @see eve_api_user_list_api_form_validate()
 * @see eve_api_user_list_api_form_submit()
 *
 * @ingroup forms
 */
function eve_api_user_list_api_form($form, &$form_state, $account) {
  if (!isset($form_state['user'])) {
    $form_state['user'] = $account;
  }
  else {
    $account = $form_state['user'];
  }

  $uid = (int) $account->uid;
  $api_list = array();

  $header = array(
    'action' => array(
      'data' => t('Action'),
    ),
    'keyID' => array(
      'data' => t('API Key ID'),
      'field' => 'a.keyID',
    ),
    'characters' => array(
      'data' => t('Characters'),
    ),
    'errorCode' => array(
      'data' => t('Status'),
      'field' => 'a.errorCode',
    ),
  );

  $result_a = db_select('eve_api_keys', 'a')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('a', array(
      'apiID',
      'keyID',
      'vCode',
      'errorCode',
      'errorText',
      'updated',
      'created',
    ))
    ->limit(10)
    ->orderByHeader($header)
    ->condition('uid', $uid, '=')
    ->execute();

  if ($result_a->rowCount()) {
    foreach ($result_a->fetchAll() as $row_a) {
      $api_list[$row_a->apiID] = array(
        'apiID' => check_plain($row_a->apiID),
        'keyID' => check_plain($row_a->keyID),
        'vCode' => check_plain($row_a->vCode),
        'errorCode' => check_plain($row_a->errorCode),
        'errorText' => check_plain($row_a->errorText),
        'updated' => check_plain($row_a->updated),
        'created' => check_plain($row_a->created),
      );

      $result_b = db_query('SELECT characterID, characterName, corporationID, corporationName, corporationTicker, allianceID, allianceName, allianceTicker FROM {eve_api_characters} WHERE deleted = 0 AND uid = :uid AND apiID = :apiID', array(
        ':uid' => $uid,
        ':apiID' => (int) $row_a->apiID,
      ));

      if ($result_b->rowCount()) {
        foreach ($result_b->fetchAll() as $row_b) {
          $api_list[$row_a->apiID]['characters'][$row_b->characterID] = array(
            'characterID' => check_plain($row_b->characterID),
            'characterName' => check_plain($row_b->characterName),
            'corporationID' => check_plain($row_b->corporationID),
            'corporationName' => check_plain($row_b->corporationName),
            'corporationTicker' => check_plain($row_b->corporationTicker),
            'allianceID' => check_plain($row_b->allianceID),
            'allianceName' => check_plain($row_b->allianceName),
            'allianceTicker' => check_plain($row_b->allianceTicker),
          );
        }
      }
    }
  }

  $options = array();

  foreach ($api_list as $api_single) {
    $list_characters = '';

    if (isset($api_single['characters'])) {
      foreach ($api_single['characters'] as $characters) {
        $corp_alliance = ($characters['allianceID'] == 0) ? $characters['corporationName'] : $characters['corporationName'] . '/' . $characters['allianceName'];
        $list_characters .= '[' . $characters['corporationTicker'] . '] ' . $characters['characterName'] . ' (' . $corp_alliance . ')';
        $list_characters .= '<br />';
      }
    }

    $options[$api_single['apiID']] = array(
      'action' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Edit'),
          '#href' => 'user/' . $uid . '/eve_api/' . $api_single['apiID'],
        ),
      ),
      'keyID' => check_plain($api_single['keyID']),
      'characters' => $list_characters,
      'errorCode' => eve_api_api_error_msg($api_single['errorCode'], $api_single['errorText']),
    );
  }

  $form['list_api']['api_keys'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No API Keys associated with your account.'),
    '#weight' => 0,
  );

  $form['list_api']['page'] = array(
    '#theme' => 'pager',
    '#weight' => 10,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  $form['#validate'] = array('eve_api_user_list_api_form_validate');
  $form['#submit'] = array('eve_api_user_list_api_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_user_list_api_form().
 *
 * @see eve_api_user_list_api_form_submit()
 */
function eve_api_user_list_api_form_validate($form, &$form_state) {
  if (!is_array($form_state['values']['api_keys']) || !count(array_filter((array) $form_state['values']['api_keys']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Form submission handler for eve_api_user_list_api_form().
 *
 * @see eve_api_user_list_api_form_validate()
 */
function eve_api_user_list_api_form_submit($form, &$form_state) {
  $account = $form_state['user'];
  $uid = (int) $account->uid;
  $api_keys = array_filter((array) $form_state['values']['api_keys']);

  foreach ($api_keys as $api_id) {
    db_delete('eve_api_keys')->condition('apiID', (int) $api_id, '=')->execute();
    db_update('eve_api_characters')->fields(array('deleted' => 1))->condition('apiID', (int) $api_id, '=')->execute();
  }

  $result = db_query('SELECT c.characterName, c.characterID FROM {eve_api_characters} c INNER JOIN {users} u ON (c.characterName = u.name) WHERE c.deleted = 0 AND u.uid = :uid', array(
    ':uid' => $uid,
  ));

  if (!$result->rowCount()) {
    $result = db_query('SELECT corporationID, corporationName FROM {eve_api_alliance_list_corporations}');

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        if ($corporation_role = user_role_load_by_name($row->corporationName)) {
          user_multiple_role_edit(array($uid), 'remove_role', $corporation_role->rid);
        }
      }
    }

    $alliance_role = user_role_load(variable_get('eve_api_alliance_role', 2));
    user_multiple_role_edit(array($uid), 'remove_role', $alliance_role->rid);

    $blue_role = user_role_load(variable_get('eve_api_blue_role', 2));
    user_multiple_role_edit(array($uid), 'remove_role', $blue_role->rid);

    $unverified_role = user_role_load(variable_get('eve_api_unverified_role', 2));
    user_multiple_role_edit(array($uid), 'add_role', $unverified_role->rid);

    db_update('users')->fields(array(
      'characterID' => 0,
    ))->condition('uid', $uid, '=')->execute();

    $form_state['redirect'] = 'user/' . $uid . '/edit';

    drupal_set_message(t('Please select a new Main Character'));
  }

  $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
  $queue->createItem(array(
    'uid' => $uid,
    'runs' => 1,
  ));

  module_invoke_all('eve_api_user_delete', $account);
  module_invoke_all('eve_api_user_update', array(
    'account' => $account,
  ));

  drupal_set_message(t('API keys successfully deleted!'));
}

/**
 * Form constructor for the user update API form.
 *
 * @param string $account
 *   The account information for the user page being viewed.
 * @param string $api_id
 *   The unique apiID that is being updated.
 *
 * @see eve_api_user_update_api_form_validate()
 * @see eve_api_user_update_api_form_submit()
 *
 * @ingroup forms
 */
function eve_api_user_update_api_form($form, &$form_state, $account, $api_id) {
  if (!isset($form_state['user'])) {
    $form_state['user'] = $account;
  }
  else {
    $account = $form_state['user'];
  }

  $uid = (int) $account->uid;

  $result = db_query('SELECT keyID, vCode FROM {eve_api_keys} WHERE apiID = :apiID', array(
    ':apiID' => (int) $api_id,
  ));

  if (!$result->rowCount()) {
    drupal_goto('user/' . $uid . '/eve_api');
  }

  $row = $result->fetchAssoc();

  $form['update_api']['info'] = array(
    '#type' => 'item',
    '#title' => t('Update API'),
    '#description' => t('Please enter your EVE API in order to update it to your existing account.'),
    '#weight' => 0,
  );

  $allow_expires = variable_get('eve_api_require_expires', FALSE) ? t('You may set an expiry, but the preferred method is to tick the box "No Expiry".') : t('Please tick the box "No Expiry".');
  $allow_type = variable_get('eve_api_require_type', TRUE) ? t('Please set this to "All".') : t('You may select your main character or "All".');

  $form['update_api']['help_character'] = array(
    '#type' => 'item',
    '#title' => t('Character:'),
    '#description' => $allow_type,
    '#weight' => 10,
  );

  $form['update_api']['help_type'] = array(
    '#type' => 'item',
    '#title' => t('Type:'),
    '#description' => t('Please set this to "Character".'),
    '#weight' => 20,
  );

  $form['update_api']['help_expire'] = array(
    '#type' => 'item',
    '#title' => t('Expires:'),
    '#description' => $allow_expires,
    '#weight' => 30,
  );

  $form['update_api']['help_mask'] = array(
    '#type' => 'item',
    '#title' => t('Access Mask:'),
    '#description' => t('Click <a href="@mask" target="_blank">here</a> to create a new key with the correct access mask pre-selected.', array('@mask' => 'http://community.eveonline.com/support/api-key/CreatePredefined?accessMask=' . variable_get('eve_api_access_mask', 268435455))),
    '#weight' => 40,
  );

  $form['update_api']['keyID'] = array(
    '#type' => 'textfield',
    '#title' => t('Key ID'),
    '#description' => t('Please enter your Key ID.'),
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 15,
    '#default_value' => isset($form_state['values']['keyID']) ? (int) $form_state['values']['keyID'] : $row['keyID'],
    '#weight' => 50,
  );

  $form['update_api']['vCode'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification Code'),
    '#description' => t('Please enter your Verification Code.'),
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 64,
    '#default_value' => isset($form_state['values']['vCode']) ? (string) $form_state['values']['vCode'] : $row['vCode'],
    '#weight' => 60,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Update API Key'),
  );

  $form_state['apiID'] = $api_id;
  $form['#validate'] = array('eve_api_user_update_api_form_validate');
  $form['#submit'] = array('eve_api_user_update_api_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_user_update_api_form().
 *
 * @see eve_api_user_update_api_form_submit()
 */
function eve_api_user_update_api_form_validate($form, &$form_state) {
  $key_id = (int) $form_state['values']['keyID'];
  $v_code = (string) $form_state['values']['vCode'];
  $api_id = (int) $form_state['apiID'];

  if (empty($key_id) || empty($v_code) || preg_match('/[^a-z0-9]/i', $v_code) || preg_match('/[^0-9]/', $key_id) || strlen($key_id) > 15 || strlen($v_code) > 64 || strlen($v_code) < 20) {
    form_set_error('keyID', t('Invalid input, please try again.'));
    form_set_error('vCode');
    return;
  }

  $result = db_query('SELECT apiID FROM {eve_api_keys} WHERE keyID = :keyID AND vCode =:vCode AND apiID != :apiID', array(
    ':keyID' => $key_id,
    ':vCode' => $v_code,
    ':apiID' => $api_id,
  ));

  if ($result->rowCount()) {
    form_set_error('keyID', t('API Key already exists!'));
    form_set_error('vCode');
    return;
  }

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if (isset($characters['error'])) {
    form_set_error('keyID', t('There was an error with the API.'));
    form_set_error('vCode');
  }
  else {
    $whitelist = array();

    if (!empty($characters)) {
      foreach ($characters['characters'] as $character) {
        $whitelist[] = (int) $character['characterID'];
      }
    }

    $result = db_query('SELECT characterID FROM {eve_api_whitelist} WHERE characterID IN (:characterIDs)', array(
      ':characterIDs' => $whitelist,
    ));

    $allow_expires = variable_get('eve_api_require_expires', FALSE) ? FALSE : !empty($characters['expires']);
    $allow_type = variable_get('eve_api_require_type', TRUE) ? $characters['type'] != 'Account' : FALSE;

    if ($result->rowCount()) {
      if ($allow_expires || ($characters['accessMask'] & 8388680) != 8388680) {
        form_set_error('keyID', t('Your account has been whitelisted, please ensure that the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked. Only (Public Information -> (Characterinfo and FacWarStats), (Private Information) -> (CharacterSheet)) are required.'));
        form_set_error('vCode');
      }
    }
    else {
      if ($allow_expires || $allow_type || ($characters['accessMask'] & variable_get('eve_api_access_mask', 268435455)) != variable_get('eve_api_access_mask', 268435455)) {
        form_set_error('keyID', t('Please ensure that all boxes are highlighted and selected for the API, the "Character" drop down box is set to "All", the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked.'));
        form_set_error('vCode');
      }
    }
  }
}

/**
 * Form submission handler for eve_api_user_update_api_form().
 *
 * @see eve_api_user_update_api_form_validate()
 */
function eve_api_user_update_api_form_submit($form, &$form_state) {
  $account = $form_state['user'];
  $uid = (int) $account->uid;
  $form_state['redirect'] = 'user/' . $uid . '/eve_api';
  $key_id = (int) $form_state['values']['keyID'];
  $v_code = (string) $form_state['values']['vCode'];
  $api_id = (int) $form_state['apiID'];

  $characters = eve_api_modify_key($account, $key_id, $v_code, $api_id);

  $result = db_query('SELECT c.characterName, c.characterID FROM {eve_api_characters} c INNER JOIN {users} u ON (c.characterName = u.name) WHERE c.deleted = 0 AND u.uid = :uid', array(
    ':uid' => $uid,
  ));

  if (!$result->rowCount()) {
    module_invoke_all('eve_api_user_delete', $account);

    db_update('users')->fields(array(
      'characterID' => 0,
    ))->condition('uid', $uid, '=')->execute();

    $form_state['redirect'] = 'user/' . $uid . '/edit';

    drupal_set_message(t('Please select a new Main Character'));
  }

  $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
  $queue->createItem(array(
    'uid' => $uid,
    'runs' => 1,
  ));

  module_invoke_all('eve_api_user_update', array(
    'account' => $account,
  ));

  if ($characters == TRUE) {
    drupal_set_message(t('There was an error updateing the API Key!'), 'error');
  }
  else {
    drupal_set_message(t('API Key successfully updated!'));
  }
}
