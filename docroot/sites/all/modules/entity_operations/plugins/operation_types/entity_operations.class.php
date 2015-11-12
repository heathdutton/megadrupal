<?php

/**
 * @file
 * Defines the class for Entity Operation VBO actions.
 * Belongs to the "action" operation type plugin.
 */

class EntityOperationsVBOOperations extends ViewsBulkOperationsBaseOperation {
// TODO: most of this is copied over from VBO: needs work!

  /**
   * Constructs an operation object.
   */
  public function __construct($operationId, $entityType, array $operationInfo, array $adminOptions) {
    parent::__construct($operationId, $entityType, $operationInfo, $adminOptions);

    list(, , $this->entityOperationKey) = explode('::', $operationId);
  }

  /**
   * Contains the options provided by the user in the configuration form.
   *
   * @var array
   */
  public $formOptions = array();

  /**
   * Returns the access bitmask for the operation, used for entity access checks.
   * TODO!
   */
  public function getAccessMask() {
    // Assume edit by default.
    if (!isset($this->operationInfo['behavior'])) {
      $this->operationInfo['behavior'] = array('changes_property');
    }

    $mask = 0;
    if (in_array('views_property', $this->operationInfo['behavior'])) {
      $mask |= VBO_ACCESS_OP_VIEW;
    }
    if (in_array('changes_property', $this->operationInfo['behavior'])) {
      $mask |= VBO_ACCESS_OP_UPDATE;
    }
    if (in_array('creates_property', $this->operationInfo['behavior'])) {
      $mask |= VBO_ACCESS_OP_CREATE;
    }
    if (in_array('deletes_property', $this->operationInfo['behavior'])) {
      $mask |= VBO_ACCESS_OP_DELETE;
    }
    return $mask;
  }

  /**
   * Returns whether the provided account has access to execute the operation.
   * TODO!
   *
   * @param $account
   */
  public function access($account) {
    // Use actions_permissions if enabled.
    if (module_exists('actions_permissions')) {
      $perm = actions_permissions_get_perm($this->operationInfo['label'], $this->operationInfo['key']);
      if (!user_access($perm, $account)) {
        return FALSE;
      }
    }
    // Check against additional permissions.
    if (!empty($this->operationInfo['permissions'])) {
      foreach ($this->operationInfo['permissions'] as $perm) {
        if (!user_access($perm, $account)) {
          return FALSE;
        }
      }
    }
    // Access granted.
    return TRUE;
  }

  /**
   * Returns whether the operation needs the full selected views rows to be
   * passed to execute() as a part of $context.
   */
  public function needsRows() {
    return !empty($this->operationInfo['pass rows']);
  }

  /**
   * Returns the configuration form for the operation.
   * Only called if the operation is declared as configurable.
   *
   * @param $form
   *   The views form.
   * @param $form_state
   *   An array containing the current state of the form.
   * @param $context
   *   An array of related data provided by the caller.
   */
  public function form($form, &$form_state, array $context) {
    $handler_class = $this->operationInfo['handler'];
    $operation_handler = new $handler_class($this->entityType, $this->entityOperationKey);

    // There is no entity to send to the form. Operations that want to use this
    // had better not be relying on it!
    // Get just the form body, with only the bare form elements, as VBO adds the
    // submit button.
    $form_state['entity_operation_form_elements'] = array(
      'form elements' => TRUE,
    );
    $form = $operation_handler->getForm($form, $form_state, $this->entityType, NULL, $this->entityOperationKey);

    return $form;
  }

  /**
   * Validates the configuration form.
   * Only called if the operation is declared as configurable.
   *
   * @param $form
   *   The views form.
   * @param $form_state
   *   An array containing the current state of the form.
   */
  public function formValidate($form, &$form_state) {
    // TODO.
  }

  /**
   * Handles the submitted configuration form.
   * This is where the operation can transform and store the submitted data.
   * Only called if the operation is declared as configurable.
   *
   * @param $form
   *   The views form.
   * @param $form_state
   *   An array containing the current state of the form.
   */
  public function formSubmit($form, &$form_state) {
    $handler_class = $this->operationInfo['handler'];
    $operation_handler = new $handler_class($this->entityType, $this->entityOperationKey);

    // Hand over to the operation handler.
    $this->formOptions = $operation_handler->formSubmitGetParameters($form, $form_state, $this->entityType, NULL, $this->entityOperationKey);
  }

  /**
   * Executes the selected operation on the provided data.
   *
   * @param $data
   *   The data to to operate on. An entity or an array of entities.
   * @param $context
   *   An array of related data (selected views rows, etc).
   */
  public function execute($data, array $context) {
    $handler_class = $this->operationInfo['handler'];
    $operation_handler = new $handler_class($this->entityType, $this->entityOperationKey);

    $data = is_array($data) ? $data : array($data);
    foreach ($data as $entity) {
      // In a bulk context we haven't yet checked access to the operation makes
      // sense.
      $operation_access = $operation_handler->operationAccess($this->entityType, $entity);
      // Skip entities whose operation access denies access.
      if ($operation_access === FALSE) {
        continue;
      }

      // We always call execute, even if it doesn't do anything.
      $operation_handler->execute($this->entityType, $entity, $context);
    }
  }

}
