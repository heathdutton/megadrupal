<?php

/**
 * @file
 * Administration functions for the relatedlinks module.
 */

/**
 * Menu Callback: relatedlinks module settings form.
 */
function _relatedlinks_settings_form() {
  $types = variable_get('relatedlinks_types', _relatedlinks_get_type_defaults());

  $form['relatedlinks_types'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Link Types'),
    '#theme' => 'relatedlinks_types_table',
    '#description' => t('<p>These are the controls for specific link types.
      <ul>
        <li>Link type: The type of link.</li>
        <li>Enabled: Should links of this type be displayed?</li>
        <li>Block: Should links of this type be displayed in a separate block or
added to the previous related links block? Newly enabled blocks will also need to
be activated on the <a href="!block">blocks page</a>.</li>
        <li>Title: An optional title for the block.</li>
        <li>Max: The maximum number of links to be displayed.</li>
        <li>Weight: Display order priority for links. Heavier types sink to the
bottom.</li>
      </ul>
    </p>', array('!block' => url('admin/structure/block'))),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  foreach ($types as $type_name => $type_values) {
    $form['relatedlinks_types'][$type_name] = array('#weight' => $type_values['weight']);
    $form['relatedlinks_types'][$type_name]['info'] = array('#markup' => $type_values['name']);

    // The name key is used during block configuration.
    $form['relatedlinks_types'][$type_name]['name'] = array(
      '#type' => 'value',
      '#value' => $type_values['name']
    );
    $form['relatedlinks_types'][$type_name]['enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $type_values['enabled']
    );
    $form['relatedlinks_types'][$type_name]['block'] = array(
      '#type' => 'checkbox',
      '#default_value' => $type_values['block']
    );
    $form['relatedlinks_types'][$type_name]['title'] = array(
      '#type' => 'textfield',
      '#size' => 20,
      '#default_value' => $type_values['title']
    );
    $form['relatedlinks_types'][$type_name]['max'] = array(
      '#type' => 'select',
      '#options' => range(0, 25),
      '#default_value' => $type_values['max']
    );
    $form['relatedlinks_types'][$type_name]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $type_values['weight']
    );
  }

  $form['content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#description' => t('Select the content types to associate with this module.
Related links blocks and forms will only appear for these nodes.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['content_types']['relatedlinks_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#options' => _relatedlinks_node_get_types(),
    '#default_value' => variable_get('relatedlinks_node_types', _relatedlinks_node_get_types(TRUE))
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Process relatedlinks settings form submissions.
 */
function _relatedlinks_settings_form_submit($form, &$form_state) {
  // Sort on 'weight'.
  uasort($form_state['values']['relatedlinks_types'], '_relatedlinks_compare');

  // Store the entire relatedlinks_types table.
  variable_set('relatedlinks_types', $form_state['values']['relatedlinks_types']);

  $node_types = array_filter($form_state['values']['relatedlinks_node_types']);
  variable_set('relatedlinks_node_types', array_keys($node_types));

  // Block table needs to be reset.
  _block_rehash();
  cache_clear_all();

  drupal_set_message(t('Configuration settings saved. If necessary, relatedlinks
blocks can be configured via the <a href="!block-page">block configuration</a> page.', array('!block-page' => url('admin/structure/block'))));
}

/**
 * Menu callback: Discovered links settings form.
 * @todo
 * UI could use a better design. Perhaps the weighting system needs to be
 * reintroduced.
 */
function _relatedlinks_discovered_settings_form() {
  if (!module_exists('taxonomy') && !module_exists('search')) {
    drupal_set_message(t('The taxonomy and/or the search module are prerequisites
for discovered links. Please use the <a href="!modules">modules page</a> to enable them.', array('!modules' => url('admin/modules'))), 'error');
    drupal_goto('admin/config/relatedlinks/configure');
  }
  $form['relatedlinks_discovered'] = array('#tree' => TRUE);
  $form['relatedlinks_discovered']['criteria'] = array(
    '#type' => 'fieldset',
    '#title' => t('Criteria'),
    '#description' => t('Define how discovered links are calculated. The following
criteria are applied to results derived from the taxonomy and/or search module
(if selected) on a best effort basis.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  $discovered = variable_get('relatedlinks_discovered', _relatedlinks_get_discovered_defaults());
  $criteria = array(
    'taxonomy' => t('Results based on taxonomy terms (if enabled).'),
    'search' => t('Results from the search module (if enabled).'),
    'node' => t('Prefer results of the same content type.'),
    'user' => t('Prefer results from the same author.'),
    'date' => t('Prefer newer results.'),
    'comments' => t('Prefer results with comments.')
  );
  $form['relatedlinks_discovered']['criteria']['ranking'] = array(
    '#type' => 'checkboxes',
    '#parents' => array('relatedlinks_discovered', 'ranking'),
    '#options' => $criteria,
    '#default_value' => $discovered['ranking']
  );

  if (module_exists('taxonomy')) {
    $voptions = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $vid => $vname) {
      $voptions[$vid] = check_plain($vname->name);
    }
    $form['relatedlinks_discovered']['taxonomy_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy options'),
      '#description' => t('Restrict results to the following vocabularies.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['relatedlinks_discovered']['taxonomy_options']['vocabularies'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Vocabularies'),
      '#parents' => array('relatedlinks_discovered', 'vocabularies'),
      '#options' => $voptions,
      '#default_value' => isset($discovered['vocabularies']) ? $discovered['vocabularies'] : array_keys($vocabularies)
    );
  }

  $form['relatedlinks_discovered']['content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#description' => t('Restrict results to the following content types.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['relatedlinks_discovered']['content_types']['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#parents' => array('relatedlinks_discovered', 'node_types'),
    '#options' => _relatedlinks_node_get_types(),
    '#default_value' => isset($discovered['node_types']) ? $discovered['node_types'] : _relatedlinks_node_get_types(TRUE)
  );

  $form['relatedlinks_discovered']['search_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search options'),
    '#description' => t('Keywords can be specified for each associated node that
can be used to aid in the calculation of results using the search module.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['relatedlinks_discovered']['search_options']['keywords'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a search keywords field.'),
    '#description' => t('The textfield will appear on node forms. Provided keywords
will be used to discover related content and will generally provide better results.
Advanced search queries can also be entered as specfied by the search modules.'),
    '#parents' => array('relatedlinks_discovered', 'keywords'),
    '#default_value' => $discovered['keywords']
  );
  $form['relatedlinks_discovered']['cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron'),
    '#description' => t('Due to the dynamic nature of discovered links, it is recommended
that they be periodically purged and recalculated.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['relatedlinks_discovered']['cron']['refresh'] = array(
    '#type' => 'select',
    '#title' => t('Automatically update discovered links older than'),
    '#description' => t('This feature requires <a href="!cron">cron</a> to be enabled.', array('!cron' => url('admin/reports/status'))),
    '#parents' => array('relatedlinks_discovered', 'cron'),
    '#options' => array(
      1 => t('One day'),
      3 => t('Three days'),
      7 => t('One week'),
      28 => t('Four weeks'),
      0 => t('Never update automatically')
    ),
    '#default_value' => $discovered['cron']
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Validate discovered links settings form submissions.
 */
function _relatedlinks_discovered_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['relatedlinks_discovered']['ranking']['taxonomy'] === $form_state['values']['relatedlinks_discovered']['ranking']['search'] &&
    $form_state['values']['relatedlinks_discovered']['ranking']['taxonomy'] === 0) {

    $error = t('The discovered links feature requires either taxonomy or search results to be enabled.');
    form_set_error('relatedlinks_discovered][criteria][ranking', $error);
  }
  if ($form_state['values']['relatedlinks_discovered']['ranking']['taxonomy'] === 'taxonomy' && !module_exists('taxonomy')) {
    form_set_error('relatedlinks_discovered][criteria][ranking', t('The taxonomy module needs to be enabled for taxonomy results to be calculated.'));
  }
  if ($form_state['values']['relatedlinks_discovered']['ranking']['search'] === 'search' && !module_exists('search')) {
    form_set_error('relatedlinks_discovered][criteria][ranking', t('The search module needs to be enabled for taxonomy results to be calculated.'));
  }
}

/**
 * Process discovered links settings form submissions.
 */
function _relatedlinks_discovered_settings_form_submit($form, &$form_state) {
  $indices = array('ranking', 'vocabularies', 'node_types');
  foreach ($indices as $index) {
    if (isset($form_state['values']['relatedlinks_discovered'][$index])) {
      $form_state['values']['relatedlinks_discovered'][$index] = array_filter($form_state['values']['relatedlinks_discovered'][$index]);
      $form_state['values']['relatedlinks_discovered'][$index] = array_keys($form_state['values']['relatedlinks_discovered'][$index]);
    }
  }

  variable_set('relatedlinks_discovered', $form_state['values']['relatedlinks_discovered']);
}

/**
 * Theme the related links configuration table.
 *
 * @param $form
 *   The configuration form to theme.
 *
 * @return
 *   A themed table of configuration settings.
 *
 * @todo
 *   Get rid of the inline styles.
 */
function theme_relatedlinks_types_table($form) {
  $types_form = $form['relatedlinks_types'];
  $types = element_children($types_form);

  foreach ($types as $type) {
    $rows[] = array(
      drupal_render($types_form[$type]['info']),
      drupal_render($types_form[$type]['enabled']),
      drupal_render($types_form[$type]['block']),
      drupal_render($types_form[$type]['title']),
      drupal_render($types_form[$type]['max']),
      drupal_render($types_form[$type]['weight'])
    );
  }

  $header = array(t('Link type'), t('Enabled'), t('Block'), t('Title'), t('Max'), t('Weight'));

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'relatedlinks_types_form',
      'style' => 'width: 100%'
    )
  ));

  return $output;
}
