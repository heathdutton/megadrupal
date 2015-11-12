CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Example Use Case
 * Contributers


INTRODUCTION
------------

Current Maintainer: Kevin Day <thekevinday@gmail.com>

Restricts changing the shortcut name to only users who have the
'allow changing shortcut name' permission.

With this module enabled users with 'shortcut_set_edit_access' will not be
able to change the shortcut name for any shortcut set. Users will need both
'shortcut_set_edit_access' and 'allow changing shortcut name' to change the
shortcut name for a given shortcut set.


INSTALLATION
------------

Simply add the module to /sites/all/modules/.


EXAMPLE USE CASE
----------------

Using the rules module, each individual user can have their own shortcut set.
Doing this means that the rule set name must not conflict on creation. Using
a generated set name of 'user-kevinday' with kevinday being the drupal user
name is a way to do this.

This can be done with a single reactionary rule:
Rule Name = Auto-Create Shortcut for New User
Rule Events Name = After saving a new user account
Rule Actions:
 - Execute Custom PHP code:
  <?php
    $set_title = 'user-' . $account->name;

    if (shortcut_set_title_exists($set_title)){
      watchdog('rules', "The shortcut set for the user %username called %shortcut_set already exists and will not be created.", array('%username' => $account->name, '%shortcut_set' => $set_title));
      return;
    }

    $shortcut_set = new stdClass();
    $shortcut_set->title = $set_title;
    shortcut_set_save($shortcut_set);

    $shortcut_set = db_select('shortcut_set', 'ss')
    ->fields('ss')
    ->condition('title', $set_title)
    ->execute()
    ->fetchObject();

    if (!is_object($shortcut_set)){
      watchdog('rules', "Cannot assign the shortcut set %shortcut_set to %username due to a database loading error.", array('%username' => $account->name, '%shortcut_set' => $set_title), WATCHDOG_ERROR);
      return;
    }

    shortcut_set_assign_user($shortcut_set, $account);
  ?>

The users themselves will have 'Edit current shortcut set' permission but not
the 'Select any shortcut set'. If the users can change the shortcut set name,
then there would be a problem. This module solves this problem by adding an
additional restriction to changing the shortcut set name.


CONTRIBUTERS
------------
Kevin Day
