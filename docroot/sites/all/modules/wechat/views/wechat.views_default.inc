<?php
/**
 * @file
 * Provide default_views for wechat.
 */

/**
 * Implements hook_views_default_views().
 */
function wechat_views_default_views() {
$view = new view();
$view->name = 'request_messages';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'wechat_request_message';
$view->human_name = 'request messages';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'request messages';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['reset_button_label'] = '重置';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['tags']['first'] = '« 第一页';
$handler->display->display_options['pager']['options']['tags']['previous'] = '‹ 前一页';
$handler->display->display_options['pager']['options']['tags']['next'] = '下一页 ›';
$handler->display->display_options['pager']['options']['tags']['last'] = '末页 »';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'id' => 'id',
  'create_time' => 'create_time',
  'wechat_event' => 'wechat_event',
  'wechat_event_key' => 'wechat_event_key',
  'from_user_name' => 'from_user_name',
  'wechat_latitude' => 'wechat_latitude',
  'wechat_location_x' => 'wechat_location_x',
  'wechat_location_y' => 'wechat_location_y',
  'wechat_longitude' => 'wechat_longitude',
  'msg_id' => 'msg_id',
  'msg_type' => 'msg_type',
  'wechat_scale' => 'wechat_scale',
  'wechat_ticket' => 'wechat_ticket',
  'to_user_name' => 'to_user_name',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'create_time' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'wechat_event' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'wechat_event_key' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'from_user_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'wechat_latitude' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'wechat_location_x' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'wechat_location_y' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'wechat_longitude' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'msg_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'msg_type' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'wechat_scale' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'wechat_ticket' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'to_user_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: Wechat request message: wechat user */
$handler->display->display_options['relationships']['from_user_name']['id'] = 'from_user_name';
$handler->display->display_options['relationships']['from_user_name']['table'] = 'wechat_request_message';
$handler->display->display_options['relationships']['from_user_name']['field'] = 'from_user_name';
/* 字段: Wechat request message: Wechat request message ID */
$handler->display->display_options['fields']['id']['id'] = 'id';
$handler->display->display_options['fields']['id']['table'] = 'wechat_request_message';
$handler->display->display_options['fields']['id']['field'] = 'id';
$handler->display->display_options['fields']['id']['exclude'] = TRUE;
$handler->display->display_options['fields']['id']['separator'] = '';
/* 字段: Wechat request message: Create time */
$handler->display->display_options['fields']['create_time']['id'] = 'create_time';
$handler->display->display_options['fields']['create_time']['table'] = 'wechat_request_message';
$handler->display->display_options['fields']['create_time']['field'] = 'create_time';
$handler->display->display_options['fields']['create_time']['label'] = '创建日期';
$handler->display->display_options['fields']['create_time']['date_format'] = 'short';
$handler->display->display_options['fields']['create_time']['custom_date_format'] = 'Y-m-d';
$handler->display->display_options['fields']['create_time']['second_date_format'] = 'long';
/* 字段: Wechat user: headimgurl */
$handler->display->display_options['fields']['headimgurl']['id'] = 'headimgurl';
$handler->display->display_options['fields']['headimgurl']['table'] = 'wechat_user';
$handler->display->display_options['fields']['headimgurl']['field'] = 'headimgurl';
$handler->display->display_options['fields']['headimgurl']['relationship'] = 'from_user_name';
$handler->display->display_options['fields']['headimgurl']['label'] = '用户头像';
$handler->display->display_options['fields']['headimgurl']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['headimgurl']['alter']['text'] = '<img src="[headimgurl]" width="50px" height="50px" />';
/* 字段: Wechat user: nickname */
$handler->display->display_options['fields']['nickname']['id'] = 'nickname';
$handler->display->display_options['fields']['nickname']['table'] = 'wechat_user';
$handler->display->display_options['fields']['nickname']['field'] = 'nickname';
$handler->display->display_options['fields']['nickname']['relationship'] = 'from_user_name';
$handler->display->display_options['fields']['nickname']['label'] = '用户';
/* 字段: Wechat request message: Message type */
$handler->display->display_options['fields']['msg_type']['id'] = 'msg_type';
$handler->display->display_options['fields']['msg_type']['table'] = 'wechat_request_message';
$handler->display->display_options['fields']['msg_type']['field'] = 'msg_type';
$handler->display->display_options['fields']['msg_type']['label'] = '消息类型';
/* 字段: Wechat request message: Rendered wechat request message */
$handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
$handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_wechat_request_message';
$handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
$handler->display->display_options['fields']['rendered_entity']['label'] = '消息内容';
$handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
$handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
$handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
$handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
/* 字段: Global: Custom text */
$handler->display->display_options['fields']['nothing']['id'] = 'nothing';
$handler->display->display_options['fields']['nothing']['table'] = 'views';
$handler->display->display_options['fields']['nothing']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing']['label'] = '文本回复';
$handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
$handler->display->display_options['fields']['nothing']['alter']['text'] = '文本';
$handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['nothing']['alter']['path'] = 'response-message/add/text?rm_id=[id]';
$handler->display->display_options['fields']['nothing']['alter']['target'] = '_blank';
$handler->display->display_options['fields']['nothing']['element_type'] = '0';
$handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nothing']['element_wrapper_type'] = '0';
/* 字段: Global: Custom text */
$handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
$handler->display->display_options['fields']['nothing_1']['table'] = 'views';
$handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing_1']['label'] = '图片回复';
$handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['nothing_1']['alter']['text'] = '图片';
$handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'response-message/add/image?rm_id=[id]';
$handler->display->display_options['fields']['nothing_1']['element_type'] = '0';
$handler->display->display_options['fields']['nothing_1']['element_label_type'] = '0';
$handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nothing_1']['element_wrapper_type'] = '0';
/* 字段: Global: Custom text */
$handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
$handler->display->display_options['fields']['nothing_2']['table'] = 'views';
$handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing_2']['label'] = '视频回复';
$handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
$handler->display->display_options['fields']['nothing_2']['alter']['text'] = '视频';
$handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'response-message/add/video?rm_id=[id]';
$handler->display->display_options['fields']['nothing_2']['alter']['target'] = '_blank';
$handler->display->display_options['fields']['nothing_2']['element_type'] = '0';
$handler->display->display_options['fields']['nothing_2']['element_label_type'] = '0';
$handler->display->display_options['fields']['nothing_2']['element_wrapper_type'] = '0';
/* 字段: Global: Custom text */
$handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
$handler->display->display_options['fields']['nothing_3']['table'] = 'views';
$handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing_3']['label'] = '回复';
$handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[nothing], [nothing_1], [nothing_2]';
/* Sort criterion: Wechat request message: Create time */
$handler->display->display_options['sorts']['create_time']['id'] = 'create_time';
$handler->display->display_options['sorts']['create_time']['table'] = 'wechat_request_message';
$handler->display->display_options['sorts']['create_time']['field'] = 'create_time';
$handler->display->display_options['sorts']['create_time']['order'] = 'DESC';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/wechat/messages/request';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Request messages';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'management';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
$translatables['request_messages'] = array(
  t('Master'),
  t('request messages'),
  t('more'),
  t('Apply'),
  t('重置'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('« 第一页'),
  t('‹ 前一页'),
  t('下一页 ›'),
  t('末页 »'),
  t('wechat user'),
  t('Wechat request message ID'),
  t('.'),
  t('创建日期'),
  t('用户头像'),
  t('<img src="[headimgurl]" width="50px" height="50px" />'),
  t('用户'),
  t('消息类型'),
  t('消息内容'),
  t('文本回复'),
  t('文本'),
  t('图片回复'),
  t('图片'),
  t('视频回复'),
  t('视频'),
  t('回复'),
  t('[nothing], [nothing_1], [nothing_2]'),
  t('Page'),
);

  $views[$view->name] = $view;  
  
$view = new view();
$view->name = 'wechat_users';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'wechat_user';
$view->human_name = 'wechat users';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'wechat users';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['reset_button_label'] = '重置';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '20';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['pager']['options']['tags']['first'] = '« 第一页';
$handler->display->display_options['pager']['options']['tags']['previous'] = '‹ 前一页';
$handler->display->display_options['pager']['options']['tags']['next'] = '下一页 ›';
$handler->display->display_options['pager']['options']['tags']['last'] = '末页 »';
$handler->display->display_options['style_plugin'] = 'table';
/* 字段: Wechat user: country */
$handler->display->display_options['fields']['country']['id'] = 'country';
$handler->display->display_options['fields']['country']['table'] = 'wechat_user';
$handler->display->display_options['fields']['country']['field'] = 'country';
$handler->display->display_options['fields']['country']['label'] = 'Country';
/* 字段: Wechat user: province */
$handler->display->display_options['fields']['province']['id'] = 'province';
$handler->display->display_options['fields']['province']['table'] = 'wechat_user';
$handler->display->display_options['fields']['province']['field'] = 'province';
$handler->display->display_options['fields']['province']['label'] = 'Province';
/* 字段: Wechat user: city */
$handler->display->display_options['fields']['city']['id'] = 'city';
$handler->display->display_options['fields']['city']['table'] = 'wechat_user';
$handler->display->display_options['fields']['city']['field'] = 'city';
$handler->display->display_options['fields']['city']['label'] = 'City';
/* 字段: Wechat user: headimgurl */
$handler->display->display_options['fields']['headimgurl']['id'] = 'headimgurl';
$handler->display->display_options['fields']['headimgurl']['table'] = 'wechat_user';
$handler->display->display_options['fields']['headimgurl']['field'] = 'headimgurl';
$handler->display->display_options['fields']['headimgurl']['label'] = 'Headimgurl';
$handler->display->display_options['fields']['headimgurl']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['headimgurl']['alter']['text'] = '<img src="[headimgurl]" width="50px" height="50px" />';
/* 字段: Wechat user: nickname */
$handler->display->display_options['fields']['nickname']['id'] = 'nickname';
$handler->display->display_options['fields']['nickname']['table'] = 'wechat_user';
$handler->display->display_options['fields']['nickname']['field'] = 'nickname';
$handler->display->display_options['fields']['nickname']['label'] = 'Nickname';
/* 字段: Wechat user: sex */
$handler->display->display_options['fields']['sex']['id'] = 'sex';
$handler->display->display_options['fields']['sex']['table'] = 'wechat_user';
$handler->display->display_options['fields']['sex']['field'] = 'sex';
$handler->display->display_options['fields']['sex']['label'] = 'Sex';
/* 字段: Wechat user: Subscribe time */
$handler->display->display_options['fields']['subscribe_time']['id'] = 'subscribe_time';
$handler->display->display_options['fields']['subscribe_time']['table'] = 'wechat_user';
$handler->display->display_options['fields']['subscribe_time']['field'] = 'subscribe_time';
$handler->display->display_options['fields']['subscribe_time']['label'] = 'Subscribe_time';
$handler->display->display_options['fields']['subscribe_time']['date_format'] = 'short';
$handler->display->display_options['fields']['subscribe_time']['second_date_format'] = 'long';
/* Sort criterion: Wechat user: Subscribe time */
$handler->display->display_options['sorts']['subscribe_time']['id'] = 'subscribe_time';
$handler->display->display_options['sorts']['subscribe_time']['table'] = 'wechat_user';
$handler->display->display_options['sorts']['subscribe_time']['field'] = 'subscribe_time';
$handler->display->display_options['sorts']['subscribe_time']['order'] = 'DESC';
$handler->display->display_options['sorts']['subscribe_time']['exposed'] = TRUE;
$handler->display->display_options['sorts']['subscribe_time']['expose']['label'] = 'Subscribe time';
/* Sort criterion: Wechat user: nickname */
$handler->display->display_options['sorts']['nickname']['id'] = 'nickname';
$handler->display->display_options['sorts']['nickname']['table'] = 'wechat_user';
$handler->display->display_options['sorts']['nickname']['field'] = 'nickname';
$handler->display->display_options['sorts']['nickname']['order'] = 'DESC';
$handler->display->display_options['sorts']['nickname']['exposed'] = TRUE;
$handler->display->display_options['sorts']['nickname']['expose']['label'] = 'nickname';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/wechat/wechat-users';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Wechat users';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'management';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
$translatables['wechat_users'] = array(
  t('Master'),
  t('wechat users'),
  t('more'),
  t('Apply'),
  t('重置'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('« 第一页'),
  t('‹ 前一页'),
  t('下一页 ›'),
  t('末页 »'),
  t('Country'),
  t('Province'),
  t('City'),
  t('Headimgurl'),
  t('<img src="[headimgurl]" width="50px" height="50px" />'),
  t('Nickname'),
  t('Sex'),
  t('Subscribe_time'),
  t('Subscribe time'),
  t('nickname'),
  t('Page'),
);
  $views[$view->name] = $view;
 
$view = new view();
$view->name = 'response_messages';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'wechat_response_message';
$view->human_name = 'response messages';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['reset_button_label'] = '重置';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['tags']['first'] = '« 第一页';
$handler->display->display_options['pager']['options']['tags']['previous'] = '‹ 前一页';
$handler->display->display_options['pager']['options']['tags']['next'] = '下一页 ›';
$handler->display->display_options['pager']['options']['tags']['last'] = '末页 »';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'id' => 'id',
  'create_time' => 'create_time',
  'from_user_name' => 'from_user_name',
  'to_user_name' => 'to_user_name',
  'rendered_entity' => 'rendered_entity',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'create_time' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'from_user_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'to_user_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'rendered_entity' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: Wechat response message: wechat user */
$handler->display->display_options['relationships']['to_user_name']['id'] = 'to_user_name';
$handler->display->display_options['relationships']['to_user_name']['table'] = 'wechat_response_message';
$handler->display->display_options['relationships']['to_user_name']['field'] = 'to_user_name';
/* 字段: Wechat response message: Create time */
$handler->display->display_options['fields']['create_time']['id'] = 'create_time';
$handler->display->display_options['fields']['create_time']['table'] = 'wechat_response_message';
$handler->display->display_options['fields']['create_time']['field'] = 'create_time';
$handler->display->display_options['fields']['create_time']['label'] = '创建日期';
$handler->display->display_options['fields']['create_time']['date_format'] = 'short';
$handler->display->display_options['fields']['create_time']['second_date_format'] = 'long';
/* 字段: Wechat user: headimgurl */
$handler->display->display_options['fields']['headimgurl']['id'] = 'headimgurl';
$handler->display->display_options['fields']['headimgurl']['table'] = 'wechat_user';
$handler->display->display_options['fields']['headimgurl']['field'] = 'headimgurl';
$handler->display->display_options['fields']['headimgurl']['relationship'] = 'to_user_name';
$handler->display->display_options['fields']['headimgurl']['label'] = '头像';
$handler->display->display_options['fields']['headimgurl']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['headimgurl']['alter']['text'] = '<img src="[headimgurl]" width="50px" height="50px" />';
/* 字段: Wechat user: nickname */
$handler->display->display_options['fields']['nickname']['id'] = 'nickname';
$handler->display->display_options['fields']['nickname']['table'] = 'wechat_user';
$handler->display->display_options['fields']['nickname']['field'] = 'nickname';
$handler->display->display_options['fields']['nickname']['relationship'] = 'to_user_name';
$handler->display->display_options['fields']['nickname']['label'] = '昵称';
/* 字段: Wechat response message: Rendered wechat response message */
$handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
$handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_wechat_response_message';
$handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
$handler->display->display_options['fields']['rendered_entity']['label'] = '回复内容';
$handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
$handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
$handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
$handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
/* Sort criterion: Wechat response message: Create time */
$handler->display->display_options['sorts']['create_time']['id'] = 'create_time';
$handler->display->display_options['sorts']['create_time']['table'] = 'wechat_response_message';
$handler->display->display_options['sorts']['create_time']['field'] = 'create_time';
$handler->display->display_options['sorts']['create_time']['order'] = 'DESC';

/* Display: 页面 */
$handler = $view->new_display('page', '页面', 'page_1');
$handler->display->display_options['path'] = 'admin/wechat/response-messages';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'response messages';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'management';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
$translatables['response_messages'] = array(
  t('Master'),
  t('more'),
  t('Apply'),
  t('重置'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('« 第一页'),
  t('‹ 前一页'),
  t('下一页 ›'),
  t('末页 »'),
  t('wechat user'),
  t('创建日期'),
  t('头像'),
  t('<img src="[headimgurl]" width="50px" height="50px" />'),
  t('昵称'),
  t('回复内容'),
  t('页面'),
);

  $views[$view->name] = $view; 
  return $views;
}