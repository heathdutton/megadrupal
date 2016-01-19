<?php

/**
 * Simple custom text box.
 */
class osf_searchprofiles_box extends boxes_box {
  /**
   * Implementation of boxes_box::options_defaults().
   */
  public function options_defaults() {
    return array(
      'sprofile' => array(
        'value' => '',
      ),
      'context' => array(
        'value' => '',
      ),
      'sp_settings' => array(
         'items' => array(
           'value' => '',
          ),
          'query' => array(
            'value' => '',
          ),
          'output_type' => array(
            'value' => '',
          ),
        ),
    );
  }

  /**
   * Implementation of boxes_box::options_form().
   */
  public function options_form(&$form_state) {
    $form = array();
    $options = $this->options;
    $form['sprofile'] = array(
      '#type' => 'select',
      '#title' => t('Search Profile'),
      '#description' => t('Choose a search profile to render in this block.'),
      '#options' => osf_searchprofiles_sprofile_list(),
      '#default_value' => $options['sprofile'],
    );
    $form['context'] = array(
      '#type' => 'checkbox',
      '#title' => t('Attempt to derive the query from page context'),
      '#description' => t('If page context not found, the default query will be used'),
      '#default_value' => $options['context'],
    );

    // SP per-block settings
    $form['sp_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search profile block settings'),
      '#description' => t('Settings for this search profile in this block.'),
      '#collapsible' => FALSE,
      '#tree' => TRUE,
    );

    $form['sp_settings']['items'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of items'),
      '#description' => t('Number of items to retrieve'),
      '#size' => 4,
      '#maxlength' => 4,
      '#required' => TRUE,
      '#default_value' => $options['sp_settings']['items'],
    );

    $form['sp_settings']['output_type'] = array(
      '#type' => 'select',
      '#title' => t('Output type'),
      '#description' => t('Would you like this block to output a simple list of links or fully themed search results?'),
      '#options' => array('links' => 'List of links', 'results' => 'Fully themed search results'),
      '#default_value' => $options['sp_settings']['output_type'],
      '#required' => TRUE,
    );

    $form['sp_settings']['query'] = array(
      '#type' => 'textfield',
      '#title' => t('Query'),
      '#description' => t('The search term(s) to use (if not derived from context)'),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
      '#default_value' => $options['sp_settings']['query'],
    );
    
    $form['sp_settings']['hide_if_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide if empty'),
      '#default_value' => $this->options['hide_if_empty'],
      '#description' => t('If checked, this option will hide the box from non-administrative users if the content is empty.')
    );    
    
    return $form;
  }

  /**
   * Implementation of boxes_box::render().
   */
  public function render() {
    $sprofile = $this->options['sprofile'];
    $block_settings = $this->options;
    $title = $sprofile;
    $content = osf_searchprofiles_render($sprofile, $block_settings);

    $hide = $this->options['sp_settings']['hide_if_empty'] && ($content == '<div class="item-list"></div>' || strpos($content, 'Your search yielded no results.') !== FALSE);
    
    return array(
      'delta' => $this->delta, // Crucial.
      'title' => !$hide ? $title : '',
      'subject' => !$hide ? check_plain($title) : '',
      'content' => !$hide ? $content : '',
      'hide_if_empty' => TRUE,
    );
  }
}
