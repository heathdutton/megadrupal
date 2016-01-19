<?php

/**
 * @file
 * Condition definition to check the node page with particular node templates
 */

/**
 * Condition to lookup for particular node with the particular node templates
 */
class ContextNodeTemplateContextCondition extends context_condition {
  /**
   * Options form. Provide additional options for your condition.
   */
  public function condition_form($context) {
    $values = $this->fetch_from_context($context, 'values');
    $templates = _get_page_templates();
    if(isset($templates['default'])){
      unset($templates['default']);
    }

    return array(
      '#type' => 'checkboxes',
      '#title' => t('Node Page Template'),
      '#options' => $templates,
      '#default_value' => !empty($values) ? $values : array(),
      '#description' => t('Select the node page template name'),
    );
  }

  /**
   * Condition form submit handler.
   */
  public function condition_form_submit($values) {
    $output = array();
    foreach ($values as $value) {
      if (trim($value)) {
        $output[] = trim($value);
      }
    }

    return $output;
  }

  /**
   * Public method that is called from hooks or other integration.
   */
  public function execute($item) {
    foreach ($this->get_contexts() as $context) {
      $node_tpls = $this->fetch_from_context($context, 'values');
      foreach ($node_tpls as $node_tpl) {
        if ($this->match($node_tpl, $item, $context)) {
          $this->condition_met($context, $node_tpl);
          break;
        }
      }
    }
  }

  /**
   * Check if node template.
   *
   * @param string $node_tpl
   *   An string of the current node template name.
   * @param array $item
   *   An array of current node.
   * @param object $context
   *   An object of context.
   */
  public function match($node_tpl, $item, $context) {
    $node = $item['page_arguments'][0];
    context_node_template_node_prepare($node);
    $match_translation = $this->fetch_from_context($context, 'options');
    if ($match_translation) {
      return ($node->template == $node_tpl || ($node->tnid != 0 && $node->tnid == $nid));
    }
    else {
      return ($node->template == $node_tpl);
    }
  }
}
