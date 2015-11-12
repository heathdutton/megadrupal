<?php
/**
 * @file
 * OG Entityform Integration: Admin callbacks.
 */

/**
 * Callback for the group admin's list of entityforms.
 *
 * @param string $group_type
 * @param int $gid
 *
 * @return string
 */
function og_entityform_admin_forms_list($group_type, $gid) {
  return views_embed_view('og_entityform_group_admin_list', 'default', $group_type, $gid);
}