<?php
/**
 * @file
 * Provide basic CVS repository interface plugin.
 *
 * All CVS repository URLs should use the format demonstrated below:
 * cvs://:pserver:anonymous:anonymous@example.com:/repository/path:module/path:revision.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('CVS'),
  'description' => t('Basic CVS repository interface plugin.'),
  'checkout' => 'worker_cvs_checkout',
  'annotate' => 'worker_cvs_annotate',
  'revision' => 'worker_cvs_revision',
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
function worker_cvs_checkout($url) {
  $parts = parse_url($url);

  // Break apart path using ':' to get repository path, module, and revision.
  list($path, $module, $revision) = explode(':', $parts['path']);

  // Build the CVS root using the parts of the URL.
  $cvs_root = escapeshellarg(':' . $parts['pass'] . '@' . $parts['host'] . ':' . $path);

  // Get the name of the current directory.
  $directory = explode('/', getcwd());
  $directory = end($directory);

  // Execute CVS checkout command with level 9 compression and checkout into
  // the current working directory.
  chdir('..');
  $return = worker_execute("cvs -z9 -d $cvs_root checkout -d $directory -r $revision $module");
  chdir($directory);
  return $return;
}

/**
 * Annotate the file and return a map of line number to committer.
 *
 * @param $file
 *   Relative path to file.
 * @return
 *   Associative array of line number ot committer ID.
 */
function worker_cvs_annotate($file) {
  $lines = worker_execute('cvs annotate ' . escapeshellarg($file));

  $map = array();
  $number = 1;
  foreach ($lines as $line) {
    // Ignore empty lines and lines that do not start with a number or do not
    // match the regular expression. The annotation line format looks like the
    // following: 1.1         (some_username    10-Mar-01): <?php
    if (!empty($line) && is_numeric($line[0]) && preg_match('/.*?\s+\((.*?)\s+.*?\)|/', $line, $match)) {
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
function worker_cvs_revision() {
  $lines = worker_execute_output('cvs log -bN');

  $latest = 0;
  foreach ($lines as $line) {
    // Looking for something like
    // "date: 2001-04-16 13:21:21 -0500;  author: foo;  state: Exp;  lines: +56 -1;".
    if (preg_match('/date: (.*?);/', $line, $match)) {
      $latest = max(strtotime($match[1]), $latest);
    }
  }
  return $latest;
}
