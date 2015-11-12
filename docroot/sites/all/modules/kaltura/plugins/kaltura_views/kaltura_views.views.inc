<?php
/**
 * @file
 * Provide views data and handlers for kaltura.module.
 */

/**
 * Implements hook_views_data().
 *
 * Provides the views module with data about node_kaltura table.
 * Enables the user to add fields to a view and filter/sort by some fields.
 * Some of the fields have only a field handler, which means they could be
 * added to the "fields" list of the view. Others have a filter handler so
 * the view could be filtered by them.
 *
 * The structure of the return value applies the views2 api.
 */
function kaltura_views_data() {
  $data['node_kaltura'] = array(
    'table' => array(
      'group' => 'kaltura',
      'title' => 'kaltura',
      'help' => t('table that holds Kaltura entries'),
      'base' => array(
        'field' => 'kaltura_entryid',
        'title' => t('Kaltura entry'),
        'help' => t('All Kaltura media entries'),
      ),
    ),
    'kaltura_media_type' => array(
      'title' => t('Kaltura entry media type'),
      'help' => t('see kaltura documentation for values'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_media_type',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'kaltura_views_handler_filter_media_type',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'kstatus' => array(
      'title' => t('Kaltura entry media status'),
      'help' => t('see kaltura documentation for values'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_media_status',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'kaltura_views_handler_filter_media_status',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'kaltura_plays' => array(
      'title' => t('Kaltura item number of plays'),
      'help' => t('Kaltura item number of plays'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_views' => array(
      'title' => t('Kaltura item number of views'),
      'help' => t('see Kaltura documentation for values'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_tags' => array(
      'title' => t('kaltura item tags'),
      'help' => t('kaltura item tags'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'categories' => array(
      'title' => t('Categories'),
      'help' => t('Categories the media belongs to.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_duration' => array(
      'title' => t('Kaltura item duration'),
      'help' => t('Kaltura item duration'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_duration',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_total_rank' => array(
      'title' => t('Kaltura item total rank'),
      'help' => t('total rank is rank multiplied by 1000'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_votes' => array(
      'title' => t('Kaltura item number of votes'),
      'help' => t('see Kaltura documentation for values'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_votes',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_rank' => array(
      'title' => t('Kaltura item rank'),
      'help' => t('see Kaltura documentation for values'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click_sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_thumbnail_url' => array(
      'title' => t('Kaltura item thumbnail'),
      'help' => t('Kaltura item thumbnail'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_thumb',
      ),
    ),
    'kaltura_source_link' => array(
      'title' => t('Kaltura item source link'),
      'help' => t('Kaltura item source link'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),
    'kaltura_download_url' => array(
      'title' => t('Kaltura item download link'),
      'help' => t('Kaltura item download link'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_url',
      ),
    ),
    'kaltura_media_date' => array(
      'title' => t('Kaltura image item media date (EXIF data)'),
      'help' => t('Kaltura image item media date (EXIF data)'),
      'field' => array(
        'handler' => 'views_handler_field_date',
      ),
    ),
    'kaltura_created_date' => array(
      'title' => t('Kaltura media created date'),
      'help' => t('Kaltura media created date'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
    ),
    'kaltura_entryid' => array(
      'title' => t('Kaltura media'),
      'help' => t('Kaltura entry'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_media',
      ),
    ),
    'kaltura_text_entryid' => array(
      'title' => t('Kaltura entry as text'),
      'help' => t('Kaltura entry as as text'),
      'field' => array(
        'field' => 'kaltura_entryid',
        'handler' => 'views_handler_field',
      ),
    ),
    'kaltura_edit_metadata' => array(
      'title' => t('Kaltura edit metadata'),
      'help' => t('Kaltura edit metadata link'),
      'field' => array(
        'field' => 'kaltura_entryid',
        'handler' => 'kaltura_views_handler_field_kaltura_edit_meta',
      ),
    ),
    'kaltura_text' => array(
      'title' => t('Kaltura text'),
      'help' => t('All entry free text'),
      'filter' => array(
        'field' => 'kaltura_title',
        'handler' => 'kaltura_views_handler_filter_kaltura_text',
      ),
    ),
    'kaltura_title' => array(
      'title' => t('Kaltura title'),
      'help' => t('Kaltura entry title'),
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
    ),
    'kaltura_description' => array(
      'title' => t('Kaltura description'),
      'help' => t('Kaltura entry description'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    // Expose the owner uid.
    'kaltura_puser_id' => array(
      'title' => t('Owner'),
      'help' => t('Relate a item to the user it belongs to.'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'field' => 'uid',
        'label' => t('Entry owner'),
      ),
    ),
  );

  return $data;
}
