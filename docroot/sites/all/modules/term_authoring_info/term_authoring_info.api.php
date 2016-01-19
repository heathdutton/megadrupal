<?php

/**
 * @file
 * Describe hooks provided by the Taxonomy Term Authoring Information module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Act on taxonomy terms authoring information before they are saved.
 *
 * Modules implementing this hook can act on the term authoring information
 * before it is inserted or updated.
 *
 * @param object $term
 *   A term object.
 */
function hook_term_authoring_info_presave($term) {
  $term->uid = 1;
  $term->created = REQUEST_TIME;
  $term->changed = REQUEST_TIME;
}
