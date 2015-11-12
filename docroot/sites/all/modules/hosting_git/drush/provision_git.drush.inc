<?php

/**
 * @file
 * Provision/Drush hooks for git commands.
 */

/**
 * Implementation of hook_drush_command().
 */
function provision_git_drush_command() {
  $items['provision-git-pull'] = array(
    'description' => 'Executes "git pull".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'reset' => 'Resets any working tree changes that would block a rebase.  USE WITH CAUTION',
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
    'aliases' => array('pull'),
  );
  $items['provision-git-fetch'] = array(
    'description' => 'Executes "git fetch".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
    'aliases' => array('fetch'),
  );
  $items['provision-git-push'] = array(
    'description' => 'Executes "git push".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('push'),
    'options' => array(
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
  );
  $items['provision-git-commit'] = array(
    'description' => 'Commits all added and changed files to the repository.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('commit'),
    'options' => array(
      'message' => 'The message to attach to the commit.',
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
  );
  $items['provision-git-reset'] = array(
    'description' => 'Executes "git reset --hard".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('reset'),
    'options' => array(
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
  );
  $items['provision-git-add'] = array(
    'description' => 'Adds files and directories to be committed.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('add'),
    'arguments' => array(
      'path' => 'Path to the file or folders you would like to add.',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
  );
  $items['provision-git-clone'] = array(
    'description' => 'Git clone a repo in a specified location.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'git_url' => dt('The git repository to clone.'),
      'path' => dt('The location where the git repository will be cloned.'),
    ),
  );
  $items['provision-git-checkout'] = array(
    'description' => 'Git checkout a branch or tag in a specified location.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'git_ref' => dt('Parameter for the git checkout command'),
    ),
    'options' => array(
      'new_branch' => "Optional: If the branch doesn't exist, create it. Equivalent to git checkout -b branchname"
    )
  );
  return $items;
}

/**
 * Helper for verifying the chosen path is a git repo.
 */
function provision_git_is_repo($repo_path = NULL) {
  // If no path specified, use the site or platform path of the alias.
  if (!$repo_path) {
    $repo_path = drush_get_option('repo_path');
  }
  if (!$repo_path) {
    if (d()->type === 'site') {
      $repo_path = d()->site_path ? d()->site_path : d()->root;
    } elseif (d()->type == 'platform') {
      $repo_path = d()->platform->root ? d()->platform->root : d()->root;
    }
  }
  // If shell returns False, this is not a git repo
  if (!drush_shell_cd_and_exec($repo_path, 'git status')) {
    drush_set_error('DRUSH_PROVISION_GIT_PULL_FAILED', dt("Git repository not found in: !path", array('!path' => $repo_path)));
    return FALSE;
  } else {
    // Find the toplevel directory for this git repository
    if (drush_shell_cd_and_exec($repo_path, 'git rev-parse --show-toplevel')) {
      $top_repo_path = implode("\n", drush_shell_exec_output());
    }
    if (drush_get_option('force_repo_path_as_toplevel', FALSE) && realpath($repo_path) != $top_repo_path) {
      drush_set_error('DRUSH_PROVISION_GIT_PULL_FAILED', dt("Path !path is not the toplevel dir of this Git repository.", array('!path' => $repo_path)));
      return FALSE;
    }
    drush_log(dt("Git repository found in !path", array('!path' => $top_repo_path)), 'ok');
    drush_set_option('repo_path', $top_repo_path);
    return TRUE;
  }
}

/**
 * Pre provision-git-pull hook
 */
function drush_provision_pre_provision_git_pull() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-push hook
 */
function drush_provision_pre_provision_git_push() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-commit hook
 */
function drush_provision_pre_provision_git_commit() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-reset hook
 */
function drush_provision_pre_provision_git_reset() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-add hook
 */
function drush_provision_pre_provision_git_add() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-clone hook
 */
function drush_provision_pre_provision_git_clone() {
  // Nothing to do here.
}

/**
 * Pre provision-git-checkout hook
 */
function drush_provision_pre_provision_git_checkout() {
  provision_git_is_repo();
}

/**
 * Implements the provision-git-pull command.
 */
function drush_provision_git_pull() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  // Only git reset --hard if option says so.
  if (drush_get_option('reset') == TRUE) {
    provision_backend_invoke($target, 'provision-git-reset');
  }

  // Execute git pull --rebase
  // @TODO: make rebase optional as well??  Haven't thought this through yet.
  if (drush_shell_cd_and_exec($repo_path, 'git pull --rebase')) {
    drush_log(dt('Git repository !path pulled.', array('!path' => $repo_path)), 'ok');
    drush_log(implode("\n", drush_shell_exec_output()), 'ok');

    // Submodule in git? lets update those.
    if (file_exists($repo_path . '/.gitmodules')) {
      drush_shell_cd_and_exec($repo_path, 'git submodule init');
      drush_log(implode("\n", drush_shell_exec_output()), 'ok');
      drush_shell_cd_and_exec($repo_path, 'git submodule update');
      drush_log(implode("\n", drush_shell_exec_output()), 'ok');
    }
  }
  else {
    return drush_set_error('DRUSH_PROVISION_GIT_PULL_FAILED', dt("Git pull failed in !path.\nThe specific errors are below:\n!errors", array('!path' => $repo_path, '!errors' => implode("\n", drush_shell_exec_output()))));
  }
}

/**
 * Implements the provision-git-fetch command.
 */
function drush_provision_git_fetch() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  if (drush_shell_cd_and_exec($repo_path, 'git fetch')) {
    drush_log(dt('Git repository !path fetched.', array('!path' => $repo_path)), 'ok');
    $out = drush_shell_exec_output();
    if ($out) {
      drush_log(implode("\n", $out), 'ok');
    }
  }
  else {
    return drush_set_error('DRUSH_PROVISION_GIT_FETCH_FAILED', dt("Git fetch failed in !path.\nThe specific errors are below:\n!errors", array('!path' => $repo_path, '!errors' => implode("\n", drush_shell_exec_output()))));
  }
}

/**
 * Implements the provision-git-push command.
 */
function drush_provision_git_push() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  //Execute git push
  if (drush_shell_cd_and_exec($repo_path, 'git push')) {
    drush_log(dt('Git repository pushed.'), 'ok');
    drush_log(implode("\n", drush_shell_exec_output()), 'ok');
  }
  else {
    return drush_set_error('DRUSH_PROVISION_GIT_PULL_FAILED', dt("Git push failed in !path.\nThe specific errors are below:\n!errors", array('!path' => $repo_path, '!errors' => implode("\n", drush_shell_exec_output()))));
  }
}

/**
 * Implements the provision-git-commit command.
 */
function drush_provision_git_commit() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  //Generate commit message
  $message = array();
  $message[] = "Provision Git Commit: $target";
  $message[] = str_repeat('=', strlen($message[0]));
  if (drush_get_option('message')) {
    $message[] = drush_get_option('message');
  }
  $message = implode("\n", $message);

  // Execute git commit
  // @TODO: Decide if we should we always assume "-a" (commit all changes)?
  $ec = drush_shell_cd_and_exec($repo_path, "git commit -am '$message'");

  // drush_sell_cd_and_exec() returns TRUE on a exist status of 0, and
  // FALSE on a non-zero exit status. Unfortunately, git exits with
  // a status of 1 if there are no files to commit. This causes
  // drush_shell_cd_and_exec() to return FALSE and we think the
  // commit failed. So we have to resort to some hackery. We scan
  // the output and count how many files don't starts with '#'.
  // if the count is 1, then we have to assume there was nothing
  // to commit.

  // Assume nothing committed
  drush_set_context('PROVISION_GIT_COMMITTED', 0);

  if (!$ec) {
    $comments = 0;
    $lines = 0;

    foreach (drush_shell_exec_output() as $line) {
      if ($line[0] == '#') {
        $comments++;
      }
      else {
        $lines++;
      }
    }
    if ($comments > 0 && $lines == 1) {
      drush_log(dt('Nothing to commit.'), 'ok');
      drush_set_context('DRUSH_ERROR_CODE', DRUSH_SUCCESS);
    }
    else {
      return drush_set_error('DRUSH_PROVISION_GIT_COMMIT_FAILED', dt("Git commit failed in !path.\nThe specific errors are below:\n!errors", array('!path' => $repo_path, '!errors' => implode("\n", drush_shell_exec_output()))));
    }
  }
  else {
    drush_set_context('PROVISION_GIT_COMMITTED', 1);
    drush_log(dt('Git repository committed.'), 'ok');
    drush_log(implode("\n", drush_shell_exec_output()), 'ok');
  }
}

/**
 * Implements the provision-git-reset command.
 */
function drush_provision_git_reset() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  // Call git reset --hard which resets to the last pulled commit
  if (drush_shell_cd_and_exec($repo_path, 'git reset --hard')) {
    drush_log(dt('Git repository reset.'), 'ok');
    if (file_exists($repo_path . '/.gitmodules')) {
      if (drush_shell_cd_and_exec($repo_path, 'git submodule foreach \'git reset --hard\'')) {
        drush_log(dt('Git submodules reset.'), 'ok');
      }
    }
  }
}

/**
 * Implements the provision-git-clone command.
 */
function drush_provision_git_clone($git_url = '', $path = '', $git_ref = '') {

  // Get URL and path from d()
  // @TODO: Move to a validate hook.
  if (!$git_url) {
    $git_url = d()->repo_url;
  }
  if (!$path) {
    switch (d()->type) {
      case 'site':
        $path = d()->site_path;
        break;
      case 'platform':
      default:
        $path = d()->root;
        break;
    }
  }
  if (!$git_ref) {
    $git_ref = d()->git_ref;
  }

  drush_log(dt('Cloning !url to !path', array('!url' => $git_url, '!path' => $path, 'ok')));

  if (is_dir($path)) {
    return drush_set_error('DRUSH_PROVISION_GIT_CLONE_FAILED', dt('Directory already exists.'));
  }

  $command = 'git clone --recursive ';
  $command .= escapeshellarg(trim($git_url));
  $command .= ' ' . escapeshellarg(trim($path));

  if ($git_ref) {
    $command .= ' --branch ' . escapeshellarg(trim($git_ref));
  }

  // Try to perform the git clone
  drush_log("Running $command", 'ok');
  if (drush_shell_exec($command)) {
    // Log the fact that it worked
    drush_log(dt('Clone successful.'), 'ok');
    drush_log(implode("\n", drush_shell_exec_output()), 'ok');
  }
  else {
    // The command failed
    return drush_set_error('DRUSH_PROVISION_GIT_CLONE_FAILED', dt("Git clone failed! \nThe specific errors are below:\n!errors", array('!errors' => implode("\n", drush_shell_exec_output()))));
  }
}

/**
 * Implements the provision-git_checkout command
 */
function drush_provision_git_checkout($git_ref = '') {

  // Get URL and path from d()
  // @TODO: Move to a validate hook.
  if (!$git_ref) {
    $git_ref = d()->git_ref;
  }
  if (d()->type == 'site') {
    $path = d()->site_path;
  }
  else {
    $path = d()->root;
  }

  // Make sure there is a git ref
  if (empty($git_ref)) {
    return drush_set_error('DRUSH_PROVISION_GIT_CHECKOUT_FAILED', dt("Git checkout failed: No git_ref specified."));
  }

  provision_backend_invoke(d()->name, 'provision-git-fetch', array(), array('repo_path' => $path));

  drush_log(dt('Checking out git reference !git_ref', array('!git_ref' => $git_ref), 'ok'));

  $command = "git checkout " . escapeshellarg(trim($git_ref));

  // Try to perform the git checkout.
  drush_log("Running $command", 'ok');
  if (drush_shell_cd_and_exec($path, $command)) {
    // Log the fact that it worked.
    drush_log(dt('Checkout successful.'), 'ok');
    drush_log(implode("\n", drush_shell_exec_output()), 'ok');

    _provision_git_update_git_meta_data();
  } else {
    // The command failed.
    return drush_set_error('DRUSH_PROVISION_GIT_CHECKOUT_FAILED', dt("Git checkout failed! \nThe specific errors are below:\n!errors", array('!errors' => implode("\n", drush_shell_exec_output()))));
  }
}

/**
 * Implements the provision-git-add command.
 */
function drush_provision_git_add($path = '') {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  // Call git add.
  // Seems to pass even if the git add failed
  if (drush_shell_cd_and_exec($repo_path, "git add $path")) {
    drush_log(dt('Git add: ' . $path), 'ok');
  }
}


/**
 * Implements drush_hook_provision_verify_validate().
 *
 * This needs to be called in the validate step so that it runs before the
 * Makefile check and the check for a Drupal installation. Those operations,
 * very inconveniently, run in the same function.
 */
function drush_provision_git_provision_verify_validate() {
  if (in_array(d()->type, array('site', 'platform')) && d()->deploy_from_git) {

    switch (d()->type) {
      case 'site':
        $context_type = 'Site';
        $repo_path = d()->site_path;
        $path_exists = provision_file()->exists(d()->site_path)->status();
        // If there's an issue with the site git repo, we end up doing a
        // checkout on the platform, if we aren't careful.
        drush_set_option('force_repo_path_as_toplevel', TRUE);
        break;

      case 'platform':
      default:
        $context_type = 'Platform';
        $repo_path = d()->root;
        $path_exists = provision_file()->exists(d()->root)->status();
        break;
    }
    // If the platform path doesn't exist, we will clone it.
    // Sites aren't verified on install, so we need to clone the repo in
    // drush_provision_git_provision_install_validate() instead.
    if (!$path_exists) {
      drush_log(dt("$context_type path does not exist. Invoking provision-git-clone."), 'ok');
      provision_backend_invoke(d()->name, 'provision-git-clone');
    }
  }
}

/**
 * Implements drush_hook_provision_install_validate().
 *
 * The site directory is created in an implementation of
 * drush_hook_pre_COMMAND(). So to clone the site repo first, we need to do it
 * in an earlier hook.
 */
function drush_provision_git_provision_install_validate() {
  if (d()->type === 'site' && d()->deploy_from_git) {
    drush_set_option('force_repo_path_as_toplevel', TRUE);
    drush_log(dt("Cloning site from: " . d()->repo_url), 'ok');
    provision_backend_invoke(d()->name, 'provision-git-clone');
  }
}

/**
 * Implements drush_hook_post_COMMAND().
 */
function drush_provision_git_post_provision_verify() {
  _provision_git_update_git_meta_data();
}


/**
 * Update the drush context with the git_url and current git_ref.
 */
function _provision_git_update_git_meta_data() {
  if (in_array(d()->type, array('site', 'platform')) && d()->deploy_from_git) {

    switch (d()->type) {
      case 'site':
        $repo_path = d()->site_path;
        break;

      case 'platform':
      default:
        $repo_path = d()->root;
        break;
    }

    if (drush_shell_cd_and_exec($repo_path, 'git config --local --get remote.origin.url')) {
      $repo_url = implode("\n", drush_shell_exec_output());
      drush_set_option('repo_url', $repo_url);
    }
    if (drush_shell_cd_and_exec($repo_path, 'git rev-parse --abbrev-ref HEAD')) {
      $git_ref = implode("\n", drush_shell_exec_output());
      drush_set_option('git_ref', $git_ref);
    }
  }
}


/**
 * Implements hook_provision_drupal_create_directories_alter().
 */
function provision_git_provision_drupal_create_directories_alter(&$mkdir, $url) {
  if (d()->type === 'site' && d()->deploy_from_git) {
    // Since we're cloning these directories, don't chmod/chgrp them.
    // We still need sites/$url as it gets 0555 permission otherwise [#2494113].
    unset($mkdir["sites/$url/modules"]);
    unset($mkdir["sites/$url/themes"]);
    unset($mkdir["sites/$url/libraries"]);
  }
}


/**
 * Register our directory as a place to find provision classes.
 */
function provision_git_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_drush_init().
 */
function provision_git_drush_init() {
  provision_git_register_autoload();
}

/**
 *  Implements hook_provision_services().
 */
function provision_git_provision_services() {
  provision_git_register_autoload();
  return array('git' => NULL);
}
