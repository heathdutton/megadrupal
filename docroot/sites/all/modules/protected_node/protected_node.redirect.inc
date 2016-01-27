<?php

/**
 * @file
 * Redirected page callback file for the protected_node module.
 */

/**
 * Create the form asking the end users for the node password.
 *
 * The function expects two $_GET with variables named: destination
 * and protected_page. The destination is a URL back to the protected
 * page of which the password is being required. The protected_page
 * variable is set to the nid of that protected page (see todo below).
 *
 * The function accepts a $_GET named 'back'. This is a URL used for the
 * Cancel link shown next to the OKAY button.
 *
 * @todo
 * The redirection uses a destination and a protected_page parameter. The
 * protected_page can be inferred from the destination since the destination
 * represents a node. We want to remove the use of the protected_page because
 * that could be set to a node nid that has nothing to do with the destination
 * (which is not a security risk, but can make it confusing.)
 *
 * @todo
 * It would be a good idea to transform this function in a theme() call instead.
 */
function protected_node_enterpassword() {
  global $user;

  // Make sure we have a destination and a node nid,
  // otherwise there is no password to check.
  // @todo Extract the nid from the destination URI?
  if (!isset($_GET['destination']) || empty($_GET['protected_page']) || !is_numeric($_GET['protected_page'])) {
    // Illegal call.
    watchdog('protected_node', 'Illegal call to /protected-node', array(), WATCHDOG_WARNING);
    drupal_access_denied();
    exit();
  }

  $node = node_load($_GET['protected_page']);
  if (!$node) {
    // Illegal node identifier.
    watchdog('protected_node', 'Invalid nid (@nid) used with /protected-node', array('@nid' => $_GET['protected_page']), WATCHDOG_WARNING);
    drupal_access_denied();
    exit();
  }

  // Some variable initialization.
  $types = node_type_get_types();
  $node_type = $types[$node->type];
  $has_token = module_exists('token');

  // Setup the title of this page.
  $title = variable_get('protected_node_title', NULL);
  if (!empty($title)) {
    if ($has_token) {
      $title = token_replace($title, array('node' => $node));
      $title = token_replace($title, array('user' => $user));
    }
    drupal_set_title($title);
  }

  // Information appear between the title and the password form.
  $info = variable_get('protected_node_info', '');
  if ($has_token) {
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
  elseif ($has_token) {
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
  if ($label) {
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

  // The node we're working on.
  $form['protected_node_nid'] = array(
    '#type' => 'hidden',
    '#value' => $_GET['protected_page'],
  );

  // Add a submit button.
  $form['protected_node_enterpassword']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK'),
  );

  // Add a cancel link when 'back' is defined (i.e. referer on the previous
  // page).
  if (isset($_GET['back'])) {
    $cancel = urldecode($_GET['back']);
  }
  elseif (variable_get('protected_node_cancel', 0)) {
    $cancel = '<front>';
  }
  if (!empty($cancel)) {
    $form['protected_node_enterpassword']['cancel'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Cancel'), $cancel),
    );
  }

  return $form;
}

/**
 * Verify that the user entered the correct password.
 *
 * For the flood control, @see user_login_authenticate_validate().
 */
function protected_node_enterpassword_validate($form, &$form_state) {
  $max_attempt = variable_get('protected_node_failed_password_ip_limit', 50);
  $flood_window = variable_get('protected_node_failed_password_ip_window', 3600);
  if (!flood_is_allowed('failed_protected_node_attempt_ip', $max_attempt, $flood_window)) {
    form_set_error('password', t('Sorry, too many failed password attempts from your IP address. This IP address is temporarily blocked. Try again later.'));
    return;
  }
  // @todo We do not want to check the global password if there is a local
  // password (i.e. extract local password instead of comparing).
  // @todo The protected_node_nid parameter should be extracted from the
  // destination URI.
  $sha1_passwd = sha1($form_state['values']['password']);
  $sha256_passwd = hash('sha256', $form_state['values']['password']);
  $protected_node_nid = $form_state['values']['protected_node_nid'];
  $nid = db_select('protected_nodes')
    ->fields('protected_nodes', array('nid'))
    ->condition('protected_node_passwd', array($sha1_passwd, $sha256_passwd), 'IN')
    ->condition('nid', $protected_node_nid)
    ->execute()
    ->fetchField();
  $node = node_load($protected_node_nid);
  if (empty($nid)) {
    // Global content type password exists ?
    switch (variable_get('protected_node_use_global_password', PROTECTED_NODE_PER_NODE_PASSWORD)) {
      case PROTECTED_NODE_PER_NODE_AND_GLOBAL_PASSWORD:
      case PROTECTED_NODE_GLOBAL_PASSWORD:
        $global_passwd = variable_get('protected_node_global_password', '');
        if (in_array($global_passwd, array($sha1_passwd, $sha256_passwd))) {
          $_SESSION['has_entered_global_password'] = 1;
          $nid = 1;
        }
        else {
          // This comes last so we avoid loading the node if another password
          // matches although that means the main global password has priority
          // which may, in the long run, be a problem (but since the result is
          // the same, I don't foresee this being a problem at all).
          $node_type_passwd = variable_get('protected_node_node_type_password_' . $node->type, '');
          if (in_array($node_type_passwd, array($sha1_passwd, $sha256_passwd))) {
            $nid = 1;
          }
        }
        if (!empty($nid)) {
          // The user found a global password.
          // Was the protected node created by an anonymous user?
          // If so, prevent the use of any global password.
          $created = db_select('node')
            ->fields('node', array('created'))
            ->condition('nid', $protected_node_nid)
            ->condition('uid', 0)
            ->execute()
            ->fetchField();
          if ($created) {
            $nid = FALSE;
          }
        }
        break;
    }

    if (empty($nid)) {
      flood_register_event('failed_protected_node_attempt_ip', $flood_window);
      form_set_error('password', t('Incorrect password!'));
    }
  }
}

/**
 * Allow the user to see this node.
 */
function protected_node_enterpassword_submit($form, &$form_state) {
  // @todo The protected_node_nid parameter should be extracted from the
  // destination URI.
  if (isset($_SESSION['has_entered_global_password'])) {
    $_SESSION['_protected_node']['passwords']['global'] = REQUEST_TIME;
  }
  else {
    $_SESSION['_protected_node']['passwords'][$form_state['values']['protected_node_nid']] = REQUEST_TIME;
  }
}
