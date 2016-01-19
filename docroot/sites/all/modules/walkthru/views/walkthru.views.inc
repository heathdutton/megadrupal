<?php

function walkthru_views_data() {
  $data = array(
    'walkthru' => array(
      'table' => array(
        'group' => t('Walkthru'),
        'base' => array(
          'field' => 'name',
          'title' => t('Walkthru'),
          'help' => t('A grouping of steps'),
        )
      ),
      'name' => array(
        'title' => t('Walkthru Machine Name'),
        'help' => t('Machine name of the walkthru'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'title' => array(
        'title' => t('Walkthru Title'),
        'help' => t('Title of the walkthru'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
    ),
    'walkthru_steps' => array(
      'table' => array(
        'group' => t('Walkthru Steps'),
        'join' => array(
          'walkthru' => array(
            'left_field' => 'name',
            'field' => 'walkthru_name',
          ),
        ),
      ),
      'name' => array(
        'title' => t('Walkthru Step Machine Name'),
        'help' => t('Machine name of the walkthru step'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'title' => array(
        'title' => t('Walkthru Step Title'),
        'help' => t('Title of the walkthru step'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'weight' => array(
        'title' => t('Walkthru Step Weight'),
        'help' => t('Weight of the walkthru step.'),
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
      'data' => array(
        'title' => t('Walkthru Step Data'),
        'help' => t('Configuration data from the walkthru step'),
        'field' => array(
          'handler' => 'views_handler_field',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
      ),
    ),
  );

  return $data;
}
