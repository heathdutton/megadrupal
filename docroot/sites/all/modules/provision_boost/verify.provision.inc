<?php

/**
 * implementation of provision_verify
 */
function drush_provision_boost_provision_verify($url = null) {
  // TODO: support platform?
  if (d()->type == 'site') {
    $config = new Provision_Config_Drushrc_Site(d()->name);

    if (isset($config->data['packages']['modules']['boost']) && ($config->data['packages']['modules']['boost']['status'] == 1)) {
      drush_log(dt("Boost: configuring site for boost support"));

      $cache_path = d()->root . '/cache';

      if (!is_dir($cache_path)) {
        provision_file()->mkdir($cache_path . '/normal/' . $url)
          ->succeed('Created <code>@path</code>')
          ->fail('Could not create <code>@path</code>')
          ->status();
      }

      provision_file()->chmod($cache_path, 0770, TRUE)
        ->succeed('Boost: Changed permissions of <code>@path</code> to @perm')
        ->fail('Boost: Could not change permissions <code>@path</code> to @perm')
        ->status();

      provision_file()->chgrp($cache_path, d('@server_master')->web_group, TRUE)
        ->succeed('Boost: Change group ownership of settings.php to @path')
        ->fail('Boost: Could not change group ownership of settings.php to @path')
        ->status();
    }
  }
}

/**
 * Inject the relevant Apache configuration in the site vhost
 */
function provision_boost_provision_apache_vhost_config($data = null) {
  $config = new Provision_Config_Drushrc_Site(d()->name);

  if (isset($config->data['packages']['modules']['boost']) && ($config->data['packages']['modules']['boost']['status'] == 1)) {
/*
   Cannot do this because we cannot trust the code.
   In some hosting environments, if the user can modify/upload modules,
   then they can modify boost_admin_generate_htaccess() to inject their
   own custom code in the vhost, which would be a big security issue.
   A solution might be to copy the code of that function from boost..
   but that means more effort to maintain it and keep it in sync.

    module_load_include('inc', 'boost', 'boost.admin');

    $host = variable_get('boost_server_name_http_host', '%{SERVER_NAME}');
    $root = variable_get('boost_document_root', '%{DOCUMENT_ROOT}');

    $htaccess = boost_admin_generate_htaccess($host, $root);
    return "RewriteEngine on\n" . $htaccess . "\n";
*/

    return file_get_contents(dirname(__FILE__) . "/boosted1.txt");
  }
}

/**
 * Inject the relevant Apache configuration into the global apache configuration
 */
function provision_boost_provision_apache_dir_config($data = null) {
  if (($version = drush_get_option('boost')) && is_numeric($version)) {
    return file_get_contents(dirname(__FILE__) . "/boosted$version.txt");
  }
}

/**
 * Persist boost settings in the drushrc.php
 */
function drush_provision_boost_post_provision_verify($url = NULL) {
  if (d()->type == 'site') {
    $config = new Provision_Config_Drushrc_Site(d()->name);

    if (isset($config->data['packages']['modules']['boost']) && ($config->data['packages']['modules']['boost']['status'] == 1)) {
      if (drush_drupal_major_version() >= 7) {
        boost_flush_caches();
      }
      else {
        // copied from boost.admin.inc function boost_reset_database_file_submit()
        // see also: http://drupal.org/node/944684
        $GLOBALS['conf']['boost_ignore_flush'] = 0;
        if (boost_cache_clear_all()) {
          db_query("TRUNCATE {boost_cache}");
          db_query("TRUNCATE {boost_cache_settings}");
          db_query("TRUNCATE {boost_cache_relationships}");
          db_query("TRUNCATE {boost_crawler}");
          _boost_rmdir_rf(BOOST_PERM_FILE_PATH, TRUE, TRUE, TRUE);
          drush_log(dt('Boost: Flushed ALL files from static page cache.'));
        } else {
          drush_log(dt('Boost: could NOT flush the cache, boost_cache_clear_all() mysteriously returned FALSE.'));
        }
      }
    }
  }
}
