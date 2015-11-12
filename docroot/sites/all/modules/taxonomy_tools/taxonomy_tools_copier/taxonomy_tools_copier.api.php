<?php

/**
 * @file
 * Hooks provided by the Taxonomy Copier module.
 */

/**
 * Make changes to the copy of taxonomy term.
 *
 * @param stdClass $term
 *   Taxonomy term object
 */
function hook_taxonomy_tools_copier_term_copy_alter(&$term) {
  $term->name = 'New term';
}
