<?php
/**
 * @file
 * MenuGeneratorCsvParse class.
 */

/**
 * Creates a menu import file based on a CSV file.
 *
 * Format for csv should look like
 * {depth}, {Menu Link Title}, {Menu Link URL}
 *  -, This Title, http://www.mysite.com
 * --, Subtitle, http://www.mysite.com/subpage.htm
 */
class MenuGeneratorCsvParse extends MenuGenerator {
  /**
   * {@inheritdoc}
   */
  public function __construct(MenuGenerationParametersCsvParse $parameters, MenuGeneratorEngineCsv $engine) {
    parent::__construct($parameters, $engine);
  }

  /**
   * Calls the steps needed to build the import file.
   *
   * @return string
   *   The contents of the menu import file that was generated.
   */
  public function generate() {
    $this->menuFileContents = $this->engine->generate();
    $this->saveImportFile();

    return $this->menuFileContents;
  }
}
