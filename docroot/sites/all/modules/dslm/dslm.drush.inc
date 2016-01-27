<?php
/**
 * @file dslm drush commands
 */

DEFINE('DSLM_PATH', dirname(__FILE__));

require_once DSLM_PATH . '/lib/dslm.class.php';

/**
 * Implementation of hook_drush_help().
 */
function dslm_drush_help($section) {
  switch ($section) {
    case 'meta:dslm:title':
      return dt('DSLM (Drupal Symlink Manager)');
    case 'meta:dslm:summary':
      return dt('Drupal symlink management Drush commands.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function dslm_drush_command() {
  $items = array();

  // Global dslm arguments
  $arguments = array(
    'dslm-base' => 'Change the dslm base directory',
    'packages-base' => 'Change the package base directory',
  );

  $release_options = array(
    'latest' => 'Use the latest release(stable) version of core and/or profile.',
    'dev' => 'Use the latest dev version of core and/or profile.',
    'current' => 'Use the "current" symlink of core and profile, if it exists.',
  );

  $items['dslm'] = array(
    'description' => 'DSLM BASE information',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'format' => 'Passes through drush_format instead of drush_log',
    ),
  );

  $items['dslm-info'] = array(
    'description' => 'DSLM site information',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
    'options' => array(
      'format' => 'Passes through drush_format instead of drush_log.',
    ),
  );

  $items['dslm-cores'] = array(
    'description' => 'DSLM Show Cores',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'format' => 'Passes through drush_format instead of drush_log.',
    ),
  );

  $items['dslm-profiles'] = array(
    'description' => 'DSLM Show Profiles',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'format' => 'Passes through drush_format instead of drush_log.',
    ),
  );

  $items['dslm-new'] = array(
    'description' => 'DSLM  New Site',
    'arguments' => $arguments,
    // @TODO Wire up a --skip-sites-default option as well
    'options' => $release_options,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['dslm-convert-core'] = array(
    'description' => 'DSLM Convert Core to Links',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => $release_options,
    'aliases' => array('dslm-cc'),
  );

  $items['dslm-switch-core'] = array(
    'description' => 'DSLM  Switch Core',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
    'options' => $release_options,
  );

  $profile_release_options = array_merge($release_options,
    array('upgrade' => 'Attempt to upgrade already symlinked profile to new version.',)
  );

  $items['dslm-add-profile'] = array(
    'description' => 'DSLM Add Profile',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
    'options' => $profile_release_options,
  );

  $items['dslm-remove-profile'] = array(
    'description' => 'DSLM Remove Profile',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  );
  
  $items['dslm-add-contrib'] = array(
    'description' => 'DSLM Add Contrib Project',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  );
  
  $items['dslm-add-custom'] = array(
    'description' => 'DSLM Add Custom Project',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  );

  $items['dslm-remove-all-custom'] = array(
    'description' => 'DSLM Remove All Symlinks for Custom Projects',
    'arguments' => $arguments,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  ); 
  
  return $items;
}

/**
 * Command callback. Displays a list of available cores.
 */
function drush_dslm() {
  // Bootstrap dslm, this grabs the instantiated and configured dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }
  if (!$info = $dslm->dslmInfo()) {
    return drush_set_error($dslm->lastError());
  }
  
  // $info['packages_base'] is returning 0... need to fix
  $out = 'DSLM BASE: ' . $info['dslm_base'] . "\n";
  $out .= 'PACKAGES BASE: ' . _dslm_get_packages_base() . "\n";
  $out .= 'SITE DIRECTORY: ' . $info['directory'] . "\n";
  $out .= 'DSLM DIRECTORY: ' . __FILE__;
  if ($format = drush_get_option('format', FALSE)) {
    drush_print(drush_format($info, '', $format));
  }
  else {
    drush_log(trim($out), 'ok');
  }
  return TRUE;
}


/**
 * Command callback. Displays the dslm information for a site.
 */
function drush_dslm_info() {
  // Bootstrap dslm, this grabs the instantiated and configured DSLM library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }

  // Iterate through the cores array and print them out
  if (!$info = $dslm->siteInfo()) {
    return drush_set_error($dslm->lastError());
  }

  $log = "Core: {$info['core']}\n";
  $info['core'] = $info['core'];
  if (!empty($info['profiles'])) {
    $log .= "Managed Profiles: ";
    foreach ($info['profiles'] as $profile => $versions) {
      $log .= "$profile-" . implode(', ', $versions);
      $info['profiles'][$profile][] = $versions;
    }
  }

  if ($format = drush_get_option('format', FALSE)) {
    drush_print(drush_format($info, '', $format));
  }
  else {
    // Render the display
    drush_log(trim($log), 'ok');
  }

  return TRUE;
}

/**
 * Command callback. Displays a list of available cores.
 */
function drush_dslm_cores() {
  // Bootstrap dslm, this grabs the instantiated and configured dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }

  // Pull cores
  $cores = $dslm->getCores();

  // Iterate through the cores array and print them out
  foreach ($cores['all'] as $key => $core) {
    $out .= "$core\n";
    $info['cores'][$core] = $core;
  }

  if ($format = drush_get_option('format', FALSE)) {
    drush_print(drush_format($info, '', $format));
  }
  else {
    drush_log(trim($out), 'ok');
  }
  return TRUE;
}

/**
 * Command callback. Displays a list of available profiles.
 */
function drush_dslm_profiles() {
  // Bootstrap dslm, this grabs the instantiated and configured dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }

  // Pull the profiles
  $profiles = $dslm->getProfiles();

  // Iterate through the profiles array and print them out
  foreach ($profiles as $name => $profile) {
    $out .= "$name:\n";
    foreach($profile['all'] as $version) {
      $out .= "  $version\n";
      $info['profiles'][$name][] = $version;
    }
  }

  if ($format = drush_get_option('format', FALSE)) {
    drush_print(drush_format($info, '', $format));
  }
  else {
    drush_log(trim($out), 'ok');
  }
  return TRUE;
}

/**
 * Command callback. Creates a new dslm Drupal site.
 */
function drush_dslm_new() {
  // Pull the args
  $args = drush_get_arguments();
  if (!isset($args[1])) {
    return drush_set_error(dt('Please provide destination for your DSLM site.'));
  }
  else {
    $site_dest = $args[1];
  }

  // Bootstrap dslm, this grabs the instantiated and configured Dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }

  // Set the profile and core, either supplied on the CLI, set with --latest, or will prompt
  if (drush_get_option('latest', FALSE)) {
    $latest = $dslm->latestCores();
    $core = $latest['release'];
  }
  elseif (drush_get_option('dev', FALSE)) {
    $latest = $dslm->latestCores();
    $core = $latest['dev'];
  }
  elseif (drush_get_option('current', FALSE)) {
    $core = 'current';
  }
  else {
    $core = isset($args[2]) ? $args[2] : FALSE;
  }

  // If we still don't have a core string try to get on iteractively
  if (!$core) {
    $core_list = $dslm->getCores();
    $pick_core = drush_choice($core_list['all']);
    if ($pick_core === FALSE) {
      return FALSE;
    }
    $core = $core_list['all'][$pick_core];
  }

  if (!$res = $dslm->newSite($site_dest, $core)) {
    return drush_set_error($dslm->lastError());
  }
  else {
    drush_log(dt('Your site has been linked in !dir.', array('!dir' => $site_dest)), 'success');
    return TRUE;
  }
}

/**
 * Command callback. Prompts the user to convert their current, normal site to
 * a symlinked site.
 *
 * After this process is complete a file listing won't show the links until
 * the directory structure is traversed out and back in.
 *
 * Maybe allow this to be done from outside of the drupal root? We can verify
 * that a lower directory is a drupal root in other ways than bootstrapping.
 */
function drush_dslm_convert_core() {
  // Bootstrap dslm, this grabs the instantiated and configured Dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }

  // Validate the desired core.
  $args = drush_get_arguments();
  $site = isset($args[1]) ? $args[1] : FALSE;
  $core = isset($args[2]) ? $args[2] : FALSE;

  // Ensure the site we are converting is a regular drupal site and that
  // we are not inside it.
  if (!$site) {
    drush_set_error(dt('You must specify a site to convert.'));
    return FALSE;
  }
  elseif (strpos($site, '.') === 0) {
    drush_set_error(dt('You cannot convert the site you are currently within.'));
    return FALSE;
  }
  elseif (!is_file("{$site}/index.php") || !is_dir("{$site}/modules")) {
    drush_set_error(dt('This is not a valid Drupal site.'));
    return FALSE;
  }
  elseif (is_link("{$site}/index.php")) {
    drush_set_error(dt('This site already appears to be linked.'));
    return FALSE;
  }

  // If we still don't have a core string, try to get one interactively
  if (!$core) {
    $core_list = $dslm->getCores();
    $pick_core = drush_choice($core_list['all']);
    if (!$pick_core) {
      return FALSE;
    }
    $core = $core_list['all'][$pick_core];
  }

  drush_print_r("Converting {$site} to DSLM version {$core}.");

  // Create a new site named SITE.dslm parallel to the current site.
  drush_print(dt('DSLM: Creating new linked site...'));
  if ($res = $dslm->newSite("{$site}.dslm", $core, TRUE)) {
    // Copy the permanent files from the old site into the new one.
    drush_print(dt("DSLM: Copying permanent contents..."));
    shell_exec("cp -r {$site}/sites {$site}.dslm");
    shell_exec("cp -r {$site}/profiles {$site}.dslm");
    shell_exec("cp -r {$site}/.git* {$site}.dslm");
    shell_exec("cp -r {$site}/.svn {$site}.dslm");

    // Move the old site to NAME.dslmbak
    drush_print(dt("DSLM: Cleaning up..."));
    rename("{$site}", "{$site}.dslmorig");

    // Move the new site to just SITE to replace the old one.
    rename("{$site}.dslm", "{$site}");

    drush_print(dt("DSLM: Core conversion complete."));
  }
  else {
    return drush_set_error($dslm->lastError());
  }
}

/**
 * Command callback. Prompts the user to switch the dslm core.
 */
function drush_dslm_switch_core() {
  // Bootstrap dslm, this grabs the instantiated and configured Dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }

  $args = drush_get_arguments();
  $core = isset($args[1]) ? $args[1] : FALSE;

  if (!$core && drush_get_option('current', FALSE)) {
    $core = 'current';
  }

  // If we still don't have a core string try to get on iteractively
  if (!$core) {
    $core_list = $dslm->getCores();
    $pick_core = drush_choice($core_list['all']);
    if (!$pick_core) {
      return FALSE;
    }
    $core = $core_list['all'][$pick_core];
  }

  $run = $dslm->switchCore($core);
  if (!$run) {
    return drush_set_error($dslm->lastError());
  }
  else {
    // @todo: get $dslm->newSite to return validated switch information rather than TRUE
    //  for reporting here.
    drush_log('The links have been switched.', 'success');
    return TRUE;
  }
}

/**
 * Command callback. Adds a profile
 */
function drush_dslm_add_profile() {
  // Bootstrap dslm, this grabs the instantiated and configured Dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }

  $args = drush_get_arguments();

  // Get a list of profiles
  $profiles = $dslm->getProfiles();

  // Get the profile name via args or interactively
  $profile_name = isset($args[1]) ? $args[1] : FALSE;

  if (drush_get_option('current', FALSE)) {
    $profile_version = 'current';
  }

  // Version might be coming through the profile_name as in my_profile-1.2
  if (!isset($profile_version)) {
    list($profile_name, $profile_version) = explode('-', $profile_name);
  }

  if (!$profile_name) {
    $choices = array();
    foreach ($profiles as $k => $v) {
      $choices[$k] = $k;
    }
    $profile_name = drush_choice($choices);
    if (!$profile_name || !isset($profiles[$profile_name])) {
      return FALSE;
    }
  }
  if ($profile_name && !$profiles[$profile_name]) {
    return drush_set_error(dt('Invalid profile name given.'));
  }

  // If profile version not provided as single string with name, check arg 2
  // as in "drush dslm-add-profile my_profile 1.2"
  $profile_version = isset($args[2]) ? $args[2] : $profile_version;

  if (drush_get_option('latest', FALSE)) {
    $latest = $dslm->latestProfiles($profile_name);
    $profile_version = $latest['release'];
  }
  elseif (drush_get_option('dev', FALSE)) {
    $latest = $dslm->latestProfiles($profile_name);
    $profile_version = $latest['dev'];
  }

  // If we still dont have a version number, prompt the user with the
  // available options.
  if (!$profile_version) {
    $choices = array();
    foreach ($profiles[$profile_name]['all'] as $k => $v) {
      $choices[$v] = $v;
    }
    $profile_version = drush_choice($choices);
  }

  // Validate that the profile name and version matches a dslm managed profile.
  if (!in_array($profile_version, $profiles[$profile_name]['all'])) {
    return drush_set_error(dt('Invalid profile version given.'));
  }

  // Determine if we are upgrading a currently managed dslm profile.
  $upgrade = drush_get_option('upgrade', FALSE);

  // Run the DSLM manageProfile method to add the profile or deal with an error
  if ($dslm->manageProfile($profile_name, $profile_version, FALSE, $upgrade)) {
    if ($upgrade) {
      drush_log(dt('The DSLM profile !profile has been upgraded to version !version.', array('!profile' => $profile_name, '!version' => $profile_version)), 'success');
    }
    else {
      drush_log(dt('The DSLM profile !profile-!version has been added.', array('!profile' => $profile_name, '!version' => $profile_version)), 'success');
    }
  }
  else {
    return drush_set_error($dslm->lastError());
  }
}

/**
 * Command callback. Removes a profile.
 */
function drush_dslm_remove_profile() {
  // Bootstrap dslm, this grabs the instantiated and configured Dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }

  if (!$info = $dslm->siteInfo()) {
    return drush_set_error($dslm->lastError());
  }

  $args = drush_get_arguments();

  // Get the profile name via args or interactively.
  $profile_name = isset($args[1]) ? $args[1] : FALSE;

  // Version might be coming through the profile_name as in my_profile-1.2, we only
  // care about the name here.
  list($profile_name, $profile_version) = explode('-', $profile_name);

  if (!$profile_name) {
    if (!empty($info['profiles'])) {
      foreach ($info['profiles'] as $profile => $versions) {
        $choices[$profile] = $profile;
      }
      $profile_name = drush_choice($choices);
    }
    else {
      return drush_set_error(dt('No DSLM profiles available to be removed.'));
    }
  }

  if ($dslm->removeProfile($profile_name)) {
    drush_log(dt('The DSLM profile !profile has been removed.', array('!profile' => $profile_name)), 'success');
  }
  else {
    return drush_set_error($dslm->lastError());
  }
}

/**
 * Command callback. Adds symlink for custom project to sites/all
 */
function drush_dslm_add_contrib() {
  drush_include(DRUSH_BASE_PATH . '/commands/pm', 'pm.drush');
  // Bootstrap dslm, this grabs the instantiated and configured Dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }
  
  $base = _dslm_get_packages_base();

  //check to see that packages exists in dslm_base or is configured
  if (!file_exists($base)) {
    return drush_set_error('Directory not found. Place packages directory in dslm_base or define correct path in packages_base variable');
  }
   
  if (!$info = $dslm->siteInfo()) {
    return drush_set_error($dslm->lastError());
  }

  $args = drush_get_arguments();
  
  unset($args[0]);
  
  $arg_list = implode(", ", $args);
  
  $requests = pm_parse_arguments($args);
  
  // pm_parse_project_version -> pm_parse_version in drush 7
  if (function_exists(pm_parse_project_version)) {
    $requests = pm_parse_project_version($requests);
  }

  foreach ($requests as $request) {

    // pm_parse_project_version -> pm_parse_version in drush 7
    // and no longer sets ['name']
    
    if (function_exists(pm_parse_version)) {
      $request_name = $request;
      $request = pm_parse_version($request);
      $request['name'] = $request_name;
    }

    // check to see if package is in dslm_base/packages or a package_base is defined
    // to determine if this is a module, theme, or library
    
    //only check if it hasn't been set
    $request['type'] = FALSE;
    
    // this code could be condensed, but it works
    if(file_exists($base . '/contrib/modules/' . $request['name'])) {
      $request['type'] = 'modules';
    }
    
    if(!$request['type'] && file_exists($base . '/contrib/themes/' . $request['name'])) {
      $request['type'] = 'themes';
    }
    
    if(!$request['type'] && file_exists($base . '/contrib/libraries/' . $request['name'])) {
      $request['type'] = 'libraries';
    }
    
    if (!$request['type']) {
      return drush_set_error($request['name'] . ' not found in modules, themes, or libraries within packages/contrib');
    }
    
    // check to see if we want a specific version
    if ($request['version']) {
      //and that the version we want exists in dslm_base
      if(!file_exists($base . '/contrib/' . $request['type'] . '/' . $request['name'] . '/' . $request['name'] . '-' . $request['version'])) {
        return drush_set_error('Version' . $request['version'] . ' not found for ' . $request['name']);
      }
    }
    
    // if there is no version specified, use current
    if (!$request['version']) {
       $request['version'] = 'current';
       if(!file_exists($base . '/contrib/' . $request['type'] . '/' . $request['name'] . '/current')) {
        return drush_set_error('Current not found for ' . $request['name']);
      }
    }
   
    // Run the DSLM manageContribPackage method to add the project or deal with an error
    if ($dslm->manageContribPackage($request['name'], $request['version'], $request['type'], $base)) {
      drush_log(dt('Symlinks for ' . $request['name'] . ' added.', array('!projects' => $arg_list)), 'success');
    }
    else {
      return drush_set_error($dslm->lastError());
    }
  }

}

/**
 * Command callback. Adds symlink for custom project to sites/all/TYPE/custom
 */
function drush_dslm_add_custom() {
  drush_include(DRUSH_BASE_PATH . '/commands/pm', 'pm.drush');
  // Bootstrap dslm, this grabs the instantiated and configured Dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }
  
  $base = _dslm_get_packages_base();

  //check to see that packages exists in dslm_base or is configured
  if (!file_exists($base)) {
    return drush_set_error('Directory not found. Place packages directory in dslm_base or define correct path in packages_base variable');
  }
  
  if (!$info = $dslm->siteInfo()) {
    return drush_set_error($dslm->lastError());
  }

  $args = drush_get_arguments();
  
  unset($args[0]);
  
  $arg_list = implode(", ", $args);
  
  $requests = pm_parse_arguments($args);

  foreach ($requests as $request) {
	$count = 0;
	
    // this code could be condensed, but it works
    if(!file_exists($base . '/custom/' . $request)) {
      return drush_set_error($request . ' not found in packages/custom');
    }

    // Run the DSLM manageContribPackage method to add the project or deal with an error
    if ($count = $dslm->manageCustomPackage($request, $base)) {
      drush_log(dt('!count symlinks for !request added.', array('!request' => $request, '!count' => $count)), 'success');
    }
    else {	
	  if (!$count) {
	    drush_log(dt('No new symlinks add.'));
      } else {
        return drush_set_error($dslm->lastError());
      }
    }
  }

}

/**
 * Command callback. Adds symlink for custom project to sites/all/TYPE/custom
 */
function drush_dslm_remove_all_custom() {
  drush_include(DRUSH_BASE_PATH . '/commands/pm', 'pm.drush');
  // Bootstrap dslm, this grabs the instantiated and configured Dslm library object
  if (!$dslm = _dslm_bootstrap()) {
    return FALSE;
  }
 
  if (!$info = $dslm->siteInfo()) {
    return drush_set_error($dslm->lastError());
  }

  if($number_removed = $dslm->removeAllCustomPackages()) {
    drush_log(dt('!number symlinks removed.', array('!number' => $number_removed)), 'success');
  }
  else {
    return drush_set_error($dslm->lastError());
  }
}

/**
 * Internal bootstrap function
 * @return object
 *  Returns an instantiated Dslm object
 */
function _dslm_bootstrap() {
  if (!$base = _dslm_get_base()) {
    return drush_set_error(dt('Please set a DSLM Drupal base in order for DSLM commands to work.'));
  }
  
  if (!$packages_base = _dslm_get_packages_base()) {
    $packages_base = $base . '/pacakges/';
  }
  
  // Instantiate the dslm object
  $dslm = new Dslm($base, $packages_base);

  // If the base is bad on construct an error will be set
  if ($last_error = $dslm->lastError()) {
    return drush_set_error($last_error);
  }

  return $dslm;
}

/**
 * Internal helper function to get the dslm drupal base folder
 * Reads the following locations in order:
 * - The cli switch --dslm-base=base
 * - The drushrc.php file $conf['dslm_base'] = /path/to/base;
 * - The dslm_BASE system environment variable
 * @return string
 *  Returns a base path or FALSE
 */
function _dslm_get_base() {
  // Initialize an empty base.
  $base = '';

  // Try to get a base from one of the appropriate sources
  if ($base_check = drush_get_option('dslm-base', FALSE)) {
    // We have this wet with --dslm-base on the cli
    $base = $base_check;
  }
  elseif ($base_check = drush_get_option('dslm_base', FALSE)) {
    // We have this set in the drushrc.php file
    $base = $base_check;
  }
  else {
    // Last ditch is to use the environment variable if it's set.
    if (getenv('DSLM_BASE')) {
      $base = getenv('DSLM_BASE');
    } else {
      // If we got here and there wasn't an environment variable set, bail
      return FALSE;
    }
  }

  // PHP doesn't resolve ~ as the home directory.
  if (isset($_SERVER['HOME'])) {
    $base = preg_replace('/^\~/', $_SERVER['HOME'], $base);
  }

  return $base;
}

/**
 * Internal helper function to get the dslm package base folder
 * Reads the following locations in order:
 * - The cli switch --packages-base=base
 * - The drushrc.php file $conf['packages_base'] = /path/to/base;
 * - The dslm_BASE system environment variable
 * @return string
 *  Returns a base path or FALSE
 */
function _dslm_get_packages_base() {
  // Initialize an empty base.
  $base = '';

  // Try to get a base from one of the appropriate sources
  if ($base_check = drush_get_option('packages-base', FALSE)) {
    // We have this wet with --dslm-base on the cli
    $base = $base_check;
  }
  elseif ($base_check = drush_get_option('packages_base', FALSE)) {
    // We have this set in the drushrc.php file
    $base = $base_check;
  }
  else {
    // Last ditch is to use the environment variable if it's set.
    if (getenv('PACKAGES_BASE')) {
      $base = getenv('PACKAGES_BASE');
    } else {
      // If we got here and there wasn't an environment variable set, bail
      return FALSE;
    }
  }

  // PHP doesn't resolve ~ as the home directory.
  if (isset($_SERVER['HOME'])) {
    $base = preg_replace('/^\~/', $_SERVER['HOME'], $base);
  }
  
  //if the base hasn't been configured, assume packages are at the root of the base
  if (!$base) { 
    $base = _dslm_get_base() . '/packages';   
  }

  return $base;
}
