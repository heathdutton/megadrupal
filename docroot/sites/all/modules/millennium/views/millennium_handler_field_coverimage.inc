<?php

class millennium_handler_field_coverimage extends views_handler_field {
  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_node'] = array(
      '#title' => t('Link this field to its node'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_node']),
    );
  }

  function render($values) {
    $biblio_data = unserialize($values->millennium_node_bib_biblio_data);
    $nid = NULL;
    if (isset($this->options['link_to_node']) && $this->options['link_to_node']) {
      $nid = $values->nid;
    }
    $coverimage = theme('millennium_coverimage_widget', array('biblio_data' => $biblio_data, 'nid' => $nid));
    return $coverimage;
  }
}
