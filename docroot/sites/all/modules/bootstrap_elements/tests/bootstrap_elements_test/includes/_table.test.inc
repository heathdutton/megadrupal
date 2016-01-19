<?php
/**
 * @file
 * Table tests.
 */

/**
 * Helper function to generate table FAPI elements.
 */
function _boostrap_elements_table(&$form) {

  $form['table'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tables'),
    '#description' => t('Example description'),
  );

  $form['table']['default'] = array(
    '#theme' => 'table',
    '#header' => array('Heading 1', 'Heading 2', 'Heading 3'),
    '#rows' => array(
      array(
        array('data' => 'Cell 1', 'class' => 'col_class_name'),
        'Cell 2',
        'Cell 3',
      ),
    ),
    '#empty' => t('Your table is empty'),
  );

}
