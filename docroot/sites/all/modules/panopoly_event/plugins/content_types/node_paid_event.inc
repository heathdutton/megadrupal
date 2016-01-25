<?php

$plugin = array(
  'single' => TRUE,
  'category' => t('Form'),
  'icon' => 'icon_node_form.png',
  'title' => t('Node paid event form'),
  'description' => t('Node paid event fields exposed as pane'),
  'required context' => new ctools_context_required(t('Form'), 'node_form'),
  'edit form' => 'panopoly_event_node_paid_event_content_type_edit_form',
  'render callback' => 'panopoly_event_node_paid_event_content_type_render',
);

function panopoly_event_node_paid_event_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('node_form');
  $block->delta = 'node-paid-form';

  if(!isset($context->form)) {
    $block->content = t('Form goes here');
    return $block;
  }

  $form_fields = array('field_paid_event', 'field_registration');
  foreach($form_fields as $field) {
    if(isset($context->form[$field])) {
      $block->content[$field] = $context->form[$field];
      unset($context->form[$field]);
    }
  }

  return $block;
}

function panopoly_event_node_paid_event_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" paid events form', array('@s' => $context->identifier));
}

function panopoly_event_node_paid_event_content_type_edit_form($form, &$form_state) {
  // provide a blank form so we have a place to have context setting.
  return $form;
}