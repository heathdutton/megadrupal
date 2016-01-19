<?php
/**
 * @file
 * Views module integration for Yandex.Webmaster original texts.
 */

/**
 * Implements hook_views_data_alter().
 */
function yandex_webmaster_original_texts_views_data_alter(&$data) {
  if (isset($data['yandex_webmaster_original_texts'])) {
    $table = &$data['yandex_webmaster_original_texts'];
    $table['table']['group'] = t('Yandex.Webmaster');
    $table['table']['base']['help'] = t('Texts sent to Yandex.Webmaster original texts.');

    $table['host_id']['title'] = t('Host ID');
    $table['host_id']['help'] = t('The ID of the host this original text was added to.');
    $table['host_id']['field']['handler'] = 'views_handler_field_numeric';
    $table['host_id']['filter']['handler'] = 'views_handler_filter_numeric';
    $table['host_id']['argument']['handler'] = 'views_handler_argument_numeric';
    $table['host_name'] = array(
      'real field' => 'host_id',
      'title' => t('Host name'),
      'help' => t('The name of the host this original text was added to.'),
      'field' => array(
        'handler' => 'yandex_webmaster_handler_field_yandex_webmaster_host_name',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    );
    $table['text_md5']['title'] = t('Text MD5');
    $table['text_md5']['help'] = t('MD5 hash of the text.');
    $table['text'] = array(
      'title' => t('Text'),
      'help' => t('Text sent to Yandex.Webmaster original texts.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );
    $table['timestamp']['title'] = t('Sent');
    $table['timestamp']['help'] = t('Date and time when the text was sent to Yandex.Webmaster original texts.');
    $table['timestamp']['field']['handler'] = 'views_handler_field_date';
    //$table['timestamp']['field']['click sortable'] = FALSE;
    $table['timestamp']['sort']['handler'] = 'views_handler_sort_date';
    $table['timestamp']['filter']['handler'] = 'views_handler_filter_date';
    $table['timestamp']['argument']['handler'] = 'views_handler_argument_date';
    $table['code']['title'] = t('Response code');
    $table['code']['help'] = t('Yandex.Webmaster response code.');
    $table['error_code']['title'] = t('Error code');
    $table['error_code']['help'] = t('Yandex.Webmaster error code.');
    $table['id']['title'] = t('Text ID');
    $table['id']['help'] = t('Yandex.Webmaster original text ID.');
    $table['link']['title'] = t('Text URL');
    $table['link']['help'] = t('Yandex.Webmaster original text URL.');
    $table['content'] = array(
      'title' => t('Excerpt'),
      'help' => t('Yandex.Webmaster original text excerpt.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    );
  }
}
