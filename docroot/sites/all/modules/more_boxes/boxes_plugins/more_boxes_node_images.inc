<?php
/**
 * @file
 * more_boxes_node_images class.
 */

class more_boxes_node_images extends boxes_box {

  /**
   * Implementation of boxes_box::options_defaults().
   */
  public function options_defaults() {
    return array(
        'nid' => NULL,
        'image' => NULL,
        'link_class' => 'default-links',
    );
  }

  /**
   * Implementation of boxes_box::options_form().
   */
  public function options_form(&$form_state) {
    $form['nid'] = array(
      '#type' => 'textfield',
      '#title' => t('Node nid'),
      '#default_value' => $this->options['nid'],
    );
    $form['image'] = array(
      '#type' => 'textfield',
      '#title' => t('Image field (machine_name)'),
      '#default_value' => $this->options['image'] ? $this->options['image'] : 'field_image',
    );
    return $form;
  }

  /**
   * Implementation of boxes_box::render().
   */
  public function render() {
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    $block['title'] = $title;
    $block['subject'] = $title;
    $block['delta'] = $this->delta;
    $node = node_load($this->options['nid']);
    if ($node) {
      node_build_content($node);   
      $content = drupal_render($node->content[$this->options['image']]);
      $block['content'] = $content;
    }
    return $block;
  }

}