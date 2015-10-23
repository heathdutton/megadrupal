<?php

/**
 * @file
 * Contains the views ui tabs style plugin.
 */

/**
 * Style plugin to render each item in an add to cart form
 *
 * @ingroup views_style_plugins
 */
class commerce_views_display_plugin_style_add_to_cart_form extends views_plugin_style {
  public $default_product_id;
  public $default_line_item_id;

  /**
   * Initialize a style plugin.
   *
   * @param $view
   * @param $display
   * @param $options
   *   The style options might come externally as the style can be sourced
   *   from at least two locations. If it's not included, look on the display.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);

    // process inputs from query params
    $this->_process_query_params();
  }

  // Set default options
  function option_definition() {
    $options = parent::option_definition();
    $options += array(
      'show_quantity' => array('default' => FALSE),
      'default_quantity' => array('default' => 1),
      'combine' => array('default' => TRUE),
      'show_single_product_attributes' => array('default' => FALSE),
      'display_path' => array('default' => FALSE),
      'line_item_type' => array('product' => t('Product')),
      'default_view_mode' => array('default' => 'line_item'),
      'force_pager_none' => array('default' => TRUE),
    );

    return $options;
  }

  /**
   * Options form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['show_quantity'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a textfield quantity widget on the add to cart form.'),
      '#default_value' => $this->options['show_quantity'],
    );

    $form['default_quantity'] = array(
      '#type' => 'textfield',
      '#title' => t('Default quantity'),
      '#default_value' => $this->options['default_quantity'] <= 0 ? 1 : $this->options['default_quantity'],
      '#element_validate' => array('commerce_cart_field_formatter_settings_form_quantity_validate'),
      '#size' => 16,
    );

    $form['combine'] = array(
      '#type' => 'checkbox',
      '#title' => t('Attempt to combine like products on the same line item in the cart.'),
      '#description' => t('The line item type, referenced product, and data from fields exposed on the Add to Cart form must all match to combine.'),
      '#default_value' => $this->options['combine'],
    );

    $form['show_single_product_attributes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show attribute widgets even if the Add to Cart form only represents one product.'),
      '#description' => t('If enabled, attribute widgets will be shown on the form with the only available options selected.'),
      '#default_value' => $this->options['show_single_product_attributes'],
    );

    // Add a conditionally visible line item type element.
    $types = commerce_product_line_item_types();

    if (count($types) > 1) {
      $form['line_item_type'] = array(
        '#type' => 'select',
        '#title' => t('Add to Cart line item type'),
        '#options' => array_intersect_key(commerce_line_item_type_get_name(), drupal_map_assoc($types)),
        '#default_value' => !empty($this->options['line_item_type']) ? $this->options['line_item_type'] : 'product',
      );
    }
    else {
      $form['line_item_type'] = array(
        '#type' => 'hidden',
        '#value' => key($types),
      );
    }

    $form['display_path'] = array(
      '#type' => 'checkbox',
      '#title' => t("Link products added to the cart from this View to this View's path if displayed as a page."),
      '#default_value' => $this->options['display_path'],
    );

    $entity_info = entity_get_info('commerce_product');
    $view_mode_options = array();
    $default_view_mode = !empty($this->options['default_view_mode']) ? $this->options['default_view_mode'] : 'line_item';
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $view_mode => $view_mode_data) {
        $view_mode_options[$view_mode] = isset($view_mode_data['label']) ? $view_mode_data['label'] : $view_mode;
      }

      if (count($view_mode_options) > 1) {
        $form['default_view_mode'] = array(
          '#type' => 'select',
          '#title' => t('Fallback view mode'),
          '#options' => array_map('check_plain', $view_mode_options),
          '#default_value' => $default_view_mode,
        );
      }
      else {
        $form['default_view_mode'] = array(
          '#type' => 'hidden',
          '#value' => key($view_mode_options),
        );
      }

    }
    else {
      $form['default_view_mode'] = array(
        '#type' => 'hidden',
        '#value' => $default_view_mode,
      );
    }

    $form['force_pager_none'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override pager and render add to cart form with all items.'),
      '#default_value' => $this->options['force_pager_none'],
    );
  }

  /**
   * Called to add the base field to a query.
   */
  function query($use_groupby = FALSE) {
    // add the base field
    $this->base_field_alias = $this->view->query->add_field($this->view->base_table, $this->view->base_field);

    // remove field's field_alias to stop any processing by
    // views_handler_field_field::post_execute()
    /** @todo: remove after #1591678 is resolved. ***/
    if (!empty($this->view->field)) {
      $field_keys = array_keys($this->view->field);
      foreach ($field_keys as $field_id) {
        // ensure field alias is set to unknown to resolve #1591678
        $this->view->field[$field_id]->field_alias = 'unknown';

        // ensure original value isset to resovle #1548240
        if (!property_exists($this->view->field[$field_id], 'original_value')) {
          $this->view->field[$field_id]->original_value = NULL;
        }
      }
    }

    // force view to show all
    if (!empty($this->options['force_pager_none'])) {
      $this->view->set_items_per_page(0);
    }
  }

  /**
   * Render the grouping sets.
   *
   * Plugins may override this method if they wish some other way of handling
   * grouping.
   *
   * @param $sets
   *   Array containing the grouping sets to render.
   * @param $level
   *   Integer indicating the hierarchical level of the grouping.
   *
   * @return string
   *   Rendered output of given grouping sets.
   */
  function render_grouping_sets($sets, $level = 0) {
    $output = '';
    foreach ($sets as $set) {
      $row = reset($set['rows']);
      // Render as a grouping set.
      if (is_array($row) && isset($row['group'])) {
        $output .= theme(views_theme_functions('views_view_grouping', $this->view, $this->display),
          array(
            'view' => $this->view,
            'grouping' => $this->options['grouping'][$level],
            'grouping_level' => $level,
            'rows' => $set['rows'],
            'fields' => $set['fields'],
            'title' => $set['group'],
            'add_to_cart_form' => !empty($set['add_to_cart_form']) ? $set['add_to_cart_form'] : array(),
            'product_display_id' => $set['product_display_id'],
          )
        );
      }
      // Render as a record set.
      else {
//dpm($set['rows'], 'rows in render sets');
        if ($this->uses_row_plugin()) {
          foreach ($set['rows'] as $index => $row) {
            $this->view->row_index = $index;
            $set['rows'][$index] = $this->row_plugin->render($row);
          }
        }
//dpm($set['rows'], 'rows in render sets after row_plugin');
        $output .= theme($this->theme_functions(), array(
          'view' => $this->view,
          'options' => $this->options,
          'grouping_level' => $level,
          'rows' => $set['rows'],
          'fields' => $set['fields'],
          'title' => $set['group'],
          'add_to_cart_form' => !empty($set['add_to_cart_form']) ? $set['add_to_cart_form'] : array(),
          'product_display_id' => $set['product_display_id'],
        ));
      }
    }
    unset($this->view->row_index);
    return $output;
  }

  /**
   * Group records as needed for rendering.
   */
  function render_grouping($records, $groupings = array(), $group_rendered = NULL) {
    $sets = parent::render_grouping($records, $groupings, $group_rendered);
    if (empty($sets)) {
      return $sets;
    }

    // Extract a default quantity for the Add to Cart form line item.
    $default_quantity = $this->options['default_quantity'] <= 0 ? 1 : $this->options['default_quantity'];

    // set display path
    $line_item_display_path = NULL;
    if ($this->options['display_path']) {
      if ($this->view->display[$this->view->current_display]->display_plugin == 'page') {
        // set to page path
        $line_item_display_path = $this->view->display[$this->view->current_display]->display_options['path'];
      }
      else {
        // fallback to current url
        $line_item_display_path = current_path();
      }
    }

    // process sets
    foreach ($sets as &$set) {
      $row_product_ids = array();
      $product_ids_index_map = array();
      foreach ($set['rows'] as $index => $row) {
        if (isset($row->product_id)) {
          $row_product_ids[] = $row->product_id;
          $product_ids_index_map[$row->product_id] = $index;
        }
      } // /row loop

      if (empty($row_product_ids)) {
        continue;
      }

      // Load all the active products intended for sale
      $products = commerce_product_load_multiple($row_product_ids, array('status' => 1));
      if (empty($products)) {
        continue;
      }

      // extract all product ids for form building
      $product_ids = array_keys($products);

      // set default product
      if (!isset($this->default_product_id) || !isset($products[$this->default_product_id])) {
        $this->default_product_id = reset($product_ids);
      }

      $default_product = $products[$this->default_product_id];
      $default_product_index = $product_ids_index_map[$default_product->product_id];

      // Build the line item we'll pass to the Add to Cart form.
      if (isset($this->default_line_item_id)) {
        $default_line_item = commerce_line_item_load($this->default_line_item_id);
        $default_line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $default_line_item);

        // Copy field instances from template to the current line item
        $line_item = commerce_product_line_item_new($default_product, $default_quantity, 0, array(), $this->options['line_item_type']);
        $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

        foreach (field_info_instances('commerce_line_item', $this->options['line_item_type']) as $field_name => $instance) {
          if (isset($default_line_item->$field_name)) {
            $line_item->$field_name = $default_line_item->$field_name;
          }
        }
      }
      else {
        $line_item = commerce_product_line_item_new($default_product, $default_quantity, 0, array(), $this->options['line_item_type']);
      }

      // initialize line item context
      if (!isset($line_item->data['context'])) {
        $line_item->data['context'] = array();
      }

      // populate context with plugin options
      $line_item->data['context'] = array(
        'product_ids' => $product_ids,
        'add_to_cart_combine' => $this->options['combine'],
        'show_single_product_attributes' => $this->options['show_single_product_attributes'],
      ) + $line_item->data['context'];

      // Add the display path to the line item's context data array if specified.
      if (isset($line_item_display_path)) {
        $line_item->data['context']['display_path'] = $line_item_display_path;
      }

      // Store the View data in the context data array as well.
      $line_item->data['context']['view'] = array(
        'view_name' => $this->view->name,
        'display_name' => $this->view->current_display,
        'human_name' => $this->view->human_name,
        'page' => $this->view->current_page,
      );

      // get form id
      $form_id = commerce_cart_add_to_cart_form_id($product_ids, $default_quantity);

      // Create a unique id
      $product_display_id_parts = array(
        'commerce_views_display',
        $this->view->name,
        $this->view->current_display,
        $form_id,
      );

      $product_display_id = drupal_html_id(implode('-', $product_display_id_parts));

      // build fields
      $field_displays = array();
      $set['fields'][$default_product_index] = array();
      if (!empty($this->view->field)) {
        $field_keys = array_keys($this->view->field);

        foreach ($field_keys as $field_id) {
          $field_handler = $this->view->field[$field_id];
          $is_field = property_exists($field_handler, 'field_info');
          $field_name = $field_handler->real_field;

          $field_displays[$field_id] = array(
            'field_id' => $field_id,
            'real_name' => $field_handler->real_field,
            'is_field' => $is_field,
            'product_display_id' => $product_display_id,
            'default_view_mode' => 'commerce_product_' . $this->options['default_view_mode'],
          );

          if ($is_field) {
            $field_name = $field_handler->definition['field_name'];
            $field_displays[$field_id]['field_name'] = $field_name;
            $field_displays[$field_id]['display'] = array(
              'type' => $field_handler->options['type'],
              'settings' => $field_handler->options['settings'],
              'label' => 'hidden',
            );
          }

          $set['fields'][$default_product_index][$field_id] = commerce_views_display_field_view_field('commerce_product', $default_product, $field_name, $field_displays[$field_id], $this->get_field($default_product_index, $field_id));
        }
      }

      // Build the Add to Cart form using the prepared values.
      $cart_context = array(
        'view_mode' => 'commerce_product_' . $this->options['default_view_mode'],
        'class_prefix' => $product_display_id, // must be same as prefix for $field_replacement_class
        'commerce_views_display' => array('field_displays' => $field_displays),
      );

      $form = drupal_get_form($form_id, $line_item, $this->options['show_quantity'], $cart_context);
      $set['add_to_cart_form'] = $form;
      $set['product_display_id'] = $product_display_id;

      // truncate set rows
      $set['rows'] = array($default_product_index => $set['rows'][$default_product_index]);

    } // /set loop
    unset($set);

//dpm($sets, '$sets');
    return $sets;
  }

  /**
   * Initialize default entities based on url parameters
   */
  function _process_query_params() {
    /** @todo: make these views filters? ****/

    // Set default line item and product
    if (isset($_GET['default_line_item'])) {
      $default_line_item_input = $_GET['default_line_item'];
      if (is_numeric($default_line_item_input) && intval($default_line_item_input) == $default_line_item_input && $default_line_item_input > 0) {
        $default_line_item_input = (int) $default_line_item_input;
        $line_item = commerce_line_item_load($default_line_item_input);
        if (!empty($line_item)) {
          $this->_clone_line_item($line_item);
        }
      }
    }
    elseif (isset($_GET['default_product'])) {
      // Only set default product
      $default_product_input = $_GET['default_product'];
      if (is_numeric($default_product_input) && intval($default_product_input) == $default_product_input && $default_product_input > 0) {
        $this->default_product_id = (int) $default_product_input;
      }
    }
  }

  /**
   * Returns cloned line item
   */
  function _clone_line_item($line_item) {
    if (!empty($line_item)) {
      $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

      // Line item must have a product, order must be accessible and line
      // item types must match.
      if (!empty($wrapper->commerce_product) &&
          commerce_order_access('view', $wrapper->order->value()) &&
          $line_item->type == $this->options['line_item_type']) {
        $this->default_line_item_id = $wrapper->getIdentifier();
        $this->default_product_id = $wrapper->commerce_product->getIdentifier();
      }
    }
  }
}
