<?php
/**
 * @file
 * context_vocabulary class, extends context_condition_node
 */

/**
 * Vocabulary as a context condition.
 */
class context_vocabulary_context_condition_vid extends context_condition_node {
  function condition_values() {
    $values = array();
    foreach (taxonomy_get_vocabularies() as $vocab) {
      $values[$vocab->machine_name] = check_plain($vocab->name);
    }
    return $values;
  }

  /**
   * Condition form.
   */
  function condition_form($context) {
    $form = parent::condition_form($context);
    $form['#type'] = 'checkboxes';
    $form['#multiple'] = TRUE;

    foreach (taxonomy_get_vocabularies() as $vocab) {
      $opts[$vocab->machine_name] = check_plain($vocab->name);
    }

    $form['#options'] = $opts;
    return $form;
  }

  /**
   * Execute.
   */
  function execute($node, $op) {
    if ($this->condition_used()) {
      // Build a list of taxonomy fields available on the current node.
      $fields = field_info_fields();
      $instance_fields = field_info_instances('node', $node->type);
      $check_fields = array();
      foreach ($instance_fields as $key => $field_info) {
        if ($fields[$key]['type'] == 'taxonomy_term_reference' ||
          ($fields[$key]['type'] == 'entityreference' && $fields[$key]['settings']['target_type'] == 'taxonomy_term')) {
          $check_fields[] = $key;
        }
      }

      if ($this->condition_used() && !empty($check_fields)) {
        foreach ($check_fields as $field) {
          if ($terms = field_get_items('node', $node, $field)) {
            foreach ($terms as $term) {
              $taxonomy_term = isset($term['tid']) ? taxonomy_term_load($term['tid']) : taxonomy_term_load($term['target_id']);
              foreach ($this->get_contexts($taxonomy_term->vocabulary_machine_name) as $context) {
                // Check the node form option.
                if ($op === 'form') {
                  $options = $this->fetch_from_context($context, 'options');
                  if (!empty($options['node_form'])) {
                    return $this->condition_met($context, $taxonomy_term->vocabulary_machine_name);
                  }
                }
                else {
                  return $this->condition_met($context, $taxonomy_term->vocabulary_machine_name);
                }
              }
            }
          }
        }
      }
    }
  }
}
