<?php

/**
 * @file
 * RDFViz rendering rules for rendering 'flowchart' relationships.
 *
 * I seached a lot, but could find no existing vocabulary or normative
 * reference for flowcharting terminology.
 * Instead, the nicest reference was
 * http://www.breezetree.com/article-excel-flowchart-shapes.htm
 * so we will use the definitions from there.
 *
 * "Process", "Decision", "Terminator" etc
 *
 * The same terms are used in
 * http://creately.com/diagram-type/objects/flowchart
 *
 * These already map well to the graphviz shape set, though it's odd I can't
 * find *good* examples of the two technologies being used together.
 * Illustrations I've found are mostly ugly with curved lines and things.
 *
 * In this fake schema, Objects (data entities) are capitalized,
 * Relationships (arrows of various intent) are the 'rel' properties and
 * are lower-case. Like most RDF schemas.
 *
 * A 'Decision' may have an 'option' attached,
 * and a 'Process' may have a 'next' relation with another 'Process'
 *
 * TODO - see how we can get the negative (failure) branches of a Decision
 * to go horizontally.
 */

$plugin = array(
  'title' => t('Flowchart'),
  'ruleset_callback' => 'rdfviz_flowchart_rendering_ruleset',
  'description' => t('Experimental flowchart vocabulary (looking for prior art)'),
);
function rdfviz_flowchart_rendering_ruleset() {
  $rules = array();
  $ns = 'fc';
  $default_atts = array(
    'color' => 'grey',
    'fontcolor' => 'slategrey',
  );

  // Flowchart convention for an action/process is box.
  $rules['flowchart_process_type']['ON'][] = 'statement_process';
  $rules['flowchart_process_type']['IF'][0]['AND'][0]['predicate_is']['value'] = 'rdf:type';
  $rules['flowchart_process_type']['IF'][0]['AND'][1]['OR'][0]['object_is']['value'] = $ns . ':Process';
  $rules['flowchart_process_type']['DO'][0]['addNode']['attributes'] = array(
    'shape' => 'box',
  ) + $default_atts;

  // Flowchart convention for a decision is diamond.
  $rules['flowchart_decision_type']['ON'][] = 'statement_process';
  $rules['flowchart_decision_type']['IF'][0]['AND'][0]['predicate_is']['value'] = 'rdf:type';
  $rules['flowchart_decision_type']['IF'][0]['AND'][1]['OR'][0]['object_is']['value'] = $ns . ':Decision';
  $rules['flowchart_decision_type']['DO'][0]['addNode']['attributes'] = array(
    'shape' => 'diamond',
  ) + $default_atts;

  // We may insert an object for each option that labels a choice.
  $rules['flowchart_option_type']['ON'][] = 'statement_process';
  $rules['flowchart_option_type']['IF'][0]['AND'][0]['predicate_is']['value'] = 'rdf:type';
  $rules['flowchart_option_type']['IF'][0]['AND'][1]['OR'][0]['object_is']['value'] = $ns . ':Option';
  $rules['flowchart_option_type']['DO'][0]['addNode']['attributes'] = array(
    'shape' => 'circle',
    'color' => 'lightgrey',
  ) + $default_atts;

  $rules['flowchart_choice_links']['LABEL'] = 'Branching decision paths using :choice';
  $rules['flowchart_choice_links']['ON'][] = 'statement_process';
  $rules['flowchart_choice_links']['IF'][0]['AND'][]['predicate_is']['value'] = $ns . ':choice';
  $rules['flowchart_choice_links']['IF'][0]['AND'][]['object_type_is']['value'] = 'uri';
  $rules['flowchart_choice_links']['DO'][]['addEdge']['attributes'] = array(
    'style' => 'dashed',
    'dir' => 'forward',
    'weight' => 0.1,
  ) + $default_atts;

  // Most flowchart arrows are links to the next step.
  // Pretty much xh:next.
  $rules['flowchart_next_links']['LABEL'] = 'Flowchart links';
  $rules['flowchart_next_links']['ON'][] = 'statement_process';
  $rules['flowchart_next_links']['IF'][0]['predicate_is']['value'] = $ns . ':next';
  $rules['flowchart_next_links']['DO'][0]['addEdge']['attributes'] = array(
    'style' => 'solid',
  ) + $default_atts;



  return $rules;
}

