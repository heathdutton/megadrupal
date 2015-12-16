<?php
/**
 * @file
 * Theme functions and preprocess hook implementations for Tealium templates
 */

use Drupal\tealium\TealiumUtagData;
use Drupal\tealium\Data\TealiumJqueryEventBinding;

/**
 * Implements template_preprocess_HOOK().
 * @param array $variables
 *  Template variables array keyed by variable name. The following keys are expected:
 *  - 'account' <string>
 *  - 'profile' <string>
 *  - 'environment' <string>
 *  - 'data_variables' <TealiumUtagData|null>
 *      Variables to be set in the Tealium universal data object.
 *  - 'view_variables' <TealiumUtagData|null>
 *      Variables to be sent to Tealium as a 'view' event on page load.
 *  - 'link_variables' <TealiumUtagData|null>
 *      Variables to be sent to Tealium as a 'link' event on page load.
 *  - 'bind_variables' <TealiumJqueryEventBinding[]>
 *      Variables to be sent to Tealium as a tracking event, when a DOM element event fires.
 * @param string $hook
 *
 * @link http://wiki.tealiumiq.com/index.php/Data_Object
 * @link http://wiki.tealiumiq.com/index.php/AJAX/Flash
 *
 * @ingroup themeable
 */
function template_preprocess_tealium(&$variables, $hook) {
  if (FALSE === is_array($variables)
      || FALSE === array_key_exists('utag_async', $variables)
      || FALSE === array_key_exists('data_variables', $variables)
      || FALSE === array_key_exists('view_variables', $variables)
      || FALSE === array_key_exists('link_variables', $variables)
      || FALSE === array_key_exists('bind_variables', $variables)
  ) {
    return;
  }

  $utag_async = $variables['utag_async'];
  if ($utag_async) {
    $variables['utag_async'] = 'true';
  }
  else {
    $variables['utag_async'] = 'false';
  }

  $data_variables = $variables['data_variables'];
  $view_variables = $variables['view_variables'];
  $link_variables = $variables['link_variables'];

  $variables['data_variables_js'] = '';
  if ($data_variables instanceof TealiumUtagData) {
    $variables['data_variables_js'] = $data_variables->getJavascriptVariables();
  }

  $variables['view_variables_js'] = '';
  if ($view_variables instanceof TealiumUtagData) {
    $variables['view_variables_js'] = $view_variables->getJavascriptVariables();
  }

  $variables['link_variables_js'] = '';
  if ($link_variables instanceof TealiumUtagData) {
    $variables['link_variables_js'] = $link_variables->getJavascriptVariables();
  }

  $bind_variables = array();
  foreach ($variables['bind_variables'] as $bind_definition) {
    if ($bind_definition instanceof TealiumJqueryEventBinding) {
      $bind_variables[] = $bind_definition->getJqueryCodeToAttachBindings();
    }
  }

  $variables['bind_variables_js'] = implode(PHP_EOL, $bind_variables);
}
