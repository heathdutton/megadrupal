<?php

/**
 * @file
 * IDC Implementation to Create / Delete / Edit (Rename) any desired roles.
 */

$plugin = array(
  'handler class' => 'Role',
);

/**
 * Class Role
 */
class Role extends IDCCommandBase {

  public static $commandName = 'role';

  /**
   * @var String new name for an updated role.
   */
  public $updated_role_name;

  /**
   * Returns the array that defines the drush command.
   *
   * @return array
   *   The drush command definition.
   */
  public static function getCommandDefinition() {
    return array(
      'description' => 'Allows the caller to grant any chosen permissions to one or more role.',
    );
  }

  /**
   * Returns the array that defines the steps of the drush command.
   *
   * Each step is a string that matches an existing method in the IDC command
   * class.
   *
   * @return array
   *   An indexed array containing the drush command steps.
   */
  public function getCommandSteps() {
    return array(
      'choose_operation',
      'choose_role',
    );
  }

  public function choose_operation($processed = FALSE) {
    if (!$processed) {
      return new IDCStepSingleChoice('Do you want to Create, Edit, or Delete a role?', drupal_map_assoc(array('create', 'edit', 'delete')));
    }
  }

  public function choose_role($processed = FALSE) {
    // Information not entered yet.
    $operation = $this->getStepResults('choose_operation');
    if (!$processed) {
      if ($operation == 'create') {
        $prompt = 'Enter the name of the role to be created';
        return new IDCStepPromptCSVDecorator($prompt);
      }
      else {
        $prompt = 'Choose the role(s) to ' . $operation;
        return new IDCStepMultipleChoice($prompt, drupal_map_assoc(user_roles()));
      }
    }
    // Input entered, and operation was 'edit'.
    else if ($operation == 'edit' && ($this->getStepResults('choose_role') != NULL)) {
      $prompt = 'Enter the new name for the role';
      $new_name_step = new IDCStepPrompt($prompt, $this->getStepResults('choose_role'));
      $this->updated_role_name = $new_name_step->processStep()->getInputResults();
    }
  }

  /**
   * Executes any final code that should run after all the command steps.
   */
  public function finishExecution() {
    $operation = $this->getStepResults('choose_operation');

    switch ($operation) {
      case 'create':
        foreach ($this->getStepResults('choose_role') as $role_name) {
          $role = new stdClass();
          $role->name = $role_name;
          user_role_save($role);
        }
        break;
      case 'edit':
        $role_name = $this->getStepResults('choose_role');
        $existing_role = user_role_load_by_name($role_name);
        $existing_role->name = $this->updated_role_name;
        user_role_save($existing_role);
        break;
      case 'delete':
        foreach ($this->getStepResults('choose_role') as $role_name) {
          user_role_delete($role_name);
        }
        break;
    }
  }
}
