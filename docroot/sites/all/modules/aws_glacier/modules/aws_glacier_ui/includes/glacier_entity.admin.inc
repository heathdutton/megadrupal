<?php

use Drupal\aws_glacier\Entity\Vault\Vault;
use Drupal\aws_glacier\Exception\VaultException;

/**
 * Form to add new vault.
 */
function glacier_vault_form($form, &$form_state, Vault $entity) {
  $form['#entity'] = $entity;

  $entity_type = $entity->entityType();

  $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);

  $property = $entity->uniqueProperty;

  $info = $entity_wrapper->$property->info();
  $form[$property] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => $info['label'],
    '#default_value' => $entity->$property,
    '#description' => $info['description'],
    '#disabled' => !empty($entity->$property),
  );

  field_attach_form($entity_type, $entity, $form, $form_state);

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // Show Delete button if we edit a item.
  $entity_id = entity_id($entity_type, $entity);
  if (!empty($entity_id) && entity_access('delete', $entity_type, $entity)) {
    $entity_uri = entity_uri($entity_type, $entity);
    $form['actions']['delete'] = array(
      '#markup' => l(t('Delete'), $entity_uri['path'] . '/delete'),
    );
  }
  return $form;
}

/**
 * Validates the unique vault name.
 *
 * Validate handler of glacier_vault_form().
 */
function glacier_vault_form_validate($form, &$form_state) {
  /** @var $entity Vault */
  $entity = $form['#entity'];
  $value = $form_state['values'][$entity->uniqueProperty];
  $entity->{$entity->uniqueProperty} = $value;
  try{
    $entity->validateUniqueProperty();
    $form['#entity'] = $entity;
  }
  catch (VaultException $e) {
    form_set_error($entity->uniqueProperty, $e->getMessage());
    return;
  }
  catch (\Exception $e) {
    form_set_error('', $e->getMessage());
    return;
  }
}

/**
 * Submit handler of glacier_vault_form().
 */
function glacier_vault_form_submit($form, &$form_state) {
  /** @var $entity Vault */
  $entity = $form['#entity'];
  $entity_type = $entity->entityType();
  try{
    entity_form_submit_build_entity($entity_type, $entity, $form, $form_state);
    $entity->save();
  }
  catch (\Exception $e) {
    form_set_error('', t('Could not save vault.'));
    watchdog_exception('aws_glacier', $e);
    return;
  }
  $entity_uri = entity_uri($entity_type, $entity);
  $form_state['redirect'] = $entity_uri['path'];

  drupal_set_message(t('%title saved.', array('%title' => entity_label($entity_type, $entity))));
}

/**
 * Form to import vaults from amazon.
 */
function aws_glacier_vault_import_form($form) {
  $form['limit'] = array(
    '#title' => t('Limit'),
    '#description' => t('Enter a limit of vaults to import. Leave empty to import all vaults.'),
    '#type' => 'textfield',
    '#size' => 5,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

use \Drupal\aws_glacier\Entity\Vault\Import;
/**
 * Submit handler for aws_glacier_vault_import_form().
 */
function aws_glacier_vault_import_form_submit($form, $form_state) {
  $import = new Import();
  try{
    $import->setLimit($form_state['values']['limit'])->run();
  }
  catch(\Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}


