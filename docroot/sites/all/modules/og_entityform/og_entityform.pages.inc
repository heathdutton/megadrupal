<?php
/**
 * @file
 * User facing pages for OG Entityform module.
 */

/**
 * Callback for the group member list of entityforms.
 *
 * @param string $group_type
 * @param int $gid
 *
 * @return string
 */
function og_entityform_forms_list($group_type, $gid) {
  return views_embed_view('og_entityform_forms_list', 'default', $group_type, $gid);
}

/**
 * Load an entityform in a tab. This breaks out the argument eform-type
 * @param $group_type
 * @param $gid
 * @param $entityform_arg
 *
 * @return array|mixed
 */
function og_entityform_form_wrapper($group_type, $gid, $entityform_arg) {
  if (preg_match('/eform-(.*)$/', $entityform_arg, $matches)) {
    module_load_include('inc', 'entityform', 'entityform.admin');
    $entityform = entityform_empty_load($matches[1]);
    return entityform_form_wrapper($entityform, 'submit', 'embedded');
  }
  drupal_access_denied();
}
