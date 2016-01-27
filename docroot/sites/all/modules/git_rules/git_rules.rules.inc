<?php
/**
 * @file
 * Rules module integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function git_rules_rules_event_info() {
  $events = array();

  $events['git_rules_git_post_receive'] = array(
    'label' => t('After receiving a git post receive'),
    'module' => 'Git Rules',
    'arguments' => array(
      'repository' => array(
        'type' => 'string',
        'label' => t('Repository name'),
      ),
      'repository_path' => array(
        'type' => 'string',
        'label' => t('Repository path'),
      ),
    ),
  );

  $events['git_rules_git_post_receive_commit'] = array(
    'label' => t('After receiving a git post receive commit'),
    'module' => 'Git Rules',
    'arguments' => array(
      'id' => array(
        'type' => 'string',
        'label' => t('The commit id'),
      ),
      'timestamp' => array(
        'type' => 'date',
        'label' => t('Commit date'),
      ),
      'author_email' => array(
        'type' => 'string',
        'label' => t('Author Email'),
      ),
      'author_name' => array(
        'type' => 'string',
        'label' => t('Author Name'),
      ),
      'message' => array(
        'type' => 'string',
        'label' => t('Commit message'),
      ),
      'repository' => array(
        'type' => 'string',
        'label' => t('The repository name'),
      ),
      'repository_path' => array(
        'type' => 'string',
        'label' => t('Repository path'),
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function git_rules_rules_condition_info() {
  $conditions = array();

  $conditions['git_rules_repository_exists'] = array(
    'label' => t('Evaluate if a repository exists'),
    'module' => 'Git Rules',
    'arguments' => array(
      'repo_path' => array(
        'type' => 'string',
        'label' => t('Repository path'),
        'description' => t('The path to the repository.'),
      ),
    ),
  );

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function git_rules_rules_action_info() {
  $actions = array();

  $actions['add'] = array(
    'label' => t('Add/Stage files to repository (git add)'),
    'group' => t('Git Rules'),
    'base' => 'git_rules_actions_add',
    'arguments' => array(
      'local_path' => array(
        'type' => 'text',
        'label' => t('Local Repository path'),
        'description' => t('The path of the repository.'),
      ),
    ),
  );

  $actions['commit'] = array(
    'label' => t('Commit staged files (git commit)'),
    'group' => t('Git Rules'),
    'base' => 'git_rules_actions_commit',
    'arguments' => array(
      'local_path' => array(
        'type' => 'text',
        'label' => t('Local Repository path'),
        'description' => t('The path of the repository.'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Commit message'),
        'description' => t('A message describing the purpose of the commit.'),
      ),
    ),
  );

  $actions['clone'] = array(
    'label' => t('Clone a repository (git clone)'),
    'group' => t('Git Rules'),
    'base' => 'git_rules_actions_clone',
    'arguments' => array(
      'external_path' => array(
        'type' => 'text',
        'label' => t('External repository path'),
        'description' => t('The url of the repository.'),
      ),
      'local_path' => array(
        'type' => 'text',
        'label' => t('Local Repository path'),
        'description' => t('The path where the repository should be cloned.'),
      ),
      'bare' => array(
        'type' => 'boolean',
        'label' => t('Bare'),
        'description' => t('Equal to the --bare property.'),
      ),
    ),
  );

  $actions['fetch'] = array(
    'label' => t('Fetch latest commits (git fetch)'),
    'group' => t('Git Rules'),
    'base' => 'git_rules_actions_fetch',
    'arguments' => array(
      'external_path' => array(
        'type' => 'text',
        'label' => t('External repository path or the remote to fetch, and refspec'),
        'description' => t('The url of the repository.'),
      ),
     'local_path' => array(
        'type' => 'text',
        'label' => t('Local Repository path'),
        'description' => t('The path of the repository.'),
      ),
    ),
  );

  $actions['init'] = array(
    'label' => t('Initialize a repository (git init)'),
    'group' => t('Git Rules'),
    'base' => 'git_rules_actions_init',
    'arguments' => array(
      'local_path' => array(
        'type' => 'text',
        'label' => t('Local Repository path'),
        'description' => t('The path where the repository should be initialized.'),
      ),
      'bare' => array(
        'type' => 'boolean',
        'label' => t('Bare'),
        'description' => t('Equal to the --bare property.'),
      ),
    ),
  );

  $actions['push'] = array(
    'label' => t('Push commits to remote (git push)'),
    'group' => t('Git Rules'),
    'base' => 'git_rules_actions_push',
    'arguments' => array(
      'local_path' => array(
        'type' => 'text',
        'label' => t('Local Repository path'),
        'description' => t('The path of the repository.'),
      ),
      'remote_name' => array(
        'type' => 'text',
        'label' => t('Remote'),
        'description' => t('The remote name.'),
      ),
      'branch' => array(
        'type' => 'text',
        'label' => t('Branch'),
        'description' => t('The branch name.'),
        'optional' => TRUE,
      ),
    ),
  );

  $actions['remote_add'] = array(
    'label' => t('Add a Remote (git remote add)'),
    'group' => t('Git Rules'),
    'base' => 'git_rules_actions_remote_add',
    'arguments' => array(
      'external_path' => array(
        'type' => 'text',
        'label' => t('External repository path'),
        'description' => t('The url of the repository.'),
      ),
      'local_path' => array(
        'type' => 'text',
        'label' => t('Local Repository path'),
        'description' => t('The path of the repository.'),
      ),
      'name' => array(
        'type' => 'text',
        'label' => t('The Remote name'),
        'description' => t('The remote name, if it already exists, this will result in errors.'),
      ),
    ),
  );

  $actions['remote_rm'] = array(
    'label' => t('Remove a Remote (git remote rm)'),
    'group' => t('Git Rules'),
    'base' => 'git_rules_actions_remote_rm',
    'arguments' => array(
      'local_path' => array(
        'type' => 'text',
        'label' => t('Local Repository path'),
        'description' => t('The path of the repository.'),
      ),
      'name' => array(
        'type' => 'text',
        'label' => t('Name'),
        'description' => t('The Remote name.'),
      ),
    ),
  );

  return $actions;
}

/**
 * Check that a repository exists by verifying the HEAD file and /objects
 * directory.
 *
 * @param string $repo_path
 * @return true if the path is a repo, false otherwise.
 */
function git_rules_repository_exists($repo_path)  {
  $repo_path = drupal_realpath($repo_path);
  $repo_path = is_dir($repo_path . '/.git') ? $repo_path . '/.git' : $repo_path;
  return file_exists($repo_path . '/HEAD') && is_dir($repo_path . '/objects');
}

/**
 * Callback for 'add' rules action.
 *
 * @param string $local_path
 */
function git_rules_actions_add($local_path) {
  if (is_dir($local_path)) {
    $local_path = drupal_realpath($local_path);
    $cmd = 'cd ' . $local_path . '; git add .';

    git_rules_command_queue_add($cmd);
  }
}

/**
 * Callback for 'commit' rules action.
 *
 * @param string $local_path
 * @param string $message
 */
function git_rules_actions_commit($local_path, $message) {
  if (is_dir($local_path)) {
    $local_path = drupal_realpath($local_path);
    $cmd = 'cd ' . $local_path . '; git commit -a -m "' . $message . '"';

    git_rules_command_queue_add($cmd);
  }
}

/**
 * Callback for 'clone' rules action.
 *
 * @param string $external_path
 * @param string $local_path
 * @param boolean $bare
 */
function git_rules_actions_clone($external_path, $local_path, $bare = FALSE) {
  $cmd = 'git clone ';

  // Add the bare parameter.
  if ($bare) {
    $cmd .= '--bare ';
  }

  $cmd .= $external_path . ' ' . drupal_realpath($local_path);

  git_rules_command_queue_add($cmd);
}

/**
 * Callback for 'fetch' rules action.
 *
 * @param string $external_path the path to the repo. The refspec can also be specified.
 * @param string $local_path
 */
function git_rules_actions_fetch($external_path, $local_path) {
  if (is_dir($local_path)) {
    $local_path = drupal_realpath($local_path);
    $cmd = 'cd ' . $local_path . '; git fetch ' . $external_path;

    git_rules_command_queue_add($cmd);
  }
}

/**
 * Callback for 'init' rules action.
 *
 * @param string $local_path the local path to the repository.
 * @param bool $bare if the repository should be inited as a bare repo.
 */
function git_rules_actions_init($local_path, $bare) {
  $local_path = drupal_realpath($local_path);
  $cmd = 'cd ' . $local_path . '; git init';
  if (!is_dir($local_path)) {
    $cmd = 'mkdir ' . $local_path . '; ' . $cmd;
  }
  if ($bare) {
    $cmd .= ' --bare ';
  }

  git_rules_command_queue_add($cmd);
}

/**
 * Callback for 'push' rules action.
 *
 * @param string $local_path the local path to the repository.
 * @param string $remote_name
 */
function git_rules_actions_push($local_path, $remote_name, $branch = '') {
  if (is_dir($local_path)) {
    $local_path = drupal_realpath($local_path);

    $cmd = 'cd ' . $local_path . '; git push ' . $remote_name;
    if (!empty($branch)) {
      $cmd .= ' ' . $branch;
    }

    git_rules_command_queue_add($cmd);
  }
}

/**
 * Callback for 'remote_add' rules action.
 *
 * @param string $external_path
 * @param string $local_path
 * @param string $remote_name
 */
function git_rules_actions_remote_add($external_path, $local_path, $remote_name) {
  if (is_dir($local_path)) {
    $local_path = drupal_realpath($local_path);

    $cmd = 'cd ' . $local_path . '; git remote add ' . $remote_name . ' ' . $external_path;

    git_rules_command_queue_add($cmd);
  }
}
/**
 * Callback for 'remote_rm' rules action.
 *
 * @param string $local_path
 * @param string $remote_name
 */
function git_rules_actions_remote_rm($local_path, $remote_name) {
  if (is_dir($local_path)) {
    $local_path = drupal_realpath($local_path);
    $cmd = 'cd ' . $local_path . '; git remote rm ' . $remote_name;

    git_rules_command_queue_add($cmd);
  }
}

