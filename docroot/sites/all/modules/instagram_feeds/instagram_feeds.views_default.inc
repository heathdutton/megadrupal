<?php
/**
 * @file
 * instagram_feeds.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function instagram_feeds_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'instagram_feeds_content';
  $view->description = 'Generate query that gives needed instagram images for rendering instagram block.';
  $view->tag = 'instagram';
  $view->base_table = 'node';
  $view->human_name = 'Instagram Feeds Content';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'clearfix';
  $handler->display->display_options['hide_admin_links'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'inst-image';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* Relationship: Instagram Username term */
  $handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
  $handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
  $handler->display->display_options['relationships']['term_node_tid']['ui_name'] = 'Instagram Username term';
  $handler->display->display_options['relationships']['term_node_tid']['label'] = 'user_term';
  $handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
    'instagram_users' => 'instagram_users',
    'blog' => 0,
    'event' => 0,
    'faq' => 0,
    'instagram_hashtags' => 0,
    'media_folders' => 0,
    'newsletter_category' => 0,
    'playlist' => 0,
    'product_brands' => 0,
    'product_category' => 0,
    'product_certifiers' => 0,
    'promo_category' => 0,
    'recipe' => 0,
    'recipe_category' => 0,
    'recipe_course' => 0,
    'recipe_cusine' => 0,
    'recipe_featured_in' => 0,
    'recipe_main_ingredient' => 0,
    'recipe_occasions' => 0,
    'recipe_prep_method' => 0,
    'tags' => 0,
    'twitter_topical_accounts' => 0,
    'vendor_type' => 0,
    'video_keywords' => 0,
  );
  /* Relationship: Flags: flag_as_inappropriate */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['ui_name'] = 'Appropriate / Inappropriate';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'flag_as_inappropriate';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Field: span class for videos */
  $handler->display->display_options['fields']['field_instf_marker']['id'] = 'field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['table'] = 'field_data_field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['field'] = 'field_instf_marker';
  $handler->display->display_options['fields']['field_instf_marker']['ui_name'] = 'span class for videos';
  $handler->display->display_options['fields']['field_instf_marker']['label'] = '';
  $handler->display->display_options['fields']['field_instf_marker']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['alter']['text'] = '<span class="video">&#9654;</span>';
  $handler->display->display_options['fields']['field_instf_marker']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_marker']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_instf_marker']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_instf_marker']['type'] = 'list_key';
  /* Field: Thumb URL */
  $handler->display->display_options['fields']['field_instf_thumb_url']['id'] = 'field_instf_thumb_url';
  $handler->display->display_options['fields']['field_instf_thumb_url']['table'] = 'field_data_field_instf_thumb_url';
  $handler->display->display_options['fields']['field_instf_thumb_url']['field'] = 'field_instf_thumb_url';
  $handler->display->display_options['fields']['field_instf_thumb_url']['ui_name'] = 'Thumb URL';
  $handler->display->display_options['fields']['field_instf_thumb_url']['label'] = '';
  $handler->display->display_options['fields']['field_instf_thumb_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_thumb_url']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_instf_thumb_url']['alter']['text'] = '<img src="[field_instf_thumb_url-url]" alt="" />';
  $handler->display->display_options['fields']['field_instf_thumb_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_thumb_url']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_instf_thumb_url']['type'] = 'link_plain';
  /* Field: URL to Instagram */
  $handler->display->display_options['fields']['field_instf_link_url']['id'] = 'field_instf_link_url';
  $handler->display->display_options['fields']['field_instf_link_url']['table'] = 'field_data_field_instf_link_url';
  $handler->display->display_options['fields']['field_instf_link_url']['field'] = 'field_instf_link_url';
  $handler->display->display_options['fields']['field_instf_link_url']['ui_name'] = 'URL to Instagram';
  $handler->display->display_options['fields']['field_instf_link_url']['label'] = '';
  $handler->display->display_options['fields']['field_instf_link_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_link_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_link_url']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_instf_link_url']['type'] = 'link_title_plain';
  /* Field: Original Image */
  $handler->display->display_options['fields']['field_instf_image_url']['id'] = 'field_instf_image_url';
  $handler->display->display_options['fields']['field_instf_image_url']['table'] = 'field_data_field_instf_image_url';
  $handler->display->display_options['fields']['field_instf_image_url']['field'] = 'field_instf_image_url';
  $handler->display->display_options['fields']['field_instf_image_url']['ui_name'] = 'Original Image';
  $handler->display->display_options['fields']['field_instf_image_url']['label'] = '';
  $handler->display->display_options['fields']['field_instf_image_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_image_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_image_url']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_instf_image_url']['type'] = 'link_plain';
  /* Field: Flag as inappropriate */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flagging';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['ops']['ui_name'] = 'Flag as inappropriate';
  $handler->display->display_options['fields']['ops']['label'] = '';
  $handler->display->display_options['fields']['ops']['exclude'] = TRUE;
  $handler->display->display_options['fields']['ops']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['ops']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['ops']['link_type'] = 'toggle';
  /* Field: Username */
  $handler->display->display_options['fields']['field_instf_user']['id'] = 'field_instf_user';
  $handler->display->display_options['fields']['field_instf_user']['table'] = 'field_data_field_instf_user';
  $handler->display->display_options['fields']['field_instf_user']['field'] = 'field_instf_user';
  $handler->display->display_options['fields']['field_instf_user']['ui_name'] = 'Username';
  $handler->display->display_options['fields']['field_instf_user']['label'] = '';
  $handler->display->display_options['fields']['field_instf_user']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_user']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_instf_user']['alter']['text'] = '<strong>@[field_instf_user]</strong>';
  $handler->display->display_options['fields']['field_instf_user']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_instf_user']['alter']['path'] = 'http://instagram.com/[field_instf_user]#';
  $handler->display->display_options['fields']['field_instf_user']['alter']['external'] = TRUE;
  $handler->display->display_options['fields']['field_instf_user']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_instf_user']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_user']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Description */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['ui_name'] = 'Description';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_plain';
  /* Field: # of likes */
  $handler->display->display_options['fields']['field_instf_like_count']['id'] = 'field_instf_like_count';
  $handler->display->display_options['fields']['field_instf_like_count']['table'] = 'field_data_field_instf_like_count';
  $handler->display->display_options['fields']['field_instf_like_count']['field'] = 'field_instf_like_count';
  $handler->display->display_options['fields']['field_instf_like_count']['ui_name'] = '# of likes';
  $handler->display->display_options['fields']['field_instf_like_count']['label'] = '';
  $handler->display->display_options['fields']['field_instf_like_count']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_like_count']['alter']['text'] = '[field_instf_like_count-value]';
  $handler->display->display_options['fields']['field_instf_like_count']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_like_count']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_instf_like_count']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 0,
  );
  /* Field: # of comments */
  $handler->display->display_options['fields']['field_instf_comments_count']['id'] = 'field_instf_comments_count';
  $handler->display->display_options['fields']['field_instf_comments_count']['table'] = 'field_data_field_instf_comments_count';
  $handler->display->display_options['fields']['field_instf_comments_count']['field'] = 'field_instf_comments_count';
  $handler->display->display_options['fields']['field_instf_comments_count']['ui_name'] = '# of comments';
  $handler->display->display_options['fields']['field_instf_comments_count']['label'] = '';
  $handler->display->display_options['fields']['field_instf_comments_count']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_instf_comments_count']['alter']['text'] = '[field_instf_comments_count-value]';
  $handler->display->display_options['fields']['field_instf_comments_count']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_instf_comments_count']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 0,
  );
  /* Field: Popup */
  $handler->display->display_options['fields']['colorbox']['id'] = 'colorbox';
  $handler->display->display_options['fields']['colorbox']['table'] = 'colorbox';
  $handler->display->display_options['fields']['colorbox']['field'] = 'colorbox';
  $handler->display->display_options['fields']['colorbox']['ui_name'] = 'Popup';
  $handler->display->display_options['fields']['colorbox']['label'] = '';
  $handler->display->display_options['fields']['colorbox']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['colorbox']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['colorbox']['trigger_field'] = 'field_instf_thumb_url';
  $handler->display->display_options['fields']['colorbox']['popup'] = '<div class="image-wrap"><a href="[field_instf_link_url]" title="View in Instagram">[field_instf_image_url]</a></div>';
  $handler->display->display_options['fields']['colorbox']['caption'] = '[body]';
  $handler->display->display_options['fields']['colorbox']['gid'] = 1;
  $handler->display->display_options['fields']['colorbox']['custom_gid'] = 'gallery_';
  $handler->display->display_options['fields']['colorbox']['width'] = '';
  $handler->display->display_options['fields']['colorbox']['height'] = '';
  /* Sort criterion: Date when Instagram image created */
  $handler->display->display_options['sorts']['field_instf_created_value']['id'] = 'field_instf_created_value';
  $handler->display->display_options['sorts']['field_instf_created_value']['table'] = 'field_data_field_instf_created';
  $handler->display->display_options['sorts']['field_instf_created_value']['field'] = 'field_instf_created_value';
  $handler->display->display_options['sorts']['field_instf_created_value']['ui_name'] = 'Date when Instagram image created';
  $handler->display->display_options['sorts']['field_instf_created_value']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Contextual filter: Content: Hash Tags (field_instf_hash_tags) */
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['id'] = 'field_instf_hash_tags_tid';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['table'] = 'field_data_field_instf_hash_tags';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['field'] = 'field_instf_hash_tags_tid';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['default_argument_options']['index'] = '0';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['validate_options']['vocabularies'] = array(
    'instagram_hashtags' => 'instagram_hashtags',
  );
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['validate_options']['type'] = 'tids';
  $handler->display->display_options['arguments']['field_instf_hash_tags_tid']['break_phrase'] = TRUE;
  /* Contextual filter: Content: Instagram Username (field_instf_user) */
  $handler->display->display_options['arguments']['field_instf_user_tid']['id'] = 'field_instf_user_tid';
  $handler->display->display_options['arguments']['field_instf_user_tid']['table'] = 'field_data_field_instf_user';
  $handler->display->display_options['arguments']['field_instf_user_tid']['field'] = 'field_instf_user_tid';
  $handler->display->display_options['arguments']['field_instf_user_tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_instf_user_tid']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_instf_user_tid']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['field_instf_user_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_instf_user_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_instf_user_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_instf_user_tid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_instf_user_tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['field_instf_user_tid']['validate_options']['vocabularies'] = array(
    'instagram_users' => 'instagram_users',
  );
  $handler->display->display_options['arguments']['field_instf_user_tid']['validate_options']['type'] = 'tids';
  $handler->display->display_options['arguments']['field_instf_user_tid']['validate']['fail'] = 'ignore';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'instagram_media_item' => 'instagram_media_item',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Instagram User blacklist */
  $handler->display->display_options['filters']['field_instf_blacklist_type_value']['id'] = 'field_instf_blacklist_type_value';
  $handler->display->display_options['filters']['field_instf_blacklist_type_value']['table'] = 'field_data_field_instf_blacklist_type';
  $handler->display->display_options['filters']['field_instf_blacklist_type_value']['field'] = 'field_instf_blacklist_type_value';
  $handler->display->display_options['filters']['field_instf_blacklist_type_value']['relationship'] = 'term_node_tid';
  $handler->display->display_options['filters']['field_instf_blacklist_type_value']['ui_name'] = 'Instagram User blacklist';
  $handler->display->display_options['filters']['field_instf_blacklist_type_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_instf_blacklist_type_value']['value'] = array(
    'black' => 'black',
  );
  /* Filter criterion: Inappropriate */
  $handler->display->display_options['filters']['flagged']['id'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['filters']['flagged']['ui_name'] = 'Inappropriate';
  $handler->display->display_options['filters']['flagged']['value'] = '0';
  $export['instagram_feeds_content'] = $view;

  return $export;
}
