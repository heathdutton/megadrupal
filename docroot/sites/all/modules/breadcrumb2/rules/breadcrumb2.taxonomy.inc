<?php

/**
 * @file
 * Contains rules integration for the breadcrumb2 module needed during taxonomy related evaluation.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Action: Fetch lightest term from node.
 */
function breadcrumb2_rules_fetch_lightest_term_from_node(EntityDrupalWrapper $node) {
  $term = breadcrumb2_node_get_lightest_term($node->value());
  if (!empty($term)) {
    $term = taxonomy_term_load($term->tid);
  }
  return array('lightest_term' => $term);
}

/**
 * Action: Fetch lightest term from node.
 */
function breadcrumb2_rules_taxonomy_get_parents_all(EntityDrupalWrapper $taxonomy_term) {

  $parent_terms = taxonomy_get_parents_all($taxonomy_term->tid->value());
  $parent_terms = array_reverse($parent_terms);
  $return = array();
  foreach ($parent_terms as $parent_term) {
    $return[$parent_term->tid] = taxonomy_term_load($parent_term->tid);
  }
  //$return = array();
  return array('parent_terms' => $return);
}


/********************Helper Function *************************************************/  

/**
 * Returns the lightest term for a given node.
 * Copy from custom_breadcrumbs_taxonomy module.
 *
 * If the term has parents, then the lightest parent's weight is used for the
 * term weight. And if the parent has multiple child terms at different depths,
 * the deepest child term will be returned. If the child terms have the same
 * depth, then the lightest child term is returned.
 *
 * @param $node
 *   The node object.
 *
 * @return
 *   The taxonomy term object.
 */
function breadcrumb2_node_get_lightest_term($node) {
  $terms = breadcrumb2_node_get_terms($node);
  if (!empty($terms)) {
    if (count($terms) > 1) {
      foreach ($terms as $term) {
        // Only consider terms in the lightest vocabulary.
        if (!isset($vid)) {
          $vid = $term->vid;
        }
        elseif ($term->vid != $vid) {
          continue;
        }
        // If the term has parents, the weight of the term is the weight of the lightest parent.
        $parents = taxonomy_get_parents_all($term->tid);
        $depth = count($parents);
        if ($depth > 0) {
          $parent = array_pop($parents);
          $weight = $parent->weight;
        }
        else {
          $weight = $term->weight;
        }
        if ((isset($lweight) && ($weight < $lweight)) || !isset($lweight)) {
          $lterm = $term;
          $lweight = $weight;
          $ldepth = $depth;
        }
        elseif (isset($lweight) && ($weight == $lweight)) {
          // If the node has multiple child terms with the same parent, choose the child with the greatest depth.
          if ($depth > $ldepth) {
            $lterm = $term;
            $ldepth = $depth;
          }
          elseif ($depth == $ldepth) {
            // If the terms have the same depth, pick the term with the lightest weight.
            $lterm = ($lterm->weight < $term->weight) ? $lterm : $term;
          }
        }
      }
      return $lterm;
    }
    else {
      return array_pop($terms);
    }
  }
}

/**
 * Copy from custom_breadcrumbs_taxonomy module.
 * Finds all terms associated with a node.
 * This is a D7 Replacement for Drupal 6 taxonomy_node_get_terms.
 */
function breadcrumb2_node_get_terms($node, $key = 'tid') {
  static $terms;
  if (isset($node->nid) && isset($node->vid)) {
    if (!isset($terms[$node->vid][$key])) {
      $query = db_select('taxonomy_index', 'r');
      $t_alias = $query->join('taxonomy_term_data', 't', 'r.tid = t.tid');
      $v_alias = $query->join('taxonomy_vocabulary', 'v', 't.vid = v.vid');
      $query->fields( $t_alias );
      $query->condition("r.nid", $node->nid);
      $query->orderBy('v.weight');
      $query->orderBy('t.weight');
      $query->orderBy('t.name');
      $result = $query->execute();
      $terms[$node->vid][$key] = array();
      foreach ($result as $term) {
        $terms[$node->vid][$key][$term->$key] = $term;
      }
    }
    return $terms[$node->vid][$key];
  }
  return array();
}

