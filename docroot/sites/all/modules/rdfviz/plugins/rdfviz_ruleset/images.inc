<?php

/**
 * @file
 * RDFViz rendering rules for rendering elements with images.
 */

$plugin = array(
  'title' => t('Images'),
  'ruleset_callback' => 'rdfviz_images_rendering_ruleset',
  'description' => t('If image resources are found, render them on the diagram as icons. Uses concepts like foaf:Image'),
);
function rdfviz_images_rendering_ruleset() {
  $rules = array();

  // Semi-core : basic install 'image' items attached to nodes can be rendered.
  // However, pure-core does not add a relationship between an image and
  // the node that contains it. dull.
  /*
  $rules['render_images']['LABEL'] = 'Try to render image items';
  $rules['render_images']['ON'][] = 'statement_process';
  $rules['render_images']['IF'][0]['type_is']['value'] = 'foaf:Image';
  $rules['render_images']['DO'][0]['addNode']['attributes'] = array(
    'shape' => 'block',
    // Set this value to the full URI of the subject.
    'image' => '!s_local',
    'label' => '',
    'border' => 'none',
  );
  */

  $rules['attach_images']['LABEL'] = 'Try to render image items that are attached to nodes';
  $rules['attach_images']['ON'][] = 'statement_process';
  // Incorrect usage
  $rules['attach_images']['IF'][0]['OR'][]['predicate_is']['value'] = 'foaf:Image';
  $rules['attach_images']['IF'][0]['OR'][]['predicate_is']['value'] = 'foaf:img';
  $rules['attach_images']['IF'][0]['OR'][]['predicate_is']['value'] = 'foaf:depicts';
  #$rules['attach_images']['IF'][0]['OR'][]['predicate_is']['value'] = 'fb:common.topic.image';
  #$rules['attach_images']['IF'][0]['OR'][]['predicate_is']['value'] = 'schema:image';
  // Either put the image out there on its own and point at it:
  /*
  $rules['attach_images']['DO'][]['addEdge']['attributes'] = array(
    'dir' => 'none',
  );
  */

  // Or embed the image in the subject itself - this will need care, but is
  // the nicest icon behavior to aim for.
  $rules['attach_images']['DO'][]['addNode']['attributes'] = array(
    'shape' => 'box',
    // no border
    'penwidth' => '0',
    // no border
    'pencolor' => 'none',
    // REALLY no border. svg doesn't listen to penwidth
    'color' => 'none',
    // no bg color
    'fillcolor' => 'none',
    'image' => '!o_local',
    'imagescale' => 'true',
    // Align label bottom when there is an image
    'labelloc' => 'b',
  );

  return $rules;
}

