<?php

/**
 * @file
 * Redirected page callback file for the protected_node module.
 *
 * This version supports any number of pages instead of a destination.
 */

/**
 * Create the form asking the end users for the node password.
 *
 * The function expects a $_GET with the variable named protected_pages.
 * This parameter is a list of comma separated NIDs.
 *
 * The function accepts a $_GET named 'back'. This is a URL used for the
 * Cancel link shown next to the OK button.
 *
 * @important
 * The function does NOT use check_plain() on the $info, $description,
 * etc. because the administrator is the only one who can enter that
 * information.
 *
 * @todo
 * It would be a good idea to transform this function in a theme() call instead.
 */
function protected_node_enter_any_password() {
  // Global $user is used for token replacement, should it be the node author?
  global $user;

  if (isset($_GET['destination'])) {
    // Illegal call.
    watchdog('protected_node', 'Illegal call to /protected-nodes: destination parameter specified.', array(), WATCHDOG_WARNING);
    drupal_access_denied();
  }

  $nids = protected_node_get_nids_from_protected_pages_parameter();

  // Check, in reverse order, whether a node password was specified.
  // If so, use that node.
  // @todo: why reverse order?
  foreach (array_reverse($nids) as $nid) {
    if (!empty($_SESSION['_protected_node']['passwords'][$nid])) {
      $when = $_SESSION['_protected_node']['passwords'][$nid];
      // Global reset time.
      if ($when > variable_get('protected_node_session_timelimit', 0)) {
        $node = node_load($nid);
        // This page reset time.
        if ($when > $node->protected_node_passwd_changed) {
          // The password is still known, go to the page.
          drupal_goto('node/' . $nid);
        }
      }
      // If the "best" node had a password and it's gone, we don't test the
      // other pages; instead we accept this one as is...
      break;
    }
  }

  // Make sure the first node exists, the others we don't care as much at this
  // point they are tested when the user submits the password and used as the
  // destination.
  $node = node_load($nids[0]);
  if (!is_object($node)) {
    // Illegal nid.
    watchdog('protected_node', 'Illegal nids[0] (@nid) to /protected-nodes: node does not exist.', array('@nid' => $nids[0]), WATCHDOG_WARNING);
    drupal_access_denied();
  }

  // Some variable initialization.
  $types = node_type_get_types();
  $node_type = $types[$node->type];
  $token_module_enabled = module_exists('token');

  // Set the title of this page.
  $title = variable_get('protected_node_title', NULL);
  if (!empty($title)) {
    if ($token_module_enabled) {
      $title = token_replace($title, array('node' => $node));
      $title = token_replace($title, array('user' => $user));
    }
    drupal_set_title($title);
  }

  // Set the information that appears between the title and the password form.
  $info = variable_get('protected_node_info', '');
  if ($token_module_enabled) {
    $info = token_replace($info, array('node' => $node));
    $info = token_replace($info, array('user' => $user));
  }
  if ($info) {
    $form['protected_node'] = array(
      '#type' => 'fieldset',
      '#description' => filter_xss_admin($info),
      '#collapsible' => FALSE,
    );
  }

  // Enter the detailed description of the protected node password.
  $description = variable_get('protected_node_description_' . $node->type, '');
  if (!$description) {
    $description = variable_get('protected_node_description', '');
  }
  if (!$description) {
    if ($node->protected_node_show_title) {
      // Embellish the title with double quotes.
      $node_title = '"' . $node->title . '"';
    }
    else {
      $node_title = '';
    }
    $description = t('The @node_type @node_title you are trying to view is password protected. Please enter the password below to proceed.',
      array('@node_type' => $node_type->name, '@node_title' => $node_title));
  }
  elseif ($token_module_enabled) {
    $description = token_replace($description, array('node' => $node));
    $description = token_replace($description, array('user' => $user));
  }
  $form['protected_node_enterpassword'] = array(
    '#type' => 'fieldset',
    '#description' => filter_xss_admin($description),
    '#collapsible' => FALSE,
  );

  // Create the password widget.
  $label = variable_get('protected_node_password_label', '');
  if ($token_module_enabled) {
    $label = token_replace($label, array('node' => $node));
    $label = token_replace($label, array('user' => $user));
  }
  else {
    $label = t('@node_type password', array('@node_type' => $node_type->name));
  }
  $form['protected_node_enterpassword']['password'] = array(
    '#type' => 'password',
    '#title' => $label,
    '#size' => 20,
  );

  // Add a submit button.
  $form['protected_node_enterpassword']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK'),
  );

  // Add a cancel link when 'back' is defined (i.e. refers on the previous
  // page).
  if (isset($_GET['back'])) {
    $cancel = urldecode($_GET['back']);
  }
  elseif (variable_get('protected_node_cancel', 0)) {
    $cancel = '<front>';
  }
  if (isset($cancel) && $cancel) {
    $form['protected_node_enterpassword']['cancel'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Cancel'), $cancel),
    );
  }

  return $form;
}

/**
 * Validate callback.
 *
 * Verify that the user entered the correct password.
 *
 * For the flood control, @see user_login_authenticate_validate().
 */
function protected_node_enter_any_password_validate($form, &$form_state) {
  $max_attempt = variable_get('protected_node_failed_password_ip_limit', 50);
  $flood_window = variable_get('protected_node_failed_password_ip_window', 3600);
  if (!flood_is_allowed('failed_protected_node_attempt_ip', $max_attempt, $flood_window)) {
    form_set_error('password', t('Sorry, too many failed password attempts from your IP address. This IP address is temporarily blocked. Try again later.'));
    return;
  }
  // Note that global password cannot work here since we couldn't know where
  // to send the user otherwise.
  $nids = protected_node_get_nids_from_protected_pages_parameter();
  $sha1_passwd = sha1($form_state['values']['password']);
  $sha256_passwd = hash('sha256', $form_state['values']['password']);

  // Get an nid matching the password and nids condition.
  // Arbitrary take the smaller nid.
  $nid = db_select('protected_nodes')
    ->fields('protected_nodes', array('nid'))
    ->condition('protected_node_passwd', array($sha1_passwd, $sha256_passwd), 'IN')
    ->condition('nid', $nids, 'IN')
    ->orderBy('nid', 'ASC')
    ->range(0, 1)
    ->execute()
    ->fetchField();

  if (empty($nid)) {
    flood_register_event('failed_protected_node_attempt_ip', $flood_window);
    form_set_error('password', t('Incorrect password!'));
  }
  else {
    // Set a value in $form_state to use in submit.
    $form_state['values']['protected_node_selected_nid'] = $nid;
  }
}

/**
 * Submit callback.
 *
 * Allow the user to see this node.
 */
function protected_node_enter_any_password_submit($form, &$form_state) {
  $nids = protected_node_get_nids_from_protected_pages_parameter();
  $nid = $form_state['values']['protected_node_selected_nid'];

  // Switch the session to the newly unlocked page.
  foreach ($nids as $n) {
    if ($n == $nid) {
      $_SESSION['_protected_node']['passwords'][$n] = REQUEST_TIME;
    }
    else {
      unset($_SESSION['_protected_node']['passwords'][$n]);
    }
  }

  drupal_goto('node/' . $nid);
}

/**
 * Helper function.
 *
 * Transforms the $_GET['protected_pages'] in a valid list
 * of $nids. Anything that is not valid we ignore. If there
 * isn't at least 1 then the function generates an access
 * denied error.
 *
 * @return array
 *   The array of nids.
 */
function protected_node_get_nids_from_protected_pages_parameter() {
  $nids = array();
  if (isset($_GET['protected_pages'])) {
    $nids_list = explode(',', $_GET['protected_pages']);
    foreach ($nids_list as $nid) {
      $nid = trim($nid);
      if (is_numeric($nid)) {
        $nids[] = $nid;
      }
    }

    // Make sure we have at least one destination otherwise there is no password
    // to check.
    if (count($nids) == 0) {
      // Illegal call.
      watchdog('protected_node', 'Illegal call to /protected-nodes: no nid specified.', array(), WATCHDOG_WARNING);
      drupal_access_denied();
    }
  }
  else {
    // Illegal call.
    watchdog('protected_node', 'Illegal call to /protected-nodes: no protected_pages parameter specified.', array(), WATCHDOG_WARNING);
    drupal_set_message(t("You need to enter nids in the protected_pages parameter i.e: protected-nodes?protected_pages=1,2,3"), 'error');
    drupal_access_denied();
  }

  return $nids;
}
