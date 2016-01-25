<?php
/**
 * @file
 */

class  more_boxes_list_2 extends boxes_box {

  public function options_defaults() {
    return array(
      'vid' => NULL,
    );
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = array();
    $vocab_options['all'] = t('All');
    foreach (taxonomy_get_vocabularies() as $vocab) {
      $vocab_options[$vocab->vid] = $vocab->name;
    }
    $form['vid'] = array(
      '#type' => 'select',
      '#title' => t('Vocabulary'),
      '#options' => $vocab_options,
      '#default_value' => $this->options['vid'],
    );
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block['title'] = $title;
    $block['subject'] = NULL;
    $block['delta'] = $this->delta;
    $vid = $this->options['vid'];
    $block['content'] = views_embed_view('more_boxes_group_by_term', 'block_1', $vid);
    return $block;
  }

}