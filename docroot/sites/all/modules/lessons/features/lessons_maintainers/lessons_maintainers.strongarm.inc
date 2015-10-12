<?php
/**
 * @file
 * lessons_maintainers.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function lessons_maintainers_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeaccess_userreference';
  $strongarm->value = array(
    'lesson' => array(
      'field_lesson_maintainers' => array(
        'all' => array(
          'view' => 'view',
        ),
        'all_published' => '0',
        'author' => array(
          'delete' => 'delete',
          'update' => 'update',
          'view' => 'view',
        ),
        'author_published' => '1',
        'create' => array(
          'ladder' => 0,
          'lesson' => 0,
          'page' => 0,
        ),
        'priority' => '0',
        'referenced' => array(
          'delete' => 0,
          'deny_delete' => 0,
          'deny_update' => 0,
          'deny_view' => 0,
          'update' => 'update',
          'view' => 'view',
        ),
        'referenced_published' => '1',
        'unused' => '0',
        'views' => array(
          'view' => '',
          'view_args' => '',
        ),
      ),
    ),
    'ladder' => array(
      'field_ladder_maintainers' => array(
        'referenced' => array(
          'view' => 'view',
          'update' => 'update',
          'delete' => 0,
          'deny_view' => 0,
          'deny_update' => 0,
          'deny_delete' => 0,
        ),
        'referenced_published' => '1',
        'create' => array(
          'page' => 0,
          'ladder' => 0,
          'lesson' => 0,
        ),
        'author' => array(
          'view' => 'view',
          'update' => 'update',
          'delete' => 'delete',
        ),
        'author_published' => '1',
        'all' => array(
          'view' => 'view',
        ),
        'all_published' => '0',
        'unused' => '0',
        'views' => array(
          'view' => '',
          'view_args' => '',
        ),
        'priority' => '0',
      ),
    ),
  );
  $export['nodeaccess_userreference'] = $strongarm;

  return $export;
}
