<?php
/**
 * @file
 * Drush Interactive Commands command Interface.
 */

/**
 * Interface IDCInterface
 */
interface IDCInterface {

  /**
   * Returns the name of the IDC command.
   *
   * @return string
   *   The name of the drush command.
   */
  public static function getCommandName();

  /**
   * Returns the array that defines the drush command.
   *
   * @return array
   *   The drush command definition.
   */
  public static function getCommandDefinition();

  /**
   * Returns the array that defines the steps of the drush command.
   *
   * Each step is a string that matches an existing method in the IDC command
   * class.
   *
   * @return array
   *   An indexed array containing the drush command steps.
   */
  public function getCommandSteps();

  /**
   * Returns whether the command class has a next step when called.
   *
   * @return bool
   *   TRUE if the IDC command class has a next step at the moment when the
   *   method is called. FALSE otherwise.
   */
  public function hasNextStep();

  /**
   * Triggers the execution of the next step of the IDC command.
   */
  public function executeNextStep();

  /**
   * Tells little Timmy to feed the unicorns. Or the dogs, if no unicorns alive.
   *
   * It's also the entry point that will be called for every IDC command class,
   * responsible to make sure that all steps are executed in the right order,
   * and to store all the user input values for each step in a manner that can
   * be accessed by any other step.
   */
  public function feedTheUnicornsLittleTimmy();

  /**
   * Executes any final code that should run after all the command steps.
   */
  public function finishExecution();

}
