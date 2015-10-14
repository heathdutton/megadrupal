<?php
/**
 * @file
 * youtube_importer_feature.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function youtube_importer_feature_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'yt_importer';
  $feeds_importer->config = array(
    'name' => 'YT importer',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsJSONPathParser',
      'config' => array(
        'context' => '$.data.items.*',
        'sources' => array(
          'jsonpath_parser:0' => 'video.id',
          'jsonpath_parser:1' => 'video.title',
          'jsonpath_parser:2' => 'video.uploaded',
          'jsonpath_parser:3' => 'video.updated',
          'jsonpath_parser:4' => 'id',
          'jsonpath_parser:5' => 'video.id',
          'jsonpath_parser:6' => 'video.uploader',
          'jsonpath_parser:7' => 'video.title',
          'jsonpath_parser:8' => 'video.description',
          'jsonpath_parser:9' => 'video.player.default',
          'jsonpath_parser:10' => 'video.player.mobile',
          'jsonpath_parser:11' => 'video.content.1',
          'jsonpath_parser:12' => 'video.content.6',
          'jsonpath_parser:13' => 'video.content.5',
          'jsonpath_parser:14' => 'video.duration',
          'jsonpath_parser:15' => 'video.aspectRatio',
          'jsonpath_parser:16' => 'video.rating',
          'jsonpath_parser:17' => 'video.likeCount',
          'jsonpath_parser:18' => 'video.ratingCount',
          'jsonpath_parser:19' => 'video.viewCount',
          'jsonpath_parser:20' => 'video.favoriteCount',
          'jsonpath_parser:21' => 'video.commentCount',
          'jsonpath_parser:22' => 'video.status.value',
          'jsonpath_parser:23' => 'video.status.reason',
          'jsonpath_parser:24' => 'video.restrictions.type',
          'jsonpath_parser:25' => 'video.restrictions.relationship',
          'jsonpath_parser:26' => 'video.restrictions.countries',
          'jsonpath_parser:27' => 'video.accessControl.comment',
          'jsonpath_parser:28' => 'video.accessControl.commentVote',
          'jsonpath_parser:29' => 'video.accessControl.videoRespond',
          'jsonpath_parser:30' => 'video.accessControl.rate',
          'jsonpath_parser:31' => 'video.accessControl.embed',
          'jsonpath_parser:32' => 'video.accessControl.list',
          'jsonpath_parser:33' => 'video.accessControl.autoplay',
          'jsonpath_parser:34' => 'video.accessControl.syndicate',
          'jsonpath_parser:35' => 'video.id',
          'jsonpath_parser:36' => 'video.thumbnail.sqDefault',
          'jsonpath_parser:37' => 'video.thumbnail.hqDefault',
          'jsonpath_parser:38' => 'video.category',
          'jsonpath_parser:39' => 'video.tags.*',
        ),
        'debug' => array(
          'options' => array(
            'context' => 0,
            'jsonpath_parser:0' => 0,
            'jsonpath_parser:1' => 0,
            'jsonpath_parser:2' => 0,
            'jsonpath_parser:3' => 0,
            'jsonpath_parser:4' => 0,
            'jsonpath_parser:5' => 0,
            'jsonpath_parser:6' => 0,
            'jsonpath_parser:7' => 0,
            'jsonpath_parser:8' => 0,
            'jsonpath_parser:9' => 0,
            'jsonpath_parser:10' => 0,
            'jsonpath_parser:11' => 0,
            'jsonpath_parser:12' => 0,
            'jsonpath_parser:13' => 0,
            'jsonpath_parser:14' => 0,
            'jsonpath_parser:15' => 0,
            'jsonpath_parser:16' => 0,
            'jsonpath_parser:17' => 0,
            'jsonpath_parser:18' => 0,
            'jsonpath_parser:19' => 0,
            'jsonpath_parser:20' => 0,
            'jsonpath_parser:21' => 0,
            'jsonpath_parser:22' => 0,
            'jsonpath_parser:23' => 0,
            'jsonpath_parser:24' => 0,
            'jsonpath_parser:25' => 0,
            'jsonpath_parser:26' => 0,
            'jsonpath_parser:27' => 0,
            'jsonpath_parser:28' => 0,
            'jsonpath_parser:29' => 0,
            'jsonpath_parser:30' => 0,
            'jsonpath_parser:31' => 0,
            'jsonpath_parser:32' => 0,
            'jsonpath_parser:33' => 0,
            'jsonpath_parser:34' => 0,
            'jsonpath_parser:35' => 0,
            'jsonpath_parser:36' => 0,
            'jsonpath_parser:37' => 0,
            'jsonpath_parser:38' => 0,
            'jsonpath_parser:39' => 0,
          ),
        ),
        'allow_override' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'youtube_video',
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'jsonpath_parser:0',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'jsonpath_parser:1',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'jsonpath_parser:2',
            'target' => 'field_yt_uploaded:start',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'jsonpath_parser:3',
            'target' => 'field_yt_updated:start',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'jsonpath_parser:4',
            'target' => 'field_yt_fup',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'jsonpath_parser:5',
            'target' => 'field_yt_vid',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'jsonpath_parser:6',
            'target' => 'field_yt_uploader',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'jsonpath_parser:7',
            'target' => 'field_yt_title',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'jsonpath_parser:8',
            'target' => 'field_yt_description',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'jsonpath_parser:9',
            'target' => 'field_yt_player_default',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'jsonpath_parser:10',
            'target' => 'field_yt_player_mobile',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'jsonpath_parser:11',
            'target' => 'field_yt_rtsp1',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'jsonpath_parser:12',
            'target' => 'field_yt_rtsp2',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'jsonpath_parser:13',
            'target' => 'field_yt_http_url',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'jsonpath_parser:14',
            'target' => 'field_yt_duration',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'jsonpath_parser:15',
            'target' => 'field_yt_aspect_ratio',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'jsonpath_parser:16',
            'target' => 'field_yt_rating',
            'unique' => FALSE,
          ),
          17 => array(
            'source' => 'jsonpath_parser:17',
            'target' => 'field_yt_likecount',
            'unique' => FALSE,
          ),
          18 => array(
            'source' => 'jsonpath_parser:18',
            'target' => 'field_yt_ratingcount',
            'unique' => FALSE,
          ),
          19 => array(
            'source' => 'jsonpath_parser:19',
            'target' => 'field_yt_viewcount',
            'unique' => FALSE,
          ),
          20 => array(
            'source' => 'jsonpath_parser:20',
            'target' => 'field_yt_favoritecount',
            'unique' => FALSE,
          ),
          21 => array(
            'source' => 'jsonpath_parser:21',
            'target' => 'field_yt_commentcount',
            'unique' => FALSE,
          ),
          22 => array(
            'source' => 'jsonpath_parser:22',
            'target' => 'field_yt_status_value',
            'unique' => FALSE,
          ),
          23 => array(
            'source' => 'jsonpath_parser:23',
            'target' => 'field_yt_status_reason',
            'unique' => FALSE,
          ),
          24 => array(
            'source' => 'jsonpath_parser:24',
            'target' => 'field_yt_restrictions_type',
            'unique' => FALSE,
          ),
          25 => array(
            'source' => 'jsonpath_parser:25',
            'target' => 'field_yt_restrictions_relation',
            'unique' => FALSE,
          ),
          26 => array(
            'source' => 'jsonpath_parser:26',
            'target' => 'field_yt_restrictions_country',
            'unique' => FALSE,
          ),
          27 => array(
            'source' => 'jsonpath_parser:27',
            'target' => 'field_yt_ac_comment',
            'unique' => FALSE,
          ),
          28 => array(
            'source' => 'jsonpath_parser:28',
            'target' => 'field_yt_ac_commentvote',
            'unique' => FALSE,
          ),
          29 => array(
            'source' => 'jsonpath_parser:29',
            'target' => 'field_yt_ac_videorespond',
            'unique' => FALSE,
          ),
          30 => array(
            'source' => 'jsonpath_parser:30',
            'target' => 'field_yt_ac_rate',
            'unique' => FALSE,
          ),
          31 => array(
            'source' => 'jsonpath_parser:31',
            'target' => 'field_yt_ac_embed',
            'unique' => FALSE,
          ),
          32 => array(
            'source' => 'jsonpath_parser:32',
            'target' => 'field_yt_ac_list',
            'unique' => FALSE,
          ),
          33 => array(
            'source' => 'jsonpath_parser:33',
            'target' => 'field_yt_ac_autoplay',
            'unique' => FALSE,
          ),
          34 => array(
            'source' => 'jsonpath_parser:34',
            'target' => 'field_yt_ac_syndicate',
            'unique' => FALSE,
          ),
          35 => array(
            'source' => 'jsonpath_parser:35',
            'target' => 'field_yt_play',
            'unique' => FALSE,
          ),
          36 => array(
            'source' => 'jsonpath_parser:36',
            'target' => 'field_yt_sqdefault',
            'unique' => FALSE,
          ),
          37 => array(
            'source' => 'jsonpath_parser:37',
            'target' => 'field_yt_hqdefault',
            'unique' => FALSE,
          ),
          38 => array(
            'source' => 'jsonpath_parser:38',
            'target' => 'field_yt_category',
            'unique' => FALSE,
          ),
          39 => array(
            'source' => 'jsonpath_parser:39',
            'target' => 'field_yt_tags',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '1',
        'input_format' => 'full_html',
      ),
    ),
    'content_type' => 'youtube_importer',
    'update' => 0,
    'import_period' => '259200',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['yt_importer'] = $feeds_importer;

  return $export;
}
