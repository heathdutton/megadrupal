<?php

/**
 * @file views_s3_handler_field.inc
 * Basic S3 field handler.
 */

/**
 * Field handler to provide simple renderer.
 */
class views_s3_actions_handler_field extends views_s3_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['actions'] = array('default' => array());
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $link_options = array();
    foreach (_get_views_s3_actions_info() as $machine_name => $definition) {
      $link_options[$machine_name] = $definition['#title'];
    }
    $form['actions'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Action links'),
      '#description' => t('List of links of actions to be performed on the current file.'),
      '#options' => $link_options,
      '#default_value' => isset($this->options['actions']) ? $this->options['actions'] : array(),
    );
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_list($data, $values) {
    $actions = array();
    foreach (_get_views_s3_actions_info() as $machine_name => $definition) {
      if ($this->options['actions'][$machine_name] && user_access($definition['#perm'])) {
        $actions[] = theme('views_s3_actions_default_action', array(
          'action' => $definition,
          'values' => (array)$values,
        ));
      }
    }
    return theme('views_s3_actions_list', array('actions' => $actions));
  }
  
  /**
   * Render function that renders the link if necessary
   */
   function render($values) {
     $value = $this->get_value($values);
     $values->bucket = $this->query->request->bucket_name;
     return $this->render_list($this->sanitize_value($value), $values);
   }

   /**
    * Called to add the field to a query.
    */
   function query() { /* No query added */ }

}
