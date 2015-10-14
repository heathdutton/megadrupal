<?php

/**
 * @file
 * Describe hooks provided by the Data integrity checker module.
 */

/**
 * Provide found data integrity issues.
 *
 * @return mixed|void
 *   Return array with found integriry issues description and section name.
 *   If nothing returned this means that no integriry issues found.
 *
 * @see di_checker.module
 */
function hook_di_checker_info() {
  return array(
    'module_name' => array(
      'title' => t('Section name'),
      'markup' => 'Some integrity issues description',
    ),
  );
}
