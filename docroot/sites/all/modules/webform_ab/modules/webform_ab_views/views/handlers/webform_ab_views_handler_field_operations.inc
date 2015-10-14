<?php

/**
 * Field handler to show a Webform A/B Test's links
 */
class webform_ab_views_handler_field_operations extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['nid'] = 'nid';
  }
  
  function option_definition() {
    $options = parent::option_definition();

    $options['add_destination'] = TRUE;

    return $options;
  }
  

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['add_destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add a destination parameter to edit and delete operation links so users return to this View on form submission.'),
      '#default_value' => $this->options['add_destination'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $nid = $this->get_value($values, 'nid');

    // Get the contextual links.
    $links = menu_contextual_links('node', 'node', array($nid));
    $additional_links = array(
      'node-webforms' => 'node/' . $nid . '/webforms',
      'node-reports' => 'node/' . $nid . '/reports'
    );
    foreach($additional_links as $id => $path) {
      $links[$id] = menu_get_item($path);
    }

    if (!empty($links)) {
      // Add the destination parameter for the node-edit and node-delete links.
      if ($this->options['add_destination']) {
        foreach ($links as $id => &$link) {
          if (in_array($id, array('node-edit', 'node-delete'))) {
            $link['query'] = drupal_get_destination();
          }
        }
      }
      
      drupal_add_css(drupal_get_path('module', 'webform_ab_views') . '/css/webform_ab_views.css');
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('webform_ab_views', 'links', 'inline', 'operations'))));
    }
  }
}
