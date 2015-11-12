<?php

/**
 * @file
 * Views hooks go here.
 */

/**
 * hook_views_data().
 */
function architecture_views_data() {

  $data = array();

  /**
   * Menu info for nodetypes
   */
  $data['menu_links'] =  array (
    'link_title' => array(
      'title' => t('Link title'),
      'help' => t('Title of the link.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'menu_name' => array(
      'title' => t('Menu title'),
      'help' => t('Title of the menu.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'router_path' => array(
      'title' => t('Router path'),
      'help' => t('Router path of the link.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
  );

  /**
   * Nodetype info
   */
  $data['node_type'] =  array(
    'table' => array(
      'base' => array(
        'field' => 'type',
        'title' => t('Nodetypes'),
        'help' => t(''),
        'weight' => 0,
      ),
    ),
    'type' => array(
      'title' => t('Nodetype machine name'),
      'help' => t('Machine name of the node type.'),
      'group' => 'Nodetype',
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'name' => array(
      'title' => t('Nodetype Name'),
      'help' => t('Human-readable name of the node type.'),
      'group' => 'Nodetype',
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'module' => array(
      'title' => t('Nodetype Module'),
      'help' => t('Module that defines the node type.'),
      'group' => 'Nodetype',
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'description' => array(
      'title' => t('Nodetype Description'),
      'help' => t('Description of the node type.'),
      'group' => 'Nodetype',
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'help' => array(
      'title' => t('Nodetype Help'),
      'help' => t('Help text of the node type.'),
      'group' => 'Nodetype',
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'title_label' => array(
      'title' => t('Nodetype Title Label'),
      'help' => t('Title label of the node type.'),
      'group' => 'Nodetype',
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'nodetype_menus' => array(
      'real field' => 'type',
      'group' => t('Menus'),
      'title' => t('Nodetype menus'),
      'help' => t('Menus available for this nodetype.'),
      'field' => array(
        'handler' => 'architecture_nodetype_menus_views_handler',
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
    ),
    'nodetype_options' => array(
      'real field' => 'type',
      'group' => t('Nodetype'),
      'title' => t('Nodetype options'),
      'help' => t('Options settings for this nodetype.'),
      'field' => array(
        'handler' => 'architecture_nodetype_options_views_handler',
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
    ),
    'nodetype_submitted' => array(
      'real field' => 'type',
      'group' => t('Nodetype'),
      'title' => t('Nodetype submitted'),
      'help' => t('Submission settings for this nodetype.'),
      'field' => array(
        'handler' => 'architecture_nodetype_submitted_views_handler',
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
    ),
    'nodetype_preview' => array(
      'real field' => 'type',
      'group' => t('Nodetype'),
      'title' => t('Nodetype preview'),
      'help' => t('Preview settings for this nodetype.'),
      'field' => array(
        'handler' => 'architecture_nodetype_preview_views_handler',
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
    ),
  );

  /**
   * Menus
   */
  if(module_exists('menu')) {
    $data['node_type']['nodetype_menu_parent'] = array(
      'real field' => 'type',
      'group' => t('Parent menu'),
      'title' => t('Nodetype parent menu item'),
      'help' => t('Parent item in menu for this nodetype.'),
      'field' => array(
        'handler' => 'architecture_nodetype_menu_parent_views_handler',
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
  }

  /**
   * Blocks
   */
  if(module_exists('block')) {
      $data['block'] = array(
      'table' => array(
        'base' => array(
          'field' => 'block',
          'title' => t('Blocks'),
          'help' => t(''),
          'weight' => 0,
        ),
      ),
      'delta' => array(
        'title' => t('Delta'),
        'group' => t('Block'),
        'help' => t('Unique name of the block.'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
      'theme' => array(
        'title' => t('Theme'),
        'group' => t('Block'),
        'help' => t('Theme for which these settings apply.'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
      'status' => array(
        'title' => t('Status'),
        'group' => t('Block'),
        'help' => t('Block enabled status.'),
        'field' => array(
          'handler' => 'architecture_block_status_views_handler',
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
      ),
      'region' => array(
        'title' => t('Region'),
        'group' => t('Block'),
        'help' => t('Region in which the block appears.'),
        'field' => array(
          'handler' => 'architecture_block_region_views_handler',
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
      ),
      'module' => array(
        'title' => t('Module'),
        'group' => t('Block'),
        'help' => t('Module which declares the block.'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
    );

    /**
     * block_custom info
     */

    $data['block_custom'] = array(
      'table' => array(
        'base' => array(
          'field' => 'block_custom',
          'title' => t('Custom block contents'),
          'help' => t('Blocks with custom contents store those contents separately from the Block table.'),
          'weight' => 0,
        ),
      ),
      'body' => array(
        'title' => t('body'),
        'group' => t('Block'),
        'help' => t('The body of this custom block.'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
      'format' => array(
        'title' => t('format'),
        'group' => t('Block'),
        'help' => t('The format of this custom block.'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
      'php' => array(
        'title' => t('PHP'),
        'real field' => 'body',
        'group' => t('Block'),
        'help' => t('Whether this block contains inline PHP.'),
        'field' => array(
          'handler' => 'architecture_php_views_handler',
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
      ),
    );

    // Join block_custom to block.
    $data['block_custom']['table']['join']['block'] = array(
      'left_field' => 'bid',
      'field' => 'bid',
    );

  }
  /**
   * Field base info
   */
  $data['field_config'] = array(
    'table' => array(
      'base' => array(
        'field' => 'id',
        'title' => t('Fields'),
        'help' => t('Gives you the lay of the land of your fields.'),
        'weight' => 0,
      ),
    ),
    'field_name' => array(
      'title' => t('Field Name'),
      'group' => t('Field base'),
      'help' => t('Name of the field.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'field_contains_php' => array(
      'title' => t('Field PHP'),
      'group' => t('Field base'),
      'real field' => 'field_name',
      'help' => t('Whether any instance of the field contains PHP.'),
      'field' => array(
        'handler' => 'architecture_php_fields_views_handler',
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
    ),
    'type' => array(
      'group' => t('Field base'),
      'title' => t('Field type'),
      'help' => t('Type of the field.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'module' => array(
      'group' => t('Field base'),
      'title' => t('Module'),
      'help' => t('Module that supplies the field.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'id' => array(
      'group' => t('Field base'),
      'title' => t('ID'),
      'help' => t('Field ID.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
  );

  /**
   * Field instance data
   */
  $data['field_config_instance'] = array(
    'field_id' => array(
      'group' => t('Field instance'),
      'title' => t('Field ID'),
      'help' => t('The ID of the field in the field_config_instance table.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'bundle' => array(
      'group' => t('Field instance'),
      'title' => t('Bundle'),
      'help' => t('The bundle (content type) on which this field appears.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'data' => array(
      'group' => t('Field instance'),
      'title' => t('Field instance data'),
      'help' => t('Serialized data from the field instance settings.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'field_label' => array(
      'real field' => 'data',
      'group' => t('Field instance'),
      'title' => t('Label'),
      'help' => t('Human-readable label of the field instance.'),
      'field' => array(
        'handler' => 'architecture_field_instance_label_views_handler',
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
    ),
    'field_description' => array(
      'real field' => 'data',
      'group' => t('Field instance'),
      'title' => t('Description'),
      'help' => t('Human-readable description of the field instance.'),
      'field' => array(
        'handler' => 'architecture_field_instance_description_views_handler',
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
    ),
    'field_required' => array(
      'real field' => 'data',
      'group' => t('Field instance'),
      'title' => t('Required'),
      'help' => t('Whether the field is required on this bundle'),
      'field' => array(
        'handler' => 'architecture_field_instance_required_views_handler',
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
    ),
  );

  /**
   * Page manager and Panels data
   */
  if(module_exists('page_manager')) {
    $data['page_manager_pages']['table']['group'] = t('Page Manager data');
    $data['page_manager_pages']['table']['base'] = array(
      'title' => t('Page Manager data'),
      'help' => t('Query for Page Manager data'),
      'query class' => 'architecture_pm_data_plugin_query',
    );
    $data['page_manager_pages']['title'] = array(
      'title' => t('Title'),
      'group' => t('Page manager'),
      'help' => t('The title of this Page Manager data'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['description'] = array(
      'title' => t('Description'),
      'group' => t('Page manager'),
      'help' => t('The description of this Page Manager data'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['enabled'] = array(
      'title' => t('Enabled'),
      'group' => t('Page manager'),
      'help' => t('The enabled state of this Page Manager data'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['storage'] = array(
      'title' => t('Storage'),
      'group' => t('Page manager'),
      'help' => t('The storage state of this Page Manager data'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['type'] = array(
      'title' => t('Type'),
      'group' => t('Page manager'),
      'help' => t('The type of this Page Manager data'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['path'] = array(
      'title' => t('Path'),
      'group' => t('Page manager'),
      'help' => t('The path of this page'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['handler'] = array(
      'title' => t('Handler'),
      'group' => t('Page manager'),
      'help' => t('The handler of this page'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['handler_title'] = array(
      'title' => t('Handler Title'),
      'group' => t('Page manager'),
      'help' => t('The handler title of this page'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['handler_admin_title'] = array(
      'title' => t('Handler Admin Title'),
      'group' => t('Page manager'),
      'help' => t('The handler admin title of this page'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['handler_body'] = array(
      'title' => t('Handler Body'),
      'group' => t('Page manager'),
      'help' => t('The handler body of this page'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['handler_subtype'] = array(
      'title' => t('Handler Subtype'),
      'group' => t('Page manager'),
      'help' => t('The handler subtype of this page'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
    $data['page_manager_pages']['handler_panel'] = array(
      'title' => t('Handler Panel'),
      'group' => t('Page manager'),
      'help' => t('The handler panel'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    );
  }

  /**
   * Nodequeues
   */

  // Nodequeue base table
  $data['nodequeue_queue'] =  array (
    'table' => array(
      'base' => array(
        'field' => 'qid',
        'title' => t('Nodequeues'),
        'help' => t('Shows info related to the nodequeues.'),
        'weight' => 0,
      ),
      'name' => array(
        'title' => t('Nodequeue machine name'),
        'group' => t('Nodequeue'),
        'help' => t('The machine name of this nodequeue'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
      'title' => array(
        'title' => t('Nodequeue title'),
        'group' => t('Nodequeue'),
        'help' => t('The title of this nodequeue'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
      'subqueue_title' => array(
        'title' => t('Subqueue title'),
        'group' => t('Nodequeue'),
        'help' => t('The subqueue of this nodequeue'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
      'qid' => array(
        'title' => t('Queue ID'),
        'group' => t('Nodequeue'),
        'help' => t('The ID of this nodequeue'),
        'field' => array(
          'handler' => 'views_handler_field',
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
      ),
    ),
  );

  // Nodequeue types
  $data['nodequeue_types'] =  array (
    'type' => array(
      'title' => t('Nodequeue type'),
      'group' => t('Nodequeue'),
      'help' => t('The types available to this nodequeue'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
  );

  // Nodequeue nodes
  $data['nodequeue_nodes'] = array (
    'nid' => array(
      'title' => t('Node ID'),
      'group' => t('Nodequeue'),
      'help' => t('The node ID'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
  );

  /**
   * User roles
   */

  // Roles table
  $data['role'] = array(
    'table' => array(
      'base' => array(
        'field' => 'rid',
        'title' => t('Roles'),
        'help' => t('Shows info related to user roles.'),
        'weight' => 0,
      ),
    ),
    'rid' => array(
      'title' => t('Role ID'),
      'group' => t('Roles'),
      'help' => t('The ID of this role'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'name' => array(
      'title' => t('Role name'),
      'group' => t('Roles'),
      'help' => t('The name of this role'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'user_count' => array(
      'title' => t('User count'),
      'real field' => 'rid',
      'group' => t('Roles'),
      'help' => t('The number of users with this role'),
      'field' => array(
        'handler' => 'architecture_role_users_views_handler',
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
    ),
  );

  // Role_permissions table
  $data['role_permission'] = array(
    'table' => array(
      'base' => array(
        'field' => 'rid',
        'title' => t('Role permissions'),
        'help' => t('Shows info related to user roles.'),
        'weight' => 0,
      ),
    ),
    'permission' => array(
      'title' => t('Role permission '),
      'group' => t('Role permissions'),
      'help' => t('Role permissions'),
      'field' => array(
        'handler' => 'architecture_role_permissions_views_handler',
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
    ),
    'module' => array(
      'title' => t('Role permission module'),
      'group' => t('Role permissions'),
      'help' => t('The module that declared these permissions'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
  );

  if(module_exists('taxonomy')) {
    /**
     * Taxonomy stuff
     */
    $data['taxonomy_term_data'] = array(
      'nodetypes' => array(
        'title' => t('Appears on (nodetypes)'),
        'real field' => 'vid',
        'group' => t('Taxonomy Vocabulary'),
        'help' => t('List of nodetypes that have a field that takes terms from this vocabulary'),
        'field' => array(
          'handler' => 'architecture_taxonomy_nodetype_views_handler',
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
      ),
    );
  }

  /*
   * Variables
   */
  $data['variable'] = array(
    'table' => array(
      'base' => array(
        'field' => 'name',
        'title' => t('Variables'),
        'help' => t('Shows variables.'),
        'weight' => 0,
      ),
    ),
    'name' => array(
      'title' => t('Variable name'),
      'group' => t('Variables'),
      'help' => t('The name of this variable'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    ),
    'value' => array(
      'title' => t('Variable value'),
      'group' => t('Variables'),
      'help' => t('The value of this variable'),
      'field' => array(
        'handler' => 'architecture_variable_value_views_handler',
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
    ),
  );

/**
 * Images
 */
  $data['imagestyles']['table']['group'] = t('Imagestyles');
  $data['imagestyles']['table']['base'] = array(
    'title' => t('Image styles data'),
    'help' => t('Query for Imagestyles data'),
    'query class' => 'architecture_imagestyles_plugin_query',
  );
  $data['imagestyles']['title'] = array(
    'title' => t('Title'),
    'group' => t('Imagestyles'),
    'help' => t('The title of this Image style'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['imagestyles']['effects'] = array(
    'title' => t('Effects'),
    'group' => t('Imagestyles'),
    'help' => t('The effects of this Image style'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['imagestyles']['settings'] = array(
    'title' => t('Settings'),
    'group' => t('Imagestyles'),
    'help' => t('The settings of this Image style'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['imagestyles']['effect_weight'] = array(
    'title' => t('Weight'),
    'group' => t('Imagestyles'),
    'help' => t('The weight of this Image style effect'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['imagestyles']['effect_desc'] = array(
    'title' => t('Description'),
    'group' => t('Imagestyles'),
    'help' => t('The description of this Image style effect'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['imagestyles']['preview'] = array(
    'title' => t('Preview'),
    'group' => t('Imagestyles'),
    'help' => t('The preview of this Image style'),
    'field' => array(
      'handler' => 'architecture_raw_views_handler',
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

  /**
   * Views
   */
  $data['views']['table']['group'] = t('Views');
  $data['views']['table']['base'] = array(
    'title' => t('Views data'),
    'help' => t('Query for Views data'),
    'query class' => 'architecture_views_data_plugin_query',
  );
  $data['views']['title'] = array(
    'title' => t('Title'),
    'group' => t('Views'),
    'help' => t('The title of this View'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['display_title'] = array(
    'title' => t('Display'),
    'group' => t('Views'),
    'help' => t('The title of this Views display'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['enabled'] = array(
    'title' => t('Status'),
    'group' => t('Views'),
    'help' => t('Is this View enabled?'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['description'] = array(
    'title' => t('Description'),
    'group' => t('Views'),
    'help' => t('The description of the View'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['access'] = array(
    'title' => t('Access'),
    'group' => t('Views'),
    'help' => t('The access rules of the display'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['base_table'] = array(
    'title' => t('Table'),
    'group' => t('Views'),
    'help' => t('The base table of the view'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['tag'] = array(
    'title' => t('Tag'),
    'group' => t('Views'),
    'help' => t('The tag of the view'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['storage'] = array(
    'title' => t('Storage'),
    'group' => t('Views'),
    'help' => t('Where this view lives'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['machine_name'] = array(
    'title' => t('Machine name'),
    'group' => t('Views'),
    'help' => t('The machine name of the view'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['view_query'] = array(
    'title' => t('Query'),
    'group' => t('Views'),
    'help' => t('The query run by the view'),
    'field' => array(
      'handler' => 'architecture_query_views_handler',
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
  $data['views']['path'] = array(
    'title' => t('path'),
    'group' => t('Views'),
    'help' => t('The path of the view'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  $data['views']['fields'] = array(
    'title' => t('Fields'),
    'group' => t('Views'),
    'help' => t('The fields of the display'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  /**
   * Table joins
   */

  // Join node_type to field_config via field_config_instance.
  $data['node_type']['table']['join']['field_config'] = array(
    'left_table' => 'field_config_instance',
    'left_field' => 'bundle',
    'field' => 'type',
  );

  // Join field_config_instance to field_config.
  $data['field_config_instance']['table']['join']['field_config'] = array(
    'left_field' => 'id',
    'field' => 'field_id',
  );

  // Join nodequeue_types to nodequeue_queue.
  $data['nodequeue_types']['table']['join']['nodequeue_queue'] = array(
    'left_field' => 'qid',
    'field' => 'qid',
  );

  // Join nodequeue_nodes to nodequeue_queue.
  $data['nodequeue_nodes']['table']['join']['nodequeue_queue'] = array(
    'left_field' => 'qid',
    'field' => 'qid',
  );

  // Join role permissions to role.
  $data['role_permission']['table']['join']['role'] = array(
    'left_field' => 'rid',
    'field' => 'rid',
  );

  // Join node to nodetype.
  $data['node']['table']['join']['node_type'] = array(
    'left_field' => 'type',
    'field' => 'type',
  );

  return $data;
}
