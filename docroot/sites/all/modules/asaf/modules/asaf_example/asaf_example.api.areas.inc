<?php

function asaf_example_api_areas($form, &$form_state) {
  $form = array(
    '#theme' => 'asaf_examples_areas',
  );

  $form['up'] = array(
    '#markup' => l('← Go to the examples list', 'examples/asaf_example'),
  );

  $form['time'] = array(
    '#type' => 'item',
    '#title' => t('Form'),
    '#markup' => 'Server time: ' . date('H:i:s'),
  );


  $form['area1'] = array(
    '#type' => 'container',
    '#asaf_area' => 'area1',
  );

  $form['area1']['time'] = array(
    '#type' => 'item',
    '#title' => 'Area #1',
    '#markup' => 'Server time: ' . date('H:i:s'),
  );

  $form['area1']['submit_area1'] = array(
    '#type' => 'submit',
    '#value' => t('Update area #1'),
    '#asaf_target_area' => 'area1',
  );

  $form['area2'] = array(
    '#type' => 'container',
    '#asaf_area' => 'area2',
  );

  $form['area2']['time'] = array(
    '#type' => 'item',
    '#title' => 'Area #2',
    '#markup' => 'Server time: ' . date('H:i:s'),
  );

  $form['area2']['area3'] = array(
    '#type' => 'container',
    '#asaf_area' => 'area3',
  );

  $form['area2']['area3']['time'] = array(
    '#type' => 'item',
    '#title' => 'Area #3',
    '#markup' => 'Server time: ' . date('H:i:s'),
  );


  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit_form'] = array(
    '#type' => 'submit',
    '#value' => t('Update form'),
    '#asaf_target_area' => 'form',
  );
  $form['actions']['submit_area1'] = array(
    '#type' => 'submit',
    '#value' => t('Update area #1'),
    '#asaf_target_area' => 'area1',
  );
  $form['actions']['submit_area2'] = array(
    '#type' => 'submit',
    '#value' => t('Update area #2'),
    '#asaf_target_area' => 'area2',
  );
  $form['actions']['submit_area3'] = array(
    '#type' => 'submit',
    '#value' => t('Update area #3'),
    '#asaf_target_area' => 'area3',
  );

  $form['#submit'] = array('asaf_example_areas_submit');

  asaf_prepare_form($form, $form_state);

  return $form;
}

function asaf_example_areas_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function theme_asaf_examples_areas($variables) {
  $output = '';

  $element = $variables['element'];

  $output .= drupal_render($element['up']);
  $output .= drupal_render($element['time']);
  $output .= '<div class="area1-wrapper">' . drupal_render($element['area1']) . '</div>';
  $output .= '<div class="area2-wrapper">' . drupal_render($element['area2']) . '</div>';
  $output .= drupal_render($element['actions']);

  foreach (element_children($element, TRUE) as $key) {
    $output .= drupal_render($element[$key]);
  }

  return $output;
}