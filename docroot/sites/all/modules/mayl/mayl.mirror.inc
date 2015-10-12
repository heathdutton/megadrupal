<?php
/**
 * @file
 * Functions to actually mirror a link to a lcoal directory.
 */

/*
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * @author Victor Shnayder (shnayder seas.harvard.edu)
 * @author Dan Margo (dmargo eecs.harvard.edu)
 */

const MIRROR_ERROR_PERMISSIONS = 0;
const MIRROR_ERROR_INVALID = 1;

// magic needed to run scripts when PHP safe_mode is on
$REROUTE_COMMANDS = False; 

// switched to PHP Flex (slower, but less ridiculous)
if ( $REROUTE_COMMANDS ) {
    /* initialize any magic secret strings / paths, etc
     * that are needed to call external commands.
     * (e.g. routing them through a cgi script that actually does the work)
     */
}

/**
 * Write a placeholder file.
 *
 * Will make the "mirror" link not-broken until wget gets around to
 * overwriting it (and also handles the case of broken links).
 */
function _mayl_placeholder_file($mirror_dir, $filename, $host_url) {
    $now = format_date(time(), 'long');
    $text = "<html>
<head><title>Loading mirror of $host_url</title></head>
<body>
<h1>$now: Loading...</h1>

<p>...mirror of <a href=\"$host_url\">$host_url</a>.</p>

<p>This page will contain the mirror once mirroring is complete.

<p>If the mirror link was created more than a few minutes ago and you still see this, there was an error mirroring the page.  Check that the page is accessible.  If it is, consult the server administrator and/or look at the error logs.
</body>
</html>
";

  // Save the placeholder.
  file_unmanaged_save_data($text, $mirror_dir . '/' . $filename, FILE_EXISTS_REPLACE);
}

/**
 * Create a mirror.
 *
 * @TODO: This needs to run (in a batch) from a cron hook.
 *
 * @return
 *   The path to the mirrored content or FALSE on error.
 *
 * Called from the plugin--create the mirror (or actually, call a script that
 * will do the actual work).
 */
function mayl_create_mirror($host_url) {
  global $REROUTE_COMMANDS;

  $mirror = mayl_get_mirror($host_url);
  if (!empty($mirror->mid)) {
    drupal_set_message(t('A mirror for @url already exists.', array('@url' => $host_url)));
    return $mirror->mid;
  }

  $wget_path  = variable_get('mayl_wget_path', '/usr/bin/wget');
  $user_agent = variable_get('mayl_user_agent', 'Mozilla/5.0');

  $url_path = drupal_parse_url($host_url, PHP_URL_PATH);
  if ($url_path === FALSE) {
    return FALSE;
  }

  // VS: By fiat, we insist that the main mirrored file is called index.html.
  // This is ensured in mirror.py
  // BUG: this will break direct links to images and other non-html content.
  $filename = "index.html";

  // Write a database record for this mirror.
  $mid = db_insert('mayl_mirrors')->fields(array(
    'host_url' => $host_url,
    'created' => REQUEST_TIME,
  ))->execute();

  $path = drupal_get_path('module', 'mayl');

  if ($REROUTE_COMMANDS ) {
    // On some hosts that use chroot (e.g. nearlyfreespeech.net with PHP Fast),
    // the web server and cgi scripts see different paths to the same files.
    // If this is the case on your system, you'll need to figure out where
    // executed scripts will see the plugin directory
    // 
    // $script_plugin_path = FILL-IN-HERE
    throw new Exception('Not implemented: needs to be customized for your hosting server.');
  }

  // Create local paths.
  $mayl_mirror_dir = variable_get('mayl_mirror_directory', 'public://mirror');
  $mirror_dir = $mayl_mirror_dir . '/' . $mid;
  $mirror_log = drupal_realpath($mayl_mirror_dir . '/' . $mid . '.log');

  // Create the new mirror directory, as wget expects it to exist!
  drupal_mkdir($mirror_dir);

  // This function is still in php, so it gets the php path
  _mayl_placeholder_file($mirror_dir, $filename, $host_url);

  // be careful about things that might have spaces or other weirdness
  $host_url = escapeshellarg($host_url);
  $ua = escapeshellarg($user_agent);

  // this may need to be adjusted if there is path weirdness
  $mirror_script_path = $path . "/mirror.py"; 

  $cmd = "$mirror_script_path --ua $ua --wget_path $wget_path " . drupal_realpath($mirror_dir) . " $host_url";

  // This needs love. Possibly even some hook_cron or batch API goo.
  // Maybe not a python script wrapping wget. Hrm.
  if ($REROUTE_COMMANDS) {
    // Custom magic to actually run cmd.
    throw new Exception('Not implemented: needs to be customized for your hosting server.');
  } else {
    exec($cmd, $output);
  }

  // @TODO: Nice things like error checking.

  // Update the database record for this mirror.
  // @TODO: Size, maybe entity id?
  db_update('mayl_mirrors')
    ->fields(array(
      'updated' => time(),
    ))
    ->condition('mid', $mid)
    ->execute();

  return $mid;
}
