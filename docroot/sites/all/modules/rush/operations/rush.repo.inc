<?php
/**
 * @file
 * Repo Operations for Drush Rush.
 * @see operations
 * @backlog Allow branch, tag, and commit id to be passed via command line.
 * @backlog Refactor to allow for multiple remotes to be set and then have pull,
 * push, and anything else work off the remote rather than url.
 * @backlog Allow for selection of remote in params files.
 */

/**
 * @addtogroup operations Operations
 * @{
 */

/**
 * @addtogroup repo-operations Version Control Operations
 * @brief Repo operation functions for Rush.
 * @details Includes functions to:
 * - Create remote github repos (public and private)
 * - Initialize a git repo
 * - Initialize a git flow ready repo
 * - Run miscellaneous git commands
 * @{
 */

/**
 * Create a remote repo using the given repo type.
 *
 * This function is a factory which delegates action to other
 * repo service functions.  Though only one parameter is required for this
 * function to route its parameters to the correct service function, the
 * repo service functions get their parameters from those passed to this
 * function.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['repo']['service'] string
 *   Required.  A string to designate the repo service such as
 *   'github' or 'bitbucket'.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_create_remote_repo($params) {

  if (empty($params['repo']['service'])) {
    return rush_empty_error('$params[\'repo\'][\'service\']', __FUNCTION__);
  }

  $function = '_rush_create_remote_repo_' . $params['repo']['service'];
  if (function_exists($function)) {
    return $function($params);
  }
  else {
    return drush_set_error('RUSH_REPO_OP_ERROR', dt('[ERROR]: *** Unable to run !function ***', array('!function' => $function)));
  }
}

/**
 * Run `git init` in the specified directory.
 *
 * Initialize a git repo in a specified directory.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_init($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  $cmd = 'git init ' . $params['build']['directory_path'];
  $msg = dt('[GIT INIT]: !path', array('!path' => $params['build']['directory_path']));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}

/**
 * Run `git flow init -d` in the specified directory.
 *
 * Initialize a git flow ready repo in a specified directory.
 * Note: Git flow must be installed and configured.
 * See: https://github.com/nvie/gitflow
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_flow_init($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  $cmd = 'cd ' . $params['build']['directory_path'] . ' ;git flow init -d';
  $msg = dt('[GIT FLOW INIT]: !path', array('!path' => $params['build']['directory_path']));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);
}

/**
 * Run `git add --all` in the specified directory.
 *
 * Add all files in the specified directory to git version control.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_add($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }

  $cmd = 'cd ' . $params['build']['directory_path'] . ' ; git add --all';
  $msg = dt('[GIT ADD ALL]: !path', array('!path' => $params['build']['directory_path']));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}

/**
 * Run `git commit --all` with a commit message.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *    - $params['repo']['commit_message'] string
 *     Required.  A string containing a commit message.
 *     Defaults to: 'Auto Commit by Drush Rush'
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_commit($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  if (empty($params['repo']['commit_message'])) {
    return rush_empty_error('$params[\'repo\'][\'commit_message\']', __FUNCTION__);
  }
  $build_directory_path = $params['build']['directory_path'];
  $repo_commit_message  = $params['repo']['commit_message'];
  $cmd                  = "cd $build_directory_path; git commit --all -m '$repo_commit_message'";
  $msg                  = dt('[GIT COMMIT ALL]: !path', array('!path' => $params['build']['directory_path']));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}

/**
 * Run `git push` in the specified directory.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['repo']['remote'] string
 *     Required.  A string indicating which remote to push to.
 *     Defaults to 'origin'.
 *   - $params['repo']['branch'] string
 *     Required.  A string indicating which branch to push to.
 *     Defaults to 'master'.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_push($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  if (empty($params['repo']['remote'])) {
    return rush_empty_error('$params[\'repo\'][\'remote\']', __FUNCTION__);
  }
  if (empty($params['repo']['branch'])) {
    return rush_empty_error('$params[\'repo\'][\'branch\']', __FUNCTION__);
  }

  $cmd = 'cd ' . $params['build']['directory_path'] . '; git push ' . $params['repo']['remote'] . ' ' . $params['repo']['branch'];
  $msg = dt('[PUSH TO REPO]: Code pushed to repo from !path', array('!path' => $params['build']['directory_path']));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}

/**
 * Run `git branch` to create a branch with the specified name.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['repo']['new_branch'] string
 *     Required. A string indicating the name of the new branch.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_branch($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  if (empty($params['repo']['new_branch'])) {
    return rush_empty_error('$params[\'repo\'][\'new_branch\']', __FUNCTION__);
  }

  $msg = dt('[Branch]: !repo_new_branch ', array('!repo_new_branch' => $params['repo']['new_branch']));
  $cmd = 'cd ' . $params['build']['directory_path'] . '; git branch ' . $params['repo']['new_branch'];

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}

/**
 * Run `git checkout` to checkout the specified branch, tag, or commit ID.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['repo']['checkout'] string
 *     Required. A string indicating which branch, tag, or commit to checkout.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_checkout($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  if (empty($params['repo']['checkout'])) {
    return rush_empty_error('$params[\'repo\'][\'checkout\']', __FUNCTION__);
  }

  $cmd = 'cd ' . $params['build']['directory_path'] . '; git checkout ' . $params['repo']['checkout'];
  $msg = dt('[CHECKOUT]: !branch ', array('!branch' => $params['repo']['checkout']));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}

/**
 * Run `git checkout -b` to create and checkout a new branch.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['repo']['create_branch'] string
 *     Required. A string indicating the name of the new branch.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_create_branch($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  if (empty($params['repo']['create_branch'])) {
    return rush_empty_error('$params[\'repo\'][\'create_branch\']', __FUNCTION__);
  }

  $msg = dt('[Create Branch]: !repo_new_branch ', array('!repo_new_branch' => $params['repo']['create_branch']));
  $cmd = 'cd ' . $params['build']['directory_path'] . '; git checkout -b ' . $params['repo']['create_branch'];

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}


/**
 * Run `git remote add` in the specified directory.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['repo']['url'] string
 *     Required. A string containing the URL of the repo.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_remote_add($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  if (empty($params['repo']['url'])) {
    return rush_empty_error('$params[\'repo\'][\'url\']', __FUNCTION__);
  }

  $cmd = 'cd ' . $params['build']['directory_path'] . '; git remote add --track master origin ' . $params['repo']['url'];
  $msg = dt('[GIT REMOTE ADD]: !path', array('!path' => $params['build']['directory_path']));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}

/**
 * Run `git clone` into the specified directory.
 *
 * Clone a repo to the specified  directory.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['repo']['url'] string
 *     Required. A string containing the URL of the repo.
 *   - $params['repo']['branch'] string
 *     Required. A string indicating which branch to push to.
 *     Defaults to 'master'.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_clone($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  if (empty($params['repo']['url'])) {
    return rush_empty_error('$params[\'repo\'][\'url\']', __FUNCTION__);
  }
  if (empty($params['repo']['branch'])) {
    return rush_empty_error('$params[\'repo\'][\'branch\']', __FUNCTION__);
  }

  $note = dt('[START GIT CLONE]: Starting: git clone !repo_url !directory_path --branch=!branch .  This may take some time.***', array(
    '!repo_url'       => $params['repo']['url'],
    '!directory_path' => $params['build']['directory_path'],
    '!branch'         => $params['repo']['branch'],
  ));
  drush_log($note, 'status');

  $cmd = 'git clone ' . $params['repo']['url'] . ' ' . $params['build']['directory_path'] . ' --branch=' . $params['repo']['branch'];
  $msg = dt('[GIT CLONE]: git clone !repo_url !directory_path --branch=!branch ***', array(
    '!repo_url'       => $params['repo']['url'],
    '!directory_path' => $params['build']['directory_path'],
    '!branch'         => $params['repo']['branch'],
  ));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);
}

/**
 * Run `git pull` in the specified directory.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_op_git_pull($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }

  $cmd = 'cd ' . $params['build']['directory_path'] . '; git pull';
  $msg = dt('[GIT PULL]: to !path', array('!path' => $params['build']['directory_path']));

  return rush_shell_exec($cmd, $msg, 'RUSH_REPO_OP_ERROR', __FUNCTION__);

}

/**
 * Create a remote repo using GITHUB's api.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['repo']['name'] string
 *   Required. The name of the remote repo to create.
 *   - $params['repo']['api_url'] string
 *   Required. A string containing the URL endpoint of the repo api such as,
 *   'https://api.github.com'.
 *   With GitHub, the endpoint is 'https://api.github.com', unless working with
 *   an enterprise install. For an enterprise install use:
 *   yourdomain.com/api/v3/
 *   See http://developer.github.com/v3/.
 *   - $params['repo']['api_account'] string
 *   Required. A string containing the URL argument to designate which account
 *   to use. With GitHub the string may be either:
 *   - '/user/' : This is the logged in user's individual account.
 *   - '/orgs/orgname' : In this case, orgname is an organization's account.
 *   - $params['repo']['user'] string
 *   Required. A string containing the API account user name.
 *   Typically a user's individual user account.
 *   $params['repo']['password'] string
 *   Required. A string containing the API account password.
 *   Typically a user's individual account password.
 *
 * @return bool
 *   TRUE on success, FALSE on fail (via drush_set_error).
 */
function _rush_create_remote_repo_github($params) {

  if (empty($params['repo']['name'])) {
    return rush_empty_error('$params[\'repo\'][\'name\']', __FUNCTION__);
  }
  if (empty($params['repo']['api_url'])) {
    return rush_empty_error('$params[\'repo\'][\'api_url\']', __FUNCTION__);
  }
  if (empty($params['repo']['api_account'])) {
    return rush_empty_error('$params[\'repo\'][\'api_account\']', __FUNCTION__);
  }
  if (empty($params['repo']['user'])) {
    return rush_empty_error('$params[\'repo\'][\'user\']', __FUNCTION__);
  }
  if (empty($params['repo']['password'])) {
    return rush_empty_error('$params[\'repo\'][\'password\']', __FUNCTION__);
  }

  $data        = $params['repo'];
  $url         = $params['repo']['api_url'] . $params['repo']['api_account'] . '/repos';
  $u_p         = $params['repo']['user'] . ':' . $params['repo']['password'];
  $data_string = json_encode($data);
  $ch          = curl_init($url);
  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
  $t_version = curl_version();
  curl_setopt($ch, CURLOPT_USERAGENT, 'curl/' . $t_version['version']);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_USERPWD, $u_p);
  curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array(
    'Content-Type: application/json',
    'Content-Length: ' . strlen($data_string),
  ));
  $result       = curl_exec($ch);
  $final_result = json_decode($result, TRUE);
  if ($final_result['name'] != $data['name']) {
    $inspect_post = json_decode($data_string);
    drush_print_r($inspect_post);
    drush_print_r($final_result);

    return drush_set_error('RUSH_REPO_OP_ERROR', dt('[ERROR]: *** Unable to create a remote repo. ***'));
  }
  else {
    $msg = dt('[CREATE REMOTE REPO]: !htmlurl.', array('!htmlurl' => $final_result['html_url']));
    drush_log($msg, 'success');

    return TRUE;
  }
}
/** @}*/
/** @}*/
