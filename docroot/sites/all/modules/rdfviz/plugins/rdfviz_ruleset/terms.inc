<?php

/**
 * @file
 * RDFViz rendering rules for displaying taxonomy terms and their relationships.
 */

$plugin = array(
  'title' => t('Terms'),
  'ruleset_callback' => 'rdfviz_terms_rendering_ruleset',
  'description' => t('Renders Drupal taxonomy terms ans graph nodes and links all participating entities to them. Uses concepts like skos:Concept'),
);
function rdfviz_terms_rendering_ruleset() {
  $rules = array();

  // Drupal core RDF : terms
  // Terms are diamonds
  $rules['format_terms_as_diamonds']['LABEL'] = 'Terms get shown as diamonds';
  $rules['format_terms_as_diamonds']['ON'][] = 'statement_process';
  $rules['format_terms_as_diamonds']['IF'][0]['AND'][0]['predicate_is']['value'] = 'rdf:type';
  $rules['format_terms_as_diamonds']['IF'][0]['AND'][1]['object_is']['value'] = 'skos:Concept';
  $rules['format_terms_as_diamonds']['DO'][0]['addNode']['attributes'] = array(
    'color' => 'palegreen3',
    'shape' => 'diamond',
  );
  // Tags are linked to their entities
  // Current vanilla core doesn't provide rel=tag syntax?
  // SIOC recommneds that the relationship between a sioc:Post and a skos:Concept
  // is sioc:topic. You need to add that yourself with RDFx?
  //
  // Alternatively, when ARC sees a rel="tag", it thinks it's mf:tag
  // from http://poshrdf.org/ns/mf#
  // HOWEVER - that resolves to text literals, and we need(?) it to be a URI
  // To figure this out,
  // Examine RDF-raw output from running on the examples in the wild from
  // http://microformats.org/wiki/rel-tag-examples-in-wild
  $rules['link_terms']['LABEL'] = 'Tags are linked to entities';
  $rules['link_terms']['ON'][] = 'statement_process';
  $rules['link_terms']['IF'][0]['OR'][]['predicate_is']['value'] = 'sioc:topic';
  #$rules['link_terms']['IF'][0]['OR'][]['predicate_is']['value'] = 'mf:tag';
  $rules['link_terms']['DO'][0]['addEdge']['attributes'] = array(
    'color' => 'palegreen2',
    'bgcolor' => 'palegreen1',
    'dir' => 'both',
    'arrowhead' => 'dot',
    'arrowtail' => 'odot',
  );
  // Terms have labels (not titles)
  $rules['label_terms']['LABEL'] = 'Terms have labels';
  $rules['label_terms']['ON'][] = 'statement_process';
  $rules['label_terms']['IF'][0]['predicate_is']['value'] = 'rdfs:label';
  $rules['label_terms']['DO'][0]['addNode']['attributes'] = array(
    // Set the label to the safe value of the term
    'label' => '!o',
  );


  // Custom, using RDFx
  // Normal vocabs link normally, but I've used RDFx to rename my 'Section'
  // vocabulary as xh:section, and I want those links to show up diferently.
  // Compare with link_terms.
  $rules['render_section_vocab_specially']['ON'][] = 'statement_process';
  $rules['render_section_vocab_specially']['IF'][0]['object_is']['value'] = 'xh:section';
  $rules['render_section_vocab_specially']['DO'][0]['addEdge']['attributes'] = array(
    'color' => 'red',
  );

  return $rules;
}

