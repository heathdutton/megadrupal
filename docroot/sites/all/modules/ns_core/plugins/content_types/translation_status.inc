<?php
/**
 * @file
 * A plugin for showing translation status
 */
if (module_exists('entity_translation')) {
  $plugin = array(
    'title' => t('Translation status'),
    'single' => TRUE,
    'description' => t('Outputs a message about the current translation status.'),
    'required context' => new ctools_context_required(t('Node'), 'node'),
    'category' => t('Node'),
    'defaults' => array(),
  );
}

function ns_core_translation_status_content_type_render($subtype, $conf, $panel_args, $context) {
  global $language_content;
  $node = $context->data;
  $block = new stdClass();
  $handler = entity_translation_get_handler('node', $node);
  if (!empty($handler)) {
    $translations = $handler->getTranslations()->data;
    if (!isset($translations[$language_content->language]) || !$translations[$language_content->language]['status']) {
      $block->content = t('No translation exists for this content.');
    }
    elseif ($translations[$language_content->language]['translate']) {
      $block->content = t('The translation for this content needs to be updated.');
    }
    else {
      $block->content = t('Translation for this content is up to date.');
    }
  }
  return $block;
}

function ns_core_translation_status_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" translation status', array('@s' => $context->identifier));
}

function ns_core_translation_status_content_type_edit_form($form, &$form_state) {
  // provide a blank form so we have a place to have context setting.
  return $form;
}

/**
 * Submit handler for the custom type settings form.
 */
function ns_core_translation_status_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

