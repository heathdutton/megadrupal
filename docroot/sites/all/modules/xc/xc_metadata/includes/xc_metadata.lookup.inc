<?php
/**
 * @file
 * XC Metadata lookup functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_metadata_lookup_form($form, &$form_state) {
  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instructions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['instructions']['how'] = array(
    '#type' => 'item',
    '#title' => t('How it works'),
    '#value' => t('This form allows an administrator to lookup a metadata entity by its Metadata ID, Identifier (such as OAI Identifier), or Node ID and, if desired, be directed to its node page.'),
  );

  $form['lookup']['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifier'),
    '#required' => TRUE,
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Identifier Type'),
    '#options' => array(
      'metadata_id' => t('Metadata ID'),
      'identifier' => t('Identifier'),
      'node_id' => t('Node ID'),
    ),
    '#default_value' => 'metadata_id',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Lookup'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_metadata_lookup_form_submit($form, &$form_state) {
  $type = $form_state['values']['type'];
  $id   = trim($form_state['values']['identifier']);

  $entity = new XCEntity(array($type => $id));
  if ($nid = $entity->node_id) {
    $form_state['redirect'] = 'node/' . $nid;
  }
  else {
    drupal_set_message(t('Cannot find requested metadata.'), 'error');
  }
}
