<?php

/**
 * @file
 * Provide views data and handlers for commune.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data().
 */

function commune_views_data() {
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['commune']['table']['group']  = t('Commune');

  // ----------------------------------------------------------------
  // Fields

  /*
  $data['commune']['context'] = array(
    'title' => t('Context'),
    'help' => t("The name of the entity this post is directed to. Can be rendered as a link to the entity's homepage."),
    'field' => array(
      'handler' => 'views_handler_field_commune_contextname', 
//'views_handler_field_comment_username',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // type of referral - @mention or direct
  $data['commune']['context_type'] = array(
    'title' => t('Context Type'),
    'help' => t('How the context was referred to - direct or @mention'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  */

  $data['commune']['eid'] = array(
    'title' => t('Context id'),
    'help' => t('If you need more fields than the eid add the commune: reference relationship'),
    'relationship' => array(
      'title' => t('Commune Context'),
      'help' => t("The Entity ID of the post's context."),
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('context'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  return $data;
}

/**
 * Use views_data_alter to add items to the node table that are
 * relevant to commune posts.
 */
function commune_views_data_alter(&$data) {

  $data['node']['commune_context'] = array(
    'title' => t('User posted to wall or @mentioned'),
    'help' => t('Display posts only if a user was @mentioned or was posted to user wall'),
    'argument' => array(
      'field' => 'uid',
      'name table' => 'users',
      'name field' => 'name',
      'handler' => 'views_handler_argument_commune_user_uid',
      'no group by' => TRUE,
    ),
    'filter' => array(
      'field' => 'uid',
      'name table' => 'users',
      'name field' => 'name',
      'handler' => 'views_handler_filter_commune_user_uid'
    ),
  );
}
