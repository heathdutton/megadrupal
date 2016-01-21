<?php

/**
 * @file
 *  The drush Issue Queue manager
 */

use Goutte\Client;
use Guzzle\Http\Client as GuzzleClient;
use Guzzle\Http\Exception\ClientErrorResponseException;

use Guzzle\Plugin\Cookie\Cookie;
use Guzzle\Plugin\Cookie\CookiePlugin;
use Guzzle\Plugin\Cookie\CookieJar\ArrayCookieJar;

define("DRUSH_IQ_CACHE_NEVER", 0);
define("DRUSH_IQ_CACHE_FOREVER", 1);
define("DRUSH_IQ_CACHE_CHECK_MOD_DATE", 2); // TODO - not supported, cannot get mod date. Works like CACHE_NEVER


/**
 * Implementation of hook_drush_command().
 */
function iq_drush_command() {
  $iq_global_options = array(
    'issue-site' => 'The site to read issues from. Defaults to https://drupal.org',
  );
  $items['iq-info'] = array(
    'description' => 'Show information about an issue from the queue on drupal.org.',
    'examples' => array(
      'drush iq-info 1234' => 'Get info on issue 1234.',
      'drush iq-info https://drupal.org/node/1234' => 'Get info on an issue identified by its URL.',
      'drush iq-info 1234-#3' => 'Get info on the patch on the third comment of issue 1234.',
      'drush iq-info 1234-5678' => 'Get info on the patch on comment id 5678 of issue 1234.',
    ),
    'arguments' => array(
      'number' => 'The issue number.',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'pipe' => 'Print the full issue info data structure.',
    ) + $iq_global_options,
    'outputformat' => array(
      'default' => 'key-value',
      'pipe-format' => 'json',
      'field-labels' => array(
        'title' => 'Title',
        'comment_count' => 'Number of Comments',
        'project' => 'Project',
        'projectTitle' => 'Project Title',
        'projectId' => 'Project ID',
        'projectUri' => 'Project URI',
        'created' => 'Created',
        'changed' => 'Changed',
        'url' => 'Issue URL',
        'projectUrl' => 'Project URL',
        'version' => 'Version',
        'component' => 'Component',
        'category' => 'Category',
        'categoryId' => 'Category ID',
        'priority' => 'Priority',
        'priorityId' => 'Priority ID',
        'authorId' => 'Author ID',
        'authorName' => 'Author',
        'authorUri' => 'Author URI',
        'authorUrl' => 'Author URL',
        'assignedName' => 'Assigned', // TODO - data not available
        'assignedId' => 'Assigned ID', // TODO - data not available
        'assignedUri' => 'Assigned URI', // TODO - data not available
        'assignedUrl' => 'Assigned URL', // TODO - data not available
        'issueStatus' => 'Status',
        'statusId' => 'Status ID',
        'patchUrl' => 'Patch URL',
        'patchIndex' => 'Patch Index', // TODO - data not available
        'patchAuthorId' => 'Patch Author ID',
        'patchName' => 'Patch Name',
        'patchId' => 'Patch ID', // TODO - contains patch index
      ),
      'fields-default' => array('title', 'url', 'project', 'projectTitle', 'projectURL', 'version', 'component', 'category', 'priority', 'assignedName', 'issueStatus', 'patchUrl', ),
      'field-mappings' => array(
        'projectId' => 'field_project/id',
        'projectUri' => 'field_project/uri',
        'statusId' => 'field_issue_status',
        'priorityId' => 'field_issue_priority',
        'categoryId' => 'field_issue_category',
        'component' => 'field_issue_component',
        'version' => 'field_issue_version',
        'authorId' => 'author/id',
        'authorUri' => 'author/uri',
      ),
      'output-data-type' => 'format-list',
    ),

    'aliases' => array('iqi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-branch'] = array(
    'description' => 'List branches created by iq commands and switch to or delete them.',
    'examples' => array(
      'drush iq-branch' => 'Let the user select the iq branch to switch to from a list of available branches.',
      'drush iq-branch delete' => 'Prompt the user and then delete the selected branch',
      'drush iq-branch list' => 'Lists all iq branches.',
      'drush iq-branch 1234' => 'Switch back to the iq branch corresponding to issue 1234.  Will create a new working branch if one does not already exist.',
    ),
    'arguments' => array(
      'branch command' => 'The branch operation to perform.  One of switch, list or delete. Optional; defaults to switch.',
      'number' => 'The issue number.',
    ),
    'options' => array(
      'all' => 'Delete all branches.  Only applicable to the "iq-branch delete" command.',
      'base' => 'When creating a new branch while working in a detached HEAD state, creates a branch to serve as the base for future diffs and merges.  Only applicable to the "iq-branch 1234" (create) command.',
    ) + $iq_global_options,
    'aliases' => array('iqb'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-create-commit-comment'] = array(
    'description' => 'Create a commit comment for the specified issue number.',
    'examples' => array(
      'drush iq-create-commit-comment 1234' => 'Generate a commit comment using the title from issue 1234, and crediting every user who provided attachments.',
    ),
    'arguments' => array(
      'number' => 'The issue number.',
    ),
    'aliases' => array('iqccc', 'ccc'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-apply-patch'] = array(
    'description' => 'Look up the most recent patch attached to the specified issue, and apply it to its project.',
    'examples' => array(
      'drush iq-apply-patch 1234' => 'Apply the newest patch attached to issue 1234.',
      'drush iq-apply-patch 1234-#5' => 'Apply the patch attached to the fifth comment of issue 1234.',
      'drush iq-apply-patch 1234-5678' => 'Apply the patch attached to comment id 5678 of issue 1234.',
      'drush iq-apply-patch 1234 --select' => 'Show all patches attached to issue 1234, and prompt for the one to apply.',
    ),
    'arguments' => array(
      'number' => 'The issue number. Optional; default is to attempt to find a patch from the issue open in a browser window.',
    ),
    'options' => array(
      'no-prefix' => 'Patch was created with --no-prefix, and therefore should be applied with -Np0 instead of -Np1.  Optional; default is to try both.',
      'no-git' => 'Do not execute any git commands. Default is to create a new branch for the issue and commit the patch.',
      'no-commit' => 'Create a new branch, but do not commit the patch to the issue working branch after applying it. Optional. Patches created by git format-patch are always committed.',
      'keep-patch' => 'Keep the patchfile after applying it.  Default is to delete the patchfile.',
      'dry-run' => 'Do not apply the patch; just print out messages about what would be done.',
      'select' => 'Prompt for which patch to apply.  Optional; default is newest patch.',
      'base' => 'When applying a patch while working in a detached HEAD state, creates a branch to serve as the base for future diffs and merges.',
    ) + $iq_global_options,
    'aliases' => array('patch', 'iqa', 'am', 'iq'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-diff'] = array(
    'description' => 'Create a diff.  Uses `git format-patch`, so author information is included.',
    'examples' => array(
      'drush iq-diff' => 'Create a diff between the current branch and its upstream branch.',
      'drush iq-diff --commit' => 'Commit the changes to the current branch after creating the diff.',
    ),
    'arguments' => array(
      'number' => 'The issue number.',
    ),
    'options' => array(
      'no-prefix' => 'Create patch with no prefix.  Not recommended; patch will have to be applied with -Np0 instead of -Np1.',
      'no-git' => 'Do not execute any git commands; just run diff. Default is to use git format-patch.',
      'commit' => 'Commit change to current patch review branch. Optional; default is to leave changes unstaged.',
      'no-squash' => 'Show all commits in the branch, like a standard format-patch. Default is to squash into a single commit.',
      'rebase' => 'Rebase before creating patch.',
      'message' => 'Commit message.  Optional; default is to use `drush iq-create-commit-comment`.',
    ) + $iq_global_options,
    'aliases' => array('diff', 'iqd'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-reset'] = array(
    'description' => 'Stop working on a patch, and return to the original branch.',
    'examples' => array(
      'drush iq-reset' => 'Safely go back to the original branch. Work on the patch may resume later by returning to the working branch.',
      'drush iq-reset --hard' => 'Permanently delete all changes, and go back to the original branch.',
    ),
    'options' => array(
      'hard' => 'Also delete the working branch.',
    ) + $iq_global_options,
    'aliases' => array('reset', 'iqr'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-merge'] = array(
    'description' => 'Merge patch working branch into the original branch.',
    'examples' => array(
      'drush iq-merge' => 'Go back to the original branch, bringing all commits along.',
      'drush iq-merge --squash' => 'Merge multiple commits into a single commit when merging. May discard some author credit, as only the last commit is recorded.',
    ),
    'options' => array(
      'squash' => 'Merge all commits into one, keeping only the last.',
      'no-commit' => 'Skip committing unstaged changes.  Default is to commit everything.',
    ) + $iq_global_options,
    'aliases' => array('merge', 'iqm'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-lint'] = array(
    'description' => 'Run lint (php -l) on files that have changed in the current patch.',
    'examples' => array(
      'drush iq-lint' => 'Run lint on modified files.',
    ),
    'aliases' => array('lint', 'iql'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-submit'] = array(
    'description' => 'Submit a patch for an issue on drupal.org.',
    'examples' => array(
      'drush iq-submit "This is my new patch"' => 'Make a patch from the current branch. The branch must have been created by drush iq-apply-patch.',
      'drush iq-submit "This is my patch" 123456' => 'Make a patch from the current branch and/or unstaged changes, and submit it to the specified issue id.',
      'drush iq-submit 123456 "This is my patch"' => 'Note that the issue id can come before or after the commit comment.',
    ),
    'arguments' => array(
      'comment' => 'Description of the patch.',
      'issue' => 'Issue id to submit the patch to.',
    ),
    'options' => array(
      'user' => 'The username to submit the patch as.',
      'pass' => 'The password to use to authenticate the username with.',
      'interdiff' => array('description' => 'Select the size ratio at which an interdiff will be made. Default is 0.3.',
        'example-value' => '0.3',
      ),
      'no-interdiff' => 'Prevent interdiff from being created. Equivalent to --interdiff=0',
      'no-patch' => 'Change issue metadata without submitting a patch',
      'do-not-test' => 'Add the -do-not-test marker to patchfile. Ignored if --filename specified.',
      'filename' => 'Specify the exact name to use with patch.',

      'title' => 'Set the issue title.',
      'version' => 'Set the issue version.',
      'component' => 'Set the issue component.',
      'assigned' => 'Set the drupal.org user who the issue is assigned to.',
      'category' => 'Set the issue category.',
      'priority' => 'Set the issue priority.',
      'issue-status' => 'Set the issue status.',
      'tags' => 'Set the issue tags.  Note that this overwrites any existing tags.',
      'tag' => 'Add a tag to the issue.  Note that the new tag will be appended to the end of any existing tags.',
      'nw' => 'Needs work.  By default, iq-submit will set the status of an issue to "Needs review" unless --issue-status is used to set it to something else.  --nw is shorthand for --issue-status="needs work".'
    ) + $iq_global_options,
    'aliases' => array('iqs'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['iq-release'] = array(
    'description' => 'Create a new release for a project on drupal.org.  Creates a git tag, uses `drush release-notes` to generate release notes, and then publishes the release.',
    'examples' => array(
      'drush iq-release "This is my awesome new version"' => 'Make a release from the current branch. The branch must be a development branch, such as 7.x-1.x.',
    ),
    'arguments' => array(
      'comment' => 'Description of the release.',
    ),
    'options' => array(
      'user' => 'The username to make the release as.',
      'pass' => 'The password to use to authenticate the username with.',
      'security' => 'Release contains security fixes. You must contact the Drupal Security team prior to releasing security fixes, or the release will not be published.  Default is no security fixes.',
      'features' => 'Release contains new features. Default is no new features.',
      'no-bugs' => 'Release contains no bug fixes. Default is contains bug fixes.',
      'tag-only' => 'Create a git tag and push it, but do not publish the release.',
      'alpha' => 'Create an alpha release. Only usable if there are no releases on the current branch, or if the previous release is an alpha release.',
      'beta' => 'Create a beta release. Only usable if there are no releases on the current branch, or if the previous release is an alpha or beta release.',
      'rc' => 'Create an RC release.  Only usable if there are no releases on the current branch, or if the previous release is alpha, beta or RC.',
      'stable' => 'Create a stable release.',
      'i-have-contacted-the-security-team' => 'In order to make a release that is tagged to contain security fixes, you must first contact the Drupal Security Team and advise them of the release. You will be prompted to confirm whether you have done this whenever using the --security flag unless you use this flag to skip the prompt.',
    ) + $iq_global_options,
    'aliases' => array('iqrel', 'rel'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
    'drush dependencies' => array('grn'),
  );
  $items['iq-open'] = array(
    'description' => 'Prompt the user and then open one file that has changed on the current branch.',
    'examples' => array(
      'drush iq-open' => 'Open one of the files changed by the last iq-apply-patch.',
      'drush iq-open --search="modification"' => 'Open one of the files changed by the last iq-apply-patch, where "modification" was added or removed by that patch.'
    ),
    'arguments' => array(
      'filter' => 'A substring for filtering the list of files. Omit this argument to choose from all patched files.',
    ),
    'options' => array(
      'all' => 'Open all changed files without prompting.',
      'grep' => 'Only consider files where the specified regular expression was added or removed by the last applied patch.',
      'search' => 'Only consider files where the specified search string was added or removed by the last applied patch.',
    ) + $iq_global_options,
    'aliases' => array('iqo'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('docs-iq-commands'),
  );
  $items['docs-iq'] = array(
    'description' => 'Issue queue commands for applying and creating patches from drupal.org.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(dirname(__FILE__) . '/README.txt'),
    'aliases' => array('docs-iq-commands'),
  );

  return $items;
}

function drush_iq_shell_exec_interactive($cmd) {
  if (!drush_get_context('DRUSH_BACKEND', FALSE)) {
    return _drush_shell_exec(func_get_args(), TRUE);
  }
  else {
    $return = _drush_shell_exec(func_get_args(), FALSE);
    drush_print(implode("\n", drush_shell_exec_output()));
    return $return;
  }
}


function drush_iq_diff($number = NULL) {
  $branch_merges_with = FALSE;
  $issue_info = array();
  $remote = FALSE;
  if (isset($number)) {
    $issue_info = drush_iq_get_info($number);
    if (!$issue_info) {
      return FALSE;
    }
    $branch = _drush_iq_get_branch($issue_info);
    $dir = _drush_iq_project_dir($issue_info);
    if (!$dir) {
      return drush_set_error('DRUSH_IQ_DIFF_NO_PROJECT', dt('Could not find the project directory for !n', array('!n' => $number)));
    }
  }
  else {
    $dir = _drush_iq_git_root_dir_from_cwd();
    $branch = _drush_iq_get_branch_at_dir($dir);
    $branch_merges_with = _drush_iq_get_branch_merges_with($dir);
    if ($branch_merges_with === FALSE) {
      return FALSE;
    }
    if (!empty($branch_merges_with)) {
      $remote = _drush_iq_get_best_remote($branch_merges_with);
      $branch_merges_with = _drush_iq_merge_branch_with_remote($remote, $branch_merges_with);
    }
  }
  if (!$branch_merges_with && ($branch != "(no branch)")) {
    $branch_merges_with = 'master';
  }

  // Not under git revision control is an error
  if (empty($branch)) {
    return drush_set_error('DRUSH_NO_VCS', dt("Error: drush can only produce diffs of projects under git version control"));
  }

  // If the user did not provide an issue number, but we can find
  // that a git branch has been made at the current working directory,
  // then we will expect that the branch tag is in the format of
  // "arbitrary-prequel-ISSUENUMBER".  If it is in this form, drush_iq_get_info
  // will be able to find the issue number.
  if (isset($branch) && ($branch != "master") && empty($issue_info)) {
    $issue_info_id = drush_iq_issue_number($branch);
    if (!empty($issue_info_id)) {
      $number = $issue_info_id['id'];
      $issue_info = drush_iq_fetch_issue_info($issue_info_id);
    }
  }
  // Add on extra flags
  $extra = "";
  $git_global_options = "";
  if (drush_get_option('no-prefix', FALSE)) {
    $extra .= ' --no-prefix';
  }
  // Don't use a pager when running in backend mode
  if (drush_get_context('DRUSH_BACKEND', FALSE)) {
    $git_global_options .= ' --no-pager';
  }
  $cwd = getcwd();
  $committed = FALSE;
  $squashed = FALSE;
  drush_op('chdir', $dir);
  $patch_description = "drush-iq";
  if (!empty($issue_info)) {
    $comment_number = array_key_exists('comment-number', $issue_info) ? $issue_info['comment-number'] : 1 + count($issue_info['comments']);
    $patch_description = _drush_iq_make_description($issue_info['title']) . '-' . $issue_info['id'] . '-' . $comment_number . '.patch';
  }
  drush_log(dt("# Create patch: !patch_description (branch: !b merge: !m)", array('!patch_description' => $patch_description, '!b' => $branch, '!m' => $branch_merges_with)), 'notice');

  // If we have modified the master branch (not recommended), just run git diff
  if (($branch == "master") || ($branch == "(no branch)") || ($branch == $branch_merges_with) || drush_get_option('no-git', FALSE)) {
    $result = drush_iq_shell_exec_interactive("git $git_global_options diff $extra %s", $branch_merges_with);
  }
  // Changes are being made under a local branch.  Do the recommended procedure for creating the diff.
  else {
    $commit_comment = _drush_iq_create_commit_comment($issue_info, drush_get_option('committer', TRUE), drush_get_option('message', FALSE));
    $result = drush_shell_exec("git status -s");
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("`git status -s` failed."));
    }
    $status_output = drush_shell_exec_output();
    // If there are unstaged changes, commit them
    if (!empty($status_output)) {
      // TODO: can we easily ignore *.patch, etc., or easily add only modified files?
      $result = drush_shell_exec("git add -u .");
      if (!$result) {
        return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("`git add -u` failed."));
      }
      $result = drush_shell_exec("git commit -m %s", $commit_comment);
      if (!$result) {
        // TODO:  can we undo the `git add -u`?
      }
      else {
        $committed = TRUE;
      }
    }
    // If the branch to merge with is remote, then fetch and rebase
    if (drush_get_option('rebase', FALSE)) {
      if ($remote) {
        $result = drush_shell_exec("git fetch %s %s && git rebase %s", $remote, $branch, $branch_merges_with);
        if (!$result) {
          return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("`git fetch && git rebase` failed."));
        }
      }
      else {
        drush_log(dt('--rebase ignored because !branch has no upstream branch.', array('!branch' => $branch)), 'warning');
      }
    }

    // Check to see if there are multiple commits on this branch. If there
    // are, make a new temporary branch and squash-merge all commits there.
    if (!drush_get_option('no-squash', FALSE)) {
      $result = drush_shell_exec("git log --oneline %s..HEAD", $branch_merges_with);
      if (!$result) {
        return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("`git log` failed."));
      }
      $commits_output = drush_shell_exec_output();
      if (count($commits_output) > 1) {
        // Try to delete our temporary branch if
        // it happens to be hanging around.  Ignore
        // errors, as it usually will not exist.
        drush_shell_exec('git branch -D drush-iq-temp 2>/dev/null');
        $result = drush_shell_exec('git checkout %s', $branch_merges_with);
        if (!$result) {
          return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not switch to branch !b", array('!b' => $branch_merges_with)));
        }
        $result = drush_shell_exec('git checkout -b drush-iq-temp');
        if (!$result) {
          return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not create a temporary branch to work with."));
        }
        $result = drush_shell_exec('git merge --squash %s', $branch);
        if (!$result) {
          // TODO: clean up
          return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not squash multiple commits onto our temporary branch."));
        }
        $result = drush_shell_exec("git commit -m %s", $commit_comment);
        if (!$result) {
          // TODO: clean up
          return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not commit squashed commits onto our temporary branch."));
        }
        $squashed = TRUE;
      }
    }

    $result = drush_iq_shell_exec_interactive("git $git_global_options format-patch %s -k --patience $extra --stdout", $branch_merges_with);
    // Switch back to the branch we were on before
    // and delete the temporary branch we created.
    if ($squashed) {
      drush_shell_exec('git checkout %s', $branch);
      drush_shell_exec('git branch -D drush-iq-temp  2>/dev/null');
    }
    if ($committed && !drush_get_option('commit', FALSE)) {
      // Get rid of our commit
      drush_shell_exec("git reset HEAD~1");
    }
  }
  drush_op('chdir', $cwd);
  return $issue_info;
}

/**
 * iq-branch command callback
 */
function drush_iq_branch($command = 'switch', $number = NULL) {
  if (!isset($number)) {
    $issue_info_id = drush_iq_issue_number($command);
    if (!empty($issue_info_id)) {
      $number = $command;
      $command = 'switch';
    }
  }
  $branch_command_aliases = array(
    'l' => 'list',
    'ls' => 'list',
    'd' => 'delete',
    'del' => 'delete',
  );
  if (array_key_exists($command, $branch_command_aliases)) {
    $command = $branch_command_aliases[$command];
  }
  $function = 'drush_iq_branch_' . $command;
  if (function_exists($function)) {
    $issue_info = _drush_iq_find_project_dir($number);
    if ($issue_info == FALSE) {
      return FALSE;
    }
    return $function($issue_info);
  }
  else {
    return drush_set_error('DRUSH_IQ_UNKNOWN_BRANCH_COMMAND', dt('Unknown branch command "!command"', array('!command' => $command)));
  }
}

/**
 * iq-branch list command callback
 */
function _drush_iq_branch_list($issue_info) {
  $iq_branches = array();
  $dir = _drush_iq_project_dir($issue_info);
  $result = drush_shell_cd_and_exec($dir, "git branch");
  if ($result) {
    $branch_output = drush_shell_exec_output();
    foreach($branch_output as $line) {
      $branch_name = substr($line, 2);
      if (substr($branch_name, 0, 9) == 'drush-iq-') {
        $iq_branches[] = $branch_name;
      }
    }
  }
  return $iq_branches;
}

function _drush_iq_branch_get_selections($issue_info) {
  $iq_branches = _drush_iq_branch_list($issue_info);
  $selections = array();
  if (array_key_exists('id', $issue_info)) {
    foreach ($iq_branches as $branch_name) {
      if (strstr('-' . $issue_info['id'] . '-', $branch_name) !== FALSE) {
        $selections[$branch_name] = $branch_name;
      }
    }
  }
  else {
    foreach ($iq_branches as $branch_name) {
      $selections[$branch_name] = $branch_name;
    }
  }
  return $selections;
}

function _drush_iq_branch_select($issue_info, $prompt) {
  $selections = _drush_iq_branch_get_selections($issue_info);
  if (empty($selections)) {
    return '';
  }
  $branch = drush_choice($selections, $prompt);
  if ($branch === FALSE) {
    return drush_user_abort();
  }
  return $branch;
}

/**
 * iq-branch list command callback
 */
function drush_iq_branch_list($issue_info) {
  $iq_branches = _drush_iq_branch_list($issue_info);
  drush_print(implode("\n", $iq_branches));
}

/**
 * iq-branch switch command callback
 */
function drush_iq_branch_switch($issue_info) {
  $branch = _drush_iq_branch_select($issue_info, dt('Select branch to switch to'));
  if ($branch === FALSE) {
    return FALSE;
  }
  if (empty($branch)) {
    if (array_key_exists('id', $issue_info)) {
      $confirm = drush_confirm(dt('No working branches available for issue !n.  Would you like to create one?', array('!n' => $issue_info['id'])));
      if (!$confirm) {
        return drush_user_abort();
      }
      return _drush_iq_create_branch($issue_info);
    }
    else {
      return drush_set_error('DRUSH_IQ_NO_BRANCH', dt('No working branches created by Drush iq commands available.'));
    }
  }
  $dir = _drush_iq_project_dir($issue_info);
  $result = drush_shell_cd_and_exec($dir, "git checkout %s", $branch);
  if ($result) {
    drush_log(dt('Switched to branch !branch', array('!branch' => $branch)), 'ok');
  }
  return $branch;
}

/**
 * iq-branch delete command callback
 */
function drush_iq_branch_delete($issue_info) {
  $dir = _drush_iq_project_dir($issue_info);
  if (drush_get_option('all', FALSE)) {
    $selections = _drush_iq_branch_get_selections($issue_info);
    drush_print("The following branches exist:\n");
    drush_print(implode("\n", $selections));
    $confirm = drush_confirm(dt('Would you like to delete them all?'));
    if (!$confirm) {
      return drush_user_abort();
    }
    foreach ($selections as $branch) {
      $result = drush_shell_cd_and_exec($dir, "git branch -D %s", $branch);
      if ($result) {
        drush_log(dt('Deleted branch !branch', array('!branch' => $branch)), 'ok');
      }
    }
    return TRUE;
  }
  $branch = _drush_iq_branch_select($issue_info, dt('Select branch to delete'));
  if ($branch === FALSE) {
    return FALSE;
  }
  if (empty($branch)) {
    if (array_key_exists('id', $issue_info)) {
      return drush_set_error('DRUSH_IQ_NO_BRANCH', dt('No working branches available for issue !n.', array('!n' => $issue_info['id'])));
    }
    else {
      return drush_set_error('DRUSH_IQ_NO_BRANCH', dt('No working branches created by Drush iq commands available.'));
    }
  }
  $result = drush_shell_cd_and_exec($dir, "git branch -D %s", $branch);
  if ($result) {
    drush_log(dt('Deleted branch !branch', array('!branch' => $branch)), 'ok');
  }
  return $branch;
}

/**
 * iq-info command callback
 */
function drush_iq_info($number = NULL) {
  if (isset($number)) {
    $issue_info = drush_iq_get_info($number);
  }
  else {
    $dir = _drush_iq_git_root_dir_from_cwd();
    $branch = _drush_iq_get_branch_at_dir($dir);
    $issue_info = drush_iq_get_info($branch);
  }
  if (empty($issue_info)) {
    return drush_set_error('DRUSH_PM_ISSUE_FAILED', dt('Could not find issue !number', array('!number' => $number)));
  }
  $issue_info = _drush_iq_select_patch($issue_info);
  $issue_info['authorName'] = _drush_iq_get_author_name($issue_info);
  $issue_info['projectTitle'] = _drush_iq_get_project_title($issue_info);

  return $issue_info;
}

/**
 * iq-create-commit-comment command callback
 */
function drush_iq_create_commit_comment($number = NULL) {
  if (isset($number)) {
    $issue_info = drush_iq_get_info($number);
  }
  else {
    $dir = _drush_iq_git_root_dir_from_cwd();
    $branch = _drush_iq_get_branch_at_dir($dir);
    $issue_info = drush_iq_get_info($branch);
  }
  if ($issue_info === FALSE) {
    return FALSE;
  }

  $result = _drush_iq_create_commit_comment($issue_info, drush_get_option('committer', FALSE));
  return $result;
}

/**
 * Find the project directory from the given issue number,
 * or from the cwd if no issue number is given.
 */
function _drush_iq_find_project_dir($number = NULL) {
  $issue_info = array();
  if (isset($number)) {
    $issue_info = drush_iq_get_info($number);
    if (!$issue_info) {
      return FALSE;
    }
    $dir = _drush_iq_project_dir($issue_info);
    if (!$dir) {
      return drush_set_error('DRUSH_IQ_NO_PROJECT', dt('Could not find the project directory for !n', array('!n' => $number)));
    }
  }
  else {
    $dir = _drush_iq_git_root_dir_from_cwd();
    $issue_info['project-dir'] = $dir;
  }
  return $issue_info;
}

/**
 * Abandon the current branch
 */
function drush_iq_reset($number = NULL) {
  $cwd = getcwd();
  $issue_info = _drush_iq_find_project_dir($number);
  if ($issue_info == FALSE) {
    return FALSE;
  }
  $dir = _drush_iq_project_dir($issue_info);
  $result = drush_shell_cd_and_exec($dir, "git status -s");
  if (!$result) {
    return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("`git status -s` failed."));
  }
  $status_output = drush_shell_exec_output();
  $unstaged_changes = (!empty($status_output));
  $branch = _drush_iq_get_branch_at_dir($dir);
  $branch_merges_with = _drush_iq_get_branch_merges_with($dir);
  if ($branch_merges_with === FALSE) {
    return FALSE;
  }
  if (empty($branch_merges_with)) {
    $branch_merges_with = 'master';
  }
  $delete = drush_get_option('hard', FALSE);
  $issue_info['branch'] = $branch;
  $issue_info['mergeBranch'] = $branch_merges_with;

  $reset_message = dt("Would you like to reset to the branch !merges_with? ", array('!merges_with' => $branch_merges_with, '!branch' => $branch));

  if (($branch == '(no branch)') || ($branch_merges_with == $branch)) {
    $reset_message = "";
    if (!$unstaged_changes) {
      return drush_set_error('DRUSH_IQ_NO_UNSTAGED_CHANGES', dt("No unstaged changes, and not on a branch; Drush cannot reset. If you have commits that you would like to get rid of, try:\n  git reset HEAD~1\nReplace '1' with the number of commits you have made. Use 'git log' and 'git status' for information."));
    }
    if (!$delete) {
      return drush_set_error('DRUSH_WILL_NOT_DELETE', dt("Not on a branch; Drush will not delete your unstaged changes unless you use the --hard flag."));
    }
    $action_message = dt("Your unstaged changes will be permanently deleted.");
  }
  elseif ($unstaged_changes) {
    $action_message = $delete ? dt("Your working branch !branch will be deleted, along with all unstaged changes.", array('!merges_with' => $branch_merges_with, '!branch' => $branch)) : dt("Your work will be preserved; you can return to it by typing:\n    git checkout !branch\nYour unstaged changes will be re-applied on top of the branch !merges_with", array('!merges_with' => $branch_merges_with, '!branch' => $branch));
  }
  else {
    $action_message = $delete ? dt("Your working branch !branch will be deleted.", array('!merges_with' => $branch_merges_with, '!branch' => $branch)) : dt("Your work will be preserved; you can return to it by typing:\n    git checkout !branch", array('!merges_with' => $branch_merges_with, '!branch' => $branch));
  }
  drush_print($reset_message . $action_message);
  $confirm = drush_confirm(dt("Is it okay to continue?"));
  if (!$confirm) {
    return drush_user_abort();
  }

  if ($delete) {
    $result = drush_shell_exec_interactive("git reset --hard HEAD");
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Failed to delete unstaged changes."));
    }
  }
  if (($branch_merges_with != $branch) && ($branch != "(no branch)")) {
    drush_op('chdir', $dir);
    $result = drush_shell_exec_interactive("git checkout %s", $branch_merges_with);
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not switch back to branch !b.", array('!b' => $branch_merges_with)));
    }
    if ($delete && ($branch != '(no branch)')) {
      $result = drush_shell_exec_interactive("git branch -D %s", $branch);
      if (!$result) {
        return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not delete branch !b.", array('!b' => $branch)));
      }
    }
  }
  drush_op('chdir', $cwd);
  return $issue_info;
}

/**
 * Check to see if any of the modified files have syntax errors.
 */
function drush_iq_lint() {
  $cwd = getcwd();
  $result = drush_shell_exec('git rev-parse --git-dir 2> /dev/null');
  if (!$result) {
    return drush_set_error('DRUSH_IQ_LINT_NO_GIT', dt("Drush iq-lint only works for files under git revision control"));
  }
  $git_rev_output = drush_shell_exec_output();
  $dir = dirname($git_rev_output[0]);
  drush_op('chdir', $dir);
  $changed_files = _drush_iq_unstaged_changes($dir);
  if (empty($changed_files)) {
    $changed_files = _drush_iq_modified_files_on_branch($dir);
  }
  list($ok, $errors, $not_scanned) = _drush_iq_lint($changed_files);
  drush_op('chdir', $cwd);
  if (!empty($errors)) {
    return drush_set_error('DRUSH_IQ_LINT_FAILED', dt("%count files failed lint check.", array("%count" => count($errors))));
  }
  if (!empty($ok)) {
    drush_log(dt("%count files are ok.", array("%count" => count($ok))), 'ok');
    return TRUE;
  }
  if (!empty($not_scanned)) {
    drush_log(dt("None of the %count changed files are php files.", array("%count" => count($not_scanned))), 'ok');
    return TRUE;
  }
  drush_log(dt("No changed files", 'ok'));
  return TRUE;
}

function _drush_iq_unstaged_changes($dir) {
  $result = drush_shell_cd_and_exec($dir, "git ls-files -m");
  if (!$result) {
    return FALSE;
  }
  return drush_shell_exec_output();
}

function _drush_iq_modified_files_on_branch($dir) {
  $issue_branch = _drush_iq_get_branch_at_dir($dir);
  $branch_merges_with = _drush_iq_get_branch_merges_with($dir);
  $result = drush_shell_cd_and_exec($dir, "git diff --name-only %s..%s", $branch_merges_with, $issue_branch);
  if (!$result) {
    return FALSE;
  }
  return drush_shell_exec_output();
}

function _drush_iq_lint($changed_files) {
  $ok = array();
  $errors = array();
  $not_scanned = array();
  foreach ($changed_files as $changed_file) {
    if ( (substr($changed_file, -4) == '.php') ||
         (substr($changed_file, -4) == '.inc') ||
         (substr($changed_file, -7) == '.module')
      ) {
      $result = drush_shell_exec("php -l %s", $changed_file);
      if (!$result) {
        return drush_set_error('DRUSH_IQ_LINT_FAILURE', dt("Failed to run php lint on !f.", array('!f' => array($changed_file))));
      }
      drush_print(implode("\n", drush_shell_exec_output()));
      if (!$result) {
        $errors[] = $changed_file;
      }
      else {
        $ok[] = $changed_file;
      }
    }
    else {
      $not_scanned[] = $changed_file;
    }
  }
  return array($ok, $errors, $not_scanned);
}

/**
 * Merge the current branch in with its upstream branch.
 */
function drush_iq_merge($number = NULL) {
  $issue_info = array();
  $cwd = getcwd();
  $issue_info = _drush_iq_find_project_dir($number);
  if ($issue_info == FALSE) {
    return FALSE;
  }
  $dir = _drush_iq_project_dir($issue_info);
  $result = drush_shell_cd_and_exec($dir, "git status -s");
  if (!$result) {
    return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("`git status -s` failed."));
  }
  $status_output = drush_shell_exec_output();
  $unstaged_changes = (!empty($status_output));
  $branch = _drush_iq_get_branch_at_dir($dir);
  if (empty($issue_info)) {
    $issue_info = drush_iq_get_info($branch);
  }
  $branch_merges_with = _drush_iq_get_branch_merges_with($dir);
  if ($branch_merges_with === FALSE) {
    return FALSE;
  }
  if (empty($branch_merges_with)) {
    $branch_merges_with = 'master';
  }
  $issue_info['branch'] = $branch;
  $issue_info['mergeBranch'] = $branch_merges_with;

  if (($branch == '(no branch)') || ($branch_merges_with == $branch)) {
    return drush_set_error('DRUSH_IQ_CANNOT_MERGE', dt("Not on a branch; cannot merge."));
  }
  drush_op('chdir', $dir);
  $changed_files = _drush_iq_unstaged_changes($dir);
  if (empty($changed_files)) {
    $changed_files = _drush_iq_modified_files_on_branch($dir);
  }
  list($ok, $errors, $not_scanned) = _drush_iq_lint($changed_files);
  if (!empty($errors)) {
    return drush_set_error('DRUSH_IQ_LINT_FAILED', dt("%count files did not pass lint check; refusing to mereg.", array('%count' => count($errors))));
  }
  if ($unstaged_changes) {
    if (!drush_get_option('no-commit', FALSE)) {
      $commit_comment = _drush_iq_create_commit_comment($issue_info, drush_get_option('committer', TRUE), drush_get_option('message', FALSE));
      // TODO: can we easily ignore *.patch, etc., or easily add only modified files?
      $result = drush_shell_exec("git add -u .");
      if (!$result) {
        return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("`git add` failed."));
      }
      $result = drush_shell_exec("git commit -m %s", $commit_comment);
      if (!$result) {
        return drush_set_error('DRUSH_IQ_DIFF_CANNOT_COMMIT', dt("Commit failed."));
      }
    }
    else {
      return drush_set_error('DRUSH_IQ_CANNOT_MERGE', dt("Please commit your unstaged changes before merging, or run command again without the --no-commit option."));
    }
  }
  $confirm = drush_confirm(dt("Would you like to merge the branch !branch with its upstream branch !merges_with? ", array('!merges_with' => $branch_merges_with, '!branch' => $branch)));
  if (!$confirm) {
    return drush_user_abort();
  }
  $squash = drush_get_option('squash', FALSE) ? '--fsquash' : '';

  $result = drush_shell_exec_interactive("git checkout %s", $branch_merges_with);
  if (!$result) {
    return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not switch back to branch !b.", array('!b' => $branch_merges_with)));
  }
  $result = drush_shell_exec_interactive("git merge %s %s", $squash, $branch);
  if (!$result) {
    return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not merge branch !b with !m.", array('!b' => $branch, '!m' => $branch_merges_with)));
  }
  $result = drush_shell_exec_interactive("git branch -D %s", $branch);
  if (!$result) {
    return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not delete branch !b.", array('!b' => $branch)));
  }
  drush_op('chdir', $cwd);
  return $issue_info;
}

function _drush_iq_create_branch(&$issue_info, $ignore = array()) {
  $project_dir = _drush_iq_project_dir($issue_info);
  $branch_merges_with = _drush_iq_get_branch_merges_with($project_dir);
  if (!$branch_merges_with) {
    $branch_merges_with = _drush_iq_get_branch_at_dir($project_dir);
    if (empty($branch_merges_with)) {
      $result = drush_shell_cd_and_exec($project_dir, "git init");
      if (!$result) {
        return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could create a new git repository."));
      }
      $result = drush_shell_cd_and_exec($project_dir, "git add -u .");
      if (!$result) {
        return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not add files to the new git repository we created."));
      }
      foreach ($ignore as $filename) {
        if (dirname($filename) == $project_dir) {
          $result = drush_shell_cd_and_exec($project_dir, "git rm --cached -- %s", basename($filename));
        }
      }
      $result = drush_shell_cd_and_exec($project_dir, "git commit -m 'Git repository created by Drush iq commands.'");
      if (!$result) {
        return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not commit files back to the repository we created."));
      }
    }
  }
  else {
    $branch_merges_with = 'origin/' . $branch_merges_with;
  }
  $set_branch_merge_with = FALSE;
  if ($branch_merges_with == "(no branch)") {
    $branch_merges_with = drush_get_option('base', FALSE);
    if (!$branch_merges_with) {
      return drush_set_error('DRUSH_IQ_BRANCH_DETACHED_HEAD', dt("Could not create branch !branch from a detached HEAD state.  It is recommended that you work off of a development branch, such as 7.x-1.x, instead.  If you would like to work from a specific tag, first run `git checkout -b base`, or run this command again with the --base=base option to create a base branch to work from.", array('!branch' => $branchlabel)));
    }
    $result = drush_shell_exec_interactive("git checkout -b %s", $branch_merges_with);
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not switch to branch !b.", array('!b' => $branch_merges_with)));
    }
    $set_branch_merge_with = TRUE;
  }
  $issue_info['mergeBranch'] = $branch_merges_with;
  drush_log(dt("Starting at branch !branchlabel", array('!branchlabel' => $branch_merges_with)), 'notice');

  // Make a branch via 'git checkout -b [description]-[issue]'
  $description = $issue_info['title'];
  $comment_number = array_key_exists('patchIndex', $issue_info) ? $issue_info['patchIndex'] : (1 + count($issue_info['comments']));
  $branchlabel = 'drush-iq-' . _drush_iq_make_description($description) . '-' . $issue_info['id'] . '-' . $comment_number;
  $issue_info['branch'] = $branchlabel;
  drush_log(dt("Switching to branch !branchlabel", array('!branchlabel' => $branchlabel)), 'ok');
  $result = drush_shell_exec_interactive("git checkout -b %s %s", $branchlabel, $branch_merges_with);
  if (!$result) {
    return drush_set_error('DRUSH_IQ_BRANCH_CREATE_ERROR', dt("Could not create branch !branch", array('!branch' => $branchlabel)));
  }
  if ($set_branch_merge_with) {
    $result = drush_shell_exec_interactive("git config branch.'%s'.merge %s", $branchlabel, $branch_merges_with);
  }
  return $branchlabel;
}

/**
 * iq-apply-patch command callback
 *
 * Given an issue number, find the most recent patch file
 * attached to it
 */
function drush_iq_apply_patch($number = NULL) {
  if (!isset($number)) {
    $issue_info = drush_iq_get_info_from_browser_window();
  }
  else {
    $issue_info = drush_iq_get_info($number);
  }
  $dry_run = drush_get_option('dry-run', FALSE);
  $result = FALSE;
  if (!$issue_info) {
    return FALSE;
  }
  $project_dir = _drush_iq_project_dir($issue_info);
  if (!$project_dir) {
    return drush_set_error('DRUSH_IQ_APPLY_PATCH_NO_PROJECT', dt('You are not working in a site, and the current directory is not named after the project.  Please cd to the project !project and try again.', array('!project' => $issue_info['project'])));
  }
  // Check to see if there is a local git repository for just this project -- that
  // is, is there a .git directory at the project root (as opposed to somewhere
  // higher up, like at the Drupal root)?
  $has_local_git_repository = is_dir($project_dir . '/.git');
  // If the user did not specify --no-git, then use git iff there is
  // a local git repository.  (Beware the double-negative below).
  $do_git_operations = !drush_get_option('no-git', !$has_local_git_repository);
  $issue_info = _drush_iq_select_patch($issue_info, drush_get_option('select', FALSE));
  if (!$issue_info) {
    return FALSE;
  }
  if (!array_key_exists('patchId', $issue_info)) {
    return drush_set_error('DRUSH_NO_PATCHES', dt("Could not find a suitable patch in !issue", array('!issue' => _drush_iq_create_commit_comment($issue_info))));
  }
  $patch_name = $issue_info['patchName'];
  $patch_id = $issue_info['patchId'];
  $patch = $issue_info['patchUrl'];
  drush_log(dt("Drush iq-apply-patch !id: downloading patchfile !patch for project !project", array('!patch' => $patch_name, '!id' => $patch_id, '!project' => $issue_info['project'])), 'ok');
  // n.b. the file returned by drush_download_file is either in the cache,
  // or is registered for deletion
  $filename = drush_download_file($patch);
  if (!file_exists($filename)) {
    return drush_set_error('DRUSH_PATCH_NOT_DOWNLOADED', dt("Patch could not be downloaded."));
  }
  $filename = realpath($filename);
  $handle = @fopen($filename, "r");
  if (!$handle) {
    return drush_set_error('DRUSH_PATCH_NOT_READABLE', dt("Patch could not be read."));
  }

  $keep_patch = FALSE;
  if (drush_get_option('keep-patch', FALSE)) {
    $keep_patch = dirname($filename) . '/iq-' . basename($filename);
    copy($filename, $keep_patch);
  }

  $cwd = getcwd();
  drush_op('chdir', $project_dir);

  $try_patch_tool = TRUE;
  if ($do_git_operations && !$dry_run) {
    $result = _drush_iq_create_branch($issue_info, array($filename, $keep_patch));
    if ($result === FALSE) {
      return FALSE;
    }
  }

  $files_to_add = array();
  if ($do_git_operations) {
    // first let us test if we need to add files to the commit later
    $result = drush_shell_exec("git apply -v --summary %s", $filename);
    if (!$result) {
      return drush_set_error('DRUSH_IQ_APPLY_FAILURE', dt("Could not apply the patch !f.", array('!f' => $filename)));
    }
    $git_apply_summary_output = drush_shell_exec_output();
    foreach ($git_apply_summary_output as $line) {
      $words = explode(" ", $line);
      if ($words[1] == 'create' || $words[1] == 'rename') {
        $prefix = substr($words[2], 0, strpos($words[2], '{'));
        $files_to_add[] = $prefix . trim($words[4], '}');
      }
    }
    $other_options = $dry_run ? '--summary' : '';
    $result = drush_shell_exec_interactive("git apply -v $other_options %s", $filename);
    if ($result === FALSE) {
      drush_log(dt("git apply failed; falling back to 'patch' tool"), 'warning');
    }
    else {
      $try_patch_tool = FALSE;
    }
  }

  if ($try_patch_tool) {
    // Try -Np1 first, then -Np0, unless the user selected --no-prefix,
    // in which case we'll try -Np0 followed by -Np1 if that does not work.
    $strip_count = 1;
    if (drush_get_option('no-prefix', FALSE)) {
      $strip_count = 0;
    }

    $result = drush_shell_exec('patch -Np%d --dry-run --batch -d %s -i %s', $strip_count, $project_dir, $filename);
    if (!$result) {
      $strip_count = !$strip_count;
      $result = drush_shell_exec('patch -Np%d --dry-run --batch -d %s -i %s', $strip_count, $project_dir, $filename);
      if (!$result) {
        return drush_set_error('DRUSH_IQ_PATCH_DID_NOT_APPLY', dt("Could not apply the patch with either -Np0 or -Np1; perhaps the patch was rolled for a different version of the project."));
      }
    }
    $dry_run_output = drush_shell_exec_output();
    // Before we apply the patch, run through the dry-run
    // output and make a note of new files that were added
    // by this patch.
    foreach ($dry_run_output as $line) {
      if (substr($line, 0, 14) == 'patching file ') {
        $file = substr($line, 14);
        if (!file_exists($project_dir . '/' . $file)) {
          $files_to_add[] = $file;
        }
      }
    }
    if ($dry_run) {
      drush_print(implode("\n", $dry_run_output));
    }
    else {
      $result = drush_shell_exec_interactive('patch -Np%d --batch --reject-file=- --prefix=/tmp/ -d %s -i %s', $strip_count, $project_dir, $filename);
    }
  }
  if ($do_git_operations && !$dry_run && !drush_get_option('no-commit', FALSE)) {
    $commit_comment = _drush_iq_create_commit_comment($issue_info, drush_get_option('committer', FALSE), drush_get_option('message', FALSE));
    // Add the modified files
    $result = drush_shell_exec("git add -u .");
    // Add the new files
    foreach ($files_to_add as $file) {
      if ($result) {
        $result = drush_shell_exec("git add %s", $file);
      }
    }
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not add patched files to the git repository."));
    }
    // Unstage the patch file and its copy (if we
    // are keeping it) if the patch file was downloaded
    // in the project directory.
    if (dirname($filename) == $project_dir) {
      $result = drush_shell_exec("git rm --cached -- %s", basename($filename));
      if ($keep_patch) {
        $result = drush_shell_exec("git rm --cached -- %s", basename($keep_patch));
      }
    }
    $authorCredit = '';
    if (array_key_exists('patchAuthorId', $issue_info)) {
      $authorCredit = drush_iq_get_author_credit($issue_info['patchAuthorId']);
    }
    if (!empty($authorCredit)) {
      $result = drush_shell_exec("git commit --author=%s -m %s", $authorCredit, $commit_comment);
    }
    else {
      $result = drush_shell_exec("git commit -m %s", $commit_comment);
    }
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not commit patched files to the git repository."));
    }
  }
  drush_op('chdir', $cwd);
  $changed_files = _drush_iq_unstaged_changes($project_dir);
  if (empty($changed_files)) {
    $changed_files = _drush_iq_modified_files_on_branch($project_dir);
  }
  list($ok, $errors, $not_scanned) = _drush_iq_lint($changed_files);
  if (!empty($errors)) {
    drush_log(dt("%count files did not pass lint check; it would be advisable to use 'drush iq-reset' to discard this patch.", array('%count' => count($errors))), 'warning');
  }
  return $issue_info;
}

function _drush_iq_get_author_name($issue_info) {
  $owner_info = drush_iq_fetch_user_info($issue_info['authorId']);
  return $owner_info['name'];
}

function drush_iq_get_author_credit($uid) {
  $user_info = drush_iq_fetch_user_info($uid);
  $name = $user_info['name'];
  return sprintf("%s <%s@%s.no-reply.drupal.org>", $name, $name, $uid);
}

function _drush_iq_get_project_name($issue_info) {
  $project_info = drush_iq_fetch_project_info($issue_info['field_project']['id']);
  return $project_info['field_project_machine_name'];
}

function _drush_iq_get_project_title($issue_info) {
  $project_info = drush_iq_fetch_project_info($issue_info['field_project']['id']);
  return $project_info['title'];
}

function _drush_iq_select_patch($issue_info, $prompt = FALSE) {
  $patch_list = _drush_iq_get_patch_list($issue_info, $prompt);
  $patches = _drush_iq_collate_patches($issue_info, $patch_list);

  if (!empty($patches)) {
    // If --select, then prompt the user
    if ($prompt) {
      $choices = array();

      foreach ($patch_list as $patch => $info) {
        $index = $info['index'] . $info['suffix'];
        $choices[$patch] = $patch;
        // TODO: when index is the comment number:
        // $choices[$patch] = array("$index", ":", $patch);
      }
      $patch = drush_choice($choices, dt("Select a patch to apply:"));
      if ($patch === FALSE) {
        return drush_user_abort();
      }
    }
    // If the issue specification included a comment number, e.g. #1078108-1, then select the patch attached to the specified comment
    elseif (array_key_exists('comment-number', $issue_info)) {
      $index = $issue_info['comment-number'];
      if (!array_key_exists($index, $patches)) {
        return drush_set_error('DRUSH_IQ_NO_PATCH', dt("Error: comment #!index does not exist or does not have a patch.", array('!index' => $index)));
      }
      $patch = $patches[$index];
    }
    else {
      // TODO: See https://drupal.org/node/1078108#comment-5659936 and
      // https://drupal.org/node/1078108#comment-5663932 for additional
      // heuristics we might apply here.
      $patch = array_pop($patches);
    }

    $patch_name = basename($patch);
    $patch_id = $issue_info['nid'];
    if (array_key_exists($patch, $patch_list)) {
      $issue_info['patchUrl'] = $patch;
      $issue_info['patchIndex'] = $patch_list[$patch]['index'] . $patch_list[$patch]['suffix'];
      $patch_id .= '-' . $issue_info['patchIndex'];
      $issue_info['patchAuthorId'] = $patch_list[$patch]['ownerId'];
      /*
      // should not be necessary any longer
      if ($patch_list[$patch]['id'] == 0) {
        $issue_info['patchAuthorId'] = $issue_info['author']['id'];
      }
      */
    }
    $issue_info['patchName'] = $patch_name;
    $issue_info['patchId'] = $patch_id;
  }
  return $issue_info;
}

/**
 * This lookup table is used to map items from
 * the issue queue json to the human-readable
 * labels used in the output of iq-info.
 */
function _drush_iq_label_map() {
  return  array(
    'Title' => 'title',
    'ID' => 'id',
    'URL' => 'url',
    'Project' => 'project',
    'Project URL' => 'projectUrl',
    'Version' => 'version',
    'Component' => 'component',
    'Category' => 'category',
    'Priority' => 'priority',
    'Assigned' => 'assignedName',
    'Status' => 'issueStatus',
    'Patch URL' => 'patchUrl',
    'Patch Index' => 'patchIndex',
    'Patch ID' => 'patchId',
  );
}

/**
 * Create a commit comment
 *
 * @param $issue_info array describing an issue; @see drush_iq_get_info()
 *
 * @returns string "#id by contributor1, contributor2: issue title"
 */
function _drush_iq_create_commit_comment($issue_info, $committer = FALSE, $message = FALSE) {
  $contributors = array();

  // If this function is being called because a patch is being
  // created right now, then add the committer to the head of the
  // credits list.
  if ($committer !== FALSE) {
    // If 'TRUE' is passed for the committer, then look up
    // the current user name from git config --list
    if ($committer === TRUE) {
      $result = drush_shell_exec("git config --list | grep '^user.name=' | sed -e 's|[^=]*=||'");
      if ($result) {
        $output = drush_shell_exec_output();
        $committer = array_pop($output);
      }
    }

    if (!empty($committer)) {
      $contributors[$committer] = $committer;
    }
  }
  // Gather up commit credits, listing most recent contributors first.
  // Everyone who added an attachment gets credit.
  if (!empty($issue_info)) {
    $patch_list = _drush_iq_get_patch_list($issue_info);
    foreach ($patch_list as $url => $patch_info) {
      $ownerId = $patch_info['ownerId'];
      $owner_info = drush_iq_fetch_user_info($ownerId);
      $contributor = $owner_info['name'];
      if (!in_array($contributor, $contributors)) {
        $contributors[$contributor] = $contributor;
      }
    }
  }
  $credits = "";
  $credits_prequel = "";
  if (!empty($contributors)) {
    $credits_prequel = " by";
    $credits = " " . implode(', ', $contributors);
  }

  $prequel = "";
  if ($issue_info) {
    $issue_number = $issue_info['id'];
    $issue_title = $issue_info['title'];
    if (!$message) {
      $message = $issue_title;
    }
    $prequel = "Issue #$issue_number$credits_prequel";
  }
  if (!$message) {
    $message = dt("Generated with Drush iq");
  }
  return "$prequel$credits: $message";
}

function drush_iq_get_info_from_browser_window() {
  // Collect an array of potential issue numbers
  $candidate_issues = array();
  $output = array();
  // Linux: use `wmctrl -l` to get a list of all window titles
  $result = drush_shell_exec('wmctrl -l');
  if ($result) {
    $output = drush_shell_exec_output();
  }
  else {
    // Windows: use `tasklist.exe /v`
    $result = drush_shell_exec('tasklist.exe /v');
    if ($result) {
      $output = drush_shell_exec_output();
    }
  }
  // Process Linux or Windows output the same way.
  foreach ($output as $line) {
    if (preg_match('/\[#([0-9]+)\] \| Drupal/', $line, $matches)) {
      $candidate_issues[] = $matches[1];
    }
  }
  // Macintosh: see https://drupal.org/node/1864142

  // Next, iterate through the candidate issues and
  // get info on each one.
  $issue_choices = array();
  $user_selections = array();
  foreach ($candidate_issues as $number) {
    $issue_info = drush_iq_get_info($number);
    // TODO: Screen issues (e.g. check to see if
    // the issue project == the project at cwd,
    // filter out issues for projects that are not enabled in the current site, etc.)
    if ($issue_info) {
      $issue_choices[$number] = $issue_info;
      $user_selections[$number] = '#' . $issue_info['id'] . ' - ' . $issue_info['project'] . ': ' . $issue_info['title'];
    }
  }
  if (empty($issue_choices)) {
    return drush_set_error('DRUSH_IQ_NO_ISSUES_FOUND_IN_BROWSER', dt("No issues found in any open browser window."));
  }
  $selection = drush_choice($user_selections, dt("Select an issue:"));
  if (!$selection) {
    return drush_user_abort();
  }
  return $issue_choices[$selection];
}

/**
 * Get information about an issue
 *
 * @param $number integer containing the issue number, or string beginning with a "#" and the issue number
 *
 * @returns array
 *  - title             Title of the issue
 *  - id                Issue number
 *  - url               URL to issue page on drupal.org
 *  - projectTitle      Title of the project issue belongs to (e.g. Drush)
 *  - projectName       Name of the project (e.g. drush) - https://drupal.org/project/{projectName}
 *  - projectId
 *  - projectUrl
 *  - version           Project version
 *  - versionId
 *  - authorName       Name of the user who submitted the issue
 *  - authorId
 *  - authorUrl
 *  - assignedName     Name of the user the issue is assigned to
 *  - assignedId
 *  - assignedUrl
 *  - component         Code, documentation, etc.
 *  - category          Bug, feature request, etc.
 *  - priority          minor, normal, major, critical
 *  - priorityId
 *  - issueStatus       active, needs work, etc.
 *  - statusId
 *  - created
 *  - changed
 *  - comments          A list (array with numeric keys) of URLs
 *  - attachments       A list (array with numeric keys) of attachments
 *      Array
 *        - contributorId uid of user submitting the patch
 *        - urls        A list of strings pointing to the attachments
 *  - contributors      An associative array keyed by uid of contributors
 *      Array
 *        - name        Name of contributor
 *        - uid         uid of contributor (same as key for this item)
 *        - profile     url to user profile on drupal.org
 */
function drush_iq_get_info($issue_spec) {
  $issue_info_id = drush_iq_issue_number($issue_spec);
  if (empty($issue_info_id)) {
    return drush_set_error('DRUSH_ISSUE_NOT_FOUND', dt("Could not find the issue !issue", array('!issue' => $issue_spec)));
  }
  $issue_info = drush_iq_fetch_issue_info($issue_info_id);
  return $issue_info;
}

/**
 *  Given an issue specification, determine
 *  the issue number that the user requested.
 */
function drush_iq_issue_number($issue_spec) {
  $issue_info_id = array();
  $number = FALSE;
  $issue_site_domain = drush_get_option('issue-site', 'drupal.org');
  $comment_number = FALSE;
  // #1234
  if (substr($issue_spec, 0, 1) == '#') {
    $issue_info_id['id'] = substr($issue_spec, 1);
  }
  // https://drupal.org/node/1234
  elseif (preg_match("#^https*://([^/]*)/node/([0-9]*)/*#", $issue_spec, $matches, PREG_OFFSET_CAPTURE)) {
    $issue_site_domain = $matches[1][0];
    $issue_info_id['id'] = $matches[2][0];
  }
  // 1234
  elseif (is_numeric($issue_spec)) {
    $issue_info_id['id'] = $issue_spec;
  }
  // description-of-issue-1234 or description-of-issue-1234-8 (description-issue or description-issue-comment)
  elseif (strpos($issue_spec, ' ') === FALSE) {
    if (preg_match('/-*([0-9]+)(-*(#*[0-9]*[a-z]*))$/', $issue_spec, $matches)) {
      $issue_info_id['id'] = $matches[1];
      if (!empty($matches[3])) {
        $issue_info_id['comment-number'] = $matches[3];
      }
    }
  }
  return $issue_info_id;
}

function drush_iq_fetch_issue_info($id) {
  return drush_iq_fetch_via_id_and_type($id, 'node', DRUSH_IQ_CACHE_NEVER);
}

function drush_iq_fetch_project_info($nid) {
  return drush_iq_fetch_via_id_and_type($nid, 'node', DRUSH_IQ_CACHE_FOREVER);
}

function drush_iq_fetch_file_info($fid) {
  return drush_iq_fetch_via_id_and_type($fid, 'file', DRUSH_IQ_CACHE_FOREVER);
}

function drush_iq_fetch_user_info($uid) {
  return drush_iq_fetch_via_id_and_type($uid, 'user', DRUSH_IQ_CACHE_FOREVER);
}

function drush_iq_fetch_project_via_name($name) {
  if (is_array($name)) {
    $name = $name['name'];
  }
  $issue_site = drush_get_option('issue-site', 'drupal.org');
  if (!strstr($issue_site, "://")) {
    $issue_site = "https://" . $issue_site;
  }
  $url = "$issue_site/api-d7/node.json?field_project_machine_name=$name";

  // In a search by name, we can get only one result (or none), so simplify the result.
  $values = drush_iq_fetch_via_rest_url($url, $name, 'node', DRUSH_IQ_CACHE_FOREVER);
  $list_of_projects = $values['list'];
  // Return an empty array if $list_of_projects is empty; otherwise, return
  // the first element.
  $result = (array)array_pop($list_of_projects);
  return drush_iq_alter_fetched_data($result, 'node');
}

/**
 *  Given a node id, fetch and decode the issue info json from drupal.org.
 */
function drush_iq_fetch_via_id_and_type($id, $issue_data_type = 'node', $cache_strategy = DRUSH_IQ_CACHE_NEVER) {
  if (is_array($id)) {
    $id = $id['id'];
  }
  $issue_site = drush_get_option('issue-site', 'drupal.org');
  if (!strstr($issue_site, "://")) {
    $issue_site = "https://" . $issue_site;
  }
  $url = "$issue_site/api-d7/$issue_data_type/$id.json";

  $result = drush_iq_fetch_via_rest_url($url, $id, $issue_data_type, $cache_strategy);
  return drush_iq_alter_fetched_data($result, $issue_data_type);
}

function drush_iq_fetch_via_rest_url($url, $id, $issue_data_type, $cache_strategy) {
  $result = array();
  $cache_dir = drush_iq_cache_dir();
  $cache_file = $cache_dir . '/iq-' . $issue_data_type . '-' . $id . '.json';

  // Check to see if we should refresh the data
  $should_refresh = !file_exists($cache_file) || ($cache_strategy == DRUSH_IQ_CACHE_NEVER);

  // Check to see if data newer than the
  // cache file exists.
  if (file_exists($cache_file) && ($cache_strategy == DRUSH_IQ_CACHE_CHECK_MOD_DATE)) {
    // TODO: we need a function to get the mod
    // date of the resource at $url.  If it is
    // newer than the cache file, then force refresh.
    $should_refresh = TRUE;
  }

  // Make note of whether cache data exists for the resource
  $filename = '';

  // If we need to refresh the cache, then fetch the data from the specified URL again
  if ($should_refresh) {
    drush_log(dt("Fetching !type json from !url", array('!type' => $issue_data_type, '!url' => $url)), 'notice');
    $filename = _drush_download_file($url, $cache_file, TRUE);
    // TODO: if filename != cache_file, copy
  }

  // If we could not (or did not) download a fresh
  // copy of the info
  if (empty($filename) && file_exists($cache_file)) {
    drush_log(dt("Cache data exists for !type at !url", array('!type' => $issue_data_type, '!url' => $url)), 'notice');
    $filename = $cache_file;
  }

  // If we have some data, then load it
  if (!empty($filename)) {
    $data = file_get_contents($filename);
  }
  if (!empty($data)) {
    $result = json_decode($data, TRUE);
    //var_export($result);
  }
  return $result;
}

function drush_iq_alter_fetched_data($result, $issue_data_type) {
  if (!empty($result)) {
    $type = $issue_data_type;
    if ($type == 'node') {
      $type = $result['type'];
    }
    $fn = "drush_iq_alter_" . $type . "_data";
    if (function_exists($fn)) {
      $result = $fn($result);
    }
  }
  return $result;
}

function drush_iq_project_issue_conversion_table() {
  return array(
    'priority' => array(
      400 => 'Critical',
      300 => 'Major',
      200 => 'Normal',
      100 => 'Minor',
    ),

    'issueStatus' => array(
      1 => 'active',
      2 => 'fixed',
      3 => 'closed (duplicate)',
      4 => 'postponed',
      5 => "closed (won't fix)",
      6 => 'closed (works as designed)',
      7 => 'closed (fixed)',
      8 => 'needs review',
      13 => 'needs work',
      14 => 'reviewed & tested by the community',
      15 => 'patch (to be ported)',
      16 => 'postponed (maintainer needs more info)',
      18 => 'closed (cannot reproduce)',
    ),

    'category' => array(
      1 => 'Bug report',
      2 => 'Task',
      3 => 'Feature request',
      4 => 'Support request',
    ),
  );
}

function drush_iq_project_issue_possible_values() {
  $conversion_table = drush_iq_project_issue_conversion_table();
  $possible_values = array();
  foreach ($conversion_table as $key => $table) {
    $possible_values[$key] = array_flip($table);
  }
  return $possible_values;
}

function drush_iq_alter_project_issue_data($issue_info) {
  $source_id_table = array(
    'issueStatus' => 'field_issue_status',
    'priority' => 'field_issue_priority',
    'category' => 'field_issue_category',
  );

  $conversion_table = drush_iq_project_issue_conversion_table();

  foreach ($conversion_table as $targetKey => $mapping_data) {
    $sourceKey = $targetKey . 'Id';
    if (array_key_exists($targetKey, $source_id_table)) {
      $sourceKey = $source_id_table[$targetKey];
    }
    if (array_key_exists($sourceKey, $issue_info)) {
      $value = $issue_info[$sourceKey];
      if (array_key_exists($value, $mapping_data)) {
        $issue_info[$targetKey] = $mapping_data[$value];
      }
      else {
        $issue_info[$targetKey] = $value;
      }
    }
  }
  $issue_info['id'] = $issue_info['nid'];
  $issue_info['projectUrl'] = "https://drupal.org/node/" . $issue_info['field_project']['id'];
  $issue_info['authorId'] = $issue_info['author']['id'];
  $issue_info['project'] = _drush_iq_get_project_name($issue_info);
  // $issue_info['assignedUrl'] = "https://drupal.org/user/" . $issue_info['assignedId'];

  return $issue_info;
}

function _drush_iq_get_patch_list($issue_info, $fetchAll = TRUE) {
  $patch_list = array();

  $index = 0;
  $comment_patches = array();
  foreach (array_reverse($issue_info['field_issue_files']) as $file_info) {
    $file_data = drush_iq_fetch_file_info($file_info['file']['id']);
    $url = $file_data['url'];
    if (substr($url, -6) == ".patch" || substr($url, -5) == ".diff") {
      $comment_patches[$index][] = array('id' => $file_data['fid'], 'url' => $url, 'ownerId' => $file_data['owner']['id']);
      if ($fetchAll == FALSE) {
        break;
      }
      $index = $index + 1; // TODO - this should be the comment number, but this info is not available. We therefore use the file number in its place.
    }
  }
  foreach ($comment_patches as $index => $patch_info) {
    // Presently, there will only ever be one patch per index;
    // however, once the index == the comment number, there may
    // be multiple.  We label multiple patches on the same
    // index "10a", "10b", and so on.
    $number = count($comment_patches) - $index;
    $label = '';
    if (count($patch_info) > 1) {
      $label = 'a';
    }
    foreach ($patch_info as $patch) {
      $patch['index'] = $number;
      $patch['suffix'] = $label;
      $url = $patch['url'];
      unset($patch['url']);
      $patch_list[$url] = $patch;
      $label = chr(ord($label) + 1);
    }
  }
  return $patch_list;
}

function _drush_iq_collate_patches($issue_info, $patch_list) {
  $patches = array();

  foreach ($patch_list as $url => $info) {
    $patches[$info['index'] . $info['suffix']] = $url;
    $patches[$info['id'] . $info['suffix']] = $url;
  }

  return $patches;
}

/**
 * Find the project directory associated with the project
 * the specified issue is associated with.
 */
function _drush_iq_project_dir(&$issue_info) {
  $result = FALSE;

  if (array_key_exists('project-dir', $issue_info)) {
    $result = $issue_info['project-dir'];
  }
  elseif(array_key_exists('project', $issue_info)) {
    $project_name = $issue_info['project'];

    // If our cwd is at the root of the project, then prefer that project over
    // one in some other location.
    $dir = _drush_iq_git_root_dir_from_cwd();
    if (basename($dir) == $project_name) {
      $result = $dir;
    }
    // TODO: Find drush extensions such as drush_extras, drush_make, drubuntu, etc.
    elseif ($project_name == 'drush') {
      $result = DRUSH_BASE_PATH;
    }
    else {
      $phase = drush_bootstrap_max();
      drush_log("bootstrapped to phase $phase");
      if ($phase >= DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION) {
        $extension_info = drush_get_extensions();
        // TODO: offer to download the project if it is not found?
        if (array_key_exists($project_name, $extension_info)) {
          $result = drush_get_context('DRUSH_DRUPAL_ROOT', '') . '/' . dirname($extension_info[$project_name]->filename);
        }
      }
      if ($phase >= DRUSH_BOOTSTRAP_DRUPAL_ROOT) {
        $root = drush_get_context('DRUSH_DRUPAL_ROOT', FALSE);
        if ($root) {
          if ($project_name == 'drupal') {
            $result = $root;
          }
          else {
            foreach (array('modules', 'sites/all/modules', 'sites/default/modules') as $loc) {
              $path = $root . '/' . $loc . '/' . $project_name;
              if (is_dir($path)) {
                $result = $path;
              }
            }
          }
        }
      }
    }
    if ($result) {
      $issue_info['project-dir'] = $result;
    }
    else {
      drush_log(dt('Could not find the project directory under the bootstrapped site'));
    }
  }

  return $result;
}

function _drush_iq_get_branch(&$issue_info) {
  $branch = FALSE;
  if (array_key_exists('branch', $issue_info)) {
    $branch = $issue_info['branch'];
  }
  else {
    $project_dir = _drush_iq_project_dir($issue_info);
    if ($project_dir) {
      $branch = _drush_iq_get_branch_at_dir($project_dir);
    }
  }
  $issue_info['branch'] = $branch;
  return $branch;
}

function _drush_iq_get_branch_at_dir($dir) {
  $result = drush_shell_cd_and_exec($dir, "git branch");
  $branch_output = drush_shell_exec_output();

  // Return the last non-empty line
  $branch = FALSE;
  while (($branch === FALSE) && !empty($branch_output)) {
    $line = array_shift($branch_output);
    if (!empty($line) && ($line[0] == '*')) {
      $branch_components = explode(' ', $line, 2);
      $branch = $branch_components[1];
    }
  }
  return $branch;
}

function _drush_iq_get_merge_branch_with_remote($merges_with) {
  $remote = _drush_iq_get_best_remote($merges_with);
  return _drush_iq_merge_branch_with_remote($remote, $merges_with);
}

function _drush_iq_merge_branch_with_remote($remote, $merges_with) {
  if (empty($remote)) {
    return $merges_with;
  }
  else {
    return $remote . '/' . $merges_with;
  }
}

function _drush_iq_get_best_remote($merges_with) {
  $remotes = _drush_iq_get_remotes($merges_with);
  if (empty($remotes)) {
    return "";
  }
  elseif (count($remotes) == 1) {
    return $remotes[0];
  }
  elseif (in_array('origin', $remotes)) {
    return 'origin';
  }
  return $remotes[0];
}

function _drush_iq_get_remotes($merges_with) {
  // Check the output of `git branch -r`; we are looking
  // for output "remote/branchname"
  $result = drush_shell_exec("git branch -r");
  $git_branch_output = drush_shell_exec_output();

  $remotes = array();
  foreach($git_branch_output as $line) {
    $line = trim($line);
    if (preg_match("#([^/]+)/(.*)#", $line, $matches)) {
      if ($matches[2] == $merges_with) {
        $remotes[] = $matches[1];
      }
    }
  }
  return $remotes;
}

function _drush_iq_find_project_via_remote_origin($dir) {
  $result = drush_shell_cd_and_exec($dir, "git remote show origin -n");
  $show_orgin_output = drush_shell_exec_output();

  $project = FALSE;
  foreach ($show_orgin_output as $line) {
    $line = trim($line);
    // Find the FETCH URL and the PUSH URL; if these are set up to
    // point at the same project, then we consider it a match.
    // Otherwise we fail.
    if (preg_match('|URL:[^/]*/([^.]*).git$|', $line, $matches)) {
      if (!$project) {
        $project = $matches[1];
      }
      elseif ($project != $matches[1]) {
        return FALSE;
      }
    }
  }
  return $project;
}

function _drush_iq_get_branch_merges_with($dir, $branch_label = FALSE) {
  $merges_with = "";
  $default_merges_with = FALSE;
  $alternate_remotes = array();

  if ($branch_label === FALSE) {
    $branch_label = _drush_iq_get_branch_at_dir($dir);
  }
  if ($branch_label == "(no branch)") {
    return $merges_with;
  }
  // We expect that the upstream remote has been configured via
  // `git branch --set-upstream drush-iq-x master`
  $result = drush_shell_cd_and_exec($dir, "git config branch.%s.merge", $branch_label);
  $branch_merge_output = drush_shell_exec_output();
  if (!empty($branch_merge_output)) {
    // Convert the branch name into its abbreviated form.
    $result = drush_shell_cd_and_exec($dir, "git rev-parse --abbrev-ref %s", $branch_merge_output[0]);
    $rev_parse_output = drush_shell_exec_output();
    return $rev_parse_output[0];
  }

  // If the upstream remote has not been configured, then we will look
  // at the output of `git remote show origin` and see if we can find
  // a fallback branch to use.
  $result = drush_shell_cd_and_exec($dir, "git remote show origin -n");
  $show_orgin_output = drush_shell_exec_output();

  foreach ($show_orgin_output as $line) {
    $line = trim($line);
    // Find all of the other branches 'master merges with remote master', etc.
    if (preg_match("/([^ ]+).* with remote (.*)/", $line, $matches)) {
      if ($matches[1] == $matches[2]) {
        $alternate_remotes[] = $matches[2];
      }
    }
  }
  // If there is only one remote, that must be the one we merge with!
  if (count($alternate_remotes) == 1) {
    $merges_with = array_shift($alternate_remotes);
  }
  elseif (!empty($alternate_remotes)) {
    // We could figure out which of these branches was the correct
    // one to use by walking the commit log.  See:
    // https://drupal.org/node/1078108#comment-6335376
    $setupstream = "";
    foreach ($alternate_remotes as $alternate) {
      $setupstream .= "\n  " . dt("git branch --set-upstream !branch !merge", array('!branch' => $branch_label, '!merge' => $alternate));
    }
    return drush_set_error('DRUSH_IQ_CANNOT_FIND_MERGE_BRANCH', dt("Could not determine which branch of !alternatives was the correct merge branch.  Use one of the following commands to select the correct one: !setupstream", array('!alternatives' => implode(',', $alternate_remotes), '!setupstream' => $setupstream)));
  }
  return $merges_with;
}

function _drush_iq_make_description($title, $trim_length = 30) {
  $description = preg_replace('/[^a-z._-]/', '', str_replace(' ', '-', strtolower($title)));

  // Clip the description off at the next dash after the 30th position
  if (strlen($description) > $trim_length) {
    $find_dash = strpos($description, '-', $trim_length);
    if ($find_dash !== FALSE) {
      $description = substr($description, 0, $find_dash);
    }
  }

  return $description;
}

function drush_iq_next_comment_number($issue_info) {
  $comment_number = 1;
  if (array_key_exists('comment_count', $issue_info)) {
    // There is a chance this might not be right if there are deleted comments.
    // We need to have the comment number added to the comments.
    $comment_number = $issue_info['comment_count'] + 1;
  }
  return $comment_number;
}

/**
 * Determine the location of the git root directory from the
 * location of the user's current working directory.  If the
 * cwd is somewhere inside the git repo, pop up to the top
 * by using git rev-parse.
 */
function _drush_iq_git_root_dir_from_cwd() {
  $dir = drush_get_context('DRUSH_OLDCWD', drush_cwd());
  exec('git rev-parse --show-toplevel 2> /dev/null', $output);
  if (!empty($output)) {
    $dir = $output[0];
  }
  return $dir;
}

/**
 * Command hook for drush iq-open
 */
function drush_iq_open($filter = NULL) {
  // "simulated" mode's default implementation does not work
  // well here, so we'll turn it off, and handle it manually.
  // The problem is that, by default, in simulated mode,
  // calls to Drush's wrappers for 'exec', 'system', etc.
  // do not execute.  We make use of the results from a number
  // of calls to 'exec' to find the candidate files, so
  // it is easier to turn off simulated mode and continue
  // to use the wrappers. We will also set DRUSH_VERBOSE, as
  // this will cause our shell commands to be echo'ed as
  // they are executed.
  $simulated = drush_get_context('DRUSH_SIMULATE', FALSE);
  drush_set_context('DRUSH_SIMULATE', FALSE);
  if ($simulated) {
    drush_set_context('DRUSH_VERBOSE', TRUE);
  }
  // The first step is to use some helpful functions
  // from drush_iq to find out what our upstream branch
  // is.  If iq-apply-patch created the branch, then
  // $branch_merges_with will contain the branch that
  // the patch was applied to -- e.g. 7.x-1.x.
  // iq-open can also be used to select a file to open
  // from the current unstaged changes.  In this case,
  // $branch_merges_with will come back either empty
  // (no remote), or will be equal to $branch (if
  // our upstream is a remote branch of the same name).
  // In this case, we will use an empty branch spec
  // to just get a list of unstaged modified files.
  $branch_spec = '';
  $dir = _drush_iq_git_root_dir_from_cwd();
  $branch = _drush_iq_get_branch_at_dir($dir);
  $branch_merges_with = _drush_iq_get_branch_merges_with($dir);
  if (!empty($branch_merges_with) && ($branch != $branch_merges_with)) {
    $branch_spec = "${branch_merges_with}..HEAD";
  }
  // If the user has specified a --search (literal)
  // or --grep (regex) search term, then we will add
  // a -S or -G search option onto our git command to
  // filter the results.
  $selection = FALSE;
  $search_spec = '';
  foreach (array('S' => 'search', 'G' => 'grep') as $git_option => $drush_option) {
    $value = drush_get_option($drush_option, FALSE);
    if ($value) {
      $search_spec = "-$git_option" . drush_escapeshellarg($value);
    }
  }
  // git diff --name-only will list just the filenames that
  // were modified.
  $result = drush_shell_exec("git diff --name-only $search_spec %s", $branch_spec);
  $changed_files = drush_shell_exec_output();
  if (empty($changed_files) && empty($search_spec)) {
    drush_log(dt("No modified files."), 'warning');
    return;
  }
  // The user can further filter the selected files
  // by filename.  If you know that a modified file
  // contains "foo" in its name, then `drush iqo foo`
  // will open the file you want without requiring
  // you to type the full path.
  if ($filter) {
    foreach ($changed_files as $key => $file) {
      if (strpos($file, $filter) === FALSE) {
        unset($changed_files[$key]);
      }
    }
  }
  if (empty($changed_files)) {
    drush_log(dt("No matching files."), 'warning');
    return;
  }
  // drush_get_editor will return the editor to
  // use; this is often specified via the EDITOR
  // or VISUAL environment variables.  The default
  // is 'vi'.
  $exec = drush_get_editor();
  // Figure out which file or files to open, or
  // ask the user to pick one.
  if (drush_get_option('all', FALSE)) {
    $selection = implode(' ', $changed_files);
  }
  elseif (count($changed_files) == 1) {
    $selection = array_pop($changed_files);
  }
  else {
    $choice = drush_choice($changed_files, dt("Select a file to open"));
    if ($choice) {
      $selection = $changed_files[$choice];
    }
  }
  // Restore handling of simulated mode.
  drush_set_context('DRUSH_SIMULATE', $simulated);
  if ($selection) {
    drush_shell_exec_interactive($exec, $selection, $selection);
  }
}

function _drush_iq_most_stable($kind1, $kind2) {
  // "stable", "rc", "beta" and "alpha" are self-explanitory
  // some folks use "unstable" before alpha; I don't think this is standard.
  // "none" means there is no release branch yet
  foreach (array('stable', 'rc', 'beta', 'alpha', 'unstable', 'none') as $check) {
    if (($check == $kind1) || ($check == $kind2)) {
      return $check;
    }
  }
  return FALSE;
}

/**
 * Command hook for drush iq-release
 */
function drush_iq_release($comment = '') {
  // Handle "simulated" mode
  $simulated = drush_get_context('DRUSH_SIMULATE', FALSE);
  drush_set_context('DRUSH_SIMULATE', FALSE);
  if ($simulated) {
    drush_set_context('DRUSH_VERBOSE', TRUE);
  }
  $dir = _drush_iq_git_root_dir_from_cwd();
  $branch = _drush_iq_get_branch_at_dir($dir);
  $branch_merges_with = _drush_iq_get_branch_merges_with($dir);
  if ($branch != $branch_merges_with) {
    return drush_set_error('DRUSH_IQ_RELEASE_BRANCHED', dt("Cannot create a release while branched. Please switch back to development branch (perhaps %branch)", array('%branch' => $branch_merges_with)));
  }
  if (!preg_match('/^[0-9]+\.x-[0-9]+.x$/', $branch)) {
    return drush_set_error('DRUSH_IQ_RELEASE_UNSUPPORTED_BRANCH', dt("Branch %branch is unsupported by iq-release.  Branches must be in the form 7.x-1.x to use this command.", array('%branch' => $branch)));
  }
  $project = _drush_iq_find_project_via_remote_origin($dir);
  if (!$project) {
    return drush_set_error('DRUSH_IQ_NO_PROJECT', dt("Could not determine project. Please check out project from git."));
  }
  $base_version = FALSE;
  $release_kind = 'none';
  // $branch should be something like "7.x-2.x"
  // $required_release_prefix will be "7.x-2."
  $required_release_prefix = substr($branch, 0, -1);
  $values = drush_invoke_process('@none', 'pm-releases', array($project), array(), array('integrate' => FALSE));

  if ($values['error_status']) {
    return drush_set_error('DRUSH_IQ_INDETERMINATE_RELEASES', dt("Could not determine existing releases for project %project", array('%project' => $project)));
  }
  if (isset($values['object'][$project]['releases'])) {
    foreach(array_keys($values['object'][$project]['releases']) as $release) {
      if (substr($release, 0, strlen($required_release_prefix)) == $required_release_prefix) {
        $current_minor_release = substr($release, strlen($required_release_prefix));
        if (is_numeric($current_minor_release[0]) && !$base_version) {
          // If $required_release_prefix will be "7.x-2.", then
          // $base_version will be something like "7.x-2.4", or
          // maybe "7.x-2.0-alpha3".  $current_minor_release
          // will then be what's left over: "0-alpha3", and
          // $base_minor_version will be "0"
          $base_version = $release;
          $base_minor_version = intval($current_minor_release);
          // $unstable might be empty, or might be "-alpha3".
          // We generally expect $unstable to be empty unless
          // $base_minor_version is 0.
          $unstable = substr($base_version, strlen($required_release_prefix) + strlen($base_minor_version));
          if (empty($unstable)) {
            $new_release_version = $required_release_prefix . ($base_minor_version + 1);
            $release_kind = "stable";
          }
          else {
            if (preg_match('#([0-9]+$)#', $unstable, $matches)) {
              // If $unstable is "-alpha12", then $unstable_version
              // will be "12", and $release_kind will be "alpha".
              $unstable_version = $matches[1];
              $release_kind = substr($unstable, 1, -strlen($unstable_version));
              $new_release_version = $required_release_prefix . $base_minor_version;
            }
            else {
              return drush_set_error('DRUSH_IQ_CONFUSING_UNSTABLE_RELEASE', dt("Did not know what to make of the version !unstable.  We expect unstable releases to end in a number -- something like 7.x-2.0-alpha3", array('!unstable' => $unstable)));
            }
          }
        }
      }
    }
  }
  $release_kinds = array(
    'stable' => "Stable", 
    'rc' => "Release Candidate", 
    'beta' => "Beta", 
    'alpha' => "Alpha",
  );
  $desired_kind = $release_kind;
  foreach ($release_kinds as $check => $description) {
    if (drush_get_option($check, FALSE)) {
      $desired_kind = $check;
    }
  }
  if ($desired_kind == "none") {
    $desired_kind = drush_choice($release_kinds, dt("What kind of release would you like to make?"));
    if ($desired_kind === FALSE) {
      return drush_user_abort();
    }
  }
  // Did the user ask for a release kind that is LESS STABLE then
  // the current release?
  if ($desired_kind != _drush_iq_most_stable($desired_kind, $release_kind)) {
    return drush_set_error('DRUSH_IQ_INSUFFICIENTLY_STABLE', dt("You cannot create a !kind release off of branch !branch, because that is less stable than the current release.  To make a less-stable release, create a new dev branch with a higher minor release number.", array('!kind' => $release_kinds[$desired_kind], '!branch' => $branch)));
  }
  // Special stuff for the first release on a branch
  if (!$base_version) {
    $new_release_version = $required_release_prefix . '0';
    // $b will be something like "7.x-2".
    $b = substr($required_release_prefix, 0, -1);
    if (preg_match('#([0-9]+$)#', $b, $matches)) {
      $current_minor = $matches[1];
      if ($current_minor > 0) {
        $base_version = substr($b, 0, -strlen($current_minor)) . ($current_minor - 1) . '.x';
      }
    }
  }
  if ($desired_kind != "stable") {
    if ($desired_kind != $release_kind) {
      $unstable_version = 1;
    }
    else {
      $unstable_version++;
    }
    $new_release_version .= '-' . $desired_kind . $unstable_version;
  }
  drush_log(dt("New release version is !version; base version is !base", array('!version' => $new_release_version, '!base' => $base_version)));
  // TODO: If there are no previous releases at all, then $base_version will be
  // empty, and the `drush release-notes` command will not work.  We could do it
  // ourselves with git log (just show all commits, since there are no releases at all).
  $values = drush_invoke_process('@none', 'release-notes', array($base_version, $branch), array(), array('integrate' => FALSE));
  $release_notes = drush_html_to_text($values['output']);
  $first_bullet = strpos($release_notes, '*');
  $content_after_bullet = substr($release_notes, $first_bullet + 1);
  if (($first_bullet === FALSE) || (strlen(trim($content_after_bullet)) == 0)) {
    return drush_set_error('DRUSH_IQ_NO_COMMITS', dt("There have been no commits since %project %version was released. You can't make a release without commits!", array('%project' => $project, '%version' => $base_version)));
  }
  if (!empty($comment)) {
    $comment .= "\n\n";
  }
  $comment .= $release_notes;

  drush_print($comment);

  $release_type_checkboxes = array(
    'security' => '100',
    'bugs' => '99',
    'features' => '98',
  );
  $default_no_list = array('bugs');
  $release_checkbox_values = array();
  foreach ($release_type_checkboxes as $option_name => $option_id) {
    $default_no = !in_array($option_name, $default_no_list);
    $option_no_value = drush_get_option('no-' . $option_name, $default_no);
    // This is TRUE if --features is selected, or FALSE if --no-features
    // is selected, or an appropriate default value (FALSE for security and
    // features, TRUE for bugs) if neither are selected.
    $option_value = drush_get_option($option_name, !$option_no_value);
    $field_key = "taxonomy_vocabulary_7[und][$option_id]";
    if ($option_value) {
      $release_checkbox_values[$field_key] = $option_id;
      drush_log(dt("Marking this release '!mark'", array('!mark' => $option_name)), 'ok');
    }
    else {
      drush_log(dt("This release not marked '!mark'", array('!mark' => $option_name)), 'ok');      
    }
  }

  if ($simulated) {
    drush_print(dt('Simulating release of %project %new_release', array('%project' => $project, '%new_release' => $new_release_version)));
  }
  else {
    $confirm = drush_confirm(dt('Would you like to create a release, %project %new_release?', array('%project' => $project, '%new_release' => $new_release_version)));
    if (!$confirm) {
      return drush_user_abort();
    }
  }
  // Check to see if the release has already been tagged
  $result = drush_shell_exec("git log %s -1", $new_release_version);
  if ($result) {
    drush_log(dt("Tag %new_release already exists in project %project.", array('%project' => $project, '%new_release' => $new_release_version)), 'ok');
  }
  elseif (!$simulated) {
    // Tag the release
    $result = drush_shell_exec("git tag %s", $new_release_version);
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Failed to tag repository."));
    }
    $git_tag_output = drush_shell_exec_output();
    drush_print(implode("\n", $git_tag_output));
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not tag release."));
    }
    // Push the tag
    $result = drush_shell_exec("git push origin tag %s", $new_release_version);
    $git_push_tag_output = drush_shell_exec_output();
    drush_print(implode("\n", $git_push_tag_output));
    if (!$result) {
      return drush_set_error('DRUSH_IQ_GIT_FAILURE', dt("Could not push tag."));
    }
  }
  if (drush_get_option('tag-only', FALSE)) {
    drush_log(dt("Creating tag only; skiping release publication."));
    return TRUE;
  }

  // Get the node id for the project
  $project_info = drush_iq_fetch_project_via_name($project);
  if (empty($project_info)) {
    return drush_set_error('DRUSH_IQ_PROJECT_ID_NOT_FOUND', dt("Could not get project id for project !project", array('!project' => $project)));
  }
  $project_id = $project_info['nid'];

  // Get the username and password to use when submitting the patch
  $drupal_org_user = drush_get_option('user');
  $drupal_org_pass = drush_get_option('pass');
  $client = _drush_iq_login_user($drupal_org_user, $drupal_org_pass, "create release");
  if (!$client) {
    return FALSE;
  }
  // Submit the new release form on drupal.org,
  // submitting $comment as the release notes

  $release_uri = "https://drupal.org/node/add/project-release/$project_id";
  drush_print("\n\nFetching $release_uri\n\n");
  $add_release_page = $client->request('GET', $release_uri);
  //drush_print($add_release_page->html());

  //
  // Goutte does not seem to give us a good way to determine the
  // possible values for our <select> form option values.  Use
  // the DomCrawler to find all of the <opiton> nodes and build
  // it ourselves, then.
  //
  $possible_release_version_values = array();
  $add_release_page->filter('option')->each(function(Symfony\Component\DomCrawler\Crawler $node, $i) use(&$possible_release_version_values) {
      $node_value = $node->attr('value');
      $node_text = trim($node->text());
      $possible_release_version_values[$node_text] = $node_value;
    });

  if (!isset($possible_release_version_values[$new_release_version])) {
    return drush_set_error('DRUSH_IQ_TAG_NOT_AVAILABLE', dt("The tag %new_release has not appeared on drupal.org yet; perhaps try running this command again in a minute or two.", array('%new_release' => $new_release_version)));    
  }

  $select_release_form = $add_release_page->selectButton('Next')->form();
  $input_fields = $select_release_form->getValues();

  // fill in release version
  $input_fields['versioncontrol_release_label_id'] = $possible_release_version_values[$new_release_version];

  drush_log(dt("Select tag %new_release (!id) for new release.", array('%new_release' => $new_release_version, '!id' => $possible_release_version_values[$new_release_version])), 'notice');

  $create_release_form = $client->submit($select_release_form, $input_fields);

  $save_release_form = $create_release_form->selectButton('Save')->form();
  $input_fields = $save_release_form->getValues() + $release_checkbox_values;

  // Store the release comment in the node body
  $input_fields['body[und][0][value]'] = $comment;

  // Check the 'security confirm' checkbox only if the user selected 
  // a release with security fixes.
  if (isset($input_fields['security'])) {
    if (drush_get_option('i-have-contacted-the-security-team', FALSE) || drush_confirm(dt("Are you sure you want to mark this release as a Security update?\n\nIf you select Security update, your release will not be published without the manual intervention of the Drupal Security Team. You should have already contacted the Security Team to coordinate a security advisory (SA) for your release before you committed any security-related patches."))) {
      $input_fields['security_update_confirm'] = '1';
    }
  }
  $crawler = _drush_iq_form_submit($client, $save_release_form, $input_fields);
  $error_text = _drush_iq_form_errors($crawler);
  if (!empty($error_text)) {
    return drush_set_error('DRUSH_IQ_RELEASE_FAILED', dt('!err', array('!err' => implode("\n", $error_text))));
  }
  drush_log(dt("Release !version submitted.", array('!version' => $new_release_version)));
}

/**
 * Find the complete list of tags for an issue.  This is done
 * via git rather than a web services call, so it requires that
 * the project be downloaded using --package-handler=git_drupalorg
 */
function drush_iq_project_version_tags($issue_info) {
  $dir = _drush_iq_project_dir($issue_info);
  $version_tags = array();
  drush_shell_cd_and_exec($dir, "git tag");
  $version_tags = drush_shell_exec_output();
  return $version_tags;
}

/**
 * Command hook for drush iq-submit
 */
function drush_iq_submit($comment = '', $number = '') {
  $branch_merges_with = FALSE;
  // Handle "simulated" mode as we do for iq-open, above.
  $simulated = drush_get_context('DRUSH_SIMULATE', FALSE);
  drush_set_context('DRUSH_SIMULATE', FALSE);
  if ($simulated) {
    drush_set_context('DRUSH_VERBOSE', TRUE);
  }
  // Issue numbers never have spaces; comments always should have.
  // If the parameters provided has a space in the issue number,
  // but no space in the comment, then swap them.
  if (!empty($number) && !empty($comment)) {
    if ((strpos($number, ' ') !== FALSE) && (strpos($comment, ' ') === FALSE)) {
      $move_comment_to_number = $comment;
      $comment = $number;
      $number = $move_comment_to_number;
    }
  }

  // Provide a default comment if the user did not provide one.
  if (empty($comment)) {
    $comment = dt("Patch submitted by the Drush iq-submit command.");
  }
  // Find the branch, and the branch that we merge with.
  $dir = _drush_iq_git_root_dir_from_cwd();
  $branch = _drush_iq_get_branch_at_dir($dir);
  $branch_merges_with = _drush_iq_get_branch_merges_with($dir);
  // If an issue number was given, then use it in preference
  // to the issue number saved in the branch name (if any).
  if (!empty($number)) {
    $issue_info = drush_iq_get_info($number);
  }
  else {
    $issue_info = drush_iq_get_info($branch);
  }
  if (empty($issue_info)) {
    return FALSE;
  }
  // Record the issue id that we will be submitting our
  // patch to.
  $id = $issue_info['id'];
  $comment_number = drush_iq_next_comment_number($issue_info);
  $short_description = _drush_iq_make_description($issue_info['title'], 16);
  $do_not_test = drush_get_option('do-not-test', FALSE) ? '-do-not-test' : '';
  $patch_filename = drush_get_option('filename', $issue_info['project'] . '-' . $short_description . '-' . $id . '-' . $comment_number . $do_not_test . '.patch');

  // Skip the patch and just add metadata if --no-patch specified
  $include_patch = !drush_get_option('no-patch', FALSE);
  $file_upload_list = array();
  if ($include_patch) {
    // Start off using the 'drush iq-diff' command
    // to take a full diff of the current branch
    // against its upstream branch
    $values = drush_invoke_process('@none', 'iq-diff', array(), array(), array('integrate' => FALSE));
    $primary_patch = $values['output'];

    // We will use 'git diff' to make an interdiff.
    // If this is empty, or if the interdiff is close
    // in size to the full diff, then we will omit it
    // when we post the patch.
    $result = drush_shell_exec("git diff");
    $git_diff_output = drush_shell_exec_output();
    $interdiff = implode("\n", $git_diff_output);

    // Nothing to see here?  Stop if there are no code differences.
    if (empty($primary_patch) && empty($interdiff)) {
      return drush_set_error('DRUSH_IQ_SUBMIT_NO_DIFF', dt("Cannot submit a patch; there are no code differences."));
    }

    // If the primary patch is empty, but the interdiff
    // has content, then we'll use the output from
    // git diff as the primary patch.
    if (empty($primary_patch) && !empty($interdiff)) {
      $primary_patch = $interdiff;
      $interdiff = '';
    }

    // If the user specified that there should not be
    // an interdiff, then empty it out here.
    if (drush_get_option('no-interdiff', FALSE)) {
      $interdiff = '';
    }

    // If there is an interdiff, check to see if it
    // is large compared to the size of the primary patch.
    if (!empty($interdiff)) {
      $interdiff_size_ratio = drush_get_option('interdiff', '0.3');
      $patch_length = strlen($primary_patch);
      $interdiff_length = strlen($interdiff);
      $max_interdiff_lengh = $patch_length * $interdiff_size_ratio;
      drush_log(dt("Patch len=%plen, Interdiff len = %ilen. Ratio=%ratio & max=%max", array('%plen' => $patch_length, '%ilen' => $interdiff_length, '%ratio' => $interdiff_size_ratio, '%max' => $max_interdiff_lengh)), 'notice');
      if ($interdiff_length > $max_interdiff_lengh) {
        $interdiff = '';
      }
    }
    // Create a temporary directory to save our patch
    // file and interdiff in.
    $iq_submit_tmp_dir = drush_tempdir();

    // Write our primary patch, and if applicable, the
    // interdiff, to the temporary directory.
    file_put_contents($iq_submit_tmp_dir . '/' . $patch_filename, $primary_patch);
    $file_upload_list[] = $iq_submit_tmp_dir . '/' . $patch_filename;
    if ($interdiff) {
      file_put_contents($iq_submit_tmp_dir . '/interdiff.txt', $interdiff);
      $file_upload_list[] = $iq_submit_tmp_dir . '/interdiff.txt';
    }
  }

  // Get the username and password to use when submitting the patch
  $drupal_org_user = drush_get_option('user');
  $drupal_org_pass = drush_get_option('pass');
  $client = _drush_iq_login_user($drupal_org_user, $drupal_org_pass, "submit patch");
  if (!$client) {
    return FALSE;
  }

  $issue_uri = "https://drupal.org/node/$id/edit";
  $issue_page = $client->request('GET', $issue_uri);

  // Submit the file-upload subform as many times as needed.
  foreach ($file_upload_list as $upload_file) {
    $form_values = array();
    $upload_form = $issue_page->selectButton('Upload')->form();
    $file_fields = $upload_form->getFiles();
    $file_field_keys = array_keys($file_fields);
    $key = array_pop($file_field_keys);
    if (!empty($key)) {
      $form_values[$key] = $upload_file;
      drush_log(dt("Attached %file as %key and submitting it.", array("%file" => basename($upload_file), '%key' => $key)), 'ok');
      if (! $simulated) {
        // Submitting here returns a new replacement instance of $issue_page.
        $issue_page = $client->submit($upload_form, $form_values);
      }
    }
    else {
      drush_log(dt('Failed to attach file; the expected file upload field was not found on the form.'), 'error');
    }
    $form_values = array();
  }

  $comment_form = $issue_page->selectButton('Save')->form();
  $input_fields = $comment_form->getValues();

  // Mapping table from the Drush command line option
  // name to the corresponding form field item to set.
  $option_to_form_item_name = array(
    'title' => 'title',
    'version' => 'field_issue_version[und]',
    'component' => 'field_issue_component[und]',
    'assigned' => 'field_issue_assigned[und]',
    'category' => 'field_issue_category[und]',
    'priority' => 'field_issue_priority[und]',
    'issueStatus' => 'field_issue_status[und]',
    'tags' => 'taxonomy_vocabulary_9[und]',
  );

  // TODO: it would be good to look up all of the releases availalbe for
  // this project, and set $possible_values['version'] appropriately.
  $possible_values = drush_iq_project_issue_possible_values();
  $version_tags = drush_iq_project_version_tags($issue_info);
  $possible_values['version'] = array_combine($version_tags, $version_tags);

  // By default, we set the status to "needs review"
  // unless the user specifies that it should be something else.
  $defaults = array();
  $soft_defaults = array();
  $defaults['status'] = drush_get_option('nw', FALSE) ? "needs work" : "needs review";

  // If user specified --tag instead of --tags, append the specified
  // tag onto the end of the existing tags.
  $append_tag = drush_get_option('tag', FALSE);
  if ($append_tag) {
    $existing_tags = $comment_fields[$option_to_form_item_name['tags']];
    if (!empty($existing_tags)) {
      $existing_tags .= ',';
    }
    $existing_tags .= $append_tag;
    drush_set_option('tags', $existing_tags);
  }

  // We'll first test to see if there is a version that
  // is named exactly after the upstream branch, but more
  // often, $branch_merges_with will be something like
  // '8.x-6.x', and the corresponding version will be
  // '8.x-6.x-dev'.
  if ($branch_merges_with) {
    $soft_defaults['version'][] = $branch_merges_with;
    $soft_defaults['version'][] = $branch_merges_with . '-dev';
  }
  // We'll also test the branch name.
  if ($branch) {
    $soft_defaults['version'][] = $branch;
    $soft_defaults['version'][] = $branch . '-dev';
  }
  // If we merge with a branch that has the same name
  // as one of the available versions, then set a default
  foreach ($soft_defaults as $default_item => $possible_defaults) {
    if (isset($possible_values[$default_item])) {
      foreach ($possible_defaults as $test_value) {
        if (array_key_exists($test_value, $possible_values[$default_item])) {
          $defaults[$default_item] = $possible_values[$default_item];
        }
      }
    }
  }

  foreach ($option_to_form_item_name as $option_name => $form_item_name) {
    // If the user specified an issue metadata item to
    // change (e.g. --status='needs work'), then we will
    // get its value here. If the option is not set, then
    // drush_get_option will return the default value we
    // pass in as the second argument.
    $new_value = drush_get_option($option_name, isset($defaults[$option_name]) ? $defaults[$option_name] : FALSE);
    // If the option was specified, then change the form
    // value as the user desired.
    if ($new_value !== FALSE) {
      // If the form field is a SELECT element, then the
      // values that it may be set to will be found in
      // the form's 'possible-values' item.  In this case,
      // we map from the value specified by the user (which
      // is the label of the OPTION) with the corresponding
      // item's value.
      if (isset($possible_values[$form_item_name])) {
        if (isset($possible_values[$form_item_name][$new_value])) {
          $new_value = $possible_values[$form_item_name][$new_value];
        }
        else {
          // If the user picked something not allowed, show
          // all of the valid choices.
          $valid_values = implode("\n  ", array_keys($possible_values[$form_item_name]));
          return drush_set_error('DRUSH_IQ_SUBMIT_INVALID_VALUE', dt('"%name" cannot be given the value "%value".  Please select one of the valid options:' . "\n  %valid", array("%value" => $new_value, "%name" => $option_name, "%valid" => $valid_values)));
        }
      }
      $form_values[$form_item_name] = $new_value;
    }
  }
  $form_values['nodechanges_comment[comment_body][und][0][value]'] = $comment;

  // We need to HTML entity decode the issue summary here, otherwise we
  // would post back a double-encoded version, which would result in issue
  // summary changes that we don't want to touch.
  $form_values['body[und][0][value]'] = html_entity_decode($comment_form->get('body[und][0][value]')->getValue(), ENT_QUOTES, 'UTF-8');

  if ($simulated) {
    drush_print("simulating iq-submit to issue #$id\n");
    drush_print(var_export($form_values, TRUE));
    drush_print("\n");
  }
  else {
    $crawler = _drush_iq_form_submit($client, $comment_form, $form_values);
    $error_text = _drush_iq_form_errors($crawler);
    if (!empty($error_text)) {
      return drush_set_error('DRUSH_IQ_SUBMIT_FAILED', dt('!err', array('!err' => implode("\n", $error_text))));
    }

    if ($include_patch) {
      drush_log(dt("Patch submitted."), 'success');
    }
    else {
      drush_log(dt("Issue modified."), 'success');
    }
  }

  return TRUE;
}

function _drush_iq_form_submit($client, $form, $values) {
  do {
    // Repeat the form submission if there is a 502 gateway error.
    $crawler = $client->submit($form, $values);
    $response = $client->getResponse();
  } while ($response->getStatus() == 502);
  return $crawler;
}

function _drush_iq_form_errors($crawler) {
  $submit_errors = $crawler->filter('.error');
  $error_text = array();
  if ($submit_errors->count() > 0) {
    $error_text = $submit_errors->each(function ($node) {
      return trim($node->text());
    });
  }
  return $error_text;
}

function _drush_iq_login_user($drupal_org_user, $drupal_org_pass, $operation_msg) {
  if (empty($drupal_org_user)) {
    return drush_set_error('DRUSH_IQ_SUBMIT_NO_USER', dt("No username specified; cannot %op.", array('%op' => $operation_msg)));
  }
  if (empty($drupal_org_pass)) {
    return drush_set_error('DRUSH_IQ_SUBMIT_NO_PASS', dt("No password specified; cannot %op.", array('%op' => $operation_msg)));
  }

  // Log on as the selected user
  $client = drush_iq_drupalorg_user_login($drupal_org_user, $drupal_org_pass);
  $error_text = drush_iq_drupalorg_login_errors();
  if (!empty($error_text)) {
    return drush_set_error('DRUSH_IQ_SUBMIT_LOGIN_FAILED', dt("Failed to log in to drupal.org as %name.\n!msg", array('%name' => $drupal_org_user, '!msg' => implode("\n", $error_text))));
  }
  drush_log(dt("Logged in to drupal.org as %name", array('%name' => $drupal_org_user)), 'ok');
  return $client;
}

/**
 * Return an array of error messages from the last login attempt.
 * If the last login attempt was successful, the array will be empty.
 */
function drush_iq_drupalorg_login_errors() {
  return drush_get_context('DRUSH_IQ_LOGIN_ERRORS');
}

/**
 * Log in to drupal.org as the specified user, with the password provided.
 *
 * @return $client - On success, a Goutte client connected to drupal site.
 *                   On failure, drush_iq_drupalorg_login_errors() will return
 *                   an array containing error messages.
 */
function drush_iq_drupalorg_user_login($user, $pass) {
  // Pull in our autoloader if we are not running Drupal 8.
  // Under Drupal 8, we will use whatever Goutte Drupal provides.
  if (drush_drupal_major_version() < 8) {
    require dirname(__FILE__) . '/vendor/autoload.php';
  }

  $client = new Client();
  $crawler = $client->request('GET', 'https://www.drupal.org/user');
  $form = $crawler->selectButton('Log in')->form();
  $crawler = $client->submit($form, array('name' => $user, 'pass' => $pass));

  $error_text = array();
  $login_errors = $crawler->filter('.error');
  if ($login_errors->count() > 0) {
    $error_text = $login_errors->each(function ($node) {
      return trim($node->text());
    });
  }
  drush_set_context('DRUSH_IQ_LOGIN_ERRORS', $error_text);
  return $client;
}

/**
 * Extract the input, select and textarea elements
 * from the specified form.
 */
function _drush_iq_get_form_fields($contents, $form_id) {
  $input_fields = array();
  $submit_buttons = array();
  $possible_values = array();

  // Clean up the form a bit before we parse it.
  // First delete everything before the <form ...>
  $contents = preg_replace('#.*(<form[^>]* id="' . $form_id . '"[^>]*>)#ms', '${1}', $contents);
  // Next, delete everything after the </form>
  $contents = preg_replace("#</form>.*#ms", "</form>", $contents);
  // Finally, we get better results from SimpleXMLElement
  // if we convert every <option ...>Label</option> element
  // into <option ... label="Label" />, and remove <optgroup>s.
  $contents = preg_replace('#(<option[^>]*)>([^<]*)</option>#ms', '${1} label="${2}" />', $contents);
  $contents = preg_replace('#</*optgroup[^>]*>#', '', $contents);

  $xml_form = new SimpleXMLElement("<?xml version='1.0' standalone='yes'?>" . $contents);
  foreach ($xml_form->xpath('//input') as $input_field) {
    if (!empty($input_field['name'])) {
      // Keep a separate list of all of the <submit ...> elements.
      if ($input_field['type'] == 'submit') {
        $submit_buttons[(string)$input_field['name']][] = (string)$input_field['value'];
      }
      else {
        $input_fields[(string)$input_field['name']] = (string)$input_field['value'];
      }
      // If the item is a checkbox, set its value to FALSE
      // if it is not checked.
      if (($input_field['type'] == 'checkbox') && (empty($input_field['checked']))) {
        $input_fields[(string)$input_field['name']] = FALSE;
      }
    }
  }
  foreach ($xml_form->xpath('//select') as $select_field) {
    if (!empty($select_field['name'])) {
      $select_name = (string)$select_field['name'];
      $selected = $select_field->xpath("option[@selected='selected']");
      if (empty($selected)) {
        $selected = $select_field->xpath("option[@selected='selected']");
      }
      if (!empty($selected)) {
        // Record the name and value of the select item
        $input_fields[$select_name] = (string)$selected[0]['value'];
      }
      else {
        // If there is no valid selected value, give the field an
        // arbitrary value to avoid validation errors.
        $selected = $select_field->xpath("option");
        if (!empty($selected)) {
          $input_fields[$select_name] = (string)$selected[0]['value'];
        }
      }
      // Map from 'label' to 'value'.  We will use
      // the 'possible-values' list to do field validation
      // if the user changes any of the issue's metadata
      // items with a commandline option.
      foreach ($select_field->xpath("option") as $option) {
        $possible_values[$select_name][(string)$option['label']] = (string)$option['value'];
      }
    }
  }
  foreach ($xml_form->xpath('//textarea') as $textarea) {
    // <textarea ...> elements are handled like <input ...> elements.
    $input_fields[(string)$textarea['name']] = (string)$textarea['value'];
  }
  $form['action'] = (string)$xml_form['action'];
  $form['submit'] = $submit_buttons;
  $form['fields'] = $input_fields;
  $form['possible-values'] = $possible_values;

  if (drush_get_context("DRUSH_VERBOSE")) {
    var_export($form);
  }
  return $form;
}

function drush_iq_cache_dir() {
  $cache_parent = drush_find_tmp();
  $cache_dir = $cache_parent . '/drush-iq-cache';
  drush_mkdir($cache_dir);
  return $cache_dir;
}
