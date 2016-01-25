<?php

/**
 * @file
 * Extends the PowerTagger module such that works with Search API, Solr Search
 * and Facet API.
 *
 * @todo:
 * - Preflabels can change, indexed documents need to be reindexed when a tag
 *   changes.
 */

// Extend the Facet API filter if it exists.
if (module_exists('facetapi') && class_exists('FacetapiFilter')) {

  // Returns the name (label) of the facet item instead of the URI (stored in
  // the Solr index)
  class FacetapiFilterUsePreflabelInsteadOfUri extends FacetapiFilter {

    /**
     * Filters facet items.
     */
    public function execute(array $build) {
      $uris = array();
      foreach ($build as $facet) {
        $uris[] = $facet['#indexed_value'];
      }

      if (!empty($uris)) {
        $prelabels_by_uri = db_select('field_data_field_uri', 'u');
        $prelabels_by_uri->join('taxonomy_term_data', 't', 't.tid = u.entity_id');
        $prelabels_by_uri = $prelabels_by_uri->fields('u', array('field_uri_value'))
          ->fields('t', array('name'))
          ->condition('u.entity_type', 'taxonomy_term')
          ->condition('u.field_uri_value', $uris)
          ->execute()
          ->fetchAllKeyed();

        if (!empty($prelabels_by_uri)) {
          foreach($build as &$facet) {
            if (isset($prelabels_by_uri[$facet['#indexed_value']])) {
              $facet['#markup'] = $prelabels_by_uri[$facet['#indexed_value']];
            }
          }
        }
      }
      return $build;
    }
  }
}

/**
 * Implements hook_entity_property_info_alter().
 */
function powertagging_entity_property_info_alter(&$info) {
  $info['node']['properties']['powertagging_tags_id'] = array(
    'type' => 'list<integer>',
    'label' => t('PowerTagging Tags IDs'),
    'sanitized' => TRUE,
    'getter callback' => 'powertagging_tags_id_search_api_property_getter_callback',
  );
  $info['node']['properties']['powertagging_tags_uri'] = array(
    'type' => 'list<uri>',
    'label' => t('PowerTagging Tags URIs'),
    'sanitized' => TRUE,
    'getter callback' => 'powertagging_tags_uri_search_api_property_getter_callback',
  );
  $info['node']['properties']['powertagging_tags_preflabel'] = array(
    'type' => 'list<text>',
    'label' => t('PowerTagging Tags prefLabels'),
    'sanitized' => TRUE,
    'getter callback' => 'powertagging_tags_preflabel_search_api_property_getter_callback',
  );
  $info['node']['properties']['powertagging_tags_altlabel'] = array(
    'type' => 'list<text>',
    'label' => t('PowerTagging Tags altLabels'),
    'sanitized' => TRUE,
    'getter callback' => 'powertagging_tags_altlabel_search_api_property_getter_callback',
  );
}

/**
 * Implements hook_facetapi_filters().
 */
function powertagging_facetapi_filters() {
  return array(
    'use_preflabel_instead_of_uri' => array(
      'handler' => array(
        'label' => t('Use prefLabels instead of URIs in PowerTagging field facets'),
        'class' => 'FacetapiFilterUsePreflabelInsteadOfUri',
      ),
    ),
  );
}

/**
 * Callback function for the entity property.
 *
 * @param array $item
 *  A facet item
 *
 * @return array
 *  A list of Ids
 */
function powertagging_tags_id_search_api_property_getter_callback($item) {
  $values = array();

  if (($field_name = powertagging_get_field_name($item->type)) == '') {
    return $values;
  }

  $field = $item->{$field_name};
  $tids = array();
  if (!empty($field) && !empty($field[LANGUAGE_NONE])) {
    foreach ($field[LANGUAGE_NONE] as $tid) {
      $values[] = $tid['tid'];
    }
  }

  return $values;
}

/**
 * Callback function for the entity property.
 *
 * @param array $item
 *  A facet item
 *
 * @return array
 *  A list of URIs
 */
function powertagging_tags_uri_search_api_property_getter_callback($item) {
  $tids = powertagging_tags_id_search_api_property_getter_callback($item);
  $values = array();

  if (!empty($tids)) {
    $tag_terms = taxonomy_term_load_multiple($tids);
    foreach ($tag_terms as $tag_term) {
      $values[] = $tag_term->field_uri[LANGUAGE_NONE][0]['value'];
    }
  }

  return $values;
}

/**
 * Callback function for the entity property.
 *
 * @param array $item
 *  A facet item
 *
 * @return array
 *  A list of term names (prefLabels)
 */
function powertagging_tags_preflabel_search_api_property_getter_callback($item) {
  $tids = powertagging_tags_id_search_api_property_getter_callback($item);
  $values = array();

  if (!empty($tids)) {
    $tag_terms = taxonomy_term_load_multiple($tids);
    foreach ($tag_terms as $tag_term) {
      $values[] = $tag_term->name;
    }
  }

  return $values;
}

/**
 * Callback function for the entity property.
 *
 * @param array $item
 *  A facet item
 *
 * @return array
 *  A list of altLabels
 */
function powertagging_tags_altlabel_search_api_property_getter_callback($item) {
  $tids = powertagging_tags_id_search_api_property_getter_callback($item);
  $values = array();

  if (!empty($tids)) {
    $tag_terms = taxonomy_term_load_multiple($tids);
    foreach ($tag_terms as $tag_term) {
      if (!empty($tag_term->field_alt_labels)) {
        $alt_labels = explode(',', $tag_term->field_alt_labels[LANGUAGE_NONE][0]['value']);
        foreach ($alt_labels as $alt_label) {
          $values[] = trim($alt_label);
        }
      }
    }
  }

  return $values;
}

/**
 * Returns all entity ids that are linked with the $term.
 *
 * @param object $term
 *   The term object.
 *
 * @return array
 *   A list of entity ids.
 */
function powertagging_get_entity_ids($term) {
  // Check if Search API is installed
  $entity_ids = array();
  if (module_exists('search_api') && function_exists('search_api_track_item_change')) {
    // Get the PowerTagging field names.
    $field_names = powertagging_get_field_name();
    foreach ($field_names as $field_name) {
      $table = 'field_data_' . $field_name;
      $column = $field_name . '_tid';

      // Get all entities linked with the given term.
      $result = db_select($table, 't')
        ->fields('t', array('entity_type', 'entity_id'))
        ->condition($column, $term->tid)
        ->execute()
        ->fetchAll();

      if (!empty($result)) {
        foreach ($result as $row) {
          $entity_ids[$row->entity_type][] = $row->entity_id;
        }
      }
    }
  }

  return $entity_ids;
}

/**
 * Updates the solr index for the given entities.
 *
 * @param array $entity_ids
 *   A list of entity ids.
 */
function powertagging_update_solr_index($entity_ids) {
  // Check if Search API is installed
  if (module_exists('search_api') && function_exists('search_api_track_item_change')) {
    // If entities found mark it for reindexing
    if (!empty($entity_ids)) {
      foreach ($entity_ids as $entity_type => $ids) {
        // Reindex the node.
        search_api_track_item_change($entity_type, $ids); 
      }
    }
  }
}




/**
 * Functions for the Apache Solr Search + Facet API (not working).
 */

function powertagging_apachesolr_index_document_build_node(ApacheSolrDocument $document, $entity, $env_id) {
  if (isset($entity->field_power_tagging_tags) && isset($entity->field_power_tagging_tags[LANGUAGE_NONE][0]['tid'])) {
    foreach ($entity->field_power_tagging_tags[LANGUAGE_NONE] as $tid) {
      $document->setMultiValue('sm_power_tagging_tags', $tid['tid']);
    }
  }
}

function powertagging_facetapi_facet_info(array $searcher_info) {
  $facets = array();

  if (isset($searcher_info['types']['node'])) {
    $facets['sm_power_tagging_tags'] = array(
      'name' => 'power_tagging_tags',
      'lable' => t('PowerTagging tags'),
      'field api name' => 'sm_power_tagging_tags',
      'description' => t('Custom field for the powertagging'),
      'map callback' => 'powertagging_map_sm_power_tagging_tags',
      'query types' => array('term', 'date'),
      'dependency plugins' => array('role'),
      'default widget' => 'links',
    );
  }

  return $facets;
}

function powertagging_map_sm_power_tagging_tags(array $values) {
  $map = array();
  foreach($values as $val) {
    $map[$val] = $val;
  }
  return $map;
}
