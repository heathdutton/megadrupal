<?php
/**
 * @file
 * openacadept_main.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function openacadept_main_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'alumni';
  $context->description = '';
  $context->tag = 'People';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'profiles:people_alumni' => 'profiles:people_alumni',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-profiles-people_alumni' => array(
          'module' => 'views',
          'delta' => '-exp-profiles-people_alumni',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('People');
  $export['alumni'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'blog';
  $context->description = '';
  $context->tag = 'Blog';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'blogs/*' => 'blogs/*',
        '~blogs/*/*' => '~blogs/*/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-Blog-block_2' => array(
          'module' => 'views',
          'delta' => 'Blog-block_2',
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blog');
  $export['blog'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'blog_entry';
  $context->description = '';
  $context->tag = 'Blog';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'blog' => 'blog',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-Blog-block_3' => array(
          'module' => 'views',
          'delta' => 'Blog-block_3',
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blog');
  $export['blog_entry'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'blogs';
  $context->description = '';
  $context->tag = 'Blog';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'blog' => 'blog',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-Blog-block_2' => array(
          'module' => 'views',
          'delta' => 'Blog-block_2',
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blog');
  $export['blogs'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'event';
  $context->description = '';
  $context->tag = 'Event';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'event' => 'event',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-events-block_3' => array(
          'module' => 'views',
          'delta' => 'events-block_3',
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
      ),
    ),
    'breadcrumb' => 'research/events',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Event');
  $export['event'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'events';
  $context->description = '';
  $context->tag = 'Event';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'events:page' => 'events:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-events-page' => array(
          'module' => 'views',
          'delta' => '-exp-events-page',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'views-events-past_events' => array(
          'module' => 'views',
          'delta' => 'events-past_events',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Event');
  $export['events'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'faculty_staff';
  $context->description = '';
  $context->tag = 'People';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'profiles:people_faculty_staff' => 'profiles:people_faculty_staff',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-5835e821cc23d89d3c1a19bbc24d02d2' => array(
          'module' => 'views',
          'delta' => '5835e821cc23d89d3c1a19bbc24d02d2',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('People');
  $export['faculty_staff'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'project';
  $context->description = '';
  $context->tag = 'Project';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'project' => 'project',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-1eb2f47692a4417bdcf75bad6682e962' => array(
          'module' => 'views',
          'delta' => '1eb2f47692a4417bdcf75bad6682e962',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'views-projects-related_projects' => array(
          'module' => 'views',
          'delta' => 'projects-related_projects',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
      ),
    ),
    'breadcrumb' => 'research/projects',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Project');
  $export['project'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'projects';
  $context->description = '';
  $context->tag = 'Project';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'projects:projects' => 'projects:projects',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-projects-projects' => array(
          'module' => 'views',
          'delta' => '-exp-projects-projects',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Project');
  $export['projects'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'publication';
  $context->description = '';
  $context->tag = 'Publication';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'publication' => 'publication',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-3337adfb76a1f9e77772b4c03e011c8a' => array(
          'module' => 'views',
          'delta' => '3337adfb76a1f9e77772b4c03e011c8a',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'views-projects-related_projects' => array(
          'module' => 'views',
          'delta' => 'projects-related_projects',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
      ),
    ),
    'breadcrumb' => 'research/publications',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Publication');
  $export['publication'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'publications';
  $context->description = '';
  $context->tag = 'Publication';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'publications:publications' => 'publications:publications',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-publications-publications' => array(
          'module' => 'views',
          'delta' => '-exp-publications-publications',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Publication');
  $export['publications'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'resource';
  $context->description = '';
  $context->tag = 'Resource';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'resource' => 'resource',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'breadcrumb' => 'resources',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Resource');
  $export['resource'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'resources';
  $context->description = '';
  $context->tag = 'Resource';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'resources:resources_filter_block' => 'resources:resources_filter_block',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-2826dc4935a5dd45c6e9d856d4e1447e' => array(
          'module' => 'views',
          'delta' => '2826dc4935a5dd45c6e9d856d4e1447e',
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Resource');
  $export['resources'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'sitewide';
  $context->description = '';
  $context->tag = 'Sitewide';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-10',
        ),
        'menu_block-1' => array(
          'module' => 'menu_block',
          'delta' => 1,
          'region' => 'footer_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Sitewide');
  $export['sitewide'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'students';
  $context->description = '';
  $context->tag = 'People';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'profiles:people_students' => 'profiles:people_students',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-profiles-people_students' => array(
          'module' => 'views',
          'delta' => '-exp-profiles-people_students',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('People');
  $export['students'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user_profile';
  $context->description = '';
  $context->tag = 'Profile';
  $context->conditions = array(
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
        'views-user_content-blog' => array(
          'module' => 'views',
          'delta' => 'user_content-blog',
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
        'views-user_content-projects_head' => array(
          'module' => 'views',
          'delta' => 'user_content-projects_head',
          'region' => 'sidebar_second',
          'weight' => 1,
        ),
        'views-user_content-projects' => array(
          'module' => 'views',
          'delta' => 'user_content-projects',
          'region' => 'sidebar_second',
          'weight' => 2,
        ),
        'views-user_content-publications' => array(
          'module' => 'views',
          'delta' => 'user_content-publications',
          'region' => 'sidebar_second',
          'weight' => 3,
        ),
      ),
    ),
    'breadcrumb' => 'node/19',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Profile');
  $export['user_profile'] = $context;

  return $export;
}
