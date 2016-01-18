<?php
// $Id: ynews_style_plugin.inc,v 1.1.2.2.2.2 2011/02/06 03:27:25 quicksketch Exp $

/**
 * @file
 * Contains the ynews style plugin.
 */

/**
 * Style plugin to render each item in an ordered or unordered list.
 *
 * @ingroup views_style_plugins
 */
class ynews_style_plugin extends views_plugin_style {
  function option_definition() {
    $options = parent::option_definition();
    $options['logo_path'] = array('default' => 'misc/powered-black-80x15.png');
    $options['channel_description'] = array('default' => 'Test description');
    $opts = array('nid', 'title', 'body', 'description', 'author', 'pubdate', 'category', 'enclosure', 'enclosure2');
    foreach ($opts as $opt_name) {
      $options[$opt_name] = array('default' => '');
    }
   
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $handlers = array_keys($this->display->handler->get_handlers('field'));
    // make array keys equal to values
    $handlers = array_combine($handlers, $handlers);
    $handlers = array(''=>t('<Select field>')) + $handlers;

    //drupal_set_message(print_r($handlers, true));
    
    parent::options_form($form, $form_state);
    $form['logo_path'] = array(
      '#title' => 'website logo path',
      '#type' => 'textfield',
      '#description' => t('Path to website .gif logo, relative to Drupal root, max size: 100px * 100px'),
      '#required' => TRUE,
      '#default_value' => $this->options['logo_path'],
    );

    $form['channel_description'] = array(
      '#title' => t('Channel description'),
      '#type' => 'textarea',
      '#required' => TRUE,
      '#default_value' => $this->options['channel_description'],
    );


    $form['nid'] = array(
      '#title' => 'item:node nid',
      '#type' => 'select',
      '#options' => $handlers,
      '#required' => TRUE,
      '#default_value' => $this->options['nid'],
    );
    $form['title'] = array(
      '#title' => 'item:title',
      '#type' => 'select',
      '#options' => $handlers,
      '#required' => TRUE,
      '#default_value' => $this->options['title'],
    );
    $form['body'] = array(
      '#title' => 'item:yandex-fulltext',
      '#type' => 'select',
      '#options' => $handlers,
      '#required' => TRUE,
      '#default_value' => $this->options['body'],
      '#description' => t('Typically, node body'),
    );
    $form['description'] = array(
      '#title' => 'item:description',
      '#type' => 'select',
      '#options' => $handlers,
      '#default_value' => $this->options['description'],
    );
    $form['author'] = array(
      '#title' => 'item:author',
      '#type' => 'select',
      '#options' => $handlers,
      '#default_value' => $this->options['author'],
    );
    $form['pubdate'] = array(
      '#title' => 'item:pubDate',
      '#type' => 'select',
      '#options' => $handlers,
      '#description' => t('Publication date'),
      '#default_value' => $this->options['pubdate'],
    );
    $form['category'] = array(
      '#title' => 'item:category',
      '#type' => 'select',
      '#options' => $handlers,
      '#description' => t('News item category (text)'),
      '#default_value' => $this->options['category'],
    );
    $form['enclosure'] = array(
      '#title' => 'item:enclosure',
      '#type' => 'select',
      '#options' => $handlers,
      '#default_value' => $this->options['enclosure'],
      '#description' => t('Image field, min-width:100px, max-width:600px'),
    );
    $form['enclosure2'] = array(
      '#title' => 'item:enclosure2',
      '#type' => 'select',
      '#options' => $handlers,
      '#default_value' => $this->options['enclosure2'],
      '#description' => t('Another image field, min-width:100px, max-width:600px'),
    );
  }

  function validate() {
    $errors = parent::validate();
    if ($this->display->handler->use_pager()) {
      $errors[] = t('The ynews style cannot be used with a pager. Disable the "Use pager" option for this display.');
    }
    return $errors;
  }

}
