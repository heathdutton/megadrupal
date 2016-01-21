<?php
/**
 * @file
 * ns_roles.features.user_role.inc
 */

/**
 * Implementation of hook_user_default_roles().
 */
function ns_roles_defaultconfig_user_default_roles() {
  $roles = array();

  // Exported role: administrator
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => '2',
    'uuid' => 'f7d6b2a9-8148-3c14-01e1-4c8562967bbc',
  );

  // Exported role: blogger
  $roles['blogger'] = array(
    'name' => 'blogger',
    'weight' => '3',
    'uuid' => '5b9a8cb7-70ce-7234-454d-4d741cf5b937',
  );

  // Exported role: chief editor
  $roles['chief editor'] = array(
    'name' => 'chief editor',
    'weight' => '4',
    'uuid' => '48e4fb27-eb48-05f4-35a3-b921e4278b91',
  );

  // Exported role: editor
  $roles['editor'] = array(
    'name' => 'editor',
    'weight' => '5',
    'uuid' => 'f1276cf2-7927-5534-f92e-e122ed562cd9',
  );

  // Exported role: super user
  $roles['super user'] = array(
    'name' => 'super user',
    'weight' => '7',
    'uuid' => 'c939facc-ff90-9df4-19b9-a6095f224ff8',
  );

  // Exported role: writer
  $roles['writer'] = array(
    'name' => 'writer',
    'weight' => '10',
    'uuid' => 'fa384293-48f3-6274-d52d-2ca8a5b2e9d8',
  );

  // Exported role: translator
  $roles['translator'] = array(
    'name' => 'translator',
    'weight' => '11',
    'uuid' => '89575d73-c16f-ec64-75db-1de9a0f082de',
  );

  return $roles;
}
