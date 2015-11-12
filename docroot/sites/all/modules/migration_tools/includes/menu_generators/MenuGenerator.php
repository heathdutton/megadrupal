<?php
/**
 * @file
 * MenuGeneration class.
 */

/**
 * Class MenuGenerator.
 *
 * Builds a menu import file for use with menu_import module.
 */
class MenuGenerator {
  public $engine;
  private $fileName;
  private $fileOutputDirectory;
  public $menuFileContents = '';
  private $parameters;


  /**
   * Constructor.
   */
  public function __construct(MenuGenerationParameters $parameters, MenuGeneratorEngine $engine) {
    $this->parameters = $parameters->build();
    $this->engine = $engine;
  }


  /**
   * Creates and saves the menu import file.
   *
   * @return string
   *   The path and filename of the file created.
   *
   * @throws MigrateException
   *   In the event it is unable to create the file.
   */
  public function saveImportFile() {
    $file = DRUPAL_ROOT . "/{$this->parameters->getImportFilePath()}{$this->parameters->getImportFilename()}";
    $menu_file_contents = $this->menuFileContents;
    try {
      $fh = fopen($file, 'w');
      if ($fh) {
        fwrite($fh, $menu_file_contents);
        fclose($fh);
        drush_print("The menu import file: {$file} was generated.\n");
      }
      else {
        $message = "Error (likely permissions) creating the file: $file\n";
        throw new MigrateException($message);
      }
    }
    catch (Exception $e) {
      drush_print("File creation failed.  Caught exception: " . $e->getMessage() . "\n");
      // Output file to terminal so it is available to use.
      drush_print("The menu file was not generated. Outputting menu to terminal.\n\n");
      drush_print($menu_file_contents);
    }
    return $file;
  }
}
