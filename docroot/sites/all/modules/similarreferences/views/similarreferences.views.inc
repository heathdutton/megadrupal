<?php

/**
 * Implements hook_views_data()
 **/
function similarreferences_views_data_alter(&$data) {
  foreach ($data as $table => $info) {
    if (!empty($info['table']['base'])) {
      $data[$table]['similarreferences'] = array(
        'title' => t('Similarity in @title', array('@title' => $info['table']['base']['title'])),
        'group' => t('Similar By References'),
        'help' => t('Percentage/count of references which the entity has in common with the entity given as argument.'),
        'field' => array(
          'handler' => 'similarreferences_handler_field_similar',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'similarreferences_handler_sort_similar',
        ),
      );
      $data[$table]['similarreferences_argument'] = array(
        'title' => t('Entityreferences in @title', array('@title' => $info['table']['base']['title'])),
        'group' => t('Similar By References'),
        'help' => t('ID of entity. Passes reference ids to Similar By References.'),
        'argument' => array(
          'handler' => 'similarreferences_handler_argument_id',
          'name field' => 'title', // the field to display in the summary.
          'numeric' => TRUE,
        ),
      );
    }
  }
}
