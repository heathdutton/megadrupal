<?php

/**
 * @file
 * Base class for all elements (has an nid, but is not a node) in
 *  the FileMaker module.
 */

abstract class FmElement extends FmAdmin {

/**********************************************************************************
 * Database Fields (Public, to work with save() in FmBase).
 *********************************************************************************/

  /**
   * Unique identifier of the node this elelement is related to.
   *
   * @int $nid
   */
  public $nid;

  /**
   * The name of the element, as stored in FileMaker.
   *
   * @string $name
   */
  public $name;

  /**
   * The sort order of the element, as it should appear on nodes.
   *
   * @int $weight
   */
  public $weight;

/**********************************************************************************
 * Public functions.
 *********************************************************************************/

  /**
   * Grabs all of the records of the current class that are related to a node.
   */
  public function get_by_nid($nid) {

    $class = get_class($this);
    
    $query = db_select($class::TABLE_NAME, 't');
    $query
      ->condition('nid', $nid)
      ->fields('t')
      ->orderBy('weight');

    $result = $query->execute();
    $result = $result->fetchAllAssoc($class::ID_FIELD_NAME);
    
    return $this->build_multiple($result);
  }

}
