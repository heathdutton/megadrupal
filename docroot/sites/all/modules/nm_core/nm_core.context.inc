<?php
/**
 * @file
 * nm_core.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_core_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_core_share';
  $context->description = 'Share buttons on nodes.';
  $context->tag = 'core';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'forum' => 'forum',
        'nm_announcement' => 'nm_announcement',
        'nm_blog' => 'nm_blog',
        'nm_event' => 'nm_event',
        'nm_gallery' => 'nm_gallery',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'sharethis-sharethis_block' => array(
          'module' => 'sharethis',
          'delta' => 'sharethis_block',
          'region' => 'content',
          'weight' => '-100',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  $export['nm_core_share'] = $context;


  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_core';
  $context->description = 'Sitewide blocks';
  $context->tag = 'core';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '~admin*' => '~admin*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu_block-nm_core-secondary-nav' => array(
          'module' => 'menu_block',
          'delta' => 'nm_core-secondary-nav',
          'region' => 'sidebar_first',
          'weight' => '-30',
        ),
        'system-help' => array(
          'module' => 'system',
          'delta' => 'help',
          'region' => 'content',
          'weight' => '-10',
        ),
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-9',
        ),
        'search-form' => array(
          'module' => 'search',
          'delta' => 'form',
          'region' => 'user_first',
          'weight' => '-17',
        ),
        'system-user-menu' => array(
          'module' => 'system',
          'delta' => 'user-menu',
          'region' => 'user_first',
          'weight' => '-10',
        ),
        'user-login' => array(
          'module' => 'user',
          'delta' => 'login',
          'region' => 'user_first',
          'weight' => '-10',
        ),
        'menu_block-nm_core-mobile-menu' => array(
          'module' => 'menu_block',
          'delta' => 'nm_core-mobile-menu',
          'region' => 'menu',
          'weight' => '-10',
        ),
        'nm_core-nodemaker_nice_menus_main' => array(
          'module' => 'nm_core',
          'delta' => 'nodemaker_nice_menus_main',
          'region' => 'menu',
          'weight' => '-9',
        ),
        'menu-menu-footer-links' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-links',
          'region' => 'footer_first',
          'weight' => '-26',
        ),
        'menu_block-nm_core-footer-menu' => array(
          'module' => 'menu_block',
          'delta' => 'nm_core-footer-menu',
          'region' => 'footer_second',
          'weight' => '-24',
        ),
        'system-powered-by' => array(
          'module' => 'system',
          'delta' => 'powered-by',
          'region' => 'footer_fourth',
          'weight' => '-10',
        ),
        'nm_core-nm_core_social_links' => array(
          'module' => 'nm_core',
          'delta' => 'nm_core_social_links',
          'region' => 'footer_third',
          'weight' => '-25',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;
  
  // Translatables
  // Included for use with string extractors like potx.
  t('Sitewide blocks');
  t('core');


  $export['nm_core'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_core_nodes';
  $context->description = 'Blocks on all nodes';
  $context->tag = 'core';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '~node/*/*' => '~node/*/*',
        '~node/*/*/*' => '~node/*/*/*',
        'node/*' => 'node/*',
        'node/*/view' => 'node/*/view',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_related_content-block' => array(
          'module' => 'views',
          'delta' => 'nm_related_content-block',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks on all nodes');
  t('core');
  $export['nm_core_nodes'] = $context;




  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_core_403';
  $context->description = 'Blocks on 403.';
  $context->tag = 'core';
  $context->conditions = array(
    'is_403' => array(
      'values' => array(
        'is_403' => 'is_403',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'user-login' => array(
          'module' => 'user',
          'delta' => 'login',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  
  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks on 403.');
  t('core');
  $export['nm_core_403'] = $context;


  return $export;
}
