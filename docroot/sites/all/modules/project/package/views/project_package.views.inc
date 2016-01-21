<?php

/**
 * @file
 * Implement Views hooks to expose project_package data to views.
 */

/**
 * Implement hook_views_data().
 */
function project_package_views_data() {
  $data = array();

  // Fields for {project_package_local_release_item}

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this group by default.
  $data['project_package_local_release_item']['table']['group']  = t('Project package');

  // Allow implicit relationship to {node} for the items in a package. To work
  // with the package_nid, you should add an explicit relationship.
  $data['project_package_local_release_item']['table']['join'] = array(
    'node' => array(
      'type' => 'INNER',
      'left_field' => 'nid',
      'field' => 'item_nid',
    ),
  );

  // package_nid
  $data['project_package_local_release_item']['package_nid'] = array(
    'title' => t('Package node'),
    'help' => t('The node ID of the package release node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the package release.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Package release node'),
    ),
  );

  // item_nid
  $data['project_package_local_release_item']['item_nid'] = array(
    'title' => t('Item node'),
    'help' => t('The node ID of an item release node included in a package.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the item releases.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Item release node'),
    ),
  );

  $data['project_package_local_release_item']['patch_count'] = array(
    'title' => t('Item patch count'),
    'help' => t('The number of patches applied to a given item release node included in a package.'),
    'real field' => 'item_nid',
    'field' => array(
      'handler' => 'project_package_handler_field_local_patch_count',
    ),
  );

  // Fields for {project_package_remote_item}

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this group by default.
  $data['project_package_remote_item']['table']['group']  = t('Project package remote items');

  // Advertise this table as a possible base table
  $data['project_package_remote_item']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Package remote items'),
    'help' => t("Remote items (or libraries) included in a release package."),
  );

  // package_nid
  $data['project_package_remote_item']['package_nid'] = array(
    'title' => t('Package node'),
    'help' => t('The node ID of the package release node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the package release.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Package release node'),
    ),
  );

  // name
  $data['project_package_remote_item']['name'] = array(
    'title' => t('Remote item name'),
    'help' => t('The name of the remote item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // url
  $data['project_package_remote_item']['url'] = array(
    'title' => t('Remote item URL'),
    'help' => t('The URL of the remote item.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Fields for {project_package_local_patch}

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this group by default.
  $data['project_package_local_patch']['table']['group']  = t('Project package local patches');

  // Allow implicit relationship to {node} for the local patches in a package.
  // To work with the package_nid, you should add an explicit relationship.
  $data['project_package_local_patch']['table']['join'] = array(
    'node' => array(
      'type' => 'INNER',
      'left_field' => 'nid',
      'field' => 'item_nid',
    ),
  );

  // package_nid
  $data['project_package_local_patch']['package_nid'] = array(
    'title' => t('Package node'),
    'help' => t('The node ID of the package release node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the package release.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Package release node'),
    ),
  );

  // item_nid
  $data['project_package_local_patch']['item_nid'] = array(
    'title' => t('Item node'),
    'help' => t('The node ID of an item release node patched in a package.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the item releases.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Item release node'),
    ),
  );

  // patch_file_url
  $data['project_package_local_patch']['patch_file_url'] = array(
    'title' => t('Patch URL'),
    'help' => t('The URL of the patch file.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // patch_description_url
  $data['project_package_local_patch']['patch_description_url'] = array(
    'title' => t('Patch description URL'),
    'help' => t('The URL that describes the patch file.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // patch_nid
  $data['project_package_local_patch']['patch_nid'] = array(
    'title' => t('Patch node'),
    'help' => t('The node ID associated with the patch file.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the node associated with the patch file.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Patch file node'),
    ),
  );

  // patch_comment_id
  $data['project_package_local_patch']['patch_comment_id'] = array(
    'title' => t('Patch comment ID'),
    'help' => t('The comment ID associated with the patch file.'),
    'field' => array(
      'handler' => 'views_handler_field_comment',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Fields for {project_package_remote_patch}

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this group by default.
  $data['project_package_remote_patch']['table']['group']  = t('Project package remote patches');

  // Allow implicit relationship to {node} for the remote patches in a package.
  // To work with the package_nid, you should add an explicit relationship.
  $data['project_package_remote_patch']['table']['join'] = array(
    'project_package_remote_item' => array(
      'type' => 'INNER',
      'left_field' => 'id',
      'field' => 'remote_id',
    ),
  );

  // package_nid
  $data['project_package_remote_patch']['package_nid'] = array(
    'title' => t('Package node'),
    'help' => t('The node ID of the package release node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the package release.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Package release node'),
    ),
  );

  // remote_id
  $data['project_package_remote_patch']['remote_id'] = array(
    'title' => t('Remote item'),
    'help' => t('The remote item ID of an item patched in a package.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the remote item.'),
      'base' => 'project_package_remote_item',
      'handler' => 'views_handler_relationship',
      'label' => t('Remote item'),
    ),
  );

  // patch_file_url
  $data['project_package_remote_patch']['patch_file_url'] = array(
    'title' => t('Patch URL'),
    'help' => t('The URL of the patch file.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // patch_description_url
  $data['project_package_remote_patch']['patch_description_url'] = array(
    'title' => t('Patch description URL'),
    'help' => t('The URL that describes the patch file.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // patch_nid
  $data['project_package_remote_patch']['patch_nid'] = array(
    'title' => t('Patch node'),
    'help' => t('The node ID associated with the patch file.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the node associated with the patch file.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Patch file node'),
    ),
  );

  // patch_comment_id
  $data['project_package_remote_patch']['patch_comment_id'] = array(
    'title' => t('Patch comment ID'),
    'help' => t('The comment ID associated with the patch file.'),
    'field' => array(
      'handler' => 'views_handler_field_comment',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function project_package_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'project_package') .'/views/handlers',
    ),
    'handlers' => array(
      'project_package_handler_field_local_patch_count' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
