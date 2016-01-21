<?php

/**
 * Menu callback.
 */
function dominion_restrict_admin_settings() {
  $form['intro'] = array(
    '#markup' => '<p>'.t('Select which restrictions apply when creating nodes.').'</p>'
  );
  
  /**
   * List all nodetypes
   */
  $options = array();
  $options[0] = t('No restrictions');
  $options[1] = t('Only on own domain');
  $options[2] = t('Only on main domain');
  $functions = dominion_get_all_functions();
  foreach ($functions as $name => $description) {
    $options['own-' . $name] = strip_tags(t('Only on own domain and require %name', array('%name' => $description)));
    $options['any-' . $name] = strip_tags(t('On all domains, but require %name', array('%name' => $description)));
  }

  $types = node_type_get_types();
  foreach ($types as $type => $info){
    $form['dominion_restrict_type_' . $type] = array(
      '#type' => 'select',
      '#title' => $info->name,
      '#options' => $options,
      '#default_value' => variable_get('dominion_restrict_type_'.$type, FALSE),
    );
  }
  
  $form['note'] = array(
    '#markup' => '<p>'.t('Note that users still need <em>&lt;type&gt; Create new content</em> permission to create content. The checkboxes above only adds an extra restriction when creating content. Users with the <em>bypass content access control</em> permission will bypass this restriction.').'</p>'
  );
  
  return system_settings_form($form);
}

