<?php
// $Id$

/**
 * @file
 * Provides admin forms for the site location_chooser
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function location_chooser_location_form($form, &$form_state) {
  // LOCATION
  foreach (array('city', 'province', 'postal_code') as $key) {
    $location_fields[$key] = array(
      'collect' => 1,
      'default' => '',
    );
  }
  $location_fields['locpick'] = array(
    'collect' => 1,
    'default' => '',
    'weight' => 100,
  );

  $location_settings = array(
    'multiple' => array(
      'min' => 0,
      'max' => 1,
      'add' => 1,
    ),
    'form' => array('fields' => $location_fields, 'weight' => 0, 'collapsible' => 0, 'collapsed' => 0),
  );
  $locations = location_load_locations('location_chooser_default', 'genid');
  $form['default_location'] = location_form($location_settings, $locations);
  $form['default_location'][0]['#title'] = t('Default Location');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function location_chooser_location_form_submit(&$form, $form_state) {
  location_save_locations($form_state['values']['default_location'], array('genid' => 'location_chooser_default'));
  drupal_set_message("The default location has been saved");
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function location_chooser_settings($form, &$form_state) {
  $nodetypes = $sources = node_type_get_names();
  $form['location_chooser_fieldset'] = array(
    '#type' => 'select',
    '#title' => t('Location form display'),
    '#description' => t('The value of this field will affect the display of the location fieldset
                        on nodes where a location_chooser box appears.  The possibilities are:
                        <ul><li>Standard field: display as a fieldset on the node form (D6 style)</li>
                        <li>Vertical tab: display location form in a vertical tab (default)</li>
                        <li>Hidden: hides location form on node entry forms (does not provide extra security for location form elements)</li></ul>'),
    '#options' => array('Standard field', 'Vertical tab', 'Hidden'),
    '#default_value' => variable_get('location_chooser_fieldset', 1),
  );
  $targets = variable_get('location_chooser_targets', array());
  $targetcount = count(array_filter($targets));
  $unsafe_roles = $type_classes = $target_classes = $style_roles = array();
  $unsafe_roles_text = '';
  $always_unsafe_roles = user_roles(FALSE, 'administer nodes') + user_roles(FALSE, 'bypass node access');
  foreach ($nodetypes as $type => $name) {
    if ($roles = user_roles(FALSE, "edit any $type content") + $always_unsafe_roles) {
      foreach ($roles as $role) {
        $unsafe_roles[$role][] = "unsafe-role-$type";
      }
    }
    $style_roles[] = "#location-chooser-warning.$type .unsafe-role-$type";
  }
  $style_text = '<style>#location-chooser-warning p {font-size:90%; line-height:1.2em; font-style:italic;}
    #location-chooser-warning span {display:none; color:red; font-weight:bold; margin-left:1.6em;}' . 
    (!empty($style_roles) ? implode(', ', $style_roles) . '{display:inline-block;}' : '') . '</style>';
  foreach ($unsafe_roles as $role => $types) {
    $type_classes = implode(' ', $types);
    $unsafe_roles_text .= "<span class='role $type_classes'> $role </span>";
  }
  $target_classes = implode(' ', array_filter($targets));
  $form['location_chooser_warning'] = array(
    '#type' => 'markup',
    '#markup' => "$style_text<div id='location-chooser-warning' class='$target_classes'><strong>" . 
      t('Users with the following role(s) can potentially see any SOURCE
 location on the site, regardless of display settings, node access rules or 
 other permissions</strong><br/>
 Roles: ') . $unsafe_roles_text . '<br/><p>' . t('For most use cases of this 
 module, the fact that some roles can bypass permissions to view location 
 choices will be acceptible and desirable; however, if you are adamant that no
 source locations be seen by content editors, you must remove the permissions
 "Administer content" and "Bypass content access control", as well as the "Edit 
 any content" permission for each of the target nodetypes.  You must also be 
 careful with any permissions provided by other modules that might grant the 
 ability to edit target nodes, e.g. node access module permissions.') . 
    '</p></div>',
  );
  $form['targets'] = array(
    '#type' => 'fieldset',
    '#title' => t('Targets') . " ($targetcount)",
    '#description' => t('Node types on which the location chooser form should appear<br/>
 NOTE: behavior may be unreliable if the same node types are targets and sources for location chooser.'),
    '#collapsible' => TRUE,
    '#collapsed' => count(array_filter($targets)),
  );
  $form['targets']['location_chooser_targets'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Targets'),
    '#options' => $nodetypes,
    '#default_value' => $targets,
  );
  $sources = variable_get('location_chooser_node_sources', array());
  $sourcecount = count(array_filter($sources)) + variable_get('location_chooser_user_source', TRUE) + variable_get('location_chooser_default_source', TRUE);
  $form['location_chooser_sources'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sources') . " ($sourcecount)",
    '#description' => t('Types of locations that will populate the location chooser select box<br/>
  <b>NOTE: If you have EVER had "link locations" enabled, and you switch a node type from a target
  to a source, you could mess up all the locations linked to by those nodes.</b>  See the note above.'),
    '#collapsible' => TRUE,
    '#collapsed' => $sourcecount,
  );
  $form['location_chooser_sources']['location_chooser_node_sources'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#options' => $nodetypes,
    '#default_value' => $sources,
  );
  $form['location_chooser_sources']['location_chooser_user_source'] = array(
    '#type' => 'checkbox',
    '#title' => t('User locations'),
    '#default_value' => variable_get('location_chooser_user_source', TRUE),
  );
  $form['location_chooser_sources']['location_chooser_default_source'] = array(
    '#type' => 'checkbox',
    '#title' => t('Site default location'),
    '#default_value' => variable_get('location_chooser_default_source', TRUE),
  );
  $form['location_chooser_author_nodes_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use only author nodes for location choices'),
    '#default_value' => variable_get('location_chooser_author_nodes_only', FALSE),
    '#description' => t('Only source nodes authored by the current user will be
 in the list of location choices.'),
  );
  $form['location_chooser_include_unpublished_nodes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include unpublished nodes in location choices'),
    '#default_value' => variable_get('location_chooser_include_unpublished_nodes', FALSE),
    '#description' => t('<strong>Warning: this option may bypass some security 
 features of Drupal!</strong> If this option is selected, users who are editing 
 location target nodes will be able to see the titles and locations from 
 <strong>unpublished source nodes</strong>, regardless of permissions.'),
  );
  $form['location_chooser_reuse_source_locations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep locations linked if source changes'),
    '#default_value' => variable_get('location_chooser_reuse_source_locations', FALSE),
    '#description' => t('<strong>Warning:</strong> If this option is selected, 
 changing a source location will change all nodes linked to that source.  
 For example, on a site which allows users to post events at their homes, if a 
 user changes his or her home location, all existing events at that location 
 will point to the new location.  Problems may arise if you change which node 
 types are sources and targets after content has been created.
 Therefore, it is suggested that you <strong>only enable this setting after you 
 are satisfied with the setup of the sources and targets above</strong>.'),
  );
  drupal_add_js(drupal_get_path('module', 'location_chooser') . '/js/location_chooser_admin.js');
  $form['array_filter'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );
  return system_settings_form($form);
}
