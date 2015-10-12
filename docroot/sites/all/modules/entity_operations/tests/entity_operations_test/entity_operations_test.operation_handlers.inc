<?php

/**
 * @file
 * Contains handler classes for test operations.
 */

/**
 * Test op handler: sets the entity title to 'Red'.
 */
class EntityOperationsTestOperationTitleRed extends EntityOperationsOperationAction {

  public $access_verb = 'edit';

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Make red',
      'description' => 'Sets the title.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Red',
      'page title' => "Set title to 'Red'",
      'button label' => t('Make it red'),
      'confirm question' => t('Are you sure you want to set the @entity-type %label to red?'),
      'submit message' => t('The @entity-type %label has been made red.'),
    );
  }

  /**
   * The red action.
   */
  function execute($entity_type, $entity, $parameters = array()) {
    $entity->title = 'Red';
    $entity->save();
  }

}

/**
 * Test op handler: sets the entity title to 'Blue'.
 */
class EntityOperationsTestOperationTitleBlue extends EntityOperationsOperationAction {

  public $access_verb = 'edit';

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Make blue',
      'description' => 'Sets the title.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Blue',
      'page title' => "Set title to 'Blue'",
      'button label' => t('Make it blue'),
      'confirm question' => t('Are you sure you want to set the @entity-type %label to blue?'),
      'submit message' => t('The @entity-type %label has been made blue.'),
    );
  }

  /**
   * The blue action.
   */
  function execute($entity_type, $entity, $parameters = array()) {
    $entity->title = 'Blue';
    $entity->save();
  }

}

/**
 * Test op handler: sets the entity title to 'Green'.
 */
class EntityOperationsTestOperationTitleGreen extends EntityOperationsOperationAction {

  public $access_verb = 'edit';

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Make Green',
      'description' => 'Sets the title.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Green',
      'page title' => "Set title to 'Green'",
      'button label' => t('Make it green'),
      'confirm question' => t('Are you sure you want to set the @entity-type %label to Green?'),
      'submit message' => t('The @entity-type %label has been made green.'),
    );
  }

  /**
   * The green action.
   */
  function execute($entity_type, $entity, $parameters = array()) {
    $entity->title = 'Green';
    $entity->save();
  }

}

/**
 * Test op handler for testing form validation. Requires a password to execute.
 *
 * Hint: the password is 'swordfish'.
 *
 * See https://www.youtube.com/watch?v=W50L4UPfWsg
 */
class EntityOperationsTestOperationSwordfish extends EntityOperationsOperationAction {

  public $access_verb = 'edit';

  /**
   * Returns basic information about the operation.
   */
  function operationInfo() {
    return array(
      'label' => 'Swordfish',
      'description' => 'Verify the password.',
    ) + parent::operationInfo();
  }

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Swordfish',
      'page title' => "Swordfish?",
      'button label' => t('Swordfish?'),
      'confirm question' => t("Do you know the password is 'Swordfish'?"),
      'submit message' => t("You guess it."),
    );
  }

  /**
   * Form builder for this operation.
   */
  function form($form, &$form_state, $entity_type, $entity, $operation_path) {
    $form = array();

    $form['password'] = array(
      '#type' => 'textfield',
      '#title' => t("What's the password?"),
    );

    return $form;
  }

  /**
   * Form validation handler for this operation.
   *
   * Receives the same parameters as the form builder as a convenience.
   */
  function formValidate($form, &$form_state, $entity_type, $entity, $operation_path) {
    if ($form_state['values']['password'] != 'swordfish') {
      form_set_error('password', t("'At's-a no fish!"));
    }
  }

  /**
   * The Swordfish action.
   */
  function execute($entity_type, $entity, $parameters = array()) {
    $entity->title = 'Swordfish';
    $entity->save();
  }

}
