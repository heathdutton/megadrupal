<?php
/**
 * @file
 * Provides tha admin interface and the ouput pane of the module.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'css' => array(drupal_get_path('module', 'entity_list_display') . '/entity_list_display_pane.css'),
  // The title in the panels GUI. For now this module only handles nodes.
  'title' => t('Entity list'),
  'single' => TRUE,
  'category' => array(t('Widgets')),
  'edit form' => 'entity_list_display_pane_content_type_edit_form',
  'render callback' => 'entity_list_display_pane_content_type_render',
  'defaults' => array(
    'override_title' => FALSE,
    'override_title_text' => '',
    'widget' => '',
  ),
  'cache' => TRUE,
  'admin title' => 'entity_list_display_pane_content_type_admin_title',
  'admin info' => 'entity_list_display_pane_content_type_admin_info',
  'required context' => new ctools_context_optional(t('Context'), 'any'),
);

/**
 * Implements hook_admin_info().
 *
 * Callback to provide administrative info.
 *
 * In this case just information the listing.
 *
 * @param array $subtype
 *   The subtype
 * @param array $conf
 *   The configuration
 * @param array $context
 *   The context that are used
 *
 * @return object
 *   A block that contains the output
 *
 */
function entity_list_display_pane_content_type_admin_info($subtype, $conf, $context = NULL) {
  $block = entity_list_display_pane_content_type_render($subtype, $conf, $args = NULL, $context);
  return $block;
}

/**
 * Implements hook_admin_title().
 *
 * Callback to provide administrative titel
 *
 * @param array $subtype
 *   The subtype
 * @param array $conf
 *   The configuration
 * @param array $context
 *   Chosen context, if any
 *
 * @return object
 *   A block that contains the output
 */
function entity_list_display_pane_content_type_admin_title($subtype, $conf, $context = NULL) {
  $output = t('Entity list display');
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $output = filter_xss_admin($conf['override_title_text']);
  }
  return $output;
}


/**
 * Implements hook_content_type_render().
 *
 * @param array $subtype
 *   The subtype
 * @param array $conf
 *   The configuration
 * @param array $args
 *   Arguments
 * @param array $context
 *   Choosen context, if any.
 *
 * @return object
 *   A block that contains the output
 *
 */
function entity_list_display_pane_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = t("List of @content_types", array('@content_types' => $conf['content_type']));
  $block->content = _entity_list_display_output($conf['number_of_posts'], $conf['view_mode'], $conf['content_type'], $context);
  return $block;
}

/**
 * The configuration form.
 *
 * @param array $form
 *   The form used for configuration.
 * @param array $form_state
 *   Form state of the form.
 *
 * @return object
 *   The complete configuration form.
 */
function entity_list_display_pane_content_type_edit_form($form, $form_state) {

  $conf = $form_state['conf'];

  $form['content_type'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#description' => t('Content type to display.'),
    '#options' => array_merge(array('none' => t('None')), node_type_get_names()),
    '#default_value' => !empty($conf['content_type']) ? $conf['content_type'] : array('none' => t('None selected')),
  );
  $form['number_of_posts'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of posts'),
    '#size' => 5,
    '#description' => t('Number of nodes to display.'),
    '#default_value' => !empty($conf['number_of_posts']) ? $conf['number_of_posts'] : '10',
  );
  // Available_view_modes is the array to contain the available view modes.
  $available_view_modes = array();
  if (empty($available_view_modes)) {
    // The function entity_get_info fetches the info of available view modes.
    $entity_info = entity_get_info('node');
    // Get the machine names of the view modes.
    $view_modes_machine_names[] = array_keys($entity_info['view modes']);
    // Get the labels (human readable) of the view modes.
    foreach ($entity_info['view modes'] as $key => $view_mode) {
      $view_modes_labels[] = $view_mode['label'];
    }
  }
  // Combine the machine view mode name with the label.
  $entities_to_display = array_combine($view_modes_machine_names[0], $view_modes_labels);
  // Output the form.
  $form['view_mode'] = array(
    '#type' => 'select',
    '#title' => t('Available view modes'),
    '#description' => t("Choose a view mode for the list."),
    '#options' => $entities_to_display,
    '#default_value' => !empty($conf['view_mode']) ? $conf['view_mode'] : 'full',
  );
  return $form;
}

/**
 * Saves the configuration.
 *
 * @param array $form
 *   The form that are beeing used.
 * @param array $form_state
 *   Form states in the form.
 *
 * @return array
 *   Settings to store
 *
 */
function entity_list_display_pane_content_type_edit_form_submit($form, $form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Outputs the list with the configuration.
 *
 * @param string $number_of_posts
 *   The total number of posts that are gong to be displayed.
 * @param string $view_mode
 *   The view mode that are going to be used.
 * @param string $content_type
 *   The content type that are going to be displayed.
 * @param array $context
 *   Choosen context, if any.
 *
 * @return object
 *   The list that are configured.
 */
function _entity_list_display_output($number_of_posts, $view_mode, $content_type, $context) {
  if (isset($context->type[2])) {
    if (isset($context->data->tid)) {
      $term = $context->data->tid;
    }
  }
  // First we need to check field names http://drupal.org/node/707832.
  // @todo: This should be a function.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', $content_type);
  $entities = $query->execute();
  list($id, $vid, $bundle) = entity_extract_ids('node', reset($entities['node']));
  foreach (field_info_instances('node', $content_type) as $instance) {
    if ($instance['widget']['module'] == 'taxonomy') {
      $field = field_info_field_by_id($instance['field_id']);
      // @todo save this as array.
      $term_field_name = $field['field_name'];
    }
  }

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', $content_type);
  $query->propertyOrderBy('created', 'DESC');
  $query->range(0, $number_of_posts);
  if (isset($term)) {
    $term = (int) $term;
    if (isset($term_field_name)) {
      if (_entity_list_display_field_exists($term_field_name, $term, $content_type) == 1) {
        $checked_term_field_name[] = $term_field_name;
      }
      foreach ($checked_term_field_name as $field_name) {
        $query->fieldCondition($field_name, 'tid', $term);
      }

    }
  }

  $entities = $query->execute();
  if (isset($entities['node'])) {
    $nodes = entity_load('node', array_keys($entities['node']));
    $output_list = node_view_multiple($nodes, $view_mode);
    $output = drupal_render($output_list);
    return $output;
  }
  else {
    return;
  }
}

/**
 * Check if a field exists.
 *
 * @param string $term_field_name
 *   The field name.
 * @param string $term
 *   The term to check for.
 * @param string $content_type
 *   The content type to check for.
 *
 * @return string
 *   Returns 0 or 1 if the field exists.
 */
function _entity_list_display_field_exists($term_field_name, $term, $content_type) {
  // Use EntityFieldQuery to look for line items referencing this product and do
  // not allow the delete to occur if one exists.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', $content_type);
  $query->fieldCondition($term_field_name, 'tid', $term);
  $query->count();
  return $query->execute() > 0 ? 1 : 0;
}
