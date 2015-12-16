<?php

/**
 * @file
 * Drush command for packaging files for release nodes.
 *
 * @author Derek Wright (http://drupal.org/user/46549)
 */

function project_release_drush_command() {
  return array(
    'release-package-run' => array(
      'description' => 'Run queued release packaging. Creates packages AND writes release history XML files.',
    ),
    'release-package' => array(
      'description' => 'Package files for release nodes.',
      'arguments' => array(
        'type' => '"branch" or "tag", required.',
        'project_id' => 'Project, either a Node ID or machine name. Omit to build all projects.'
      ),
      'required-arguments' => TRUE,
    ),
    'release-create-history' => array(
      'description' => 'Creates XML release history for projects.',
      'arguments' => array(
        'project_id' => 'Project, either a Node ID or machine name. Omit to build all projects.',
      ),
    ),
    'release-check-status' => array(
      'description' => 'Check the status and visibility for releases.',
      'arguments' => array(
        'project_id' => 'Project, either a Node ID or machine name. Omit to check all projects.',
      ),
    ),
  );
}

function project_release_drush_help($section) {
  switch ($section) {
    case 'meta:project_release:title':
      return dt('Project release commands');
  }
}

function drush_project_release_release_package_run() {
  $queue = DrupalQueue::get('project_release_package');
  drush_log(dt('@number items in queue.', array('@number' => $queue->numberOfItems())), 'ok');

  while ($item = $queue->claimItem()) {
    drush_project_release_release_package($item->data['type'], $item->data['project']);
    drush_project_release_release_create_history($item->data['project']);
    $queue->deleteItem($item);
  }
}

function drush_project_release_release_package_validate($type = NULL, $project_id = NULL) {
  if ($type !== 'tag' && $type !== 'branch') {
    return drush_set_error('INVALID_TYPE', dt('First type must be "branch" or "tag".'));
  }
}

function drush_project_release_release_package($type, $project_id) {
  // Force the right umask while this script runs, so that everything is created
  // with sane file permissions.
  umask(0022);

  // Load the include file for packager-related helper functions.
  module_load_include('inc', 'project_release', 'includes/packager');

  // Find the project.
  $project_node = project_load($project_id);
  if ($project_node === FALSE) {
    return drush_set_error('INVALID_PROJECT', dt('ERROR: Project ID @id not found', array('@id' => $project_id)));
  }
  if ($project_node->status != NODE_PUBLISHED) {
    return drush_set_error('UNPUBLISHED_PROJECT', dt('ERROR: Project @id not published', array('@id' => $project_id)));
  }
  if (!$project_node->field_project_has_releases[$project_node->language][0]['value']) {
    return drush_set_error('NO_RELEASES_PROJECT', dt('ERROR: Project @id does not have releases', array('@id' => $project_id)));
  }

  $num_built = 0;
  $num_considered = 0;

  // Find release nodes.
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', project_release_release_node_types())
    ->fieldCondition('field_release_project', 'target_id', $project_node->nid)
    ->fieldCondition('field_release_build_type', 'value', $type === 'tag' ? 'static' : 'dynamic')
    ->execute();
  if (empty($result)) {
    drush_log(dt('No !type releases found for @id.', array('!type' => $type, '@id' => $project_id)), 'warning');
    return;
  }

  // If needed, prod module_implements() to recognize our
  // project_release_watchdog() implementation.
  if (!in_array('project_release', module_implements('watchdog'))) {
    module_implements('watchdog', FALSE, TRUE);
  }

  foreach (array_keys($result['node']) as $release_nid) {
    // We don't want to waste too much RAM by leaving all these loaded nodes
    // in RAM, so we reset the node_load() cache each time we call it.
    $release_node = node_load($release_nid, NULL, TRUE);
    if (empty($release_node)) {
      watchdog('package_error', "Can't load release node for release ID %nid", array('%nid' => $release_nid), WATCHDOG_ERROR);
      continue;
    }

    // If file is not empty, a tag release is already packaged.
    if ($type === 'tag' && !empty($release_node->field_release_files)) {
      continue;
    }
    // If published and file is not empty, a branch release is already
    // packaged. Published releases may be repackaged.
    if ($type === 'branch' && $release_node->status != NODE_PUBLISHED && !empty($release_node->field_release_files)) {
      continue;
    }

    // Find the packager plugin.
    $packager = project_release_get_packager_plugin($release_node, drush_tempdir());
    if ($packager === FALSE) {
      watchdog('package_error', "Can't find packager plugin to use for %release", array('%release' => $release_node->title), WATCHDOG_ERROR);
      continue;
    }

    drush_log(dt('Packaging @title (nid @nid)', array('@title' => $release_node->title, '@nid' => $release_node->nid)));

    // Delete existing log messages.
    db_delete('project_release_package_errors')
      ->condition('nid', $release_node->nid)
      ->execute();
    project_release_watchdog(NULL, $release_node->nid);

    $files = array();
    $rval = $packager->createPackage($files);
    $num_considered += 1;

    switch ($rval) {
      case 'success':
      case 'rebuild':
        project_release_packager_update_node($release_node, $files);
        module_invoke_all('project_release_create_package', $project_node, $release_node);
        $num_built += 1;
        if ($rval === 'rebuild') {
          $msg = '%release_title has changed, re-packaged.';
        }
        else {
          $msg = 'Packaged %release_title.';
        }
        watchdog('package_' . $type, $msg, array('%release_title' => $release_node->title), WATCHDOG_INFO, l(t('View'), 'node/' . $release_node->nid));
        break;
    }

    // Write any log entries.
    project_release_watchdog(NULL, FALSE);
  }

  if ($num_built > 0 || $type === 'branch') {
    drush_log(dt('Done packaging !type releases for @project_short_name: !num_built built, !num_considered considered.', array('@project_short_name' => $project_node->field_project_machine_name[$project_node->language][0]['value'], '!type' => $type, '!num_built' => $num_built, '!num_considered' => $num_considered)), 'ok');
  }
}

/**
 * Drush-only implementation of hook_watchdog(). Write out messages to
 * project_release_package_errors table.
 *
 * @param $log_entry
 *   An associative array, see hook_watchdog() for keys.
 * @param $new_nid
 *   Set release nid for log entries to be stored with. FALSE to reset and
 *   write out any entries.
 */
function project_release_watchdog($log_entry = NULL, $new_nid = NULL) {
  static $nid = FALSE;
  static $messages = array();

  if (is_null($new_nid)) {
    if (!is_array($log_entry['variables'])) {
      $log_entry['variables'] = array();
    }
    // Invoked via hook_watchdog(), store message if there is a current node.
    if ($nid !== FALSE && $log_entry['severity'] < WATCHDOG_NOTICE) {
      $messages[] = t($log_entry['message'], $log_entry['variables']);
    }
  }
  else {
    // Clearing out nid, write out messages.
    if ($new_nid === FALSE && !empty($messages)) {
      db_insert('project_release_package_errors')->fields(array(
        'nid' => $nid,
        'messages' => serialize($messages),
      ))->execute();
      $messages = array();
    }

    // Store new nid.
    $nid = $new_nid;
  }
}

function drush_project_release_release_create_history($project_id = NULL) {
  // Force the right umask while this script runs, so that everything is created
  // with sane file permissions.
  umask(0022);

  $api_vid = variable_get('project_release_api_vocabulary', 0);

  if (is_null($project_id)) {
    drush_log(dt('Generating XML release history files for all projects.'), 'ok');
  }
  else {
    $project_node = project_load($project_id);
    if ($project_node === FALSE) {
      return drush_set_error('INVALID_PROJECT', dt('Project ID @id not found.', array('@id' => $project_id)));
    }
  }

  if (isset($project_node)) {
    project_release_history_generate_project_xml($project_node);
  }
  else {
    // Generate all xml files for projects with releases.
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', project_project_node_types())
    ->fieldCondition('field_project_type', 'value', 'full')
    ->execute();
    if (!empty($result)) {
      foreach ($result['node'] as $project) {
        project_release_history_generate_project_xml(node_load($project->nid, NULL, TRUE));

        // Reset caches so we aren't leaking memory.
        entity_get_controller('field_collection_item')->resetCache();
        drupal_static_reset('field_language');
        drupal_lookup_path('wipe');
        if (module_exists('versioncontrol')) {
          foreach (versioncontrol_get_backends() as $backend) {
            $backend->getController('repo')->resetCache();
          }
        }
      }
    }
  }

  // If we're operating on all projects, generate the huge list, too.
  if (is_null($project_id)) {
    $result = db_query('SELECT n.title, n.nid, n.status, n.type, mn.field_project_machine_name_value AS machine_name, u.name AS username FROM {node} n INNER JOIN {field_data_field_project_machine_name} mn ON n.nid = mn.entity_id INNER JOIN {users} u ON n.uid = u.uid');
    $xml = '';
    foreach ($result as $project) {
      $xml .= " <project>\n";
      $xml .= '  <title>' . check_plain($project->title) . "</title>\n";
      $xml .= '  <short_name>' . check_plain($project->machine_name) . "</short_name>\n";
      $xml .= '  <link>' . url('node/' . $project->nid, array('absolute' => TRUE)) . "</link>\n";
      $xml .= '  <dc:creator>' . check_plain($project->username) . "</dc:creator>\n";
      $xml .= '  <type>' . check_plain($project->type) . "</type>\n";
      $term_query = db_query('SELECT v.name AS vocab_name, v.vid, td.name AS term_name, td.tid FROM {taxonomy_index} ti INNER JOIN {taxonomy_term_data} td ON ti.tid = td.tid INNER JOIN {taxonomy_vocabulary} v ON td.vid = v.vid WHERE ti.nid = :nid', array(':nid' => $project->nid));
      $xml_terms = '';
      foreach ($term_query as $term) {
        $xml_terms .= '   <term><name>' . check_plain($term->vocab_name) . '</name>';
        $xml_terms .= '<value>' . check_plain($term->term_name) . "</value></term>\n";
      }
      $xml_terms .= _project_release_compatibility_term($project);

      if (!empty($xml_terms)) {
        $xml .= "  <terms>\n" . $xml_terms . "  </terms>\n";
      }
      if (!$project->status) {
        // If it's not published, we can skip the rest for this project.
        $xml .= "  <project_status>unpublished</project_status>\n";
      }
      else {
        $xml .= "  <project_status>published</project_status>\n";
        // Include a list of API terms if available.
        if (!empty($api_vid)) {
          $term_query = db_query('SELECT DISTINCT td.name FROM field_data_field_release_project rp INNER JOIN taxonomy_index ti ON ti.nid = rp.entity_id INNER JOIN taxonomy_term_data td ON ti.tid = td.tid AND td.vid = :api_vid WHERE rp.field_release_project_target_id = :nid ORDER BY td.weight ASC', array(':api_vid' => $api_vid, ':nid' => $project->nid));
          $xml_api_terms = '';
          foreach ($term_query as $api_term) {
            $xml_api_terms .= '   <api_version>' . check_plain($api_term->name) . "</api_version>\n";
          }
          if (!empty($xml_api_terms)) {
            $xml .= "  <api_versions>\n" . $xml_api_terms . "  </api_versions>\n";
          }
        }
      }

      $xml .= " </project>\n";
    }
    if (empty($xml)) {
      return drush_set_error('NO_PROJECTS', dt('No projects found on this server.'));
    }
    return project_release_history_write_xml($xml);
  }
}

/**
 * Generate the XML history file for a given project name and API compatibility
 * term.
 *
 * If a history file already exists for this combination, this function will
 * generate a new history and atomically replace the old one (currently, just
 * logs to watchdog for debugging).
 *
 * @param $project
 *   Project node entity.
 * @param $api_tid
 *   Taxonomy id (tid) of the API compatibility term to use, or NULL if
 *   all terms are considered.
 */
function project_release_history_generate_project_xml($project) {
  $vocabularies = taxonomy_vocabulary_get_names();

  drush_log(dt('Generating release history for @machine_name.', array('@machine_name' => $project->field_project_machine_name[$project->language][0]['value'])), 'ok');

  $releases_xml = array(
    'all' => '',
  );

  if ($project->status) {
    foreach (project_release_query_releases($project->nid) as $release) {
      $output = " <release>\n";
      $output .= '  <name>' . check_plain($release->title) . "</name>\n";
      $output .= '  <version>' . check_plain($release->field_release_version[$release->language][0]['value']) . "</version>\n";
      if (!empty($release->field_release_vcs_label[$release->language][0]['value'])) {
        $output .= '  <tag>' . check_plain($release->field_release_vcs_label[$release->language][0]['value']) . "</tag>\n";
      }
      foreach (array('major', 'minor', 'patch', 'extra') as $type) {
        $vers_type = 'version_' . $type;
        // 0 is a totally legitimate value for any of these version fields, so
        // we need to test with isset() instead of !empty(). However, we do
        // *not* want to include anything in the release history XML if the
        // value is an empty string.
        if (isset($release->{'field_release_' . $vers_type}[$release->language][0]['value'])
            && $release->{'field_release_' . $vers_type}[$release->language][0]['value'] !== '') {
          $output .= "  <$vers_type>" . check_plain($release->{'field_release_' . $vers_type}[$release->language][0]['value']) . "</$vers_type>\n";
        }
      }

      // Need to fetch list of files for this release
      if (!empty($release->field_release_files[$release->language])) {
        $files = array_map('field_collection_field_get_entity', $release->field_release_files[$release->language]);
      }
      else {
        $files = array();
      }

      if ($release->status) {
        // Published, so we should include the links.
        $output .= "  <status>published</status>\n";
        $output .= '  <release_link>' . url('node/' . $release->nid, array('absolute' => TRUE)) . "</release_link>\n";
        if (!empty($files[0])) {
          $download_link = field_view_value('field_collection_item', $files[0], 'field_release_file', $files[0]->field_release_file[$release->language][0]);
          $output .= '  <download_link>' . file_create_url($download_link['#file']->uri) . "</download_link>\n";
        }
      }
      else {
        $output .= "  <status>unpublished</status>\n";
      }
      // We want to include the rest of these regardless of the status.
      if (!empty($files[0])) {
        $output .= '  <date>' . check_plain($files[0]->field_release_file[$release->language][0]['timestamp']) . "</date>\n";
        $output .= '  <mdhash>'  . check_plain($files[0]->field_release_file_hash[$release->language][0]['value']) . "</mdhash>\n";
        $output .= '  <filesize>' . check_plain($files[0]->field_release_file[$release->language][0]['filesize']) . "</filesize>\n";
      }

      $output .= "  <files>\n";
      foreach ($files as $file) {
        $output .= "   <file>\n";
        if ($release->status) {
          $download_link = field_view_value('field_collection_item', $file, 'field_release_file', $file->field_release_file[$release->language][0]);
          $output .= '    <url>' . file_create_url($download_link['#file']->uri) . "</url>\n";
        }
        $file_parts = explode('.', $file->field_release_file[$release->language][0]['filename']);
        $archive_type = array_pop($file_parts);
        // See if the previous extension is '.tar' and if so, add that, so we
        // see 'tar.gz' or 'tar.bz2' instead of just 'gz' or 'bz2'.
        $previous_ext = array_pop($file_parts);
        if ($previous_ext == 'tar') {
          $archive_type = $previous_ext . '.' . $archive_type;
        }
        else {
          // Put it back on the array, so our profile logic below still works.
          array_push($file_parts, $previous_ext);
        }
        $output .= '    <archive_type>' . $archive_type . "</archive_type>\n";

        /// @todo: This is a drupal.org-specific hack.
        /// @see http://drupal.org/node/1003764
        if ($project->type === 'project_distribution') {
          $variant_chunk = array_pop($file_parts);
          if (strrpos($variant_chunk, 'no-core') !== FALSE) {
            $variant = 'projects';
          }
          elseif (strrpos($variant_chunk, 'core') !== FALSE) {
            $variant = 'full';
          }
          else {
            $variant = 'profile-only';
          }
          $output .= '    <variant>' . $variant . "</variant>\n";
        }
        $output .= '    <md5>' . check_plain($file->field_release_file_hash[$release->language][0]['value']) . "</md5>\n";
        $output .= '    <size>' . check_plain($file->field_release_file[$release->language][0]['filesize']) . "</size>\n";
        $output .= '    <filedate>' . check_plain($file->field_release_file[$release->language][0]['timestamp']) . "</filedate>\n";
        $output .= "   </file>\n";
      }
      $output .= "  </files>\n";

      $tids = array();
      $api_tid = project_release_get_release_api_tid($release);
      foreach (taxonomy_get_vocabularies() as $vocabulary) {
        if (isset($release->{'taxonomy_' . $vocabulary->machine_name}[$release->language])) {
          foreach ($release->{'taxonomy_' . $vocabulary->machine_name}[$release->language] as $term) {
            if ($term['tid'] != $api_tid) {
              $tids[] = $term['tid'];
            }
          }
        }
      }
      $terms_output = '';
      if (!empty($tids)) {
        foreach (taxonomy_term_load_multiple($tids) as $term) {
          $terms_output .= '   <term><name>' . check_plain($vocabularies[$term->vocabulary_machine_name]->name) . '</name>';
          $terms_output .= '<value>' . check_plain($term->name) . "</value></term>\n";
        }
      }
      if ($terms_output !== '') {
        $output .= "  <terms>\n" . $terms_output . "  </terms>\n";
      }
      $output .= " </release>\n";

      $releases_xml['all'] .= $output;
      if (isset($releases_xml[$api_tid])) {
        $releases_xml[$api_tid] .= $output;
      }
      else {
        $releases_xml[$api_tid] = $output;
      }
    }
  }

  foreach ($releases_xml as $api_tid => $release_xml) {
    $xml = '<title>' . check_plain($project->title) . "</title>\n";
    $xml .= '<short_name>' . check_plain($project->field_project_machine_name[$project->language][0]['value']) . "</short_name>\n";
    $xml .= '<dc:creator>' . check_plain($project->name) . "</dc:creator>\n";
    $xml .= '<type>' . check_plain($project->type) . "</type>\n";

    if ($api_tid === 'all') {
      $api_version = 'all';
    }
    else {
      $api_term = taxonomy_term_load($api_tid);
      if ($api_term === FALSE) {
        continue;
      }
      $api_version = check_plain($api_term->name);
    }
    $xml .= '<api_version>' . $api_version . "</api_version>\n";

    if ($project->status) {
      $project_status = 'published';
      if ($api_tid !== 'all') {
        // Include the info about supported and recommended major versions.
        $query = db_query('SELECT major, supported, recommended FROM {project_release_supported_versions} WHERE nid = :nid AND tid = :tid AND (supported = 1 OR recommended = 1)', array(':nid' => $project->nid, ':tid' => $api_tid));
        $supported_majors = array();
        $recommended_major = NULL;
        foreach ($query as $version_info) {
          if ($version_info->supported) {
            $supported_majors[] = $version_info->major;
          }
          if ($version_info->recommended) {
            $recommended_major = $version_info->major;
          }
        }
        if (isset($recommended_major)) {
          $xml .= '<recommended_major>' . $recommended_major . "</recommended_major>\n";
        }
        if (empty($supported_majors)) {
          $project_status = 'unsupported';
        }
        else {
          $xml .= '<supported_majors>' . implode(',', $supported_majors) . "</supported_majors>\n";
          // To avoid confusing existing clients, include <default_major>, too.
          $xml .= '<default_major>' . min($supported_majors) . "</default_major>\n";
        }
      }
      $xml .= '<project_status>' . $project_status . "</project_status>\n";
      $xml .= '<link>' . url('node/' . $project->nid, array('absolute' => TRUE)) . "</link>\n";

      // To prevent the update(_status) module from having problems parsing the
      // XML, the terms need to be at the end of the information for the project.
      $tids = array();
      foreach (taxonomy_get_vocabularies() as $vocabulary) {
        if (isset($project->{'taxonomy_' . $vocabulary->machine_name}[$project->language])) {
          foreach ($project->{'taxonomy_' . $vocabulary->machine_name}[$project->language] as $term) {
            $tids[] = $term['tid'];
          }
        }
      }
      if (!empty($tids)) {
        $xml .= "  <terms>\n";
        $xml .= _project_release_compatibility_term($project);
        foreach (taxonomy_term_load_multiple($tids) as $term) {
          $xml .= '   <term><name>' . check_plain($vocabularies[$term->vocabulary_machine_name]->name) . '</name>';
          $xml .= '<value>' . check_plain($term->name) . "</value></term>\n";
        }
        $xml .= "  </terms>\n";
      }
    }
    else {
      $xml .= "<project_status>unpublished</project_status>\n";
    }

    if (!empty($release_xml)) {
      $xml .= "<releases>\n" . $release_xml . "</releases>\n";
    }

    project_release_history_write_xml($xml, $project, $api_version);
  }
}

/**
 * Add backward compatibility project type term.
 *
 * Drupal.org-specific hack, see https://drupal.org/node/2126123. Drush
 * expects taxonomy terms which no longer exist on Drupal.org.
 */
function _project_release_compatibility_term($project) {
  $extra_terms = array(
    'project_core' => "   <term><name>Projects</name><value>Drupal core</value></term>\n",
    'project_distribution' => "   <term><name>Projects</name><value>Distributions</value></term>\n",
    'project_module' => "   <term><name>Projects</name><value>Modules</value></term>\n",
    'project_theme' => "   <term><name>Projects</name><value>Themes</value></term>\n",
    'project_theme_engine' => "   <term><name>Projects</name><value>Theme engines</value></term>\n",
    'project_translation' => "   <term><name>Projects</name><value>Translations</value></term>\n",
  );

  return isset($extra_terms[$project->type]) ? $extra_terms[$project->type] : '';
}

/**
 * Write out the XML history for a given project and version to a file.
 *
 * @param $xml
 *   String containing the XML representation of the history.
 * @param $project
 *   An object containing (at least) the title and field_project_machine_name
 *   of project.
 * @param $api_version
 *   The API compatibility version the history is for.
 */
function project_release_history_write_xml($xml, $project = NULL, $api_version = NULL) {
  // Dublin core namespace according to http://dublincore.org/documents/dcmi-namespace/
  $dc_namespace = 'xmlns:dc="http://purl.org/dc/elements/1.1/"';
  $full_xml = '<?xml version="1.0" encoding="utf-8"?>' . "\n";
  if (is_null($project)) {
    // We are outputting a global project list.
    $project_dir = 'project-list';
    $filename = 'project-list-all.xml';
    $full_xml .= '<projects ' . $dc_namespace . ">\n" . $xml . "</projects>\n";
  }
  else {
    // Setup the filenames we'll be using.  Normally, we'd have to be extra
    // careful with field_project_machine_name to avoid malice here, however,
    // that's validated on the project edit form to prevent any funny
    // characters, so that much is safe.  The rest of these paths are just from
    // the global variables at the top of this script, so we can trust those.
    // The only one we should be careful of is the taxonomy term for the API
    // compatibility.
    $project_dir = $project->field_project_machine_name[$project->language][0]['value'];
    $filename = $project->field_project_machine_name[$project->language][0]['value'] . '-' . strtr($api_version, '/', '_') . '.xml';
    $full_xml .= '<project ' . $dc_namespace . ">\n" . $xml . "</project>\n";
  }

  // Make sure we've got the right project-specific subdirectory.
  $project_dir = file_build_uri(variable_get('project_release_history_root', 'release-history') . '/' . $project_dir);
  if (!file_prepare_directory($project_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    $wrapper = file_stream_wrapper_get_instance_by_uri($project_dir);
    return drush_set_error('NO_DIRECTORY', dt('@path does not exist or is not writable.', array('@path' => $wrapper->realpath())));
  }

  // Write out file.
  if (file_unmanaged_save_data($full_xml, $project_dir . '/' . $filename, FILE_EXISTS_REPLACE)) {
    module_invoke_all('project_release_history_xml_written', $project, $api_version);
  }
  else {
    $wrapper = file_stream_wrapper_get_instance_by_uri($project_dir . '/' . $filename);
    return drush_set_error('FILE_WRITE', dt('Can\'t write to @file.', array('@file' => $wrapper->realpath())));
  }
}

/**
 * Check status (recommended, latest, update status, etc) for releases.
 */
function drush_project_release_release_check_status($project_id = NULL) {
  $api_vid = variable_get('project_release_api_vocabulary', 0);

  if (is_null($project_id)) {
    drush_log(dt('Checking release status for all projects.'), 'ok');
    project_release_release_check_status_all();
  }
  else {
    $project_node = project_load($project_id);
    if ($project_node === FALSE) {
      return drush_set_error('INVALID_PROJECT', dt('Project ID @id not found.', array('@id' => $project_id)));
    }
    else {
      drush_log(dt('Checking release status for @title [@machine_name]', array('@title' => $project_node->title, '@machine_name' => $project_node->field_project_machine_name[$project_node->language][0]['value'])), 'ok');
    }
    project_release_release_check_status_project($project_node);
  }
}

/**
 * Check release status for all branches of all projects.
 *
 * Queries {project_release_supported_versions} for all branches across all
 * projects, groups the branches per-project, and invokes a helper function,
 * project_release_compute_update_status_multiple(), to actually compute the
 * update status of every release on each branch, update the release download
 * table (once per project), etc.
 *
 * @see project_release_compute_update_status_multiple()
 */
function project_release_release_check_status_all() {
  $results = db_query("SELECT nid, tid, major FROM {project_release_supported_versions} WHERE supported = 1");
  $branch_count = 0;
  $project_count = 0;
  $branches = array();
  $current_nid = 0;
  foreach ($results as $branch) {
    $branch_count++;
    if ($current_nid != $branch->nid) {
      if (!empty($branches)) {
        project_release_compute_update_status_multiple($current_nid, $branches);
        $project_count++;
        $branches = array();
      }
      $current_nid = $branch->nid;
    }
    $branches[] = array('api_tid' => $branch->tid, 'major' => $branch->major);
  }
  project_release_compute_update_status_multiple($current_nid, $branches);
  $project_count++;
  drush_log(dt('Checked release status for @branch_count branches across @project_count projects.', array('@branch_count' => $branch_count, '@project_count' => $project_count)), 'ok');
}

/**
 * Check the release status on a single project.
 *
 * Queries {project_release_supported_versions} for all branches for a single
 * project and invokes project_release_compute_update_status_multiple() once
 * to compute the release update status, rebuild the download table, etc.
 *
 * @see project_release_compute_update_status_multiple()
 */
function project_release_release_check_status_project($project) {
  $results = db_query("SELECT tid, major FROM {project_release_supported_versions} WHERE supported = 1 AND nid = :nid", array(':nid' => $project->nid));
  $branches = array();
  foreach ($results as $branch) {
    $branches[] = array('api_tid' => $branch->tid, 'major' => $branch->major);
  }
  project_release_compute_update_status_multiple($project->nid, $branches);
  drush_log(dt('Checked release status for @count branches.', array('@count' => count($branches))), 'ok');
}
