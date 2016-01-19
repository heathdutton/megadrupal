<?php
/**
 * @file
 * Drush commands for Migration Tools.
 */

/**
 * Implements hook_drush_command().
 */
function migration_tools_drush_command() {
  $items = array();

  $items['migration-tools-migrate-images'] = array(
    'description' => "Migrate all image files from a source directory to the public files directory.",
    'aliases' => array('mtmi'),
    'arguments' => array(
      'organization' => 'The organization to migrate',
    ),
  );

  $items['migration-tools-html-folders'] = array(
    'description' => "Find all folders that contain html files.",
    'aliases' => array('mtscan'),
    'arguments' => array(
      'organization' => 'The organization to migrate',
    ),
    'options' => array(
      'ignore' => 'Comma delimited list of strings in folder to ignore',
      'extensions' => 'Comma delimited list of file extentions to find. (default: htm, html)',
    ),
    'examples' => array(
      'drush mtscan group_a' => 'Finds all directories in the source documents that contain .htm or .html files.',
      'drush mtscan group_a --extensions="htm,php"' => 'Finds all directories in the source documents that contain .htm or .php files.',
      'drush mtscan group_a --ignore="templates,archive"' => 'Finds all directories in the source documents that contain .htm or .html files except for the directories containing the word "templates" or "archive".',
    ),
  );

  $items['migration-tools-file-folders'] = array(
    'description' => "Find all folders that contain binary/not-image files.",
    'aliases' => array('dmff'),
    'arguments' => array(
      'organization' => 'The organization to migrate',
    ),
    'options' => array(
      'ignore' => 'Comma delimited list of strings in folder to ignore',
    ),
  );

  $items['mt-generate-menu'] = array(
    'description' => "Generate a file to import a menu by crawling an html menu.",
    'aliases' => array('dgm', 'migration-tools-generate-menu'),
    'arguments' => array(
      'organization' => 'The abbreviation of the organization to migrate (ex. oarm, ola, etc.)',
    ),
    'options' => array(
      'menu-location-uri' => "If the menu is in www.somesite.gov/foo/bar/index.html then this option should be set to 'foo/bar/index.html'. If no option is given the default will be www.somesite.gov/<organization>",
      'local-base-uri' => "When pages are migrated into drupal they will usually be at <organization>/page-title-alias, if for some reason, <organization> is not the base path of all the pages relevant to the menu that is being generated this option can be used to override that.",
      'css-selector' => "The generation algorithm starts evaluating a menu hierarchy at the outer most ul of the menu.If that ul is not at the default css-selector 'div.leftnav>ul', then you can use this option to point the algorithm to another ul.",
      'engine' => "Optional. Sets the engine used for generation. Defaults to district.",
      'recurse' => "Optional. Enables recursion for loading the pages in the initial menu, to look for more links. Defaults to FALSE.",
    ),
  );

  $items['mt-menu-import-file-from-seed'] = array(
    'description' => "Generate a file to import a menu from a simple csv seed file that contains depth, title, destination",
    'aliases' => array('dgmiffs', 'migration-tools-menu-import-file-from-seed'),
    'arguments' => array(
      'seed-file-name' => 'The filename of the csv file. ex: menu-og-1659.csv',
    ),
    'options' => array(
      'fallback-page' => "Optional. If no page exists for the path link to this Default: <nolink>",
      'import-file-path' => 'Optional. The menu Feature based path where the import file should be created.',
      'csv-path' => "Optional. The menu Feature based path to the seed  CSV file.",
    ),
  );

  $items['mt-import-group-menu'] = array(
    'description' => "Imports a menu for a specific organic group.",
    'aliases' => array('digm'),
    'arguments' => array(
      'file' => 'Required. menu text file name. Assumed that file exists in sites/all/module/custom/migration_tools/sources',
      'organization' => 'The organization to migrate',
    ),
    'examples' => array(
      'drush digm oip-menu.txt oip' => 'Import menu for organic group with group abbreviation oip',
    ),
  );

  $items['migration-tools-fix-legacy-links'] = array(
    'description' => "Fix links utilizing anchors in a migration group.",
    'aliases' => array('dmfll'),
    'arguments' => array(
      'migration' => 'The name of a specific migration.',
    ),
    'options' => array(
      'group' => "A migration group.",
      'idlist' => "A comma separeted list of nids.",
      'extradebug' => "Show more debugging information.",
    ),
    'examples' => array(
      'drush dmfll EoirPage' => 'Fix links with anchors in nodes from the given migration',
    ),
  );

  $items['mt-generate-migration-class'] = array(
    'description' => "Generates a MT migration class.",
    'aliases' => array('dgm'),
    'arguments' => array(
      'config' => 'A yml file referencing the organization configuration to be used.',
    ),
    'options' => array(
      'type' => '(optional) The type of migration to generate. Valid values are district, organization. Defaults to organization.',
    ),
    'examples' => array(
      'drush dgm atr.yml' => 'Generate a standard migration class using atr.yml for configuration.',
      'drush dgm ak.yml --type=district' => 'Generate a district migration class using ak.yml for configuration.',
    ),
  );

  return $items;
}

/**
 * Drush command callback.
 */
function drush_migration_tools_mt_migrate_fix_legacy_links($migration = NULL) {
  $fixer = new MigrationNodesLegacyLinksFixer();

  $nid_list = drush_get_option('idlist');
  $migration_name = $migration;
  $migration_group = drush_get_option('group');

  try {
    if ($nid_list) {
      $nids = explode(",", $nid_list);
      $fixer->setNids($nids);
    }
    elseif ($migration_name) {
      $fixer->getNidsFromMigration($migration_name);
    }
    elseif ($migration_group) {
      $fixer->getNidsFromMigrationGroup($migration_group);
    }
    else {
      drush_set_error("Either the \$argument or one of the 2 options (idlist, group) are required by this drush command");
    }
  }
  catch (Exception $e) {
    drush_migration_tools_debug_output($e->getMessage());
  }

  $fixer->fix();

  if (drush_get_option('extradebug')) {
    $fixer->printExtraDebugging();
  }
}


/**
 * Drush command callback.
 */
function drush_migration_tools_mt_migrate_images($organization) {
  // Let's get our source directory, and our output directory.
  $source_directory = variable_get("migration_tools_base_dir");
  if (!$source_directory) {
    throw new Exception("the migration_tools_base_dir variable has not been defined");
  }

  if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://')) {
    $output_directory = $wrapper->realpath();
  }
  else {
    throw new Exception("We couldn't get the absolute path for public://");
  }

  migration_tools_move_images($source_directory, $organization, $output_directory);
}

/**
 * Drush command callback for mt-generate-migrate-commands.
 *
 * @param string $groups
 *   A string of group abbreviations, separated by commas.
 */
function drush_migration_tools_mt_generate_migrate_commands($groups) {
  drush_print("");
  drush_print('drush migrate-register');
  drush_print("");

  $groups = explode(',', $groups);
  $brackets = drush_get_option('brackets');
  $prefix = '';
  if ($brackets) {
    $prefix = '[] ';
  }

  foreach ($groups as $group) {
    $group = trim($group);

    // Argument may be passed as "group-name|old-dir", or simply "group-name".
    if (strpos($group, '|')) {
      list($group, $directory) = explode('|', $group);
    }
    else {
      $directory = $group;
    }
    // Add usao/" prefix for district directories.
    if (strpos($group, 'usao-') === 0) {
      $directory = 'usao/' . $directory;
    }

    drush_print($prefix . "drush mi --group=$group --feedback='30 seconds'");
    drush_print($prefix . "drush digm $group-menu.txt $group");

    drush_print($prefix . "drush dmi $directory");
    drush_print("");
  }
}

/**
 * Drush command callback.
 */
function drush_migration_tools_mt_migrate_html_folders($organization) {
  $ignore = drush_get_option('ignore');
  $ignore = explode(",", $ignore);

  $extensions = drush_get_option('extensions');
  if (empty($extensions)) {
    $extensions = array("htm", "html");
  }
  else {
    $extensions = explode(",", $extensions);
  }

  drush_print_r(mt_migrate_find_folders_with_files($organization, $extensions, $ignore));
}

/**
 * Drush command callback.
 */
function drush_migration_tools_mt_migrate_file_folders($organization) {
  $ignore = drush_get_option('ignore');
  $ignore = explode(",", $ignore);
  drush_print_r(mt_migrate_find_folders_with_files($organization,
    array("pdf", "txt", "rtf", "doc", "docx", "xls", "xlsx", "csv", "mp3",
      "mp4", "wpd", "wp", "qpw", "xml", "ppt", "pptx"), $ignore));
}

/**
 * Find all the folders inside of org migraton souce that match the extensions.
 *
 * @param string $organization
 *   The org abbreviations for the org we want to process.
 * @param array $exts
 *   The extension of the type of files that we want to find.
 * @param array $ignore
 *   A list of words that if found in a folder path will ignore it from the
 *   list.
 *
 * @return string
 *   A JSON string of an array containing all of the folders that contain files
 *   of the given extensions.
 *
 * @throws Exception
 *   If migrate_html_base_dir variable is not set.
 */
function mt_migrate_find_folders_with_files($organization, $exts, $ignore = array()) {
  $source_directory = variable_get("migration_tools_base_dir");
  if (!$source_directory) {
    throw new Exception("the migration_tools_base_dir variable has not been defined");
  }
  $organization_dir = $source_directory . "/{$organization}";

  $file_types = $exts;

  $files = array();

  foreach ($file_types as $ft) {
    $output = shell_exec("find {$organization_dir} -type f -name '*.{$ft}'");
    $files = array_merge($files, explode("\n", $output));
  }

  $folders = array();
  // Let's get all the folders.
  foreach ($files as $f) {
    $file = str_replace($source_directory . "/", "", $f);
    $pieces = explode("/", $file);

    // Remove the file part.
    $pieces = array_slice($pieces, 0, count($pieces) - 1);
    $key = implode("/", $pieces);
    $ignore_folder = migration_tools_ignore_folder($key, $ignore);
    if (!empty($key) && !$ignore_folder) {
      $folders[$key] = TRUE;
    }
  }

  return var_export(array_keys($folders));
}

/**
 * Ignore this folder, or not.
 *
 * @param string $folder
 *   a string representing a folder.
 * @param array $ignore
 *   A list of string to find in folders to ignore.
 *
 * @return bool
 *   Should this folder be ignored.
 */
function migration_tools_ignore_folder($folder, $ignore = array()) {
  $ignore_it = FALSE;

  foreach ($ignore as $string) {
    if (!empty($string) && substr_count($folder, $string) > 0) {
      $ignore_it = TRUE;
      break;
    }
  }

  return $ignore_it;
}

/**
 * Move all the images from a directory tree to an output directory.
 *
 * The directory strucuture in the output directory will be an exact match of
 * the directory structury from the source directory.
 *
 * @param string $parent_input_directory
 *   The parent directory where anothe directory with images is located.
 * @param string $directory
 *   The directory that contains images.
 * @param string $parent_output_directory
 *   The directory where we want to save all the images.
 */
function migration_tools_move_images($parent_input_directory, $directory, $parent_output_directory) {

  // Make the directory where our files will go.
  $final_output_directory = "$parent_output_directory/$directory";

  // Get all files from the source directory.
  $final_input_directory = "$parent_input_directory/$directory";
  $files = scandir($final_input_directory);
  $count = 0;
  foreach ($files as $file) {
    $ext = pathinfo("$final_input_directory/$file", PATHINFO_EXTENSION);
    $ext = strtoupper($ext);
    // If any of the files are images, copy them.
    if ($ext == "GIF" || $ext == "PNG" || $ext == "JPG" || $ext == "JPEG") {
      // We only want to create the output directory if we have images.
      if (!file_exists($final_output_directory)) {
        mkdir($final_output_directory, 0777, TRUE);
      }

      watchdog("migration_tools", "FILE: @file EXT: @ext \n", array('@file' => $file, 'ext' => $ext));

      if (copy("$final_input_directory/$file", "$final_output_directory/$file")) {
        $count++;
        watchdog("migration_tools", "File @file was copied to $final_output_directory \n", array('@file' => $file));
      }
      else {
        watchdog("migration_tools", "There was an error copying @file \n", array('@file' => $file), WATCHDOG_ERROR);
      }
    }
    // If we are dealing with a directory, let's recurse.
    elseif (is_dir("$final_input_directory/$file") && "{$file}" != "." && "{$file}" != "..") {
      watchdog("migration_tools", "Found a directory {$file} \n");
      migration_tools_move_images($final_input_directory, $file, $final_output_directory);
    }
  }
  if ($count > 0) {
    $message = dt('Moved @count images into -> @final_output_directory.', array('@count' => $count, '@final_output_directory' => $final_output_directory));
    drush_log($message, 'success');
  }
}

/**
 * Drush command callback.
 */
function drush_migration_tools_mt_generate_menu($organization) {
  $css_selector = drush_get_option('css-selector');
  $local_base_uri = drush_get_option('local-base-uri');
  $menu_location_uri = drush_get_option('menu-location-uri');
  $engine_option = drush_get_option('engine', 'district');
  $recurse = drush_get_option('recurse', FALSE);

  $engines = array(
    'district' => 'MenuGeneratorEngineDistrict',
    'ust' => 'MenuGeneratorEngineUst',
  );

  $parameters = new MenuGenerationParametersHtmlCrawl($organization);

  if ($local_base_uri) {
    $parameters->setUriLocalBase($local_base_uri);
  }
  $parameters->setRecurse($recurse);

  if ($menu_location_uri) {
    $parameters->setUriMenuLocation($menu_location_uri);
    $parameters->initialMenuLocation = $menu_location_uri;
  }
  elseif ($local_base_uri) {
    $parameters->initialMenuLocation = $local_base_uri;
  }
  else {
    $parameters->initialMenuLocation = $organization;
  }

  $engine_class = $engines[$engine_option];
  $engine = new $engine_class($parameters);

  if ($css_selector) {
    $engine->setInitialCssSelector($css_selector);
  }

  $generator = new MenuGenerator($parameters, $engine);
  $file_name = $generator->generate();
}

/**
 * Drush command callback for generating a menu import file from a simple csv.
 *
 * @param string $seed_csv_filename
 *   The filename of the seed file to be processed.
 */
function drush_migration_tools_mt_menu_import_file_from_seed($seed_csv_filename) {
  $csv_path = drush_get_option('csv-path');
  $import_file_path = drush_get_option('import-file-path');
  $fallback_page = drush_get_option('fallback-page', '<nolink>');

  $parameters = new MenuGenerationParametersCsvParse($seed_csv_filename);
  $parameters->setFallbackPage($fallback_page);
  $parameters->setImportFilePath($import_file_path);
  $parameters->setSourcePath($csv_path);

  $engine = new MenuGeneratorEngineCsv($parameters);
  $generator = new MenuGeneratorCsvParse($parameters, $engine);
  $generator->generate();
}

/**
 * Drush command callback.
 */
function drush_migration_tools_mt_generate_menu_district($organization) {
  module_load_include('inc', 'migration_tools', 'includes/migration_tools');
  migration_tools_create_menu_file("usao|" . $organization, TRUE);
}

/**
 * Drush command callback.
 */
function drush_migration_tools_mt_import_group_menu($file_name, $group_abbrev) {
  module_load_include('inc', 'migration_tools', 'includes/migration_tools');
  module_load_include('inc', 'mt_sitewide', 'includes/mt_sitewide.groups');

  $group = mt_sitewide_load_org_by_abbrev($group_abbrev);
  $group_menu_name = 'menu-og-' . $group->nid;
  $file_path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'migration_tools') . '/sources/' . $file_name;

  drush_menu_import_import($file_path, $group_menu_name);
}

/**
 * Drush command callback for mt-generate-migration-class.
 *
 * @param string $config_filename
 *   The filename of the YML file containing the organization configuration.
 */
function drush_migration_tools_mt_generate_migration_class($config_filename) {
  module_load_include('inc', 'migration_tools', 'includes/drush/MtMigrationGenerator');

  // Default type option to 'organization'.
  $org_type = drush_get_option('type');
  if (empty($org_type)) {
    $org_type = 'organization';
  }

  try {
    $generator = new MtMigrationGenerator($config_filename, $org_type);
    $generator->generate();
  }
  catch (Exception $e) {
    drush_print('Migration generation failed with error: ' . $e->getMessage());
  }

}

/**
 * Wrapper function to output info if migration_tools_drush_debug = TRUE.
 *
 * @param mixed $output
 *   Thing to pass to drush_print_r().
 */
function drush_migration_tools_debug_output($output = '') {
  if (variable_get('migration_tools_drush_debug', FALSE)) {
    drush_print_r($output);
  }
}
