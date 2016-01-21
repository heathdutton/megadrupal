<?php

/**
 * @file
 * jsTree example pages
 */

/**
 * Basic jsTree element.
 */
function jstree_example_a($form, &$form_state) {
  $form['jstree'] = array(
    '#type' => 'jstree',
    '#tree_options' => array(
      'plugins' => array('types'),
      'core' => array(
        'data' => array(
          'url' => url('jstree/a/data')
        ),
      ),
    ),
    '#title' => t('jsTree'),
    '#required' => TRUE
  );

  return $form;
}
