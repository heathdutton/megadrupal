<?php

/**
 * @file
 * FacetAPI helper functions.
 */

/**
 * Implements hook_apachesolr_facets().
 *
 * Returns an array keyed by block delta.
 */
function apachesolr_biblio_apachesolr_facets() {
  // Return empty array, facetapi integration needed here.

  // Static caching because every biblio facet block can
  // potentially call this function.
  static $facets = array();
  if (empty($facets)) {
    $fields = _apachesolr_biblio_get_fields();

    foreach ($fields as $fid => $biblio) {
      if ($biblio['facetable']) {
        $key = apachesolr_index_key($biblio);
        $title = db_query("SELECT title FROM {biblio_field_type_data} WHERE ftdid = :ftdid", array(':ftdid' => $fid))->fetchField();
        $facets[$key] = array(
          'description' => t('Biblio: Filter by !title (!key)', array(
            '!title' => $title,
            '!key' => $key,
          )),
          'label' => t('!title (!key)', array(
            '!title' => $title,
            '!key' => $key,
          )),
          'field' => $key,
          'content types' => array('biblio'),
        );

        // Date fields.
        if ($biblio['index_type'] == 'date') {
          $facets[$key]['query types'] = array('term', 'date', 'date_range');
        }
      }
    }

    if (variable_get('apachesolr_biblio_index_authors') == 1) {
      // Contributors are a special case.
      $facets['sm_biblio_contributors'] = array(
        'description' => t('Biblio: Filter by Citation Contributor / Author (sm_biblio_contributors)'),
        'label' => 'Citation Contributor / Author (sm_biblio_contributors)',
        'field' => 'sm_biblio_contributors',
        'content types' => array('biblio'),
      );
    }
    // Title is a special case.
    $facets['sm_biblio_title'] = array(
      'description' => t('Biblio: Title (sm_biblio_title)'),
      'label' => 'Title (sm_biblio_title)',
      'field' => 'sm_biblio_title',
      'content types' => array('biblio'),
    );
  }

  return $facets;
}
