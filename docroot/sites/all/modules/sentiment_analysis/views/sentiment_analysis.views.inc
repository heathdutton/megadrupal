<?php

/**
 * Implements hook_views_data().
 */
function sentiment_analysis_views_data() {
  // Adding Custom Table to Views.
  $table = array(
    // Views Data name.
    'sentiment_analysis_details' => array(
      'table' => array(
        'group' => 'Sentiment Analysis',
        'base' => array(
          'field' => 'sentiment',
          'title' => 'Sentiment',
          'help' => 'Sentiment Analysis Details'
        )
      ),
      'sentiment' => array(
        'title' => t('Sentiment'),
        'help' => t('Sentiment Name'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'handler' => 'views_handler_sort'
        )
      ),
      'score' => array(
        'title' => t('Sentiment Score'),
        'help' => t('Sentiment Score of Sentence'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'handler' => 'views_handler_sort'
        )
      ),
      'uid' => array(
        'title' => t('User ID'),
        'help' => t('User ID that inputted Value'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'handler' => 'views_handler_sort'
        )
      ),
      'nid' => array(
        'title' => t('NID'),
        'help' => t('Node ID'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'handler' => 'views_handler_sort'
        )
      ),
      'cid' => array(
        'title' => t('Comment ID'),
        'help' => t('Comment ID'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'handler' => 'views_handler_sort'
        )
      ),
      'page_url' => array(
        'title' => t('Page URL'),
        'help' => t('Page URL'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'handler' => 'views_handler_sort'
        )
      ),
      'time' => array(
        'title' => t('Time'),
        'help' => t('Time'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'handler' => 'views_handler_sort'
        )
      ),
      'sentence_description' => array(
        'title' => t('Sentiment Sentence'),
        'help' => t('Sentiment Sentence'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort' => array(
          'handler' => 'views_handler_sort'
        )
      ),
    )
  );
  return $table;
}
