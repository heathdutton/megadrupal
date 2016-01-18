<?php
/**
 * @file party.views.inc
 * Contains Views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function party_views_plugins() {
  return array(
    'display' => array(
      'party_piece' => array(
        'title' => t('Party component'),
        'help' => t("Display the view as a component for CRM parties."),
        'handler' => 'party_plugin_display_party_piece',
        'theme' => 'views_view',
        'contextual links locations' => array(),
        //'theme' => 'views_attach_display_node_content_view',
        //'theme file' => 'views_attach.theme.inc',
        //'theme path' => drupal_get_path('module', 'views_attach'),
        //'uses hook nodeapi' => TRUE,
        'uses hook party_party_pieces' => TRUE,
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'use more' => FALSE,
        'accept attachments' => TRUE,
        'admin' => t('Party component'),
        //'help topic' => 'display-node-content',
      ),
    ),
    'row' => array(
      'party_attached_entity' => array(
        'title' => t('Party attached entity'),
        'help' => t('Display entities attached to a party with their default display.'),
        'handler' => 'party_plugin_row_attached_entity',
        'uses fields' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
        //'theme' => 'views_view_row_node', // @todo: needed?
        'base' => array('party'), // Only works with party base.
        ),
     ),
  );
}

/**
 * Implements hook_views_data().
 *
 * We can implement this *and* get entity module's views data for free because
 * we set our entity's 'views controller class' in hook_entity_info().
 */
function party_views_data() {
  $data = array();

  // Some field handlers for views
  // Delete Link
  // @todo: use entity properties?
  $data['party']['delete_party'] = array(
    'title' => t('Delete Link'),
    'help' => t('Provide a link to delete the party.'),
    'field' => array(
      'real field' => 'pid',
      'handler' => 'party_handler_field_delete_link',
    ),
  );

  // The unified attached entity table.
  // @todo: put this into its own group?
  $data['party_attached_entity']['table']['group'] = t('Party');
  $data['party_attached_entity']['table']['join'] = array(
    // Join to party base table.
    'party' => array(
      'left_field' => 'pid',
      'field' => 'pid',
    ),
  );
  // The eid field provides a relationship to the base table for the entity
  // involved in this data set.
  $data['party_attached_entity']['eid'] = array(
    // @todo: give the entity type here as well as the set name?
    'title' => t('Attached entity Id'),
    // The attached entity field produces multiple rows per party, one per
    // attached entity. Use this with the 'party_attached_entity' row plugin
    // for displays showing a single party.
    'field' => array(
      'label' => t('Attached entity Id'),
      'help' => t('The id of attached entities. Will produce multiple rows per party.'),
      'handler' => 'views_handler_field_numeric',
    ),
    // The attached entity relationship can't be added here, as the base of a
    // relationship cannot be dynamic.
  );

  $data['party_attached_entity']['data_set'] = array(
    'title' => t('Attached entity data set'),
    'field' => array(
      'label' => t('Attached entity data set'),
      'help' => t('The data set of attached entities. Will produce multiple rows per party.'),
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'label' => t('Attached entity data set'),
      'help' => t('The data set of attached entities.'),
      'handler' => 'party_handler_filter_party_attached_entity_data_set',
    ),
  );

  $data['party_attached_entity']['entity_type'] = array(
    'title' => t('Attached entity type'),
    'field' => array(
      'label' => t('Attached entity type'),
      'help' => t('The type of attached entities. Will produce multiple rows per party.'),
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'label' => t('Attached entity type'),
      'help' => t('The type of attached entities.'),
      'handler' => 'party_handler_filter_party_attached_entity_type',
    ),
  );

  $data['party_attached_entity']['entity_bundle'] = array(
    'title' => t('Attached entity bundle'),
    'field' => array(
      'label' => t('Attached entity bundle'),
      'help' => t('The bundle type of attached entities. Will produce multiple rows per party.'),
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'label' => t('Attached entity bundle'),
      'help' => t('The bundle of attached entities.'),
      'handler' => 'party_handler_filter_party_attached_entity_bundle',
    ),
  );

  $data['party_attached_entity']['delta'] = array(
    'title' => t('Attached entity delta'),
    'field' => array(
      'label' => t('Attached entity delta'),
      'help' => t('The delta of attached entities. Will produce multiple rows per party.'),
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'label' => t('Attached entity delta'),
      'help' => t('The delta of attached entities.'),
      'handler' => 'views_handler_field_numeric',
    ),
  );

  // Add relationships to attached entities.
  // Get data set and entity info.
  $sets = party_get_data_set_info();
  $entity_info = entity_get_info();

  // Group data sets by entity type.
  $data_sets_by_entity = array();
  foreach ($sets as $set_name => $set) {
    $data_sets_by_entity[$set['entity type']][$set_name] = $set;
  }

  foreach ($data_sets_by_entity as $entity_type => $data_sets) {
    // We don't want to let relationships to entities with no base table.
    if (empty($entity_info[$entity_type]['base table'])) {
      continue;
    }

    // For each entity, add a fake field based on the 'eid' field.
    $data['party_attached_entity']['eid_' . $entity_type] = array(
      'title' => t('Attached !entity-type', array(
        '!entity-type' => $entity_info[$entity_type]['label'],
      )),
      // Give the real field for this fake field.
      'real field' => 'eid',
      'relationship' => array(
        'label' => t('Attached !entity-type', array(
          '!entity-type' => $entity_info[$entity_type]['label'],
        )),
        'help' => t('Relates a party to entities within particular data sets.'),
        'handler' => 'party_handler_relationship_party_attached_eid',
        // The base has to be set here and can't be dynamic :(
        'base' => $entity_info[$entity_type]['base table'],
        // This allows us to not show this relationship if the base is already
        // the right hand side so users won't create circular relationships.
        // @todo: figure this out:
        //'skip base' => array($entity_right_join_info['base table']),
        // Some things for our handler to find:
        'entity_type' => $entity_type,
      ),
    );

    // Set up the reverse relationship.
    $data[$entity_info[$entity_type]['base table']]['party_attached'] = array(
      'title' => t('Party the !entity-type is attached to', array(
        '!entity-type' => $entity_info[$entity_type]['label'],
      )),
      // Give the real field for this fake field.
      'relationship' => array(
        'label' => t('Party the !entity-type is attached to', array(
          '!entity-type' => $entity_info[$entity_type]['label'],
        )),
        'help' => t('Relates to a party from the attached entity.'),
        'handler' => 'party_handler_relationship_party_from_attached_entity',
        'base' => 'party',
        'base field' => 'pid',
        'entity_type' => $entity_type,
      ),
    );
  }

  return $data;
}
