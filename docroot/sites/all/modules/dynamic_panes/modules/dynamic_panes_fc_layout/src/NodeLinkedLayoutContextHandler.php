<?php

/**
 * @file
 * Contains class for get layout from node context.
 */

namespace Drupal\dynamic_panes_fc_layout;

use Drupal\dynamic_panes\ContextHandler;

/**
 * Class for get layout from node context.
 */
class NodeLinkedLayoutContextHandler extends ContextHandler {

  /**
   * Implements ContextHandler::initLayouts().
   */
  protected function initLayouts() {
    if ($this->context->is_type('node') && !empty($this->context->data)) {
      $node = $this->context->data;
      $is_enabled = variable_get('dynamic_panes_fc_layout_enabled_' . $node->type, FALSE);

      if ($is_enabled) {
        $wrapper = entity_metadata_wrapper('node', $node);
        if (isset($wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_NAME})) {
          if ($layout = $wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_NAME}->value()) {
            $this->addLayout($wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_NAME});
          }
        }
      }
    }
  }
}
