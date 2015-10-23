<?php

/**
 * @file
 * Definition of views_idle_filter_handler.
 */

/**
 * Views Idle Filter handler.
 *
 * @ingroup views_filter_handlers
 */
class views_idle_filter_handler extends views_handler_filter {

  function option_definition() {
    $options = parent::option_definition();

    $options['exposed'] = array('default' => TRUE, 'bool' => TRUE);

    $options['content'] = array('default' => '', 'translatable' => TRUE, 'format_key' => 'format');
    $options['format'] = array('default' => NULL);
    $options['tokenize'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Lock the exposed checkbox.
    $form['expose_button']['checkbox']['checkbox']['#disabled'] = TRUE;
    $form['expose_button']['checkbox']['#attributes']['class'][] = 'element-invisible';

    $form['content'] = array(
      '#type' => 'text_format',
      '#title' => t('Additional description'),
      '#default_value' => $this->options['content'],
      '#rows' => 2,
      '#format' => isset($this->options['format']) ? $this->options['format'] : filter_default_format(),
      '#wysiwyg' => FALSE,
    );

    $form['tokenize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use replacement tokens in additional description'),
      '#default_value' => $this->options['tokenize'],
    );

    $options = array();
    $count = 0;

    // Get a list of the available arguments for token replacement.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
    }
    $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => 'ww'));
    $options[t('Global tokens')][t('global tokens')] = t('any global Drupal tokens such as [site:name]');

    $output = '<p>' . t('The following tokens are available. If you would like to have the characters \'[\' and \']\' please use the html entity codes \'%5B\' or  \'%5D\' or they will get replaced with empty space.' . '</p>');
    foreach (array_keys($options) as $type) {
      if (!empty($options[$type])) {
        $items = array();
        foreach ($options[$type] as $key => $value) {
          $items[] = $key . ' == ' . check_plain($value);
        }
        $output .= theme('item_list',
          array(
            'items' => $items,
            'title' => $type,
            'type' => 'ul',
          ));
      }
    }

    $form['token_help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#value' => $output,
      '#id' => 'edit-options-token-help',
      '#dependency' => array(
        'edit-options-tokenize' => array(1),
      ),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }

  function options_submit(&$form, &$form_state) {
    $form_state['values']['options']['format'] = $form_state['values']['options']['content']['format'];
    $form_state['values']['options']['content'] = $form_state['values']['options']['content']['value'];
    parent::options_submit($form, $form_state);
  }

  /**
   * Display the filter on the administrative summary.
   */
  function admin_summary() {
    return check_plain($this->options['expose']['label']);
  }

  function expose_form(&$form, &$form_state) {
    $form['#theme'] = 'views_ui_expose_filter_form';
    // #flatten will move everything from $form['expose'][$key] to $form[$key]
    // prior to rendering. That's why the pre_render for it needs to run first,
    // so that when the next pre_render (the one for fieldsets) runs, it gets
    // the flattened data.
    array_unshift($form['#pre_render'], 'views_ui_pre_render_flatten_data');
    $form['expose']['#flatten'] = TRUE;

    $form['expose']['label'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->options['expose']['label'],
      '#title' => t('Label'),
      '#size' => 40,
    );

    $form['expose']['identifier'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->options['id'],
      '#title' => t('Filter identifier'),
      '#size' => 40,
      '#description' => t('This will appear in the URL after the ? to identify this filter. Cannot be blank.'),
      '#fieldset' => 'more',
    );
  }

  function value_form(&$form, &$form_state) {
    if (!empty($form_state['exposed'])) {
      $content = $this->options['content'];
      if ((strlen($content) > 0) && (!empty($this->options['tokenize']))) {
        $content = $this->view->style_plugin->tokenize_value(token_replace($content, array(), array('clear' => TRUE)), 0);
      }

      $form['value'] = array(
        '#type' => 'item',
        '#markup' => check_markup($content, isset($this->options['format']) ? $this->options['format'] : filter_default_format(), '', FALSE),
        '#theme' => 'container',
        '#theme_wrappers' => array('container'),
        '#attributes' => array(
          'class' => array(
            'form-item',
            'form-item-views-idle-filter',
            'form-item-' . strtr($this->options['expose']['identifier'], array(' ' => '-', '_' => '-', '[' => '-', ']' => '')),
          ),
        ),
      );
    }
  }

  function accept_exposed_input($input) {
    return TRUE;
  }

  function query() {
    // Do nothing.
  }

}
