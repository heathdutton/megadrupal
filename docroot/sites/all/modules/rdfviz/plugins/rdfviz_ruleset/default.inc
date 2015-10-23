<?php

/**
 * @file
 * RDFViz rendering rules for basic elements.
 *
 * This file has the parameters spaced out and annotated for illustration.
 * The same rules could be expressed much more compactly.
 */

$plugin = array(
  'title' => t('Default'),
  'ruleset_callback' => 'rdfviz_default_rendering_ruleset',
  'description' => t('Apply labels to things, all things are graph nodes. Uses concepts like dc:title, foaf:Document'),
);

/**
 * Default ruleset plugin.
 *
 * @return array
 *   list of rendering rules.
 */
function rdfviz_default_rendering_ruleset() {
  $rules = array();

  $rules['label_anything'] = array(
    'LABEL' => 'Anything with a dc:title should use that as a label',
    // When this happens.
    // (Alternative is when processing a subject array, subject_process)
    // "ON" Must be an array (though it only makes sense to have one) for
    // close compatability with rules module.
    'ON' => array('statement_process'),
    'IF' => array(
      array(
        // This is a simple case, just one condition.
        // Always nested in a numbered array though.
        'predicate_is' => array(
          'value' => 'dc:title',
        ),
      ),
      // If you have multiple conditions at this level,
      // they are matched with AND unless otherwise set.
    ),
    // Actions to perform on the graphviz object being built.
    'DO' => array(
      // Actions are always in a list. Even it there's only one.
      array(
        // Add a node to the graph.
        'addNode' => array(
          // With these parameters.
          'attributes' => array(
            // Some substitutions are allowed.
            // Based on the ARC statement being processed,
            // %s, %o, %p are the subject, predicate, object values
            // (raw uris usually)
            // For dc:title, we know the o_type will be 'literal'
            // so just use the %o value.
            'label' => '!o',
          ),
        ),
      ),
    ),
  );

  // A slightly deeper example.
  $rules['render_nodes_as_blocks'] = array(
    'LABEL' => 'Render nodes (all foaf:Document) as blocks',
    // When processing a triple statement.
    'ON' => array('statement_process'),
    'IF' => array(
      array(
        // Nesting conditions in an AND makes them both required.
        'AND' => array(
          // List the conditions to match here.
          // Note that the conditions are two deep.
          // $rule['IF']['AND][0]['predicate_is]['value'] = 'rdf:type'
          array(
            'predicate_is' => array(
              'value' => 'rdf:type',
            ),
          ),
          array(
            'object_is' => array(
              'value' => 'foaf:Document',
            ),
          ),
        ),
      ),
    ),
    'DO' => array(
      array(
        // Add a node to the graph.
        'addNode' => array(
          // With these parameters.
          'attributes' => array(
            'shape' => 'note',
            'style' => 'filled',
            'fillcolor' => 'powderblue',
          ),
          // Optional.
          'group' => NULL,
        ),
      ),
    ),
  );

  return $rules;
}

