<?php
/**
 * @file
 * purl_active_paths.admin.inc
 * Purl Active Paths admin settings page callback.
 */

function purl_active_paths_form() {
  $form = array();

  $form['providers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Paths to exclude/include'),
  );

  $form['providers']['purl_active_paths_opt_type'] = array(
    '#type' => 'radios',
    '#title' => t('Opt type'),
    '#description' => t('In include mode, only paths defined in the admin or hook_purl_active_paths will be rewritten by purl in outbound links. In exclude mode, these paths will be excluded from rewrites.'),
    '#options' => array(t('Exclude'), t('Include')),
    '#default_value' => variable_get('purl_active_paths_opt_type', 0),
  );

  $description = t("Enter one path per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
  $form['providers']['purl_active_paths_default_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Default paths'),
    '#default_value' => variable_get('purl_active_paths_default_paths', ''),
    '#description' => $description,
  );

  $form['providers']['providers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional by provider'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  foreach (module_invoke_all('purl_provider') as $id => $provider) {
    $overridden = variable_get("purl_active_paths_paths_{$id}", '');
    $form['providers']['providers']["purl_active_paths_paths_{$id}"] = array(
      '#type' => 'textarea',
      '#title' => t('%name', array('%name' => $provider['name'])),
      '#default_value' => $overridden,
      '#description' => $description,
    );

    if ($overridden) {
      $form['providers']['providers']['#collapsed'] = FALSE;
    }
  }

  return system_settings_form($form);
}
