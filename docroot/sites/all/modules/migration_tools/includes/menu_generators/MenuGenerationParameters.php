<?php
/**
 * @file
 * MenuGenerationParameters class.
 */

/**
 * Class MenuGenerationParameters for a generating a menu_import file.
 */
class MenuGenerationParameters {
  /**
   * The domain/host of the site where the new menu will live.
   * @var string
   */
  private $domain;

  /**
   * The page on the site where any menu item that has no existing page should
   * be used as the destination.
   * @var string
   */
  private $fallbackPage;

  /**
   * The path relative to the docroot of the site where the import file
   * will be generated.
   * @var string
   */
  private $importFilePath;
  public $importFileName;
  private $importUrl;

  private $menuModulePath;

  private $sourceFileName;
  private $sourcePath;
  private $sourceUri;

  private $subDirectory;
  private $uriMenuLocationBasePath;


  /**
   * Constructor.
   */
  public function __construct() {
    $this->setUriMenuLocationBasePath();
  }

  /**
   * Performs parameter processing and returns the final parameters instance.
   *
   * Call this parent last in your build() extension.
   *
   * @return \MenuGenerationParameters
   *   The parameter object.
   */
  public function build() {
    return $this;
  }

  /**
   * Setter for the domain of the site.
   */
  public function setDomain($url = '') {
    if (empty($url)) {
      $url = variable_get('migration_tools_base_domain', '');
    }
    $this->domain = $url;
  }

  /**
   * Getter for the domain of the site.
   */
  public function getDomain() {
    if (empty($this->domain)) {
      $this->setDomain();
    }
    return $this->domain;
  }


  /**
   * Setter for the menu generation source file.
   */
  public function setSourceFileName($location) {
    $this->sourceFileName = $location;
  }

  /**
   * Getter for the menu generation source file.
   */
  public function getSourceFileName() {

    return $this->sourceFileName;
  }


  /**
   * Getter.
   *
   * @return string
   *   The path where the import file can be found.
   */
  public function getSourcePath() {
    return $this->sourcePath;
  }

  /**
   * Setter for the location of the file to be processed.
   */
  public function setSourcePath($source_path) {
    // $seed_location can not be empty.
    if (empty($source_path)) {
      $module_path = $this->getMenuModulePath();
      $source_path = variable_get('migration_tools_custom_menu_csv_source_path', 'menu_source/menu_csv');
      $source_path = "{$module_path}/{$source_path}";
    }
    // Make sure it ends in a path /.
    $source_path = UrlTools::normalizePathEnding($source_path);
    $this->sourcePath = $source_path;
  }

  /**
   * Getter for the full path and filename of the item to be parsed.
   *
   * @return string
   *   The full path and filename of the item to be parsed
   */
  public function getSourceUri() {
    $path = $this->getSourcePath();
    $filename = $this->getSourceFileName();
    return "{$path}{$filename}";
  }


  /**
   * Getter for the filename of the import file that will be created.
   */
  public function getImportFileName() {
    if (empty($this->importFileName)) {
      $this->setImportFilename();
    }
    return $this->importFileName;
  }


  /**
   * Setter for the filename of the import file that will be created.
   */
  public function setImportFileName($file_name = '') {
    $this->importFileName = $file_name;
  }


  /**
   * Getter for the import file path to where the import file will be saved.
   *
   * @return string
   *   The path where the import file will be saved.
   */
  public function getImportFilePath() {
    if (empty($this->importFilePath)) {
      $this->setImportFilePath();
    }
    return $this->importFilePath;
  }

  /**
   * Setter for the path where the generated import file will be saved.
   */
  public function setImportFilePath($import_file_location) {
    // $import_file_location can not be empty.
    if (empty($import_file_location)) {
      // Not passed, so try to construct it.
      $import_file_path = $this->getMenuModulePath();
      $import_destination = variable_get('migration_tools_custom_menu_dest_path', 'menu_source');
      $import_file_location = "{$import_file_path}/{$import_destination}/";
    }
    $import_file_location = UrlTools::normalizePathEnding($import_file_location);
    $this->importFilePath = $import_file_location;
  }


  /**
   * Setter for default fallback location to link to if no link is present.
   */
  public function setFallbackPage($fallback_link = NULL) {
    if ($fallback_link === NULL) {
      // Build the fallback link.
      $fallback_link = '<nolink>';
    }
    $this->fallbackPage = $fallback_link;
  }

  /**
   * Getter for the default fallback location to link to.
   *
   * @return string
   *   The fallback page/location to use if there is none.
   */
  public function getFallbackPage() {
    if (empty($this->fallbackPage)) {
      $this->setFallbackPage();
    }
    return $this->fallbackPage;
  }


  /**
   * Sets module path of the menu controlling feature where import files belong.
   *
   * @param string $menu_module_path
   *   Optional. A path to the module where import files should be created.
   *
   * @throws Exception
   */
  public function setMenuModulePath($menu_module_path = '') {
    if (empty($menu_module_path)) {
      $menu_feature = check_plain(variable_get('migration_tools_custom_menu_module', ''));
      $menu_module_path = drupal_get_path('module', $menu_feature);
      if (empty($menu_module_path)) {
        throw new Exception("The path for the Menu feature was needed, but has not been set. Visit /admin/config/migration_tools");
      }
    }
    $this->menuModulePath = $menu_module_path;
  }


  /**
   * Getter of path to the module where import files should be created.
   *
   * @return string
   *   A path to the module where import files should be created.
   */
  public function getMenuModulePath() {
    if (empty($this->menuModulePath)) {
      $this->setMenuModulePath();
    }
    return $this->menuModulePath;
  }


  /**
   * Setter for the location of the menu links being parsed.
   */
  public function setUriMenuLocationBasePath($base_path = '') {
    if (empty($base_path)) {
      $base_path = $this->getDomain();
    }
    $base_path = UrlTools::normalizePathEnding($base_path);
    $this->uriMenuLocationBasePath = $base_path;
  }

  /**
   * Getter.
   */
  public function getUriMenuLocationBasePath() {
    return $this->uriMenuLocationBasePath;
  }

  /**
   * Setter for the subdirectory where  a menu resides.
   */
  public function setSubDirectory($subdirectory) {
    $subdirectory = UrlTools::normalize($subdirectory);
    $this->subDirectory = $subdirectory;
  }

  /**
   * Getter for the subdirectory where  a menu resides.
   *
   * @returns string
   *   The subidrectory for a given menu.
   */
  public function getSubDirectory() {
    return $this->subDirectory;
  }
}
