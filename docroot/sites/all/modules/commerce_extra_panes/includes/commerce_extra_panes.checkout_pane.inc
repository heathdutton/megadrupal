<?php

/**
 * @file
 * Checkout pane callback functions.
 */

/**
 * Settings form customizations for the pane.
 */
function commerce_extra_panes_contents_settings_form($checkout_pane) {
  $form = array();
  return $form;
}

/**
 * Checkout pane callback: returns the cart contents View for inclusion in the
 *   checkout form.
 */
function commerce_extra_panes_contents_checkout_form($form, &$form_state, $checkout_pane, $order) {
  global $language;
  $pane_form = array();
  list($pane_type, $entity_type, $entity_id) = explode('__', $checkout_pane['pane_id']);
  $entity = entity_load_single($entity_type, $entity_id);

  if ($entity->status) {
    if (module_exists('translation') && !empty($entity->tnid)) {
      $translations = translation_node_get_translations($entity->tnid);
      // Get the right node given the language.
      if (isset($translations[$language->language])) {
        $entity = entity_load_single($entity_type, $translations[$language->language]->nid);
      }
    }

    $elements = entity_view($entity_type, array($entity), 'checkout_pane', $language->language, TRUE);
    $content = theme('commerce_extra_panes_checkout_form', array('elements' => $elements));

    // Replace tokens in the rendered content.
    $content = token_replace($content, array('node' => $entity, 'commerce-order' => $order), array('language' => $language));

    $pane_form[$checkout_pane['pane_id']]['entity'] = array(
      '#markup' => $content,
      '#weight' => 0,
    );
  }
  return $pane_form;
}

/**
 * Checkout pane callback: returns the cart contents review data for the
 *   Review checkout pane.
 */
function commerce_extra_panes_contents_review($form, $form_state, $checkout_pane, $order) {
  global $language;
  list($pane_type, $entity_type, $entity_id) = explode('__', $checkout_pane['pane_id']);
  $entity = entity_load_single($entity_type, $entity_id);
  if (!$entity->status) {
    return;
  }

  // If the entity is translated, get the translation. Let's asume node by now.
  if (module_exists('translation') && !empty($entity->tnid)) {
    $translations = translation_node_get_translations($entity->tnid);
    // Get the right node given the language.
    if (isset($translations[$language->language])) {
      $entity = entity_load_single($entity_type, $translations[$language->language]->nid);
    }
  }

  // Replace tokens in the rendered content.
  $entity->rendered = entity_view($entity_type, array($entity), 'checkout_pane', $language->language, TRUE);
  $content = theme('commerce_extra_panes_review', array('entity' => $entity));

  return token_replace($content, array('node' => $entity, 'commerce-order' => $order), array('language' => $language));
}
