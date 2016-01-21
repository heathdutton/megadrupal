<?php
/**
 * @file
 * Contains all implementations of OG module hooks.
 */

/**
 * Implements hook_og_ui_get_group_admin().
 *
 * Adds a link to the Group Admin tab for Administering Flags.
 */
function og_flag_og_ui_get_group_admin($group_type, $gid) {
  $items = array();

  if (og_user_access($group_type, $gid, 'administer og flags')) {
    $items['flags'] = array(
      'title' => t('Administer Flags'),
      'description' => t('Administer flag settings for the group.'),
      // The final URL will be "group/$group_type/$gid/admin/og-flag".
      'href' => 'admin/og-flag',
    );
  }

  return $items;
}

/**
 * Implements hook_og_permission().
 *
 * Defines the group level admin flags permissions and permissions for
 * every flag that can be overridden in a group.
 */
function og_flag_og_permission() {
  $items = array();
  // Add permission for administering group flags.
  $items['administer og flag'] = array(
    'title' => t('Administer OG Flag'),
    'description' => t('Users may change the organic group flag settings.'),
    'default role' => array(OG_ADMINISTRATOR_ROLE),
  );

  // Add permissions for using each possible flag.
  $flags = flag_get_flags();
  // Provide flag and unflag permissions for each flag.
  foreach ($flags as $flag_name => $flag) {
    $items["flag $flag_name"] = array(
      'title' => t('Flag %flag_title', array('%flag_title' => $flag->title)),
    );
    $items["unflag $flag_name"] = array(
      'title' => t('Unflag %flag_title', array('%flag_title' => $flag->title)),
    );
  }

  return $items;
}

