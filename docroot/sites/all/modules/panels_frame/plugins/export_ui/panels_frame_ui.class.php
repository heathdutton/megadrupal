<?php

abstract class panels_frame_ui extends ctools_export_ui {

  function hook_menu(&$items) {
    parent::hook_menu($items);

    // For added convienience, allow additional operation links to be available
    // as local tasks.
    $prefix = $this->plugin['menu']['menu prefix'] . '/' . $this->plugin['menu']['menu item'] . '/list/%ctools_export_ui/';
    $prefix_length = strlen($prefix);
    $ops = array('delete', 'revert', 'clone');

    foreach ($items as $path => &$item) {
      if (substr($path, 0, $prefix_length) === $prefix) {
        $str[$path] = $item;
        if (in_array(substr($path, $prefix_length), $ops)) {
          $item['type'] = MENU_LOCAL_TASK;
        }
      }
    }
  }

  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);

    // Add a filter element for Category.
    $options = array('all' => t('- All -'));
    foreach ($this->items as $item) {
      if (!empty($item->category)) {
        $options[$item->category] = $item->category;
      }
    }

    $form['top row']['category'] = array(
      '#type' => 'select',
      '#title' => t('Category'),
      '#options' => $options,
      '#default_value' => 'all',
      '#weight' => -10,
    );

    $form['top row']['search']['#size'] = 20;
  }

  function list_table_header() {
    $headers = parent::list_table_header();

    // Replace the "Title" header with "Label".
    foreach ($headers as $key => $value) {
      if ($value['data'] == t('Title')) {
        $headers[$key]['data'] = t('Label');
        break;
      }
    }

    // Insert the Category header after the first two headers.
    $column = array(array('data' => t('Category'), 'class' => array('ctools-export-ui-category')));
    array_splice($headers, 2, 0, $column);
    return $headers;
  }

  function list_build_row($item, &$form_state, $operations) {
    parent::list_build_row($item, $form_state, $operations);

    // Set up additional sorting for Label and Category.
    switch ($form_state['values']['order']) {
      case 'label':
        $this->sorts[$item->name] = $item->label;
        break;
      case 'category':
        $this->sorts[$item->name] = ($item->category ? $item->category : t('Miscellaneous')) . $item->label;
        break;
    }

    // Add an additional Category column after the first two columns.
    $category = $item->category ? check_plain($item->category) : t('Miscellaneous');
    $column = array(array('data' => $category, 'class' => array('ctools-export-ui-category')));
    array_splice($this->rows[$item->name]['data'], 2, 0, $column);
  }

  function list_css() {
    parent::list_css();
    ctools_add_css('panels-frame.ui', 'panels_frame');
  }

  function list_render(&$form_state) {
    $table = array(
      'header' => $this->list_table_header(),
      'rows' => $this->rows,
      'empty' => $this->plugin['strings']['message']['no items'],
      'attributes' => array(
        'id' => 'ctools-export-ui-list-items',
        'class' => array('panels-frame-ui'),
      ),
    );
    return theme('table', $table);
  }

  function list_sort_options() {
    $options = parent::list_sort_options();

    // Replace option labels "title" with "Label"
    $options['disabled'] = t('Enabled, Label');
    $options['label'] = t('Label');

    // Add an additional Category option after the first three options.
    $category_option = array('category' => t('Category'));
    array_splice($options, 3, 0, $category_option);

    return $options;
  }

  function list_filter($form_state, $item) {
    // Additional filter logic for Category.
    if ($form_state['values']['category'] != 'all' && $form_state['values']['category'] != $item->category) {
      return TRUE;
    }
    return parent::list_filter($form_state, $item);
  }

  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    // Customize edit form for our preferences.
    $form['info']['label']['#title'] = t('Label');
    $form['info']['label']['#size'] = 30;
    $form['info']['name']['#size'] = 30;

    $form['info']['description']['#rows'] = 2;
    $form['info']['description']['#resizable'] = FALSE;

    // Add the Category form element.
    $form['info']['category'] = array(
      '#type' => 'textfield',
      '#size' => 24,
      '#default_value' => $form_state['item']->category,
      '#title' => t('Category'),
      '#description' => t('What category this layout should appear in. If left blank the category will be "Miscellaneous".'),
    );
  }

  function edit_form_validate(&$form, &$form_state) {
    parent::edit_form_validate($form, $form_state);

    // Validate Categories to make sure they don't contain illegal characters.
    if (isset($form_state['values']['category'])) {
      if (preg_match("/[^A-Za-z0-9 \-_:]/", $form_state['values']['category'])) {
        form_error($form['info']['category'], t('Categories may contain only alphanumeric characters, hyphens, colons, underscores, or spaces.'));
      }
    }
  }

  function edit_form_submit(&$form, &$form_state) {
    parent::edit_form_submit($form, $form_state);

    // The frame should be saved to the database now, so we should be able to
    // remove the object cache.
    panels_frame_cache_clear($form_state['item']->plugin, $form_state['cache_key']);
  }

  function delete_form_submit(&$form_state) {
    parent::delete_form_submit($form_state);

    // Also remove from object cache to keep things tidy.
    $cache_key = 'edit-' . $form_state['item']->name;
    panels_frame_cache_clear($form_state['item']->plugin, $cache_key);
  }
}
