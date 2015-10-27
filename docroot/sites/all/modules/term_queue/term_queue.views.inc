<?php
/**
 * Implementation of hook_views_data
 * Describe table structure to Views.
 *
 * This hook should be placed in MODULENAME.views.inc and it will be auto-loaded.
 * MODULENAME.views.inc must be in the directory specified by the 'path' key
 * returned by MODULENAME_views_api(), or the same directory as the .module
 * file, if 'path' is unspecified.
 *
 * The full documentation for this hook is in the advanced help.
 * @link http://views-help.doc.logrus.com/help/views/api-tables @endlink
 */
function term_queue_views_data() {
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['term_queue']['table']['group'] = t('Taxonomy queue');
  $data['term_queue_term']['table']['group'] = t('Taxonomy queue');
  
  $data['term_queue']['table']['join'] = array(
    // term_queue links to taxonomy_term_data via the term_queue_term table using qid.
    'taxonomy_term_data' => array(
      'left_table' => 'term_queue_term',
      'left_field' => 'qid',
      'field' => 'qid',
    ),
  );
  
  $data['term_queue_term']['table']['join'] = array(
    // term_queue links to taxonomy_term_data via the term_queue_term table using qid.
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );
  
  // Next, describe each of the individual fields in this table to Views. For
  // each field, you may define what field, sort, argument, and/or filter
  // handlers it supports. This will determine where in the Views interface you
  // may use the field.
  $data['term_queue']['qid'] = array(
    'title' => t('Queue Id'),
    'help' => t('Id for the term queue'),
    );
  $data['term_queue']['machine_name'] = array(
    'title' => t('Machine Name'),
    
    'field' => array(
      'help' => t('Machine-Name of the term queue a term is a member of.'),
      'handler' => 'views_handler_field',
    ),
    
    'filter' => array(
      'help' => t('Filter the results of "Taxonomy: Term" to a particular term queue'),
      //'handler' => 'views_handler_filter_vocabulary_machine_name',
      'handler' => 'views_handler_filter_term_queue_machine_name',
    ),

    );
  $data['term_queue']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the term queue'),
    );
  $data['term_queue']['description'] = array(
    'title' => t('Description'),
    'help' => t('A description of the term queue'),
    );
  $data['term_queue_term']['weight'] = array(
    'title' => t('Weight'),
    
    'field' => array(
      'help' => t('The weight of a term in a term queue'),
      'handler' => 'views_handler_field',
    ),
    
    'sort' => array(
      'help' => t('Sort by the weight of a term in a term queue'),
      //'handler' => 'views_handler_filter_vocabulary_machine_name',
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}