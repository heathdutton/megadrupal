<?php

/**
 * @file build.instance.inc
 * Build an instance using a site alias
 */

/**
 * Implements hook_drush_engine_type_info().
 */
function instance_drush_engine_type_info() {
  return array(
    'classes' => array(),
    'build' => array(),
  );
}

/**
 * Implements hook_drush_engine_ENGINE().
 */
function instance_drush_engine_classes() {
  return array(
    'paths' => array(
      'description' => "Non-Drupal path handlers",
    ),
  );
}

/**
 * Implements hook_drush_engine_ENGINE().
 */
function instance_drush_engine_build() {
  return array(
    'switcher' => array(
      'description' => "Switch between build making methods",
    ),
    'makefile' => array(
      'description' => "Build a site using Drush make",
    ),
    'composer' => array(
      'description' => "Build a site using composer",
    ),
  );
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_instance_build_validate($alias) {
  // Check alias exists
  if(!($alias_info = drush_sitealias_get_record($alias))) {
    return drush_set_error("DRUSH_INSTANCE_SITEALIAS_NONE", "Alias $alias does not exist");
  }
  // Check it's a local alias
  elseif (array_key_exists('remote-host', $alias_info)) {
    return drush_set_error("DRUSH_INSTANCE_SITEALIAS_REMOTE", "Sorry, drush instance does not currently support remote hosts like $alias.");
  }

  // Check it has a source for building the site
  $uses_build_type = NULL;
  if (array_key_exists('instance', $alias_info)
    && array_key_exists('sources', $alias_info['instance'])) {
      foreach (array("composer", "makefile") as $build_type) {
      if (array_key_exists($build_type, $alias_info['instance']['sources'])) {
        $uses_build_type = $build_type;
      }
    }
  }
  if (!$uses_build_type) {
    drush_set_error("DRUSH_INSTANCE_GIT_REPO_MAKEFILE", "No buildfile source specified for $alias: you should specify \$alias['instance']['sources']['makefile'] or 'composer' as a makefile-like definition");
  }
  $alias_info['_build_type'] = $uses_build_type;

  // Add the build/rebuild status
  $alias_info['_is_rebuild'] = drush_get_option('rebuild');
  // Add the option for which useful templates to build
  $alias_info['_has_templates'] = drush_get_option('templates');
  // Unset these and any other options, so drush make doesn't pick up on them
  $alias_info['command_line'] = array();
  $commands = instance_drush_command();
  foreach(array_keys($commands['instance-build']['options']) as $option) {
    $alias_info['command_line'][$option] = drush_get_option($option);
    drush_unset_option($option);
  }

  // Check whether or not the site exists
  if (file_exists($alias_info['root'])) {
    // This is OK, if rebuild is specified
    if (!$alias_info['_is_rebuild']) {
      drush_set_error("DRUSH_INSTANCE_ALREADY_EXISTS", "Instance $alias already exists at " . $alias_info['root']);
    }
  }
  // If rebuild IS specified, but there's no existing build, raise error
  elseif ($alias_info['_is_rebuild']) {
    drush_set_error("DRUSH_INSTANCE_ALREADY_EXISTS", "Instance $alias does not exist at " . $alias_info['root'] . "; cannot rebuild");
  }

  // If there's an error by this point, we shouldn't continue
  if (drush_get_error() == DRUSH_FRAMEWORK_ERROR) {
    return;
  }

  // Makefile-as-version-control branch/tag/revision override
  // Refspec types copied in priority order from make_download_git()
  $all_refspec_types = array('tag', 'revision', 'branch');
  foreach($all_refspec_types as $refspec_type) {
    if ($refspec = $alias_info['command_line'][$refspec_type]) {
      $alias_info['instance']['sources']['makefile'][$refspec_type]
        = $refspec;
      // Unset every other type, so the command-line override prevails
      foreach($all_refspec_types as $unset_type) {
        if ($unset_type != $refspec_type) {
          unset($alias_info['instance']['sources']['makefile'][$unset_type]);
        }
      }
      // Once we've found one, we're done
      break;
    }
  }

  // Defaults
  // Makefile name
  if (!array_key_exists('makefile_path', $alias_info['instance'])) {
    $alias_info['instance']['makefile_path'] = 'instance.make';
  }
  // If there's no extra source for sites/default, assume it's the same as the makefile
  if (!array_key_exists('sites_default', $alias_info['instance']['sources'])) {
    $alias_info['instance']['sources']['sites_default'] =
      $alias_info['instance']['sources']['makefile'];
  }

  // Validation of paths array
  // used to be only run during rebuild
  _drush_instance_build_validate_paths($alias_info);

  // Save the site alias in context for use in the actual command
  drush_set_context("DRUSH_INSTANCE", $alias_info);
}

/**
 * Implements drush_hook().
 */
function drush_instance_build($alias) {
  // Get all the config we'll need first
  $alias_info = drush_get_context("DRUSH_INSTANCE");
  $instance = $alias_info['instance'];
  $rebuild = (bool)$alias_info['_is_rebuild'];

  // Get build switcher and build based on build type (makefile, composer).
  drush_include_engine("build", "switcher");
  $builder = instance_build_switcher_get_builder($alias_info['_build_type'], $alias_info);
  try {
    $builder->build();
  } catch (Exception $e) {
    if ($e instanceof DrushInstanceBuildException) {
      return drush_set_error($e->type, $e->_message);
    }
    throw $e;
  }

  $alias_info['_old_dir'] = $builder->old_dir;
 
  // Now move all preserved files over: we do this as late
  // as possible to avoid e.g. moving large filetrees across partitions,
  // as we can assume that e.g. /var/www/* is all on one partition (?)
  // but the same can't be said for e.g. /tmp
  // Also change permissions if required at this point
  $path_success = TRUE;
  if ($instance['paths']) {
    drush_include_engine("classes", "paths");
    $path_handler = new DrushInstancePaths($alias_info, $instance['paths'], $rebuild);
    if (!$path_handler->handleAllPaths()) {
      return drush_set_error("Your codebase has been rebuilt, but some paths have had problems. It could therefore be in a partially built state, so please check the error log above.");
    }
  }

  // Build some template files
  if ($alias_info['_has_templates']) {
    $templates = split(",",
      $alias_info['_has_templates'] == "all" ? "vhost,crontab" : $alias_info['_has_templates']);
    foreach($templates as $template_key) {
      _drush_instance_render_to_file($template_key, substr($alias, 1), $alias_info);
    }
  }

  // Finally, we're done!
  drush_log(dt("Finished !alias: deployed to !root",
            array("!alias" => $alias, "!root" => $alias_info['root'])),
            "ok");
}

/**
 * Generic handler for all permissions
 */
function _drush_instance_perm_path($alias_info, $path, $config) {
  // If path doesn't exist: for now, quit silently. TODO.
  // Ultimately we'd like to know *why* it doesn't exist (typically rebuild).
  if (!file_exists($alias_info['root'] . "/$path")) {
    return TRUE;
  }

  // Specific handler based on type
  $type = $config['permissions']['type'];
  $handler = "_drush_instance_perm_path_$type";
  if (!function_exists($handler)) {
    drush_set_error("No handler method exists for setting perms on paths of type=$type");
    return FALSE;
  }
  return $handler($alias_info, $path, $config['permissions']);
}

/**
 * Helper function to make a file writeable by all
 */
function _drush_instance_perm_path_chmod_all($alias_info, $path, $config) {
  return chmod($alias_info['root'] . "/$path", 0757);
}

/**
 * Helper function to make a file writeable by group
 */
function _drush_instance_perm_path_chmod_group($alias_info, $path, $config) {
  return chmod($alias_info['root'] . "/$path", 0775);
}

/**
 * Stub-out of theme_render_template().
 *
 * Render templates into usable configuration files
 */
function _drush_instance_theme_render_template($template_file, $variables) {
  extract($variables, EXTR_SKIP); // Extract the variables to a local namespace
  ob_start(); // Start output buffering
  include dirname(__FILE__) . "/$template_file"; // Include the template file
  $contents = ob_get_contents(); // Get the contents of the buffer
  ob_end_clean(); // End buffering and discard
  return $contents; // Return the contents
}

/**
 * Render one of our template files using the above
 */
function _drush_instance_render_to_file($key, $alias_name, $alias_info) {
  // Does template file exist
  $file = "templates/$key.tpl.php";
  if (!file_exists(dirname(__FILE__) . "/$file")) {
    return drush_log("Template $key cannot be rendered as it doesn't exist", "warning");
  }
  // Render template to string
  $content = _drush_instance_theme_render_template(
    $file,
    array("alias_name" => substr($alias, 1), "alias_info" => $alias_info)
  );

  file_put_contents($alias_info['root'] . "/sites/$key", $content);
}
