<?php

/**
 * @file
 * Provide views handler so that Amazon Image can be displayed in the various
 * ways that are available.
 *
 * @author rfay
 *
 */

class views_handler_field_amazon_image extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['image_size'] = array('default' => 'smallimage');
    $options['link_format'] = array('default' => 'amazon');
    $options['presentation_format'] = array('default' => 'markup');
    return $options;
  }
  /**
   * Override init function to provide generic option to link to node.
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    $this->additional_fields['height'] = 'height';
    $this->additional_fields['width'] = 'width';
    $this->additional_fields['detailpageurl'] = array(
      'table' => 'amazon_item', 'field' => 'detailpageurl', 'value' => 'amazon_item_detailpageurl'
    );
    $this->additional_fields['asin'] = 'asin';
    $this->additional_fields['url'] = 'url';
  }

  function ensure_my_table() {
    if (empty($this->table_alias)) {
      $join_extra = array();
      if (!empty($this->options['image_size'])) {
        $join_extra[] = array('field' => 'size', 'value' => $this->options['image_size'], 'numeric' => FALSE);
      }

      $join = new views_join();
      $join->construct($this->table, 'amazon_item', 'asin', 'asin', $join_extra);

      $this->table_alias = $this->query->ensure_table($this->table, $this->relationship, $join);
    }
    return $this->table_alias;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['image_size'] = array(
      '#title' => t('Image size'),
      '#type' => 'select',
      '#options' => array(
        'smallimage' => t('Small'),
        'mediumimage' => t("Medium"),
        'largeimage' => t("Large"),
      ),
      '#default_value' => $this->options['image_size'],
    );

    $form['link_format'] = array(
      '#title' => t('Link behavior'),
      '#type' => 'radios',
      '#options' => array(
        'plain' => t('No link'),
        'amazon' => t("A link to the product's Amazon page"),
      ),
      '#default_value' => $this->options['link_format'],
    );
    if (module_exists('amazon_store')) {
      $form['link_format']['#options']['amazon_store'] = t("A link to the product's Amazon Store page (Amazon Store Module)");
    }
    $form['presentation_format'] = array(
      '#title' => t('Presentation format'),
      '#type' => 'select',
      '#options' => array(
        'markup' => t('HTML img markup'),
        'plain_url' => t('Plain URL to image'),
      ),
      '#default_value' => $this->options['presentation_format'],
    );
  }

  function render($values) {
    $url = $this->get_value($values, 'url');
    $asin = $this->get_value($values, 'asin');

    // We may not have a URL. It's not guaranteed that Amazon will give us one.
    if (empty($url)) {
      return;
    }
    $attributes = array(
      'height' => $this->get_value($values, 'height'),
      'width' => $this->get_value($values, 'width'),
    );

    // Choose presentation style
    if ($this->options['presentation_format'] == 'markup') {
      $image = theme('image', array('path' => $url, 'alt' => NULL, 'title' => NULL, 'attributes' => $attributes, 'getsize' => FALSE));
    }
    else {
      $image = $url;
    }

    switch ($this->options['link_format']) {
      case 'plain':
        return $image;
        break;
      case 'amazon':
        if ($detailpageurl = $this->get_value($values, 'detailpageurl')) {
          return l($image, $detailpageurl, array('html' => TRUE));
        }
        else {
          return $image;
        }
        break;
      case 'amazon_store':
        return l($image, AMAZON_STORE_PATH . '/item/' . $asin, array('html' => TRUE));
        break;
    }
  }
}
