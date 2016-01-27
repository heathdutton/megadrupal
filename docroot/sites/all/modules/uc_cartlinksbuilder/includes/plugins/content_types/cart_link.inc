<?php

$plugin = array(
    'title' => t('Ubercart Cart Link'),
    'render callback' => 'uc_cartlinksbuilder_cart_link_render',
    'edit form' => 'uc_cartlinksbuilder_cart_link_edit_form',
    'category' => t('Node'),
    'no title override' => FALSE,
    'single' => TRUE,
    'required context' => new ctools_context_required(t('Node'), 'node'),
);

function uc_cartlinksbuilder_cart_link_render($subtype, $conf, $args, $context) {
  $office = $context->data;

  $content = new stdClass();

  $cart_link_path = 'cart/add/p' . $context->data->nid . '_q' . $conf['quantity'];

  $link = l($conf['link_label'], $cart_link_path, array('query' => array('destination' => $conf['link_destination'])));

  $content->content = $link;

  return $content;
}

function uc_cartlinksbuilder_cart_link_edit_form($form, $form_state) {
  $conf = $form_state['conf'];

  $form['link_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Link Label'),
      '#default_value' => $conf['link_label'],
  );

  $form['link_destination'] = array(
      '#type' => 'textfield',
      '#title' => t('Link Destination'),
      '#default_value' => $conf['link_destination'],
  );

  $form['quantity'] = array('#type' => 'textfield', '#title' => t('Quantity'), '#default_value' => $conf['quantity']);

  return $form;
}

function uc_cartlinksbuilder_cart_link_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}