<?php
/**
 * @file
 * Content type that displays the relcontext context type.
 */

$plugin = array(
  // Used in add content dialogs.
  'title' => t('Contextual Entity'),
  'content_types' => 'contextual_entity_list_entity_list_content_type_pane',
  'single' => TRUE,
  'render callback' => 'contextual_entity_list_entity_list_content_type_pane_render',
  'icon' => 'icon_example.png',
  'description' => t('Contextual entity'),
  'required context' => new ctools_context_required(t('Contextual Entity'), 'contextual_entity_list_entity_list_context_pane'),
  'category' => array(t('Widgets'), -9),
  'edit form' => 'contextual_entity_list_entity_list_content_type_pane_edit_form',

);

/**
 * Render callback: Displays a list of content.
 *
 * @param string $subtype
 *   Description of a parameter for this page.
 *
 * @return object
 *   A render object for a page containing a list of content.
 */
function contextual_entity_list_entity_list_content_type_pane_render($subtype, $conf, $args, $context) {

  $pageconf = isset($conf['config-page-number']) ? $conf['config-page-number'] : 5;
  $entity_result_data = _contextual_entity_list_entity_result_data($context->data->contextdata['entity-type'], $context->data->contextdata['bundle'], $pageconf);
  $output = '';
  if (count($entity_result_data) > 0) {
    foreach ($entity_result_data as $node) {
      $nodeview = node_view($node);
      $nodeview['comment-section'] = comment_node_page_additions($node);
      if (isset($nodeview['comment-section']['comments'])) {
        unset($nodeview['comment-section']['comments']['pager']);
      }
      $output .= drupal_render($nodeview);
    }

    $output .= theme('pager');
  }

  else {
    $output = 'No Records found';
  }

  $block = new stdClass();
  $block->title = '';
  $block->content = $output;
  return $block;
}

/**
 * Configuration callback form for the content type.
 */
function contextual_entity_list_entity_list_content_type_pane_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['config-page-number'] = array(
    '#type' => 'textfield',
    '#title' => t('Config number of items per page'),
    '#size' => 10,
    '#description' => t('The number of entities to be displayed on each page.'),
    '#default_value' => !empty($conf['config-page-number']) ? $conf['config-page-number'] : '',
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit call back for the configuration form.
 */
function contextual_entity_list_entity_list_content_type_pane_edit_form_submit($form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Function for EntityField Queryi using the context value.
 * @TODO Need to work on other entity as well as currently work on node only.
 */
function _contextual_entity_list_entity_result_data($entity_type, $context_bundle, $conf) {

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type)
    ->entityCondition('bundle', $context_bundle);
  if ($entity_type == 'node') {
    $nodeentity = $query->pager($conf)->propertyCondition('status', 1)->execute();
    if (isset($nodeentity['node'])) {
      $result = node_load_multiple(array_keys($nodeentity['node']));
    }
  }

  elseif ($entity_type == 'taxonomy_term') {
    $taxonomyentity = $query->execute();
    if (isset($taxonomyentity['taxonomy_term'])) {
      $termids = array_keys($taxonomyentity['taxonomy_term']);
      $nodes = taxonomy_select_nodes($termids[0], TRUE, $conf);
      if (count($nodes) > 0) {
        $result = node_load_multiple($nodes);
      }
    }
  }
  return $result;
}
