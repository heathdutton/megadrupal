<?php
/**
 * @file
 * Scan files and ensure they are all registered.
 *
 * Bulk operation to scan your files directory and ensure that every file has a
 * corresponding entry in the 'managed files' table.
 *
 * Without it, you can't re-use files via filefield_sources as promised.
 *
 * Original D6 by John Locke on 02/23/2010
 * From http://www.freelock.com/blog/john-locke/2010-02
 *      /using-file-field-imported-files-drupal-drush-rescue
 *
 * Upgraded to D7 by dman.
 * For a less-naive solution to this problem
 * (actually scan pages and attach the right files to individual nodes)
 *
 * @see http://drupal.org/project/file_ownage
 *
 *
 * USAGE:
 *   Trial run:
 * drush --verbose findfiles sites/sitename/files
 *   Real run:
 * drush findfiles sites/sitename/files true
 *
 * BACKUP your DB first!
 */

/**
 * Provide module specific drush commands.
 */
function file_ownage_drush_command() {

  $items = array();
  $items['findfiles'] = array(
    'description' => 'Search filesystem for files by path, and import them into the required Drupal database table',
    'arguments' => array(
      'filepath' => 'Name of path to scan. Required.',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'yes' => 'Actually do it. Without this option set it will be a dry-run only.',
    ),
    'examples' => array(
      'drush findfiles --verbose sites/default/files' => 'Preview what would happen if this was run on all files in the site directory',
      'drush findfiles --yes sites/default/files/ftp-uploads' => 'Actually run the process on all files within the ftp-uploads directory',
      'drush findfiles random/path' => 'Fail. Directories outside of the public files directory are not supported by Drupal file-wrappers',
    ),
  );
  return $items;
}

/**
 * Drush command callback.
 *
 * Implements drush_hook_COMMAND().
 *
 * @param string $scandir
 *   Directory to scan.
 *
 * @return bool
 *   Success.
 */
function drush_file_ownage_findfiles($scandir) {

  if (empty($scandir)) {
    drush_log("Directory path is required. Try " . variable_get('file_public_path', NULL), 'error');
    return FALSE;
  }
  $commit = drush_get_option('yes', FALSE);
  $files_directory = variable_get('file_public_path', NULL);
  drush_log("Scanning directory $scandir for any unregistered files", 'ok');
  $ar = file_scan_directory($scandir, '@.*@');
  foreach ($ar as $item) {
    // Need to think in file wrappers, from the beginning.
    // Chop off the normal files dir
    // so we have a path relative to the files dir.
    $file_uri_path = str_replace($files_directory . '/', '', $item->uri);
    // $file_uri = file_build_uri($file_uri_path);
    // $local_filepath = str_replace($scandir . '/', '', $item->uri);
    $file = new stdClass();
    $file->fid = NULL;
    // DO NOT USE $item->name as it truncates the suffix.
    // Normally that would be nice but it cripples IMCE!!
    $file->filename = basename($item->uri);
    $file->uri = file_build_uri($file_uri_path);
    $file->filemime = file_get_mimetype($file->uri);
    global $user;
    $file->uid = $user->uid;
    $file->status = FILE_STATUS_PERMANENT;

    // Look for file in {file_managed} table.
    drush_log("Checking db for {$file->uri}");
    $result = db_query("SELECT * FROM {file_managed} WHERE uri = :uri", array(':uri' => $file->uri));
    $record = NULL;
    foreach ($result as $record) {
      // Found at least one.
      drush_log("Found registered file OK    : {$file->uri} fid:{$record->fid}", 'ok');
    }

    if (!$record) {
      drush_log('File not found in the DB yet: ' . $file->uri, 'warning');

      if ($commit) {
        drush_log('Saving file to database: ' . $file->uri, 'notice');
        // Get file wrapper CRUD to save it for us.
        drupal_chmod($file->uri);
        file_save($file);
        // Other modules - specifically filefield_sources -
        // May not play ball unless the file is 'in use' as well.
        // @see file_managed_file_validate()
        // @see file_usage_list($file);
        // We don't have anything useful to tell it, about previous usage
        // so just say it's managed by 'system'
        // Now the filefield_sources imce browse
        // will at least be able to find these.
        file_usage_add($file, 'system', 'file', $file->fid);
        // Even *with* this, it still won't be available to the
        // "Reference existing" option
        // Until used at leased once *by the same field*
        drush_log("Saved file to database: file: {$file->uri} fid:{$record->fid}", 'ok');
      }
    }
  }
}
