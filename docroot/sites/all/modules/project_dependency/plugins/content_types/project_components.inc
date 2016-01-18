<?php
/**
 * @file
 * Provide project components based on the release context we're looking at.
 * TODO: Add an issue to the project issue queue to bring this plugin in.
 */
$plugin = array(
  // No subtypes.
  'single' => TRUE,
  'title' => t('Project components'),
  'description' => t('Modules belonging to the project.'),
  'category' => array(t('Project'), -10),
  'edit form' => 'project_dependency_components_content_type_edit_form',
  'render callback' => 'project_dependency_components_pane_content_type_render',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Run-time rendering of the body of the block (content type)
 */
function project_dependency_components_pane_content_type_render($subtype, $conf, $args, $context = NULL) {
  $block = new stdClass();
  if (isset($context->data->nid) && project_release_node_is_release($context->data)) {
    $node = $context->data;
    $block->title = t('Components');
    $block->content = project_dependency_components($node);
  }
  return $block;
}

/**
 * Provide a form callback so we can set the required context.
 */
function project_dependency_components_content_type_edit_form($form, &$form_state) {
  return $form;
}
