<?php

function project_solr_block_project_solr_compatibility($query, $response) {
  if (module_exists('project_release')) {
    $facet = variable_get('project_solr_project_release_api_tids_alias', 'drupal_core');
    $terms = array();
    $active_term_counts = array();

    $active_terms = project_release_compatibility_list();

    if (isset($response->facet_counts->facet_fields->$facet)) {
      foreach ($response->facet_counts->facet_fields->$facet as $tid => $count) {
        if (!empty($active_terms[$tid])) {
          $active_term_counts[$tid] = $count;
        }
      }
    }

    foreach ($active_terms as $tid => $term_name) {
      if (!empty($active_term_counts[$tid])) {
        $active = $query->hasFilter($facet, $tid);
        $new_query = clone $query;
        $path = $new_query->getPath();
        $new_query->removeFilter($facet, $term->tid);
        $options = array();
        if ($active) {
          $options['query'] = $new_query->getSolrsortUrlQuery();
          $link = theme('apachesolr_unclick_link', $term_name, $path, $options);
        }
        else {
          $new_query->add_filter($facet, $tid);
          $options['query'] = project_solr_append_api_term($new_query->get_url_queryvalues(), $tid);
          $link = theme('project_solr_no_count_facet_link', $term_name, $path, $options, $active, $response->response->numFound);
        }
        $terms[$term_name] = $link;
      }
    }

    if (!empty($terms)) {
      return array(
        'subject' => t('Filter by compatibility'),
        'content' => theme('apachesolr_facet_list', $terms, 200),
      );
    }
  }
}