<?php
/**
 * @file
 * Content plugin which renders VK share button.
 */

$plugin = array(
    'single'       => TRUE,
    'title'        => t('VK share button'),
    'description'  => t('Adds plugin with VK share button'),
    'category'     => t('VK'),
    'required context' => new ctools_context_required(t('Node'), 'node'),
  );

/**
 * Implementation of MODULENAME_FILENAME_content_type_render($subtype, $conf, $panel_args, &$context)
 * This is the ctools render callback.
 */
function vk_share_panels_vk_share_content_type_render($subtype, $conf, $context = NULL) {
  $block = new stdClass();

  $button = _render_vk_share_button($context['argument_nid_1']->data);
  $block->content = $button['title'];
  return $block;
}