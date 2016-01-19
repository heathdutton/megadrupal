<?php

function stop_spam_views_data() {
  return array(
    'stop_spam_links' => array(
      'table' => array(
        'group' => t('External links'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'entity_id',
            'extra' => array(
              array(
                'field' => 'entity_type',
                'value' => 'node',
              ),
            ),
          ),
          
          'comment' => array(
            'left_field' => 'cid',
            'field' => 'entity_id',
            'extra' => array(
              array(
                'field' => 'entity_type',
                'value' => 'comment',
              ),
            ),
          ),
        ),
      ),
      
      'host' => array(
        'title' => t('Host'),
        'help' => t('Host of external URL'),
        'field' => array(
          'click sortable' => TRUE,
          'handler' => 'views_handler_field',
        ),
        'filter' => array(
          'allow empty' => TRUE,
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'url' => array(
        'title' => t('URL'),
        'help' => t('External URL from link or image'),
        'field' => array(
          'handler' => 'views_handler_field',
        ),
        'filter' => array(
          'allow empty' => TRUE,
          'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'timestamp' => array(
        'title' => t('Index updated'),
        'help' => t('The date the external links entry was created/updated.'),
        'field' => array(
          'click sortable' => TRUE,
          'handler' => 'views_handler_field_date',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_date',
        ),
      ),
      'is_new' => array(
        'title' => t('New / updated'),
        'help' => t('Whether or not the index entry was added/updated since previous index update.'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
          'click sortable' => TRUE,
          'output formats' => array(
            'new' => array(t('Fresh'), ''),
          ),
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_boolean_operator',
          'label' => t('New / updated'),
          'type' => 'yes-no',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
    ),
  );
}
