<?php
/**
 * @file
 * Administration settings form area for Social Share Privacy module
 */

require_once(drupal_get_path('module', 'socialshareprivacy') . '/socialshareprivacy.install');

 /**
 * General configuration form for controlling the behaviour.
 */
function socialshareprivacy_admin_settings() {
    $form = array();

    $socialshareprivacy_path = _socialshareprivacy_get_path();

    $form['socialshareprivacy_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#default_value' => $socialshareprivacy_path,
      '#description' => t('The location the socialSharePrivacy plugin is installed in. Use a path relative to the Drupal root directory.'),
      '#after_build' => array('_socialshareprivacy_admin_settings_check_plugin_path'),
    );

    $form['socialshareprivacy_services'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Enabled services'),
        '#default_value' => variable_get('socialshareprivacy_services', unserialize(SOCIALSHAREPRIVACY_DEFAULT_VALUE_SERVICES)),
        '#options' => array(
            'facebook' => 'Facebook',
            'twitter' => 'Twitter',
            'gplus' => 'Google+',
        ),
    );

    $form['socialshareprivacy_services_permaoption'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Enabled permanent options'),
        '#description' => t('Enabling this option for a social network means that the user gets an option to enable permanent data transfer to the corresponding network, thus reverting to the non-privacy 1-click sharing workflow.'),
        '#default_value' => variable_get('socialshareprivacy_services_permaoption', unserialize(SOCIALSHAREPRIVACY_DEFAULT_VALUE_SERVICES)),
        '#options' => array(
            'facebook' => 'Facebook',
            'twitter' => 'Twitter',
            'gplus' => 'Google+',
        ),
    );

    $form['socialshareprivacy_facebook_action'] = array(
      '#type' => 'radios',
      '#title' => t('Facebook button action'),
      '#options' => array(
        'recommend' => t('Recommend'),
        'like' => t('Like'),
      ),
      '#default_value' => variable_get('socialshareprivacy_facebook_action', 'recommend'),
    );

    $form['socialshareprivacy_ext_link'] = array(
      '#type' => 'radios',
      '#title' => t('External link (info icon)'),
      '#default_value' => variable_get('socialshareprivacy_ext_link', ''),
      '#options' => array(
        '' => t('Open the link in the current window.'),
        'blank' => t('Open the link in a new window.'),
        'remove' => t('Remove the click event of the link.'),
      ),
    );

    $form['socialshareprivacy_ext_link_target'] = array(
      '#type' => 'textfield',
      '#title' => t('Change default info link'),
      '#description' => t('If you do not want the info icon to link to the original explanatory page at heise.de you can override the target by specifying a URL here.'),
      '#default_value' => variable_get('socialshareprivacy_ext_link_target', 'http://www.heise.de/ct/artikel/2-Klicks-fuer-mehr-Datenschutz-1333879.html'),
    );

    $form['socialshareprivacy_js_minified'] = array(
      '#type' => 'radios',
      '#title' => t('Use minified javascript file'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
      '#default_value' => variable_get('socialshareprivacy_js_minified', 'true'),
    );

    $form['socialshareprivacy_reset_css'] = array(
      '#type' => 'radios',
      '#title' => t('Include reset.css'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
      '#default_value' => variable_get('socialshareprivacy_reset_css', 'false'),
    );

    $form['socialshareprivacy_force_german'] = array(
      '#type' => 'radios',
      '#title' => t('Ignore Drupal\'s translations and use the original German texts from the plugin.'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
      '#default_value' => variable_get('socialshareprivacy_force_german', 'false'),
    );

    $form['socialshareprivacy_neutral_button'] = array(
      '#type' => 'radios',
      '#title' => t('Use language-neutral Facebook button'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
      '#default_value' => variable_get('socialshareprivacy_neutral_button', 'false'),
    );

    $form['socialshareprivacy_cookie_expires'] = array(
      '#type' => 'textfield',
      '#title' => t('Cookie expiration time'),
      '#description' => t('Cookie expiration time in days.'),
      '#default_value' => variable_get('socialshareprivacy_cookie_expires', '365'),
    );

    $form['socialshareprivacy_referrertrack'] = array(
      '#type' => 'textfield',
      '#title' => t('Tracking parameter'),
      '#description' => t('The tracking parameter will be added to the end of the URL which is useful for tracking the referrer.'),
      '#default_value' => variable_get('socialshareprivacy_referrertrack', ''),
    );

    return system_settings_form($form);
}

function socialshareprivacy_admin_fieldmgmt() {

  if (!module_exists('field_ui')) {
    drupal_set_message(t('Enable the Field UI module to adjust the Social Share Privacy field settings, manage node fields or manage the display.'), 'warning');
  }

    $ret[] = array('#markup' => t('Enable/Disable the Socialshareprivacy field for your content types (disabling will delete the data for all nodes of the content type):<br/>'));
    $ret[] = drupal_get_form('socialshareprivacy_form_admin_fieldmgmt');
    return $ret;
}

function socialshareprivacy_form_admin_fieldmgmt($form, &$form_state) {
    $form = array();

    $head = array(
      'contenttype' => array('data' => t('Content types'), 'field' => 'contenttype'),
    );

    $options = array();
    $default_values = array();
    foreach (node_type_get_names() as $name => $value) {
      $options['ssp_fieldmgmt_' . $name] = array(
        'contenttype' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $value,
            '#href' => 'admin/structure/types/manage/' . $name . '/fields',
          ),
        ),
      );
      $is_enabled = 0;
      if (field_info_instance('node', variable_get('socialshareprivacy_field_name'), $name)) {
        $is_enabled = 1;
      }
      $default_values['ssp_fieldmgmt_' . $name] = $is_enabled;
    }

    $form['ssp_fieldmgmt'] = array(
      '#type' => 'tableselect',
      '#header' => $head,
      '#options' => $options,
      '#default_value' => $default_values,
      '#empty' => t('No content available.'),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    return $form;
}

function socialshareprivacy_form_admin_fieldmgmt_submit($form, &$form_state) {
  $form_values =  $form_state['values'];
  $ssp_fieldmgmt = $form_values['ssp_fieldmgmt'];
  $entity = 'node';
  foreach ($ssp_fieldmgmt as $k => $v) {
    if ($v===0) { // this means checked ..
      __remove_custom_field($entity, drupal_substr($k, 14));
    }
    else {
      __add_custom_field($entity, drupal_substr($k, 14));
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

function socialshareprivacy_admin_bulkupdate() {

    $ret[] = array('#markup' => t('Update the Socialshareprivacy boolean for existing nodes:<br/>Nothing to select? !url.<br/>', array('!url' => l(t('Enable a content type'), 'admin/config/user-interface/socialshareprivacy/fieldmgmt'))));
    $ret[] = drupal_get_form('socialshareprivacy_form_admin_bulk');
    return $ret;
}

function socialshareprivacy_form_admin_bulk($form, &$form_state) {
    $form = array();

    $head = array(
      'contenttype' => array('data' => t('Content types'), 'field' => 'contenttype'),
    );

    $options = array();
    $options['none'] = 'none';
    foreach (node_type_get_names() as $name => $value) {
      if (field_info_instance('node', variable_get('socialshareprivacy_field_name'), $name)) {
        $options[$name] = $value;
      }
    }

    $form['ssp_bulkupdate_type'] = array(
      '#type' => 'select',
      '#title' => t('Please choose a content type'),
      '#options' => $options,
      '#default_value' => 'none',
    );

    $options = array();
    $options[LANGUAGE_NONE] = t('Undetermined');
    foreach (language_list() as $obj) {
      $options[$obj->language] = $obj->native;
    }
    $form['ssp_bulkupdate_langs'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Please choose the desired languages'),
      '#options' => $options,
    );

    $form['ssp_bulkupdate_value'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable SSP'),
      '#description' => t('If this box is checked then SSP will be enabled on the chosen nodes, otherwise it will be disabled.'),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );

    return $form;
}

function socialshareprivacy_form_admin_bulk_submit($form, &$form_state) {
  $form_values =  $form_state['values'];
  $contenttype = $form_values['ssp_bulkupdate_type'];
  $valuetoset = $form_values['ssp_bulkupdate_value'];
  if ($valuetoset=='') {
    $valuetoset = '0';
  }
  $lang_choosen = FALSE;
  $langs = $form_values['ssp_bulkupdate_langs'];
  foreach ($langs as $lang => $value) {
    if ($lang==$value) {
      $lang_choosen = TRUE;
    }
  }

  if ($contenttype!='none' && $lang_choosen) {
    $node_type = 'page'; // Machine name of the content type
    $entity = 'node';

    $query = new EntityFieldQuery;
    $result = $query
      ->entityCondition('entity_type', $entity)
      ->propertyCondition('type', $contenttype)
      ->execute();

    if (!empty($result['node'])) {
      $nodes = entity_load('node', array_keys($result['node']));

      foreach ($nodes as $node) {
        foreach ($langs as $lang => $value) {
          if ($lang == $value) {
            $node->field_socialshareprivacy[$lang][0]['value'] = $valuetoset;
          }
        }
        node_save($node);
      }
    }
    drupal_set_message(t('The nodes of type "@contenttype" have been updated.', array('@contenttype' => $contenttype)));
  }
  else {
    if ($contenttype=='none') {
    drupal_set_message(t('Please choose a content type.'), 'error');
    }

    if (!$lang_choosen) {
      drupal_set_message(t('Please choose at least one language.'), 'error');
    }
  }
}
