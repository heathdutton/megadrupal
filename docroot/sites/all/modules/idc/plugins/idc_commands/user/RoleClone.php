<?php

/**
 * @file
 * IDC Implementation to Clone permissions from a role into another one.
 */

$plugin = array(
  'handler class' => 'RoleClone',
);

/**
 * Class RoleClone
 */
class RoleClone extends IDCCommandBase implements IDCInterface {

  public static $commandName = 'role-clone';

  /**
   * @var Boolean Whether the chosen role will be cloned into a new role or not.
   */
  public $clone_into_new_role;

  /**
   * Returns the array that defines the drush command.
   *
   * @return array
   *   The drush command definition.
   */
  public static function getCommandDefinition() {
    return array(
      'description' => t('Clone permissions from one role into another.'),
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
      'choose_source_role',
      'choose_destination_role',
    );
  }

  public function choose_source_role($processed = FALSE) {
    if (!$processed) {
      $prompt = outputColours::getColouredOutput('Choose the role to clone:', 'yellow');
      return new IDCStepSingleChoice($prompt, drupal_map_assoc(user_roles()));
    }
  }

  public function choose_destination_role($processed = FALSE) {
    if (!$processed) {
      $prompt = 'Choose the role where the chosen role permissions will be cloned into:';
      $user_roles = drupal_map_assoc(user_roles());
      // Add special entry to create a new role.
      $user_roles['create-role'] = t('Create a new role');
      return new IDCStepSingleChoice(outputColours::getColouredOutput($prompt, 'yellow'), $user_roles);
    }
    elseif ($this->getStepResults('choose_destination_role') == 'create-role') {
      // Ask the user the name of the new role, and save it as result of this
      // step.
      $prompt = 'Enter the name for the new role';
      $new_role_name = new IDCStepPrompt(outputColours::getColouredOutput($prompt, 'yellow'));
      $this->setStepResults('choose_destination_role', $new_role_name->processStep()->getInputResults());
      $this->clone_into_new_role = TRUE;
    }
  }

  /**
   * Executes any final code that should run after all the command steps.
   */
  public function finishExecution() {
    $source_role = user_role_load_by_name($this->getStepResults('choose_source_role'));
    $source_role_permissions = user_role_permissions(array($source_role->rid => $source_role->rid));
    $source_role_permissions = $source_role_permissions[$source_role->rid];

    $destination_role_name = $this->getStepResults('choose_destination_role');

    // Create new role if needed or load existing one.
    if ($this->clone_into_new_role) {
      // New role.
      $new_role = new stdClass();
      $new_role->name = $destination_role_name;
      user_role_save($new_role);
      $destination_role = $new_role;
    }
    else {
      // For existing roles, reset their permissions.
      $destination_role = user_role_load_by_name($destination_role_name);

      $permissions_list = array();
      foreach (module_invoke_all('permission') as $permission => $permission_info) {
        $permissions_list[] = $permission;
      }
      user_role_revoke_permissions($destination_role->rid, $permissions_list);
    }

    // Finally, save the 'source' permissions into the destination role.
    if (!empty($destination_role)) {
      user_role_grant_permissions($destination_role->rid, array_keys($source_role_permissions));
      $this->printMessage(outputColours::getColouredOutput(shellIcons::$checkmark . " " . "Successfully cloned role", 'green'));
    }
    return;
  }
}
