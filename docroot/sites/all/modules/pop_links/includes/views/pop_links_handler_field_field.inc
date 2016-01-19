<?php

/**
 * @file
 *
 * Views handler to extend the normal Field API field handler. Allows users to mark certain
 * fields as representing external URLs that should be tracked; this also requires each row
 * to contain a valid node ID field.
 */

class pop_links_handler_field_field extends views_handler_field_field {
  
  function get_items($values) {
    if ($this->options['alter']['make_link'] == TRUE && $this->options['alter']['external'] == TRUE) {
      global $user;
      if (_pop_links_track($user)) {
        $entity = $this->get_value($values, 'entity');
        // Check to see if it's a node
        if (isset($entity->nid) && isset($entity->vid) && isset($entity->type)) {
          // Check to see if it's one of the node types with external link tracking enabled
          $node_types = variable_get('pop_links_node_types', array('page'));
          if (in_array($entity->type, $node_types)) {
            // Add Javascript settings for tracking.
            $view_name = $this->view->name;
            $wrapper = '.view.view-id-' . $view_name;
            $row_index = $this->view->row_index;
            $row_wrapper = '.views-row-' . ($row_index + 1);
            $field_name = $this->field;
            $field_wrapper = '.views-field-' . drupal_clean_css_identifier($field_name);
            $selector = $wrapper . ' ' . $row_wrapper . ' ' . $field_wrapper;
            $setting = array(
              'pop_links' => array(
                $selector => array(
                  'nid' => $entity->nid,
                ),
              ),
            );
            // Remember the path to the Javascript file across function calls (saves memory):
            static $js_path = -1;
            if ($js_path == -1) {
              $js_path = drupal_get_path('module', 'pop_links') . '/pop_links.js';
            }
            $sheets = drupal_add_js($setting, array('type' => 'setting'));
            if (!isset($sheets[$js_path])) {
              drupal_add_js($js_path);
            }
          }
        }
      }
    }
    return parent::get_items($values);
  } 
}