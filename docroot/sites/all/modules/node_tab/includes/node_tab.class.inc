<?php

/**
 * Entity class representing the node_tab entity type.
 */
class NodeTab extends Entity {

  /**
   * The node type on which this tab is used.
   */
  public $node_type;

  /**
   * The machine name of the tab. Used in URLs.
   *
   * @var string
   */
  public $name;

  /**
   * The label of the tab.
   *
   * @var string
   */
  public $label;

  /**
   * The weight of the tab.
   *
   * @var int
   */
  public $weight;

  public function __construct($values = array()) {
    parent::__construct($values, 'node_tab');
  }
}
