<?php

/**
 * @file
 * Contains class for Field Collection block class.
 */

namespace Drupal\dynamic_panes_fc_layout;

use Drupal\dynamic_panes\Block;

/**
 * Class for Field Collection block class.
 */
class FieldCollectionBlock extends Block {

  /**
   * @var int
   */
  protected $blockWrapperId;

  /**
   * @var int
   */
  protected $delta;

  /**
   * Implements Block::init().
   */
  public function init($context) {
    if (isset($context['block_wrapper_id'])) {
      $this->blockWrapperId = $context['block_wrapper_id'];
    }

    if (isset($context['delta'])) {
      $this->delta = $context['delta'];
    }
  }

  /**
   * Implements Block::initAdminLinks().
   */
  protected function initAdminLinks() {
    $this->adminLinks = array();

    $this->adminLinks['edit-block'] = array(
      'title' => t('Edit block'),
      'href' => $this->getAdminLinkUrl('edit'),
      'query' => array(drupal_get_destination()),
      'attributes' => array('title' => t('Edit block')),
    );

    $this->adminLinks['delete-block'] = array(
      'title' => t('Delete block'),
      'href' => $this->getAdminLinkUrl('delete'),
      'query' => array(drupal_get_destination()),
      'attributes' => array('title' => t('Delete block')),
    );
  }

  /**
   * Implements Block::preRender().
   */
  protected function preRender() {
    $content = array();
    if ($this->data) {
      $content = entity_view('bean', array($this->data), $this->data->view_mode);
    }
    return $content;
  }

  /**
   * Implements Block::getID().
   */
  public function getID() {
    return $this->data->bid;
  }

  /**
   * Get url for edit/delete block.
   *
   * @param string $op
   *   The 'edit' or 'delete' used as part of url.
   *
   * @return string
   *   The url of link.
   */
  protected function getAdminLinkUrl($op) {
    $path = array(
      'dynamic-panes-fc-layout',
      $this->blockWrapperId,
      $this->delta,
      $op,
    );

    return implode('/', $path);
  }
}
