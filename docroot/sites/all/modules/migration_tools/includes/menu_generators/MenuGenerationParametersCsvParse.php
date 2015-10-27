<?php
/**
 * @file
 * MenuGenerationParametersCsvParse class.
 */

/**
 * Class MenuGenerationParametersCsvParse for a generating a menu_import file.
 */
class MenuGenerationParametersCsvParse extends MenuGenerationParameters {

  /**
   * Constructor.
   *
   * @param string $seed_csv_filename
   *   The file name of the csv file to read and process.
   */
  public function __construct($seed_csv_filename) {
    if (empty($seed_csv_filename)) {
      throw new Exception("The parameter for the filename of the CSV file must be specified.");
    }
    $this->setSourceFileName($seed_csv_filename);
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    // Update the source path to include the drupal root.
    $source_file_path  = DRUPAL_ROOT . "/{$this->getSourcePath()}";
    $this->setSourcePath($source_file_path);

    return parent::build();
  }


  /**
   * {@inheritdoc}
   */
  public function setImportFileName($file_name = '') {
    if (empty($file_name)) {
      $file_name = $this->getSourceFileName();

      // Trim off .csv or .txt.
      $search = array(
        '.csv',
        '.txt',
      );
      $file_name = str_ireplace($search, '', $file_name);
      // Append -export.txt.
      $file_name = "{$file_name}-export.txt";
    }
    $this->importFileName = $file_name;
  }
}
