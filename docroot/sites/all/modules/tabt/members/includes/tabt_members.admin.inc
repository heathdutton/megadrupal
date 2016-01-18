<?php
/**
 * @file
 * The admin pages for the TabT Members-module.
 */

/**
 * Make an admin-page to set some settings for the this module.
 */
function tabt_members_admin() {
  $form['tabt_note'] = array(
    '#markup' => '<strong>Note:</strong> You have to clear all caches after changing these settings.',
  );
  $form['tabt_members_memberspage'] = array(
    '#type' => 'textfield',
    '#title' => t('TabT Memberspage link'),
    '#default_value' => variable_get('tabt_members_memberspage', 'members'),
    '#size' => 20,
    '#description' => t("The path to the memberslisting and playerprofiles.<br />PlayerID's will be appended for playerprofiles."),
    '#required' => TRUE,
    '#element_validate' => array('_tabt_validate_path'),
  );
  $form['tabt_members_memberspage_title'] = array(
    '#type' => 'textfield',
    '#title' => t('TabT Memberspage title'),
    '#default_value' => variable_get('tabt_members_memberspage_title', 'Members'),
    '#size' => 20,
    '#description' => t('The pagetitle of the memberslisting.'),
    '#required' => TRUE,
  );
  $form['tabt_members_profile_idfield'] = array(
    '#type' => 'textfield',
    '#title' => t('ID profile field'),
    '#default_value' => variable_get('tabt_members_profile_idfield'),
    '#field_prefix' => 'field_',
    '#size' => 10,
    '#description' => t('The name (not the label) of the profile field containing the player-ID.'),
  );
  $form['tabt_members_profile_firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name profile field'),
    '#default_value' => variable_get('tabt_members_profile_firstname'),
    '#field_prefix' => 'field_',
    '#size' => 10,
    '#description' => t('The name (not the label) of the profile field containing the players first name.'),
  );

  $form['tabt_members_profile_lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name profile field'),
    '#default_value' => variable_get('tabt_members_profile_lastname'),
    '#field_prefix' => 'field_',
    '#size' => 10,
    '#description' => t('The name (not the label) of the profile field containing the players last name.'),
  );

  $form['tabt_members_picpage_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('NodeId profilepictures page'),
    '#default_value' => variable_get('tabt_members_picpage_nid'),
    '#size' => 10,
    '#description' => t('The nodeid for the page where you can upload profile pictures for members.'),
  );

  $form['tabt_members_get_results'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load individual results'),
    '#default_value' => variable_get('tabt_members_get_results'),
    '#size' => 10,
    '#description' => t('Choose whether or not to load additional individual results from the TabT-site.<br />Note that his can increase loadtimes.'),
  );
  return system_settings_form($form);
}

/**
 * Make an admin-page where the memberlist can be slightly altered.
 */
function tabt_members_admin_members() {
  $members = tabt_members_getmembers();
  $rows = array();
  foreach ($members as $member) {
    if ($member->UserID != NULL) {
      $user = user_load($member->UserID);
      $row = array();
      $row['user'] = array(
        '#markup' => l($user->name, 'user/' . $user->uid),
      );
      $row['playerid'] = array(
        '#markup' => $member->UniqueIndex,
      );
      $row['tabtname'] = array(
        '#markup' => $member->FirstName . ' ' . $member->LastName,
      );
      $row['name'] = array(
        '#type' => 'textfield',
        '#size' => '30',
        '#disabled' => TRUE,
        '#default_value' => $member->Name,
      );
      $row['profilepic'] = array(
        '#type' => 'textfield',
        '#size' => '40',
        '#default_value' => $member->ProfilePic,
      );
      $row['operations'] = array(
        '#markup' => l(t('Edit'), 'user/' . $user->uid . '/edit'),
      );
      $rows[$member->UniqueIndex] = $row;
    }
  }
  if (count($rows) > 0) {
    $form['tabt_members_registered'] = array(
      '#type' => 'fieldset',
      '#title' => t('Registered Users'),
      '#description' => t('The following members have registered on this site. Their name will be overriden by the value in their profile, but you can enter a picture for when they have not uploaded one.<br />For profile pictures, you can upload the picture at !profilepage and enter the (local) path (for example, <em>/sites/all/files/members/picture.png</em>),<br />or you can enter the link to a picture on another site (not recommended).', array('!profilepage' => l(t('Picture page'), 'node/' . variable_get('tabt_members_picpage_nid')))),
    );
    $form['tabt_members_registered']['tabt_members_regoverview'] = array(
      '#theme' => 'tabt_members_admin_members_memberlist',
      '#tree' => TRUE,
    );

    $form['tabt_members_registered']['tabt_members_regoverview']['tabt_members_members'] = array();

    $form['tabt_members_registered']['tabt_members_regoverview']['tabt_members_members'] = $rows;
  }

  $form['tabt_members_unregistered'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unregistered Users'),
    '#description' => t('The following members have not registered on this site. You can change their displayed name or their profile picture.<br />For profile pictures, you can upload the picture at !profilepage and enter the (local) path (for example, <em>/sites/all/files/members/picture.png</em>),<br />or you can enter the link to a picture on another site (not recommended).', array('!profilepage' => l(t('Picture page'), 'node/' . variable_get('tabt_members_picpage_nid')))),
  );
  $form['tabt_members_unregistered']['tabt_members_unregoverview'] = array(
    '#theme' => 'tabt_members_admin_members_memberlist_unregistered',
    '#tree' => TRUE,
  );

  $form['tabt_members_unregistered']['tabt_members_unregoverview']['tabt_members_members'] = array();
  foreach ($members as $member) {
    if ($member->UserID == NULL) {
      $row = array();
      $row['user'] = array();
      $row['playerid'] = array(
        '#markup' => $member->UniqueIndex,
      );
      $row['tabtname'] = array(
        '#markup' => $member->FirstName . ' ' . $member->LastName,
      );
      $row['name'] = array(
        '#type' => 'textfield',
        '#size' => '30',
        '#default_value' => $member->Name,
      );
      $row['profilepic'] = array(
        '#type' => 'textfield',
        '#size' => '40',
        '#default_value' => $member->ProfilePic,
      );
      $row['operations'] = array();
      $form['tabt_members_unregistered']['tabt_members_unregoverview']['tabt_members_members'][$member->UniqueIndex] = $row;
    }
  }

  $result = db_select('tabt_members', 't')
  ->fields('t')
  ->execute()
  ->fetchAll();

  if (count($result) > 0) {

    $members = tabt_members_get_members_by_id();

    $rows = array();
    foreach ($result as $member) {
      if (!isset($members[$member->id])) {
        $row = array();
        $row['user'] = array();
        $row['playerid'] = array(
          '#markup' => $member->id,
        );
        $row['tabtname'] = array(
          '#markup' => t('N/A'),
        );
        $row['name'] = array(
          '#type' => 'textfield',
          '#size' => '30',
          '#default_value' => $member->name,
        );
        $row['profilepic'] = array(
          '#type' => 'textfield',
          '#size' => '40',
          '#default_value' => $member->pic,
        );
        $row['operations'] = array();
        $rows[$member->id] = $row;
      }
    }
    if (count($rows) > 0) {
      $form['tabt_nonmembers_unregistered'] = array(
        '#type' => 'fieldset',
        '#title' => t('Unregistered Users'),
        '#description' => t('The following players are not members, but have a name and/or profilepicture stored. You can change their displayed name or their profile picture.<br />For profile pictures, you can upload the picture at !profilepage and enter the (local) path (for example, <em>/sites/all/files/members/picture.png</em>),<br />or you can enter the link to a picture on another site (not recommended).', array('!profilepage' => l(t('Picture page'), 'node/' . variable_get('tabt_members_picpage_nid')))),
      );
      $form['tabt_nonmembers_unregistered']['tabt_nonmembers_unregoverview'] = array(
        '#theme' => 'tabt_members_admin_members_memberlist_unregistered',
        '#tree' => TRUE,
      );

      $form['tabt_nonmembers_unregistered']['tabt_nonmembers_unregoverview']['tabt_members_members'] = array();
      $form['tabt_nonmembers_unregistered']['tabt_nonmembers_unregoverview']['tabt_members_members'] = $rows;
    }
  }
  // Submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submission function for tabt_members_admin form.
 */
function tabt_members_admin_members_submit($form, &$form_state) {
  $members = tabt_members_get_members_by_id();
  if (isset($form_state['values']['tabt_members_regoverview']['tabt_members_members'])) {
    foreach ($form_state['values']['tabt_members_regoverview']['tabt_members_members'] as $id => $member) {
      if (!isset($members[$id])) {
        continue;
      }
      _tabt_update_profilepic($id, $member, $members);
    }
  }
  if (isset($form_state['values']['tabt_members_unregoverview']['tabt_members_members'])) {
    foreach ($form_state['values']['tabt_members_unregoverview']['tabt_members_members'] as $id => $member) {
      if (!isset($members[$id])) {
        continue;
      }
      _tabt_members_update_name($id, $member, $members);
      _tabt_members_update_profilepic($id, $member, $members);
    }
  }
  if (isset($form_state['values']['tabt_nonmembers_unregoverview']['tabt_members_members'])) {
    foreach ($form_state['values']['tabt_nonmembers_unregoverview']['tabt_members_members'] as $id => $member) {
      if (isset($members[$id])) {
        continue;
      }
      _tabt_members_update_name($id, $member, $members);
      _tabt_members_update_profilepic($id, $member, $members);
    }
  }
  cache_clear_all('tabt_members', 'cache_tabt', TRUE);
  drupal_set_message(t('The Members were updated and the TabTmembers-cache was cleared.'), 'status');
}

/**
 * Update name of members.
 *
 * @param id
 *   Memberid.
 * @param member
 *   The member to process.
 * @param members
 *   A list of all members.
 */
function _tabt_members_update_name($id, $member, $members) {
  if ($member['name'] == '' || (isset($members[$id]) && $member['name'] === $members[$id]->FirstName . ' ' . $members[$id]->LastName)) {
    $result = db_select('tabt_members', 't')
    ->fields('t')
    ->condition('id', $id)
    ->execute()
    ->fetchObject();
    if ($result) {
      if ($result->pic) {
        db_update('tabt_members')
        ->fields(array(
          'name' => NULL,
        ))
        ->condition('id', $id)
        ->execute();
      }
      else {
        db_delete('tabt_members')
        ->condition('id', $id)
        ->execute();
      }
    }
  }
  elseif ((!isset($members[$id]) || $member['name'] !== $members[$id]->Name) && $member['name'] != '') {
    $result = db_select('tabt_members', 't')
    ->fields('t')
    ->condition('id', $id)
    ->execute()
    ->fetchObject();
    if ($result) {
      db_update('tabt_members')
      ->fields(array(
        'name' => $member['name'],
      ))
      ->condition('id', $id)
      ->execute();
    }
    else {
      db_insert('tabt_members')
      ->fields(array(
        'id' => $id,
        'name' => $member['name'],
      ))
      ->execute();
    }
  }
}

/**
 * Update profilepictures of members.
 *
 * @param id
 *   Memberid.
 * @param member
 *   The member to process.
 * @param members
 *   A list of all members.
 */
function _tabt_members_update_profilepic($id, $member, $members) {
  if ($member['profilepic'] == '' || (isset($members[$id]) && isset($members[$id]->Results->Pic) && $members[$id]->Results->Pic == $member['profilepic'])) {
    $result = db_select('tabt_members', 't')
    ->fields('t')
    ->condition('id', $id)
    ->execute()
    ->fetchObject();
    if ($result) {
      if ($result->name) {
        db_update('tabt_members')
        ->fields(array(
          'pic' => NULL,
        ))
        ->condition('id', $id)
        ->execute();
      }
      else {
        db_delete('tabt_members')
        ->condition('id', $id)
        ->execute();
      }
    }
  }
  elseif (!isset($members[$id]) || $member['profilepic'] !== $members[$id]->ProfilePic) {
    $result = db_select('tabt_members', 't')
    ->fields('t')
    ->condition('id', $id)
    ->execute()
    ->fetchObject();
    if ($result) {
      db_update('tabt_members')
      ->fields(array(
        'pic' => $member['profilepic'],
      ))
      ->condition('id', $id)
      ->execute();
    }
    else {
      db_insert('tabt_members')
      ->fields(array(
        'id' => $id,
        'pic' => $member['profilepic'],
      ))
      ->execute();
    }
  }
}