<?php

// Helper functions for userpoints_node_action.module

define("USERPOINTS_NODE_ACTION_PUBLISH", "status");
define("USERPOINTS_NODE_ACTION_STICKY", "sticky");
define("USERPOINTS_NODE_ACTION_PROMOTE", "promote");
define("USERPOINTS_NODE_ACTION_CREATE", "userpoints_node_created");

define("USERPOINTS_NODE_ACTION_READ_CREATE", "<Create>");
define("USERPOINTS_NODE_ACTION_READ_PUBLISH", "<Publish>");
define("USERPOINTS_NODE_ACTION_READ_STICKY", "<Sticky>");
define("USERPOINTS_NODE_ACTION_READ_PROMOTE", "<Promote>");

global $userpoints_node_action_read_remove;
$userpoints_node_action_read_remove = array('<', '>');

define("USERPOINTS_NODE_ACTION_MESSAGE_START", "<link_text>");
define("USERPOINTS_NODE_ACTION_MESSAGE_MIDDLE", "<end_link>");
define("USERPOINTS_NODE_ACTION_MESSAGE_END", "<end_text>");

define("USERPOINTS_NODE_ACTION_DELIMITER", "|");

define("USERPOINTS_NODE_ACTION_DEFAULT_MESSAGE", "You do not have enough !category !points to !action this item");

/**
 * Admin form for new rules
 */
function userpoints_node_action_admin_add_form() {
  // Get list of node types and roles
  $roles = user_roles(true);
  $pubtypes = node_type_get_names();
  $actions = _userpoints_node_action_get_actions();
  $categories = userpoints_get_categories();
  
  // Build the form
  $form['add'] = array(
    '#id' => 'userpoints_node_action_admin_add_form',
    '#redirect' => 'admin/config/people/userpoints_node_action',
    '#type' => 'fieldset',
  );
  $form['add']['title'] = array(
    '#markup' => '<h2>' . t('Add Node Action Rule') . '</h2>',
  );
  $form['add']['role'] = array(
    '#type' => 'select',
    '#required' => true,
    '#title' => t('Role'),
    '#options' => $roles,
    '#multiple' => true,
  );
  $form['add']['pubtype'] = array(
    '#type' => 'select',
    '#required' => true,
    '#title' => t('Publication type'),
    '#options' => $pubtypes,
    '#multiple' => true,
  );
  $form['add']['action'] = array(
    '#type' => 'select',
    '#required' => 'true',
    '#title' => t('Action'),
    '#options' => $actions,
    '#description' => t('Actions that are &lt;highlighted&gt; are core actions, other actions are custom defined'),
  );
  $form['add']['category'] = array(
    '#type' => 'select',
    '#required' => 'true',
    '#title' => t('Category'),
    '#options' => $categories,
  );
  $form['add']['points'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#title' => t('!Points', userpoints_translation()),
    '#description' => t('Number of !points it costs to perform the selected action on the selected node for the selected user role.', userpoints_translation()),
    '#maxlength' => 8,
    '#size' => 8,
    '#element_validate' => array('userpoints_node_action_validate_points'),
  );
  $form['add']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  $form['#validate'][] = 'userpoints_node_action_admin_add_form_validate';
  return $form;
}

/**
 * Admin form for the custom message settings
 */
function userpoints_node_action_admin_message_form() {
  // Build the form
  $form['message'] = array(
    '#id' => 'userpoints_node_action_admin_message_form',
    '#redirect' => 'admin/config/people/userpoints_node_action',
    '#type' => 'fieldset',
  );
  $form['message']['title'] = array(
    '#markup' => '<h2>' . t('Customize message options') . '</h2>',
  );
  $form['message']['customMessage'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom message'),
    '#default_value' => variable_get('userpoints_node_action_custom_message', ''),
    '#description' => 'Enter a custom message here that will only get displayed once per form submission. This is useful for things like notifying the user of how to purchase more points. You may use "!points" to refer to the correct term, and may use the custom schema to include links in your message that will open in a new window: ' . htmlentities(USERPOINTS_NODE_ACTION_MESSAGE_START) . 'path' . htmlentities(USERPOINTS_NODE_ACTION_MESSAGE_MIDDLE) . 'desc' . htmlentities(USERPOINTS_NODE_ACTION_MESSAGE_END) . ' (the message needs all three key character sets, exactly as shown). For example, the following message: "' . htmlentities(USERPOINTS_NODE_ACTION_MESSAGE_START) . '/store/item' . htmlentities(USERPOINTS_NODE_ACTION_MESSAGE_MIDDLE) . 'click here to buy more' . htmlentities(USERPOINTS_NODE_ACTION_MESSAGE_END) . '" will effectively call the l() function with the text being "click here to buy more" and being a link to the appropriate /store/item location. This message will get displayed once upon form validation, regardless of how many rules are conflicted with. If you do not wish to have a custom message, simply leave this field blank.',
  );
  $message = userpoints_node_action_translate_message(variable_get('userpoints_node_action_custom_message', ''));
  if (!empty($message)) {
    $form['message']['customMessageExample'] = array(
      '#prefix' => '<div class="form-item"><label>Example of your message: </label><div>',
      '#suffix' => '</div><div class="description">' . t('This is what your custom message will look like when there is a rule conflict.') . '</div>',
      '#markup' => $message,
    );
  }
  $form['message']['displayMessages'] = array(
    '#type' => 'textarea',
    '#title' => t('Override default message'),
    '#default_value' => variable_get('userpoints_node_action_display_custom_message', USERPOINTS_NODE_ACTION_DEFAULT_MESSAGE),
    '#description' => t("This message displays for each rule that has failed (leave blank for no message). The schema used in the above message can be used, as well as the following variables:<br/>!points - correct term for points<br/>!cur_points - current number of points the user has for the relevant category<br/>!num_points - number of points needed (this includes previous action costing)<br/>!category - category of points (may be a blank string)<br/>!action - action the user is trying to take<br/>The default value of this is \"" . USERPOINTS_NODE_ACTION_DEFAULT_MESSAGE . "\". Note that if you don't display default messages and don't provide any custom messages, it will probably be very confusing for the user as the node will be invalid without giving a reason."),
  );
  $form['message']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * This function gets the custom set message
 */
function userpoints_node_action_translate_message($string) {
  $string = t($string);
  if (!empty($string)) {
    $matches = array();
    preg_match_all('/(' . USERPOINTS_NODE_ACTION_MESSAGE_START . ')(.*?)(' . USERPOINTS_NODE_ACTION_MESSAGE_MIDDLE . ')(.*?)(' . USERPOINTS_NODE_ACTION_MESSAGE_END . ')/', $string, $matches, PREG_SET_ORDER);
    foreach ($matches as $replacer) {
      // The order of matches is the following:
      // 0: Entire match (will need this for the replacement)
      // 1: The first marker
      // 2: The link URL
      // 3: The second marker
      // 4: The link text
      // 5: The third marker
      $linker = l($replacer[4], $replacer[2], array('attributes' => array('target' => '_blank')));
      $string = str_replace($replacer[0], $linker, $string);
    }
  }
  return t($string, userpoints_translation());
}

/**
 * Hook for modifying the admin form
 */
function userpoints_node_action_admin_modify_form() {
  // Get list of node types and roles, and actual rules
  $roles = user_roles(true);
  $pubtypes = node_type_get_names();
  $actions = _userpoints_node_action_get_actions();
  $categories = userpoints_get_categories();
  $rules = userpoints_node_action_get_all();
  
  // Build the form
  $form = array(
    '#id' => 'userpoints_node_action_admin_modify_form',
    '#redirect' => 'admin/config/people/userpoints_node_action',
  );
  $form['modify'] = array(
    '#type' => 'fieldset',
    '#tree' => true,
  );
  $form['modify']['title'] = array(
    '#markup' => '<h2>' . t('Stored rules') . '</h2>',
  );
  $form['#theme'] = "userpoints_node_action_admin_modify_form";
  
  // Loop for every stored rule
  $i = 0;
  if (!empty($rules)) {
    foreach ($rules as $rule) {
      // This is a interesting substitution to hide rows that have actions that don't exist anymore
      if (array_key_exists($rule['action'], $actions)) {
        $form['modify'][$i]['role'] = array(
          '#type' => 'select',
          '#required' => true,
          '#options' => $roles,
          '#default_value' => $rule['role_id'],
          '#multiple' => true,
        );
        $form['modify'][$i]['pubtype'] = array(
          '#type' => 'select',
          '#required' => true,
          '#options' => $pubtypes,
          '#default_value' => $rule['pubtype'],
          '#multiple' => true,
        );
        $form['modify'][$i]['action'] = array(
          '#type' => 'select',
          '#required' => true,
          '#options' => $actions,
          '#default_value' => $rule['action'],
        );
        $form['modify'][$i]['category'] = array(
          '#type' => 'select',
          '#required' => true,
          '#options' => $categories,
          '#default_value' => $rule['category'],
        );
        $form['modify'][$i]['points'] = array(
          '#type' => 'textfield',
          '#required' => true,
          '#maxlength' => 8,
          '#size' => 8,
          '#default_value' => $rule['points'],
          '#element_validate' => array('userpoints_node_action_validate_points'),
        );
        $form['modify'][$i]['id'] = array(
          '#type' => 'hidden',
          '#value' => $rule['id'],
        );
        $i++;
      }
    }
  }
  
  if ($i > 0) {
    $form['modify']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Modify'),
      '#weight' => 65535,
      '#tree' => false,
    );
  }
  else {
    $form['modify']['none'] = array(
      '#markup' => t('There are no current rules configured. Add one using the above form.'),
    );
  }
  
  return $form;
}

/**
 * Theme function to render modify rules form in admin side
 */
function theme_userpoints_node_action_admin_modify_form($variables) {
  $form = $variables['form'];
  $header = array(t('Role'), t('Publication type'), t('Action'), t('Category'), t('!Points', userpoints_translation()), '');
  $i = 0;
  $rows = array();
  // Go through all the rows, unset them and turn them into a table
  while (!empty($form['modify'][$i])) {
    $row = array(
      drupal_render($form['modify'][$i]['role']),
      drupal_render($form['modify'][$i]['pubtype']),
      drupal_render($form['modify'][$i]['action']),
      drupal_render($form['modify'][$i]['category']),
      drupal_render($form['modify'][$i]['points']),
      drupal_render($form['modify'][$i]['id']) . l(t('delete'), 'admin/config/people/userpoints_node_action/delete/' . $form['modify'][$i]['id']['#value']),
    );
    unset($form['modify'][$i]);
    $rows[] = $row;
    $i++;
  }
  // Make the table a form element
  $table = theme('table', array('header' => $header, 'rows' => $rows));
  $form['modify']['table'] = array(
    '#markup' => $table,
    '#weight' => 65535,
  );
  return drupal_render_children($form);
}

/**
 * Get all rules and return as multidimensional array
 */
function userpoints_node_action_get_all() {
  $result = db_query("SELECT * FROM {userpoints_node_action}");
  $rules = array();
  foreach ($result as $rule) {
    // Fix up the delimited values
    $rule = (array)$rule;
    $delims = array('role_id', 'pubtype');
    foreach ($delims as $d) {
      $rule['str_' . $d] = $rule[$d];
      // If there isn't a delimiter, this must be an older version, leave it alone
      if (strpos($rule[$d], USERPOINTS_NODE_ACTION_DELIMITER) !== false) {
        // Clear out any of the extra and just get the good data
        $arr = explode(USERPOINTS_NODE_ACTION_DELIMITER, $rule[$d]);
        $empties = array_keys($arr, "");
        foreach ($empties as $e) {
          unset($arr[$e]);
        }
        $rule[$d] = array_values($arr);
      }
    }
    $rules[] = $rule;
  }
  return $rules;
}

/**
 * Get all node actions as array, for formatting
 */
function _userpoints_node_action_get_actions() {
  static $userpoints_node_actions_array = array();
  
  if (empty($userpoints_node_actions_array)) {
    $userpoints_node_actions_array = array(
      USERPOINTS_NODE_ACTION_CREATE => USERPOINTS_NODE_ACTION_READ_CREATE,
      USERPOINTS_NODE_ACTION_PUBLISH => USERPOINTS_NODE_ACTION_READ_PUBLISH,
      USERPOINTS_NODE_ACTION_STICKY => USERPOINTS_NODE_ACTION_READ_STICKY,
      USERPOINTS_NODE_ACTION_PROMOTE => USERPOINTS_NODE_ACTION_READ_PROMOTE,
    );
    // Check the data from custom pub to get any additional options
    if (module_exists('custom_pub')) {
      $types = variable_get('custom_pub_types', array());
      if (!empty($types)) {
        foreach ($types as $type) {
          if (!empty($type['type']) && !empty($type['name']) &&
              !array_key_exists($type['type'], $userpoints_node_actions_array)) {
            $userpoints_node_actions_array[$type['type']] = $type['name'];
          }
        }
      }
    }
  }
  return $userpoints_node_actions_array;
}

/**
 * Returns an object that holds the cost and category. Since we're only getting one (as well as there
 * should only be one limit per person), multiple roles may cause multiple rules to be applied. If this
 * is the case, the rule that costs the most points will be applied.
 * Passing false as category will get the rules for all categories of points
 */
function _userpoints_node_action_user_get_cost($user, $nodetype, $action, $category = false) {
  $i = count($user->roles);
  $addRoles = "";
  foreach ($user->roles as $key => $role) {
    $addRoles .= "role_id LIKE '%" . USERPOINTS_NODE_ACTION_DELIMITER . $key . USERPOINTS_NODE_ACTION_DELIMITER . "%'";
    $i--;
    if ($i > 0) {
      $addRoles .= ' OR ';
    }
  }
  $q = "SELECT MAX(points) as points, category FROM {userpoints_node_action} WHERE action = '$action' AND ($addRoles) AND pubtype LIKE :pubtype";
  if ($category !== false) {
    $q .= " AND category = $category";
  }
  $q .= " GROUP BY category ORDER BY points ASC";
  $results = db_query($q, array(":pubtype" => "%" . _userpoints_node_action_generate_delimited_data(array($nodetype)) . "%"));
  $costs = array();
  foreach ($results as $result) {
    $costs[] = $result;
  }
  return $costs;
}

/**
 * Gets a status message based on if the user has enough points
 */
function _userpoints_node_action_get_status($account, $pubtype, $action, &$totalCost) {
  global $user;
  global $userpoints_node_action_read_remove;
  $results = array();
  $result = null;
  
  if (empty($account) || empty($pubtype) || empty($action)) {
    return $results;
  }
  
  $actions = _userpoints_node_action_get_actions();
  $cost = _userpoints_node_action_user_get_cost($account, $pubtype, $action);
  
  if (!empty($cost)) {
    $updated = array();
    foreach ($cost as $c) {
      if (empty($totalCost[$c->category])) {
        $totalCost[$c->category] = 0;
      }
      $totalCost[$c->category] += $c->points;
      $updated[$c->category] = 1;
    }
    
    foreach ($totalCost as $cat => $amount) {
      if ($updated[$cat]) {
        $points = userpoints_get_current_points($account->uid, $cat);
        if ($points < $amount) {
          $message = variable_get('userpoints_node_action_display_custom_message', USERPOINTS_NODE_ACTION_DEFAULT_MESSAGE);
          if (!empty($message)) {
            $result = userpoints_node_action_translate_message(t($message, userpoints_translation() + array(
              "!category" => ($cat?taxonomy_term_load($cat)->name:''),
              "!cur_points" => $points,
              "!num_points" => $amount,
              "!action" => str_replace($userpoints_node_action_read_remove, "", $actions[$action]),
            )));
          }
          $results[] = $result;
        }
      }
    }
  }
  return $results;
}

/**
 * Makes a user pay for their actions (sounds a little ominous)
 */
function _userpoints_node_action_payment($account, $node, $action) {
  global $user;
  $results = array();
  $result = null;
  $title = $node->title;
  $pubtype = $node->type;
  $actions = _userpoints_node_action_get_actions();
  $cost = _userpoints_node_action_user_get_cost($account, $pubtype, $action);
  if (!empty($cost)) {
    foreach ($cost as $c) {
      $points = userpoints_get_current_points($account->uid, $c->category);
      // They don't have enough points, even though we calculated it before...
      if ($points < $c->points) {
        $result = array(
          'status' => false,
          'reason' => t("Not enough !points ", userpoints_translation()),
        );
        if ($c->category != 0) {
          $result['reason'] .= t("of type %category ", array('%category' => taxonomy_term_load($c->category)->name));
        }
        $result['reason'] .= t("to %action this item", array('%action' => htmlentities($actions[$action])));
      }
      else {
        $params = array(
          'uid' => intval($account->uid),
          'points' => ($c->points * -1),
          'tid' => intval($c->category),
          'display' => true,
          'operation' => 'node_action',
          'description' => t('!Points cost for action: "%action" ', userpoints_translation() + array('%action' => htmlentities($actions[$action]))),
        );
        if ($c->category != 0) {
          $params['description'] .= t("of type %category ", array('%category' => taxonomy_term_load($c->category)->name));
        }
        $params['description'] .= t('on a node of type: "%type" called "%title"', array('%type' => $pubtype, '%title' => $title));
        $result = userpoints_userpointsapi($params);
      }
      if (!empty($result)) {
        $results[] = $result;
      }
    }
  }
  return $results;
}

/**
 * Checks to see if the new action status has previously been set. For instance, if
 * a node was published, then simply edited without changing the publish status, the
 * user shouldn't be charged for it again
 */
function _userpoints_node_action_has_status_changed($nid, $action, $newStatus = 1) {
  // It seems the only reliable way to do this is to reload the node. It could be a direct
  // query to the node table, but node_load would be more resistant to change.
  // If the nid is empty, it's because they're making a new node
  if (empty($nid)) {
    return true;
  }
  $node = node_load($nid);
  
  // Oops? Must be a problem, return true by default;
  if (empty($action) || empty($node)) {
    return true;
  }
  
  // If the statuses are different
  if (isset($node->$action) && $node->$action !== null && $node->$action != $newStatus) {
    return true;
  }
  else {
    return false;
  }
}

/**
 * Helper function for invoking rules
 */
function _userpoints_node_action_invoke_rule($type, $account, $node) {
  if (module_exists('rules')) {
    switch ($type) {
      case "success":
        rules_invoke_event('userpoints_node_action_payment_success', $account, $node);
        break;
      case "failure":
        rules_invoke_event('userpoints_node_action_payment_failure', $account, $node);
        break;
      default:
        break;
    }
  }
}

/**
 * Function to generate delimited strings out of arrays
 */
function _userpoints_node_action_generate_delimited_data($arr) {
  $ret = "";
  if (!empty($arr)) {
    foreach ($arr as $a) {
      $ret .= USERPOINTS_NODE_ACTION_DELIMITER . $a;
    }
    $ret .= USERPOINTS_NODE_ACTION_DELIMITER;
  }
  return $ret;
}
