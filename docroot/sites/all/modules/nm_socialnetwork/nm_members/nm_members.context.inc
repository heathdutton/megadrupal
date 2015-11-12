<?php
/**
 * @file
 * nm_members.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_members_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_member_profile';
  $context->description = '';
  $context->tag = 'members';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'members/*/blog' => 'members/*/blog',
      ),
    ),
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'all',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_member_profile-block' => array(
          'module' => 'views',
          'delta' => 'nm_member_profile-block',
          'region' => 'content',
          'weight' => '-10',
        ),
        'views-nm_member_profile-block_1' => array(
          'module' => 'views',
          'delta' => 'nm_member_profile-block_1',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
        'views-nm_member_profile-block_2' => array(
          'module' => 'views',
          'delta' => 'nm_member_profile-block_2',
          'region' => 'sidebar_first',
          'weight' => '-9',
        ),
        'views-nm_member_profile-block_4' => array(
          'module' => 'views',
          'delta' => 'nm_member_profile-block_4',
          'region' => 'sidebar_first',
          'weight' => '-8',
        ),
        'views-nm_member_profile-block_5' => array(
          'module' => 'views',
          'delta' => 'nm_member_profile-block_5',
          'region' => 'sidebar_first',
          'weight' => '-7',
        ),
        'views-nm_member_profile-block_3' => array(
          'module' => 'views',
          'delta' => 'nm_member_profile-block_3',
          'region' => 'sidebar_first',
          'weight' => '-6',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('members');
  $export['nm_member_profile'] = $context;

  return $export;
}
