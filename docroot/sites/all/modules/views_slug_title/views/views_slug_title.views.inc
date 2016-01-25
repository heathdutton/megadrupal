<?php
/**
 * @file
 * Views Slug Title views data.
 */

/**
 * Implements hook_views_data().
 */
function views_slug_title_views_data() {
  $data = array();
  $data['views_slug_title']['table']['group'] = t('Slug');
  $data['views_slug_title']['table']['base'] = array(
    'field' => 'entity_id',
    'title' => t('Slug'),
  );
  $data['views_slug_title']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'entity_id',
    ),
  );
  $data['views_slug_title']['slug'] = array(
    'title' => t('Slug title'),
    'help' => t('A slug title for node entity.'),
    'field' => array(
      'field' => 'slug',
      'group' => t('Slug'),
      'handler' => 'views_handler_field',
    ),
    'argument' => array('handler' => 'ViewsSlugTitleHandlerArgumentSlug'),
    'filter' => array('handler' => 'views_handler_filter_string'),
  );
  return $data;
}
