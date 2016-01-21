<?php

/**
 * @file
 * Provides a new Feed item node condition for Context.
 */

define('CONTEXT_FEEDS_FEED_ITEM_NODE_VIEW', 0);
define('CONTEXT_FEEDS_FEED_ITEM_NODE_FORM', 1);
define('CONTEXT_FEEDS_FEED_ITEM_NODE_FORM_ONLY', 2);

class context_feeds_feed_item_node extends context_condition {
  /**
   * {@inheritdoc}
   */
  function condition_values() {
    return [
      'TRUE' => t('Node is a Feed item node'),
      'FALSE' => t('Node is not a Feed item node'),
    ];
  }

  /**
   * {@inheritdoc}
   */
  function condition_form($context) {
    $form = parent::condition_form($context);
    $form['#type'] = 'radios';
    if (empty($form['#default_value'])) {
      $form['#default_value'] = 'TRUE';
    }
    else {
      $form['#default_value'] = current($form['#default_value']);
    }
    return $form;
  }

  /**
   * Condition form submit handler.
   *
   * Storing values in an array because that iss what Context prefers.
   */
  function condition_form_submit($values) {
    return array_filter([$values => $values]);
  }

  /**
   * {@inheritdoc}
   */
  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    return [
      'node_form' => [
        '#title' => t('Set on node form'),
        '#type' => 'select',
        '#options' => [
          CONTEXT_FEEDS_FEED_ITEM_NODE_VIEW => t('No'),
          CONTEXT_FEEDS_FEED_ITEM_NODE_FORM => t('Yes'),
          CONTEXT_FEEDS_FEED_ITEM_NODE_FORM_ONLY => t('Only on node form'),
        ],
        '#description' => t('Set this context on node forms'),
        '#default_value' => isset($defaults['node_form']) ? $defaults['node_form'] : TRUE,
      ],
    ];
  }

  /**
   * Execute the context.
   */
  function execute($node, $op) {
    // Check, if this node is a Feeds item node.
    if (isset($node->nid) && feeds_get_feed_nid($node->nid, 'node')) {
      foreach ($this->get_contexts() as $context) {
        // Check the node form option.
        $options = $this->fetch_from_context($context, 'options');
        if ($op === 'form') {
          $options = $this->fetch_from_context($context, 'options');
          if (!empty($options['node_form']) && in_array($options['node_form'], [CONTEXT_FEEDS_FEED_ITEM_NODE_FORM, CONTEXT_FEEDS_FEED_ITEM_NODE_FORM_ONLY])) {
            $this->condition_met($context, $node->nid);
          }
        }
        elseif (empty($options['node_form']) || $options['node_form'] != CONTEXT_FEEDS_FEED_ITEM_NODE_FORM_ONLY) {
          $this->condition_met($context, $node->nid);
        }
      }
    }
  }
}
