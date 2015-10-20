<?php

// $Id: 

// Helper functions for userpoints_poll_action.module

define("USERPOINTS_POLL_ACTION_VOTE", 1);

define("USERPOINTS_POLL_ACTION_MESSAGE_START", "<link_text>");
define("USERPOINTS_POLL_ACTION_MESSAGE_MIDDLE", "<end_link>");
define("USERPOINTS_POLL_ACTION_MESSAGE_END", "<end_text>");

define("USERPOINTS_POLL_ACTION_DEFAULT_MESSAGE", "You do not have enough !category !points to !action on this poll");

/**
 * Admin form for new rules
 */
function userpoints_poll_action_admin_add_form() {
  // Get list of roles and other data
  $roles = user_roles(true);
  $actions = _userpoints_poll_action_get_actions();
  $categories = userpoints_get_categories();
  
  // Build the form
  $form['add'] = array(
    '#id' => 'userpoints_poll_action_admin_add_form',
    '#redirect' => 'admin/config/people/userpoints_poll_action',
    '#type' => 'fieldset',
  );
  $form['add']['title'] = array(
    '#markup' => '<h2>' . t('Add Poll Action Rule') . '</h2>',
  );
  $form['add']['role'] = array(
    '#type' => 'select',
    '#required' => true,
    '#title' => t('Role'),
    '#options' => $roles,
  );
  $form['add']['action'] = array(
    '#type' => 'select',
    '#required' => 'true',
    '#title' => t('Action'),
    '#options' => $actions,
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
    '#description' => t('Number of !points it costs to perform the selected action on polls for the selected user role.', userpoints_translation()),
    '#maxlength' => 8,
    '#size' => 8,
    '#element_validate' => array('userpoints_poll_action_validate_points'),
  );
  $form['add']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  $form['#validate'][] = 'userpoints_poll_action_admin_add_form_validate';
  return $form;
}

/**
 * Admin form for the custom message
 */
function userpoints_poll_action_admin_message_form() {
  // Build the form
  $form['message'] = array(
    '#id' => 'userpoints_poll_action_admin_message_form',
    '#redirect' => 'admin/config/people/userpoints_poll_action',
    '#type' => 'fieldset',
  );
  $form['message']['title'] = array(
    '#markup' => '<h2>' . t('Customize message options') . '</h2>',
  );
  $form['message']['customMessage'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom message'),
    '#default_value' => variable_get('userpoints_poll_action_custom_message', ''),
    '#description' => 'Enter a custom message here. You may use "!points" to refer to the correct term, and may use the custom schema to include links in your message that will open in a new window: ' . htmlentities(USERPOINTS_POLL_ACTION_MESSAGE_START) . 'path' . htmlentities(USERPOINTS_POLL_ACTION_MESSAGE_MIDDLE) . 'desc' . htmlentities(USERPOINTS_POLL_ACTION_MESSAGE_END) . ' (the message needs all three key character sets, exactly as shown). For example, the following message: "' . htmlentities(USERPOINTS_POLL_ACTION_MESSAGE_START) . '/store/item' . htmlentities(USERPOINTS_POLL_ACTION_MESSAGE_MIDDLE) . 'click here to buy more' . htmlentities(USERPOINTS_POLL_ACTION_MESSAGE_END) . '" will effectively call the l() function with the text being "click here to buy more" and being a link to the appropriate /store/item location. This message will get displayed once upon form validation, regardless of how many rules are conflicted with. If you do not wish to have a custom message, simply leave this field blank.',
  );
  $message = userpoints_poll_action_translate_message(variable_get('userpoints_poll_action_custom_message', ''));
  if (!empty($message)) {
    $form['message']['customMessageExample'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="form-item"><label>Example of your message: </label><div>',
      '#suffix' => '</div><div class="description">' . t('This is what your custom message will look like when there is a rule conflict.') . '</div>',
      '#markup' => $message,
    );
  }
  $form['message']['displayMessages'] = array(
    '#type' => 'textarea',
    '#title' => t('Override default message'),
    '#default_value' => variable_get('userpoints_poll_action_display_custom_message', USERPOINTS_POLL_ACTION_DEFAULT_MESSAGE),
    '#description' => t("This message displays for each rule that has failed (leave blank for no message). The schema used in the above message can be used, as well as the following variables:<br/>!points - correct term for points<br/>!cur_points - current number of points the user has for the relevant category<br/>!num_points - number of points needed (this includes previous action costing)<br/>!category - category of points (may be a blank string)<br/>!action - action the user is trying to take<br/>The default value of this is \"" . USERPOINTS_POLL_ACTION_DEFAULT_MESSAGE . "\". Note that if you don't display default messages and don't provide any custom messages, it will probably be very confusing for the user as the poll will be invalided without giving a reason."),
  );
  $form['message']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/* This function gets the custom set message */
function userpoints_poll_action_translate_message($string) {
  $string = t($string);
  if (!empty($string)) {
    $matches = array();
    preg_match_all('/(' . USERPOINTS_POLL_ACTION_MESSAGE_START . ')(.*?)(' . USERPOINTS_POLL_ACTION_MESSAGE_MIDDLE . ')(.*?)(' . USERPOINTS_POLL_ACTION_MESSAGE_END . ')/', $string, $matches, PREG_SET_ORDER);
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

function userpoints_poll_action_admin_modify_form() {
  // Get list of roles and actual rules
  $roles = user_roles(true);
  $actions = _userpoints_poll_action_get_actions();
  $categories = userpoints_get_categories();
  $rules = userpoints_poll_action_get_all();
  
  // Build the form
  $form = array(
    '#id' => 'userpoints_poll_action_admin_modify_form',
    '#redirect' => 'admin/config/people/userpoints_poll_action',
  );
  $form['modify'] = array(
    '#type' => 'fieldset',
    '#tree' => true,
  );
  $form['modify']['title'] = array(
    '#markup' => '<h2>' . t('Stored rules') . '</h2>',
  );
  $form['#theme'] = "userpoints_poll_action_admin_modify_form";
  
  // Loop for every stored rule
  $i = 0;
  if (!empty($rules)) {
    foreach ($rules as $rule) {
      $form['modify'][$i]['role'] = array(
        '#type' => 'select',
        '#required' => true,
        '#options' => $roles,
        '#default_value' => $rule['role_id'],
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
        '#element_validate' => array('userpoints_poll_action_validate_points'),
      );
      $form['modify'][$i]['id'] = array(
        '#type' => 'hidden',
        '#value' => $rule['id'],
      );
      $i++;
    }
    
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
function theme_userpoints_poll_action_admin_modify_form($variables) {
  $form = $variables['form'];
  $header = array(t('Role'), t('Action'), t('Category'), t('!Points', userpoints_translation()), '');
  $i = 0;
  $rows = array();
  // Go through all the rows, unset them and turn them into a table
  while (!empty($form['modify'][$i])) {
    $row = array(
      drupal_render($form['modify'][$i]['role']),
      drupal_render($form['modify'][$i]['action']),
      drupal_render($form['modify'][$i]['category']),
      drupal_render($form['modify'][$i]['points']),
      drupal_render($form['modify'][$i]['id']) . l(t('delete'), 'admin/config/people/userpoints_poll_action/delete/' . $form['modify'][$i]['id']['#value']),
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
function userpoints_poll_action_get_all() {
  $result = db_query("SELECT * FROM {userpoints_poll_action}");
  $rules = array();
  foreach ($result as $rule) {
    $rules[] = (array)$rule;
  }
  return $rules;
}

/**
 * Get all poll actions as array, for formatting
 */
function _userpoints_poll_action_get_actions($database = false) {
  // If not for database purposes, get formal names
  if ($database == false) {
    return array(
      USERPOINTS_POLL_ACTION_VOTE => 'Vote',
    );
  }
  else {
    // Otherwise, get databased names
    return array(
      USERPOINTS_POLL_ACTION_VOTE => 'vote',
    );
  }
}

/**
 * Returns an object that holds the cost and category. Since we're only getting one (as well as there
 * should only be one limit per person), multiple roles may cause multiple rules to be applied. If this
 * is the case, the rule that costs the most points will be applied.
 * Passing false as category will get the rules for all categories of points
 */
function _get_userpoints_poll_action_user_cost($user, $action, $category = false) {
  $i = count($user->roles);
  $addRoles = "";
  foreach ($user->roles as $key => $role) {
    $addRoles .= 'role_id = ' . $key;
    $i--;
    if ($i > 0) {
      $addRoles .= ' OR ';
    }
  }
  $q = "SELECT MAX(points) as points, category FROM {userpoints_poll_action} WHERE ($addRoles) AND action = $action";
  if ($category !== false) {
    $q .= " AND category = $category";
  }
  $q .= " GROUP BY category  ORDER BY points ASC ";
  $results = db_query($q);
  $costs = array();
  foreach ($results as $result) {
    $costs[] = $result;
  }
  return $costs;
}

/**
 * Gets a status message based on if the user has enough points
 */
function _get_userpoints_poll_action_status($action, &$totalCost) {
  global $user;
  $results = array();
  $result = null;
  $actions = _userpoints_poll_action_get_actions(true);
  $cost = _get_userpoints_poll_action_user_cost($user, $action);
  
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
        $points = userpoints_get_current_points($user->uid, $cat);
        if ($points < $amount) {
          $message = variable_get('userpoints_poll_action_display_custom_message', USERPOINTS_POLL_ACTION_DEFAULT_MESSAGE);
          if (!empty($message)) {
            $result = userpoints_poll_action_translate_message(t($message, userpoints_translation() + array(
              "!category" => ($cat?taxonomy_term_load($cat)->name:''),
              "!cur_points" => $points,
              "!num_points" => $amount,
              "!action" => $actions[$action],
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
function _userpoints_poll_action_payment($poll, $action) {
  global $user;
  $results = array();
  $result = null;
  $title = $poll->title;
  $actions = _userpoints_poll_action_get_actions();
  $cost = _get_userpoints_poll_action_user_cost($user, $action);
  if (!empty($cost)) {
    foreach ($cost as $c) {
      $points = userpoints_get_current_points($user->uid, $c->category);
      // They don't have enough points, even though we calculated it before...
      if ($points < $c->points) {
        $result = array(
          'status' => false,
          'reason' => t("Not enough !points ", userpoints_translation()),
        );
        if ($c->category != 0) {
          $result['reason'] .= t("of type %category ", array('%category' => taxonomy_term_load($c->category)->name));
        }
        $result['reason'] .= t("to %action on this poll", array('%action' => $actions[$action]));
      }
      else {
        $params = array(
          'uid' => intval($user->uid),
          'points' => ($c->points * -1),
          'tid' => intval($c->category),
          'display' => true,
          'operation' => 'poll_action',
          'description' => t('!Points cost for action: "%action" ', userpoints_translation() + array('%action' => $actions[$action])),
        );
        if ($c->category != 0) {
          $params['description'] .= t("of type %category ", array('%category' => taxonomy_term_load($c->category)->name));
        }
        $params['description'] .= t('on a poll called "%title"', array('%title' => $title));
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
 * Helper function for invoking rules
 */
function _userpoints_poll_action_invoke_rule($type, $account, $poll) {
  if (module_exists('rules')) {
    switch ($type) {
      case "success":
        rules_invoke_event('userpoints_poll_action_payment_success', $account, $poll);
        break;
      case "failure":
        rules_invoke_event('userpoints_poll_action_payment_failure', $account, $poll);
        break;
      case "semi-success":
        rules_invoke_event('userpoints_poll_action_payment_semi-success', $account, $poll);
        break;
      default:
        break;
    }
  }
}
