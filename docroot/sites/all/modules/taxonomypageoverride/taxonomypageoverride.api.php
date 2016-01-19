<?php

/**
 * @file
 * Hooks provided by the Taxonomy Term Page Override module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Specify taxonomy term page overrides
 *
 * Return associative array specifying the path to menu items which are called
 * instead of the default page handler at taxonomy/term/%. You may return the
 * path to a view or to a menu router defined by a custom module. The first
 * instance of percent sign (%) is replaced by the tid of the term.
 *
 * The returned array must be structured in the following way:
 * - On the base level us the keys 'vocabulary' or 'term' to specify the scope
 *   of an override.
 * - On the second level specify the vocabulary or term by entering the
 *   machine_name of the vocabulary or the tid of the term respectively.
 * - On the third level specify which flavour of page you want to override.
 *   Currently 'page' and 'feed' are accepted.
 * - Assign an override specification in form of an associative array 
 *   containing the following keys:
 *   - A path (required) pointing at the new menu handler which will be used
 *     instead of the default taxonomy term page.
 *   - The optional 'reset_title' and 'reset_breadcrumb' options specifying
 *     whether the original title and breadcrum should be restored after
 *     rendering the page.
 *   - An 'access callback'. If not specified it is first looked up in the
 *     overriden menu item and if this is not available, 'user_access' is used.
 *   - The 'access arguments' array is optional and defaults to array('access
 *     content') if the access callback is 'user_access' and to array(2)
 *     otherwise.
 *   - The option 'recursive' applicable for taxonomy terms. When set, a given
 *     override will also be applied to all subterms of the term the override
 *     is assigned to.
 */
function hook_taxonomypageoverride_info() {
  $items['vocabulary']['tags']['page'] = array(
    'path' => 'path-to-custom-view/%',
    'reset_title' => TRUE,
    'reset_breadcrumb' => TRUE,
  );

  $items['term'][42]['page'] = array(
    'path' => 'what-was-that-question-again-view/%',
    'reset_title' => FALSE,
    'reset_breadcrumb' => FALSE,
    'recursive' => TRUE,
  );

  return $items;
}

/**
 * @} End of "addtogroup hooks"
 */
