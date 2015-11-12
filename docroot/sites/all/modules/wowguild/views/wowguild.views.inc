<?php
/**
 * @file
 * Providing extra functionality for the MMOToon UI via views.
 */


/**
 * Implements hook_views_data()
 */
function wowguild_views_data_alter(&$data) {
  /*
  $data['wowtoon_cached_data']['table']['group']  = t('WoWToon');
  $data['wowtoon_cached_data']['table']['base'] = array(
      'field' => 'toon_key',
      'title' => t('Toon'),
      'help' => t('World of Warcraft Characters.'),
      'access query tag' => 'user_access',
  );

  $data['wowtoon_cached_data']['fullname'] = array(
      'field' => array(
        'title' => t('Fullname'),
        'help' => t('Name and title of WoWToon'),
        'handler' => 'avatar_handler_wowtoon_field',
    ),
  );
  */
  $data['wowtoon_cached_data']['table']['group']  = $data['avatar']['table']['group'];
  $data['wowtoon_owners']['table']['group']  = $data['avatar']['table']['group'];
  $data['wowtoon_cached_data']['table']['join'] = array(
      'avatar' => array(
        'left_table' => 'wowtoon_owners',
        'left_field' => 'toon_key',
        'field' => 'toon_key',
    )
  );

  $data['wowtoon_owners']['table']['join'] = array(
        'avatar' => array(
          'left_field' => 'aid',
          'field' => 'aid',
    )
  );
  /*
  $data['wowtoon_feed_item']['table']['join'] = array(
        'avatar' => array(
          'left_table' => 'wowtoon_owners',
          'left_field' => 'toon_key',
          'field' => 'toon_key',
    )
  );


  $data['wowtoon_owners']['toon_key'] = array(
        'field' => array(
          'field' => 'toon_key',
          'title' => t('Owner Toon Key'),
          'help' => t('Toon Key'),
          'handler' => 'views_handler_field',
    ),
  );
  */
  $data['wowtoon_cached_data']['level'] = array(
        'title' => t('Level'),
        'help' => t('Level of this Character'),
  		'field' => array(
            'field' => 'level',
            'title' => t('Level'),
            'help' => t('Level of this Character'),
            'click sortable' => TRUE,
            'handler' => 'views_handler_field',
	    ),
  		'filter' => array(
  			'field' => 'avg_ilevel',
  			'handler' => 'views_handler_filter_numeric',
  		)
);
  $data['wowtoon_cached_data']['avg_ilevel'] = array(
      'title' => t('Average iLevel'),
      'help' => t('Average iLevel of equipable gear.'),
  		'field' => array(
            'field' => 'avg_ilevel',
            'title' => t('Average iLevel'),
            'help' => t('Average Item Lvel of this character'),
            'click sortable' => TRUE,
            'handler' => 'views_handler_field',
    	),
  		'filter' => array(
  			'field' => 'avg_ilevel',
  			'handler' => 'views_handler_filter_numeric',
  		)
  );
  $data['wowtoon_cached_data']['achievement_points'] = array(
          'field' => array(
            'field' => 'achievement_points',
            'title' => t('Achievement Points'),
            'click sortable' => TRUE,
            'help' => t('Total number of Achievement Points'),
            'handler' => 'views_handler_field',
    ),
  );


  $data['wowtoon_cached_data']['classId'] = array(
        'field' => array(
          'field' => 'classId',
          'title' => t('ClassId'),
          'help' => t('Class ID - Used to colorize name link'),
          'handler' => 'views_handler_field',
    ),
  );
  $data['wowtoon_cached_data']['class'] = array(
        'field' => array(
          'field' => 'class',
          'title' => t('Class'),
          'help' => t('Class of WoWToon'),
          'handler' => 'avatar_handler_wowtoon_field',
    ),
  );
  $data['wowtoon_cached_data']['race'] = array(
        'field' => array(
          'field' => 'race',
          'title' => t('Race'),
          'help' => t('Race of WoWToon'),
          'handler' => 'avatar_handler_wowtoon_field',
    ),
  );
  $data['wowtoon_cached_data']['guild_rank'] = array(
      'title' => t('Guild Rank'),
      'help' => t('Rank of this character in this guild.'),
      'field' => array(
          'field' => 'guild_rank',
          'handler' => 'avatar_handler_wowtoon_field',
          'click sortable' => TRUE,
      ),
      'filter' => array(
            'field' => 'guild_rank',
            'handler' => 'views_handler_filter_string',  // TODO: When Views Issue 1514162 is fixed, change to 'views_handler_filter_numeric'
          )
  );

  /** Activity Feed Items **/
  /*
  $data['wowtoon_cached_data']['table']['join'] = array(
        'wowtoon_feed_item' => array(
          'left_field' => 'tid',
          'field' => 'keyid',
    )
  );*/

  $data['wowtoon_feed_item']['description']['field']['handler'] = 'wowguild_handler_wowtoon_feed_item_field';
  // $data['wowtoon_feed_item']['description']['field']['type'] = 'text';

  //$data['wowtoon_feed_item']['description']['field']['field_name'] = 'description';
  $data['wowtoon_feed_item']['dateCompleted']['field']['handler'] = 'views_handler_field_date';
  //$data['wowtoon_feed_item']['dateCompleted']['field']['type'] = 'text_unformatted';
}


/**
* Implements hook_views_data()
function wowguild_views_data() {
  // ----------------------------------------------------------------
  // users table

  //die("asdasdasdasdasd");
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['wowtoon_cached_data']['table']['group']  = t('WoWToon');
  $data['wowtoon_cached_data']['table']['base'] = array(
    'field' => 'toon_key',
    'title' => t('Toon'),
    'help' => t('World of Warcraft Characters.'),
    'access query tag' => 'user_access',
  );
  $data['avatar']['table']['join'] = array(
        'wowtoon_cached_data' => array(
          'left_field' => 'toon_key',
          'field' => 'toon_key',
  )
  );

  $data['wowtoon_cached_data']['toon_key'] = array(
    'title' => t('ToonKey'),
    'help' => t('The toon ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'tid',
  ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );

  $data['wowtoon_cached_data']['name'] = array(
    'title' => t('Name'),
    'help' => t('Character Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_text',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_filter_text',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
  $data['wowtoon_cached_data']['realm'] = array(
    'title' => t('Realm'),
    'help' => t('Character Realm'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_text',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_filter_text',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
  $data['wowtoon_cached_data']['level'] = array(
    'title' => t('Level'),
    'help' => t('Character Level'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_argument_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
  $data['wowtoon_cached_data']['guild_name'] = array(
    'title' => t('Guild Name'),
    'help' => t('Name of Guild'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_text',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_argument_text',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
/*
  $data['wowtoon_cached_data']['guild_rank'] = array(
    'title' => t('Guild Rank'),
    'help' => t('Characters rank in the guild'),
    'field' => array(
      'field' => 'guild_rank',
      'handler' => 'wowtoon_handler_field_wowtoon_cached_data',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_argument_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
  $data['wowtoon_cached_data']['guild_weekly'] = array(
    'title' => t('Weekly Points'),
    'help' => t('Points earned for the guild this week'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_argument_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
  $data['wowtoon_cached_data']['guild_lifetime'] = array(
    'title' => t('Lifetime Points'),
    'help' => t('Points earned for the guild'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_argument_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
  $data['wowtoon_cached_data']['achievement_points'] = array(
    'title' => t('Achievement Points'),
    'help' => t('Character Achievement Points'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_argument_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
  $data['wowtoon_cached_data']['avg_ilevel'] = array(
    'title' => t('iLevel'),
    'help' => t('Item level of items equiped'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_argument_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );
  $data['wowtoon_cached_data']['avg_ilevel_best'] = array(
    'title' => t('iLevel Best'),
    'help' => t('Item level os all items owned by character'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'name',
  ),
    'filter' => array(
      'handler' => 'views_handler_argument_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );



  $data['wowtoon_cached_data']['gid'] = array(
    'title' => t('Gid'),
    'help' => t('The guild ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
  ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'tid',
  ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
  ),
    'sort' => array(
      'handler' => 'views_handler_sort',
  ),
  );

  // class
  // Class can be displayed as text or an image.
  $data['wowtoon_cached_data']['class'] = array(
    'title' => t('Class'),
    'help' => t("Displays Character's Class."),
    'field' => array(
      'field' => 'class',
      'handler' => 'wowtoon_handler_field_wowtoon_cached_data',
  ),
  );
  // race
  // Race can be displayed as text or an image.
  $data['wowtoon_cached_data']['race'] = array(
    'title' => t('Race'),
    'help' => t("Displays Character's Race."),
    'field' => array(
      'field' => 'race',
      'handler' => 'wowtoon_handler_field_wowtoon_cached_data',
  ),
  );
  */

/*
  return $data;
}
*/


/**
 * Implementation of hook_views_handlers()
 *
 * hook_views_handlers tells the views module how to present fields within the UI and also how to
 * display fields within a view itself. You will find a file corresponding to each parent item
 * in the module directory.
 *
 * format: [module]_handler_[type]_[tablename]_[fieldname]
 *
 * This function does not seem to be used in Drupal 7.
 */

function wowguild_views_handlers() {
  return array(
  'info' => array(
    'path' => drupal_get_path('module', 'wowguild') . '/', //includes/views/
  ),
    'handlers' => array(
      'views_handler_wowtoon_cached_data_tid' => array(
         'parent' => 'views_handler_field',
  ),
      'views_handler_argument_wowtoon_cached_data_tid' => array(
         'parent' => 'views_handler_argument_numeric',
  ),
       'wowtoon_handler_field_wowtoon_cached_data_class_img' => array(
         'parent' => 'views_handler_field',
         'file' => 'wowtoon_handler_field_wowtoon_cached_data_class_img.inc'
  ),
  ),
  );
}


/**
 * Implement hook_views_default_views().
 */
function wowguild_views_default_views() {
  $views = array();



  /***************************/
  /*** BEGIN WOWTOON VIEWS ***/
  /***************************/
$view = new view;
$view->name = 'wowtoon_views';
$view->description = '';
$view->tag = 'wowtoon';
$view->base_table = 'avatar';
$view->human_name = 'WoWToon Views';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Roster';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view any wowtoon avatar';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'tid' => 'tid',
  'race' => 'race',
  'class' => 'class',
  'name' => 'name',
  'guild_rank' => 'guild_rank',
  'avg_ilevel' => 'avg_ilevel',
  'achievement_points' => 'achievement_points',
);
$handler->display->display_options['style_options']['default'] = 'guild_rank';
$handler->display->display_options['style_options']['info'] = array(
  'tid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'race' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'class' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'guild_rank' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'avg_ilevel' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'achievement_points' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: Avatar: Avatar ID */
$handler->display->display_options['fields']['aid']['id'] = 'aid';
$handler->display->display_options['fields']['aid']['table'] = 'avatar';
$handler->display->display_options['fields']['aid']['field'] = 'aid';
$handler->display->display_options['fields']['aid']['exclude'] = TRUE;
$handler->display->display_options['fields']['aid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['aid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['aid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['aid']['alter']['external'] = 0;
$handler->display->display_options['fields']['aid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['aid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['aid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['aid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['aid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['aid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['aid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['aid']['alter']['html'] = 0;
$handler->display->display_options['fields']['aid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['aid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['aid']['hide_empty'] = 0;
$handler->display->display_options['fields']['aid']['empty_zero'] = 0;
$handler->display->display_options['fields']['aid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['aid']['format_plural'] = 0;
/* Field: Avatar: ClassId */
$handler->display->display_options['fields']['classId']['id'] = 'classId';
$handler->display->display_options['fields']['classId']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['classId']['field'] = 'classId';
$handler->display->display_options['fields']['classId']['exclude'] = TRUE;
$handler->display->display_options['fields']['classId']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['classId']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['classId']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['classId']['alter']['external'] = 0;
$handler->display->display_options['fields']['classId']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['classId']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['classId']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['classId']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['classId']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['classId']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['classId']['alter']['trim'] = 0;
$handler->display->display_options['fields']['classId']['alter']['html'] = 0;
$handler->display->display_options['fields']['classId']['element_label_colon'] = 1;
$handler->display->display_options['fields']['classId']['element_default_classes'] = 1;
$handler->display->display_options['fields']['classId']['hide_empty'] = 0;
$handler->display->display_options['fields']['classId']['empty_zero'] = 0;
$handler->display->display_options['fields']['classId']['hide_alter_empty'] = 1;
/* Field: Avatar: Race */
$handler->display->display_options['fields']['race']['id'] = 'race';
$handler->display->display_options['fields']['race']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['race']['field'] = 'race';
$handler->display->display_options['fields']['race']['exclude'] = TRUE;
$handler->display->display_options['fields']['race']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['race']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['race']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['race']['alter']['external'] = 0;
$handler->display->display_options['fields']['race']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['race']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['race']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['race']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['race']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['race']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['race']['alter']['trim'] = 0;
$handler->display->display_options['fields']['race']['alter']['html'] = 0;
$handler->display->display_options['fields']['race']['element_label_colon'] = 1;
$handler->display->display_options['fields']['race']['element_default_classes'] = 1;
$handler->display->display_options['fields']['race']['hide_empty'] = 0;
$handler->display->display_options['fields']['race']['empty_zero'] = 0;
$handler->display->display_options['fields']['race']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['race']['display_type'] = '1';
/* Field: Avatar: Class */
$handler->display->display_options['fields']['class']['id'] = 'class';
$handler->display->display_options['fields']['class']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['class']['field'] = 'class';
$handler->display->display_options['fields']['class']['label'] = 'Race/Class';
$handler->display->display_options['fields']['class']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['class']['alter']['text'] = '[race] [class]';
$handler->display->display_options['fields']['class']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['class']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['class']['alter']['external'] = 0;
$handler->display->display_options['fields']['class']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['class']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['class']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['class']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['class']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['class']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['class']['alter']['trim'] = 0;
$handler->display->display_options['fields']['class']['alter']['html'] = 0;
$handler->display->display_options['fields']['class']['element_label_colon'] = 1;
$handler->display->display_options['fields']['class']['element_default_classes'] = 1;
$handler->display->display_options['fields']['class']['hide_empty'] = 0;
$handler->display->display_options['fields']['class']['empty_zero'] = 0;
$handler->display->display_options['fields']['class']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['class']['display_type'] = '1';
/* Field: Avatar: Level */
$handler->display->display_options['fields']['level']['id'] = 'level';
$handler->display->display_options['fields']['level']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['level']['field'] = 'level';
$handler->display->display_options['fields']['level']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['level']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['level']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['level']['alter']['external'] = 0;
$handler->display->display_options['fields']['level']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['level']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['level']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['level']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['level']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['level']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['level']['alter']['trim'] = 0;
$handler->display->display_options['fields']['level']['alter']['html'] = 0;
$handler->display->display_options['fields']['level']['element_label_colon'] = 1;
$handler->display->display_options['fields']['level']['element_default_classes'] = 1;
$handler->display->display_options['fields']['level']['hide_empty'] = 0;
$handler->display->display_options['fields']['level']['empty_zero'] = 0;
$handler->display->display_options['fields']['level']['hide_alter_empty'] = 1;
/* Field: Avatar: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'avatar';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['path'] = 'avatar/[aid]';
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['external'] = 0;
$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['name']['alter']['rel'] = '[aid]';
$handler->display->display_options['fields']['name']['alter']['link_class'] = 'avatar-popup';
$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_class'] = 'color-c[classId] shadow';
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
/* Field: Avatar: Guild Rank */
$handler->display->display_options['fields']['guild_rank']['id'] = 'guild_rank';
$handler->display->display_options['fields']['guild_rank']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['guild_rank']['field'] = 'guild_rank';
$handler->display->display_options['fields']['guild_rank']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['external'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['guild_rank']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['guild_rank']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['trim'] = 0;
$handler->display->display_options['fields']['guild_rank']['alter']['html'] = 0;
$handler->display->display_options['fields']['guild_rank']['element_label_colon'] = 1;
$handler->display->display_options['fields']['guild_rank']['element_default_classes'] = 1;
$handler->display->display_options['fields']['guild_rank']['hide_empty'] = 0;
$handler->display->display_options['fields']['guild_rank']['empty_zero'] = 0;
$handler->display->display_options['fields']['guild_rank']['hide_alter_empty'] = 1;
/* Field: Avatar: Average iLevel */
$handler->display->display_options['fields']['avg_ilevel']['id'] = 'avg_ilevel';
$handler->display->display_options['fields']['avg_ilevel']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['avg_ilevel']['field'] = 'avg_ilevel';
$handler->display->display_options['fields']['avg_ilevel']['label'] = 'iLevel';
$handler->display->display_options['fields']['avg_ilevel']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['external'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['avg_ilevel']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['avg_ilevel']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['trim'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['html'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['element_label_colon'] = 1;
$handler->display->display_options['fields']['avg_ilevel']['element_default_classes'] = 1;
$handler->display->display_options['fields']['avg_ilevel']['hide_empty'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['empty_zero'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['hide_alter_empty'] = 1;
/* Field: Avatar: Achievement Points */
$handler->display->display_options['fields']['achievement_points']['id'] = 'achievement_points';
$handler->display->display_options['fields']['achievement_points']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['achievement_points']['field'] = 'achievement_points';
$handler->display->display_options['fields']['achievement_points']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['external'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['achievement_points']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['achievement_points']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['trim'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['html'] = 0;
$handler->display->display_options['fields']['achievement_points']['element_label_colon'] = 1;
$handler->display->display_options['fields']['achievement_points']['element_default_classes'] = 1;
$handler->display->display_options['fields']['achievement_points']['hide_empty'] = 0;
$handler->display->display_options['fields']['achievement_points']['empty_zero'] = 0;
$handler->display->display_options['fields']['achievement_points']['hide_alter_empty'] = 1;
/* Contextual filter: Avatar: Gid */
$handler->display->display_options['arguments']['gid']['id'] = 'gid';
$handler->display->display_options['arguments']['gid']['table'] = 'avatar';
$handler->display->display_options['arguments']['gid']['field'] = 'gid';
$handler->display->display_options['arguments']['gid']['default_action'] = 'default';
$handler->display->display_options['arguments']['gid']['default_argument_type'] = 'php';
$handler->display->display_options['arguments']['gid']['default_argument_options']['code'] = 'variable_set(\'wowguild_guild_gid\', 0)';
$handler->display->display_options['arguments']['gid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['gid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['gid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['gid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['gid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['gid']['not'] = 0;
/* Filter criterion: Avatar: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'avatar';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wowtoon' => 'wowtoon',
);

/* Display: Guild Roster */
$handler = $view->new_display('page', 'Guild Roster', 'page');
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Avatar: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'avatar';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wowtoon' => 'wowtoon',
);
/* Filter criterion: Avatar: Name */
$handler->display->display_options['filters']['name']['id'] = 'name';
$handler->display->display_options['filters']['name']['table'] = 'avatar';
$handler->display->display_options['filters']['name']['field'] = 'name';
$handler->display->display_options['filters']['name']['operator'] = 'contains';
$handler->display->display_options['filters']['name']['exposed'] = TRUE;
$handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['label'] = 'Search by Name';
$handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
$handler->display->display_options['filters']['name']['expose']['required'] = 0;
$handler->display->display_options['filters']['name']['expose']['remember'] = 1;
$handler->display->display_options['filters']['name']['expose']['multiple'] = FALSE;
$handler->display->display_options['path'] = 'roster';
$handler->display->display_options['menu']['title'] = 'Roster';
$handler->display->display_options['menu']['weight'] = '2';
$handler->display->display_options['menu']['name'] = 'main-menu';

/* Display: My Toons */
$handler = $view->new_display('page', 'My Toons', 'page_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'tid' => 'tid',
  'race' => 'race',
  'class' => 'class',
  'name' => 'name',
  'guild_rank' => 'guild_rank',
  'avg_ilevel' => 'avg_ilevel',
  'achievement_points' => 'achievement_points',
);
$handler->display->display_options['style_options']['default'] = 'name';
$handler->display->display_options['style_options']['info'] = array(
  'tid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'race' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'class' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'guild_rank' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'avg_ilevel' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'achievement_points' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Avatar: Avatar ID */
$handler->display->display_options['fields']['aid']['id'] = 'aid';
$handler->display->display_options['fields']['aid']['table'] = 'avatar';
$handler->display->display_options['fields']['aid']['field'] = 'aid';
$handler->display->display_options['fields']['aid']['exclude'] = TRUE;
$handler->display->display_options['fields']['aid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['aid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['aid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['aid']['alter']['external'] = 0;
$handler->display->display_options['fields']['aid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['aid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['aid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['aid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['aid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['aid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['aid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['aid']['alter']['html'] = 0;
$handler->display->display_options['fields']['aid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['aid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['aid']['hide_empty'] = 0;
$handler->display->display_options['fields']['aid']['empty_zero'] = 0;
$handler->display->display_options['fields']['aid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['aid']['format_plural'] = 0;
/* Field: Avatar: ClassId */
$handler->display->display_options['fields']['classId']['id'] = 'classId';
$handler->display->display_options['fields']['classId']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['classId']['field'] = 'classId';
$handler->display->display_options['fields']['classId']['exclude'] = TRUE;
$handler->display->display_options['fields']['classId']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['classId']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['classId']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['classId']['alter']['external'] = 0;
$handler->display->display_options['fields']['classId']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['classId']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['classId']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['classId']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['classId']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['classId']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['classId']['alter']['trim'] = 0;
$handler->display->display_options['fields']['classId']['alter']['html'] = 0;
$handler->display->display_options['fields']['classId']['element_label_colon'] = 1;
$handler->display->display_options['fields']['classId']['element_default_classes'] = 1;
$handler->display->display_options['fields']['classId']['hide_empty'] = 0;
$handler->display->display_options['fields']['classId']['empty_zero'] = 0;
$handler->display->display_options['fields']['classId']['hide_alter_empty'] = 1;
/* Field: Avatar: Race */
$handler->display->display_options['fields']['race']['id'] = 'race';
$handler->display->display_options['fields']['race']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['race']['field'] = 'race';
$handler->display->display_options['fields']['race']['exclude'] = TRUE;
$handler->display->display_options['fields']['race']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['race']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['race']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['race']['alter']['external'] = 0;
$handler->display->display_options['fields']['race']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['race']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['race']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['race']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['race']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['race']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['race']['alter']['trim'] = 0;
$handler->display->display_options['fields']['race']['alter']['html'] = 0;
$handler->display->display_options['fields']['race']['element_label_colon'] = 1;
$handler->display->display_options['fields']['race']['element_default_classes'] = 1;
$handler->display->display_options['fields']['race']['hide_empty'] = 0;
$handler->display->display_options['fields']['race']['empty_zero'] = 0;
$handler->display->display_options['fields']['race']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['race']['display_type'] = '1';
/* Field: Avatar: Class */
$handler->display->display_options['fields']['class']['id'] = 'class';
$handler->display->display_options['fields']['class']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['class']['field'] = 'class';
$handler->display->display_options['fields']['class']['label'] = 'Race/Class';
$handler->display->display_options['fields']['class']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['class']['alter']['text'] = '[race] [class]';
$handler->display->display_options['fields']['class']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['class']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['class']['alter']['external'] = 0;
$handler->display->display_options['fields']['class']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['class']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['class']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['class']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['class']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['class']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['class']['alter']['trim'] = 0;
$handler->display->display_options['fields']['class']['alter']['html'] = 0;
$handler->display->display_options['fields']['class']['element_label_colon'] = 1;
$handler->display->display_options['fields']['class']['element_default_classes'] = 1;
$handler->display->display_options['fields']['class']['hide_empty'] = 0;
$handler->display->display_options['fields']['class']['empty_zero'] = 0;
$handler->display->display_options['fields']['class']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['class']['display_type'] = '1';
/* Field: Avatar: Level */
$handler->display->display_options['fields']['level']['id'] = 'level';
$handler->display->display_options['fields']['level']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['level']['field'] = 'level';
$handler->display->display_options['fields']['level']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['level']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['level']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['level']['alter']['external'] = 0;
$handler->display->display_options['fields']['level']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['level']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['level']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['level']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['level']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['level']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['level']['alter']['trim'] = 0;
$handler->display->display_options['fields']['level']['alter']['html'] = 0;
$handler->display->display_options['fields']['level']['element_label_colon'] = 1;
$handler->display->display_options['fields']['level']['element_default_classes'] = 1;
$handler->display->display_options['fields']['level']['hide_empty'] = 0;
$handler->display->display_options['fields']['level']['empty_zero'] = 0;
$handler->display->display_options['fields']['level']['hide_alter_empty'] = 1;
/* Field: Avatar: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'avatar';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['path'] = 'avatar/[aid]';
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['external'] = 0;
$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['name']['alter']['rel'] = '[aid]';
$handler->display->display_options['fields']['name']['alter']['link_class'] = 'avatar-popup';
$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_class'] = 'color-c[classId] shadow';
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
/* Field: Avatar: Average iLevel */
$handler->display->display_options['fields']['avg_ilevel']['id'] = 'avg_ilevel';
$handler->display->display_options['fields']['avg_ilevel']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['avg_ilevel']['field'] = 'avg_ilevel';
$handler->display->display_options['fields']['avg_ilevel']['label'] = 'iLevel';
$handler->display->display_options['fields']['avg_ilevel']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['external'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['avg_ilevel']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['avg_ilevel']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['trim'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['alter']['html'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['element_label_colon'] = 1;
$handler->display->display_options['fields']['avg_ilevel']['element_default_classes'] = 1;
$handler->display->display_options['fields']['avg_ilevel']['hide_empty'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['empty_zero'] = 0;
$handler->display->display_options['fields']['avg_ilevel']['hide_alter_empty'] = 1;
/* Field: Avatar: Achievement Points */
$handler->display->display_options['fields']['achievement_points']['id'] = 'achievement_points';
$handler->display->display_options['fields']['achievement_points']['table'] = 'wowtoon_cached_data';
$handler->display->display_options['fields']['achievement_points']['field'] = 'achievement_points';
$handler->display->display_options['fields']['achievement_points']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['external'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['achievement_points']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['achievement_points']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['trim'] = 0;
$handler->display->display_options['fields']['achievement_points']['alter']['html'] = 0;
$handler->display->display_options['fields']['achievement_points']['element_label_colon'] = 1;
$handler->display->display_options['fields']['achievement_points']['element_default_classes'] = 1;
$handler->display->display_options['fields']['achievement_points']['hide_empty'] = 0;
$handler->display->display_options['fields']['achievement_points']['empty_zero'] = 0;
$handler->display->display_options['fields']['achievement_points']['hide_alter_empty'] = 1;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Avatar: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'avatar';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Avatar: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'avatar';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wowtoon' => 'wowtoon',
);
$handler->display->display_options['path'] = 'user/%/toons';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Toons';
$handler->display->display_options['menu']['weight'] = '0';




$views[$view->name] = $view;




/***************************/
/*** BEGIN ACTIVITY FEED ***/
/***************************/
$view = new view;
$view->name = 'activity_feed';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'wowtoon_feed_item';
$view->human_name = 'Activity Feed';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Activity Feed';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view any wowtoon avatar';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '1';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'achid' => 'achid',
  'keyid' => 'keyid',
  'icon' => 'icon',
  'description' => 'description',
  'dateCompleted' => 'dateCompleted',
);
$handler->display->display_options['style_options']['default'] = 'dateCompleted';
$handler->display->display_options['style_options']['info'] = array(
  'achid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'keyid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'icon' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'description' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'dateCompleted' => array(
    'sortable' => 0,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: Activity Feed Item: Activity feed item ID */
$handler->display->display_options['fields']['achid']['id'] = 'achid';
$handler->display->display_options['fields']['achid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['achid']['field'] = 'achid';
$handler->display->display_options['fields']['achid']['exclude'] = TRUE;
$handler->display->display_options['fields']['achid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['achid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['achid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['achid']['alter']['external'] = 0;
$handler->display->display_options['fields']['achid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['achid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['achid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['achid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['achid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['achid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['achid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['achid']['alter']['html'] = 0;
$handler->display->display_options['fields']['achid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['achid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['achid']['hide_empty'] = 0;
$handler->display->display_options['fields']['achid']['empty_zero'] = 0;
$handler->display->display_options['fields']['achid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['achid']['format_plural'] = 0;
/* Field: Activity Feed Item: Keyid */
$handler->display->display_options['fields']['keyid']['id'] = 'keyid';
$handler->display->display_options['fields']['keyid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['keyid']['field'] = 'keyid';
$handler->display->display_options['fields']['keyid']['exclude'] = TRUE;
$handler->display->display_options['fields']['keyid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['external'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['keyid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['keyid']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['html'] = 0;
$handler->display->display_options['fields']['keyid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['keyid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['keyid']['hide_empty'] = 0;
$handler->display->display_options['fields']['keyid']['empty_zero'] = 0;
$handler->display->display_options['fields']['keyid']['hide_alter_empty'] = 1;
/* Field: Activity Feed Item: Icon */
$handler->display->display_options['fields']['icon']['id'] = 'icon';
$handler->display->display_options['fields']['icon']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['icon']['field'] = 'icon';
$handler->display->display_options['fields']['icon']['exclude'] = TRUE;
$handler->display->display_options['fields']['icon']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['icon']['alter']['text'] = '<img src="[icon]" />';
$handler->display->display_options['fields']['icon']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['icon']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['icon']['alter']['external'] = 0;
$handler->display->display_options['fields']['icon']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['icon']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['icon']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['icon']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['icon']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['icon']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['icon']['alter']['trim'] = 0;
$handler->display->display_options['fields']['icon']['alter']['html'] = 0;
$handler->display->display_options['fields']['icon']['element_label_colon'] = 1;
$handler->display->display_options['fields']['icon']['element_default_classes'] = 1;
$handler->display->display_options['fields']['icon']['hide_empty'] = 1;
$handler->display->display_options['fields']['icon']['empty_zero'] = 0;
$handler->display->display_options['fields']['icon']['hide_alter_empty'] = 1;
/* Field: Activity Feed Item: Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = '';
$handler->display->display_options['fields']['description']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['description']['alter']['text'] = '[icon] [description]';
$handler->display->display_options['fields']['description']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['description']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['description']['alter']['external'] = 0;
$handler->display->display_options['fields']['description']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['description']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['description']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['description']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['description']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['description']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['description']['alter']['trim'] = 0;
$handler->display->display_options['fields']['description']['alter']['html'] = 0;
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['description']['element_default_classes'] = 1;
$handler->display->display_options['fields']['description']['hide_empty'] = 0;
$handler->display->display_options['fields']['description']['empty_zero'] = 0;
$handler->display->display_options['fields']['description']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['description']['link_to_entity'] = 0;
/* Field: Activity Feed Item: DateCompleted */
$handler->display->display_options['fields']['dateCompleted']['id'] = 'dateCompleted';
$handler->display->display_options['fields']['dateCompleted']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['dateCompleted']['field'] = 'dateCompleted';
$handler->display->display_options['fields']['dateCompleted']['label'] = 'Date Completed';
$handler->display->display_options['fields']['dateCompleted']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['external'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['dateCompleted']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['dateCompleted']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['trim'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['html'] = 0;
$handler->display->display_options['fields']['dateCompleted']['element_label_colon'] = 1;
$handler->display->display_options['fields']['dateCompleted']['element_default_classes'] = 1;
$handler->display->display_options['fields']['dateCompleted']['hide_empty'] = 0;
$handler->display->display_options['fields']['dateCompleted']['empty_zero'] = 0;
$handler->display->display_options['fields']['dateCompleted']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['dateCompleted']['date_format'] = 'time ago';

/* Display: Activity Feed */
$handler = $view->new_display('page', 'Activity Feed', 'page');
$handler->display->display_options['path'] = 'admin/content/wowtoon_feed_items/list';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'List';
$handler->display->display_options['menu']['weight'] = '-10';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'Activity Feeds';
$handler->display->display_options['tab_options']['weight'] = '0';

/* Display: Guild Feed */
$handler = $view->new_display('block', 'Guild Feed', 'guild_feed_block');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Activity Feed Item: Activity feed item ID */
$handler->display->display_options['fields']['achid']['id'] = 'achid';
$handler->display->display_options['fields']['achid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['achid']['field'] = 'achid';
$handler->display->display_options['fields']['achid']['exclude'] = TRUE;
$handler->display->display_options['fields']['achid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['achid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['achid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['achid']['alter']['external'] = 0;
$handler->display->display_options['fields']['achid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['achid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['achid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['achid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['achid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['achid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['achid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['achid']['alter']['html'] = 0;
$handler->display->display_options['fields']['achid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['achid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['achid']['hide_empty'] = 0;
$handler->display->display_options['fields']['achid']['empty_zero'] = 0;
$handler->display->display_options['fields']['achid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['achid']['format_plural'] = 0;
/* Field: Activity Feed Item: Keyid */
$handler->display->display_options['fields']['keyid']['id'] = 'keyid';
$handler->display->display_options['fields']['keyid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['keyid']['field'] = 'keyid';
$handler->display->display_options['fields']['keyid']['exclude'] = TRUE;
$handler->display->display_options['fields']['keyid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['external'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['keyid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['keyid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['html'] = 0;
$handler->display->display_options['fields']['keyid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['keyid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['keyid']['hide_empty'] = 0;
$handler->display->display_options['fields']['keyid']['empty_zero'] = 0;
$handler->display->display_options['fields']['keyid']['hide_alter_empty'] = 1;
/* Field: Activity Feed Item: Icon */
$handler->display->display_options['fields']['icon']['id'] = 'icon';
$handler->display->display_options['fields']['icon']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['icon']['field'] = 'icon';
$handler->display->display_options['fields']['icon']['exclude'] = TRUE;
$handler->display->display_options['fields']['icon']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['icon']['alter']['text'] = '<img src="[icon]" />';
$handler->display->display_options['fields']['icon']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['icon']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['icon']['alter']['external'] = 0;
$handler->display->display_options['fields']['icon']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['icon']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['icon']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['icon']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['icon']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['icon']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['icon']['alter']['trim'] = 0;
$handler->display->display_options['fields']['icon']['alter']['html'] = 0;
$handler->display->display_options['fields']['icon']['element_label_colon'] = 1;
$handler->display->display_options['fields']['icon']['element_default_classes'] = 1;
$handler->display->display_options['fields']['icon']['hide_empty'] = 1;
$handler->display->display_options['fields']['icon']['empty_zero'] = 0;
$handler->display->display_options['fields']['icon']['hide_alter_empty'] = 1;
/* Field: Activity Feed Item: Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = '';
$handler->display->display_options['fields']['description']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['description']['alter']['text'] = '[icon] [description]';
$handler->display->display_options['fields']['description']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['description']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['description']['alter']['external'] = 0;
$handler->display->display_options['fields']['description']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['description']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['description']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['description']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['description']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['description']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['description']['alter']['trim'] = 0;
$handler->display->display_options['fields']['description']['alter']['html'] = 0;
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['description']['element_default_classes'] = 1;
$handler->display->display_options['fields']['description']['hide_empty'] = 0;
$handler->display->display_options['fields']['description']['empty_zero'] = 0;
$handler->display->display_options['fields']['description']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['description']['link_to_entity'] = 0;
/* Field: Activity Feed Item: DateCompleted */
$handler->display->display_options['fields']['dateCompleted']['id'] = 'dateCompleted';
$handler->display->display_options['fields']['dateCompleted']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['dateCompleted']['field'] = 'dateCompleted';
$handler->display->display_options['fields']['dateCompleted']['label'] = 'Date Completed';
$handler->display->display_options['fields']['dateCompleted']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['external'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['dateCompleted']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['dateCompleted']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['trim'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['html'] = 0;
$handler->display->display_options['fields']['dateCompleted']['element_label_colon'] = 1;
$handler->display->display_options['fields']['dateCompleted']['element_default_classes'] = 1;
$handler->display->display_options['fields']['dateCompleted']['hide_empty'] = 0;
$handler->display->display_options['fields']['dateCompleted']['empty_zero'] = 0;
$handler->display->display_options['fields']['dateCompleted']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['dateCompleted']['date_format'] = 'time ago';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Activity Feed Item: Keyid */
$handler->display->display_options['arguments']['keyid']['id'] = 'keyid';
$handler->display->display_options['arguments']['keyid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['arguments']['keyid']['field'] = 'keyid';
$handler->display->display_options['arguments']['keyid']['default_action'] = 'default';
$handler->display->display_options['arguments']['keyid']['default_argument_type'] = 'php';
$handler->display->display_options['arguments']['keyid']['default_argument_options']['code'] = '$arg2 = arg(2);
      if (arg(0) == \'guild\' && is_numeric(arg(1)) && empty($arg2)) {
        return arg(1);
     }
return 0;';
$handler->display->display_options['arguments']['keyid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['keyid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['keyid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['keyid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['keyid']['glossary'] = 0;
$handler->display->display_options['arguments']['keyid']['limit'] = '0';
$handler->display->display_options['arguments']['keyid']['transform_dash'] = 0;
$handler->display->display_options['arguments']['keyid']['break_phrase'] = 0;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Activity Feed Item: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'guild_feed';
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['block_caching'] = '4';

/* Display: Toon Feed */
$handler = $view->new_display('block', 'Toon Feed', 'toon_feed_block');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Activity Feed Item: Activity feed item ID */
$handler->display->display_options['fields']['achid']['id'] = 'achid';
$handler->display->display_options['fields']['achid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['achid']['field'] = 'achid';
$handler->display->display_options['fields']['achid']['exclude'] = TRUE;
$handler->display->display_options['fields']['achid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['achid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['achid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['achid']['alter']['external'] = 0;
$handler->display->display_options['fields']['achid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['achid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['achid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['achid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['achid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['achid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['achid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['achid']['alter']['html'] = 0;
$handler->display->display_options['fields']['achid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['achid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['achid']['hide_empty'] = 0;
$handler->display->display_options['fields']['achid']['empty_zero'] = 0;
$handler->display->display_options['fields']['achid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['achid']['format_plural'] = 0;
/* Field: Activity Feed Item: Keyid */
$handler->display->display_options['fields']['keyid']['id'] = 'keyid';
$handler->display->display_options['fields']['keyid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['keyid']['field'] = 'keyid';
$handler->display->display_options['fields']['keyid']['exclude'] = TRUE;
$handler->display->display_options['fields']['keyid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['external'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['keyid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['keyid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['keyid']['alter']['html'] = 0;
$handler->display->display_options['fields']['keyid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['keyid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['keyid']['hide_empty'] = 0;
$handler->display->display_options['fields']['keyid']['empty_zero'] = 0;
$handler->display->display_options['fields']['keyid']['hide_alter_empty'] = 1;
/* Field: Activity Feed Item: Icon */
$handler->display->display_options['fields']['icon']['id'] = 'icon';
$handler->display->display_options['fields']['icon']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['icon']['field'] = 'icon';
$handler->display->display_options['fields']['icon']['exclude'] = TRUE;
$handler->display->display_options['fields']['icon']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['icon']['alter']['text'] = '<img src="[icon]" />';
$handler->display->display_options['fields']['icon']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['icon']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['icon']['alter']['external'] = 0;
$handler->display->display_options['fields']['icon']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['icon']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['icon']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['icon']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['icon']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['icon']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['icon']['alter']['trim'] = 0;
$handler->display->display_options['fields']['icon']['alter']['html'] = 0;
$handler->display->display_options['fields']['icon']['element_label_colon'] = 1;
$handler->display->display_options['fields']['icon']['element_default_classes'] = 1;
$handler->display->display_options['fields']['icon']['hide_empty'] = 1;
$handler->display->display_options['fields']['icon']['empty_zero'] = 0;
$handler->display->display_options['fields']['icon']['hide_alter_empty'] = 1;
/* Field: Activity Feed Item: Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = '';
$handler->display->display_options['fields']['description']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['description']['alter']['text'] = '[icon] [description]';
$handler->display->display_options['fields']['description']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['description']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['description']['alter']['external'] = 0;
$handler->display->display_options['fields']['description']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['description']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['description']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['description']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['description']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['description']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['description']['alter']['trim'] = 0;
$handler->display->display_options['fields']['description']['alter']['html'] = 0;
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['description']['element_default_classes'] = 1;
$handler->display->display_options['fields']['description']['hide_empty'] = 0;
$handler->display->display_options['fields']['description']['empty_zero'] = 0;
$handler->display->display_options['fields']['description']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['description']['link_to_entity'] = 0;
/* Field: Activity Feed Item: DateCompleted */
$handler->display->display_options['fields']['dateCompleted']['id'] = 'dateCompleted';
$handler->display->display_options['fields']['dateCompleted']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['fields']['dateCompleted']['field'] = 'dateCompleted';
$handler->display->display_options['fields']['dateCompleted']['label'] = 'Date Completed';
$handler->display->display_options['fields']['dateCompleted']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['external'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['dateCompleted']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['dateCompleted']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['trim'] = 0;
$handler->display->display_options['fields']['dateCompleted']['alter']['html'] = 0;
$handler->display->display_options['fields']['dateCompleted']['element_label_colon'] = 1;
$handler->display->display_options['fields']['dateCompleted']['element_default_classes'] = 1;
$handler->display->display_options['fields']['dateCompleted']['hide_empty'] = 0;
$handler->display->display_options['fields']['dateCompleted']['empty_zero'] = 0;
$handler->display->display_options['fields']['dateCompleted']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['dateCompleted']['date_format'] = 'time ago';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Activity Feed Item: Keyid */
$handler->display->display_options['arguments']['keyid']['id'] = 'keyid';
$handler->display->display_options['arguments']['keyid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['arguments']['keyid']['field'] = 'keyid';
$handler->display->display_options['arguments']['keyid']['default_action'] = 'default';
$handler->display->display_options['arguments']['keyid']['default_argument_type'] = 'php';
$handler->display->display_options['arguments']['keyid']['default_argument_options']['code'] = '$arg2 = arg(2);
      if (arg(0) == \'avatar\' && is_numeric(arg(1)) && empty($arg2)) {
        $toon = avatar_load(arg(1));
        if ($toon->type == \'wowtoon\') {
          return arg(1);
        }
     }
return 0;';
$handler->display->display_options['arguments']['keyid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['keyid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['keyid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['keyid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['keyid']['glossary'] = 0;
$handler->display->display_options['arguments']['keyid']['limit'] = '0';
$handler->display->display_options['arguments']['keyid']['transform_dash'] = 0;
$handler->display->display_options['arguments']['keyid']['break_phrase'] = 0;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Activity Feed Item: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'toon_feed';
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['block_caching'] = '4';

/* Display: Guild Achievements */
$handler = $view->new_display('page', 'Guild Achievements', 'guild_achievement_page');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '20';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '1';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'achid' => 'achid',
  'keyid' => 'keyid',
  'icon' => 'icon',
  'description' => 'description',
  'dateCompleted' => 'dateCompleted',
);
$handler->display->display_options['style_options']['default'] = 'dateCompleted';
$handler->display->display_options['style_options']['info'] = array(
  'achid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'keyid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'icon' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'description' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'dateCompleted' => array(
    'sortable' => 0,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Activity Feed Item: Keyid */
$handler->display->display_options['arguments']['keyid']['id'] = 'keyid';
$handler->display->display_options['arguments']['keyid']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['arguments']['keyid']['field'] = 'keyid';
$handler->display->display_options['arguments']['keyid']['default_action'] = 'default';
$handler->display->display_options['arguments']['keyid']['default_argument_type'] = 'php';
$handler->display->display_options['arguments']['keyid']['default_argument_options']['code'] = 'return wowguild_get_guild_gid();';
$handler->display->display_options['arguments']['keyid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['keyid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['keyid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['keyid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['keyid']['glossary'] = 0;
$handler->display->display_options['arguments']['keyid']['limit'] = '0';
$handler->display->display_options['arguments']['keyid']['transform_dash'] = 0;
$handler->display->display_options['arguments']['keyid']['break_phrase'] = 0;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Activity Feed Item: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'wowtoon_feed_item';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'guild_feed';
$handler->display->display_options['path'] = 'guild_achievements';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Guild Achievements';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;



  $views[$view->name] = $view;


  /****************************/
  /*** BEGIN WOWGUILD VIEWS ***/
  /****************************/
  $view = new view;
  $view->name = 'wowguilds';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'wowguild_entity';
  $view->human_name = 'WoWGuilds';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'WoW Guilds';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: WoW Guild: Wow guild ID */
  $handler->display->display_options['fields']['gid']['id'] = 'gid';
  $handler->display->display_options['fields']['gid']['table'] = 'wowguild_entity';
  $handler->display->display_options['fields']['gid']['field'] = 'gid';
  /* Field: WoW Guild: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'wowguild_entity';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'guild/[gid]';
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  /* Field: Field: Realm */
  $handler->display->display_options['fields']['field_wowtoon_realm']['id'] = 'field_wowtoon_realm';
  $handler->display->display_options['fields']['field_wowtoon_realm']['table'] = 'field_data_field_wowtoon_realm';
  $handler->display->display_options['fields']['field_wowtoon_realm']['field'] = 'field_wowtoon_realm';
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_wowtoon_realm']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_wowtoon_realm']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_wowtoon_realm']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_wowtoon_realm']['field_api_classes'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/wowguild';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'WoW Guilds';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;

  $views[$view->name] = $view;

  return $views;

}
