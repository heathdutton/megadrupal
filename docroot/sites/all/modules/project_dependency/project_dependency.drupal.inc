<?php

/**
 * @file
 * Provides Drupal implementation of project release info parsing and batch API.
 */

module_load_include('package.inc', 'project_dependency');

/**
 * Minimum API compatibility to process. We will not look at modules before
 * Drupal 6.
 */
define('PROJECT_DEPENDENCY_MINIMUM_DRUPAL_VERSION', 6);

/**
 * Directory for git repositories to be processed.
 *
 * These get checked out automatically, but the whole process goes faster
 * if it's already prepopulated with everything we need.
 *
 */
define('PROJECT_DEPENDENCY_SOURCECODE_DIRECTORY', variable_get('project_dependency_sourcecode_directory', '/tmp/project_dependency_sourcecode_directory'));

/**
 * Path to the upstream git repository.
 *
 * If the source code repository of any project is not found locally, it will be
 * fetched from this path.
 */
define('PROJECT_DEPENDENCY_UPSTREAM_REPOSITORY_PATH', variable_get('project_dependency_upstream_repository_path', 'git://git.drupal.org/project'));

/**
 * Parse all the .info files.
 *
 * @param $info_files
 *   List of component .info files.
 * @param $tag
 *   Release tag.
 *
 * @return
 *   Associative array of component information keyed by component name and
 *   merged with an array of defaults.
 */
function project_dependency_info_parse(array $info_files, $tag) {
  $defaults = array(
    'name' => 'Unknown',
    'description' => '',
    'dependencies' => array(),
    'test_dependencies' => array(),
  );

  $version = project_dependency_core_api_from_term($tag);
  $info_file_pattern = $version >7 ? '.info.yml' : '.info';

  $info = array();
  foreach ($info_files as $file) {
    $component = drupal_basename($file, $info_file_pattern);
    if ($version >= 8) {
      // Drupal 8 uses .info.yml files (except for releases before the switch).
      // We use Symfony module parsing since this modules is still D7 here.
      // When porting this to D8, use native info file parser.
      $info[$component] = \Symfony\Component\Yaml\Yaml::parse($file) +
        $defaults;
    }
    else {
      // D7+ caches the result in the parsing of the file, so must be reset.
      drupal_static_reset('drupal_parse_info_file');
      $info[$component] = drupal_parse_info_file($file) + $defaults;
    }

    // Change info keys to suite project_dependency_info.
    $info[$component]['title'] = $info[$component]['name'];
    $info[$component]['name'] = $component;
  }
  return $info;
}

/**
 * Get the selected release node that contains a component and is compatible
 * with an API.  Restrict possibilities to projects and modules with the same
 * name as the component name and then consider all releases where the module
 * name matches the component name.
 *
 * @param string $parsed_component
 *   Parsed component array.
 * @param int $api_tid
 *   Core API compatibility tid.
 *
 * @return object
 *   Release node object or FALSE if no release is found.
 */
function project_dependency_select_release($parsed_component, $api_tid) {
  if (isset($parsed_component['project'])) {
    // Looks for release with project name match.
    $project = $parsed_component['project'];
    $component = $parsed_component['name'];
  }
  else {
    // Looks for release with project name the same as the module name.
    $project = $parsed_component['name'];
    $component = $parsed_component['name'];
  }
  $sql = 'SELECT DISTINCT release_nid FROM {project_dependency_component} pdc
    LEFT JOIN {field_data_field_release_project} AS pr
      ON pdc.release_nid=pr.entity_id
    LEFT JOIN {field_data_field_project_machine_name} AS project
      ON pr.field_release_project_target_id=project.entity_id
    WHERE pdc.name = :cname
      AND project.field_project_machine_name_value = :pname';
  $params = array(':cname' => $component, ':pname' => $project);
  $project_release_nids = db_query($sql, $params)
    ->fetchCol();
  if (!empty($project_release_nids)) {
    $release = project_dependency_get_release($project_release_nids,
      $parsed_component, $api_tid);
    if ($release) {
      return $release;
    }
  }
  
  if (!isset($parsed_component['project'])) {
    // Look for a release from project that does not match the module name.
    $sql = 'SELECT DISTINCT release_nid FROM {project_dependency_component}
      WHERE name = :cname';
    $project_release_nids = db_query($sql, array(':cname' => $component))
      ->fetchCol();
    if (empty($project_release_nids)) {
      return FALSE;
    }

    $release = project_dependency_get_release($project_release_nids,
      $parsed_component, $api_tid);
  }

  return $release;
}

/**
 * Get the selected release node from the possible release nodes that contains
 * a component and is compatible with an API.  First look for a recommended
 * release.  If there is no recommended release then select the latest
 * release (highest nid).
 *
 * Sadly, the component may appear in more than one product, since there's no
 * guaranteed uniqueness of a submodule name. So we're just preferring the
 * most recent release node (highest nid).
 *
 * @param string $project_release_nids
 *   Possible project release nodes to consider.
 * @param string $parsed_component
 *   Parsed component array.
 * @param int $api_tid
 *   Core API compatibility tid.
 *
 * @return object
 *   Release node object or FALSE if no release is found.
 */
function project_dependency_get_release($project_release_nids,
$parsed_component, $api_tid) {

  // Select only supported releases of the right version.
  // Prefer recommended releases, highest major version and
  // most recent release (highest nid).
  $sql = db_select('project_release_supported_versions', 'supported')
    ->fields('supported', array('recommended_release'))
    ->condition('supported.tid', $api_tid)
    ->condition('supported.supported', 1)
    ->condition('supported.recommended_release', $project_release_nids, 'IN')
    ->orderBy('recommended', 'DESC')
    ->orderBy('major', 'DESC')
    ->orderBy('nid', 'DESC');
  if (isset($parsed_component['versions'])) {
    $items = $sql->execute();
    foreach ($items as $item) {
      $release = node_load($item->recommended_release);
      $version = project_dependency_release_version($release);
      $result = drupal_check_incompatibility($parsed_component, $version);
      if (is_null($result)) {
        return $release;
      }
      else {
        $project_release_nids = array_diff($project_release_nids,
          array($item->recommended_release));
      }
    }
  }
  else {
    $recommended = $sql->range(0, 1)
      ->execute()
      ->fetchField();
    if ($recommended) {
      $release = node_load($recommended);
      return $release;
    }
  }

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'project_release')
    ->fieldCondition('taxonomy_vocabulary_6', 'tid', $api_tid)
    ->propertyCondition('nid', $project_release_nids)
    ->propertyOrderBy('nid', 'DESC');
  if (isset($parsed_component['versions'])) {
    $project_releases = $query->execute();
    if (!empty($project_releases['node'])) {
      foreach ($project_releases['node'] as $project_release) {
        $release = node_load($project_release->nid);
        $version = project_dependency_release_version($release);
        $result = drupal_check_incompatibility($parsed_component, $version);
        if (is_null($result)) {
          return $release;
        }
      }
    }
  }
  else {
    $project_releases = $query->range(0, 1)
      ->execute();
    if (!empty($project_releases['node'])) {
      $project_release = current($project_releases['node']);
      $release_node = node_load($project_release->nid);
      return $release_node;
    }
  }

  return FALSE;
}

/**
 * Return version without the Drupal version prefix.
 *
 * @param array $release
 *   Release node.
 *
 * @return string
 *   Return version
 */
function project_dependency_release_version($release) {
  $version = project_dependency_field_value($release, 'field_release_version');
  $version = explode('-', $version);
  if ($version[count($version) - 1] == 'dev') {
    unset($version[count($version) - 1]);
  }
  if (count($version) > 1) {
    unset($version[0]);
  }
  $version = implode('-', $version);
  return $version;
}

/**
 * Attempt to determine the Drupal core API term.
 *
 * @param $node
 *   Node object.
 *
 * @return
 *   Core API term to which the node belongs, otherwise FALSE.
 */
function project_dependency_info_core_api($node) {
  $tid = project_release_get_release_api_tid($node);
  return taxonomy_term_load($tid);
}

/**
 * Given a core API taxonomy term name like '6.x', return the numeric core api
 * associated with it.
 *
 * @param $core_api_term_name
 */
function project_dependency_core_api_from_term($core_api_term_name) {
  return preg_replace('/\.[x0-9].*$/', '', $core_api_term_name);
}

/**
 * Check out a release and determine relevant information.
 *
 * Locate all .info files contained within the project, determine the names of
 * the module(s) contained within the project and store the dependencies for
 * later processing.
 *
 * @param $release
 *   A keyed array of release information, containing
 *   - 'tag' => The tag of the release (like 7.x-1.0)
 *   - 'uri' => The project shortname (like 'views')
 *   - 'nid' => The nid of the project release node
 *
 * @return
 *   An array of dependencies, or FALSE on failure.
 */
function project_dependency_info_batch_process_release(array $release) {
  // Use batch directory.
  if (!is_dir(PROJECT_DEPENDENCY_SOURCECODE_DIRECTORY)) {
    if (!mkdir(PROJECT_DEPENDENCY_SOURCECODE_DIRECTORY)) {
      watchdog('project_dependency', 'Failed to create PROJECT_DEPENDENCY_SOURCECODE_DIRECTORY %directory', array('%directory' => PROJECT_DEPENDENCY_SOURCECODE_DIRECTORY));
      return FALSE;
    }
  }

  // Checkout release.
  $url = PROJECT_DEPENDENCY_UPSTREAM_REPOSITORY_PATH . '/' . escapeshellcmd($release['repo']) . '.git';
  $tag = escapeshellcmd($release['tag']);
  $directory = $release['uri'];

  $work_tree = PROJECT_DEPENDENCY_SOURCECODE_DIRECTORY . "/$directory";
  $git_dir = "$work_tree/.git";
  $esc_git_dir = escapeshellarg($git_dir);
  $work_tree = escapeshellarg($work_tree);
  // If the config file in git dir doesn't exist, then remove the whole thing.
  if (!is_file("$git_dir/config")) {
    $command = "rm -rf $work_tree 2>&1";
    exec($command, $output, $status);
    if ($status) {
      watchdog('project_dependency', 'Failed to execute command %cmd; output=%output', array('%cmd' => $command, '%output' => implode(' ', $output)));
      return FALSE;
    }
    // Clone the repo
    $command = "git clone $url $work_tree 2>&1";
    exec($command, $output, $status);
    if ($status) {
      watchdog('project_dependency', 'Failed to execute git command %cmd; output=%output', array('%cmd' => $command, '%output' => implode(' ', $output)));
      return FALSE;
    }
  }
  // But if git dir already existed and looked healthy
  else {
    $command = "git --work-tree $work_tree --git-dir $esc_git_dir fetch 2>&1";
    exec($command, $output, $status);
    if ($status) {
      watchdog('project_dependency', 'Failed to execute git command %cmd; output=%output', array('%cmd' => $command, '%output' => implode(' ', $output)));
      return FALSE;
    }
  }

  // We need to make sure the directory is clean, so reset and clean
  // git reset --hard
  exec("git --work-tree $work_tree --git-dir $esc_git_dir reset --hard 2>&1", $output, $status);
  // git clean -fd
  exec("git --work-tree $work_tree --git-dir $esc_git_dir clean -fd 2>&1", $output, $status);
  // Make sure we don't have an existing branch by this name, or it won't be updated properly.
  // First get into a detached-head state so we know we can delete the branch.
  unset($output);
  exec("git --work-tree=$work_tree --git-dir $esc_git_dir rev-list --max-count=1 HEAD", $output, $status);
  $hash = $output[0];
  exec("git --work-tree=$work_tree --git-dir $esc_git_dir checkout '$hash' 2>&1", $output, $status);

  // Then authoritatively delete any branch by that name. OK for this to fail.
  unset($output);
  exec("git --work-tree=$work_tree --git-dir $esc_git_dir branch -D '$tag' 2>&1", $output, $status);

  // git checkout tag
  $command = "git --work-tree=$work_tree --git-dir $esc_git_dir checkout '$tag' 2>&1";
  unset($output);
  exec($command, $output, $status);
  if ($status) {
    watchdog('project_dependency', 'Failed to execute git command %cmd; output=%output', array('%cmd' => $command, '%output' => implode(' ', $output)));
    return FALSE;
  }

  // Scan checkout for .info files and create a list in the same format that
  // project release uses, so that standard API functions can be used.

  // Note that DRUPAL_PHP_FUNCTION_PATTERN croaks in file_scan_directory()
  // "WD php: Warning: ereg(): REG_ERANGE in file_scan_directory()"
  // $function_pattern = DRUPAL_PHP_FUNCTION_PATTERN;
  $function_pattern = '[a-zA-Z][a-zA-Z0-9_]*';
  $profile_info_files = file_scan_directory(PROJECT_DEPENDENCY_SOURCECODE_DIRECTORY . '/' . $directory, '/^' . $function_pattern . '\.(theme|profile)$/');
  // Date module has a date.theme file.
  $exceptions = variable_get('project_dependency_filename_type_exceptions',
    array('date.theme'));
  foreach ($profile_info_files as $filepath => $file) {
    if (in_array($file->filename, $exceptions)) {
      unset($profile_info_files[$filepath]);
    }
  }
  // If any profile or theme files are found, skip this release entirely.
  if (!empty($profile_info_files)) {
    return FALSE;
  }
  $info_files = array();
  $version = project_dependency_core_api_from_term($tag);
  $info_file_pattern = $version >7 ? '.info.yml' : '.info';

  $files = file_scan_directory(
    PROJECT_DEPENDENCY_SOURCECODE_DIRECTORY . '/' . $directory,
    "/^{$function_pattern}{$info_file_pattern}\$/"
  );

  foreach ($files as $file) {
    $info_files[] = $file->uri;
  }
  $info = project_dependency_info_parse($info_files, $release['tag']);

  $release_node = node_load($release['nid']);
  $api_term = project_dependency_info_core_api($release_node);
  foreach ($info as $component => $component_info) {
    foreach (array('dependencies', 'test_dependencies') as $dependency_type) {
      if (is_array($info[$component][$dependency_type])) {
        foreach ($info[$component][$dependency_type] as $key => $dependency) {
          $parsed_dependency = project_dependency_parse_dependency($dependency, $api_term->name);
          if (!isset($parsed_dependency['project'])) {
            $project = project_dependency_guess_project($parsed_dependency,
              $api_term->tid);
            if ($project) {
              $project_name = project_dependency_field_value($project, 'field_project_machine_name');
              $info[$component][$dependency_type][$key] = $project_name . ':' .
                $dependency;
            }
            else {
              // Probably should set an error message here.
              watchdog('project_dependency', 'Failed to find a release for component %component as dependency of %depending_component, release_nid=%release_nid', array('%component' => $parsed_dependency['name'], '%depending_component' => $component, '%release_nid' => $release_node->nid));
            }
          }
        }
      }
    }
  }

  $dependencies = array();
  foreach ($info as $module => $module_info) {
    $dependencies[$module] = $module_info['dependencies'];
    $test_dependencies[$module] = $module_info['test_dependencies'];
  }

  // Clear previous records for the release.
  project_dependency_info_package_clear($release['nid']);

  $components_in_release = array_keys($info);
  // Store the list of components contained by the project.
  $component_info = project_dependency_info_package_list_store($release['nid'], $info);
  foreach ($component_info as $component => $item) {
    // First process real, required dependencies
    $new_dependencies = array();
    if (is_array($info[$component]['dependencies'])) {
      foreach ($info[$component]['dependencies'] as $new_dependency) {
        $new_dependencies[$new_dependency] = array('external' => !in_array($new_dependency, $components_in_release));
      }
    }
    project_dependency_info_package_dependencies_store($item['release_nid'], $item['component_id'], $new_dependencies);

    // Now process test_dependencies as "RECOMMENDED"
    $test_dependencies = array();
    if (is_array($info[$component]['test_dependencies'])) {
      foreach ($info[$component]['test_dependencies'] as $new_dependency) {
        $test_dependencies[$new_dependency] = array('external' => !in_array($new_dependency, $components_in_release));
      }
    }
    project_dependency_info_package_dependencies_store($item['release_nid'], $item['component_id'], $test_dependencies, PROJECT_DEPENDENCY_DEPENDENCY_RECOMMENDED);
  }

  return $dependencies;
}


/**
 * Build dependency information for a single release node if it meets the
 * minimum Drupal major version API requirement.
 *
 * @param $shortname
 *   The shortname of the project.
 * @param $node
 *   The project_release node for the release.
 *
 * @return
 *   An array of dependency information or FALSE on failure.
 */
function project_dependency_process_release($shortname, $node) {
  $pid = project_dependency_field_value($node, 'field_release_project', 'target_id');
  $project = node_load($pid);
  $release = array(
    'tag' => project_dependency_field_value($node, 'field_release_vcs_label'),
    'uri' => $shortname,
    'nid' => $node->nid,
    'repo' => $project->versioncontrol_project['repo']->name,
  );

  $core_api_term = project_dependency_info_core_api($node);
  if (empty($core_api_term)) {
    return FALSE;
  }

  $core_api = project_dependency_core_api_from_term($core_api_term->name);
  if ($core_api < PROJECT_DEPENDENCY_MINIMUM_DRUPAL_VERSION) {
    return FALSE;
  }

  $dependencies = project_dependency_info_batch_process_release($release);
  return $dependencies;
}

/**
 * Find all dependencies (as release nids) of a given release nid and optional
 * components.
 *
 * If $depending_components are provided, only their dependencies will be
 * returned, rather than the dependencies of every component in the release.
 *
 * OK, I'm sorry this is a mindbending function. I've tried to compensate with
 * lots of comments, but suspect it's nowhere near adequate.
 *
 * This is a recursive, accumulative function, gathering the results we need in
 * $dependency_releases.
 *
 * @param $release_node
 *   release node.
 * @param $project_node
 *   related project node
 * @param $depending_components
 *   an array of component names, like
 *   array('views_ui', 'token', 'page_manager')
 * @param $dependency_releases (by reference)
 *   The array of release nids that are depended upon by the given components.
 * @param $recursion_level
 *   Recursion level of this call to prevent infinite recursion on some
 *   database problem.
 *
 * Same as  project_dependency_info_process_dependencies($rid, $modules[$module], $dependencies);
 */
function project_dependency_get_external_component_dependencies($release_node,
  $project_node, $depending_components, &$dependency_releases,
  $recursion_level = 0) {

  // Limit recursions for safety.
  if ($recursion_level > 10) {
    return;
  }

  // Check to see if input is garbage.
  if (!isset($release_node->nid)) {
    return;
  }

  // Get the api_term of the *depending* components.
  if (!($api_term = project_dependency_info_core_api($release_node))) {
    watchdog('project_dependency',
      'ERROR: No core release API term found for release nid=%nid',
      array('%nid' => $release_node->nid));
    return;
  }

  // If no list of depending components was passed in, create a list of all
  // components in this release.
  if (empty($depending_components)) {
    $sql = 'SELECT name FROM {project_dependency_component}
      WHERE release_nid = :release_nid';
    $depending_components = db_query($sql,
      array(':release_nid' => $release_node->nid))->fetchCol();
  }
  // Now we have a list of component names for which we need to find
  // external dependencies. Walk through them and for each one, find the
  // external (not in this release) dependencies.
  foreach ($depending_components as $depending_component) {
    // Get a list of keyed arrays the describe external dependencies.
    $external_dependencies = project_dependency_find_external_dependencies($release_node->nid, $depending_component);

    if (empty($external_dependencies)) {
      continue;
    }

    // Walk through the external dependencies to find releases they are shipped
    // in.
    foreach ($external_dependencies as $key => $dependency) {
      $parsed_dependency = project_dependency_parse_dependency(
        $dependency->dependency, $api_term->name);

      if (!isset($parsed_dependency['project'])) {
        $project = project_dependency_guess_project($parsed_dependency,
          $api_term->tid);
        
        if ($project) {
          $project_name = project_dependency_field_value($project,
            'field_project_machine_name');
          $external_dependencies[$key]->dependency = $project_name . ':' .
            $dependency->dependency;
        }
        else {
          watchdog('project_dependency', 'Failed to find a release for component %component as dependency of %depending_component, release_nid=%release_nid', array('%component' => $parsed_dependency['name'], '%depending_component' => $dependency->depender, '%release_nid' => $release_node->nid));
        }

      }
    }

    foreach ($external_dependencies as $component_info) {
      $dependency_component = $component_info->dependency;
      $parsed_component =
        project_dependency_parse_dependency($dependency_component, $api_term->name);
      $dependency_component = $parsed_component['name'];

      // Find the releases that component was shipped in.
      // We could hope that it would only be in one package/rid,
      // but there are probably cases (refactoring) where that is not
      // the case.
      $api_tid = $api_term->tid;
      $release = project_dependency_select_release($parsed_component, $api_tid);
      // In the case we found no possible release, we must halt processing and
      // continue to the next item
      if (empty($release)) {
        watchdog('project_dependency', 'Failed to find a release for component %component as dependency of %depending_component, release_nid=%release_nid', array('%component' => $dependency_component, '%depending_component' => $depending_component, '%release_nid' => $release_node->nid));
        continue;
      }

      // If the release has already been listed and is REQUIRED, we don't want
      // to overwrite it, so will not overwrite with a RECOMMENDED release.
      // Also will give first precedence to the first release found for component.
      if (empty($dependency_releases[$dependency_component]) || $dependency_releases[$dependency_component]['dependency_type'] == PROJECT_DEPENDENCY_DEPENDENCY_RECOMMENDED) {
        $pid = project_dependency_field_value($release, 'field_release_project', 'target_id');
        $project = node_load($pid);
        // We don't need to study the dependencies of the drupal project.
        $machine_name = project_dependency_field_value($project, 'field_project_machine_name');
        if ($machine_name != 'drupal') {
          $sql = 'SELECT component_id FROM {project_dependency_component}
            WHERE release_nid=:nid AND name=:name';
          $cid = db_query($sql, array(
            ':nid' => $release->nid,
            ':name' => $dependency_component)
          )->fetchField();
          // Add to the dependencies.
          $dependency_releases[$dependency_component] = array(
            'uri' => $machine_name,
            'pid' => $pid,
            'cid' => $cid,
            'release_nid' => $release->nid,
            'version' => project_dependency_field_value($release, 'field_release_version'),
            'tag' => project_dependency_field_value($release, 'field_release_vcs_label'),
            'version_major' => project_dependency_field_value($release, 'field_release_version_major'),
            'component' => $dependency_component,
            'dependency_type' => $component_info->dependency_type,
          );
          // Recurse to find dependencies of the discovered dependency, but only
          // for the component which discovered the dependent release.
          project_dependency_get_external_component_dependencies($release,
            $project, array($dependency_component), $dependency_releases,
            $recursion_level + 1);
        }
      }
    }
  }
}


/**
 * Given a release nid and component name, find all external dependencies for
 * that component.
 * @param $release_nid
 *   Release nid.
 * @param $component_name
 *   Component name.
 *
 * @return
 *   An array of component information.
 */
function project_dependency_find_external_dependencies($release_nid, $component_name) {
  $sql = 'SELECT DISTINCT(pdc.component_id), name AS depender, dependency,
      release_nid, dependency_type
    FROM {project_dependency_dependency} pdd
    INNER JOIN {project_dependency_component} pdc
      ON pdd.component_id = pdc.component_id
    WHERE  pdc.name = :pdcname AND pdc.release_nid = :pdcrelease_nid';
  $result = db_query($sql, array(':pdcname' => $component_name, ':pdcrelease_nid' => $release_nid));
  $rows = $result->fetchAll();
  return $rows;
}

/**
 * Parses a dependency for comparison by drupal_check_incompatibility().
 *
 * Clone of drupal_parse_dependency() with the project added in.  Needed until
 * drupal_parse dependency is fixed in Drupal 7 core.
 *
 * @param $dependency
 *   A dependency string, for example 'project:module (>=7.x-4.5-beta5, 3.x)'.
 *
 * @return
 *   An associative array with four keys:
 *   - 'project' includes the name of the project containing the module.
 *   - 'name' includes the name of the module to depend on.
 *   - 'original_version' contains the original version string (which can be
 *     used in the UI for reporting incompatibilities).
 *   - 'versions' is a list of associative arrays, each containing the keys
 *     'op' and 'version'. 'op' can be one of: '=', '==', '!=', '<>', '<',
 *     '<=', '>', or '>='. 'version' is one piece like '4.5-beta3'.
 *   Callers should pass this structure to drupal_check_incompatibility().
 *
 * @see drupal_check_incompatibility()
 */
function project_dependency_parse_dependency($dependency) {
  $value = array();
  // Split out the optional project name.
  if (strpos($dependency, ':')) {
    list($project_name, $dependency) = explode(':', $dependency);
    $value['project'] = $project_name;
  }
  // We use named subpatterns and support every op that version_compare
  // supports. Also, op is optional and defaults to equals.
  $p_op = '(?P<operation>!=|==|=|<|<=|>|>=|<>)?';
  // Core version is always optional: 7.x-2.x and 2.x is treated the same.
  $p_core = '(?:' . preg_quote(DRUPAL_CORE_COMPATIBILITY) . '-)?';
  $p_major = '(?P<major>\d+)';
  // By setting the minor version to x, branches can be matched.
  $p_minor = '(?P<minor>(?:\d+|x)(?:-[A-Za-z]+\d+)?)';
  $parts = explode('(', $dependency, 2);
  $value['name'] = trim($parts[0]);
  if (isset($parts[1])) {
    $value['original_version'] = ' (' . $parts[1];
    foreach (explode(',', $parts[1]) as $version) {
      if (preg_match("/^\s*$p_op\s*$p_core$p_major\.$p_minor/", $version, $matches)) {
        $op = !empty($matches['operation']) ? $matches['operation'] : '=';
        if ($matches['minor'] == 'x') {
          // Drupal considers "2.x" to mean any version that begins with
          // "2" (e.g. 2.0, 2.9 are all "2.x"). PHP's version_compare(),
          // on the other hand, treats "x" as a string; so to
          // version_compare(), "2.x" is considered less than 2.0. This
          // means that >=2.x and <2.x are handled by version_compare()
          // as we need, but > and <= are not.
          if ($op == '>' || $op == '<=') {
            $matches['major']++;
          }
          // Equivalence can be checked by adding two restrictions.
          if ($op == '=' || $op == '==') {
            $value['versions'][] = array('op' => '<', 'version' => ($matches['major'] + 1) . '.x');
            $op = '>=';
          }
        }
        $value['versions'][] = array('op' => $op, 'version' => $matches['major'] . '.' . $matches['minor']);
      }
    }
  }
  return $value;
}

/**
 * Guess the project for a dependency.
 *
 * Sadly, the module may appear in more than one project, since there's no
 * guaranteed uniqueness of a module name.
 *
 * @param string $parsed_dependency
 *   Parsed dependency array.
 * @param int $api_tid
 *   Core API compatibility tid.
 *
 * @return object
 *   Project node object or FALSE if no release is found.
 */
function project_dependency_guess_project($parsed_dependency, $api_tid) {
  $release = project_dependency_select_release($parsed_dependency, $api_tid);
  if (empty($release)) {
    return FALSE;
  }
  $pid = project_dependency_field_value($release, 'field_release_project', 'target_id');
  $project = node_load($pid);
  return $project;
}
