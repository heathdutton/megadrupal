<?php
// $Id$

/**
 * @file
 *
 * Ctools plugin to provide a uc product context.
 */

/**
 * Implementation of specially named hook_ctools_contexts().
 */
function uc_panels_uc_ctools_contexts() {
  $args['uc'] = array(
    'title' => t("Ubercart Product"),
    'description' => t('A node object that is flagged as an Ubercart Product.'),
    'context' => 'uc_panels_context_create_group',
    'settings form' => 'uc_panels_context_group_settings_form',
    'settings form validate' => 'uc_panels_context_group_settings_form_validate',
    'settings form submit' => 'ctools_context_node_settings_form_submit',
    'keyword' => 'uc',
    'context name' => 'uc',
    'placeholder form' => array(
      '#type' => 'textfield',
      '#description' => t('Enter the node ID of a group for this argument'),
    ),
  );
  return $args;
}


/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function uc_panels_context_create_group($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('uc');
  $context->plugin = 'uc';

  if ($empty) {
    return $context;
  }

  if ($conf) {
    $data = node_load($data['nid']);
  }

  if (!empty($data)) {
    // uc_set_group_context($data);
    $context->data     = $data;
    $context->title    = $data->title;
    $context->argument = $data->nid;
    return $context;
  }
}

function uc_panels_context_group_settings_form($conf, $external = FALSE) {
  if ($external) {
    $form['external'] = array(
      '#type' => 'checkbox',
      '#default_value' => $conf['external'],
      '#title' => t('Require this context from an external source (such as containing panel page).'),
      '#description' => t('If selected, product selection (below) will be ignored.'),
    );
  }


  $form['node'] = array(
    '#prefix' => '<div class="no-float">',
    '#suffix' => '</div>',
    '#title' => t('Enter the NID of a product'),
    '#type' => 'textfield',
    '#maxlength' => 512,
    '#autocomplete_path' => 'panels/node/autocomplete',
    '#weight' => -10,
  );

  if (!empty($conf['nid'])) {
    $info = db_fetch_object(db_query("SELECT * FROM {node} n WHERE n.nid = :n.nid", array(':n.nid' => $conf['nid'])));
    if ($info) {
      $form['node']['#description'] = t('Currently set to "%title"', array('%title' => $info->title));
    }
  }

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $conf['nid'],
  );

  return $form;
}

/**
 * Validate a node.
 */
function uc_panels_context_group_settings_form_validate($form, &$form_values, &$form_state) {
  // Validate the autocomplete
  if (empty($form_values['external']) && empty($form_values['nid']) && empty($form_values['node'])) {
    form_error($form['node'], t('You must select a node.'));
    return;
  }

  if (empty($form_values['node'])) {
    return;
  }

  $nid = $form_values['node'];
  $preg_matches = array();
  $match = preg_match('/\[nid: (\d+)\]/', $nid, $preg_matches);
  if (!$match) {
    $match = preg_match('/^nid: (\d+)/', $nid, $preg_matches);
  }

  if ($match) {
    $nid = $preg_matches[1];
  }
  if (is_numeric($nid)) {
    // TODO Please convert this statement to the D7 database API syntax.
    $node = db_fetch_object(db_query(db_rewrite_sql("SELECT n.nid, n.type FROM {node} n WHERE n.nid = %d"), $nid));
  }
  else {
    // TODO Please convert this statement to the D7 database API syntax.
    $node = db_fetch_object(db_query(db_rewrite_sql("SELECT n.nid, n.type FROM {node} n WHERE LOWER(n.title) = LOWER('%s')"), $nid));
  }

  if (!$node) {
    form_error($form['node'], t('Invalid group selected.'));
  }
  elseif (!uc_product_is_product($node->type)) {
    form_error($form['node'], t('Node is of type %type which not a product type.', array('%type' => $node->type)));
  }
  else {
    form_set_value($form['nid'], $node->nid, $form_state);
  }

}
