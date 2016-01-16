<?php

/**
 * @file
 *   Administration functions for the Node Style module.
 */

/**
 * Menu callback: Display a table of available schemes.
 */
function _node_style_settings_form() {
  $defaults = variable_get('node_style', array('defaults' => array()));
  $defaults = $defaults['defaults'];

  $types = node_type_get_names();
  $schemes = array(0 => t('--None--')) + _node_style_schemes_get();
  $form['defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node type default associations'),
    '#description' => t('Choose the default style to associate with each node type.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  foreach ($types as $type => $name) {
    $form['defaults'][$type] = array(
      '#type' => 'select',
      '#title' => t('%name:', array('%name' => $name)),
      '#options' => $schemes,
      '#default_value' => isset($defaults[$type]) ? $defaults[$type] : 0
    );
  }

  $form['insecure_attachments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Insecure attachment uploads allowed?'),
    '#description' => t('Drupal, in the interests of <strong>security</strong>, adds a <em>.txt</em> suffix to <em>.js</em> file uploads. If JavaScript file attachments are being used by any node style, then this setting will need to be enabled in order to override this security feature.'),
    '#default_value' => variable_get('allow_insecure_uploads', 0)
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Process relatedlinks settings form submissions.
 */
function _node_style_settings_form_submit($form, &$form_state) {
  variable_set('allow_insecure_uploads', $form_state['values']['insecure_attachments']);

  $node_style = array('defaults' => $form_state['values']['defaults']);
  variable_set('node_style', $node_style);

  drupal_set_message(t('Configuration settings saved.'));
}

/**
 * Menu callback: Display a table of available schemes.
 */
function _node_style_schemes() {
  // Not using _node_style_schemes_get() due to the COUNT().
  $result = db_query("SELECT s.name, s.sid, COUNT(m.sid) AS count
    FROM {node_style_schemes} s
    LEFT JOIN {node_style_maps} m USING (sid)
    GROUP BY s.sid, s.name
    ORDER BY s.name"
  )->fetchAll();
  $rows = array();
  foreach ($result as $row) {
    $rows[] = array(check_plain($row->name), $row->count, l(t('Edit'), 'admin/structure/node_style/edit/' . $row->sid), l(t('Delete'), 'admin/structure/node-style/delete/' . $row->sid));
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No schemes found.'), 'colspan' => 4));
  }
  $header = array(t('Scheme'), t('Use count'), array('data' => t('Operations'), 'colspan' => 2));

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Menu callback: Add a new scheme.
 */
function _node_style_schemes_add($form, &$form_state, $sid = NULL) {
  $scheme = array();

  if (isset($sid)) {
    $scheme = _node_style_scheme_get($sid);
    if ($scheme) {
      $form['sid'] = array('#type' => 'value', '#value' => $sid);
    }
    else {
      drupal_not_found();
      return;
    }
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Scheme name'),
    '#description' => t('A name to identify this scheme in a node form.'),
    '#default_value' => isset($scheme->name) ? $scheme->name : '',
    '#required' => TRUE
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($scheme->machine_name) ? $scheme->machine_name : NULL,
    '#maxlength' => 255,
    '#machine_name' => array(
      'exists' => '_node_style_scheme_get_by_machine_name'
    )
  );

  $form['node_style'] = array('#type' => 'vertical_tabs');

  $hide_array['html'] = array(
    'head_title'     => t('Title (title bar - only display site name)')
  );
  $hide_array['page'] = array(
    'title'          => t('Title (content)'),
    'site_name'      => t('Site name'),
    'site_slogan'    => t('Site slogan'),
    'logo'           => t('Logo'),
    'main_menu'      => t('Main menu'),
    'secondary_menu' => t('Secondary menu'),
    'breadcrumb'     => t('Breadcrumb'),
    'feed_icons'     => t('Feed icons')
  );
  $hide_array['node'] = array(
    'picture'   => t('User picture'),
    'display_submitted' => t('Submission information'),
    'links'     => t('Node links'),
    'comments'  => t('Comments (& comment form)')
  );

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display'),
    '#description' => t('Standard Drupal display elements can optionally be hidden using the following controls. More complex customisations can also be achieved via implementations of hook_node_style_MACHINE_NAME().'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'node_style'
  );
  foreach ($hide_array as $hook => $values) {
    $form['display'][$hook] = array(
      '#type' => 'checkboxes',
      '#title' => t('Hide the following %hook elements:', array('%hook' => $hook)),
      '#options' => $hide_array[$hook],
      '#default_value' => isset($scheme->variables) ? $scheme->variables['display'][$hook] : array()
    );
  }

  $themes = list_themes();
  $options = array();
  foreach ($themes as $id => $theme) {
    // There is a possibility of discrepancies here if a selected theme is
    // disabled in the theme admin pages after being enabled here.
    if ($theme->status == 1) {
      $options[$id] = $theme->name;
    }
  }
  asort($options);
  $options = array('default' => t('default')) + $options;

  $form['markup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Markup'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'node_style'
  );
  $form['markup']['classes_body'] = array(
    '#type' => 'textfield',
    '#title' => t('&lt;BODY&gt; classes'),
    '#description' => t('Inject classes within the <em>BODY</em> tag for node pages styled with this scheme. For e.g., <em>style-foo style-bar</em>'),
    '#default_value' => isset($scheme->variables) ? $scheme->variables['classes_body'] : ''
  );
  $form['markup']['classes_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Node classes'),
    '#description' => t('Inject these classes within the DIV for the node. For e.g., <em>style-foo style-bar</em>'),
    '#default_value' => isset($scheme->variables) ? $scheme->variables['classes_node'] : ''
  );
  // @todo: These would perhaps be better off as file uploads? Make the whole
  // shebang an entity?
  $form['markup']['head'] = array(
    '#type' => 'textarea',
    '#title' => t('Insert mark-up into &lt;head&gt;&lt;head/&gt;'),
    '#description' => t('e.g. Add a stylesheet: &lt;style type="text/css" media="all"&gt;@import "/misc/drupal.css";&lt;/style&gt; or a Javascript file etc.'),
    '#default_value' => isset($scheme->variables) ? $scheme->variables['head'] : '',
    '#rows' => 2,
    '#resizable' => TRUE
  );

  $form['custom_theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'node_style'
  );
  $form['custom_theme']['theme'] = array(
    '#type' => 'select',
    '#title' => t('Select a theme to display for this node'),
    '#description' => t('Select default to use the default theme.'),
    '#options' => $options,
    '#default_value' => isset($scheme->variables) ? $scheme->variables['theme'] : array()
  );

  $options = array(
    'js' => t('JavaScript (.js)'),
    'css' => t('Cascading Stylesheet (.css)')
  );
  $form['attachments'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attachments'),
    '#description' => t('Any attachments of the selected file types will be included during display. All file <em>fields</em> will be checked. There are obvious <strong>security risks</strong> that need to be considered.'),
    '#tree' => FALSE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'node_style'
  );
  $form['attachments']['file_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select the file types to <em>include</em> during display:'),
    '#description' => t('For security reasons, <em>.js</em> files will be saved as <em>.js.txt</em> files. To override this limitation, visit the !settings-page. <em>Caveat Emptor!</em>', array('!settings-page' => l(t('settings page'), 'admin/structure/node-style/settings'))),
    '#options' => $options,
    '#default_value' => isset($scheme->variables) ? $scheme->variables['file_types'] : array()
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save scheme'));
  $form['#submit'][] = 'node_style_scheme_submit';

  return $form;
}

/**
 * Menu callback: Edit a scheme.
 */
function _node_style_schemes_edit($form, &$form_state, $sid = NULL) {
  if (isset($sid)) {
    return _node_style_schemes_add($form, $form_state, $sid);
  }

  drupal_not_found();
}

/**
 * Process a scheme add / edit form submission.
 */
function node_style_scheme_submit($form, &$form_state) {
  $scheme['display'] = array_filter($form_state['values']['display']);
  $scheme['classes_body'] = $form_state['values']['classes_body'];
  $scheme['classes_node'] = $form_state['values']['classes_node'];
  $scheme['head'] = $form_state['values']['head'];
  $scheme['theme'] = $form_state['values']['theme'];
  $scheme['file_types'] = array_filter($form_state['values']['file_types']);

  if (isset($form_state['values']['sid'])) {
    _node_style_scheme_update($form_state['values']['sid'], $form_state['values']['machine_name'], $form_state['values']['name'], $scheme);
    drupal_set_message(t('Style scheme %name updated successfully.', array('%name' => $form_state['values']['name'])));
  }
  else {
    _node_style_scheme_add($form_state['values']['machine_name'], $form_state['values']['name'], $scheme);
    drupal_set_message(t('Style scheme %name added successfully.', array('%name' => $form_state['values']['name'])));
  }

  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/node-style';
}

/**
 * Menu callback: Delete a scheme.
 */
function _node_style_schemes_delete($form, &$form_state, $sid = NULL) {
  if (is_numeric($sid)) {
    $scheme = _node_style_scheme_get($sid);
    if ($scheme) {
      $form['sid'] = array('#type' => 'value', '#value' => $sid);
      $form['scheme_name'] = array('#type' => 'value', '#value' => $scheme->name);
      $form['#redirect'] = 'admin/structure/node-style';

      return confirm_form($form,
        t('Are you sure you want to delete the scheme %title?', array('%title' => $scheme->name)),
        'admin/structure/node-style/',
        t('This action cannot be undone.'),
        t('Delete'),
        t('Cancel')
      );
    }
  }

  drupal_not_found();
}

/**
 * Process a scheme deletion form submission.
 */
function _node_style_schemes_delete_submit($form, &$form_state) {
  db_query("DELETE FROM {node_style_maps} WHERE sid = :sid", array(':sid' => $form_state['values']['sid']));
  db_query("DELETE FROM {node_style_schemes} WHERE sid = :sid", array(':sid' => $form_state['values']['sid']));

  drupal_set_message(t('The scheme %title has been deleted.', array('%title' => $form_state['values']['scheme_name'])));
  watchdog('content', 'The scheme %title has been deleted.', array('%title' => $form_state['values']['scheme_name']));

  $form_state['redirect'] = 'admin/structure/node-style';

  cache_clear_all();
}
