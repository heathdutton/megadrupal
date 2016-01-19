<?php
/**
 * @file
 * An admin page to list all registered validators.
 */

/**
 * The validators page callback.
 */
function magic_forms_validators_page() {
  $output = array();

  $validators = magic_forms_validators();
  $rows = array();
  foreach ($validators as $id => $validator) {
    $rows[] = array(
      $id,
      $validator['title'],
      $validator['description'],
      empty($validator['types']) ? t('All types') : implode(', ', $validator['types']),
    );
  }

  $output['table'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('ID'),
      t('Title'),
      t('Description'),
      t('Field types'),
    ),
    '#rows' => $rows,
    '#empty' => t('There are no validators vailable.'),
  );

  return $output;
}
