<?php
/**
 * @file
 * oa_migrate_legacy_user.features.inc
 */

/**
 * Implements hook_field_group_info_alter().
 */
function oa_migrate_legacy_user_field_group_info_alter(&$data) {
  if (isset($data['group_user_profile|user|user|form'])) {
    $data['group_user_profile|user|user|form']->data['children'][1] = 'field_oa_migrate_legacy_user_org'; /* WAS: '' */
    $data['group_user_profile|user|user|form']->data['children'][2] = 'field_oa_migrate_legacy_user_tel'; /* WAS: '' */
    $data['group_user_profile|user|user|form']->data['children'][3] = 'field_oa_migrate_legacy_user_url'; /* WAS: '' */
    $data['group_user_profile|user|user|form']->data['children'][4] = 'field_oa_migrate_legacy_user_adr'; /* WAS: '' */
  }
}
