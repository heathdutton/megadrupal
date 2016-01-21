<?php

/**
 * @file
 * Part of the "Better Form Errors" module that will be included for
 * displaying help.
 */

/**
 * Implemens hook_help().
 *
 * https://api.drupal.org/
 *  api/drupal/modules!system!system.api.php/function/hook_help/7
 */
function better_form_errors_help($path, $arg) {
  $module_info = _better_form_errors_module_info();
  $readme_menu_item = 'admin/' . $module_info['MODULE_NAME'] . '/readme';
  switch ($path) {

    case 'admin/help#' . $module_info['MODULE_NAME']:
      $link_module = l(
        $module_info['MODULE_FULLNAME'],
        $module_info['MODULE_HOMEPAGE'],
        array(
          'attributes' => array(
            'target' => '_blank',
            'title' => t('Homepage') . ' ' . t('of module') . ' "' . $module_info['MODULE_FULLNAME'] . '"',
          ),
        )
      );
      $link_author = l(
        $module_info['AUTHOR_NAME'],
        $module_info['AUTHOR_HOMEPAGE'],
        array(
          'attributes' => array(
            'target' => '_blank',
            'title' => t('Author') . ' ' . t('of module') . ' "' . $module_info['MODULE_FULLNAME'] . '"',
          ),
        )
      );
      $image_demo = theme(
        'image',
        array(
          'path' => $module_info['MODULE_URL'] . 'img/' . $module_info['MODULE_NAME'] . '.demo.png',
          'width' => '680',
          'height' => '210',
          'alt' => t('Drupal form validation error message example'),
          'title' => t('Demonstration') . ' ' . t('of module') . ' "' . $module_info['MODULE_FULLNAME'] . '"',
          'attributes' => array(
            'class' => 'img-demo',
          ),
        )
      );
      $link_readme = l(
        $module_info['MODULE_README'],
        $readme_menu_item,
        array(
          'attributes' => array(
            'title' => $module_info['MODULE_README'] . ' ' . t('of module') . ' "' . $module_info['MODULE_FULLNAME'] . '"',
          ),
        )
      );

      $placeholders = array(
        '!link_module' => $link_module,
        '!version_module' => $module_info['MODULE_VERSION'],
        '!link_author' => $link_author,
        '!link_readme' => $link_readme,
      );

      $return = "\n" . '<p>' . t(
        "Welcome to the Help page of Drupal's !link_module module version !version_module, developed by !link_author.",
        $placeholders
      ) . '</p>';
      $return .= '<p>' . $image_demo . '</p>';
      $return .= '<p>' . t(
        "Complete documentation is available in the module's !link_readme file.",
        $placeholders
      ) . '</p>';
      if ($module_info['TEMP_PATH']) {
        $return .= '<p>Content of error messages is written into directory <code>';
        $return .= $module_info['TEMP_PATH'] . '</code></p>';
      }
      drupal_add_css($module_info['MODULE_URL'] . 'css/' . $module_info['MODULE_NAME'] . '.help.css', array(
        'media' => 'screen, print',
        )
      );
      return $return;

    case $readme_menu_item:
      $path_readme = $module_info['MODULE_PATH'] . $module_info['MODULE_README'];
      if (is_readable($path_readme)) {
        $contents_readme = file_get_contents($path_readme);
        if (!drupal_strlen($contents_readme)) {
          $contents_readme = t('Error') . ': ' . t('Getting content of file failed!');
        }
      }
      else {
        $contents_readme = t('Error') . ': ' . t('File is not readable!');
      }
      drupal_add_css($module_info['MODULE_URL'] . 'css/' . $module_info['MODULE_NAME'] . '.help.css', array(
        'media' => 'screen, print',
        )
      );
      $return = "\n" . '<p><pre class="pre-readme">' . check_plain($contents_readme) . '</pre></p>';
      return $return;
  }
}
