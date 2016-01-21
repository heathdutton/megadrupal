<?php

/**
 * @file
 * Definition of views_plugin_pager_some.
 */

/**
 * Plugin for sessions more link with a context to machine name
 *
 * @ingroup views_pager_plugins
 */
class views_plugin_pager_session_more extends views_plugin_pager_full {
  function option_definition() {
    $options = parent::option_definition();
    $options['pager_label'] = array('default' => t('More Sessions'));
    $options['tokenize'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // Remove full pager options.
    $form['total_pages']['#access'] = $form['quantity']['#access']  = $form['tags']['#access']  = $form['expose']['#access'] = FALSE;
    $form['pager_label'] = array(
      '#type' => 'textfield',
      '#title' => 'Label for pager',
      '#required' => TRUE,
      '#description' => t('Create a label to use for the pager'),
      '#default_value' => $this->options['pager_label'],
    );
    // @TODO: Refactor token handling into a base class.
    $form['tokenize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use replacement tokens from the first row'),
      '#default_value' => $this->options['tokenize'],
    );

        // Get a list of the available fields and arguments for token replacement.
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $options[t('Fields')]["[$field]"] = $handler->ui_name();
    }

    $count = 0; // This lets us prepare the key as we want it printed.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
    }

    if (!empty($options)) {
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
              'type' => $type
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
  }

  function use_pager() {
    return TRUE;
  }

  function render($input) {
    // Special HTML stuff because I don't want to create a template yet.
    if ($this->total_items > $this->options['items_per_page']) {
      $value = $this->options['pager_label'];
      if ($this->options['tokenize']) {
        $value = $this->view->style_plugin->tokenize_value($value, 0);
      }
      $link = t('<a href="@url">@label</a>', array('@url' => url(current_path() . '/' . str_replace('_', '-', $this->view->args[2])), '@label' => $value));
      $wrapper = '<div class="view-pager">' . $link . '</div>';
      return $wrapper;
    }
  }
}
