<?php

/**
 * @file
 * The main class to extract, modify and import files from an edge archive
 *
 * The main workflow is:
 *
 * - Extract archive in tmp dir
 * - Find the working directory with the main JS Edge file (_edge.js) and get
 *   the project name and composition id from it
 * - Alter _edge.js file to call custom JS function at runtime which alters path
 *   prefixes in the DOM
 * - Alter _edgePreload.js file to call custom JS function at runtime which
 *   alters path prefixes in the DOM and the preloader object
 * - replace the general edge stage id (id of the main DIV container) with a
 *   project specific id. This is really important to run multiple compositions
 *   along each other on one page
 */


class EdgeCompositionBuilder {

  /**
   * Main archive file which contains the composition
   */
  protected $file = '';

  /**
   * Tmp directory that the archive will be extracted to
   */
  protected $tmpDirBase = "";

  /**
   * Tmp directory that contains the composition files, sub dir of $tmpDirBase
   */
  protected $tmpDir = "";


  /**
   *  Base edge directory
   */
  protected $edgeDir = '';

  /**
   *  Base edge directory for all projects
   */
  protected $edgeProjectDir = '';

  /**
   * Path of the project
   */
  protected $destinationDir = "";

  /**
   * Path of the main edge file _edge.js
   */
  protected $mainEdgeJS = "";

  /**
   * Path of the preloader file _edgePreload.js
   */
  protected $preloaderFile = "";

  /**
   * Name of the project
   */
  protected $projectName = "";

  /**
   * Composition Id
   */
  protected $stageClass = "";

  /**
   * Version of Edge that the composition uses.
   */
  protected $edgeVersion = '';

  /**
   * Dimensions of the composition.
   */
  protected $dimensions;

  /**
   * Libraries that are being used by the composition
   */
  protected $libraries = array();

  /**
   * Build status
   */
  protected $buildSuccess = FALSE;


  /**
   * Default constructor which sets up filename and directory paths.
   *
   * @param string $base_dir
   *   Edge base directory.
   * @param string $project_dir
   *   Edge project directory
   */
  public function __construct($base_dir, $project_dir) {
    global $user;
    $this->edgeDir = $base_dir;
    $this->edgeProjectDir = $project_dir;
    $this->tmpDirBase = 'temporary://' . '/edge_suite/src_' . $user->uid;
    $this->tmpDir = $this->tmpDirBase;

    $this->dimensions = array(
      'width' => 0,
      'height' => 0,
      'min-width' => 0,
      'max-width' => 0,
      'min-height' => 0,
      'max-height' => 0,
    );
  }

  /**
   * Name of the project, after the composition has been processed.
   * @return string
   *   Project name
   */
  public function getProjectName() {
    return $this->projectName;
  }

  /**
   * Composition Id (class name) of the project.
   * @return string
   *   Composition Id (class name)
   */
  public function getCompositionId() {
    return $this->stageClass;
  }

  /**
   * Return the edge version.
   * @return string
   *   Edge version as string, e.g. 0.1.6
   */
  public function getEdgeVersion() {
    return $this->edgeVersion;
  }

  /**
   * Return build status.
   * @return bool
   *   true if build was successful, false otherwise
   */
  public function getBuildSuccess() {
    return $this->buildSuccess;
  }

  /**
   * Return the dimensions and its limits of the composition stage.
   * @return array
   *   dimension values for width, height including  min and max values
   */
  public function getDimensions() {
    return $this->dimensions;
  }

  /**
   * Main function to process/build an edge composition.
   *
   * @param string $filename
   *   Archive to extract.
   * @param string $destination_dirname
   *   Project destination folder.
   * @param bool $replace_project
   *   True if an existing project should be overwritten.
   * @param bool $replace_libraries
   *   True if existing libraries in the shared folder should be overwritten
   *
   * @return bool
   *   true if build process was successful, false otherwise
   * @throws Exception
   */
  public function processArchive($filename, $destination_dirname, $replace_project = FALSE, $replace_libraries = FALSE) {
    $this->file = $filename;
    $this->destinationDir = $this->edgeProjectDir . '/' . $destination_dirname;

    // Check if destination already exists and if it should be replaced. Other
    // abort as there is no valid destination available.
    if (file_exists($this->destinationDir)) {
      if ($replace_project) {
        file_unmanaged_delete_recursive($this->destinationDir);
      }
      else {
        throw new Exception(t('The composition directory already exists. Please use the according action to replace a composition.'));
      }
    }
    drupal_mkdir($this->destinationDir);

    // Extract the archive.
    $this->extract($this->file, $this->tmpDirBase);

    // Find the working directory, set _edge.js file, project name and comp id.
    $this->validateArchiveStructure();

    // Get the preloader file _edgePreload.js.
    $this->getPreloader();

    // Alter main JS edge file to so it calls custom JS from edge_drupal.js.
    $this->alterMainEdgeJs();

    // Alter preload JS file so it calls custom JS from edge_drupal.js.
    $this->alterPreloaderJs();

    // Read in all the libraries and copy them to a shared edge library folder.
    $this->readLibs($replace_libraries);

    // Move all assets (and the _edgeAction.js)
    $this->moveEdgeFiles($this->tmpDir, $this->destinationDir);

    // Get a list of the remaining files and tell the user these files will
    // be ignored.
    $ignored_files = file_scan_directory($this->tmpDir, '/\.*$/');

    // Generate ignored and obsolete file messages.
    $files_names_obsolete = array();
    $files_names_ignored = array();
    foreach ($ignored_files as $f) {
      $obsolete_file_patterns = array(
        $this->projectName . '_oam.xml',
        $this->projectName . '.edge',
        $this->projectName . '.html',
      );
      if (in_array($f->filename, $obsolete_file_patterns)) {
        $files_names_obsolete[] = $f->filename;
      }
      else {
        $files_names_ignored[] = $f->filename;
      }
    }
    $this->setFileMessages($files_names_obsolete, 'The following files were ignored as they are not needed:');
    $this->setFileMessages($files_names_ignored, 'The following files were ignored as they are not supported:', 'warning');

    // Delete the tmp archive directory with all remaining files.
    $this->cleanup();

    // Set build success.
    $build_success = TRUE;

    return $build_success;
  }

  /**
   * Sets the message followed by a list of file names.
   *
   * @param array $file_list
   *   List of file names
   * @param string $message
   *   Message to be set on top of the list (will be processed through t())
   * @param string $type
   *   Standard Drupal message type
   */
  protected function setFileMessages($file_list, $message, $type = 'status') {
    if (!empty($file_list)) {
      $files_list_string = t($message) . '</br>';
      foreach ($file_list as $file) {
        $files_list_string .= check_plain($file) . '</br>';
      }
      drupal_set_message($files_list_string, $type);
    }
  }

  /**
   * Delete the tmp archive directory with all remaining files.
   */
  public function cleanup() {
    if (file_exists($this->tmpDirBase)) {
      file_unmanaged_delete_recursive($this->tmpDirBase);
    }
  }

  /**
   * Extracts the archive and sets the path to the working directory (tmpDir).
   *
   * @throws Exception
   */
  protected function extract($file, $destination) {
    // Check if file really exists.
    if (!file_exists($file)) {
      throw new Exception(t('Could not find archive: %file.', array('%file' => $file)));
    }

    // Check if zip lib is available.
    if (!class_exists('ZipArchive')) {
      throw new Exception(t('The server does not support zip extraction, class ZipArchive not found.', array('%file' => $file)));
    }

    // Copy OAM file.
    if (substr($file, -4) == '.oam') {
      $file_oam = $file;
      $file_zip = substr($file_oam, 0, count($file_oam) - 5) . '.zip';
      drupal_set_message(t('OAM file has been temporarily copied to .zip file for processing.'));
      $success = file_unmanaged_copy($file_oam, $file_zip);
      if ($success) {
        $file_archive = $file_zip;
        $oam_source = TRUE;
      }
    }
    else {
      $oam_source = FALSE;
      $file_archive = $file;
    }

    // Get archiver.
    $archiver = archiver_get_archiver($file_archive);

    // Clean up copied zip file.
    if ($oam_source) {
      file_unmanaged_delete($file_archive);
    }

    if (!$archiver) {
      throw new Exception(t('Cannot extract %file, not a valid archive.', array('%file' => $file)));
    }

    // Delete old edge tmp dir.
    file_unmanaged_delete_recursive($destination);

    // Extract archive.
    $archiver->extract($destination);
  }

  /**
   * Finds the main edge file _edge.js, sets the project name.
   *
   * @throws Exception
   */
  protected function validateArchiveStructure() {
    // Find all files that match the pattern.
    $main_edge_files = file_scan_directory($this->tmpDir, '/_edge\.js$/', array('recurse' => TRUE));
    if (count($main_edge_files) == 0) {
      throw new Exception(t('Aborting, no file found that matches the main edge JS filename pattern. This is not a valid composition archive.'));
    }
    else {
      // Todo: pop or shift? Avoid choosing edge.js in the published folder.
      $main_edge_file = array_pop($main_edge_files);
      $this->tmpDir = drupal_dirname($main_edge_file->uri);
      $this->mainEdgeJS = $main_edge_file->uri;
      $project_name_tmp = substr($main_edge_file->name, 0, strlen($main_edge_file->name) - strlen('_edge'));

      // Sanitize project name.
      if (preg_match('/^[a-zA-Z0-9_-]*$/', $project_name_tmp) == 1) {
        $this->projectName = $project_name_tmp;
      }
      else {
        throw new Exception(t('Aborting, the project name is not valid.'));
      }

      // Check if there were multiple main js files.
      if (count($main_edge_files) > 0) {
        $msg_dir = substr($this->tmpDir, strlen($this->tmpDirBase));
        drupal_set_message(t('Multiple edge.js files found. Everything outside @dir will be ignored.', array('@dir' => $msg_dir)));
      }
    }
  }

  /**
   * Finds the preloader file _edgePreload.js.
   *
   * @throws Exception
   */
  protected function getPreloader() {
    if (!empty($this->projectName)) {
      $js_file_tmp = $this->tmpDir . '/' . $this->projectName . '_edgePreload.js';
    }
    else {
      throw new Exception('Cannot read main JS edge file, project name not set.');
    }

    if (file_exists($js_file_tmp)) {
      $this->preloaderFile = $js_file_tmp;
    }
    else {
      throw new Exception(t('Cannot find main JS edge file @path.', array('@path' => $js_file_tmp)));
    }
  }

  /**
   * Alters the main edge file.
   *
   * A calls to alterDomPaths() will be added to add path prefixes to the DOM
   * assets.
   *
   * @throws Exception
   */
  protected function alterMainEdgeJs() {
    $file = $this->mainEdgeJS;
    $content = file_get_contents($file);

    // Parse the dimensions.
    $this->parseDimensions($content);

    // Extract the composition id.
    // The relevant JS structure (might be minified).
    // (function($, Edge, compId){....})(jQuery, AdobeEdge, "MyProject");.
    $pattern = '/}\)\(jQuery,\s?AdobeEdge,\s?\"([a-zA-Z0-9_-]*)\"/';
    $matches = array();
    $found = preg_match($pattern, $content, $matches);
    if (!$found) {
      throw new Exception(t('Unable to parse stage name from main edge file'));
    }
    $this->stageClass = $matches[1];

    // Replace alterRegisterCompositionDefn() with custom call to alter
    // symbols and fonts.
    $register_pattern = '/Edge.registerCompositionDefn\(compId,\s?symbols,\s?fonts,\s?resources\);/';
    $register_replace = 'AdobeEdge.alterRegisterCompositionDefn(compId, symbols, fonts, resources, Edge.registerCompositionDefn);';
    $content = preg_replace($register_pattern, $register_replace, $content);

    // Write modified file.
    $fh = fopen($file, "w");
    fwrite($fh, $content);
    fclose($fh);
  }


  /**
   * Parse dimensions from the content and set them in the dimension array.
   *
   * @param string $content
   *   Content of the main edge.js file.
   */
  protected function parseDimensions($content) {
    // Parse dimensions of the composition.
    // Uncompressed version.
    // TODO: Drop support.
    // Get the stage section first.
    $pattern_stage_style = '/"\${_stage}":\s?\[\n?\r?((\s*\[(.)*\],?)*)/i';
    $matches = array();
    if (preg_match($pattern_stage_style, $content, $matches)) {
      $matches_dimensions = array();
      // Parse the actual pixel.
      $dimension_keys = implode('|', array_keys($this->dimensions));
      $pattern_stage_size = '/\["style",\s?"(' . $dimension_keys . ')*",\s?\'([0-9]*(px|%))\'\]/';
      if (count($matches) > 1 && preg_match_all($pattern_stage_size, $matches[1], $matches_dimensions) && count($matches_dimensions) == 4 && count($matches_dimensions[1]) == count($matches_dimensions[2])) {
        $keys = $matches_dimensions[1];
        $values = $matches_dimensions[2];

        foreach (array_keys($this->dimensions) as $dimension_key) {
          $pos = array_search($dimension_key, $keys);
          if ($pos !== FALSE) {
            $this->dimensions[$dimension_key] = $values[$pos];
          }
        }
      }
    }

    // Minified version.
    // Find stage variable first.
    $stage_variable_match = array();
    $dimension_error_msg = '';
    if (preg_match('/(e\d*)=\'\$\{_Stage\}\'/', $content, $stage_variable_match)) {
      $stage_variable = $stage_variable_match[1];

      // Find property function calls for stage variable.
      $pattern_stage_style = '/;A1.A\(' . $stage_variable . '\)(\.P\([a-z0-9,%"_]+\))*;/i';
      $matches = array();
      if (preg_match($pattern_stage_style, $content, $matches)) {
        $properties = array();
        // Explode property values.
        if (preg_match_all('/P\(([a-z0-9,%"_]+)\)/i', $matches[0], $properties)) {
          foreach ($properties[1] as $property_string) {
            $property_values = explode(',', $property_string);
            // Full properties: P(h,280,_,_,p) or P(w,100,_,_,"%").
            if (count($property_values) == 5) {
              // Check if it's pixel or relative measure.
              $measure = '';
              if ($property_values[4] == '"%"') {
                $measure = '%';
              }
              else {
                if ($property_values[4] == 'p') {
                  $measure = 'px';
                }
                else {
                  continue;
                }
              }
            }
            // Compressed properties: P(xw,600).
            if (count($property_values) == 2) {
              $measure = 'px';
            }

            if (!empty($measure)) {
              switch ($property_values[0]) {
                case 'h':
                  $this->dimensions['height'] = $property_values[1] . $measure;
                  break;

                case 'w':
                  $this->dimensions['width'] = $property_values[1] . $measure;
                  break;

              }
            }
          }
        }
        else {
          $dimension_error_msg = t('Unable to explode stage properties.');
        }
      }
      else {
        $dimension_error_msg = t('Unable to spot stage property calls.');
      }
    }
    else {
      $dimension_error_msg = t('Unable to spot stage variable.');
    }

    if ($this->dimensions['height'] == 0 || $this->dimensions['width'] == 0) {
      drupal_set_message(t('Auto detection of stage dimensions failed: ') . $dimension_error_msg);
    }
    else {
      drupal_set_message(t('Auto detection of stage dimensions successful.'));
    }
  }

  /**
   * Alters the edge preloader file by injecting custom function calls.
   *
   * A call to loadResources will be replaced by a call to the Edge-Drupal
   * bridge JS function alterPreloadPaths() which will add path prefixes to
   * all resource and the call the original loader.
   * Calls to alterDomPaths() will be added to add path prefixes to the DOM
   * assets as well
   */
  protected function alterPreloaderJs() {
    $file = $this->preloaderFile;
    $content = file_get_contents($file);

    // Search for the loadResource function call.
    $load_pattern = '/loadResources\(aLoader,\s?doDelayLoad\);/';

    // Replace original call with call to custom function, alters preload paths.
    $load_replace = 'AdobeEdge.alterPreloadPaths(compId, aLoader, doDelayLoad, loadResources);';
    $content = preg_replace($load_pattern, $load_replace, $content);

    // Find the end of the main JS function and inject to function calls to
    // alter preContent and dlContent DOM, see JS function doc.
    // Expected structure (might be minified).
    // (function(compId){...})("MyProject").
    $dom_pattern = '/\}\)\(\s?\"' . $this->stageClass . '\"\);/';
    $dom_replace = 'AdobeEdge.alterDomPaths(preContent.dom, compId);' . "\n";
    $dom_replace .= 'AdobeEdge.alterDomPaths(dlContent.dom, compId);' . "\n";
    $dom_replace .= '})("' . $this->stageClass . '");';
    $content = preg_replace($dom_pattern, $dom_replace, $content);

    // Write new file.
    $fh = fopen($file, "w");
    fwrite($fh, $content);
    fclose($fh);
  }

  /**
   * Helper function which replaces a string in a file with str_replace.
   *
   * @param string $file
   *   The file to work with
   * @param string $search
   *   Search phrase
   * @param string $replace
   *   Replace phrase
   */
  protected function replaceStringInFile($file, $search, $replace) {
    $content = file_get_contents($file);

    $content = str_replace($search, $replace, $content);

    $fh = fopen($file, "w");
    fwrite($fh, $content);
    fclose($fh);
  }

  /**
   * Library handling.
   *
   * Checks which libraries are included in the project library directory and
   * copies them to the shared library folder.
   */
  protected function readLibs($overwrite = FALSE) {
    // Read edge common lib files.
    $edge_lib_files = file_scan_directory($this->tmpDir . '/edge_includes', '/.*/', array('recurse' => FALSE));

    // Set up shared folder if it doesn't exist.
    if (!file_exists($this->edgeDir . '/edge_includes')) {
      drupal_mkdir($this->edgeDir . '/edge_includes');
    }

    // Collect libs in array to save them in configuration.
    $lib_names = array();
    $lib_updates = $lib_ignored = $lib_added = array();
    foreach ($edge_lib_files as $lib) {
      // Only look at .js files.
      if (preg_match('/\.js$/', $lib->uri)) {
        // Check if lib already exists in shared folder.
        $exists = FALSE;
        if (file_exists($this->edgeDir . '/edge_includes/' . $lib->filename)) {
          $exists = TRUE;
        }

        if ($overwrite || !$exists) {
          $moved = file_unmanaged_move($lib->uri, $this->edgeDir . '/edge_includes', FILE_EXISTS_REPLACE);
          if (!$moved) {
            drupal_set_message(t('Library @lib could not be added/moved.', array('@lib' => check_plain($lib->filename))), 'warning');
          }
          if ($overwrite) {
            $lib_updates[] = $lib->filename;
          }
          else {
            $lib_added[] = $lib->filename;
          }
        }
        else {
          $lib_ignored[] = $lib->filename;
          file_unmanaged_delete($lib->uri);
        }
        // Add to lib array.
        $lib_names[] = $lib->filename;

        // Check for edge version, look for e.g. edge.0.1.6.min.js.
        if (empty($this->edgeVersion)) {
          $matches = array();
          preg_match('/edge\.([\.0-9]*)(?:\.min)?\.js$/', $lib->filename, $matches);
          if (count($matches) > 0) {
            $this->edgeVersion = $matches[1];
          }
        }
      }
    }

    // Output library file messages.
    $this->setFileMessages($lib_updates, 'The following libraries have been updated:');
    $this->setFileMessages($lib_added, 'The following libraries have been added:');
    $this->setFileMessages($lib_ignored, 'The following libraries are already present on the server, the existing versions will be used:');

    $this->libraries = $lib_names;
  }

  /**
   * Copies all asset files with certain extensions.
   *
   * Copies files with js,png,... extension from the tmp
   * project directory to the permanent project directory.
   */
  public static function moveEdgeFiles($src, $dest_schema) {
    // TODO: Test.
    $allowed_extensions = variable_get('edge_suite_allowed_asset_formats', 'js,png,jpg,gif,svg');
    // Strip spaces.
    $allowed_extensions = str_replace(' ', '', $allowed_extensions);
    // Replace commas.
    $allowed_extensions = str_replace(',', '|', $allowed_extensions);
    // Check if list is valid.
    if (!preg_match('/^([a-zA-Z0-9]{2,6}\|)*([a-zA-Z0-9]{2,6})$/', $allowed_extensions)) {
      drupal_set_message(t('Asset file extension list not valid, default extensions (js|png|jpg|gif|svg) will be used.'));
      $allowed_extensions = 'js|png|jpg|gif|svg';
    }

    $files = file_scan_directory($src, '/\.(' . $allowed_extensions . ')$/');
    // TODO: Does this work with nested folders?
    // Todo: Check mimetype. Are mime types calculated from the file extension?
    foreach ($files as $f) {
      $dir = dirname($f->uri);
      // Create folder structure in the permanent directory.
      // Get dir path relative to project tmp base path without leading slash.
      $rel_dir = substr($dir, strlen($src) + 1);
      // Skip published folder.
      if (substr($rel_dir, 0, 8) === 'publish/') {
        continue;
      }
      if (!file_exists($dest_schema . '/' . $rel_dir)) {
        drupal_mkdir($dest_schema . '/' . $rel_dir);
      }
      // TODO: Feedback if file couldn't be moved.
      $moved = file_unmanaged_move($f->uri, $dest_schema . '/' . $rel_dir);
    }
  }

}
