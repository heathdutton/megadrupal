<?php

/**
 * @file
 * RDFViz rendering rules for User accounts.
 */

$plugin = array(
  'title' => t('Users'),
  'ruleset_callback' => 'rdfviz_users_rendering_ruleset',
  'description' => t('Renders user account information. Uses concepts like foaf:Person, sioc:UserAccount and extracts their name if possible.'),
);
function rdfviz_users_rendering_ruleset() {
  $rules = array();

  // Show user accounts on the graph, and link users to their content

  // Users can look funny
  $rules['render_users_differently'] = array(
    'LABEL' => 'Give user account items distinctive appearance',
    'ON' => array('statement_process'),
    'IF' => array(
      array(
        'predicate_is' => array(
          'value' => 'rdf:type',
        ),
      ),
      array(
        'OR' => array(
          array(
            'object_is' => array(
              'value' => 'sioc:UserAccount',
            ),
          ),
          array(
            'object_is' => array(
              'value' => 'foaf:Person',
            ),
          ),
        ),
      ),
    ),
    'DO' => array(
      array(
        // Add a rendering for this
        'addNode' => array(
          // With these parameters
          'attributes' => array(
            'shape' => 'egg',
            'style' => 'filled',
            'fillcolor' => 'mistyrose',
            'color' => 'mistyrose3',
          ),
        ),
      ),
    ),
  );

  $rules['link_author_to_page'] = array(
    'LABEL' => 'Link author (dc:creator) to a page',
    'ON' => array('statement_process'),
    'IF' => array(
      array(
        'OR' => array(
          array(
            'predicate_is' => array(
              'value' => 'sioc:has_creator',
            ),
          ),
          array(
            'predicate_is' => array(
              'value' => 'dc:creator',
            ),
          ),
        ),
      ),
    ),
    'DO' => array(
      array(
        // Add an edge to the graph
        'addEdge' => array(
          // With these parameters
          'attributes' => array(
            'dir' => 'back',
            'style' => 'dotted',
            'color' => 'powderblue',
          ),
        ),
      ),
    ),
  );


  $rules['label_people'] = array(
    'LABEL' => 'Anything with a foaf:name should use that as a label',
    'ON' => array('statement_process'),
    'IF' => array(
      array(
        // This example demonstrates why we always need the extra array
        // inside condition lists. We cannot key on 'predicate_is' as
        // we are checking it twice.
        'OR' => array(
          array(
            'predicate_is' => array(
              'value' => 'foaf:name',
            ),
          ),
          array(
            'predicate_is' => array(
              'value' => 'foaf:firstName',
            ),
          ),
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
  # The above is equivaalent to:
  # $rules['label_people']['ON'][] = 'statement_process';
  # $rules['label_people']['IF']['OR'][]['predicate_is']['value'] = 'foaf:name';
  # $rules['label_people']['IF']['OR'][]['predicate_is']['value'] = 'foaf:firstName';
  # $rules['label_people']['DO'][]['addNode']['attributes']['label'] = '%p';

  // EXAMPLE
  // You could illustrate a social graph like this, but first you must set up user references and label them as 'foaf:knows
  $rules['who_knows_who']['ON'][] = 'statement_process';
  $rules['who_knows_who']['IF'][0]['AND'][0]['predicate_is']['value'] = 'foaf:knows';
  $rules['who_knows_who']['DO'][0]['addEdge']['attributes'] = array(
    'color' => 'grey',
    'label' => 'knows',
    'dir' => 'forward',
    'shape' => 'odiamond',
    'weight' => 0.2,
  );

  return $rules;
}

