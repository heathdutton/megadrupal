<?php
/**
 * @file
 * The memberspage- and memberpage-callbacks.
 */

/**
 * Decide which page to show based on arguments.
 *
 * @param arg1
 *   Playerid or Season.
 * @param arg2
 *   Season if arg1 is playerid.
 */
function tabt_members_memberspage_callback($arg1 = NULL, $arg2 = NULL) {
  if ($arg1 == NULL) {
    return tabt_members_memberspage();
  }
  elseif (tabt_is_valid_season($arg1)) {
    return tabt_members_memberspage($arg1);
  }
  else {
    return tabt_members_memberpage($arg1, $arg2);
  }
}

/**
 * Create a page listing all members.
 *
 * @param season
 *   The season to look at.
 */
function tabt_members_memberspage($season = NULL) {
  $season = tabt_process_seasonnumber($season);

  // Get all members
  $members = tabt_members_getmembers($season);

  drupal_set_title(variable_get('tabt_members_memberspage_title', 'Members') . ' - ' . tabt_get_seasonname($season));

  return array(
    '#theme'     => 'tabt_members',
    '#members'   => $members,
    '#view_mode' => 'full',
    '#language'  => NULL,
    '#season'    => $season,
  );
}

/**
 * Create a playerprofile for 1 member.
 *
 * If the player has a Drupal Account, the visitor will be redirected to the
 * userprofile.
 *
 * @param pid
 *   The playerID. If this is incorrect, the Not Found-page will be displayed.
 */
function tabt_members_memberpage($pid, $season = NULL) {
  $season = tabt_process_seasonnumber($season);

  // PlayerID's are numeric.
  if (!is_numeric($pid)) {
    drupal_not_found();
    return;
  }

  // If the player has registered, go to the userprofile.
  if (tabt_is_current_season($season) && tabt_members_get_user($pid)) {
    drupal_goto('user/' . tabt_members_get_user($pid));
    return;
  }

  // Get the member.
  $member = tabt_members_get_member($pid, $season);
  // If we don't have a member, the ID is invalid.
  if ($member == NULL || $member == '' || !$member) {
    drupal_not_found();
    return;
  }

  // Set the pagetitle.
  drupal_set_title($member->Name . ' - ' . tabt_get_seasonname($season));
  return array(
    '#theme'     => 'tabt_memberpage',
    '#member'   => $member,
    '#view_mode' => 'full',
    '#language'  => NULL,
    '#season'   => $season,
  );
}