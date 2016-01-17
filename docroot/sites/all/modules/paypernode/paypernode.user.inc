<?php

/**
 * @file
 * Contains functions for the user interface of pay-per-node module.
 *
 * This file contains function for building user interface and administering
 * user pay-per-node balance.
 */

/**
 * User balance
 */
function paypernode_user_page($current_user) {
  if (user_access('administer pay-per-node system')) {
    return paypernode_user_admin($current_user->uid);
  }
  else {
    return paypernode_user_balance($current_user->uid);
  }
}
/**
 * Display user balance, not editable
 */
function paypernode_user_balance($uid) {
  drupal_set_title(t('Your pay per node balance'));
  $balance = paypernode_user_get_balance($uid);
  $rows = array();
  if (count($balance)) {
    $nodetypes = paypernode_get_types();

    $header = array(t('Type'), t('Purchased'), t('Created'), t('Available'), '');
    foreach ($balance as $type => $data) {
      if (!empty($nodetypes[$type]) && $data->allowed>0) {
        $parts=explode(':', $type);
        $parts[0]=str_replace('_', '-', $parts[0]);
        $url_to_create=implode('/', $parts);
        $rows[] = array(
          $nodetypes[$type]->name,
          $data->allowed,
          $data->created,
          $left = $data->allowed - $data->created,
          $left ? l(t('create'), sprintf("node/add/%s", $url_to_create) ) : '--'
          );
      }
    }
  }
  if ($rows) {

    $output = '<h2>' . t('Pay per node balance') . '</h2>';
    $output .= theme('table', array('header' => $header, 'rows' => $rows ));

    $created = db_query('SELECT title, created, changed, n.nid, type, pt.node_type as ppn_type FROM {node} n INNER JOIN {paypernode_tracker} pt ON n.nid=pt.nid WHERE uid=:uid', array(':uid' => $uid));

    $header = array(t('Type'), t('Title'), t('Created'), t('Changed'), '');
    $rows=array();
    while ($created_node = $created->fetchObject()) {
      if (isset($nodetypes[$created_node->ppn_type])) {
        $rows[] = array(
          $nodetypes[$created_node->ppn_type]->name,
          $created_node->title,
          format_date($created_node->created, 'small'),
          format_date($created_node->changed, 'small'),
          l(t('modify'), "node/$created_node->nid/edit", array('query' => array('destination' => 'user/' . $uid . '/paypernode')))
        );
      }
    }
    if (count($rows)) {
      $output .= '<h2>' . t('Your pay per node contents') . '</h2>';
      $output .= theme('table', array('header' => $header, 'rows' => $rows ));
    }
  }
  else {
    $output = t('You have not purchased any content type');
  }
  return $output;
}

/**
 * Admin interface for single user
 */
function paypernode_user_admin($uid) {
  return drupal_get_form('paypernode_user_admin_form', $uid);
}

function paypernode_user_admin_form($form, &$form_state, $uid) {
  $account = user_load($uid);
  drupal_set_title(t('Pay per node balance for user !user_name', array('!user_name' => $account->name)));
  $balance = paypernode_user_get_balance($uid);

  // Build table and form together
  $thead = '<table>';
  $header = array(t('Content type'), t('Purchased'), t('Created'), t('Available'), t('Last updated'));
  $thead .= ' <thead><tr>';
  foreach ($header as $cell) {
    $thead .= _theme_table_cell($cell, 1);
  }
  $thead .= " </tr></thead>\n";
  $thead .= "<tbody>\n";

  $form['uid'] = array('#type' => 'value', '#value' => $uid);
  $form['table_head'] = array('#markup' => $thead);
  $trclass = 'odd'; // row type
  foreach (paypernode_get_types() as $type => $object) {
    $form[$type] = array(
      '#value'  => '',
      '#tree'   => TRUE,
      '#prefix' => "<tr class=\"$trclass\">",
      '#suffix' => '</tr>'
    );
    $allowed = isset($balance[$type]) ? $balance[$type]->allowed : 0;
    $created = isset($balance[$type]) ? $balance[$type]->created : 0;

    $form[$type]['name'] =  array(
      '#type' => 'item',
      '#markup' => $object->name,
      '#prefix' => '<td>',
      '#suffix' => '</td>'
    );
    $form[$type]['allowed'] =  array(
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $allowed,
      '#prefix' => '<td>',
      '#suffix' => '</td>'
    );
    $form[$type]['created'] =  array(
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $created,
      '#prefix' => '<td>',
      '#suffix' => '</td>'
    );
    $form[$type]['available'] =  array(
      '#type' => 'item',
      '#markup' => $allowed - $created,
      '#prefix' => '<td>',
      '#suffix' => '</td>'
    );

    $form[$type]['changed'] =  array(
      '#type' => 'item',
      '#markup' => isset($balance[$type]) ? format_date($balance[$type]->changed) : '--',
      '#prefix' => '<td>',
      '#suffix' => '</td>'
    );

    $trclass = ($trclass == 'even') ? 'odd' : 'even';
  }
  $form['table_end'] = array('#markup' => "</tbody>\n</table>\n");
  $form['submit'] = array('#type' => 'submit', '#value' => t('Update'));

  return $form;
}

function paypernode_user_admin_form_submit(&$form_id, &$form_state) {
  $uid = $form_state['values']['uid'];
  foreach (paypernode_get_types() as $type => $name) {
    if (isset($form_state['values'][$type]) && is_numeric($allowed = $form_state['values'][$type]['allowed']) && is_numeric($created = $form_state['values'][$type]['created'])) {
      paypernode_update_user_balance($uid, $type, $allowed, $created, 'set');
    }
  }
  drupal_set_message(t('User account has been updated'));
}
