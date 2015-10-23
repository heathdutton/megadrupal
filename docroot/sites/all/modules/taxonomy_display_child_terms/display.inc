<?php

/**
* @file
* Taxonomy Display term display extra plugin class.
*/

/**
* Add a taxonomy display plugin to use mini panels for displaying the term.
*/
class TaxonomyDisplayChildTermDisplay extends TaxonomyDisplayTermDisplayHandler {
  /**
   * Prepare our output data in a build array for when a term is viewed.
   *
   * @see TaxonomyDisplayTermDisplayHandler::displayTerm()
   */
  public function displayTerm($term, $options = NULL) {
    $build = array('term' => FALSE);

    // Current Term
    $build['term'][$term->tid] = taxonomy_term_view($term, 'parent');

    // Build child if available
    $childs = taxonomy_get_children($term->tid, $term->vid);
    if (!empty($childs)) {
      foreach ($childs as $term) {
        $build['term'][$term->tid] = taxonomy_term_view($term, 'children');
      }
    }

    return $build;
  }

  /**
   * Provide a form for user configuration of our plugin for the vocabulary.
   *
   * @see TaxonomyDisplayHandlerForm::formFieldset()
   */
  public function formFieldset(&$form, &$values, $options = NULL) {}

  /**
   * Prepare the values to be stored by Taxonomy Display on our behalf.
   *
   * Anything returned in this method will be provided to our displayTerm()
   * method above when a taxonomy term is viewed using our plugin.
   *
   * @see TaxonomyDisplayHandlerForm::formSubmit()
   */
  public function formSubmit($form, &$values) {}
}