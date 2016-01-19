<?php

$aliases['example-production'] = array (
  'uri' => 'www.example.com',
  'root' => '/var/www/drupal/example-production/current',
  'path-aliases' => array(
    '%files' => 'sites/default/files',
    '%dump-dir' => '/var/lib/sitedata/drupal/example-production/tmp',
    '%dump' => '/var/lib/sitedata/drupal/example-production/tmp/example-production-local.sql',
  ),
  'command-specific' => array(
    // Ensure all cache or non-essential tables are not included in the
    // database dump, so it is as small as possible.
    'sql-dump' => array(
      'no-ordered-dump' => TRUE,
      'result-file' => '/var/lib/sitedata/drupal/example-production/tmp/example-production-local.sql',
      'structure-tables-list' => 'apachesolr_index_entities,apachesolr_index_entities_node,authmap,cache,cache_apachesolr,cache_block,cache_bootstrap,cache_entity_comment,cache_entity_file,cache_entity_node,cache_entity_taxonomy_term,cache_entity_taxonomy_vocabulary,cache_entity_user,cache_field,cache_file_styles,cache_filter,cache_form,cache_image,cache_libraries,cache_menu,cache_metatag,cache_page,cache_panels,cache_path,cache_rules,cache_styles,cache_token,cache_update,cache_views,cache_views_data,captcha_sessions,ctools_css_cache,ctools_object_cache,flood,forward_log,forward_statistics,history,queue,sessions,watchdog,webform_submissions,webform_submitted_data',
    ),
  ),
  'preview-sync' => array(
    'from' => TRUE,
    'to' => FALSE,
  ),
);

$aliases['example-preview'] = array (
  'uri' => 'preview.example.com',
  'root' => '/var/www/drupal/example-preview/current',
  // Needed if your preview environment is on a different physical webserver.
  //'remote-host' => 'preview.servers.example.com',
  //'remote-user' => 'drupal-site-preview',
  'path-aliases' => array(
    '%files' => 'sites/default/files',
    '%dump-dir' => '/var/lib/sitedata/drupal/example-preview/tmp',
    '%dump' => '/var/lib/sitedata/drupal/example-preview/tmp/example-preview-local.sql',
  ),
  'command-specific' => array(
    // Drush 6.x feature.
    'core-status' => array(
      'format' => 'json',
    ),
    'env-switch' => array(
      'force' => TRUE,
      'strict' => FALSE,
    ),
  ),
  'preview-sync' => array(
    'from' => FALSE,
    'to' => TRUE,
  ),
);
