<?php

/**
 * @file
 * Descriptions on how to use APIs for the External Help module.
 */

/**
 * Defines the list of external help pages for a module.
 *
 * This function should be placed in mymodule.externalhelp.inc. Each url can
 * then be fetched by calling externalhelp_url('mymodule', 'topic'), and a full
 * list of all pages can be fetched with externalhelp_list('mymodule').
 *
 * @return
 *   An associative array containing all external help pages. The keys are the
 *   topics used when calling help pages later on, and the values are arrays
 *   with two entries: label (translated and human-readable) and url.
 */
function hook_externalhelp() {
  $topics = array(
    'overview' => array(
      'label' => t('Overview of My Module'),
      'url' => 'http://drupal.org/my-module-overview',
    ),
    'configuration_details' => array(
      'label' => t('Details about My Module configuration'),
      'url' => 'http://drupal.org/my-module-config',
    ),
  );

  return $topics;
}

/**
 * Example implementation of hook_help, utilizing External Help.
 */
function mymodule_help($path, $arg) {
  if ($path == 'admin/help#mymodule') {
    $output['header'] = array(
      '#markup' => t('The help for My Module is available on drupal.org – please use the links below to learn more. Feel free to help improving the online documentation!'),
    );
    $output['links'] = array(
      '#markup' => externalhelp_list('mymodule'),
    );
    return render($output);
  }
}
