<?php
// @codingStandardsIgnoreFile
/**
 * @file
 * Extend Full Pager plugin to create show more pager.
 */

class ViewsShowMorePlugin extends views_plugin_pager_full {
  /**
   * Summary title overwrite.
   */
  function summary_title() {
    $count = !empty($this->options['offset']) ? $this->options['offset'] : $this->options['items_per_page'];

    return format_plural(
      $count, 'Initial @skip item, ', 'Initial @skip items, ', array('@skip' => $count))
    . format_plural(
      $this->options['items_per_page'], 'Per click @count item', 'Per click @count items',
      array(
        '@count' => $this->options['items_per_page'],
      )
    );
  }

  /**
   * Options definition overwrite.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['result_display_method'] = array(
      'default' => 'append',
    );
    $options['show_more_text'] = array(
      'default' => t('Show more'),
      'translatable' => TRUE,
    );
    $options['show_more_empty_text'] = array(
      'default' => '',
      'translatable' => TRUE,
    );
    $options['effects'] = array(
      'contains' => array(
        'type' => array('default' => 'none'),
        'speed_type' => array('default' =>  ''),
        'speed' => array('default' => ''),
        'speed_value' => array('default' => ''),
      ),
    );
    $options['advance'] = array(
      'contains' => array(
        'content_selector' => array('default' => ''),
        'pager_selector' => array('default' => ''),
      ),
    );
    return $options;
  }

  /**
   * Options form overwrite.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $exclude = array('total_pages', 'quantity', 'expose', 'tags');
    foreach ($exclude as $ex) {
      unset($form[$ex]['#title']);
      unset($form[$ex]['#description']);
      $form[$ex]['#attributes'] = array('class' => array('element-invisible'));
    }

    $form['items_per_page']['#description'] = t('The number of items to display per show more click.');

    $form['offset']['#title'] = t('Initial items');
    $form['offset']['#description'] = t('The number of items to display initially. Enter 0 for use same as items per page (show more click).');

    // Result display method.
    $form['result_display_method'] = array(
      '#type' => 'select',
      '#title' => t('Result display method'),
      '#description' => t('<strong>Append</strong> result display method append the new content after the existing content on the page in ajax mode and in no-ajax mode replace the content by page refresh. <strong>Replace</strong> result display method replace the content with new content both in ajax and no-ajax mode. In no-ajax mode it refresh the page.'),
      '#options' => array(
        'append' => t('Append'),
        'html' => t('Replace'),
      ),
      '#default_value' => $this->options['result_display_method'] ? $this->options['result_display_method'] : 'append',
      '#weight' => -1,
    );
    // Option for users to specify the text used on the 'show more' button.
    $form['show_more_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Show more pager link text'),
      '#description' => t('Text for the button which used to load more items. Like "Show More".'),
      '#default_value' => $this->options['show_more_text'] ? $this->options['show_more_text'] : t('Show more'),
      '#weight' => -1,
    );
    // Option for users to specify the text used on the 'show more' button
    // when no mor result is found.
    $form['show_more_empty_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Show more pager link text when empty'),
      '#description' => t('Empty text when no more items exist to load. Like "No more results".'),
      '#default_value' => $this->options['show_more_empty_text'] ? $this->options['show_more_empty_text'] : '',
      '#weight' => -1,
    );

    // Affect the way that Views Load More adds new rows.
    $form['effects'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
      '#title' => t('Animation'),
      '#input' => TRUE,
    );
    $form['effects']['type'] = array(
      '#type' => 'select',
      '#options' => array(
        'none' => t('None'),
        'fade' => t('FadeIn'),
        'scroll' => t('Scroll to New Content'),
        'scroll_fade' => t('Scroll to New Content & FadeIn'),
      ),
      '#default_vaue' => 'none',
      '#title' => t('Animation Type'),
      '#default_value' => $this->options['effects']['type'],
    );
    $form['effects']['speed_type'] = array(
      '#type' => 'select',
      '#options' => array(
        'slow' => t('Slow'),
        'fast' => t('Fast'),
        'custom' => t('Custom'),
      ),
      '#states' => array(
        'visible' => array(
          '#edit-pager-options-effects-type' => array(
            array('value' => 'fade'),
            array('value' => 'scroll'),
            array('value' => 'scroll_fade'),
          ),
        ),
      ),
      '#title' => t('Animation Speed'),
      '#default_value' => $this->options['effects']['speed_type'],
    );
    $form['effects']['speed_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Animation speed in ms'),
      '#states' => array(
        'visible' => array(
          '#edit-pager-options-effects-speed-type' => array('value' => 'custom'),
        ),
      ),
      '#default_value' => $this->options['effects']['speed_value'],
      '#element_validate' => array('element_validate_integer_positive'),
    );

    // Advanced options, override default selectors.
    $form['advance'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
      '#title' => t('Advanced Options'),
      '#input' => TRUE,
    );
    // Option to specify the content_selector, which is the wrapping div for
    // views rows. This allows the JS to both find new rows on next pages and
    // know where to put them in the page.
    $form['advance']['content_selector'] = array(
      '#type' => 'textfield',
      '#title' => t('Content jquery selector'),
      '#description' => t('jQuery selector for the rows wrapper, relative to the view container. Use when you override the views markup. Views Show More pager requires a wrapping element for the rows.  Default <strong><code>&gt; .view-content</code></strong>.'),
      '#default_value' => $this->options['advance']['content_selector'],
    );
    // Option to specify the pager_selector, which is the pager relative to the
    // view container.
    $form['advance']['pager_selector'] = array(
      '#type' => 'textfield',
      '#title' => t('Pager jquery selector'),
      '#description' => t('jQuery selector for the pager, relative to the view container. Use when you override the pager markup. Default <strong><code>.pager-show-more</code></strong>.'),
      '#default_value' => $this->options['advance']['pager_selector'],
    );
  }

  /**
   * Options form validate.
   */
  function options_validate(&$form, &$form_state) {
    $effect_speed = $form_state['values']['pager_options']['effects']['speed_type'];
    $effect_speed_val = $form_state['values']['pager_options']['effects']['speed_value'];
    if ($effect_speed == 'custom') {
      if ($effect_speed_val == '') {
        form_set_error('pager_options][effects][speed_value', t('Animation speed is required.'));
      }
      else {
        $form_state['values']['pager_options']['effects']['speed'] = $effect_speed_val;
      }
    }
    else {
      $form_state['values']['pager_options']['effects']['speed'] = $effect_speed;
    }
  }

  /**
   * Query overwrite.
   */
  function query() {
    parent::query();

    $others_page = $this->options['items_per_page'];
    $first_page = !empty($this->options['offset']) ? $this->options['offset'] : $others_page;

    if ($this->current_page == 0) {
      $this->view->query->limit = $first_page;
      $this->view->query->offset = 0;
    }
    else {
      $this->view->query->limit = $others_page;
      $this->view->query->offset = $first_page + (($this->current_page - 1) * $others_page);
    }
  }

  /**
   * Render overwrite.
   */
  function render($input) {
    $pager_theme = views_theme_functions('pager_show_more', $this->view, $this->display);
    $output = theme($pager_theme, array(
      'parameters' => $input,
      'element' => $this->options['id'],
      'result_display_method' => $this->options['result_display_method'],
      'show_more_text' => $this->options['show_more_text'],
      'show_more_empty_text' => $this->options['show_more_empty_text'],
    ));
    return $output;
  }

  /**
   * Get pager total count.
   */
  function get_pager_total() {
    if ($items_per_page = intval($this->get_items_per_page())) {
      if ($initial_items = intval($this->get_offset())) {
        return 1 + ceil(($this->total_items - $initial_items) / $items_per_page);
      }
      else {
        return ceil($this->total_items / $items_per_page);
      }
    }
    else {
      return 1;
    }
  }

  /**
   * Execute the count query, which will be done just prior to the query
   * itself being executed.
   */
  function execute_count_query(&$count_query) {
    $this->total_items = $count_query->execute()->fetchField();
    $this->update_page_info();
    return $this->total_items;
  }

}
