<?php
/**
 * @file
 * IDC Implementation to grant / revoke permissions to any desired roles.
 */

$plugin = array(
  'handler class' => 'RolePermissions',
);

/**
 * Class RolePermissions
 */
class RolePermissions extends IDCCommandBase implements IDCInterface {

  public static $commandName = 'role-permissions';

  public static function getCommandDefinition() {
    return array(
      'description' => 'Allows the caller to grant any chosen permissions to one or more role.',
    );
  }

  public function getCommandSteps() {
    return array(
      'choose_operation',
      'choose_module',
      'choose_perms',
      'choose_roles',
    );
  }

  public function choose_operation($processed = FALSE) {
    if (!$processed) {
      return new IDCStepSingleChoice('Do you want to grant, or revoke permissions?', drupal_map_assoc(array('grant', 'revoke')));
    }
  }

  public function choose_module($processed = FALSE) {
    $prompt_label = 'Choose the module whose permissions you want to ' . $this->getStepResults('choose_operation');

    if (!$processed) {
      $module_list = module_list(FALSE, FALSE, TRUE);
      $modules_with_permissions = array();
      foreach ($module_list as $module) {
        if (module_hook($module, 'permission')) {
          $modules_with_permissions[$module] = $module;
        }
      }
      return new IDCStepSingleChoice($prompt_label, $modules_with_permissions);
    }
  }

  public function choose_perms($processed = FALSE) {
    if (!$processed) {
      $module_chosen = $this->getStepResults('choose_module');
      $permissions = module_invoke($module_chosen, 'permission');
      $step = new IDCStepMultipleChoice('Choose the permissions you want to ' . $this->getStepResults('choose_operation'), drupal_map_assoc(array_keys($permissions)));
      return $step;
    }
  }

  public function choose_roles($processed = FALSE) {
    if (!$processed) {
      $all_results = $this->getStepResults();
      $prompt = ($all_results['choose_operation'] == 'grant')
        ? 'Choose the roles you want to grant the permissions to'
        : 'Choose the roles you want to revoke the permissions from';
      // Step label and options.
      $step = new IDCStepMultipleChoice($prompt, drupal_map_assoc(user_roles()));
      return $step;
    }
  }

  public function finishExecution() {
    $command_results = $this->getStepResults();
    $permissions_chosen = $command_results['choose_perms'];
    $roles_chosen = $command_results['choose_roles'];
    // Get the function to use to grant / revoke permissions.
    $function = ($command_results['choose_operation'] == 'grant') ? 'user_role_grant_permissions' : 'user_role_revoke_permissions';
    // Verb to use for the console output.
    $verb = ($command_results['choose_operation'] == 'grant') ? 'granted' : 'revoked';
    $user_roles = user_roles();
    foreach ($roles_chosen as $role_name) {
      try {
        $rid = array_search($role_name, $user_roles);
        $function($rid, $permissions_chosen);
        $this->log(outputColours::getColouredOutput(shellIcons::$checkmark . " " .  'Permissions ' . $verb . ' for "' . $role_name . ' [rid: ' . $rid . ']" role.', 'green') , 'ok');
      }
      catch (Exception $e) {
        $this->terminate('Permissions could not be changed. Execution interrupted.');
        return;
      }
    }
    $this->log('Finished execution.', 'ok');
  }
}
