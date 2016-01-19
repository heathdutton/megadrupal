<?php
/**
 * @file
 * Contains all function that build specific pages for opac users module.
 */

/**
 * Build current user checkouts page.
 *
 * @return string
 *   HTML output.
 */
function opac_users_checkouts_page() {
  if (count($_SESSION['opac_user'])) {
    $user_id = $_SESSION['opac_user']['opac_user_id'];
    $serv_id = $_SESSION['opac_user']['opac_server'];

    ctools_include('object-cache');
    $checkouts = ctools_object_cache_get('opac_users_page', 'checkouts');
    if (!$checkouts) {
      module_load_include('inc', 'opac_users', "opac_users.db");
      module_load_include('inc', 'opac', "includes/opac.db");

      // Create an instance of the corresponding connector.
      $server = opac_server_load($serv_id);
      $connector = opac_get_instance($server);

      // Get all user's checkouts.
      $checkouts = $connector->patronCheckouts($user_id);

      // We call the theme for the page only if there are checkouts.
      if (count($checkouts)) {
        // Format required date fields.
        $today = time();
        foreach ($checkouts as $i => $checkout) {
          // Is the document overdue?
          if ($checkout['duedate'] < $today) {
            $checkouts[$i]['overdue'] = 1;
          }

          // Add server/library name.
          $checkouts[$i]['serv_name'] = $server->serv_name;
        }
        ctools_object_cache_set('opac_users_page', 'checkouts', $checkouts);
      }
    }
    $output = theme('opac_users_checkouts', array('checkouts' => $checkouts, 'serverid' => $serv_id));
  }
  else {
    $output = "<h1>" . t('You are not authentified for this library.') . "</h1>";
  }

  return $output;
}

/**
 * Clear CTools cache and go back to checkouts page.
 */
function opac_users_refresh_checkouts_page() {
  ctools_include('object-cache');
  ctools_object_cache_clear('opac_users_page', 'checkouts');
  $path = variable_get('opac_users_checkouts_page_path', 'opac-user/checkouts');
  drupal_goto($path);
}

/**
 * Build current user holds page.
 *
 * @return string
 *   HTML output.
 */
function opac_users_holds_page() {
  if (count($_SESSION['opac_user'])) {
    $serv_id = $_SESSION['opac_user']['opac_server'];
    $user_id = $_SESSION['opac_user']['opac_user_id'];

    ctools_include('object-cache');
    $holds = ctools_object_cache_get('opac_users_page', 'holds');
    if (!$holds) {
      module_load_include('inc', 'opac_users', "opac_users.db");
      module_load_include('inc', 'opac', "includes/opac.db");

      // Get the user's server and create an instance of the connector.
      $server = opac_server_load($serv_id);
      $connector = opac_get_instance($server);

      // Get all user's holds.
      $holds = $connector->patronHolds($user_id);
      ctools_object_cache_set('opac_users_page', 'holds', $holds);
    }

    $output = theme('opac_users_holds', array('holds' => $holds, 'serverid' => $serv_id));
  }
  else {
    $output = "<h1>" . t('You are not authentified for this library.') . "</h1>";
  }

  return $output;
}

/**
 * Clear CTools cache and go back to holds page.
 */
function opac_users_refresh_holds_page() {
  ctools_include('object-cache');
  ctools_object_cache_clear('opac_users_page', 'holds');
  $path = variable_get('opac_users_holds_page_path', 'opac-user/holds');
  drupal_goto($path);
}

/**
 * Form constructor for the user library account form.
 *
 * This page allows users to create an opac account
 * by entering login/password and choosing the
 * ILS server they suscribed.
 *
 * @see opac_users_library_account_form_submit()
 * @ingroup forms
 */
function opac_users_library_account_form($form, &$form_state) {
  $opac_users_auth_method = variable_get('opac_users_auth_method', 0);

  if ($opac_users_auth_method == OPAC_USER_AUTH_METHOD_LIBRARY) {
    module_load_include('inc', 'opac', "includes/opac.db");
    module_load_include('inc', 'opac_users', "opac_users.db");
    module_load_include('inc', 'opac_users', "includes/opac_users.user");

    // Get only available servers.
    $servers = opac_server_load_multiple(FALSE, array('serv_enabled' => 1));
    // Build opac_server dropdown options.
    $options = array();
    foreach ($servers as $id => $server) {
      $options[$id] = $server->serv_name;
    }

    // Get the current user this form is for.
    global $user;
    $account = user_load($user->uid);

    $form['uid'] = array(
      '#type' => 'hidden',
      '#value' => $user->uid,
    );

    $opac_login = opac_user_get_login($account);
    $opac_password = opac_user_get_password($account);
    $opac_server = opac_user_get_server($account);

    $form['opac_login'] = array(
      '#type' => 'textfield',
      '#title' => t('Library login'),
      '#default_value' => isset($opac_login) ? $opac_login : '',
      '#size' => 20,
      '#maxlength' => 60,
      '#required' => TRUE,
      '#attributes' => array(
        'autocomplete' => array('off'),
      ),
    );
    $form['opac_pass'] = array(
      '#type' => 'password',
      '#title' => t('Library password'),
      '#maxlength' => 128,
      '#size' => 20,
      '#required' => TRUE,
      '#attributes' => array('value' => array(isset($opac_password) ? $opac_password : '')),
    );
    $form['opac_server'] = array(
      '#type' => 'select',
      '#title' => t('Library'),
      '#options' => $options,
      '#default_value' => isset($opac_server) ? $opac_server : '',
      '#description' => t('Select the library for which you have an account.'),
      '#required' => TRUE,
    );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));
    // Test button allows to check the authentification succeeded.
    $form['actions']['test'] = array('#type' => 'submit', '#value' => t('Test'));
  }
  else {
    if ($opac_users_auth_method == 1) {
      $markup = t('ILS authentication is configured to use your Drupal credentials.', array(), array('context' => 'libraries'))
        . " " . t("This page should not be used.");
    }
    else {
      $markup = t('ILS authentication is disabled.', array(), array('context' => 'libraries'));
    }
    $form['information'] = array(
      '#markup' => $markup,
    );
  }

  return $form;
}

/**
 * Form submission handler for opac_users_library_account_form().
 */
function opac_users_library_account_form_submit($form, &$form_state) {
  $opac_users_auth_method = variable_get('opac_users_auth_method', 0);

  if ($opac_users_auth_method == 2) {
    module_load_include('inc', 'opac', "includes/opac.db");
    module_load_include('inc', 'opac_users', "includes/opac_users.user");
    $values = $form_state['values'];

    // Is it an authentification test?
    $test = FALSE;
    if ($form_state['clicked_button']['#value'] == $values['test']) {
      $test = TRUE;
    }

    // Create an instance of the connector and check
    // if the authenticateUser exists.
    $server = opac_server_load($values['opac_server']);
    $connector = opac_get_instance($server);
    if (!method_exists($connector, 'authenticateUser')) {
      form_set_error('opac_users', t("Authentification error: Your Library server doesn't support authentification."));
    }

    // Processing authentification. If the returned value
    // equals 0, that means it fails.
    $patron_id = $connector->authenticateUser($values['opac_login'], $values['opac_pass']);
    if ($patron_id == 0) {
      form_set_error('opac_users', t('Authentification fails: Invalid login or password'));
    }
    // Authentification succeeded. We can create the
    // opac account if it's not a test.
    else {
      if (!$test) {
        global $user;
        $account = user_load($user->uid);
        opac_user_set_login($account, $values['opac_login']);
        opac_user_set_password($account, $values['opac_pass']);
        opac_user_set_server($account, $values['opac_server']);
        user_save($account);
        $_SESSION['opac_user'] = array(
          'opac_user_id' => $patron_id,
          'opac_server' => $values['opac_server'],
        );
      }
      drupal_set_message(t('Authentification success'), 'status', TRUE);
    }
  }
}

/**
 * Form constructor for the user holding item form.
 *
 * It is a confirmation step to hold an item after
 * the user had clicked on hold link on the items
 * list page for a given node.
 *
 * @param object $node
 *   The opac node for which holding items.
 *
 * @param string $itemid
 *   The item identifier in ILS server.
 *
 * @see opac_users_holditem_form_submit()
 * @ingroup forms
 */
function opac_users_holditem_form($form, &$form_state, $node, $itemid = NULL) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  module_load_include('inc', 'opac', "includes/opac.db");

  // Get server and record identifiers.
  list($recordid, $serverid) = opac_get_record($node->nid);

  $opac_user = $_SESSION['opac_user'];

  // Opac user does not exists.
  if (!isset($opac_user['opac_user_id'])) {
    $form['message'] = array(
      '#markup' => "<h4>" . t("Your are not allowed to perform this operation") . "</h4>",
    );
    $form['back'] = array(
      '#type' => 'submit',
      '#value' => t('Back to items list', array(), array('context' => 'libraries')),
    );
    return $form;
  }
  // This user is not a borrower of correponding ILS.
  elseif ($opac_user['opac_server'] != $serverid) {
    $form['message'] = array(
      '#markup' => "<h4>" . t("You are not allowed to hold any items for this library", array(), array('context' => 'libraries')) . "</h4>",
    );
    $form['back'] = array(
      '#type' => 'submit',
      '#value' => t('Back to items list', array(), array('context' => 'libraries')),
    );
    return $form;
  }
  $patron_id = $opac_user['opac_user_id'];

  $server = opac_server_load($serverid);
  $connector = opac_get_instance($server);

  // Check if user can place a hold.
  if (isset($itemid)) {
    $items_status = $connector->itemsStatus($recordid, array(), $patron_id);
    $items = $items_status['items'];
    if (!isset($items[$itemid]) || !$items[$itemid]['canhold']) {
      $form['message'] = array(
        '#markup' => "<h4>" . t("You are not allowed to hold this item", array(), array('context' => 'libraries')) . "</h4>",
      );
      $form['back'] = array(
        '#markup' => l(t('Go back to items list', array(), array('context' => 'libraries')), 'node/' . $node->nid . '/items'),
      );
      return $form;
    }
  }
  elseif (!$connector->biblioIsHoldable($recordid, $patron_id)) {
    $form['message'] = array(
      '#markup' => "<h4>" . t("You are not allowed to hold this title", array(), array('context' => 'libraries')) . "</h4>",
    );
    $form['back'] = array(
      '#markup' => l(t('Go back to record', array(), array('context' => 'libraries')), 'node/' . $node->nid),
    );
    return $form;
  }

  // Hidden values of the following elements will be use
  // in the submit function only.
  $form['serverid'] = array('#type' => 'hidden', '#value' => $serverid);
  $form['recordid'] = array('#type' => 'hidden', '#value' => $recordid);
  $form['nid'] = array('#type' => 'hidden', '#value' => $node->nid);
  $form['itemid'] = array('#type' => 'hidden', '#value' => $itemid);
  $form['opacuserid'] = array('#type' => 'hidden', '#value' => $patron_id);

  $form['message'] = array(
    '#markup' => "<h4>" . t('Please confirm you want to hold', array(), array('context' => 'libraries')) . ' ' . l($node->title, "node/$node->nid") . "</h4>",
  );

  // Get specific ILS hold form.
  if (method_exists($connector, 'placeHoldForm')) {
    $record = array(
      'recordid' => $recordid,
      'record_title' => $node->title,
      'nid' => $node->nid,);
    $connector_form = $connector->placeHoldForm($patron_id, $record, $itemid);
    $form += $connector_form;
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['confirm'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm hold', array(), array('context' => 'libraries')),
  );
  $form['actions']['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back to items list', array(), array('context' => 'libraries')),
  );

  return $form;
}

/**
 * Form submission handler for opac_users_holditem_form().
 */
function opac_users_holditem_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $values = $form_state['values'];
  // Check if the user has confirmed hold or wish to go back.
  if (isset($values['confirm']) && $form_state['clicked_button']['#value'] == $values['confirm']) {
    // Place the hold.
    $server = opac_server_load($values['serverid']);
    $connector = opac_get_instance($server);
    $result = $connector->placeHold($values);

    if ($result['success']) {
      drupal_set_message("<b>" . t('Item held with success', array(), array('context' => 'libraries')) . "</b><br />" . $result['message']);
    }
    else {
      drupal_set_message("<b>Error</b><br />" . $result['message'], 'error');
    }
    ctools_include('object-cache');
    ctools_object_cache_clear('opac_users_page', 'holds');
    drupal_goto("node/" . $values['nid'] . "/items");
  }
  // Go back to the items list.
  elseif ($form_state['clicked_button']['#value'] == $values['back']) {
    drupal_goto("node/" . $values['nid'] . "/items");
  }
}

/**
 * Form constructor for the user cancelling hold form.
 *
 * @param object $node
 *   The opac node for which holding items.
 *
 * @param string $itemid
 *   The item identifier in ILS server.
 *
 * @see opac_users_cancelhold_form_submit()
 * @ingroup forms
 */
function opac_users_cancelhold_form($form, &$form_state, $node, $itemid) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  module_load_include('inc', 'opac', "includes/opac.db");

  // Item identifiers value will be used in the submit function only.
  $form['itemid'] = array('#type' => 'hidden', '#value' => $itemid);

  // Ask the user for confirm or not.
  $form['message'] = array(
    '#markup' => "<h4>" . t('Please confirm you want to cancel hold on', array(), array('context' => 'libraries')) . ' ' . l($node->title, "node/$node->nid") . "</h4>",
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Confirm'));
  return $form;
}

/**
 * Form submission handler for opac_users_cancelhold_form().
 */
function opac_users_cancelhold_form_submit($form, &$form_state) {
  $opac_user = $_SESSION['opac_user'];
  $values = $form_state['values'];
  $result;
  // If yes, checking if credential are correct
  // and cancel the hold.
  if (isset($opac_user['opac_user_id'])) {
    $server = opac_server_load($opac_user['opac_server']);
    $connector = opac_get_instance($server);
    $result = $connector->cancelHold($opac_user['opac_user_id'], $values['itemid']);
  }

  if ($result) {
    drupal_set_message(t('Hold canceled with success', array(), array('context' => 'libraries')), 'status', TRUE);
  }
  else {
    drupal_set_message(t('Unable to cancel hold', array(), array('context' => 'libraries')), 'error', TRUE);
  }
  ctools_include('object-cache');
  ctools_object_cache_clear('opac_users_page', 'holds');
  drupal_goto("opac-user/holds");
}

/**
 * Form constructor for the user renewing loan form.
 *
 * @param object $node
 *   The opac node for which holding items.
 *
 * @param string $itemid
 *   The item identifier in ILS server.
 *
 * @see opac_users_renewloan_form_submit()
 * @ingroup forms
 */
function opac_users_renewloan_form($form, &$form_state, $node, $itemid) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  module_load_include('inc', 'opac', "includes/opac.db");

  // Item identifiers value will be used in the submit function only.
  $form['itemid'] = array('#type' => 'hidden', '#value' => $itemid);

  // Ask the user for confirm or not.
  $form['message'] = array(
    '#markup' => "<h4>" . t('Please confirm you want to renew this loan: ', array(), array('context' => 'libraries')) . l($node->title, "node/$node->nid") . "</h4>",
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Confirm'));
  return $form;
}

/**
 * Form submission handler for opac_users_renewloan_form().
 */
function opac_users_renewloan_form_submit($form, &$form_state) {
  $opac_user = $_SESSION['opac_user'];
  $values = $form_state['values'];
  $result;

  // If yes, checking if credential are correct
  // and cancel the hold.
  if (isset($opac_user['opac_user_id'])) {
    $server = opac_server_load($opac_user['opac_server']);
    $connector = opac_get_instance($server);
    $result = $connector->renewItem($opac_user['opac_user_id'], $values['itemid']);
  }

  if ($result['success']) {
    drupal_set_message(t('Renew success'), 'status', TRUE);
  }
  else {
    drupal_set_message(t('Unable to renew loan', array(), array('context' => 'libraries')), 'error', TRUE);
  }
  ctools_include('object-cache');
  ctools_object_cache_clear('opac_users_page', 'checkouts');
  drupal_goto("opac-user/checkouts");
}

/**
 * Display user's issues history.
 */
function opac_users_issues_history_page() {
  if (count($_SESSION['opac_user'])) {
    $user_id = $_SESSION['opac_user']['opac_user_id'];
    $serv_id = $_SESSION['opac_user']['opac_server'];

    ctools_include('object-cache');
    $issues_history = ctools_object_cache_get('opac_users_page', 'issues_history');
    if (!$issues_history) {
      module_load_include('inc', 'opac_users', "opac_users.db");
      module_load_include('inc', 'opac', "includes/opac.db");

      // Create an instance of the corresponding connector.
      $server = opac_server_load($serv_id);
      $connector = opac_get_instance($server);

      // Get all user's issues_history.
      $issues_history = $connector->patronIssuesHistory($user_id);

      ctools_object_cache_set('opac_users_page', 'issues_history', $issues_history);
    }
    $output = theme('opac_users_issues_history', array('issues' => $issues_history, 'serverid' => $serv_id));
  }
  else {
    $output = "<h1>" . t('You are not authentified for this library.') . "</h1>";
  }

  return $output;
}

/**
 * Clear CTools cache and go back to checkouts page.
 */
function opac_users_refresh_issues_history_page() {
  ctools_include('object-cache');
  ctools_object_cache_clear('opac_users_page', 'issues_history');
  $path = variable_get('opac_users_issues_history_page_path', 'opac-user/issues-history');
  drupal_goto($path);
}
