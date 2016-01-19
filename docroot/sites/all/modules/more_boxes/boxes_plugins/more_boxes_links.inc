<?php
/**
 * @file
 */

define('LINKS', 5);

class more_boxes_links extends boxes_box {
  /**
   * Implementation of boxes_box::options_defaults().
   */
  public function options_defaults() {
    $lnk = array();
    for ($i = 1; $i <= LINKS; $i++) {
      //$lnk["link_{$i}"] = array;
      $lnk["link_{$i}"]["title"] = "";
      $lnk["link_{$i}"]["url"] = "";
    }
    
    $ret = array(
      'link_class' => 'default-link',
    ) + $lnk;

    return $ret;
  }

  /**
   * Implementation of boxes_box::options_form().
   */
  public function options_form(&$form_state) {
    $form = array();
    for ($i = 1; $i <= LINKS; $i++) {
      $form["link_{$i}"] = array(
      '#type' => 'fieldset',
      '#title' => t("Link @i", array('@i' => $i)),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
      $form["link_{$i}"]["title"] = array(
      '#type' => 'textfield',
      '#title' => t("Link title"),
      '#default_value' => isset($this->options["link_{$i}"]["title"]) ? $this->options["link_{$i}"]["title"] : "",
    );
      $form["link_{$i}"]["url"] = array(
        '#type' => 'textfield',
        '#title' => t("Link URL"),
        '#default_value' => isset($this->options["link_{$i}"]["url"]) ? $this->options["link_{$i}"]["url"] : "",
      );
    }

    $form['link_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Class'),
      '#default_value' => $this->options['link_class'],
    );
    return $form;
  }

  /**
   * Implementation of boxes_box::render().
   */
  public function render() {
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    $block['title'] = $title;
    $block['subject'] = NULL;
    $block['delta'] = $this->delta;
    $link_options = array(
      'external' => TRUE,  
      'attributes' => array('class' => $this->options['link_class']),
      ''  
    );
    $output = "";
    foreach ($this->options as $link) {
      if (is_array($link) && !empty($link['url'])) {
        $output .= l($link['title'], $link['url'], $link_options);
      }
    }
    $block['content'] = $output;
    
    return $block;
  }
}