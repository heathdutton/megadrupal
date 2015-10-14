<?php

/**
 * Implements hook_views_data_alter()
 **/
function views_related_content_views_data_alter(&$data) {
  $data['views_related_content']['views_related_content']['moved to'] = array('node', 'related_content_type');
  $data['node']['related_content_type'] = array(
    'title' => t('Content Type'),
    'group' => t('Related Content'),
    'help' => t('You are related by Content type'),
    'filter' => array(
      'handler' => 'related_node_handler_filter_content_type',
    ),
  );

  $data['node']['related_author'] = array(
    'title' => t('Author'),
    'group' => t('Related Content'),
    'help' => t('You are related by Content Author'),
    'filter' => array(
      'handler' => 'views_related_content_handler_filter_author',
    ),
  );

  $data['node']['related_term'] = array(
    'title' => t('Term'),
    'group' => t('Related Content'),
    'help' => t('You are related by Terms (Vocabylary)'),
    'filter' => array(
      'handler' => 'views_related_content_handler_filter_term',
    ),
  );

  $data['node']['related_field_reference'] = array(
    'title' => t('Field Reference'),
    'group' => t('Related Content'),
    'help' => t('You are related by Entity (Field Reference)'),
    'filter' => array(
      'handler' => 'views_related_content_handler_filter_field_reference',
    ),
  );
}

/**
 * Implements hook_views_query_alter()
 **/
function views_related_content_views_query_alter(&$view, &$query) {
  $curNode = menu_get_object();
  if($curNode && $curNode->nid) {
    $count_where = 1;

    if (isset($view->display_handler->handlers['filter']['related_author']->definition) && isset($curNode)) {
      $query->add_where($count_where, 'node.uid', $curNode->uid, '=');
      if (isset($curNode->nid)) {
        $query->add_where(1, 'node.nid', $curNode->nid, '<>');
      }
    }
    
    if (isset($view->display_handler->handlers['filter']['related_content_type']->definition) && isset($curNode)) {
      $query->add_where($count_where, 'node.type', $curNode->type, '=');
      if (isset($curNode->nid)) {
        $query->add_where(1, 'node.nid', $curNode->nid, '<>');
      }
    }

    if (isset($view->display_handler->handlers['filter']['related_term']->value) && isset($curNode)) {
      $listTaxonomySelected = $view->display_handler->handlers['filter']['related_term']->value;

      $join = new views_join();
      $join->table = 'taxonomy_index';
      $join->left_table = 'node';
      $join->field = 'nid';
      $join->left_field = 'nid';
      $join->type = 'LEFT';
      $query->table_queue['taxonomy_index'] = array (
        'alias' => 'taxonomy_index',
        'table' => 'taxonomy_index',
        'relationship' => 'node',
        'join' => $join,
      );
      
      $join = new views_join();
      $join->table = 'taxonomy_term_data';
      $join->left_table = 'taxonomy_index';
      $join->field = 'tid';
      $join->left_field = 'tid';
      $join->type = 'LEFT';
      $query->table_queue['taxonomy_term_data'] = array (
        'alias' => 'taxonomy_term_data',
        'table' => 'taxonomy_term_data',
        'relationship' => 'taxonomy_index',
        'join' => $join,
      );

      $curNodeTaxonomy = array();
      foreach ($curNode as $key => $value) {
        if (isset($value['und'][0]['taxonomy_term'])) {
          $curNodeTaxonomy[$key] = array();
          foreach ($value['und'] as $key_item => $value_item) {
            $curNodeTaxonomy[$key][] = $value_item['taxonomy_term']->name;
          }
        }
      }
      
      $count_where++;
      foreach ($listTaxonomySelected as $key => $value) {
        if (array_key_exists('field_' . $key, $curNodeTaxonomy)) {
          # find taxonomy
          foreach ($curNodeTaxonomy['field_' . $key] as $key_cur_taxonomy => $value_term) {
            # search term in current taxonomy
            $query->add_where($count_where, 'taxonomy_term_data.name', $value_term, '=');
          }
          # Set conditon type where
          $query->where[$count_where]['type'] = 'OR';
          $count_where++;
        }
      }

      $query->add_field('node', 'nid', 'node_nid', array('function' => 'groupby'));
      $query->add_groupby('node.nid');      
      if (isset($curNode->nid)) {
        $query->add_where(1, 'node.nid', $curNode->nid, '<>');
      }
    }

    if (isset($view->display_handler->handlers['filter']['related_field_reference']->value) && isset($curNode)) {
      $listFieldReference = $view->display_handler->handlers['filter']['related_field_reference']->value;

      # List node entity reference of field
      $list_ref_cur_node = array();
      foreach ($listFieldReference as $key => $value) {
        if (isset($curNode->{$value}['und'])) {
          foreach ($curNode->{$value}['und'] as $key_node => $value_node) {
            $list_ref_cur_node[$value][] = $value_node['target_id'];
          }
        }
      }

      foreach ($listFieldReference as $key => $value) {
        $count_where++;
        $query->add_where($count_where, 'field_data_' . $key . '.' . $key . '_target_id', $list_ref_cur_node[$value], 'IN');
        $query->where[$count_where]['type'] = 'OR';
      }

     
    }
  }
}

