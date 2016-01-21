<?php
/**
 * @file
 * Provide basic svn repository interface plugin.
 *
 * All svn repository URLs should use the format demonstrated below:
 * svn://http://example.com/svn/trunk
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('svn'),
  'description' => t('Basic svn repository interface plugin.'),
  'checkout' => 'worker_svn_checkout',
  'annotate' => 'worker_svn_annotate',
  'revision' => 'worker_svn_revision',
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
function worker_svn_checkout($url) {
  $url = escapeshellarg(str_replace('svn://', '', $url));
  return worker_execute("svn checkout --non-interactive --trust-server-cert $url .");
}

/**
 * Annotate the file and return a map of line number to committer.
 *
 * @param $file
 *   Relative path to file.
 * @return
 *   Associative array of line number ot committer ID.
 */
function worker_svn_annotate($file) {
  $lines = worker_execute_output('svn annotate '. escapeshellarg($file));

  $map = array();
  $number = 1;
  foreach ($lines as $line) {
    // Looking for something like "   123 author <?php".
    if (preg_match('/^\s+\d+\s+(.*?) /m', $line, $match)) {
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
function worker_svn_revision() {
  $lines = worker_execute_output('svn up');

  foreach ($lines as $line) {
    // Looking for textual reference to the current revision.
    if (preg_match('/^(At|Updated to) revision (\d+)\.$/m', $line, $match)) {
      return $match[2];
    }
  }
  return FALSE;
}
