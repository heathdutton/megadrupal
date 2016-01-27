<?php // $Id: auth_drupal.php,v 1.1 2009-11-10 15:20:48 arborrow Exp $
      // auth_drupal.php


$string['auth_drupaltitle'] = 'Drupal cookie';
$string['auth_drupaldescription'] = 'This method integrates Drupal login with Moodle in a rather useful special case: when Drupal and Moodle share both the database and the domain name. The module will look for a Drupal cookie that represents a valid, authenticated session, and will use it to create an authenticated Moodle session for the same user, creating an account if needed. The e-mail address is synched with the Drupal user table on each login.';
$string['auth_drupaldebugauthdb'] = 'Debug ADOdb';
$string['auth_drupaldebugauthdbhelp'] = 'Debug ADOdb connection to external database - use when getting empty page during login. Not suitable for production sites.';

$string['auth_drupaluserstoremove'] = 'User entries to remove: $a';
$string['auth_drupaluserstoupdate'] = 'User entries to update: $a';
$string['auth_drupaldeleteuser'] ='Deleted user $a[0] id $a[1]';
$string['auth_drupaldeleteusererror'] = 'Error deleting user $a';
$string['auth_drupalsuspenduser'] ='Suspended user $a[0] id $a[1]';
$string['auth_drupaldeleteusererror'] = 'Error suspending user $a';
$string['auth_drupalupdateuser'] ='Updated user $a';

?>
