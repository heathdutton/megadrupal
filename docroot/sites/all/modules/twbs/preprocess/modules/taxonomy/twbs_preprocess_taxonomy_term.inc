<?php

/**
 * Process variables for taxonomy-term.tpl.php.
 */
function twbs_preprocess_taxonomy_term(&$variables) {
  $variables['view_mode'] = $variables['elements']['#view_mode'];
  $variables['term'] = $variables['elements']['#term'];
  $term = $variables['term'];

  $uri = entity_uri('taxonomy_term', $term);
  $variables['term_url']  = url($uri['path'], $uri['options']);
  $variables['term_name'] = check_plain($term->name);
  $variables['page']      = $variables['view_mode'] == 'full' && taxonomy_term_is_page($term);

  // Flatten the term object's member fields.
  $variables = array_merge((array) $term, $variables);

  // Helpful $content variable for templates.
  $variables['content'] = array();
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // field_attach_preprocess() overwrites the $[field_name] variables with the
  // values of the field in the language that was selected for display, instead
  // of the raw values in $term->[field_name], which contain all values in all
  // languages.
  field_attach_preprocess('taxonomy_term', $term, $variables['content'], $variables);

  // Gather classes, and clean up name so there are no underscores.
  $vocabulary_name_css = str_replace('_', '-', $term->vocabulary_machine_name);
  $variables['classes_array'][] = 'vocabulary-' . $vocabulary_name_css;

  $variables['theme_hook_suggestions'][] = 'taxonomy_term__' . $term->vocabulary_machine_name;
  $variables['theme_hook_suggestions'][] = 'taxonomy_term__' . $term->tid;
}
