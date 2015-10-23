<?php
/**
 * @file
 * Administration pages for the Adminer module.
 */

/**
 * Returns settings form.
 */
function adminer_settings_form() {
  $adminers = adminer_get_all_php();
  if (empty($adminers)) {
    $form['adminer_php'] = array(
      '#type' => 'item',
      '#title' => t('Adminer version'),
      '#markup' => t('Not installed') . ' (' . l(t('Download'), 'http://www.adminer.org/latest.php', array('attributes' => array('target' => '_blank'))) . ')',
      '#description' => t('Please download <strong>adminer-x.x.x.php</strong> to <strong>!adminer_dir</strong>.', array('!adminer_dir' => drupal_get_path('module', 'adminer') . '/adminer/')),
    );
  }
  else {
    $form['adminer_php'] = array(
      '#type' => 'select',
      '#title' => t('Adminer version'),
      '#default_value' => adminer_get_php(),
      '#options' => $adminers,
    );
  }

  $styles = adminer_get_all_css();
  if (empty($styles)) {
    $form['adminer_css'] = array(
      '#type' => 'item',
      '#title' => t('Adminer style'),
      '#markup' => t('Not installed') . ' (' . l(t('Download'), 'http://www.adminer.org/#extras', array('attributes' => array('target' => '_blank'))) . ')',
      '#description' => t('Please download styles to <strong>!styles_dir</strong> (you can rename css files).', array('!styles_dir' => drupal_get_path('module', 'adminer') . '/adminer/styles/')),
    );
  }
  else {
    $form['adminer_css'] = array(
      '#type' => 'select',
      '#title' => t('Adminer style'),
      '#default_value' => adminer_get_css(),
      '#options' => $styles,
    );
  }

  $header = array(
    'plugin' => t('Plugin'),
    'description' => t('Description'),
    'required' => t('Required'),
    'installed' => t('Installed'),
  );
  $rows = array();
  $all_plugins = adminer_get_all_plugins();
  $plugins = adminer_get_plugins();
  foreach ($all_plugins as $name => $plugin) {
    $rows[$name] = array(
      'plugin' => '<strong>' . $name . '</strong>',
      'description' => check_plain($plugin['description']),
      'required' => (isset($plugin['required']) && $plugin['required']) ? t('Required') : t('Optional'),
      'installed' => t('Installed'),
    );
    if (!isset($plugins[$name])) {
      $rows[$name]['installed'] = t('No');
      if (isset($plugin['url'])) {
        $rows[$name]['installed'] .= ' (' . l(t('Download'), $plugin['url'], array('attributes' => array('target' => '_blank'))) . ')';
      }
    }
  }
  $form['adminer_plugins'] = array(
    '#type' => 'item',
    '#title' => t('Plugins'),
    '#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
    )),
    '#description' => t('Please download plugins to <strong>!plugins_dir</strong>.', array('!plugins_dir' => drupal_get_path('module', 'adminer') . '/adminer/plugins/')),
  );

  return system_settings_form($form);
}
