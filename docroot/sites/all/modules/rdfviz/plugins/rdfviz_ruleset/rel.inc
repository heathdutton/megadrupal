<?php

/**
 * @file
 * RDFViz rendering rules for rendering 'rel' relationships.
 */

$plugin = array(
  'title' => t('Rel'),
  'ruleset_callback' => 'rdfviz_rel_rendering_ruleset',
  'description' => t('Add "rel" relationships (link rel="up") or entityreference links if found in documents. Uses xh:up, xh:subsection, dc:references, metadata that may be found embedded in well-structured documents.'),
);
function rdfviz_rel_rendering_ruleset() {
  $rules = array();

  // Non-core extensions to RDF. Custom relationships from RDFx
  // rel=up Tags create links between items.
  $rules['rel_up_links']['LABEL'] = 'rel=up Tags create links between items';
  $rules['rel_up_links']['ON'][] = 'statement_process';
  $rules['rel_up_links']['IF'][]['predicate_is']['value'] = 'xh:up';
  $rules['rel_up_links']['DO'][]['addEdge']['attributes'] = array(
    'color' => 'blue',
    #'dir' => 'back',
  );
  // Don't use dir=back. Better to use custom flag 'reverse'
  // As that changes hierarchy in the document order.
  $rules['rel_up_links']['DO'][0]['addEdge']['reverse'] = TRUE;

  // rel=subsection Tags create links between items
  // An item with menu children (xh:subsection) links to its children.
  $rules['rel_subsection_links']['LABEL'] = 'rel=subsection Tags create links to child items';
  $rules['rel_subsection_links']['ON'][] = 'statement_process';
  $rules['rel_subsection_links']['IF'][]['predicate_is']['value'] = 'xh:subsection';
  $rules['rel_subsection_links']['DO'][]['addEdge']['attributes'] = array(
    'color' => 'purple',
    'style' => 'solid',
  );

  /*
  // rel=next Tags create links between siblings
  // Links between sublings, using rel=next
  // This was handy for illustrating,
  // but doesn't help structural layout much.
  $rules['rel_next_links']['LABEL'] = 'rel=next Tags create links between siblings';
  $rules['rel_next_links']['ON'][] = 'statement_process';
  $rules['rel_next_links']['IF'][]['predicate_is']['value'] = 'xh:next';
  $rules['rel_next_links']['DO'][]['addEdge']['attributes'] = array(
    'color' => 'grey',
    'style' => 'bold',
    'dir' => 'forward',
    'weight' => 0.1,
  );
  $rules['rel_prev_links']['LABEL'] = 'rel=prev Tags create links between siblings';
  $rules['rel_prev_links']['ON'][] = 'statement_process';
  $rules['rel_prev_links']['IF'][]['predicate_is']['value'] = 'xh:prev';
  $rules['rel_prev_links']['DO'][]['addEdge'] = array(
    'attributes' => array(
      'color' => 'grey',
      'style' => 'bold',
      'weight' => 0.1,
    ),
    'reverse' => TRUE,
  );
  */


  // A link formed with noderelation, mapped to 'references'
  // using rdfx_ui mapping.
  // Display this as a directional link.
  // This will only work after you've created such an architecture.
  $rules['link_dc_references']['LABEL'] = 'Entityrelation links to other items, may be mapped with dc:relation';
  $rules['link_dc_references']['ON'][] = 'statement_process';
  $rules['link_dc_references']['IF'][0]['predicate_is']['value'] = 'dc:references';
  $rules['link_dc_references']['DO'][0]['addEdge']['attributes'] = array(
    'color' => 'grey',
    'style' => 'dashed',
    'label' => 'link',
  );

  return $rules;
}

