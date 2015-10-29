<?php
/**
 * @file
 * Contains function relating to drush-integration of this module.
 */

/**
 * Describes each drush command implemented by the module.
 *
 * @return string
 *   The first line of description when executing the help for a given command
 */
function mlist_drush_help($command) {
  switch ($command) {
    case 'drush:mailman-sync':
      return dt('Sync subscribers with the mailman for the selected roles.');
  }
}
/**
 * Registers a drush command and constructs the full help for that command.
 *
 * @return array
 *   And array of command descriptions
 */
function mlist_drush_command() {
  $items = array();
  $items['mailman-sync'] = array(
    'description' => dt('Sync subscribers with the mailman for the selected roles.'),
    'options'   => array(
      'commit' => dt('Set to 1 to perform the sync operation. By default a dry run is performed.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mailman-sync',
      'Using command alias' => 'drush mmsync',
      'Commit the changes' => 'drush mmsync --commit=1',
      'Include emails from a profile field named \'profile_alt_email\'' => 'drush mmsync --profile=profile_alt_email',
    ),
    'aliases' => array('mmsync'),
  );
  return $items;
}
/**
 * Sync mailman lists.
 *
 * NOTE: The following code is executed when drush 'mmsync' or
 * 'drush mailman-sync' is called
 */
function drush_mlist_mailman_sync() {
  $commit = drush_get_option('commit');
  $profile = drush_get_option('profile');

  if (!command_exists('sync_members')) {
    drush_print('The "sync_members" command is not in the current PATH. Cannot sync mailing lists.');
    return;
  }

  // Get the set of lists.
  $lists = db_select('mailing_lists', 'ml')
    ->fields('ml', array('list_id', 'list_name', 'sync'))
    ->execute();

  // Iterate through the lists and see if they have syncing turned on.
  $num_lists = 0;
  while ($list = $lists->fetchObject()) {
    if ($list->sync) {
      drush_print('Syncing ' . $list->list_name . '...');
      $roles = explode(',', $list->sync);
      // Test if the roles include 'authenticated user'.
      $tresult = db_select('role', 'r')
        ->fields('r', array('name'))
        ->condition('name', 'authenticated user')
        ->condition('rid', $roles)
        ->execute()
        ->fetchField();
      $usql = '';
      $usql_args = array();
      $alternate_emails = NULL;
      // If the roles to be synced include 'authenticated user', select emails
      // for all registerd users.
      if ($tresult == 'authenticated user') {
        // Create the SQL for retreiving all emails for all users.
        $usql = 'SELECT distinct mail FROM {users} WHERE uid > 1';
        // If a profile field name is provided, then get the alternate email.
        if ($profile) {
          $profile_sql = "
            SELECT distinct value
            FROM {users} U
              INNER JOIN
               (SELECT uid, value
                FROM {profile_values} PV
                  INNER JOIN {profile_fields} PF ON PF.fid = PV.fid
                WHERE PF.name = :profile
                  AND PV.value <> '') ALT ON ALT.uid = U.uid
          ";
          $alternate_emails = db_query($profile_sql, array(':profile' => $profile));
        }
      }
      // Else, only select emails for the selected user group.
      else {
        // The SQL for retreiving email for all users in the specified roles.
        $usql = '
          SELECT distinct mail
          FROM {users} U
            INNER JOIN {users_roles} UR ON U.uid = UR.uid
          WHERE rid IN (:roles) AND U.uid > 1
        ';
        $usql_args = array(':roles' => $roles);
        // If a profile field name is provided, get the alt email addresses.
        if ($profile) {
          $profile_sql = "
            SELECT distinct value
            FROM {users} U
              INNER JOIN
                (SELECT uid, value
                 FROM {profile_values} PV
                   INNER JOIN {profile_fields} PF ON PF.fid = PV.fid
                 WHERE PF.name = :profile
                   AND PV.value <> '') ALT  ON ALT.uid = U.uid
              INNER JOIN {users_roles} UR ON U.uid = UR.uid
            WHERE rid IN (:roles) AND U.uid <> 0
          ";
          $alternate_emails = db_query($profile_sql, array(':profile' => $profile, ':roles' => $roles));
        }
      }
      // Get the email addresses for the users.
      $users = db_query($usql, $usql_args);
      // Construct a temporary file used for sycing the lists.
      $tmp = file_directory_temp() . '/' . $list->list_name . '-' . REQUEST_TIME . '.txt';
      drush_print($tmp);
      $handle = fopen($tmp, 'c');
      // Iterate through the users and add email addresses to the temp file.
      $counter = 0;
      while ($user = $users->fetchObject()) {
        if (strpos($user->mail, '@') !== FALSE) {
          fwrite($handle, str_replace(array(' ', ','), array('', ''), $user->mail) . "\n");
          $counter++;
        }
      }
      // If the profile file is provided then iterate through the alternate
      // emails and add those to the list as well.
      if ($profile) {
        while ($email = $alternate_emails->fetchObject()) {
          if (strpos($email->value, '@') !== FALSE) {
            fwrite($handle, str_replace(array(' ', ','), array('', ''), $email->value) . "\n");
            $counter++;
          }
        }
      }
      fclose($handle);
      // If this is a commit then run the command for syncing the list.
      if ($commit) {
        $cmd = "sync_members -w=no -g=no -a=no -f $tmp $list->list_name";
        system($cmd);
        drush_print("\nUpdated: " . $counter . " member(s) currently subscribed to the $list->list_name mailing list.");
        drush_print("The change has been committed.");
      }
      // If this is not a commit then it is a dry run.
      else {
        $cmd = "sync_members -n -w=no -g=no -a=no -f $tmp $list->list_name";
        system($cmd);
        drush_print("\nUpdated: " . $counter . " member(s) currently subscribed to the $list->list_name mailing list.");
        drush_print("This is just a Dry-Run. No change has been committed. Use 'drush mmsync --commit=1' to make the change permanent.");
      }
      // Remove the temp file.
      unlink($tmp);
    }
  }
  if ($num_lists == 0) {
    drush_print("There are no lists configured for syncing. No syncing performed.");
  }
}

/**
 * Determines if a command exists on the current environment.
 *
 * @param string $command
 *   The command to check.
 *
 * @return bool
 *   returns TRUE on success otherwise FALSE.
 */
function command_exists($command) {
  $where_command = (PHP_OS == 'WINNT') ? 'where' : 'which';

  $process = proc_open(
    "$where_command $command",
    array(
      0 => array("pipe", "r"),
      1 => array("pipe", "w"),
      2 => array("pipe", "w"),
    ),
    $pipes
  );
  if ($process !== FALSE) {
    $stdout = stream_get_contents($pipes[1]);
    fclose($pipes[1]);
    fclose($pipes[2]);
    proc_close($process);

    return $stdout != '';
  }

  return FALSE;
}
