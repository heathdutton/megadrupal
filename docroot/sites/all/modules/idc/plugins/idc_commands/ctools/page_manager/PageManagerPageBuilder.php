<?php
/**
 * @file
 * IDC Implementation to create page manager pages.
 */

$plugin = array(
  'handler class' => 'PageManagerPageBuilder',
  'dependencies' => array(
    'page_manager',
  ),
);

class PageManagerPageBuilder extends IDCCommandBase {

  public static $commandName = 'page-manager-add';

  /**
   * Returns the array that defines the drush command.
   *
   * @return array
   *   The drush command definition.
   */
  public static function getCommandDefinition() {
    return array(
      'description' => t('Creates a new Page Manager Page'),
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
      'choose_machine_name' => new IDCStepPrompt("Choose a machine name for the page"),
      'choose_admin_title' => new IDCStepPrompt("Enter the Admin Title for the page"),
      'choose_description' => new IDCStepPrompt("Enter a description for the page"),
      'choose_path' => new IDCStepPrompt("Specify the path for the page"),
      'choose_other_options',
    );
  }

  public function choose_other_options($processed = FALSE) {
    if (!$processed) {
      $options = array(
        'make_frontpage' => 'Make this page your frontpage.',
        'admin_paths' => 'Use this page in an admin overlay.',
      );
      return new IDCStepMultipleChoice("Select the options desired for the page", $options);
    }
  }

  /**
   * Executes any final code that should run after all the command steps.
   */
  public function finishExecution() {
    // Gather page manager page info and store it in the database.
    $results = $this->getStepResults();
    try {
      ctools_include('page', 'page_manager', 'plugins/tasks');
      $conf = array(
        'admin_paths' => in_array('admin_paths', $results['choose_other_options']),
      );
      $page = new PageManagerPage($results['choose_machine_name'], $results['choose_admin_title'], $results['choose_description'], $results['choose_path'], $conf);
      page_manager_page_save($page);
      if (in_array('make_frontpage', $results['choose_other_options'])) {
        variable_set('site_frontpage', $results['choose_path']);
      }
    }
    catch (Exception $e) {
      $this->terminate(t('Page Manager Page could not be created. Exception: ' . $e->getMessage()));
    }
  }
}

/**
 * Quick, incomplete class to emulate a Page Manager Page.
 *
 * Class PageManagerPage
 */
class PageManagerPage {

  /**
   * Constructs a new page manager page object that can be used as the $page
   * argument for page_manager_page_save().
   *
   * @param $machineName
   * @param $adminTitle
   * @param $adminDescription
   * @param $path
   * @param $conf
   */
  public function __construct($machineName, $adminTitle, $adminDescription, $path, $conf) {
    // Static values.
    $this->task = 'page';
    $this->pid = NULL;
    // export_type = 1 means object is in db. @see ctools/includes/export.inc.
    $this->export_type = 1;
    $this->type = "Normal";

    // Access, menu, and arguments config not supported yet.
    $this->access = array();
    $this->menu = array();
    $this->arguments = array();

    // Actual page config.
    $this->name = $machineName;
    $this->admin_title = $adminTitle;
    $this->admin_description = $adminTitle;
    $this->path = $path;
    $this->conf = $conf;
  }

}
