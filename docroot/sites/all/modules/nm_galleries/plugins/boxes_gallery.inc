<?php

/**
 * Simple custom text box.
 */
class boxes_gallery extends boxes_box {
  /**
   * Implementation of boxes_box::options_defaults().
   */
  public function options_defaults() {
    return array(
      'gallery' => array(
        'value' => '',
      ),
    );
  }

  /**
   * Implementation of boxes_box::options_form().
   */
  public function options_form(&$form_state) {
    $form = array();

    $optionsq = db_select('node', 'n')
      ->fields('n', array('nid', 'title'))
      ->condition('type', 'nm_gallery')
      ->execute()->fetchAll();

    $options = array();
    foreach ($optionsq as $k) {
      $options[$k->nid] = $k->title . ' [' . $k->nid . ']';
    }

    if ($options) {
      $form['gallery'] = array(
        '#type' => 'select',
        '#base_type' => 'select',
        '#title' => t('Featured Gallery'),
        '#options' => $options,
        '#default_value' => $this->options['gallery'],
        '#description' => t('The gallery shown to the user.'),
      );
    }
    else {
      $destination = drupal_get_destination();
      $link = l('create a gallery', 'node/add/nm-gallery', array('query' => array('destination' => $destination['destination']),'attributes' => array('title' => 'create a gallery')));
      $form['gallery'] = array(
        '#type' => 'markup',
        '#markup' => t('You don\'t have any Galleries created yet.  Please go !link first.', array('!link' => $link)),
        '#title' => t('Featured Gallery'),      );
    }


    return $form;

  }

  /**
   * Implementation of boxes_box::render().
   */
  public function render() {
    $content = '';
    if (!empty($this->options['gallery'])) {
      $node = node_load($this->options['gallery']);
      $content = render(node_view($node, 'nm_embed_gallery'));

    }
    $title = isset($this->title) ? $this->title : NULL;
    return array(
      'delta' => $this->delta, // Crucial.
      'title' => $title,
      'subject' => $title,
      'content' => $content,
    );
  }
}
