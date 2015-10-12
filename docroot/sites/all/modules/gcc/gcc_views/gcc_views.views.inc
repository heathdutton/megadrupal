<?php

/**
 * @file
 * GCC Views integration.
 */

/**
 * Implements hook_views_data().
 */
function gcc_views_views_data() {

  $data = array();

  $data['gcc_membership']['table']['group'] = t('GCC');

  $data['gcc_membership']['state'] = array(

    'title' => t('Membership State'),
    'help' => t('The state of the membership'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_gcc_membership_state',
    ),
  );

  $data['views']['gcc_context'] = array(

    'title' => t('GCC Context'),
    'help' => t('Allow to detecte a gcc context.'),
    'group' => t('GCC'),
    'area' => array(
      'handler' => 'views_handler_area_gcc_context',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function gcc_views_views_data_alter(&$data) {

  $field_audience = field_info_field(GCC_FIELD_AUDIENCE);
  $audience_table = _field_sql_storage_tablename($field_audience);
  $audience_entity_type_column = _field_sql_storage_columnname(GCC_FIELD_AUDIENCE, 'entity_type');
  $audience_entity_id_column = _field_sql_storage_columnname(GCC_FIELD_AUDIENCE, 'entity_id');

  $field_enable = field_info_field(GCC_FIELD_ENABLE);
  $enable_table = _field_sql_storage_tablename($field_enable);
  $enable_uid_column = _field_sql_storage_columnname(GCC_FIELD_ENABLE, 'uid');
  $enable_value_column = _field_sql_storage_columnname(GCC_FIELD_ENABLE, 'value');

  $entity_info = entity_get_info();

  // Filter only the enabled group.
  $data[$enable_table]['group_enabled'] = array(

    'real field' => $enable_value_column,
    'group' => t('GCC'),
    'title' => t('Group Enabled'),
    'help' => t('Filter only the enabled group'),
    'filter' => array(

      'handler' => 'views_handler_filter_boolean_operator',
      'use equal' => TRUE,
      'label' => t('Filter only the enabled group'),
      'type' => 'yes-no',
      'accept null' => TRUE,
    ),
  );

  if (function_exists('gcc_context_get_context')) {

    // Filter on the current context.
    $data[$enable_table]['curent_context'] = array(

      'group' => t('GCC'),
      'title' => t('Current Context'),
      'help' => t('Filter on the current group context'),
      'filter' => array(

        'handler' => 'views_handler_filter_gcc_current_context',
        'label' => t('Filter on the current group context'),
      ),
    );
  }

  foreach ($entity_info as $entity_type => $info) {

    // Skip non GCC Entity.
    if (!isset($info['gcc'])) {
      continue;
    }

    // If the entity can be used as a group.
    if ($info['gcc']['group']) {

      // Filter only the group i'm a member of.
      $data[$entity_info[$entity_type]['base table']]['my_group_' . $entity_type] = array(

        'group' => t('GCC'),
        'title' => t('Current user Groups (@entity_type)', array('@entity_type' => $entity_info[$entity_type]['label'])),
        'help' => t('Filter only the group (@entity_type) the current user is member', array('@entity_type' => $entity_info[$entity_type]['label'])),
        'real field' => $entity_info[$entity_type]['entity keys']['id'],
        'filter' => array(

          'handler' => 'views_handler_filter_gcc_my_group',
          'entity_type' => $entity_type,
        ),
      );

      // Link the group audience field to the group.
      $data[$audience_table]['group_' . $entity_type] = array(

        'group' => t('GCC'),
        'relationship' => array(

          'title' => t('Groups from content (@entity_type)', array('@entity_type' => $entity_info[$entity_type]['label'])),
          'label' => t('Groups from content (@entity_type)', array('@entity_type' => $entity_info[$entity_type]['label'])),
          'help' => t('Relate the groups of type @entity_type this entity belongs to.', array('@entity_type' => $entity_info[$entity_type]['label'])),
          'base' => $entity_info[$entity_type]['base table'],
          'base field' => $entity_info[$entity_type]['entity keys']['id'],
          'relationship field' => $audience_entity_id_column,
          'extra' => array(
            array(
              'table' => $audience_table,
              'field' => $audience_entity_type_column,
              'value' => $entity_type,
            ),
          ),
        ),
      );

      // Link the membership to the group.
      $data['gcc_membership']['group_' . $entity_type] = array(

        'group' => t('GCC'),
        'relationship' => array(

          'title' => t('Groups from membership (@entity_type)', array('@entity_type' => $entity_info[$entity_type]['label'])),
          'label' => t('Groups from membership (@entity_type)', array('@entity_type' => $entity_info[$entity_type]['label'])),
          'help' => t('Relate the groups of type @entity_type this membership belongs to.', array('@entity_type' => $entity_info[$entity_type]['label'])),
          'base' => $entity_info[$entity_type]['base table'],
          'base field' => $entity_info[$entity_type]['entity keys']['id'],
          'relationship field' => 'entity_id',
          'extra' => array(
            array(
              'table' => 'gcc_membership',
              'field' => 'entity_type',
              'value' => $entity_type,
            ),
          ),
        ),
      );

      // Link the groups to its membership.
      $data[$entity_info[$entity_type]['base table']]['gcc_membership'] = array(

        'group' => t('GCC'),
        'relationship' => array(

          'title' => t('Group Memberships'),
          'label' => t('Group Memberships'),
          'help' => t('List the membership of this groups'),
          'base' => 'gcc_membership',
          'base field' => 'entity_id',
          'relationship field' => $entity_info[$entity_type]['entity keys']['id'],
          'extra' => array(
            array('field' => 'entity_type', 'value' => $entity_type),
          ),
        ),
      );

      // Add a reverse relationship for each possible type of group content.
      foreach ($entity_info as $content_type => $content_config) {

        // Skip non GCC Entity.
        if (!isset($content_config['gcc'])) {
          continue;
        }

        // If not a group content type, ignore.
        if (!$content_config['gcc']['group content']) {
          continue;
        }

        $data[$entity_info[$entity_type]['base table']]['gcc_groups_content_' . $content_type] = array(

          'group' => t('GCC'),
          'relationship' => array(

            'title' => t('Groups Contents from group (@entity_type from @group_type)', array('@entity_type' => $entity_info[$content_type]['label'], '@group_type' => $entity_info[$entity_type]['label'])),
            'label' => t('Groups Contents from group (@entity_type from @group_type)', array('@entity_type' => $entity_info[$content_type]['label'], '@group_type' => $entity_info[$entity_type]['label'])),
            'help' => t('Relate the groups content of type @entity_type  from group of type @group_type.', array('@entity_type' => $entity_info[$content_type]['label'], '@group_type' => $entity_info[$entity_type]['label'])),
            'handler' => 'views_handler_relationship_entity_reverse',
            'field_name' => GCC_FIELD_AUDIENCE,
            'field table' => $audience_table,
            'field field' => $audience_entity_id_column,
            'base' => $entity_info[$content_type]['base table'],
            'base field' => $entity_info[$content_type]['entity keys']['id'],
            'join_extra' => array(
              array('field' => $audience_entity_type_column, 'value' => $entity_type),
              array('field' => 'entity_type', 'value' => $content_type),
            ),
          ),
        );
      }
    }

    // If the entity can be used as a group content.
    if ($info['gcc']['group content']) {

      // Reverse link the audience field to the content entity.
      $data[$entity_info[$entity_type]['base table']]['table']['join'][$audience_table] = array(

        'left_field' => 'entity_id',
        'field' => 'nid',
        'extra' => array(
          array(
            'table' => $audience_table,
            'field' => 'entity_type',
            'value' => $entity_type,
          ),
        ),
      );
    }
  }

  // Link the user to its membership.
  $data['users']['gcc_membership'] = array(

    'group' => t('GCC'),
    'relationship' => array(

      'title' => t('User Memberships'),
      'label' => t('User Memberships'),
      'help' => t('List the membership of this user'),
      'base' => 'gcc_membership',
      'base field' => 'uid',
      'relationship field' => 'uid',
    ),
  );

  $data['users']['table']['join']['gcc_membership'] = array(

    'left_field' => 'uid',
    'field' => 'uid',
  );
}
