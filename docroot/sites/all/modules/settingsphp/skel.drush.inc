<?php

/**
 * Implements hook_drush_command().
 */
function skel_drush_command() {
  $items = array();
  $items['skel-files-create'] = array(
    'description' => 'Creates a skel of folders to store files (public, private, translations, ...) out of the document root.',
    'options' => array(
      'owner' => array(
        'description'   => 'Owner of the shared folders. Defaults to the user running the command.',
        'example-value' => 'root',
      ),
      'group' => array(
        'description'   => 'Group of the shared folders. Defaults to --owner.',
        'example-value' => 'www-data',
      ),
      'all' => 'Initialize shared folders for all sites found.',
      'mode' => array(
        'description' => 'Creation mode. Defaults to 2770.',
        'example-value' => '0755',
      ),
      'ancestors' => 'Number of ancestors of the Drupal root to ascend and place the shared folders. Defaults to 1.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
  );

  return $items;
}

/**
 * Validation callback for 'skel-files-create'.
 */
function drush_skel_files_create_validate() {
  // Validate owner.
  $owner = drush_get_option('owner', FALSE);
  if ($owner) {
    $exists = drush_shell_exec('getent passwd %s > /dev/null', $owner);
    if (!$exists) {
      drush_set_error('SKEL_FILES', dt('Unknown owner :owner.', array(':owner' => $owner)));
    }
  }
  else {
    $processowner = posix_getpwuid(posix_geteuid());
    $owner = $processowner['name'];
    drush_set_option('owner', $owner);
  }

  // Validate group.
  $group = drush_get_option('group', FALSE);
  if (!$group) {
    $group = $owner;
    drush_set_option('group', $group);
  }
  $exists = drush_shell_exec('getent group %s > /dev/null', $group);
  if (!$exists) {
    drush_set_error('SKEL_FILES', dt('Unknown group :group.', array(':group' => $group)));
  }
}

/**
 * Command callback for 'skel-files-create'.
 */
function drush_skel_files_create() {
  $owner = drush_get_option('owner');
  $group = drush_get_option('group');
  $mode = drush_get_option('mode', '2770');
  $ancestors = drush_get_option('ancestors', 1);
  $folders = array('public', 'private', 'translations');

  if (drush_get_option('all')) {
    # Iterate all site folders and create a skel.
    $dir = drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites';
    $sites = drush_scan_directory($dir, '/^settings.php/', array('.', '..', 'all'));
    foreach ($sites as $site) {
      $site_dir = basename(dirname($site->filename));
      _drush_skel_files_create($site_dir, $owner, $group, $mode, $ancestors, $folders);
    }
  }
  else {
    $site_dir = basename(conf_path());
    _drush_skel_files_create($site_dir, $owner, $group, $mode, $ancestors, $folders);
  }
}

/**
 * Creates the files skel for a site.
 */
function _drush_skel_files_create($site_dir, $owner, $group, $mode, $ancestors, $folders) {
  $ancestors = str_repeat('../', $ancestors);
  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  $shared = $drupal_root . '/' . $ancestors . 'shared/' . $site_dir;

  # Create shared folders and set permissions.
  foreach ($folders as $folder) {
    $command = "mkdir -p -m ${mode} ${shared}/${folder}";
    drush_op_system($command);
    $command = "chown -R ${owner}:${group} ${shared}/${folder}";
    drush_op_system($command);
  }

  # Vagrant nfs shares need write permission on the parent folder
  # in order to write inside, even with sudo.
  $command = "stat -c%a sites/${site_dir}";
  drush_shell_exec($command);
  $mode_sitedir = current(drush_shell_exec_output());
  $command = "chmod u+w sites/${site_dir}";
  drush_op_system($command);

  # Remove and recreate files/ symlink.
  $files_dir = 'sites/' .$site_dir . '/files';
  $command = "test -e ${files_dir} && rm ${files_dir}";
  drush_op_system($command);

  $command = "ln -s ${ancestors}../../shared/${site_dir}/public files";
  $cwd = getcwd();
  drush_op('chdir', 'sites/' . $site_dir);
  drush_op_system($command);
  drush_op('chdir', $cwd);

  # Restore permissions on parent folder.
  $command = "chmod ${mode_sitedir} sites/${site_dir}";
  drush_op_system($command);
}

