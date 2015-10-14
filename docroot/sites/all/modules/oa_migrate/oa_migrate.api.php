<?php

/**
 * @file
 * Contains documentation about the Open Atrium Migrate module's hooks.
 */

/**
 * @defgroup oa_clone Open Atrium Migrate
 * @{
 * Hooks from the Open Atrium Migrate module.
 */

/**
 * Alter migration configuration before it's registered.
 *
 * This hook is called by oa_migrate_register_migrations() with an array
 * describing the migrations before actually registering them. You can use
 * this to, for example, swap out a Migration class or change the configuration
 * passed to one of them.
 *
 * @param array &$migrations
 *   An associative array following the same format passed into the
 *   oa_migrate_generate() function.
 * @param array $config
 *   An associative array following the same format as $config passed into
 *   th oa_migrate_register_migrations() function.
 *
 * @see oa_migrate_generate()
 * @see oa_migrate_register_migrations()
 */
function hook_oa_migrate_alter(&$migrations, $config) {
  // Replace the Migration class used to migrate users with a custom one.
  $migrations['user']['class_name'] = 'MyCustomDrupalUserMigration';
}

/**
 * @}
 */
