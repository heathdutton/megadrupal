<?php

namespace Drupal\campaignion_action;

class ActionBase {
  protected $type;
  protected $node;

  public static function fromNode($node) {
    if (isset($node->action)) {
      return $node->action;
    }
    if ($type = TypeBase::fromContentType($node->type)) {
      // give type the control over which class is used.
      return $type->actionFromNode($node);
    }
  }

  public function __construct(TypeInterface $type, $node) {
    $this->type = $type;
    $this->node = $node;
    $this->node->action = $this;
  }

  /**
   * Called whenever hook_node_presave() is called on this node.
   */
  public function presave() {
    $node = $this->node;
    if (isset($node->translation_source)) {
      $_SESSION['webform_template'] = $node->translation_source->nid;
    } else {
      if (!isset($node->nid) && empty($node->webform['components'])) {
        if ($nid = $this->type->defaultTemplateNid()) {
          $_SESSION['webform_template'] = $nid;
        }
      }
    }
  }

  /**
   * Called whenever hook_node_prepare is called on this node.
   */
  public function prepare() {
    $node = $this->node;
    if (module_exists('webform_ajax') && isset($node->webform)) {
      $node->webform += array(
        'webform_ajax' => 1,
      );
    }
  }

  /**
   * Called whenever the node is saved (either by update or insert).
   */
  public function save() {
  }

  /**
   * Called whenever hook_node_update() is called on this node.
   */
  public function update() {
    $this->save();
  }

  /**
   * Called whenever hook_node_insert() is called on this node.
   */
  public function insert() {
    $this->save();
  }
}
