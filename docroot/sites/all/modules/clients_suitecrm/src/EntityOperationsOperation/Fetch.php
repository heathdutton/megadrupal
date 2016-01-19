<?php
/**
 * @file
 * Generic entity form for suiteCRM entities.
 */

namespace Drupal\clients_suitecrm\EntityOperationsOperation;

/**
 * Op handler for fetching a remote SuiteCRM entity.
 */
class Fetch extends \EntityOperationsOperationForm {

  /**
   * Returns basic information about the operation.
   */
  public function operationInfo() {
    return array(
      'label' => 'Fetch',
      'description' => 'Fetches a remote entity .',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   */
  public function operationStrings() {
    return array(
      'tab title' => 'Fetch from remote',
      'label' => 'Fetch from remote',
      'description' => 'Produces a form to fetch a remote suiteCRM entity.',
      'page title' => t('Fetch remote entity'),
      'button label' => t('Fetch now'),
      'confirm question' => NULL,
      'submit message' => t('The @entity-type %label has been fetched.'),
    );
  }

  /**
   * Properties for the menu item specific to this handler.
   */
  public function menu_item($operation_path, $operation_definition, $loader_position) {
    $entity_info = entity_get_info($this->entityType);
    $form_id = $this->getFormID($this->entityType, $operation_path);
    return array(
      'path' => $entity_info['admin ui']['path'] . '/fetch',
      'type' => MENU_LOCAL_ACTION,
      'page callback' => 'drupal_get_form',
      'page arguments' => array(
        $form_id,
        $this->entityType,
        get_class($this),
        '',
        $operation_path,
      ),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
    );
  }

  /**
   * Get the form ID for the operation form.
   *
   * @param string $entity_type
   *   The entity type.
   * @param string $operation_path
   *   The path component for the operation.
   *
   * @return string
   *   A form ID that will be recognized by entity_operations_forms().
   */
  public function getFormID($entity_type, $operation_path) {
    // Force the form ID, so that child classes get the same one. This means
    // both the add and fetch generic operation forms get the same form ID, and
    // thus makes form alteration easier.
    return implode('_', array(
      'entity_operations_operation_form',
      $entity_type,
      'fetch',
    ));
  }

  /**
   * Form builder for this operation.
   */
  public function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Kill the cancel link.
    unset($form_state['entity_operation_form_elements']['cancel link']);

    return $this->entityForm($form, $form_state, $entity_type, $entity, $operation_path, 'fetch');
  }

  /**
   * Helper for form(), building just the entity form.
   *
   * Keeps the entity form separate from any wrapping logic and allows clearer
   * reuse by subclasses.
   *
   * Note that overriding this will cause inheritance problems because this is
   * also used by the EntityOperationsOperationAddGeneric handler. If you want
   * to customize the form for your entity, you may want to consider using
   * the EntityOperationsOperationEdit handler instead.
   *
   * @param ...
   *   The same parameters as form().
   * @param string $form_op
   *   The 'op' value to set on the entity form state. Either 'edit' or 'add'.
   */
  public function entityForm($form, &$form_state, $entity_type, $entity, $operation_path, $form_op) {

    $form['remote_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Remote identifier'),
      '#description' => t('The record id on the remote system'),
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Form validation handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  public function formValidate($form, &$form_state, $entity_type, $entity, $operation_path) {
    if (!($form_state['#remote_entity'] = remote_entity_load_by_remote_id($this->entityType, $form_state['values']['remote_id']))) {
      form_set_error('remote_id', t('Unable to load @remote_entity', array('@remote_entity' => $form_state['values']['remote_id'])));
    }
  }

  /**
   * Form submit handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  public function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {

    if (!empty($form_state['#remote_entity'])) {
      drupal_set_message(t('Sucessfuly fetched and stored'));
    }

    // Redirect to the entity.
    if (empty($form_state['redirect']) && !empty($form_state['#remote_entity'])) {
      $form_state['redirect'] = $this->getFormSubmitRedirect($this->entityType, $form_state['#remote_entity']);
    }
  }

}
