<?php

/**
 * @file
 * RDFViz rendering rules for DC subject tagging.
 */

$plugin = array(
  'title' => t('Subjects'),
  'ruleset_callback' => 'rdfviz_subjects_rendering_ruleset',
  'description' => t('Renders Dublin Core subjects (Tags) as references. Uses concepts like dc:subject, either as plaintext or URIs'),
);
function rdfviz_subjects_rendering_ruleset() {
  $rules = array();


  $rules['dc_subjects_are_tags']['LABEL'] = 'Tags are connected to the thing they tag.';
  $rules['dc_subjects_are_tags']['ON'][] = 'statement_process';
  $rules['dc_subjects_are_tags']['IF'][0]['AND'][0]['predicate_is']['value'] = 'dc:subject';
  $rules['dc_subjects_are_tags']['DO'][]['addEdge']['attributes'] = array(
    'color' => 'mediumseagreen',
    'dir' => 'both',
    'arrowhead' => 'dot',
    'arrowtail' => 'odot',
  );

  $rules['dc_subject_uris_are_things']['LABEL'] = 'Subjects are things, and may be URIs';
  $rules['dc_subject_uris_are_things']['ON'][] = 'statement_process';
  $rules['dc_subject_uris_are_things']['IF'][0]['AND'][]['predicate_is']['value'] = 'dc:subject';
  // The object type is a URI.
  $rules['dc_subject_uris_are_things']['IF'][0]['AND'][]['attribute_matches']['o_type'] = 'uri';
  $rules['dc_subject_uris_are_things']['DO'][]['addNode']['attributes'] = array(
    // Add an element representing the target URI it points at.
    'shape' => 'pentagon',
    'id' => '!o_curie',
    #'label' => '!o_curie',
    'URL' => '!o',
    'color' => 'mediumseagreen',
    'fontcolor' => 'mediumseagreen',
    'background' => 'palegreen2',
  );

  $rules['dc_subject_literals_are_things']['LABEL'] = 'Subjects are things, even though they are sometimes just strings';
  $rules['dc_subject_literals_are_things']['ON'][] = 'statement_process';
  $rules['dc_subject_literals_are_things']['IF'][0]['AND'][]['predicate_is']['value'] = 'dc:subject';
  // The object type is a literal string.
  $rules['dc_subject_literals_are_things']['IF'][0]['AND'][]['attribute_matches']['o_type'] = 'literal';
  $rules['dc_subject_literals_are_things']['DO'][]['addNode']['attributes'] = array(
    'shape' => 'pentagon',
    // This is a string literal, so has no ID.
    // To instantiate it, set the id to = the string.
    'id' => '!o',
    'URL' => '',
    'color' => 'mediumseagreen',
    'fontcolor' => 'mediumseagreen',
    'background' => 'palegreen2',
  );
  return $rules;
}

