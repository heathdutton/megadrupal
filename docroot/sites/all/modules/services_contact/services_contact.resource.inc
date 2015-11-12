<?php

/**
 * Determines whether the current user can access the resource.
 *
 * @param string $perm
 *   String indicating which permission to check access on.
 *
 * @return boolean
 *   Boolean indicating whether or not the user has access to the resource.
 *
 * @see node_access()
 */
function _contact_resource_access($perm) {
  return user_access($perm);
}

/**
 * The site contact form service resource callback.
 */
function services_contact_site($name, $mail, $subject, $cid, $message, $copy) {
  global $user;
  // Don't let authenticated users send e-mail with anyone else's name.
  if ($user->uid != 0 && $name != $user->name) {
    return services_error(t('Name does not match current user.'), 406);
  }
  // Don't let authenticated users send e-mail as anyone else.
  if ($user->uid != 0 && $mail != $user->mail) {
    return services_error(t('E-mail address does not match current user.'), 406);
  }
  // Load the required includes.
  module_load_include('inc', 'contact', 'contact.pages');
  // Fill in the form state.
  $form_state['values']['name'] = $name;
  $form_state['values']['mail'] = $mail;
  $form_state['values']['subject'] = $subject;
  if (isset($cid)) { $form_state['values']['cid'] = $cid; }
  $form_state['values']['message'] = $message;
  // Only authenticated users can send themselves a copy.
  if (isset($copy) && $user->uid != 0) { $form_state['values']['copy'] = $copy; }
  // Submit the form.
  $ret = drupal_form_submit('contact_site_form', $form_state);
  if ($errors = form_get_errors()) {
    return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  }
  else {
    return true;
  }
}

/**
 * The personal contact form service resource callback.
 */
function services_contact_personal($name, $mail, $to, $subject, $message, $copy) {
  global $user;
  // Don't let authenticated users send e-mail with anyone else's name.
  if ($user->uid != 0 && $name != $user->name) {
    return services_error(t('Name does not match current user.'), 406);
  }
  // Don't let authenticated users send e-mail as anyone else.
  if ($user->uid != 0 && $mail != $user->mail) {
    return services_error(t('E-mail address does not match current user.'), 406);
  }
  // Load the required includes.
  module_load_include('inc', 'contact', 'contact.pages');
  // Fill in the form state.
  $form_state['values']['name'] = $name;
  $form_state['values']['mail'] = $mail;
  $form_state['values']['to'] = $to;
  $form_state['values']['subject'] = $subject;
  $form_state['values']['message'] = $message;
  // Only authenticated users can send themselves a copy.
  if (isset($copy) && $user->uid != 0) { $form_state['values']['copy'] = $copy; }
  // Submit the form.
  $ret = drupal_form_submit('contact_personal_form', $form_state, user_load_by_name($to));
  if ($errors = form_get_errors()) {
    return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  }
  else {
    return true;
  }
}

/**
 * Retrieves all the contact form categories.
 */
function services_contact_categories() {
  global $user;
  $query = db_query('SELECT * FROM {contact} c ORDER BY c.weight ASC');
  $results = $query->fetchAll();
  if (!user_access('administer contact form')) {
    foreach($results as $result) {
      if (isset($result->recipients)) { unset($result->recipients); }
      if (isset($result->reply)) { unset($result->reply); }
    }
  }
  return $results;
}

