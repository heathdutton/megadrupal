<?php

/**
 * @file
 * Contains abstract class for block classes.
 */

namespace Drupal\dynamic_panes;

/**
 * Abstract class for block classes.
 */
abstract class Block {

  /**
   * @var mixed
   */
  protected $data;

  /**
   * @var array
   */
  protected $adminLinks;

  /**
   * Constructor for this block.
   *
   * @param mixed $data
   *   Contains content of block.
   * @param array $context
   *   An array contains any info about context.
   */
  public function __construct($data, $context) {
    $this->data = $data;
    $this->adminLinks = array();
    $this->init($context);
    $this->initAdminLinks();
  }

  /**
   * Init block.
   *
   * @param array $context
   *   An array contains any info about context.
   */
  protected abstract function init($context);

  /**
   * Init admin links of this block.
   */
  protected abstract function initAdminLinks();

  /**
   * Pre render block.
   *
   * @return array
   *   The renderable array.
   */
  protected abstract function preRender();

  /**
   * Get block ID.
   *
   * @return mixed
   *   The unique block id.
   */
  public abstract function getID();

  /**
   * Get admin links for this block.
   *
   * @return array
   *   An array contains admin links.
   */
  public function getAdminLinks() {
    return $this->adminLinks;
  }

  /**
   * Get admin links of this block by type.
   *
   * @param string $type
   *   The type of admin links.
   *
   * @return array
   *   An array contains info about each link of this type.
   */
  public function getAdminLinksByType($type) {
    return isset($this->adminLinks[$type]) ? $this->adminLinks[$type] : array();
  }

  /**
   * Render content of this block.
   *
   * @return string
   *   The rendered content of this block.
   */
  public function render() {
    $output = $this->preRender();
    return render($output);
  }
}
