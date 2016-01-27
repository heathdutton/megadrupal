<?php

/**
 * @file
 * Contains functions for the admin interface of pay-per-node module.
 *
 * This file contains function for building the admin interface: overview
 * of all user's balances, bulk operations, allowed types and more.
 */

/*
 *  paypernode administration interface
 *
 */
function paypernode_admin_overview() {
  $output = '';

  $query = db_select('users', 'u');
  $query->fields('u', array('uid', 'name'));
  $query->join('paypernode_user', 'p', 'u.uid = p.uid');
  $query->groupBy('u.uid');
  $query->groupBy('u.name');
  $query->addExpression('SUM(p.allowed)', 'allowed');
  $query->addExpression('SUM(p.created)', 'created');
  $result = $query->extend('PagerDefault')->limit(30)->execute();

  // Present results
  if ($result->rowCount()) {
    $header = array(t('user'), t('total'), t('created'), t('left'), t('administer'));
    while ($user = $result->fetchObject()) {
      $rows[] = array(
        theme('username', array('account' => $user)),
        $user->allowed,
        $user->created,
        $user->allowed - $user->created,
        l(t('edit'), "user/$user->uid/paypernode")
      );
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
    $output .= theme('pager');
  }
  else {
    $output .= t("No Pay per node data for users");
  }
  return $output;
}

/**
 *  Admin can setup here which types an user can create through pay-per-node system
 *
 */
function paypernode_admin_types() {
  return drupal_get_form('paypernode_admin_types_form');
}

function paypernode_admin_types_form($form_state) {
  $form = array();
  $options=paypernode_get_types(TRUE);
  if (count($options)) {
    $form['type']=array(
      '#title' => t('Select one or more content types'),
      '#tree'  => TRUE,
      '#type' => 'fieldset',
    );
    foreach ($options as $type_system_name => $object) {
      $form['type'][$type_system_name] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($object->name),
        '#default_value' => variable_get('paypernode_type_' . $type_system_name, FALSE),
      );
    }
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  }
  else {
    $form['submit'] = array('#value' => t('No content types available.'));
  }
  return $form;
}

function paypernode_admin_types_form_submit($form, $form_state) {
  $values=isset($form_state['values']['type']) ? $form_state['values']['type'] : array();
  foreach (paypernode_get_types(TRUE) as $value => $name) {
    if (isset($values[$value]) && $values[$value]==1) {
      variable_set('paypernode_type_' . $value, TRUE);
    }
    else {
      variable_set('paypernode_type_' . $value, FALSE);
    }
  }
  drupal_set_message(t('Allowed content types updated.'));
}

/**
 * Bulk account administration
 * Allows to add a number of nodes for a given role
 */
function paypernode_admin_bulk() {
  return drupal_get_form('paypernode_admin_bulk_form');
}

function paypernode_admin_bulk_form($form_state) {
  $ppn_types=paypernode_get_types();
  $options=array();
  $form = array();
  if (count($ppn_types)) {
    foreach ($ppn_types as $ppn_system_name => $ppn_object) {
      $options[$ppn_system_name]=$ppn_object->name;
    }
    $form['bulk']=array(
      '#title' => t('Bulk operations'),
      '#type' => 'fieldset',
    );
    $form['bulk']['role'] = array(
      '#type' => 'radios',
      '#title' => t('Role'),
      '#options' => user_roles(1),
      '#required' => TRUE,
      '#description' => t("User's role to perform the operation."),
    );
    $form['bulk']['node_type'] = array(
      '#type' => 'radios',
      '#title' => t('Node type'),
      '#required' => TRUE,
      '#options' => $options,
      '#description' => t("Node type to perform the operation."),
    );
    $form['bulk']['amount'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#maxlength' => 4,
      '#required' => TRUE,
      '#title' => t('Number of nodes'),
      '#default_value' => 0,
      '#description' => t("Number of nodes to add. This can be negative."),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );
  }
  else {
    $link = l(t('allowed content types'), 'admin/settings/paypernode/allowed');
    $form['submit'] = array(
      '#value' => t('No content types available (see !link)', array('!link' => $link))
    );
  }
  return $form;
}

function paypernode_admin_bulk_form_submit($form, &$form_state) {
  $rid = $form_state['values']['role'];
  $node_type = $form_state['values']['node_type'];
  $amount = $form_state['values']['amount'];

  // Manage exception for "authenticated user", rid = 2, that is not in users_roles table
  if ($rid == 2) {
    $result = db_query("SELECT u.uid, p.allowed, p.created FROM {users} u LEFT JOIN {paypernode_user} p ON u.uid = p.uid AND p.node_type = :node_type WHERE u.uid > 0", array(':node_type' => $node_type));
  }
  else {
    $result = db_query("SELECT u.uid, p.allowed, p.created FROM {users} u INNER JOIN {users_roles} r ON u.uid = r.uid LEFT JOIN {paypernode_user} p ON u.uid = p.uid AND p.node_type = :node_type WHERE r.rid = :rid", array(':node_type' => $node_type, ':rid' => $rid));
  }

  // Update user accounts
  $user_count = 0;
  $node_count = 0;
  while ($user = $result->fetchObject()) {
    if (is_numeric($user->allowed)) {
      $total = $user->allowed + $amount;
      // Check before update for negative balance and allowed >= created
      if ($total >= 0 && $total >= $user->created) {
        paypernode_update_user_balance($user->uid, $node_type, $amount);
        $user_count++;
        $node_count += $amount;
      }
      elseif ($user->created < $user->allowed) {
        // Just set balance to null, allowed = created
        paypernode_update_user_balance($user->uid, $node_type, $user->created, $user->created, 'set');
        $user_count++;
        $node_count += $user->created - $user->allowed;
      } // else nothing to do for this user
    }
    elseif ($amount > 0) {
      // User has no previous balance of that type. Do not allow negative balance
      paypernode_update_user_balance($user->uid, $node_type, $amount, 0, 'set');
      $user_count++;
      $node_count += $amount;
    }
  }
  drupal_set_message(t('Updated %user_count user accounts. Added %node_count nodes.', array('%user_count' => $user_count, '%node_count' => $node_count)));
}

