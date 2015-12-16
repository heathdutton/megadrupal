<?php

/**
 * @file
 * Serves administration pages of unique_comments.
 */

/**
 * Admin main page.
 */

/**
 * Menu callback: Shows the unique comments settings.
 */
function unique_comments_admin_form($form, &$form_state) {

  $content_types = node_type_get_types();
  foreach ($content_types as $ct) {
    $types[$ct->type] = check_plain($ct->name);
  }

  $form['unique_comments_status'] = array(
    '#type' => 'radios',
    '#title' => t('Unique Comments Status'),
    '#default_value' => variable_get('unique_comments_status', 'None'),
    '#options' => drupal_map_assoc(array('None', 'Per Node', 'Site Wide')),
    '#description' => t('Select to ensure originality either: not at all, per node or sitewide!'),
  );

  $form['unique_comments_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Types'),
    '#description' => t('Select which content types Unique Comments will work on. If no content types are ticked, it is equivalent to selecting "None" in the above section.'),
    '#options' => $types,
    '#default_value' => variable_get('unique_comments_content_types', array()),
  );

  return system_settings_form($form);
}

/**
 * Callback for the node form to display all nodes exempted from Unique Comments
 */
function unique_comments_node_list() {
  $header = array(
    array(
      'data' => t('Added'),
      'field' => 'added',
    ),
    array(
      'data' => t('Node Title'),
      'field' => 'nid',
    ),
    array(
      'data' => t('Added by User'),
      'field' => 'uid',
    ),
    array(
      'data' => t('Operations'),
    ),
  );


  $query = db_select('unique_comments', 'uc')
    ->fields('uc', array('ucid', 'nid', 'uid', 'added'))
    ->extend('TableSort')
    ->orderByHeader($header);


  $result = $query->execute();
  $rows = array();

  foreach ($result as $data) {
    $node = node_load($data->nid);
    $rows[] = array(
      format_date($data->added),
      l($node->title, 'node/' . $data->nid),
      l($node->name, 'user/' . $data->uid),
      l(t('delete'), 'admin/config/content/unique_comments/delete/' . $data->ucid));
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('class' => array('unique_comments')),
    'empty' => '<em>' . t('There are currently no nodes exempt from Unique Comments.') . '</em>',
  ));
}

/**
 * Callback to delete a node from being exempt from Unique Comments
 */
function unique_comments_node_exemption_delete($form, &$form_state, $ucid) {
  $query = db_query('SELECT ucid, nid, uid, added from {unique_comments} where ucid = :ucid', array(':ucid' => $ucid));

  $form = array();
  $form['ucid'] = array(
    '#type' => 'value',
    '#value' => $ucid,
  );

  $result = $query->fetchAll();
  if ($result) {
    $output = confirm_form($form,
                  t('Are you sure you want to delete this exemption?'),
                  'admin/config/content/unique_comments/nodes',
                  t('Deleting this will cause node %nid to be affected by Unique Comments again.', array('%nid' => $result[0]->nid)),
                  t('Delete'),
                  t('Cancel'));
  }
  else {
    drupal_set_message(t('Invalid parameters'), 'warning');
    drupal_goto('admin/config/content/unique_comments/nodes');
  }

  return $output;
}

/**
 * Submit function for deleting a nid from the Unique Comments table
 */
function unique_comments_node_exemption_delete_submit($form, &$form_state) {
  db_delete('unique_comments')
    ->condition('ucid', $form_state['values']['ucid'])
    ->execute();

  drupal_set_message(t('The exemption has been deleted.'));
  $form_state['redirect'] = 'admin/config/content/unique_comments/nodes';
}

/**
 * Form callback for adding an exemption for a node from Unique Comments
 */
function unique_comments_node_exemption_add($form, &$form_state) {
  $form['unique_comments_node_exemption_add'] = array(
    '#type' => 'textfield',
    '#title' => t('Add Exemption.'),
    '#description' => t('Add in a Node ID here to create a new exemption from Unique Comments'),
    '#size' => 10,
    '#element_validate' => array('_unique_comments_node_exemption_add_field_validate'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  return $form;
}

/**
 * Form validation callback for adding node to exemptions.
 * This is done to ensure the NID is numeric and that the node exists.
 */
function _unique_comments_node_exemption_add_field_validate($element, &$form_state) {
  if (!is_numeric($element['#value'])) {
    form_error($element, t('Node IDs must be numeric!'));
  }
  $node = node_load($element['#value']);
  if (empty($node)) {
    form_error($element, t("The node doesn't currently exist, please use a valid NID"));
  }
  else {
    $type = node_type_get_type($node);
    if (!in_array($node->type, variable_get('unique_comments_content_types', array(TRUE)), TRUE)) {
      form_error($element, t('The node type "@type" is not currently allowed by Unique Comments', array('@type' => $type->name)));
    }
    if (unique_comments_node_is_disabled($element['#value'])) {
      form_error($element, t('The Node ID selected has already been exempted.'));
    }
  }
}

/**
 * Form submit function to insert the record into the Unique Comments db.
 */
function unique_comments_node_exemption_add_submit($form, &$form_state) {
  global $user;
  db_insert('unique_comments')
    ->fields(array(
      'nid' => $form_state['values']['unique_comments_node_exemption_add'],
      'uid' => $user->uid,
      'added' => REQUEST_TIME,
    ))
    ->execute();
  drupal_set_message(t('Node ID @nid added to the exemption list', array('@nid' => $form_state['values']['unique_comments_node_exemption_add'])));
}
