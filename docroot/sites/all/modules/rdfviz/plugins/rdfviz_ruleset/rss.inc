<?php

/**
 * @file
 * RDFViz rendering rules for RSS feeds.
 */

$plugin = array(
  'title' => t('RSS'),
  'ruleset_callback' => 'rdfviz_rss_rendering_ruleset',
  'description' => t('Renders data from RSS feeds with items as graph nodes. Uses concepts like rss:item rss:label'),
);
function rdfviz_rss_rendering_ruleset() {
  $rules = array();

  // We can also figure out semantics from rss feeds
  $rules['rss_item_is_a_thing'] = array(
    'LABEL' => 'RSS items are documents',
    'ON' => array('statement_process'),
    'IF' => array(
      array(
        'predicate_is' => array(
          'value' => 'rdf:type',
        ),
      ),
      array(
        'object_is' => array(
          'value' => 'rss:item',
        ),
      ),
    ),
    'DO' => array(
      array(
        // Add a rendering for this
        'addNode' => array(
          // With these parameters
          'attributes' => array(
            'shape' => 'note',
            'style' => 'filled',
            'fillcolor' => 'powderblue',
          ),
        ),
      ),
    ),
  );

  $rules['rss_titles_are_labels'] = array(
    'LABEL' => 'Anything with a rss:title should use that as a label',
    'ON' => array('statement_process'),
    'IF' => array(
      array(
        'predicate_is' => array(
          'value' => 'rss:title',
        ),
      ),
    ),
    'DO' => array(
      array(
        'addNode' => array(
          'attributes' => array(
            'label' => '!o',
          ),
        ),
      ),
    ),
  );

  return $rules;
}

