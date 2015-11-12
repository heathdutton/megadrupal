<?php
/**
 * @file includes/go.hooks.form.inc
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function go_form_system_cron_settings_alter(&$form, $form_state) {
  if (!empty($_GET['module'])) {
    if (module_exists($_GET['module'])) {
      timer_start('go_cron_links');
      $function = "{$_GET['module']}_cron";
      $function();
      $time = timer_read('go_cron_links');
      drupal_set_message("Ran <strong>{$function}()</strong> in {$time} ms");
      drupal_goto('admin/config/system/cron');
    }
  }

  foreach (module_implements('cron') as $module) {
    $items[] = l($module, 'admin/config/system/cron', array('query' => array('module' => $module)));
  }

  $form['go_cron_jobs'] = array(
    '#type' => 'markup',
    'links' => array(
      '#theme' => 'item_list',
      '#title' => t('You can run a specific cron job by clicking the following links'),
      '#items' => $items,
    )
  );
}
