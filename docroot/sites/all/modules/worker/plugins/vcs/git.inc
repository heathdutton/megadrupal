<?php
/**
 * @file
 * Provide basic git repository interface plugin.
 *
 * All git repository URLs should use the format demonstrated below:
 * cvs://user@example.com:git/stuff.git.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('git'),
  'description' => t('Basic git repository interface plugin.'),
  'checkout' => 'worker_git_checkout',
  'annotate' => 'worker_git_annotate',
  'revision' => 'worker_git_revision',
  'apply' => 'worker_patch_apply',
  'modified files' => 'worker_patch_modified_files',
);

/**
 * Checkout from the repository into working directory.
 *
 * @param $url
 *   A VCS repository URL.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 */
function worker_git_checkout($url) {
  $parts = explode('/', $url);
  $branch = escapeshellarg(end($parts));

  list(, $url) = explode('://', $url, 2);
  $url = substr($url, 0, -1 * strlen($branch) + 1);
  $url = escapeshellarg($url);

  return worker_execute("git clone --depth 1 -b $branch $url .");
}

/**
 * Annotate the file and return a map of line number to committer.
 *
 * @param $file
 *   Relative path to file.
 * @return
 *   Associative array of line number ot committer ID.
 */
function worker_git_annotate($file) {
  $lines = worker_execute_output('git annotate ' . escapeshellarg($file));

  $map = array();
  $number = 1;
  foreach ($lines as $line) {
    // Annotate should look like the following
    // "3c44d80d        (Author    2010-07-28 02:44:58 -0500       1)<?php"
    if (preg_match('/.*?\s+\(\s+(.*?)\s+.*?\)/', $line, $match)) {
      $map[$number++] = $match[1];
    }
  }
  return $map;
}

/**
 * Get the current revision of the code.
 *
 * @return
 *   Revision identifier.
 */
function worker_git_revision() {
  $lines = worker_execute_output('git log -n 1');

  foreach ($lines as $line) {
    // Looking for something like
    // "commit d8aec96e7b501d6727a5af28c0bbec43db6d1a14".
    if (preg_match('/^commit (.*?)$/m', $line, $match)) {
      return $match[1];
    }
  }
  return FALSE;
}
