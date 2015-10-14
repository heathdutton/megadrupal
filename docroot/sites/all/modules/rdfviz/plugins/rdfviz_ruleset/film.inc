<?php

/**
 * @file
 * RDFViz rendering rules for a "Film" schema. Using fb.film .
 */

$plugin = array(
  'title' => t('Film'),
  'ruleset_callback' => 'rdfviz_film_rendering_ruleset',
  'description' => t('Render information about a film, such as its genre, runtime, director and actors. Uses concepts from fb.film '),
);
function rdfviz_film_rendering_ruleset() {
  $rules = array();

  // What is a 'film' and how should it be rendered.
  $rules['show_film_type']['ON'][0] = 'statement_process';
  $rules['show_film_type']['IF'][0]['AND'][0]['predicate_is']['value'] = 'rdf:type';
  $rules['show_film_type']['IF'][0]['AND'][1]['object_is']['value'] = 'fb:film.film';
  #$rules['show_film_type']['IF'][0]['AND'][1]['OR'][]['object_is']['value'] = 'fb:film.film';
  #$rules['show_film_type']['IF'][0]['AND'][1]['OR'][]['object_is']['value'] = 'schema:Movie';
  $rules['show_film_type']['DO'][0]['addNode']['attributes']['shape'] = 'note';

  $rules['link_director_to_film']['ON'][0] = 'statement_process';
  $rules['link_director_to_film']['IF'][0]['predicate_is']['value'] = 'fb:film.film.directed_by';
  #$rules['link_director_to_film']['IF'][0]['OR'][]['predicate_is']['value'] = 'fb:film.film.directed_by';
  #$rules['link_director_to_film']['IF'][0]['OR'][]['predicate_is']['value'] = 'schema:director';
  $rules['link_director_to_film']['DO'][0]['addEdge']['attributes'] = array(
    'style' => 'solid',
    'color' => 'darkslateblue',
    'label' => 'Director',
    'len' => 2,
  );

  $rules['link_writer_to_film']['ON'][0] = 'statement_process';
  $rules['link_writer_to_film']['IF'][0]['predicate_is']['value'] = 'fb:film.film.written_by';
  #$rules['link_writer_to_film']['IF'][0]['OR'][]['predicate_is']['value'] = 'fb:film.film.written_by';
  #$rules['link_writer_to_film']['IF'][0]['OR'][]['predicate_is']['value'] = 'schema:author';
  $rules['link_writer_to_film']['DO'][0]['addEdge']['attributes'] = array(
    'style' => 'solid',
    'color' => 'darkslateblue',
    'label' => 'Writer',
    'len' => 2,
  );

  $rules['link_producer_to_film']['ON'][0] = 'statement_process';
  $rules['link_producer_to_film']['IF'][0]['predicate_is']['value'] = 'fb:film.film.produced_by';
  #$rules['link_producer_to_film']['IF'][0]['OR'][]['predicate_is']['value'] = 'fb:film.film.produced_by';
  #$rules['link_producer_to_film']['IF'][0]['OR'][]['predicate_is']['value'] = 'schema:producer';
  $rules['link_producer_to_film']['DO'][0]['addEdge']['attributes'] = array(
    'style' => 'solid',
    'color' => 'darkslateblue',
    'label' => 'Producer',
    'len' => 2,
  );

  // Even if all we have is a name on the page,
  // (haven't really been told that the producer is typeof person)
  // it should still be styled as a person.
  // The ideal state is for the link to be wrapped in a full typeof wrapper
  // and get styled through it's own attributes. But most often a reference
  // will just be a href.

  // A rule to style the OBJECT of a statement.
  // Normally that doesn't happen a lot. Use the 'reverse'
  // argument to trigger this.
  /*
  $rules['link_producer_to_film']['DO'][1]['addNode'] = array(
    // Normally addnode aplies to the subject.
    // Setting reverse allows us to addnode representing the object.
    'reverse' => TRUE,
    'attributes' => array(
      'shape' => 'egg',
      'style' => 'filled',
      'fillcolor' => 'mistyrose',
      'color' => 'mistyrose3',
    ),
  );
  */



  $rules['show_a_film_performance']['ON'][] = 'statement_process';
  $rules['show_a_film_performance']['IF'][0]['AND'][0]['predicate_is']['value'] = 'rdf:type';
  $rules['show_a_film_performance']['IF'][0]['AND'][1]['object_is']['value'] = 'fb:film.performance';
  $rules['show_a_film_performance']['DO'][0]['addNode']['attributes'] = array(
    'shape' => 'invhouse',
    'label' => 'role',
    'color' => 'blue',
  );

  $rules['link_actor_to_performance']['ON'][] = 'statement_process';
  $rules['link_actor_to_performance']['IF'][0]['predicate_is']['value'] = 'fb:film.performance.actor';
  $rules['link_actor_to_performance']['DO'][0]['addEdge']['attributes'] = array(
    #'style' => 'dotted',
    'color' => 'darkslateblue',
    #'len' => 0.5,
  );
  // The character name is plain label
  $rules['link_role_to_performance']['ON'][] = 'statement_process';
  $rules['link_role_to_performance']['IF'][0]['predicate_is']['value'] = 'fb:film.performance.character';
  $rules['link_role_to_performance']['DO'][0]['addNode']['attributes'] = array(
    'shape' => 'box',
    'id' => '!o',
    'label' => '"!o"',
  );
  $rules['link_role_to_performance']['DO'][0]['addEdge']['attributes'] = array(
    #'style' => 'dotted',
    'color' => 'darkslateblue',
    #'len' => 0.5,
  );

  $rules['link_performance_to_film']['ON'][0] = 'statement_process';
  // Got some unexpected bnodes showing up. Ensure we only show real types
  $rules['link_performance_to_film']['IF'][0]['AND'][0]['object_type_is']['value'] = 'uri';
  $rules['link_performance_to_film']['IF'][0]['AND'][1]['predicate_is']['value'] = 'fb:film.film.starring';
  $rules['link_performance_to_film']['DO'][0]['addEdge']['attributes'] = array(
    'style' => 'dashed',
    'color' => 'blue',
    'len' => 2,
  );

  $rules['show_a_film_job']['ON'][0] = 'statement_process';
  $rules['show_a_film_job']['IF'][0]['AND'][0]['predicate_is']['value'] = 'rdf:type';
  $rules['show_a_film_job']['IF'][0]['AND'][1]['object_is']['value'] = 'fb:film.film_crew_gig';
  $rules['show_a_film_job']['DO'][0]['addNode']['attributes'] = array(
    'shape' => 'invhouse',
    'label' => 'job',
    'color' => 'darkgreen',
  );

  $rules['link_job_to_film']['ON'][0] = 'statement_process';
  $rules['link_job_to_film']['IF'][0]['AND'][0]['predicate_is']['value'] = 'fb:film.film.other_crew';
  // Got some unexpected bnodes showing up. Ensure we only show real types
  $rules['link_job_to_film']['IF'][0]['AND'][1]['object_type_is']['value'] = 'uri';
  $rules['link_job_to_film']['DO'][0]['addEdge']['attributes'] = array(
    'style' => 'dotted',
    'color' => 'darkgreen',
    'len' => 2,
  );

  $rules['link_person_to_job']['ON'][0] = 'statement_process';
  $rules['link_person_to_job']['IF'][0]['predicate_is']['value'] = 'fb:film.film_crew_gig.crewmember';
  $rules['link_person_to_job']['DO'][0]['addEdge']['attributes'] = array(
    #'style' => 'dotted',
    'color' => 'darkslateblue',
    #'len' => 0.5
  );
  $rules['link_job_description_to_job']['ON'][0] = 'statement_process';
  $rules['link_job_description_to_job']['IF'][0]['predicate_is']['value'] = 'fb:film.film_crew_gig.film_crew_role';
  $rules['link_job_description_to_job']['DO'][0]['addEdge']['attributes'] = array(
    #'style' => 'dotted',
    'color' => 'darkslateblue',
    #'len' => 0.5
  );
  /*

*/


  return $rules;
}

