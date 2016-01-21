<?php
/**
 * @file
 * Edge suite composition functionality.
 */


/* COMPOSITION DEFINITION FUNCTIONS */

/**
 * Creates a new composition from an archive.
 *
 * Extracts the given archive, processes all the files and adds entries
 *
 * @param object $file
 *   The archive file (as Drupal file object) which contains the composition
 * @param bool $replace_libs
 *   Set to true if libraries of the composition should overwrite existing libs.
 * @param int $definition_id_current
 *   Set definition id of existing composition id to replace the composition
 *   with the new composition from the archive.
 *
 * @return int
 *   Returns the definition id of the build definition
 * @throws Exception
 * @todo Implement composition definition replace functionality.
 */
function edge_suite_comp_create($file, $replace_libs = FALSE, $definition_id_current = 0) {
  $success = FALSE;
  $definition_id = 0;

  // Check if the definition is being updated.
  $update = FALSE;
  if ($definition_id_current > 0) {
    $definition_old = edge_suite_comp_load_definition($definition_id_current);
    if ($definition_old != NULL) {
      $update = TRUE;
    }

    // Delete the stored archive.
    $archive_old = EDGE_SUITE_COMP_ARCHIVE_DIR . '/' . $definition_old->project_name_unique . '.' . $definition_old->archive_extension;
    file_unmanaged_delete($archive_old);

    // Delete the unpacked files, means the project directory.
    $project_dir = EDGE_SUITE_COMP_PROJECT_DIR . '/' . $definition_old->project_name_unique;
    if (is_dir($project_dir)) {
      file_unmanaged_delete_recursive($project_dir);
    }
  }

  // Set up project paths.
  global $user;
  $project_tmp_name = 'clean_u' . $user->uid . '_t' . REQUEST_TIME;
  $project_tmp_path = EDGE_SUITE_COMP_PROJECT_DIR . '/' . $project_tmp_name;

  // Check if main edge directory and project dir exists, if not create it.
  if (!is_dir(EDGE_SUITE_COMP_PROJECT_DIR)) {
    if (!is_dir(EDGE_SUITE_PUBLIC_DIR)) {
      drupal_mkdir(EDGE_SUITE_PUBLIC_DIR);
    }
    drupal_mkdir(EDGE_SUITE_COMP_PROJECT_DIR);
  }

  // Set up composition builder.
  $edge_build = new EdgeCompositionBuilder(EDGE_SUITE_PUBLIC_DIR, EDGE_SUITE_COMP_PROJECT_DIR);
  try {
    // The composition is not being replaced right away, it will be build first
    // and then copied to the right location if needed.
    $edge_build->processArchive($file->uri, $project_tmp_name, FALSE, $replace_libs);

    $project_name = $edge_build->getProjectName();

    if (!empty($project_name)) {
      // Check if private folder has been configured.
      if (!file_stream_wrapper_valid_scheme("private")) {
        file_delete($file);
        throw new Exception(t("Private file path needs to be configured."));
      }

      // Get archive file extension so it can be saved in the DB. The name of
      // the archive is build from data present the definition.
      $ext = pathinfo($file->filename, PATHINFO_EXTENSION);

      $info = array(
        'version' => $edge_build->getEdgeVersion(),
      );
      $info += $edge_build->getDimensions();

      // Add composition definition to the DB.
      $def_record = array(
        'project_name' => $project_name,
        'composition_id' => $edge_build->getCompositionId(),
        'archive_extension' => $ext,
        'info' => $info,
        'uid' => $user->uid,
        'created' => REQUEST_TIME,
        'changed' => REQUEST_TIME,
      );


      // Check if this is an update or insert and modify accordingly.
      if ($update) {
        $def_record['definition_id'] = $definition_id_current;
        drupal_write_record('edge_suite_composition_definition', $def_record, 'definition_id');
      }
      else {
        drupal_write_record('edge_suite_composition_definition', $def_record);
      }

      $definition_id = $def_record['definition_id'];
      $def_name = edge_suite_comp_unique_name($def_record);

      if (!$update) {
        $instance_config = array();

        // Create default block instance.
        $inst_record = array(
          'definition_id' => $definition_id,
          'label' => 'Default',
          'type' => 'block',
          'config' => $instance_config,
        );

        drupal_write_record('edge_suite_composition_instance', $inst_record);
        $instance_id = $inst_record['instance_id'];
      }

      $project_path = EDGE_SUITE_COMP_PROJECT_DIR . '/' . $def_name;
      rename($project_tmp_path, $project_path);

      // Set up new name for archive.
      $archived_file = $def_name . '.' . $ext;

      // Check if edge source project folder exists, if not create it.
      if (!is_dir(EDGE_SUITE_COMP_ARCHIVE_DIR)) {
        if (!is_dir(EDGE_SUITE_PRIVATE_DIR)) {
          drupal_mkdir(EDGE_SUITE_PRIVATE_DIR);
        }
        drupal_mkdir(EDGE_SUITE_COMP_ARCHIVE_DIR);
      }

      // The file gets copied by 'unmanaged' so it gets pulled out from
      // drupal control, the managed tmp file itself gets deleted below.
      // TODO: check if we are replacing?
      file_unmanaged_copy($file->uri, EDGE_SUITE_COMP_ARCHIVE_DIR . '/' . $archived_file, FILE_EXISTS_REPLACE);

      $success = TRUE;
    }
  } catch (Exception $e) {
    // Clean out all files.
    drupal_set_message($e->getMessage(), 'error');

    // Make sure source extraction directory gets cleaned out.
    $edge_build->cleanup();

    // Delete project files. Make sure both directories get deleted.
    if (file_exists($project_tmp_path)) {
      file_unmanaged_delete_recursive($project_tmp_path);
    }
    if (isset($project_path) && file_exists($project_path)) {
      file_unmanaged_delete_recursive($project_path);
    }
    // Delete DB entries if any have been created.
    if (isset($definition_id)) {
      db_delete('edge_suite_composition_definition')
        ->condition('definition_id', $definition_id)
        ->execute();
      if (isset($instance_id)) {
        db_delete('edge_suite_composition_instance')
          ->condition('instance_id', $instance_id, '=')
          ->execute();
      }
    }
    // TODO: Libraries might have been copied. Critical?
  }

  // Delete the tmp file no matter what. An unmanaged copy should be in the
  // source folder by now (to be able to rebuild).
  file_delete($file);

  if ($success) {
    drupal_set_message(t("@project was successfully imported.", array('@project' => $project_name)));
  }
  else {
    drupal_set_message(t("Errors occurred while creating the uploaded project. The import was most likely corrupted."), 'error');

  }

  return $definition_id;
}

/**
 * Builds a unique name for a composition definition.
 *
 * The unique name will be build through the user id, the definition id and
 * the project name, to provide maximal readability.
 *
 * @param object $definition
 *   Composition definition
 *
 * @return null|string
 *   Returns the name or null if the definition is not valid
 */
function edge_suite_comp_unique_name($definition) {
  // TODO: change signature.
  $name = NULL;
  if (isset($definition['definition_id']) && isset($definition['project_name'])) {
    $name = strtolower($definition['project_name']);
    $name .= '_' . $definition['definition_id'];
  }
  return $name;
}

/**
 * Deletes all the current composition files and rebuilds them.
 *
 * As Edge is a moving target me might need to rebuild compositions for
 * compatibility reasons in the future or simply in case of file corruption.
 * The rebuilding process is done via the stored archives.
 *
 * @param int $definition_id
 *   Id of the edge composition definition that should be rebuild
 * @param bool $replace_libs
 *   Set to true if libraries of the composition should overwrite existing libs.
 */
function edge_suite_comp_rebuild($definition_id, $replace_libs = FALSE) {

  // Load composition definition that needs to be rebuild.
  $definition = edge_suite_comp_load_definition($definition_id);

  // Rebuild composition.
  $edge_build = new EdgeCompositionBuilder(EDGE_SUITE_PUBLIC_DIR, EDGE_SUITE_COMP_PROJECT_DIR);
  try {
    // Get path of stored archive.
    $file = EDGE_SUITE_COMP_ARCHIVE_DIR . '/' . $definition->project_name_unique . '.' . $definition->archive_extension;

    // Rebuild composition.
    $edge_build->processArchive($file, $definition->project_name_unique, TRUE, $replace_libs);
    $project_name = $edge_build->getProjectName();

    $info = array(
      'version' => $edge_build->getEdgeVersion(),
    );
    $info += $edge_build->getDimensions();

    // Update composition definition.
    $def_record = array(
      'definition_id' => $definition->definition_id,
      'project_name' => $project_name,
      'composition_id' => $edge_build->getCompositionId(),
      'info' => $info,
      'changed' => REQUEST_TIME,
    );
    drupal_write_record('edge_suite_composition_definition', $def_record, 'definition_id');

    drupal_set_message(t("@project was successfully rebuild.", array('@project' => $project_name)));
  } catch (Exception $e) {
    $edge_build->cleanup();
    drupal_set_message(t("Errors occurred while rebuilding @project.", array('@project' => $definition->project_name)), 'error');
    drupal_set_message($e->getMessage(), 'error');
  }
}

/**
 * Rebuilds all compositions.
 */
function edge_suite_comp_rebuild_all() {
  // Get all definition ids.
  $query = db_select('edge_suite_composition_definition', 'd')
    ->fields('d')
    ->execute();

  // Rebuild all definitions.
  if ($query->rowCount() > 0) {
    foreach ($query as $row) {
      edge_suite_comp_rebuild($row->definition_id);
    }
  }
}

/**
 * Load a composition definition and optionally all its instances.
 *
 * @param int $definition_id
 *   Id of the composition definition
 * @param bool $load_instances
 *   Load all instances of the definition
 *
 * @return mixed|null
 *   Composition definition as an object
 */
function edge_suite_comp_load_definition($definition_id, $load_instances = TRUE) {
  $def_query = db_select('edge_suite_composition_definition', 'd')
    ->fields('d')
    ->condition('definition_id', $definition_id, '=')
    ->execute();

  $definition = NULL;
  if ($def_query->rowCount() > 0) {
    // Grab composition.
    $definition = $def_query->fetchObject();
    $definition->info = (object) unserialize($definition->info);

    $inst_query = db_select('edge_suite_composition_instance', 'i')
      ->fields('i')
      ->condition('definition_id', $definition_id, '=')
      ->execute();

    $definition->instances = array();
    if ($inst_query->rowCount() > 0) {
      $definition->instances = $inst_query->fetchAllAssoc('instance_id');
    }

    // TODO:
    $definition->project_name_unique = edge_suite_comp_unique_name((array) $definition);
  }
  return $definition;
}

/**
 * Load a composition instance.
 *
 * The instance with the specified id is loaded. The related definition gets
 * loaded as well and is flatly integrated into the instance.
 *
 * @param int $instance_id
 *   Id of the composition instance.
 *
 * @return object|null
 *   The instance including the related definition data.
 */
function edge_suite_comp_load_instance($instance_id) {
  $query = db_select('edge_suite_composition_instance', 'i');
  $query->leftJoin('edge_suite_composition_definition', 'd', 'i.definition_id = d.definition_id');
  $result = $query->fields('i')
    ->fields('d')
    ->condition('i.instance_id', $instance_id, '=')
    ->execute();

  $instance = NULL;
  if ($result->rowCount() > 0) {
    // Grab composition.
    $instance = $result->fetchObject();
    // TODO:
    $instance->project_name_unique = edge_suite_comp_unique_name((array) $instance);
    $instance->config = (object) unserialize($instance->config);
    if ($instance->info != NULL) {
      $instance->info = (object) unserialize($instance->info);
    }
  }
  return $instance;
}

/**
 * Fully delete a composition from the DB and filesystem.
 *
 * Deletes the definition, all instances and all files of the composition.
 *
 * @param int $definition_id
 *   Id of the composition definition
 *
 * @return bool
 *   True if composition was deleted, false otherwise
 */
function edge_suite_comp_delete_definition($definition_id) {
  // Load composition definition to get all the necessary data.
  $definition = edge_suite_comp_load_definition($definition_id);

  // Delete the unpacked files, means the project directory.
  $project_dir = EDGE_SUITE_COMP_PROJECT_DIR . '/' . $definition->project_name_unique;
  if (is_dir($project_dir)) {
    file_unmanaged_delete_recursive($project_dir);
  }

  // Delete the stored archive.
  $archive = EDGE_SUITE_COMP_ARCHIVE_DIR . '/' . $definition->project_name_unique . '.' . $definition->archive_extension;
  if (file_exists($archive)) {
    file_unmanaged_delete($archive);
  }

  // Delete all instances.
  foreach ($definition->instances as $instance_id => $instance) {
    edge_suite_comp_delete_instance($instance_id);
  }

  // Delete composition definition from table.
  $num_definitions = db_delete('edge_suite_composition_definition')
    ->condition('definition_id', $definition_id)
    ->execute();

  return $num_definitions > 0 ? TRUE : FALSE;
}

/**
 * Delete a particular composition instance.
 *
 * @param int $instance_id
 *   Id of the composition instance.
 *
 * @return bool
 *   True if instance was deleted, false otherwise
 */
function edge_suite_comp_delete_instance($instance_id) {
  // Delete block entries. If blocks are not unregistered new edge blocks
  // sometimes don't show up in the overview listing. This is a bit strange as
  // there seems to be no API call for modules to unregister blocks.
  db_delete('block')
    ->condition('delta', 'edge_suite_composition_block_' . $instance_id)
    ->execute();

  // Delete all instances from table.
  $num_instances = db_delete('edge_suite_composition_instance')
    ->condition('instance_id', $instance_id, '=')
    ->execute();

  return $num_instances > 0 ? TRUE : FALSE;
}


/**
 * Render the stage with the given values and configuration.
 *
 * @param object $instance
 *   The full instance.
 *
 * @return string
 *   Fully rendered output
 */
function edge_suite_comp_render($instance) {
  $group = 1000;

  // Add drupal-edge bridging javascript.
  drupal_add_js(drupal_get_path('module', 'edge_suite') . '/edge_drupal.js',
    array(
      'group' => $group,
      'weight' => -1,
    )
  );

  // Get preloader filepath.
  $preloader_file = EDGE_SUITE_COMP_PROJECT_DIR . '/' . $instance->project_name_unique . '/' . $instance->project_name . '_edgePreload.js';
  drupal_add_js($preloader_file,
    array(
      'group' => $group,
      'weight' => 1,
    )
  );

  // Get real base path to edge directory.
  $edge_path = edge_suite_schema_to_relative_real_path(EDGE_SUITE_PUBLIC_DIR);
  $edge_project_path = edge_suite_schema_to_relative_real_path(EDGE_SUITE_COMP_PROJECT_DIR);

  $div_id = 'Stage_' . $instance->project_name_unique . '_' . $instance->instance_id;

  // Register every composition with its path within the AdobeEdge variable so
  // the path can be used by the drupal-edge bridging JS code to set up paths
  // to all the libraries and media assets correctly.
  // TODO: Find a better/cleaner/more generic way to do this. See static cache.
  $js = 'window.AdobeEdge = window.AdobeEdge || {};';
  $js .= 'window.AdobeEdge.pathPrefix = window.AdobeEdge.pathPrefix || {};';
  $js .= "AdobeEdge.pathPrefix.libs = '" . $edge_path . "';";
  $js .= "AdobeEdge.pathPrefix.comps = AdobeEdge.pathPrefix.comps || {};";
  $js .= "AdobeEdge.pathPrefix.comps['" . $instance->composition_id . "'] = '" . $edge_project_path . "/" . $instance->project_name_unique . "';";

  drupal_add_js($js,
    array(
      'type' => 'inline',
      'group' => $group,
      'weight' => -2,
    )
  );

  // Build the configuration string.
  $data = "";
  if (isset($instance->config->json_config) && !empty($instance->config->json_config)) {
    // Replace tokens.
    if (module_enable(array('token'))) {
      // Try to retrieve a node for the current path.
      $node = menu_get_object();
      $instance->config->json_config = token_replace($instance->config->json_config, array('node' => $node));
    }

    $data = '<!-- Injected Data Begin -->' . "\n";
    $data .= '<script class="data" type="text/data">' . "\n";
    $data .= $instance->config->json_config . "\n";
    $data .= '</script>' . "\n";
    $data .= '<!-- Injected Data End -->' . "\n";
  }

  // Add dimension if it was possible to parse them. This avoids jumping of the
  // empty divs when using multiple instances on one page.
  $height = '';
  if (isset($instance->info->height) && intval($instance->info->height) > 0) {
    $height = 'height:' . $instance->info->height . ';';
  }

  $width = '';
  if (isset($instance->info->width) && intval($instance->info->width) > 0) {
    $width = 'width:' . $instance->info->width . ';';
  }

  // Put everything together.
  $output = '<div id="' . $div_id . '" style="' . $height . $width . '" class="' . $instance->composition_id . '">' . $data . '</div>';

  return $output;
}

/**
 * Loads and renders a composition instance.
 *
 * @param int $instance_id
 *   Id of the composition instance.
 * @param bool $full_html_page
 *   True if a full HTML page should be rendered.
 *
 * @return string
 *   Fully rendered output
 */
function edge_suite_comp_view($instance_id, $full_html_page = FALSE) {
  // Load composition instance.
  $instance = edge_suite_comp_load_instance($instance_id);

  if (!empty($instance)) {
    if ($full_html_page) {
      // Get javascript storage and clean it out as its not needed.
      $javascript = &drupal_static('drupal_add_js', array());
      // Add a dummy entry so defaults are not being set.
      $javascript = array('0' => NULL);

      // Render composition instance.
      $content = edge_suite_comp_render($instance);

      // Output full HTML structure.
      $full_html = edge_suite_render_full_page($content);
      print $full_html;

      // Return null to get a clean exit.
      return NULL;
    }
    else {
      if (isset($instance->config->embed_mode) && $instance->config->embed_mode == 'iframe') {
        $url = base_path() . 'edge_suite/iframe/' . $instance->instance_id;
        // Get dimensions and set iframe.
        $width = isset($instance->info->width) ? $instance->info->width : 0;
        $height = isset($instance->info->height) ? $instance->info->height : 0;
        $frame = '<iframe src="' . $url . '" name="composition-' . $instance->instance_id . '" width="' . $width . '" height="' . $height . '" scrolling="no" marginheight="0" marginwidth="0" frameborder="0"></iframe> ';
        return $frame;
      }
      else {
        $content = edge_suite_comp_render($instance);
        return $content;
      }
    }
  }
  else {
    drupal_set_message(t('Could not load composition instance with ID @id', array('@id' => $instance_id)));
    return "";
  }
}

/**
 * Renders a full HTML page for the given stage content.
 *
 * @param string $stage_content
 *   The rendered stage content.
 *
 * @return string
 *   Returns the full HTML document, ready to be printed.
 *
 * @todo Move into template file?
 */
function edge_suite_render_full_page($stage_content) {
  $js = drupal_get_js();

  $html = '<!DOCTYPE html>' . "\n";
  $html .= '<html>' . "\n";
  $html .= '<head>' . "\n";
  $html .= '<meta http-equiv="X-UA-Compatible" content="IE=edge"/>' . "\n";
  $html .= $js;
  $html .= '</head>' . "\n";
  $html .= '<body>' . "\n";
  $html .= $stage_content . "\n";
  $html .= '</body>' . "\n";
  $html .= '</html>' . "\n";

  return $html;
}
