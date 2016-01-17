<?php

/**
 * @file
 * RDFViz rendering rules for a network and services diagram.
 */

$plugin = array(
  'title' => t('Network'),
  'ruleset_callback' => 'rdfviz_network_rendering_ruleset',
  'description' => t('Render a network diagram made up of services and hosts. Uses concepts like doap:Project and network:provider'),
);
function rdfviz_network_rendering_ruleset() {
  $rules = array();

  // What is a 'website' and how should it be rendered.
  $rules['show_website_type']['ON'][] = 'statement_process';
  $rules['show_website_type']['IF'][0]['AND'][0]['predicate_is']['value'] = 'rdf:type';
  $rules['show_website_type']['IF'][0]['AND'][1]['object_is']['value'] = 'fb:internet.website';
  $rules['show_website_type']['DO'][0]['addNode']['attributes']['shape'] = 'note';

  // What 'provides' a service is a strong link.
  $rules['link_service_to_host']['ON'][] = 'statement_process';
  $rules['link_service_to_host']['IF'][0]['OR'][0]['predicate_is']['value'] = 'network:provider';
  $rules['link_service_to_host']['DO'][0]['addEdge']['attributes'] = array(
    'style' => 'dotted',
    'color' => 'darkslateblue',
  );
  // This is a custom addition to influence layout.
  $rules['link_service_to_host']['DO'][0]['addEdge']['reverse'] = TRUE;

  // References to the database and repository are long-distance two-way links.
  $rules['link_service_to_client']['ON'][] = 'statement_process';
  $rules['link_service_to_client']['IF'][0]['OR'][]['predicate_is']['value'] = 'network:database';
  $rules['link_service_to_client']['IF'][0]['OR'][]['predicate_is']['value'] = 'doap:repository';
  $rules['link_service_to_client']['DO'][0]['addEdge']['attributes'] = array(
    'style' => 'solid',
    'color' => 'grey',
    'arrowhead' => 'vee',
    'weight' => 5,
    #'arrowtail' => 'vee',
    #'dir' => 'both',
  );
  // This is a custom addition to influence layout.
  $rules['link_service_to_client']['DO'][0]['addEdge']['reverse'] = TRUE;

  $rules['link_project_classification']['LABEL'] = 'Network items may be tagged with a project';
  $rules['link_project_classification']['ON'][] = 'statement_process';
  $rules['link_project_classification']['IF'][0]['predicate_is']['value'] = 'dc:isPartOf';
  $rules['link_project_classification']['DO'][0]['addEdge']['attributes'] = array(
    'color' => 'palegreen4',
    #'shape' => 'diamond',
    'bgcolor' => 'palegreen1',
    'dir' => 'back',
  );

  $rules['link_schemes']['LABEL'] = 'A resource has schemes, Use them for color-coding';
  $rules['link_schemes']['ON'][] = 'statement_process';
  $rules['link_schemes']['IF'][0]['AND'][]['predicate_is']['value'] = 'network:scheme';
  $rules['link_schemes']['IF'][0]['AND'][]['object_is']['value'] = 'http';
  $rules['link_schemes']['DO'][0]['addNode']['attributes'] = array(
    'fontcolor' => 'blue',
  );

  $rules['link_schemes_ssh']['LABEL'] = 'A resource has schemes, Use them for color-coding';
  $rules['link_schemes_ssh']['ON'][] = 'statement_process';
  $rules['link_schemes_ssh']['IF'][0]['AND'][]['predicate_is']['value'] = 'network:scheme';
  $rules['link_schemes_ssh']['IF'][0]['AND'][]['object_is']['value'] = 'ssh';
  $rules['link_schemes_ssh']['DO'][0]['addNode']['attributes'] = array(
    'fontcolor' => 'red',
  );


  return $rules;
}

