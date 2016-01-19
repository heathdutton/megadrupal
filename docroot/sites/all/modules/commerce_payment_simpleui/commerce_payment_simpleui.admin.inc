<?php

/**
 * @file
 * Commerce Payment Simple UI admin page callback.
 */

/**
 * Commerce Payment Simple UI page callback.
 */
function commerce_payment_simpleui_page() {
  RulesPluginUI::$basePath = 'admin/commerce/config/payment-methods';
  $options = array(
    'show plugin' => FALSE,
    'show events' => FALSE,
    'show execution op' => FALSE,
    'show status' => FALSE,
  );

  // Add the table for enabled payment method rules.
  $content['enabled']['title']['#markup'] = '<h3>' . t('Enabled payment method rules') . '</h3>';

  $conditions = array(
    'event' => 'commerce_payment_methods',
    'plugin' => 'reaction rule',
    'active' => TRUE,
  );
  $enabled = commerce_payment_simpleui_get_simple_table($conditions, $options);
  $content['enabled']['rules'] = $enabled;
  $content['enabled']['rules']['#empty'] = t('There are no active payment methods.');

  // Add the table for disabled payment method rules.
  $content['disabled']['title']['#markup'] = '<h3>' . t('Disabled payment method rules') . '</h3>';

  $conditions['active'] = FALSE;
  $disabled = commerce_payment_simpleui_get_simple_table($conditions, $options);
  $content['disabled']['rules'] = $disabled;

  $content['disabled']['rules']['#empty'] = t('There are no disabled payment methods.');

  // Store the function name in the content array to make it easy to alter the
  // contents of this page.
  $content['#page_callback'] = 'commerce_payment_simpleui_page';

  return $content;
}

/**
 * Helper function to generate a simplified table of payment rules.
 *
 * Code adapted/modified from RulesUIController->overviewTable().
 */
function commerce_payment_simpleui_get_simple_table($conditions = array(), $options = array()) {
  $options += array(
    'hide status op' => FALSE,
    'show plugin' => TRUE,
    'show events' => isset($conditions['plugin']) && $conditions['plugin'] == 'reaction rule',
    'show execution op' => !(isset($conditions['plugin']) && $conditions['plugin'] == 'reaction rule'),
  );
  // By default show only configurations owned by rules.
  $conditions += array(
    'owner' => 'rules',
  );

  $entities = entity_load('rules_config', FALSE, $conditions);
  ksort($entities);

  // Prepare some variables used by overviewTableRow().
  $cache = rules_get_cache();

  $rows = array();
  foreach ($entities as $id => $entity) {
    if (user_access('bypass rules access') || $entity->access()) {
      $rows[] = _commerce_payment_simpleui_tablerow($conditions, $id, $entity, $options, $cache);
    }
  }
  // Assemble the right table header.
  $header = array(
    t('Name'),
    t('Event'),
    t('Plugin'),
    t('Status'),
    array('data' => t('Operations')),
  );
  if (!$options['show events']) {
    // Remove the event heading as there is no such column.
    unset($header[1]);
  }
  if (!$options['show plugin']) {
    unset($header[2]);
  }

  if (!$options['show status']) {
    unset($header[3]);
  }
  // Fix the header operation column colspan.
  $num_cols = isset($rows[0]) ? count($rows[0]) : 0;
  if (($addition = $num_cols - count($header)) > 0) {
    $header[4]['colspan'] = $addition + 1;
  }

  $table = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('None.'),
  );
  $table['#attributes']['class'][] = 'rules-overview-table';
  $table['#attached']['css'][] = drupal_get_path('module', 'rules') . '/ui/rules.ui.css';

  return $table;
}

/**
 * Helper function to add table rows.
 *
 * Code adapted/modified from RulesUIController->overviewTableRow().
 */
function _commerce_payment_simpleui_tablerow($conditions, $name, $config, $options, $cache) {
  // Build content includes the label, as well as a short overview including
  // the machine name.
  $row[] = $config->label;

  // Add events if the configs are assigned to events.
  if ($options['show events']) {
    $events = array();
    if ($config instanceof RulesTriggerableInterface) {
      foreach ($config->events() as $event_name) {
        $event_handler = rules_get_event_handler($event_name, $config->getEventSettings($event_name));
        $events[] = $event_handler->summary();
      }
    }
    $row[] = implode(", ", $events);
  }
  if ($options['show plugin']) {
    $plugin = $config->plugin();
    $row[] = isset($cache['plugin_info'][$plugin]['label']) ? $cache['plugin_info'][$plugin]['label'] : $plugin;
  }
  if ($options['show status']) {
    $row[] = array(
      'data' => array(
        '#theme' => 'entity_status',
        '#status' => $config->status,
      ),
    );
  }

  // Loop over the actions to find the first enabled payment method.
  foreach ($config->actions() as $action) {
    // Parse the action name into a payment method ID.
    if (strpos($action->getElementName(), 'commerce_payment_enable_') === 0) {
      $row[] = l(t('configure'), RulesPluginUI::path($name, 'edit', $action));
      break;
    }
  }

  // General link options params array.
  $params = array(
    'attributes' => array('class' => array('action')),
    'query' => drupal_get_destination(),
  );

  if (!$options['hide status op']) {
    // Add either an enable or disable link.
    $text = $config->active ? t('disable') : t('enable');
    $active_class = $config->active ? 'disable' : 'enable';
    $params['attributes']['class'][] = $active_class;
    $path = RulesPluginUI::path($name, $active_class);

    $row[] = $config->hasStatus(ENTITY_FIXED) ? '' : l($text, $path, $params);
  }

  if ($config->hasStatus(ENTITY_OVERRIDDEN)
    && !$config->hasStatus(ENTITY_FIXED)) {

    $params['attributes']['class'][] = 'revert';
    $row[] = l(t('reset'), RulesPluginUI::path($name, 'revert'), $params);
  }
  else {
    $row[] = '';
  }
  return $row;
}
