<?php

/**
 * @file
 * Provide views data and handlers for entity_gallery.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data().
 */
function entity_gallery_views_data() {
  // ----------------------------------------------------------------
  // entity_gallery table -- basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['entity_gallery']['table']['group'] = t('Galleries');

  // Advertise this table as a possible base table
  $data['entity_gallery']['table']['base'] = array(
    'field' => 'egid',
    'title' => t('Galleries'),
    'weight' => -10,
    'access query tag' => 'entity_gallery_access',
    'defaults' => array(
      'field' => 'title',
    ),
  );
  $data['entity_gallery']['table']['entity type'] = 'entity_gallery';

  $data['entity_gallery']['table']['default_relationship'] = array(
    'entity_gallery_revision' => array(
      'table' => 'entity_gallery_revision',
      'field' => 'vid',
    ),
  );

  // ----------------------------------------------------------------
  // entity_gallery table -- fields

  // egid
  $data['entity_gallery']['egid'] = array(
    'title' => t('EGid'),
    'help' => t('The entity gallery ID.'), // The help that appears on the UI,
    // Information for displaying the egid
    'field' => array(
      'handler' => 'views_handler_field_entity_gallery',
      'click sortable' => TRUE,
    ),
    // Information for accepting an egid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_entity_gallery_egid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'egid',
    ),
    // Information for accepting an egid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a egid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // title
  // This definition has more items in it than it needs to as an example.
  $data['entity_gallery']['title'] = array(
    'title' => t('Title'), // The item it appears as on the UI,
    'help' => t('The gallery title.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'field' => 'title', // the real field. This could be left out since it is the same.
      'group' => t('Gallery'), // The group it appears in on the UI. Could be left out.
      'handler' => 'views_handler_field_entity_gallery',
      'click sortable' => TRUE,
      'link_to_entity_gallery default' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // created field
  $data['entity_gallery']['created'] = array(
    'title' => t('Post date'), // The item it appears as on the UI,
    'help' => t('The date the gallery was posted.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // changed field
  $data['entity_gallery']['changed'] = array(
    'title' => t('Updated date'), // The item it appears as on the UI,
    'help' => t('The date the gallery was last updated.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Gallery type
  $data['entity_gallery']['type'] = array(
    'title' => t('Type'), // The item it appears as on the UI,
    'help' => t('The gallery type (for example, "file", "node", "product", etc).'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_entity_gallery_type',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_entity_gallery_type',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_entity_gallery_type',
    ),
  );

  // published status
  $data['entity_gallery']['status'] = array(
    'title' => t('Published'),
    'help' => t('Whether or not the gallery is published.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'published-notpublished' => array(t('Published'), t('Not published')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
      'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // published status + extra
  $data['entity_gallery']['status_extra'] = array(
    'title' => t('Published or admin'),
    'help' => t('Filters out unpublished galleries if the current user cannot view it.'),
    'filter' => array(
      'field' => 'status',
      'handler' => 'views_handler_filter_entity_gallery_status',
      'label' => t('Published or admin'),
    ),
  );

  // sticky
  $data['entity_gallery']['sticky'] = array(
    'title' => t('Sticky'), // The item it appears as on the UI,
    'help' => t('Whether or not the gallery is sticky.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'sticky' => array(t('Sticky'), t('Not sticky')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Sticky'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Whether or not the gallery is sticky. To list sticky galleries first, set this to descending.'),
    ),
  );

  // Define some fields based upon views_handler_field_entity in the entity
  // table so they can be re-used with other query backends.
  // @see views_handler_field_entity

  $data['views_entity_entity_gallery']['table']['group'] = t('Gallery');

  $data['entity_gallery']['view_entity_gallery']['moved to'] = array('views_entity_entity_gallery', 'view_entity_gallery');
  $data['views_entity_entity_gallery']['view_entity_gallery'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the gallery.'),
      'handler' => 'views_handler_field_entity_gallery_link',
    ),
  );

  $data['entity_gallery']['edit_entity_gallery']['moved to'] = array('views_entity_entity_gallery', 'edit_entity_gallery');
  $data['views_entity_entity_gallery']['edit_entity_gallery'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the gallery.'),
      'handler' => 'views_handler_field_entity_gallery_link_edit',
    ),
  );

  $data['entity_gallery']['delete_entity_gallery']['moved to'] = array('views_entity_entity_gallery', 'delete_entity_gallery');
  $data['views_entity_entity_gallery']['delete_entity_gallery'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the gallery.'),
      'handler' => 'views_handler_field_entity_gallery_link_delete',
    ),
  );

  $data['entity_gallery']['path'] = array(
    'field' => array(
      'title' => t('Path'),
      'help' => t('The aliased path to this gallery.'),
      'handler' => 'views_handler_field_entity_gallery_path',
    ),
  );


  // Bogus fields for aliasing purposes.

  $data['entity_gallery']['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('Date in the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_entity_gallery_created_fulldate',
    ),
  );

  $data['entity_gallery']['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('Date in the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_entity_gallery_created_year_month',
    ),
  );

  $data['entity_gallery']['created_year'] = array(
    'title' => t('Created year'),
    'help' => t('Date in the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_entity_gallery_created_year',
    ),
  );

  $data['entity_gallery']['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('Date in the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_entity_gallery_created_month',
    ),
  );

  $data['entity_gallery']['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('Date in the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_entity_gallery_created_day',
    ),
  );

  $data['entity_gallery']['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('Date in the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_entity_gallery_created_week',
    ),
  );

  $data['entity_gallery']['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('Date in the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_entity_gallery_created_fulldate',
    ),
  );

  $data['entity_gallery']['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('Date in the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_entity_gallery_created_year_month',
    ),
  );

  $data['entity_gallery']['changed_year'] = array(
    'title' => t('Updated year'),
    'help' => t('Date in the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_entity_gallery_created_year',
    ),
  );

  $data['entity_gallery']['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('Date in the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_entity_gallery_created_month',
    ),
  );

  $data['entity_gallery']['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('Date in the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_entity_gallery_created_day',
    ),
  );

  $data['entity_gallery']['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('Date in the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_entity_gallery_created_week',
    ),
  );

  // uid field
  $data['entity_gallery']['uid'] = array(
    'title' => t('Author uid'),
    'help' => t('The user authoring the gallery. If you need more fields than the uid add the gallery: author relationship'),
    'relationship' => array(
      'title' => t('Author'),
      'help' => t('Relate gallery to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('author'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  $data['entity_gallery']['uid_revision'] = array(
    'title' => t('User has a revision'),
    'help' => t('All galleries where a certain user has a revision'),
    'real field' => 'egid',
    'filter' => array(
      'handler' => 'views_handler_filter_entity_gallery_uid_revision',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_entity_gallery_uid_revision',
    ),
  );

  // ----------------------------------------------------------------------
  // Gallery revision table

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['entity_gallery_revisions']['moved to'] = 'entity_gallery_revision';
  $data['entity_gallery_revision']['table']['entity type'] = 'entity_gallery';
  $data['entity_gallery_revision']['table']['group']  = t('Gallery revision');
  // Support the conversion of the field description
  $data['entity_gallery_revisions']['entity_gallery_description']['moved to'] = array('field_revision_data', 'entity_gallery_description-revision_id');

  // Advertise this table as a possible base table
  $data['entity_gallery_revision']['table']['base'] = array(
    'field' => 'vid',
    'title' => t('Gallery revisions'),
    'help' => t('Gallery revision is a history of changes to galleries.'),
    'defaults' => array(
      'field' => 'title',
    ),
  );

  // For other base tables, explain how we join
  $data['entity_gallery_revision']['table']['join'] = array(
    // Directly links to entity_gallery table.
    'entity_gallery' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['entity_gallery_revision']['table']['default_relationship'] = array(
    'entity_gallery' => array(
      'table' => 'entity_gallery',
      'field' => 'egid',
    ),
  );

  // uid field for entity gallery revision
  $data['entity_gallery_revision']['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a gallery revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('revision user'),
    ),
  );

  // egid
  $data['entity_gallery_revision']['egid'] = array(
    'title' => t('EGid'),
    // The help that appears on the UI.
    'help' => t('The revision EGID of the gallery revision.'),
    // Information for displaying the egid.
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a egid as an argument.
    'argument' => array(
      'handler' => 'views_handler_argument_entity_gallery_egid',
      'click sortable' => TRUE,
      'numeric' => TRUE,
    ),
    // Information for accepting a egid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a egid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'entity_gallery',
      'base field' => 'egid',
      'title' => t('Gallery'),
      'label' => t('Get the actual gallery from a gallery revision.'),
    ),
  );

  // vid
  $data['entity_gallery_revision']['vid'] = array(
    'title' => t('Vid'),
    // The help that appears on the UI.
    'help' => t('The revision ID of the gallery revision.'),
    // Information for displaying the vid.
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a vid as an argument.
    'argument' => array(
      'handler' => 'views_handler_argument_entity_gallery_vid',
      'click sortable' => TRUE,
      'numeric' => TRUE,
    ),
    // Information for accepting a vid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a vid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'entity_gallery',
      'base field' => 'vid',
      'title' => t('Gallery'),
      'label' => t('Get the actual gallery from a gallery revision.'),
    ),
  );

  // title
  $data['entity_gallery_revision']['title'] = array(
    'title' => t('Title'), // The item it appears as on the UI,
    'help' => t('The gallery title.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'field' => 'title', // the real field
      'handler' => 'views_handler_field_entity_gallery_revision',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // log field
  $data['entity_gallery_revision']['log'] = array(
    'title' => t('Log message'), // The item it appears as on the UI,
    'help' => t('The log message entered when the revision was created.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_xss',
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // revision timestamp
  // changed field
  $data['entity_gallery_revision']['timestamp'] = array(
    'title' => t('Updated date'), // The item it appears as on the UI,
    'help' => t('The date the gallery was last updated.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['entity_gallery_revision']['link_to_revision'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the revision.'),
      'handler' => 'views_handler_field_entity_gallery_revision_link',
    ),
  );

  $data['entity_gallery_revision']['revert_revision'] = array(
    'field' => array(
      'title' => t('Revert link'),
      'help' => t('Provide a simple link to revert to the revision.'),
      'handler' => 'views_handler_field_entity_gallery_revision_link_revert',
    ),
  );

  $data['entity_gallery_revision']['delete_revision'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the gallery revision.'),
      'handler' => 'views_handler_field_entity_gallery_revision_link_delete',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function entity_gallery_views_plugins() {
  return array(
    'module' => 'views', // This just tells our themes are elsewhere.
    'row' => array(
      'entity_gallery' => array(
        'title' => t('Gallery'),
        'help' => t('Display the gallery with standard gallery view.'),
        'handler' => 'views_plugin_row_entity_gallery_view',
        'path' => drupal_get_path('module', 'entity_gallery') . '/views', // not necessary for most modules
        'base' => array('entity_gallery'), // only works with 'entity_gallery' as base.
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-entity-gallery',
      ),
    ),
    'argument validator' => array(
      'entity_gallery' => array(
        'title' => t('Gallery'),
        'handler' => 'views_plugin_argument_validate_entity_gallery',
      ),
    ),
    'argument default' => array(
      'entity_gallery' => array(
        'title' => t('Gallery ID from URL'),
        'handler' => 'views_plugin_argument_default_entity_gallery'
      ),
    ),
  );
}

/**
 * Implements hook_preprocess_entity_gallery().
 */
function entity_gallery_row_entity_gallery_view_preprocess_entity_gallery(&$vars) {
  $entity_gallery = $vars['entity_gallery'];
  $options = $vars['view']->style_plugin->row_plugin->options;

  if (!$options['links']) {
    unset($vars['content']['links']);
  }
}

/**
 * Implements hook_views_query_substitutions().
 */
function entity_gallery_views_query_substitutions() {
  return array(
    '***ADMINISTER_ENTITY_GALLERIES***' => intval(user_access('administer entity galleries')),
    '***VIEW_OWN_UNPUBLISHED_ENTITY_GALLERIES***' => intval(user_access('view own unpublished entity galleries')),
    '***BYPASS_ENTITY_GALLERY_ACCESS***' =>  intval(user_access('bypass entity gallery access')),
  );
}

/**
 * Implements hook_views_analyze().
 */
function entity_gallery_views_analyze($view) {
  $ret = array();
  // Check for something other than the default display:
  if ($view->base_table == 'entity_gallery') {
    foreach ($view->display as $id => $display) {
      if (empty($display->handler)) {
        continue;
      }
      if (!$display->handler->is_defaulted('access') || !$display->handler->is_defaulted('filters')) {
        // check for no access control
        $access = $display->handler->get_option('access');
        if (empty($access['type']) || $access['type'] == 'none') {
          $select = db_select('role', 'r');
          $select->innerJoin('role_permission', 'p', 'r.rid = p.rid');
          $result = $select->fields('r', array('name'))
            ->fields('p', array('permission'))
            ->condition('r.name', array('anonymous user', 'authenticated user'), 'IN')
            ->condition('p.permission', 'access entity galleries')
            ->execute();

          foreach ($result as $role) {
            $role->safe = TRUE;
            $roles[$role->name] = $role;
          }
          if (!($roles['anonymous user']->safe && $roles['authenticated user']->safe)) {
            $ret[] = views_ui_analysis(t('Some roles lack permission to access galleries, but display %display has no access control.', array('%display' => $display->display_title)), 'warning');
          }
          $filters = $display->handler->get_option('filters');
          foreach ($filters as $filter) {
            if ($filter['table'] == 'entity_gallery' && ($filter['field'] == 'status' || $filter['field'] == 'status_extra')) {
              continue 2;
            }
          }
          $ret[] = views_ui_analysis(t('Display %display has no access control but does not contain a filter for published entity galleries.', array('%display' => $display->display_title)), 'warning');
        }
      }
    }
  }
  foreach ($view->display as $id => $display) {
    if ($display->display_plugin == 'page') {
      if ($display->handler->get_option('path') == 'gallery/%') {
        $ret[] = views_ui_analysis(t('Display %display has set gallery/% as path. This will not produce what you want. If you want to have multiple versions of the gallery view, use panels.', array('%display' => $display->display_title)), 'warning');
      }
    }
  }

  return $ret;
}
