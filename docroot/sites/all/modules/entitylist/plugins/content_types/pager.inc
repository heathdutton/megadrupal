<?php

$plugin = array(
  'title' => t('Pager'),
  'category' => t('Entity list'),
  'description' => t('Displays a pager for a given list.'),
  'required context' => new ctools_context_required(t('List'), 'entitylist'),
  'defaults' => array(),
  'admin title' => 'entitylist_ctools_pager_admin_title',
  'render callback' => 'entitylist_ctools_pager_render',
  'edit form' => 'entitylist_ctools_pager_edit_form',
);

function entitylist_ctools_pager_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module  = 'entitylist';
  $block->content = NULL;

  // Get the list.
  $list = entitylist_list_load_from_context($context);
  $pager = $list->getPager();
  if (!$pager) {
    return $block;
  }
  $list->initializePager();
  $block->content = theme('pager', array('element' => $pager['element']));
  return $block;
}

function entitylist_ctools_pager_admin_title($subtype, $conf, $context) {
  return t('Pager for "@list"', array('@list' => $context->identifier));
}

function entitylist_ctools_pager_edit_form($form, &$form_state) {
  return $form;
}

function entitylist_ctools_pager_edit_form_submit($form, &$form_state) {
}
