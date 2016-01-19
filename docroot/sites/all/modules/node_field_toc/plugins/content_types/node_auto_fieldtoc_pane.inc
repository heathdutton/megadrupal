<?php

/**
 * @file
 * Defines the Node field auto toc in-page navigation pane.
 */

$plugin = array(
  'title' => t('Node auto-field TOC pane'),
  'content_types' => 'node_auto_fieldtoc_pane',
  'single' => TRUE,
  'defaults' => array(
    'toc_title' => 'Table of Contents',
    'view_mode' => 'full',
    'list_type' => 'ul',
    'toc_class' => '',
    'ignore_body' => 'Ignore',
  ),
  'render callback' => 'node_auto_fieldtoc_pane_render',
  'description' => t('An automatic TOC for fields in a node based on visible fields in a view mode.'),
  'edit form' => 'node_auto_fieldtoc_pane_edit_form',
  'category' => array(t('Miscellaneous'), -9),
  'required context' => array(
    new ctools_context_required(t('Node'), 'node'),
  ),
);

/**
 * This ctools content type provides a ul pane with in page links to a node.
 */
function node_auto_fieldtoc_pane_render($subtype, $conf, $args, $context) {
  // Quick return if context is not present.
  if (empty($context)) {
    return;
  }

  // Check for configuration.
  if (!isset($conf)) {
    $conf = array(
      'toc_title' => 'Table of Contents',
      'view_mode' => 'full',
      'list_type' => 'ul',
      'toc_class' => '',
    );
  }

  // Create block for output later.
  $block = new stdClass();

  // Get "this node" context from all available contexts.
  foreach ($context as $curr_context) {
    if ($curr_context->type[2] == 'node') {
      $node_context = $curr_context;
    }
  }

  // No, this has no node context, so we should leave.
  if (!isset($node_context)) {
    return;
  }
  else {
    if (variable_get('node_auto_fieldtoc_enable_' . $node_context->data->type, '0') == '0') {
      // This content type is not enabled for node auto field toc.
      return;
    }
  }

  // Get the fields for this node type.
  $instances = _field_invoke_get_instances(
    'node',
    $node_context->data->type,
    array(
      'default' => TRUE,
      'deleted' => FALSE,
    )
  );

  // Now examine the instances array, for each field that has a label displayed
  // in the specified view mode, create a key of that item's weight from that
  // display mode.  Add the label of that field to the name value of that key
  // and the instance field name to use as a machine ready in page link name.
  $in_page_links = array();

  if (count($instances)) {
    // We've checked to see if there are fields. Now let's see if the selected
    // view mode has been configured.
    if (isset($conf['view_mode'])) {
      // $conf array is set - check to see if view mode exists.
      if (isset($instances[0]['display'][$conf['view_mode']])) {
        // Ok this is set.
        $view_mode = $conf['view_mode'];
      }
      else {
        // This view mode hasn't been configured for this field instance in
        // this node.
        $view_mode = 'default';
      }
    }
    else {
      // Some how $conf is not set here - set to default.
      $view_mode = 'default';
    }
  }
  else {
    // There are no fields, this isn't going anywhere.
    return;
  }

  foreach ($instances as $instance) {
    if ($instance['display'][$view_mode]['label'] != 'hidden') {
      // Clean up fragment for consistent in page link usage.
      // Get fragment.
      $fragment_type = variable_get('node_auto_fieldtoc_field_fragment_' . $node_context->data->type, 'label');
      $name = $instance['label'];
      $fragment = check_plain(strtolower($instance[$fragment_type]));
      $in_page_links[$instance['display'][$view_mode]['weight']] = array(
        'name' => $name,
        'fragment' => $fragment,
      );
    }
  }

  // Sort by the weight keys which can be adjusted via the UI according to the
  // full display.
  ksort($in_page_links);

  // If we have any links, lets add them to ul list to display in the block.
  if (count($in_page_links)) {
    $list = array();
    if (isset($conf['toc_title'])) {
      $list_title = $conf['toc_title'];
    }
    else {
      $list_title = 'Table of Contents';
    }
    // Get the english name of the content type for the heading.
    $list['title'] = $list_title;
    $list['items'] = array();
    if (isset($conf['list_type'])) {
      $list_type = $conf['list_type'];
    }
    else {
      $list_type = 'ul';
    }
    $list['type'] = $list_type;
    if (isset($conf['toc_class'])) {
      $class = 'autotoc ' . check_plain($conf['toc_class']);
    }
    else {
      $class = 'autotoc';
    }
    $list['attributes'] = array(
      'class' => $class,
    );

    foreach ($in_page_links as $page_link) {
      // Assemble each of these headers into in-page links.
      $current_path = drupal_get_path_alias('node/' . $node_context->data->nid);
      $fragment = $page_link['fragment'];
      $base = base_path();
      $url = url('/' . $current_path, array('fragment' => $fragment));

      $list['items'][] = array(
        'data' => l($page_link['name'], $current_path, array('fragment' => $fragment)),
      );
    }
    // Add a themed ul list to the block content.
    $block->content = theme_item_list($list);
  }

  // Return the block content of the empty block class.
  return $block;
}

/**
 * This returns the edit form.
 */
function node_auto_fieldtoc_pane_edit_form($form, &$form_state) {
  // Set list type.
  $conf = $form_state['conf'];

  $form['toc_title'] = array(
    '#type' => 'textfield',
    '#title' => t('TOC Title'),
    '#description' => t('This text will be used as the table of contents list title. Enter <none> for no title.'),
    '#default_value' => isset($conf['toc_title']) ? $conf['toc_title'] : 'Table of Contents',
  );

  // Choose view mode to use for visibility settings.
  $entity_info = entity_get_info('node');
  $view_array = array_keys($entity_info['view modes']);
  foreach ($view_array as $view_mode => $mode) {
    $view_modes[$mode] = $mode;
  }

  $form['view_mode'] = array(
    '#type' => 'select',
    '#title' => t('View mode for field visibility'),
    '#options' => $view_modes,
    '#description' => t("Choose the view mode that will supply field visibility settings. Hidden fields for a content type's view mode display settings will not appear in the auto toc."),
    '#required' => TRUE,
    '#default_value' => isset($conf['view_mode']) ? $conf['view_mode'] : 'full',
  );

  $list_style = array(
    'ol' => 'Ordered',
    'ul' => 'Unordered',
  );
  // Set list type ul or ol.
  $form['list_type'] = array(
    '#type' => 'select',
    '#title' => t('TOC List Style'),
    '#options' => $list_style,
    '#description' => t("Select the list style you want to use for the TOC."),
    '#required' => TRUE,
    '#default_value' => isset($conf['list_type']) ? $conf['list_type'] : 'Ordered',
  );
  $form['toc_class'] = array(
    '#type' => 'textfield',
    '#title' => t('TOC CSS Custom Class'),
    '#description' => t('This css class will be added to the TOC list.'),
    '#default_value' => isset($conf['toc_class']) ? $conf['toc_class'] : '',
  );

  return $form;
}


/**
 * Pane callback for 'admin info'.
 */
function node_auto_fieldtoc_pane_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = 'Field TOC';
    return $block;
  }
}


/**
 * Submit handler for edit form.
 */
function node_auto_fieldtoc_pane_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
