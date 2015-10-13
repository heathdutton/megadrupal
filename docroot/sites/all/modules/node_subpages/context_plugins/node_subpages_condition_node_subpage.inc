<?php
/**
 * @file
 * Context module integration for Node Subpages
 */

/**
 * Add context condition for Node Subpages
 */
class node_subpages_condition_node_subpage extends context_condition {
  function condition_values() {
    $values = array();
    foreach (node_subpages_list() as $node_type => $subpages) {
      foreach ($subpages as $subpage_key => $details) {
        // Don't add a condition for the regular node View page
        if ($details['default_view_page']) {
          continue;
        }

        $values[$node_type][$node_type . ':' . $subpage_key] = check_plain($details['title']);
      }
    }
    return $values;
  }

  /**
   * Modify the condition form to group the subpags by Node type
   */
  function condition_form($context) {
    $form = parent::condition_form($context);
    $form['#type'] = 'select';
    $form['#size'] = 12;
    $form['#multiple'] = TRUE;
    $form['#options'] = array();
    foreach (node_subpages_list() as $node_type => $subpages) {
      foreach ($subpages as $subpage_key => $details) {
        // Don't add a condition for the regular node View page
        if ($details['default_view_page']) {
          continue;
        }

        $form['#options'][$node_type][$node_type . ':' . $subpage_key] = check_plain($details['title']);
      }
    }
    return $form;
  }


  function execute($node_type, $subpage_key) {
    foreach ($this->get_contexts($node_type . ':' . $subpage_key) as $context) {
      $this->condition_met($context);
    }
  }
}
