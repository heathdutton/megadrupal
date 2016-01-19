<?php

/**
 * @file drulenium_comments.inc
 * 
 * Base class for drulenium boxes 
 */
abstract class drulenium_default extends boxes_box {

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    return drulenium_test_plugin_defaults($this->test_plugin_name);
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $fieldset = array(
        '#type' => 'fieldset', 
        '#title' => t('Drulenium plugin attributes'), 
        '#collapsible' => TRUE 
    );
    
    return $fieldset + $form = drulenium_test_plugin_fb_settings_form($this->test_plugin_name, $this->options);
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    $block['title'] = $title;
    $block['subject'] = $title;
    $block['delta'] = $this->delta;
    $block['content'] = drulenium_box_view($this);
    return $block;
  }
}