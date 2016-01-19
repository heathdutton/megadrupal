<?php

/**
 * @file
 * Basic admin forms, validators, and submit handlers.
 */

/**
 * Form constructor for the PMPAPI groups list form.
 *
 * @ingroup forms
 */

function pmpapi_groups_list() {
  $group_list = pmpapi_fetch_groups();
  if ($group_list) {
    $edit_base = 'admin/config/services/pmp/groups/edit/';
    $delete_base = 'admin/config/services/pmp/groups/delete/';

    $vars['header'] = array(
      'title' => array(
        'data' => t('Group'),
      ),
      'guid' => array(
        'data' => t('Group GUID'),
      ),
      'edit' => array(
        'data' => t('Edit'),
      ),
      'delete' => array(
        'data' => t('Delete'),
      ),
    );

    $groups = array();
    $form = array();
    foreach ($group_list as $group) {
      $title = check_plain($group->attributes->title);
      $guid = $group->attributes->guid;
      $edit = l('Edit', $edit_base . $guid);
      $delete = l('Delete', $delete_base . $guid);
      $groups[] =  array($title, $guid, $edit, $delete);
    }

    $vars['rows'] = $groups;
    return theme('table', $vars);
  }
  else {
    $form['pmpapi_groups_list_none'] = array(
      '#markup' => t('Unable to get a list of PMP groups.'),
    );
  }
  return $form;
}

/**
 * Form constructor for the PMPAPI edit groups form.
 *
 * @see pmpapi_groups_edit_form_submit()
 *
 * @ingroup forms
 */
function pmpapi_groups_edit_form($form, &$form_state, $guid = '') {
  if ($guid) {
    $group = pmpapi_fetch_group($guid);
    $form['guid'] = array(
      '#type' => 'hidden',
      '#value' => $guid,
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Group name'),
    '#default_value' => !empty($group->attributes->title) ? $group->attributes->title : '',
    '#disabled' => (boolean) $guid,
    '#required' => TRUE,
  );
  
  $pmp = new PMPAPIDrupal();
  $pmp->pull(array('limit' => 1000, 'profile'=>'organization', 'searchsort' => 'title'));
  $possible_orgs = array();
  foreach($pmp->query->results->docs as $doc) {
    $possible_orgs[$doc->attributes->guid] = $doc->attributes->title;
  }
  
  $selected_orgs = array();
  if (!empty($group->items)) {
    foreach($group->items as $item) {
      $selected_orgs[] = $item->attributes->guid;
    }
  }
  
  $form['orgs'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#required' => TRUE,
    '#title' => t('Add organizations'),
    '#options' => $possible_orgs,
    '#default_value' => $selected_orgs,
  );
  
  // Submit Button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Group'),
  );

  return $form;
}

/**
 * Form submission handler for pmpapi_groups_edit_form().
 */
function pmpapi_groups_edit_form_submit($form, &$form_state) {
  $values = array(
    'profile' => 'group',
    'attributes' => array(
      'title' => $form_state['values']['name'],
    ),
  );

  if (!empty($form_state['values']['guid'])) {
    $values['attributes']['guid'] = $form_state['values']['guid'];
  }

  foreach($form_state['values']['orgs'] as $org) {
    $item = new stdClass();
    $item->href = variable_get('pmpapi_base_url') . '/docs/' . $org;
    $values['items'][] = $item;
  }
  if (pmpapi_send($values)) {
    $transaction = (!empty($form_state['values']['guid'])) ? t('edited') : t('created');
    $message = t('Successfully !transaction the group: @group', array('!transaction' => $transaction, '@group' => $form_state['values']['name']));
    drupal_set_message($message);
  }
  else {
    drupal_set_message(t('Error sending group data to the PMP.'), 'warning');
  }
  $form_state['redirect'] = 'admin/config/services/pmp/groups';
}

/**
 * Form constructor for the PMPAPI groups delete form.
 *
 * @see pmpapi_groups_delete_form_submit()
 *
 * @ingroup forms
 */
function pmpapi_groups_delete_form($form, &$form_state, $guid) {
  $question = t('Are you sure you want to delete the group with guid: @guid?', array('@guid' => $guid));
  return confirm_form(array(), $question, 'admin/config/services/pmp/groups/edit/' . $guid);
}

/**
 * Form submission handler for pmpapi_groups_delete_form().
 */
function pmpapi_groups_delete_form_submit($form, &$form_state) {
  $guid = arg(6);
  pmpapi_remove($guid);
  $form_state['redirect'] = 'admin/config/services/pmp/groups';
}