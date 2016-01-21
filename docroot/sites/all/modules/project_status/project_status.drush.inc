<?php

// avoid errors under old versions of drush
if (!defined('DRUSH_BOOTSTRAP_MAX')) {
  define('DRUSH_BOOTSTRAP_MAX', DRUSH_BOOTSTRAP_DRUPAL_FULL);
}

/*
   define drush command
*/
function project_status_drush_command() {
  $items = array();
  $items['project-status-rebuild'] = array(
      'callback' => 'drush_project_status_rebuild',
      'description' => dt('Rebuild module cache in system table'),
      'options' => array(
        'pipe' => dt('Machine-readable module status information from the Drupal system table'),
        ),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      );
  $items['project-status'] = array(
      'callback' => 'drush_project_status_report',
      'description' => dt('List modules not in use in a platform'),
      'options' => array(
        'site-modules'       => dt('Show modules in sites directories, not just sites/all/ and sites/default/'),
        'show-used'          => dt('Show modules in use, in addition to unused modules'),
        'no-unused'          => dt('Do not show modules which are not in use (implies --show-used)'),
        'no-group-projects'  => dt("Don't group projects"),
        'filter'             => dt('Only report modules whose names match this string (regex)'),
        'filter-strict'      => dt('Only report modules strictly matching --filter'),
        'site-filter'        => dt('Only report sites whose names match this string (regex)'),
        'site-filter-strict' => dt('Only report sites strictly matching --site-filter'),
        'rebuild'            => dt('Rebuild module cache in system table for each site'),
        'pipe'               => dt('List of modules not used on any listed platform, one per line'),
        ),
      'arguments' => array(
        'platform' => dt('Required. One or more directories containing Drupal platforms to scan.'),
        ),
      'examples' => array(
        'drush project-status /var/aegir/platforms/pressflow-6.20-prod' => 'List modules not enabled on any site in this platform',
        'drush project-status platforms/pressflow-6.20-prod platforms/pressflow-6.20-stage --show-used --no-unused --site-modules' => 'List all modules enabled on these platforms, listed by platform and site',
        ),
      'bootstrap' => DRUSH_BOOTSTRAP_MAX,
      'aliases' => array('psr'),
      );
  return $items;
}

/*
   define help message for drush command
*/
function project_status_drush_help($section) {
  switch ($section) {
    case 'drush:project-status-rebuild':
      return dt("Calls module_rebuild_cache() to rebuild system table with module details.");
    case 'drush:project-status':
      return dt("Reports which modules are being used by sites in the specified platforms. By default this report lists modules in sites/all/ and sites/default/ which are not used (disabled or not installed) in any site in these platforms.");
  }
}

/*
   implement command to rebuild drush's internal cache of module locations and status
*/
function drush_project_status_rebuild() {
  drush_include_engine('drupal', 'environment');
  $modules = drush_get_modules();
  $debug = drush_get_option('debug', FALSE);
  $pipe = drush_get_option('pipe', FALSE);
  if ($debug) {
    drush_print_r($modules);
  }
  if ($pipe) {
    $status_report = array();
    foreach ($modules as $name => $module) {
      $status_report[] = sprintf("%s\t%s\t%s\t%s\t%s", $name, $module->filename,
          $module->status, $module->schema_version, serialize($module->info));
    }
    drush_print_pipe($status_report);
  }
}

/*
   implement command to report module status
*/
function drush_project_status_report() {
  $debug = drush_get_option('debug', FALSE);
  $scan_site_modules = drush_get_option('site-modules', FALSE);
  $no_group_projects = drush_get_option('no-group-projects', FALSE);
  $no_unused = drush_get_option('no-unused', FALSE);
  $show_used = drush_get_option('show-used', FALSE);
  $filter = drush_get_option('filter', FALSE);
  $filter_strict = drush_get_option('filter-strict', FALSE);
  $site_filter = drush_get_option('site-filter', FALSE);
  $site_filter_strict = drush_get_option('site-filter-strict', FALSE);
  $rebuild = drush_get_option('rebuild', FALSE);
  $root = drush_get_option('root', FALSE);
  $pipe = drush_get_option('pipe', FALSE);
  if ($no_unused && !$show_used) {
    $show_used = TRUE;
  }
  $args = array_unique(func_get_args());
  if ($root) {
    $args[] = $root;
  }
  if (count($args) == 0) {
    drush_log(dt('No platform directories specified, using current directory.'), 'notice');
    $args[] = '.';
  }
  $platform_version = FALSE;
  $total_sites = 0;
  $modules = array();
  foreach ($args as $platform_dir) {
    $tmp = realpath($platform_dir);
    $platform_dir = realpath($platform_dir);
    if (!is_dir($platform_dir)) {
      drush_log(dt('@dir is not a directory!', array('@dir' => $platform_dir)), 'error');
      continue;
    }
    if (!is_dir($platform_dir."/sites")) {
      drush_log(dt('@dir is not a sites directory!', array('@dir' => $platform_dir."/sites")), 'error');
      continue;
    }
    drush_shell_exec("drush --root=$platform_dir php-eval 'print drush_drupal_version()'");
    $version = drush_shell_exec_output();
    $version = array_pop($version);
    if (!$platform_version) {
      $platform_version = (int) $version;
      // prior to drupal 6 the system table didn't cache this information
      if ($platform_version < 6) {
        $rebuild = TRUE;
      }
    }
    elseif ($platform_version != (int) $version) {
      drush_log(dt('@dir is a different major version (@this_version) of Drupal than a previously scanned platform (@first_version)!',
      array('@dir' => $platform_dir,
      '@this_version' => (int) $version,
      '@first_version' => $platform_version)), 'error');
      return;
    }
    drush_print(dt("Scanning Drupal $version platform @platform_dir", array("@platform_dir" => $platform_dir)));
    $sites = scandir($platform_dir."/sites");
    // clean up sites array
    $tmp = array();
    foreach ($sites as $site) {
      if (in_array($site, array('.', '..', 'all', 'default'))) {
        continue;
      }
      $site_dir = $platform_dir.'/sites/'.$site;
      if (!is_dir($site_dir)) {
        continue;
      }
      if (is_link($site_dir)) {
        continue;
      }
      if (!is_readable($site_dir.'/settings.php')) {
        continue;
      }
      if ($site_filter) {
        if ($site_filter_strict && $site_filter != $site) {
          continue;
        }
        elseif (!preg_match("/$site_filter/", $site)) {
          continue;
        }
      }
      $tmp[] = $site;
    }
    $sites = $tmp;
    // include default only if there are no other sites and we're not trying to filter
    if (!count($sites) && !$site_filter) {
      $sites[] = 'default';
    }
    $total_sites += count($sites);
    foreach ($sites as $site) {
      $site_dir = $platform_dir.'/sites/'.$site;
      drush_print(" * $site");

      if ($rebuild) {
        $cmd = "drush --root=$platform_dir --uri=$site project-status-rebuild --pipe";
      }
      else {
        $sql = "select name, filename, status, schema_version, info from system where type = 'module' order by filename";
        $echo = 'echo "'.$sql.'"';
        $drush = "drush --root=$platform_dir --uri=$site sqlc";
        $cmd = $echo . ' | ' . $drush;
      }

      drush_shell_exec($cmd);
      $site_modules = drush_shell_exec_output();
      if (!$rebuild) {
        array_shift($site_modules);
      }

      foreach ($site_modules as $site_module) {
        if (!$site_module) {
          continue;
        }
        $matches = explode("\t", $site_module, 5);
        $name = $matches[0];
        $path = $matches[1];
        $status = $matches[2];
        $schema_version = $matches[3];
        if ($matches[4] && $matches[4] !== NULL && $matches[4] != 'NULL') {
          $info = unserialize($matches[4]);
        }
        else {
          $info = array();
        }

        if (count($matches) != 5) {
          drush_log(dt("Can't parse system table: row was @row", array("@row" => $site_module)), 'error');
          if ($debug) {
            drush_print_r($matches);
            drush_print(count($matches));
          }
          continue;
        }
        if (!$name) {
          drush_log(dt("Can't parse system table: missing name! row was @row", array("@row" => $site_module)), 'error');
          if ($debug) {
            drush_print_r($matches);
            drush_print_r($info);
          }
          continue;
        }

        if ($debug) {
          $info_array = print_r($info,1);
          $info_array = '';
          drush_print("- name=$name site=$site status=$status path=$path info=$info_array");
        }

        // ignore core modules
        if (substr($path, 0, 8) == 'modules/') {
          if (isset($info['package']) && ($info['package'] == 'Pressflow' || $info['package'] == 'Development')) {
            // don't ignore the modules pressflow places in modules/
            // these are: cookie_cache_bypass & path_alias_cache (package: Pressflow)
            // and simpletest (package: Development)
          }
          else {
            continue;
          }
        }

        // ignore profile modules
        if (substr($path, 0, 9) == 'profiles/') {
          continue;
        }
        // ignore modules in site directories by default
        if (!$scan_site_modules) {
          if (substr($path, 0, 10) != 'sites/all/' &&
              substr($path, 0, 14) != 'sites/default/' &&
              substr($path, 0, 8) != 'modules/') {
            continue;
          }
        }
        // ignore hidden modules
        if (isset($info['hidden'])) {
          continue;
        }
        // sanity check -- ignore missing files
        if (!is_readable($platform_dir.'/'.$path)) {
          continue;
        }
        // sanity check -- ignore empty files
        if (filesize($platform_dir.'/'.$path) === 0) {
          continue;
        }
        // there was previously a sanity check here for modules with no files
        // using the test count($info['files']) == 0, but as of drush 5 and
        // drupal 7 this is set from a module's .info file, where listing files
        // is optional.

        if (isset($info['project'])) {
          $project = $info['project'];
        }
        else {
          $project = $name;
        }

        $real_name = $name;
        if (!$no_group_projects) {
          $name = $project;
        }

        if ($filter) {
          if ($filter_strict && $name != $filter) {
            continue;
          }
          elseif (!preg_match("/$filter/", $name)) {
            continue;
          }
        }

        if ($status) {
          $status_note = dt('Enabled');
        }
        else {
          $status_note = dt('Not installed');
        }

        if (!isset($modules[$name])) {
          $modules[$name] = array();
          $modules[$name][$platform_dir][$site] = $status_note;
        }
        elseif (!isset($modules[$name][$platform_dir])) {
          $modules[$name][$platform_dir][$site] = $status_note;
        }
        elseif (!isset($modules[$name][$platform_dir][$site])) {
          $modules[$name][$platform_dir][$site] = $status_note;
        }
        elseif ($modules[$name][$platform_dir][$site] != dt('Enabled')) {
          $modules[$name][$platform_dir][$site] = $status_note;
        }

        if ($debug) {
          drush_print("= name=$name real_name=$real_name site=$site status=$status_note path=$path project=$project");
        }
      }
    }
  }

  ksort($modules);
  if ($debug) {
    drush_print_r($modules);
  }
  $unused_modules = array();
  foreach($modules as $name => $platforms) {
    ksort($modules[$name]);
    foreach($modules[$name] as $platform => $sites) {
      $used = FALSE;
      ksort($modules[$name][$platform]);
      foreach($modules[$name][$platform] as $site => $status) {
        if ($status == dt('Enabled')) {
          $used = TRUE;
          break;
        }
      }
      if (!$used && !in_array($name, $unused_modules)) {
        $unused_modules[] = $name;
      }
    }
  }

  $enabled_modules = array();
  foreach($modules as $name => $platforms) {
    foreach($modules[$name] as $platform => $sites) {
      if (in_array($name, $unused_modules)) {
        continue;
      }
      if (!in_array($name, $enabled_modules)) {
        $enabled_modules[] = $name;
      }
    }
  }

  if (!$no_unused) {
    if (count($unused_modules)) {
      $unused_count = count($unused_modules);
      drush_print(dt("Found @unused unused modules", array("@unused" => $unused_count)));
      foreach ($unused_modules as $unused_module) {
        if ($pipe) {
          if ($show_used) {
            drush_print_pipe("0\t$unused_module\n");
          }
          else {
          drush_print_pipe("$unused_module\n");
        }
        }
        else {
          drush_print(" * $unused_module");
        }
      }
    }
    else {
      drush_print(dt("No unused modules"));
    }
  }

  if ($show_used) {
    if (count($enabled_modules)) {
      $enabled_count = count($enabled_modules);
      drush_print(dt("Found @enabled enabled modules", array("@enabled" => $enabled_count)));
      foreach ($enabled_modules as $enabled_module) {
        drush_print(" * $enabled_module");
        foreach ($modules[$enabled_module] as $platform => $sites) {
          foreach ($modules[$enabled_module][$platform] as $site => $status) {
            if ($status == dt('Enabled')) {
              if ($total_sites > 1) {
                if ($pipe) {
                  if (count($args) > 1) {
                    drush_print_pipe("$enabled_module\t$platform\t$site\n");
                  }
                  else {
                    drush_print_pipe("$enabled_module\t$site\n");
                  }
                }
                else {
                  if (count($args) > 1) {
                    drush_print("   * $platform : $site");
                  }
                  else {
                    drush_print("   * $site");
                  }
                }
              }
              else {
                if ($pipe) {
                  if ($no_unused) {
                    drush_print_pipe("$enabled_module\n");
                  }
                  else {
                    drush_print_pipe("1\t$enabled_module\n");
                  }
                }
              }
            }
          }
        }
      }
    }
    else {
      drush_print(dt("No used modules"));
    }
  }
}
