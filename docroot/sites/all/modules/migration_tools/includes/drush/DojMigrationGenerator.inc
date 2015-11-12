<?php

/**
 * @file
 * Contains functions used for generating migration classes dynamically.
 */

require_once DRUPAL_ROOT . "/../vendor/autoload.php";
use Symfony\Component\Yaml\Parser;

/**
 * Class MtMigrationGenerator.
 */
class MtMigrationGenerator {

  /**
   * @var string
   *   Valid values include organization and district.
   */
  protected $orgType;

  /**
   * @var array
   *   Organization configuration. Loaded from yaml.
   */
  protected $orgConfig;

  /**
   * @var array
   *   The migration template configuration. Loaded from yaml.
   */
  protected $templateConfig;

  /**
   * @var string
   *   The filepath of the migration_tools module directory.
   */
  protected $modulePath;

  /**
   * Constructs MtMigrationGenerator object.
   *
   * @param string $config_filename
   *   The name of the yaml file containing the organization configuration.
   * @param string $org_type
   *   The type of organization migration to generated. Valid values are
   *   organization and district. This controls the migration template that
   *   will be used.
   */
  public function __construct($config_filename, $org_type) {
    $this->orgType = $org_type;
    $this->modulePath = DRUPAL_ROOT . '/' . drupal_get_path('module', 'migration_tools');
    $this->setTemplateConfig();
    $this->setOrgConfig($config_filename);
  }

  /**
   * Generates the migration class.
   */
  public function generate() {
    $this->writeMigrationInfo();
    $this->registerMigration();
    $this->writeMigrationFile();
  }

  /**
   * Get and validate the migration template configuration.
   *
   * @return array
   *   The loaded yaml configuration.
   */
  protected function setTemplateConfig() {
    $template_config_filepath = "{$this->modulePath}/scripts/config/{$this->orgType}.yml";

    $required = array('twig');
    $this->templateConfig = $this->getConfiguration($template_config_filepath, $required);
  }

  /**
   * Get and validate the organization configuration.
   *
   * @param string $config_filename
   *   The filename of the organization configuration yaml file.
   *
   * @return array
   *   The loaded yaml configuration.
   */
  protected function setOrgConfig($config_filename) {
    $org_config_filepath = "{$this->modulePath}/scripts/{$this->orgType}s/$config_filename";

    if (!file_exists($org_config_filepath)) {
      throw new Exception("The File $org_config_filepath does not exist!");
    }

    $required = array(
      'abbreviation',
      'full_name',
      'directory',
    );
    $this->orgConfig = $this->getConfiguration($org_config_filepath, $required);

    if (!empty($this->orgConfig['pr_subdirectory']) && empty($this->orgConfig['component_tid'])) {
      throw new Exception("The component_tid property is required for press release migrations.");
    }
  }

  /**
   * Get YAML configuration from a file and validate required properties.
   */
  public function getConfiguration($path, $required) {
    // Initialize the yaml parser.
    $yaml = new Parser();

    // Parse YAML contents.
    $config = $yaml->parse(file_get_contents($path));

    // Validate the local configuration.
    foreach ($required as $key) {
      if (!array_key_exists($key, $config)) {
        throw new Exception("Missing '{$key}' from {$path} configuration file");
      }
    }

    return $config;
  }

  /**
   * Adding the file info to the .info file.
   */
  public function writeMigrationInfo() {
    // Let's add the file to migration_tools.info.
    $include_filename = str_replace("-", "_", $this->orgConfig['abbreviation']);
    $info_file = "{$this->modulePath}/migration_tools.info";
    $include_filepath = "organizations/{$include_filename}.inc";
    $text = "files[] = {$include_filepath}\n";

    $contents = file_get_contents($info_file);
    if (strpos($contents, $text) == FALSE) {
      file_put_contents($info_file, $text, FILE_APPEND);
      drush_print_r("$include_filepath has been added to migration_tools.info.");
    }
  }

  /**
   * Add the group and the migrations to the api array.
   */
  public function registerMigration() {
    // Now, Let's add the necessary info to migration_tools.migrate.inc
    // $migrations_filepath = "{$this->modulePath}/includes/migration_tools_migrations.inc";
    // $api = include "{$this->modulePath}/includes/migration_tools_migrations.inc";

    $migrations = array('Page', 'File');
    if (!empty($this->orgConfig['pr_subdirectory'])) {
      $migrations[] = 'PressRelease';
    }

    // Add the group definition.
    $api['groups'][$this->orgConfig['abbreviation']] = array(
      'title' => $this->orgConfig['full_name'],
    );

    $base_class_name = $this->getBaseMigrationClassName();

    foreach ($migrations as $migration) {
      $api['migrations']["{$base_class_name}{$migration}"] = array(
        'group_name' => $this->orgConfig['abbreviation'],
        'class_name' => "{$base_class_name}{$migration}Migration",
      );
    }

    // Save the modifications.
    $header = "<?php\n/**\n * @file\n * Migrations.\n */\n\n// @codingStandardsIgnoreStart\nreturn ";
    $footer = ";\n// @codingStandardsIgnoreEnd\n";
    $definitions = str_replace("=> \n", "=>\n", var_export($api, TRUE));
    $contents = $header . $definitions . $footer;
    file_put_contents($migrations_filepath, $contents);

    drush_print("The group and migrations have been registered in the api array.");
  }

  /**
   * Generate the migration file.
   */
  public function writeMigrationFile() {
    // Generate the classes with twig.
    $templates_dir = "{$this->modulePath}/scripts/templates/";
    $loader = new Twig_Loader_Filesystem($templates_dir);
    $twig_env = new Twig_Environment($loader);

    $info = array(
      'abbreviation' => $this->orgConfig['abbreviation'],
      'full_name' => $this->orgConfig['full_name'],
      'class_base_name' => $this->getBaseMigrationClassName($this->orgConfig['abbreviation']),
      'directory' => $this->orgConfig['directory'],
      'recurse' => TRUE,
    );

    $source_directories = $this->getSourceDirectories();
    $info['page'] = $source_directories['page'];

    if (!empty($source_directories['press'])) {
      $info['press'] = $source_directories['press'];
      $info['recurse'] = FALSE;
    }

    // Build regex based on $extensions.
    $extensions = $this->getFileExtensions();
    $extensions = implode('|', $extensions);
    $regex = "/.*\.(" . $extensions . ")$/i";
    $info['file_extensions'] = $regex;

    if (!empty($this->orgConfig['component_tid'])) {
      $info['component_tid'] = $this->orgConfig['component_tid'];
    }

    $classes_file_data = $twig_env->render("{$this->templateConfig['twig']}", array('info' => $info));
    file_put_contents("{$this->modulePath}/organizations/{$this->getMigrationClassFileName()}", $classes_file_data);

    drush_print("File with migrations was created.");
  }

  /**
   * Gets an array of source directories containing HTML.
   *
   * If there is a special subdirectory for press releases, this will split
   * the list of directories containing press releases from those containing
   * basic pages. Otherwise, it will return an empty array so that the entire
   * directory structure is used.
   */
  protected function getSourceDirectories() {
    $directories = array('page' => array());
    $extentions = $this->getFileExtensions();
    $extentions = implode(',', $extentions);

    if (!empty($this->orgConfig['pr_subdirectory'])) {

      // Generate an array of all subdirectories containing HTML.
      // @todo Replace usage of shell_exec() with direct calls to php function.
      if ($this->orgConfig['drush_alias']) {
        $html_array_data = shell_exec("drush @{$this->templateConfig['drush_alias']} mt-migrate-html-folders {$this->orgConfig['directory']} --extensions='{$extentions}'");
      }
      else {
        $html_array_data = shell_exec("drush mt-migrate-html-folders {$this->orgConfig['directory']} --extensions='{$extentions}'");
      }
      $data = explode("\n", $html_array_data);

      $directories['press'] = array();
      foreach ($data as $line) {
        // Ignore array things.
        if (substr_count($line, "(") == 0 && substr_count($line, ")") == 0 && !empty($line)) {
          if (substr_count($line, "{$this->orgConfig['pr_subdirectory']}") > 0) {
            $directories['press'][] = trim($line);
          }
          else {
            $directories['page'][] = trim($line);
          }
        }
      }
    }
    else {
      $directories['page'][] = trim("0 => '{$this->orgConfig['directory']}',");
    }

    return $directories;
  }


  /**
   * Gets an array of file extensions (filetypes) to be processed.
   *
   * @return array
   *   An array of file extensions.
   */
  protected function getFileExtensions() {

    if (!empty($this->orgConfig['file_extensions'])) {
      // file_extensions is overridden.
      $file_extensions = $this->orgConfig['file_extensions'];

    }
    else {
      // No overides so use these defaults.
      $file_extensions = array(
        'html',
        'htm',
      );

    }

    return $file_extensions;
  }

  /**
   * The camel case version of the abbreviation.
   */
  public function getBaseMigrationClassName() {
    $abbreviation_pieces = explode("-", $this->orgConfig['abbreviation']);
    $class_name = '';
    foreach ($abbreviation_pieces as $piece) {
      $class_name .= ucfirst($piece);
    }

    return $class_name;
  }

  /**
   * Make the migration file name from the district abbreviation.
   */
  public function getMigrationClassFileName() {
    $abbr_pieces = explode("-", $this->orgConfig['abbreviation']);
    $file = '';
    $counter = 0;
    foreach ($abbr_pieces as $piece) {
      $file .= ($counter >= 1) ? "_" : "";
      $file .= $piece;
      $counter++;
    }
    return $file . ".inc";
  }
}
