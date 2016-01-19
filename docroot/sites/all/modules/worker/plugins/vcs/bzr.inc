<?php
/**
 * @file
 * Provide basic bzr repository interface plugin.
 *
 * All bzr repository URLs should use the format demonstrated below:
 * bzr://lp:~example-project/example/trunk.
 *
 * Also note that in order to checkout of launchpad you will need to add a
 * command to run during setup to login the user that is to perform the
 * checkout, for example: bzr launchpad-login example-user.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('bzr'),
  'description' => t('Basic bzr repository interface plugin.'),
  'checkout' => 'worker_bzr_checkout',
  'annotate' => 'worker_bzr_annotate',
  'revision' => 'worker_bzr_revision',
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
function worker_bzr_checkout($url) {
  $url = escapeshellarg(str_replace('bzr://', '', $url));
  return worker_execute("bzr checkout --lightweight $url .");
}

/**
 * Annotate the file and return a map of line number to committer.
 *
 * @param $file
 *   Relative path to file.
 * @return
 *   Associative array of line number ot committer ID.
 */
function worker_bzr_annotate($file) {
  $lines = worker_execute_output("bzr annotate --long --all $file");

  $map = array();
  $number = 1;
  foreach ($lines as $line) {
    // Annotate output should look like the following
    // "127   author    20010310 | <?php".
    if (preg_match('/\d+\s+(.*?)\s+\d+\s+\|/', $line, $match)) {
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
function worker_bzr_revision() {
  $lines = worker_execute_output('bzr log -l 1');

  foreach ($lines as $line) {
    // Looking for something like "revno: 16054".
    if (preg_match('/^revno: (\d+)$/m', $line, $match)) {
      return $match[1];
    }
  }
  return FALSE;
}
