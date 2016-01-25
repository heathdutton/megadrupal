<?php
// $Id$

/**
 * @file
 * Page callbacks for Batch operations.
 */

function _slc_import_users($result, &$context = array()) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_user'] = 0;
    $context['sandbox']['max'] = count($result);
  }
  $student = $result[$context['sandbox']['progress']];
	$user_mail = $student->electronicMail[0]->emailAddress;
	$user_name = $user_mail;
	if(empty($user_mail)){
		$user_mail = 'nikhildubbaka+abc@gmail.com';
		$user_name = drupal_system_string($student->name->firstName . ' ' . $student->name->lastSurname);
	}
	$user = _slc_user_check($user_name, $user_mail, 0);
	$user->field_slc_first_name['und'][0]['value'] = $student->name->firstName;
	$user->field_slc_last_name['und'][0]['value'] = $student->name->lastSurname;
	user_save($user);
    $context['results'][] = $student->id . ' : ' . check_plain($student->name->firstName . ' ' . $student->name->lastSurname);
    $context['sandbox']['progress']++;
    $context['sandbox']['current_user'] = $student->id;
    $context['message'] = check_plain($student->name->firstName . ' ' . $student->name->lastSurname);
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function _slc_import_finished($success, $results, $operations) {
  // The 'success' parameter means no fatal PHP errors were detected. All
  // other error management should be handled using 'results'.
  if ($success) {
    $message = format_plural(count($results), 'One post processed.', '@count users imported.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
  // Providing data for the redirected page is done through $_SESSION.
  $items = array();
  foreach ($results as $result) {
    $items[] = t('Loaded node %title.', array('%title' => $result));
  }
  $_SESSION['my_batch_results'] = $items;
}

 function _slc_user_check($user_name, $user_mail, $send_mail) {
  //  Check for the existence of current user $newnode->user_name
  //  Create if not exists
  //  put uid accordingly $newnode->uid
  $result = db_query("SELECT uid FROM {users} WHERE name = :name LIMIT 1", array(':name' => $user_name));
$user_id = $result->fetchField();
  if ($user_id!="") {
    //  user exists
    return $user_id;
  }   
  else {
    //  user not exists
    //  So create user
    //  send mail to user email id
    //  Drupal User module function that generates MD5 hash password
    $pass = user_password();
   // $role_id = variable_get('camtasia_relay_user_roles', DRUPAL_AUTHENTICATED_RID);
    
 $role_name = 'SLC Student';
 $role_id = db_query("SELECT rid FROM {role} WHERE name = :name", array(':name' => $role_name))->fetchField();
    //'access' => 0, /* optional, i didnt want user to be able to use this account yet*/
    $roles = array($role_id => $role_name);
    $details = array(
      'name' => $user_name,
      'pass' => $pass,
      'mail' => $user_mail,
      'status' => 1,
      'roles' => $roles,
      );
    
    $user = user_save(null, $details);
    if ($send_mail==1) {
      //	yes
      _user_mail_notify('register_admin_created', $user,  NULL);
    }
  }   //  else
  return $user;  
}

function drupal_system_string($string, $replacement = '_') {
return preg_replace('/[^a-z0-9]+/', $replacement, strtolower($string));
}