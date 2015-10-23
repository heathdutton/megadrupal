<?php
/**
 * @file
 * theme.inc
 * Throw preprocess and theme function in this file.
 */

/**
 * Preprocess variables for Le Gate gate theme.
 */
function template_preprocess_le_gate_gate(&$vars) {
  $instructions = variable_get('le_gate_instructions', array('format' => 'filtered_html', 'value' => 'You know what to do...DO IT!'));
  $vars['content']['instructions'] = array(
    '#type' => 'markup',
    '#markup' => $instructions['value'],
  );
  $type = variable_get('le_gate_type', 'links');
  switch ($type) {
    case 'links':
      $vars['content']['links'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => 'le-gate-links-wrapper',
        ),
      );
      $vars['content']['links']['link1'] = array(
        '#theme' => 'link',
        '#path' => variable_get('le_gate_link_1_path', ''),
        '#text' => variable_get('le_gate_link_1_text', 'Yes'),
        '#options' => array(
          'html' => TRUE,
          'attributes' => array(
            'class' => array('le-gate-link-1-link'),
          ),
        ),
      );
      $vars['content']['links']['link2'] = array(
        '#theme' => 'link',
        '#path' => variable_get('le_gate_link_2_path', ''),
        '#text' => variable_get('le_gate_link_2_text', 'No'),
        '#options' => array(
          'html' => TRUE,
          'attributes' => array(
            'class' => array('le-gate-link-2-link'),
          ),
        ),
      );
      $cookie_link = variable_get('le_gate_links_cookie', 'link1');
      if ($cookie_link == 'link1') {
        $vars['content']['links']['link1']['#options']['attributes']['class'][] = 'le-gate-pass-link';
      }
      else {
        $vars['content']['links']['link2']['#options']['attributes']['class'][] = 'le-gate-pass-link';
      }
      break;
    case 'date':
      $vars['content']['date'] = drupal_get_form('le_gate_date_form');
      break;
  }
}
