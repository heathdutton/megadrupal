<?php

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _xc_search_menu() {
  // PUBLIC PART
  $items['xc/search'] = array(
    'title callback' => 'xc_search_search',
    'title arguments' => array('name', TRUE),
    'page callback' => 'xc_search_view',
    'page arguments' => array('xc_search'),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'xc_search.pages.inc',
  );

  $items['xc/search/%'] = array(
    'title callback' => 'xc_search_search',
    'title arguments' => array('name', TRUE),
    'page callback' => 'xc_search_view',
    'page arguments' => array('xc_search'),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'xc_search.pages.inc',
  );

  // ADMIN PART
  $items['admin/xc/search'] = array(
    'title' => 'Search Interface Setup',
    'description' => 'Indexing, facets, display templates, highlighting.  This menu enables you to build a search interface: create or modify facets, configure the highlighter, more like this, and display templates for brief view, and full record view for each type of metadata format.',
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
    'page callback' => 'system_admin_menu_block_page',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 4,
  );
  $items['xc_search/ajax/image_info'] = array(
    'title' => 'XC Search image info Ajax gateway',
    'description' => 'XC Search image info Ajax gateway',
    'page callback' => 'xc_search_ajax_image_info',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );
  $items['xc_search/ajax/ncip_info'] = array(
    'title' => 'XC Search NCIP info Ajax gateway',
    'description' => 'XC Search NCIP info Ajax gateway',
    'page callback' => 'xc_search_ajax_ncip_info',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );
  $items['xc_search/ajax/ncip_info_full'] = array(
    'title' => 'XC Search NCIP info Ajax gateway',
    'description' => 'XC Search NCIP info Ajax gateway',
    'page callback' => 'xc_search_ajax_ncip_info_full',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );
  $items['xc_search/ajax/bookmark_item'] = array(
    'title' => 'Save bookmark Ajax gateway',
    'description' => 'Save bookmark Ajax gateway',
    'page callback' => 'xc_search_ajax_bookmark_item',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );
  $items['xc_search/ajax/remove_bookmark_item'] = array(
    'title' => 'Remove bookmark Ajax gateway',
    'description' => 'Remove bookmark Ajax gateway',
    'page callback' => 'xc_search_ajax_remove_bookmark_item',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );
  $items['xc_search/ajax/open_worldcat_identity_url'] = array(
    'title' => 'Save item Ajax gateway',
    'description' => 'Save item Ajax gateway',
    'page callback' => 'xc_search_ajax_open_worldcat_identity_url',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );
  $items['xc_search/ajax/more_facets'] = array(
    'title' => 'More facets Ajax gateway',
    'description' => 'List more facets through Ajax',
    'page callback' => 'xc_search_ajax_more_facet',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );
  $items['xc_search/ajax/xc_synonyms'] = array(
    'title' => 'Get synonyms Ajax gateway',
    'description' => 'List synonyms through Ajax',
    'page callback' => 'xc_search_ajax_synonyms',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );
  $items['xc_search/ajax/xc_suggestions'] = array(
    'title' => 'Get suggestions Ajax gateway',
    'description' => 'List suggestions through Ajax',
    'page callback' => 'xc_search_ajax_suggestions',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );

  $items['xc_search/autocomplete'] = array(
    'title' => 'Search autocomplete',
    'page callback' => 'xc_search_autocomplete',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'searching/xc_search_ajax.inc',
  );

  // TODO: check what is this
  $items['admin/xc/search/restore_defaults'] = array(
    'title' => 'Restore default settings',
    'description' => 'Restore default settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_restore_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
  );
  // this is temporary thing, and use in the case the MySQL in not sync
  // with Solr, so the node is missing
  $items['admin/xc/search/full/%xc_search_solr_doc'] = array(
    'title' => 'XC Search full',
    'description' => 'Testing page for full record display',
    'page callback' => 'xc_search_full',
    'page arguments' => array(4),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  );

  $items['xc_search/more_facets'] = array(
    'title' => 'More facets',
    'description' => 'List more facets',
    'page callback' => 'xc_search_show_more_facets',
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  );

  $items['admin/xc/search/ui'] = array(
    'title' => 'Search User Interface',
    'description' => 'Add a new search interface (search page and associated results page) or edit existing search interfaces',
    'page callback' => 'xc_search_ui_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 1,
    'file' => 'admin/xc_search_ui.inc',
  );

  $items['admin/xc/search/sortoption'] = array(
    'title' => 'Sorting options',
    'description' => 'Sorting options builder',
    'page callback' => 'xc_search_sortoption_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 1,
    'file' => 'admin/xc_search_sortoption.inc',
  );

  $items['admin/xc/search/display_template'] = array(
    'title' => 'Display templates',
    'description' => 'Create, modify, delete, and view record display templates',
    'page callback' => 'xc_search_display_template_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 2,
    'file' => 'admin/xc_search_display_template.inc',
  );
  $items['admin/xc/search/display_template_element'] = array(
    'title' => 'Display template element',
    'description' => 'Create, modify, delete, and view record display template elements',
    'page callback' => 'xc_search_display_template_element_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 3,
    'file' => 'admin/xc_search_display_template_element.inc',
  );
  $items['admin/xc/search/facet'] = array(
    'title' => 'Facets',
    'description' => 'Create, modify, delete, and view facet groups and facet properties',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 4,
  );
  $items['admin/xc/search/highlighter'] = array(
    'title' => 'Why did I get these results for my query',
    'description' => 'Create, modify, delete, and view facets highlighters',
    'page callback' => 'xc_search_highlighter_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 5,
    'file' => 'admin/xc_search_highlighter.inc',
  );
  $items['admin/xc/search/mlt'] = array(
    'title' => 'More Like This settings',
    'description' => 'Create, modify, delete, and view "more like this" query parameters',
    'page callback' => 'xc_search_mlt_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 6,
    'file' => 'admin/xc_search_mlt.inc',
  );

  // facet
  $items['admin/xc/search/facet/group'] = array(
    'title' => 'Facet groups',
    'description' => 'Facet groups',
    'page callback' => 'xc_search_facet_group_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 2,
    'file' => 'admin/xc_search_facet_group.inc',
  );
  $items['admin/xc/search/facet/fieldfacetset'] = array(
    'title' => 'Field facet properties',
    'description' => 'Create, modify, and view field facet properties',
    'page callback' => 'xc_search_field_facet_properties_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 4,
    'file' => 'admin/xc_search_field_facet_properties.inc',
  );
  $items['admin/xc/search/facet/queryfacetset'] = array(
    'title' => 'Query facet properties',
    'description' => 'Create, modify, and view query facet properties',
    'page callback' => 'xc_search_query_facet_properties_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 5,
    'file' => 'admin/xc_search_query_facet_properties.inc',
  );
  $items['admin/xc/search/facet/datefacetset'] = array(
    'title' => 'Date facet properties',
    'description' => 'Create, modify, and view date facet properties',
    'page callback' => 'xc_search_date_facet_properties_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'weight' => 6,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );

  // facet group
  $items['admin/xc/search/facet/group/list'] = array(
    'title' => 'List facet groups',
    'description' => 'List facet groups',
    'page callback' => 'xc_search_facet_group_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 2,
    'file' => 'admin/xc_search_facet_group.inc',
  );
  $items['admin/xc/search/facet/group/add'] = array(
    'title' => 'Create new facet group',
    'description' => 'Create new facet group',
    'page callback' => 'xc_search_facet_group_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
    'file' => 'admin/xc_search_facet_group.inc',
  );
  $items['admin/xc/search/facet/group/import_defaults'] = array(
    'title' => 'Import default facet group settings',
    'description' => 'Import default facet group settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_group_import_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_facet_group.inc',
  );
  $items['admin/xc/search/facet/group/%xc_search_facet_group'] = array(
    'description' => 'Show facet group',
    'title callback' => 'xc_search_facet_group_title',
    'title arguments' => array(5),
    'page callback' => 'xc_search_facet_group_view',
    'page arguments' => array(5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_facet_group.inc',
  );
  $items['admin/xc/search/facet/group/%xc_search_facet_group/view'] = array(
    'title' => 'View',
    'description' => 'View facet group',
    'page callback' => 'xc_search_facet_group_view',
    'page arguments' => array(5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_facet_group.inc',
  );
  $items['admin/xc/search/facet/group/%xc_search_facet_group/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit facet group',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_group_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'admin/xc_search_facet_group.inc',
  );
  $items['admin/xc/search/facet/group/%xc_search_facet_group/reorder'] = array(
    'title' => 'Reorder',
    'description' => 'Reorder facet group\'s fields',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_group_reorder_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'admin/xc_search_facet_group.inc',
  );
  $items['admin/xc/search/facet/group/%xc_search_facet_group/add_field_facet'] = array(
    'title' => 'Add field facet',
    'description' => 'Add field facet',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_field_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
    'file' => 'admin/xc_search_facet_field.inc',
  );
  $items['admin/xc/search/facet/group/%xc_search_facet_group/add_query_facet'] = array(
    'title' => 'Add query facet',
    'description' => 'Add query facet',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_query_field_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
    'file' => 'admin/xc_search_facet_query_field.inc',
  );
  $items['admin/xc/search/facet/group/%xc_search_facet_group/add_date_facet'] = array(
    'title' => 'Add date facet',
    'description' => 'Add date facet',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_date_field_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 5,
    'file' => 'admin/xc_search_facet_date_field.inc',
  );

  // field facet set
  $items['admin/xc/search/facet/fieldfacetset/list'] = array(
    'title' => 'List field facet properties',
    'description' => 'Create, modify, and view field facet properties',
    'page callback' => 'xc_search_field_facet_properties_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_field_facet_properties.inc',
  );
  $items['admin/xc/search/facet/fieldfacetset/add'] = array(
    'title' => 'Add new facet attribute set',
    'description' => 'Add new facet attribute set',
    'page callback' => 'xc_search_field_facet_properties_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_field_facet_properties.inc',
  );
  $items['admin/xc/search/facet/fieldfacetset/import_defaults'] = array(
    'title' => 'Import default field facet properties',
    'description' => 'Import default field facet properties',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_field_facet_properties_import_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_display_template.inc',
  );
  $items['admin/xc/search/facet/fieldfacetset/%xc_search_field_facet_properties'] = array(
    'description' => 'Show field facet properties',
    'page callback' => 'xc_search_field_facet_properties_view',
    'page arguments' => array(5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_field_facet_properties.inc',
  );
  $items['admin/xc/search/facet/fieldfacetset/%xc_search_field_facet_properties/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_field_facet_properties.inc',
  );
  $items['admin/xc/search/facet/fieldfacetset/%xc_search_field_facet_properties/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit facet attribute set',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_field_facet_properties_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_field_facet_properties.inc',
  );

  // query facet set
  $items['admin/xc/search/facet/queryfacetset/list'] = array(
    'title' => 'List query facet attribute sets',
    'description' => 'Create, modify, and view Solr servers used for metadata storage and search',
    'page callback' => 'xc_search_query_facet_properties_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_query_facet_properties.inc',
  );
  $items['admin/xc/search/facet/queryfacetset/add'] = array(
    'title' => 'Add new query facet attribute set',
    'description' => 'Add new facet attribute set',
    'page callback' => 'xc_search_query_facet_properties_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_query_facet_properties.inc',
  );
  $items['admin/xc/search/facet/queryfacetset/%xc_search_query_facet_properties'] = array(
    'description' => 'Show query facet attribute set',
    'page callback' => 'xc_search_query_facet_properties_view',
    'page arguments' => array(5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_query_facet_properties.inc',
  );
  $items['admin/xc/search/facet/queryfacetset/%xc_search_query_facet_properties/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_query_facet_properties.inc',
  );
  $items['admin/xc/search/facet/queryfacetset/%xc_search_query_facet_properties/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit query facet attribute set',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_query_facet_properties_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_query_facet_properties.inc',
  );

  // date facet
  $items['admin/xc/search/facet/datefacetset/list'] = array(
    'title' => 'List date facet properties',
    'description' => 'Create, modify, and view date facet properties',
    'page callback' => 'xc_search_date_facet_properties_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );
  $items['admin/xc/search/facet/datefacetset/add'] = array(
    'title' => 'Add new date facet properties',
    'description' => 'Add new date facet properties',
    'page callback' => 'xc_search_date_facet_properties_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );
  $items['admin/xc/search/facet/datefacetset/import_defaults'] = array(
    'title' => 'Import default date facet properties',
    'description' => 'Import default date facet properties',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_date_facet_properties_import_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );
  $items['admin/xc/search/facet/datefacetset/%xc_search_date_facet_properties'] = array(
    'description' => 'Show date facet attribute set',
    'page callback' => 'xc_search_date_facet_properties_view',
    'page arguments' => array(5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );
  $items['admin/xc/search/facet/datefacetset/%xc_search_date_facet_properties/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );
  $items['admin/xc/search/facet/datefacetset/%xc_search_date_facet_properties/test'] = array(
    'title' => 'Test',
    'description' => 'Test date facet attribute set',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_date_facet_properties_test_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );
  $items['admin/xc/search/facet/datefacetset/%xc_search_date_facet_properties/test/%'] = array(
    'title' => 'Test',
    'description' => 'Test date facet attribute set',
    'page callback' => 'xc_search_date_facet_properties_test_result',
    'page arguments' => array(5, 7),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );
  $items['admin/xc/search/facet/datefacetset/%xc_search_date_facet_properties/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit date facet attribute set',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_date_facet_properties_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_date_facet_properties.inc',
  );
  $items['admin/xc/search/facet/field'] = array(
    'title' => 'Add',
    'description' => 'Add facet field',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_field_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_facet_field.inc',
  );
  $items['admin/xc/search/facet/field/import_defaults'] = array(
    'title' => 'Import default fields',
    'description' => 'Import default fields',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_field_import_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_display_template.inc',
  );
  $items['admin/xc/search/facet/field/%xc_search_facet_field'] = array(
    'title' => 'Edit',
    'description' => 'Edit date facet attribute set',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_field_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_facet_field.inc',
  );
  $items['admin/xc/search/facet/field/%xc_search_facet_field/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit date facet attribute set',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_field_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_facet_field.inc',
  );
  $items['admin/xc/search/facet/field/%xc_search_facet_field/remove'] = array(
    'title' => 'Edit',
    'description' => 'Edit date facet attribute set',
    'page callback' => 'xc_search_facet_field_remove',
    'page arguments' => array(5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_facet_field.inc',
  );

  // query facets
  $items['admin/xc/search/facet/queryfield/%xc_search_facet_field'] = array(
    'title' => 'Edit',
    'description' => 'Edit query facet properties',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_query_field_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_facet_query_field.inc',
  );
  $items['admin/xc/search/facet/queryfield/%xc_search_facet_field/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit query facet properties',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_query_field_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_facet_query_field.inc',
  );

  // date facets
  $items['admin/xc/search/facet/datefield/%xc_search_facet_field'] = array(
    'title' => 'Edit',
    'description' => 'Edit date facet properties',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_date_field_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_facet_date_field.inc',
  );
  $items['admin/xc/search/facet/datefield/%xc_search_facet_field/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit date facet properties',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_facet_date_field_edit_form', 5),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_facet_date_field.inc',
  );

  // highlighter
  $items['admin/xc/search/highlighter/list'] = array(
    'title' => 'List',
    'description' => 'List highlighters',
    'page callback' => 'xc_search_highlighter_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
    'file' => 'admin/xc_search_highlighter.inc',
  );
  $items['admin/xc/search/highlighter/add'] = array(
    'title' => 'Add highlighter',
    'description' => 'Add new highlighter',
    'page callback' => 'xc_search_highlighter_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_highlighter.inc',
  );
  $items['admin/xc/search/highlighter/import_defaults'] = array(
    'title' => 'Import default highlighters',
    'description' => 'Import default highlighters',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_highlighter_import_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_highlighter.inc',
  );
  $items['admin/xc/search/highlighter/%xc_search_highlighter'] = array(
    'description' => 'Show highlighter',
    'page callback' => 'xc_search_highlighter_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_highlighter.inc',
  );
  $items['admin/xc/search/highlighter/%xc_search_highlighter/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_highlighter.inc',
  );
  $items['admin/xc/search/highlighter/%xc_search_highlighter/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit highlighter',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_highlighter_edit_form', 4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_highlighter.inc',
  );
  // TODO: $items['admin/xc/search/highlighter/%xc_search_highlighter/delete'] = array(

  // more like this
  $items['admin/xc/search/mlt/list'] = array(
    'title' => 'List',
    'description' => 'List More Like This settings',
    'page callback' => 'xc_search_mlt_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
    'file' => 'admin/xc_search_mlt.inc',
  );
  $items['admin/xc/search/mlt/add'] = array(
    'title' => 'Add new',
    'description' => 'Add new More Like This settings',
    'page callback' => 'xc_search_mlt_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_mlt.inc',
  );
  $items['admin/xc/search/mlt/add'] = array(
    'title' => 'Add new',
    'description' => 'Add new More Like This settings',
    'page callback' => 'xc_search_mlt_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_mlt.inc',
  );
  $items['admin/xc/search/mlt/import_defaults'] = array(
    'title' => 'Import default more like this settings',
    'description' => 'Import default more like this settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_mlt_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_mlt.inc',
  );
  $items['admin/xc/search/mlt/%xc_search_mlt'] = array(
    'description' => 'Show More Like This settings',
    'page callback' => 'xc_search_mlt_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_mlt.inc',
  );
  $items['admin/xc/search/mlt/%xc_search_mlt/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_mlt.inc',
  );
  $items['admin/xc/search/mlt/%xc_search_mlt/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit More Like This settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_mlt_edit_form', 4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_mlt.inc',
  );
  // TODO: $items['admin/xc/search/mlt/%xc_search_mlt/delete'] = array(

  // display template elements
  $items['admin/xc/search/display_template_element/list'] = array(
    'title' => 'List',
    'description' => 'List display template elements',
    'page callback' => 'xc_search_display_template_element_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
    'file' => 'admin/xc_search_display_template_element.inc',
  );
  $items['admin/xc/search/display_template_element/add'] = array(
    'title' => 'Create display template element',
    'description' => 'Create display template element',
    'page callback' => 'xc_search_display_template_element_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_display_template_element.inc',
  );
  $items['admin/xc/search/display_template_element/import_defaults'] = array(
    'title' => 'Import default display template elements',
    'description' => 'Import default display template elements',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_display_template_element_import_defaults'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_display_template_element.inc',
  );
  $items['admin/xc/search/display_template_element/%xc_search_display_template_element'] = array(
    'description' => 'Show display template element',
    'page callback' => 'xc_search_display_template_element_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_display_template_element.inc',
  );
  $items['admin/xc/search/display_template_element/%xc_search_display_template_element/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_display_template_element.inc',
  );
  $items['admin/xc/search/display_template_element/%xc_search_display_template_element/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit display template element',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_display_template_element_edit_form', 4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_display_template_element.inc',
  );

  // display templates
  $items['admin/xc/search/display_template/list'] = array(
    'title' => 'List',
    'description' => 'List display templates',
    'page callback' => 'xc_search_display_template_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
    'file' => 'admin/xc_search_display_template.inc',
  );
  $items['admin/xc/search/display_template/add'] = array(
    'title' => 'Add display template',
    'description' => 'Create a new display template',
    'page callback' => 'xc_search_display_template_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_display_template.inc',
  );
  $items['admin/xc/search/display_template/import_defaults'] = array(
    'title' => 'Import default templates',
    'description' => 'Import default display templates',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_display_template_import_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_display_template.inc',
  );
  $items['admin/xc/search/display_template/%xc_search_display_template'] = array(
    'description' => 'Show display template',
    'page callback' => 'xc_search_display_template_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_display_template.inc',
  );
  $items['admin/xc/search/display_template/%xc_search_display_template/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_display_template.inc',
  );
  $items['admin/xc/search/display_template/%xc_search_display_template/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit display template',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_display_template_edit_form', 4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_display_template.inc',
  );

  // search UI-s
  $items['admin/xc/search/ui/list'] = array(
    'title' => 'List',
    'description' => 'List search user interfaces',
    'page callback' => 'xc_search_ui_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
    'file' => 'admin/xc_search_ui.inc',
  );
  $items['admin/xc/search/ui/add'] = array(
    'title' => 'Add UI',
    'description' => 'Create new search user interface',
    'page callback' => 'xc_search_ui_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_ui.inc',
  );
  $items['admin/xc/search/ui/restore'] = array(
    'title' => 'Restore default UIs',
    'description' => 'Restore default search user interfaces',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_ui_import_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_ui.inc',
  );
  $items['admin/xc/search/ui/%xc_search_ui'] = array(
    'description' => 'Show search user interfaces',
    'page callback' => 'xc_search_ui_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_ui.inc',
  );
  $items['admin/xc/search/ui/%xc_search_ui/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_ui.inc',
  );
  $items['admin/xc/search/ui/%xc_search_ui/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit search user interface',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_ui_edit_form', 4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_ui.inc',
  );
  // TODO: $items['admin/xc/search/ui/%xc_search_ui/delete'] = array(

  // sorting options
  $items['admin/xc/search/sortoption/list'] = array(
    'title' => 'List',
    'description' => 'List sorting options',
    'page callback' => 'xc_search_sortoption_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
    'file' => 'admin/xc_search_sortoption.inc',
  );
  $items['admin/xc/search/sortoption/add'] = array(
    'title' => 'Add sorting option',
    'description' => 'Create new sorting option',
    'page callback' => 'xc_search_sortoption_add',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'admin/xc_search_sortoption.inc',
  );
  $items['admin/xc/search/sortoption/reorder'] = array(
    'title' => 'Change order',
    'description' => 'Change order of options',
    // 'page callback' => 'xc_search_sortoption_changeorder',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_sortoption_reorder_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'admin/xc_search_sortoption.inc',
  );
  $items['admin/xc/search/sortoption/restore'] = array(
    'title' => 'Restore default sorting options',
    'description' => 'Restore default sorting options',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_sortoption_import_defaults_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
    'file' => 'admin/xc_search_sortoption.inc',
  );
  $items['admin/xc/search/sortoption/%xc_search_sortoption'] = array(
    'description' => 'Show sorting option',
    'title callback' => 'xc_search_sortoption_title',
    'title arguments' => array(4),
    'page callback' => 'xc_search_sortoption_view',
    'page arguments' => array(4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search_sortoption.inc',
  );
  $items['admin/xc/search/sortoption/%xc_search_sortoption/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_sortoption.inc',
  );
  $items['admin/xc/search/sortoption/%xc_search_sortoption/edit'] = array(
    'title' => 'Edit',
    'description' => 'Edit sort option',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_sortoption_edit_form', 4),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_sortoption.inc',
    'weight' => 1,
  );

  $items['admin/xc/search/sortoption/%xc_search_sortoption/enable'] = array(
    'title' => 'Enable',
    'description' => 'Enable sort option',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_sortoption_trigger_form', 4, TRUE),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_sortoption.inc',
    'weight' => 2,
  );

  $items['admin/xc/search/sortoption/%xc_search_sortoption/disable'] = array(
    'title' => 'Disable',
    'description' => 'Disable sort option',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_sortoption_trigger_form', 4, FALSE),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search_sortoption.inc',
    'weight' => 3,
  );

  // TODO: $items['admin/xc/search/ui/%xc_search_ui/delete'] = array(

  $items['admin/xc/search/location'] = array(
    'title' => 'Locations',
    'description' => 'List locations',
    'page callback' => 'xc_search_holdings_location_list',
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search.holdings_location.inc',
  );
  $items['admin/xc/search/location/list'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'admin/xc_search_sortoption.inc',
  );
  $items['admin/xc/search/location/upload'] = array(
    'title' => 'Upload location table',
    'description' => 'Upload location table',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_holdings_location_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_LOCAL_TASK,
    'file' => 'admin/xc_search.holdings_location.inc',
  );

  // full record display options
  $items['admin/xc/search/frd'] = array(
    'title' => 'Full record display options',
    'description' => 'Full record display options',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('xc_search_full_record_display_options_form'),
    'access arguments' => array(ADMINISTER_XC_SEARCH),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'admin/xc_search.full_record_display_options.inc',
  );

  return $items;
}
