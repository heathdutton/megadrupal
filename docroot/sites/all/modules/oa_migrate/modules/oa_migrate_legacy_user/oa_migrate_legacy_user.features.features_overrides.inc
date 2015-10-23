<?php
/**
 * @file
 * oa_migrate_legacy_user.features.features_overrides.inc
 */

/**
 * Implements hook_features_override_default_overrides().
 */
function oa_migrate_legacy_user_features_override_default_overrides() {
  // This code is only used for UI in features. Exported alters hooks do the magic.
  $overrides = array();

  // Exported overrides for: field_group
  $overrides["field_group.group_user_profile|user|user|form.data|children|1"] = 'field_oa_migrate_legacy_user_org';
  $overrides["field_group.group_user_profile|user|user|form.data|children|2"] = 'field_oa_migrate_legacy_user_tel';
  $overrides["field_group.group_user_profile|user|user|form.data|children|3"] = 'field_oa_migrate_legacy_user_url';
  $overrides["field_group.group_user_profile|user|user|form.data|children|4"] = 'field_oa_migrate_legacy_user_adr';

 return $overrides;
}
