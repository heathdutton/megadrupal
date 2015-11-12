<?php

/**
 * @file
 * Contains abstract class for region classes.
 */

namespace Drupal\dynamic_panes;

/**
 * Abstract class for region classes.
 */
abstract class Region {

  /**
   * @var mixed
   */
  protected $data;

  /**
   * @var Layout
   */
  protected $layout;

  /**
   * @var array
   */
  protected $blocks;

  /**
   * @var array
   */
  protected $adminLinks;

  /**
   * Constructor for this region.
   *
   * @param mixed $data
   *   Contains any info about region.
   * @param Layout $layout
   *   The layout object.
   */
  public function __construct($data, $layout) {
    $this->data = $data;
    $this->layout = $layout;
    $this->blocks = array();
    $this->addBlockLinks = array();
    $this->initBlocks();
    $this->initAdminLinks();
  }

  /**
   * Init blocks of this region.
   */
  protected abstract function initBlocks();

  /**
   * Init admin links of this region.
   */
  protected abstract function initAdminLinks();

  /**
   * Get type of this region.
   *
   * @return string
   *   The human readable name of this region.
   */
  public abstract function getRegionType();

  /**
   * Get region ID.
   *
   * @return mixed
   *   The unique region id.
   */
  public abstract function getRegionID();

  /**
   * Get region name.
   *
   * @return string
   *   The region name of this layout.
   */
  public abstract function getRegionName();

  /**
   * Get region name.
   *
   * @return string
   *   The region name of this layout.
   */

  /**
   * Get form element name of this region used in sort form.
   *
   * @param int $level
   *   The blocks level.
   *
   * @return string
   *   The form element name of this region.
   */
  public abstract function getFormElementName($level);

  /**
   * Wrap block to block class.
   *
   * @param object $block
   *   The object contains any info about block.
   * @param array $context
   *   An array contains any info about context.
   *
   * @return Block|bool
   *   Block object if exist, FALSE otherwise.
   */
  protected function wrapBlock($block, $context = array()) {
    $layout_handler = $this->layout->getLayoutHandler();
    $class = $layout_handler->getBlockClass();
    $reflection = new \ReflectionClass($class);
    if ($reflection->isSubclassOf('\Drupal\dynamic_panes\Block')) {
      return new $class($block, $context);
    }

    return FALSE;
  }

  /**
   * Get admin links for this region.
   *
   * @return array
   *   An array contains admin links.
   */
  public function getAdminLinks() {
    return $this->adminLinks;
  }

  /**
   * Get admin links of this region by type.
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
   * Get blocks of this region.
   *
   * @return array
   *   An array contains blocks attached to this region.
   */
  public function getBlocks() {
    return $this->blocks;
  }

  /**
   * Get blocks of this region by level.
   *
   * @param int $level
   *   The blocks level.
   *
   * @return Block[]
   *   An array contains blocks attached to this region.
   */
  public function getBlocksByLevel($level) {
    return isset($this->blocks[$level]) ? $this->blocks[$level] : array();
  }

  /**
   * Form constructor for sort.
   *
   * @param array $form
   *   An array contains form structure.
   */
  public function sortForm(&$form) {
    $submit = get_class($this) . '::sortFormSubmit';

    if (!isset($form['#sort_handlers']) || !in_array($submit, $form['#sort_handlers'])) {
      $form['#sort_handlers'][] = $submit;
    }
  }

  /**
   * Form submission handler for Region::sortForm().
   */
  public static function sortFormSubmit($form, &$form_state) {
  }
}
