<?php
/**
 * @file
 * IDC Implementation to a list of all IDC commands available and execute one.
 */

$plugin = array(
  'handler class' => 'IDCList',
);

/**
 * Class IDCList
 */
class IDCList extends IDCCommandBase implements IDCInterface {

  /**
   * @var string Drush command name.
   */
  public static $commandName = 'interactive-drush-commands';

  /**
   * Returns the Drush command array.
   *
   * @return array
   *   The Drush command array.
   */
  public static function getCommandDefinition() {
    return array(
      'description' => 'Shows a list of available IDC commands and allows to execute one of them.',
      'aliases' => array('idc'),
      'examples' => array(
        'drush idc' => 'Shows a list of the available IDC commands so the user can execute any of them.',
      ),
    );
  }

  /**
   * Returns the steps of this command.
   *
   * @return array
   *   The command steps.
   */
  public function getCommandSteps() {
    return array(
      'show_idc_commands_list',
    );
  }

  /**
   * Shows a list of available IDC commands and allows to execute one of them.
   *
   * @param bool $processed
   * @return IDCStepSingleChoice
   */
  public function show_idc_commands_list($processed = FALSE) {
    if ($processed) {
      $command_chosen = $this->getStepResults('show_idc_commands_list');
      if ($command_chosen) {
        drush_invoke($command_chosen);
      }
    }
    // Get all available IDC commands.
    $idc_commands = idc_get_drush_commands();
    $command_list = array();
    foreach ($idc_commands as $command_name => $command_array) {
      $command_list[$command_name] = $command_array['idc']['handler class'] . ' : ("drush ' . $command_name . '")';
    }
    return new IDCStepSingleChoice('Select a IDC Command to execute', $command_list);
  }

  public function finishExecution() {
    // Don't need to do anything.
  }
}
