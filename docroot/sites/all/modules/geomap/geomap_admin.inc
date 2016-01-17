<?php
/**
 * @file
 * Geomap Admin functions and API controllers.
 */

/**
 * Admin form
 *
 * @ingroup forms
 */
function geomap_admin_form($form, $form_state) {
  //even if a user has permission to view the information, they need access to update it
  $permission_to_update = user_access('administer geomap');

  $path_to_module = base_path() . drupal_get_path('module', 'geomap');
  $custom = _geomap_render_override_admin_display();

  $altitude_options = array();

  $form = array();

  $group_weight = -99;

  //general information for module
  $form['overrides'] = array(
      '#type' => 'fieldset',
      '#weight' => $group_weight++,
      '#title' => t('File Overrides'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('Place the Geomap .js or .css files in @directory on your server to override the original. These files will not be overwritten when Geomap is updated. Changes to the original file must be added to these files by hand.', array(
          '@directory' => $path_to_module . '/custom',
      )),
  );

  $form['overrides']['list'] = array(
      '#weight' => $group_weight++,
      '#value' => $custom,
  );

  $form['googlemap_domains_keys'] = array(
      '#type' => 'fieldset',
      '#weight' => $group_weight++,
      '#title' => t('Registered Google Map API Keys'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
  );

  $form['googlemap_domains_keys']['set_pairs'] = array(
      '#type' => 'fieldset',
      '#weight' => $group_weight++,
      '#collapsible' => FALSE,
      '#description' => _get_domain_keys(),
  );

  $form['googlemap_domains_keys']['add_pair'] = array(
      '#type' => 'fieldset',
      '#weight' => $group_weight++,
      '#collapsible' => FALSE,
      '#description' => check_plain(t('Enter a new pair here. For local installations, you may enter localhost as a domain and it will be used.') . ' ' . ($permission_to_update ? '' : t('You do not have permission to edit the domain keys.'))),
  );

  if ($permission_to_update) {

    $form['googlemap_domains_keys']['add_pair']['domain'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#title' => t('Domain'),
        '#field_suffix' => '<span id="domain-ok"><img src="' . $path_to_module . '/theme/cancel.png" alt="not ok" title="not ok" /></span>',
        /* Verify that the domain is valid */
        '#ajax' => array(
            'callback' => 'geomap_admin_verify_domain',
            'wrapper' => 'domain-ok',
            'method' => 'replace',
            'effect' => 'fade',
            'event' => 'blur',
        ),
    );

    $form['googlemap_domains_keys']['add_pair']['key'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#title' => t('Google Maps API Key'),
    );

    $form['googlemap_domains_keys']['add_pair']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Add Domain Key'),
    );

    // For each node type, the following may be set
    // Icon, Shadow Icon, Transparent Icon, offset x, offset y, altitude (if present)
    $node_types = array_merge(array('default' => 'default'), node_type_get_names());

    $form['node_settings'] = array(
        '#type' => 'fieldset',
        '#weight' => $group_weight++,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t('Icon Settings Per Node Type.'),
        '#description' => t('Leaving any value blank will load the default value for Google Maps. See @link  for information about icon settings.', array('@link' => l(t('Google Maps Reference'), 'http://code.google.com/intl/en-US/apis/maps/documentation/javascript/v2/reference.html', array(
            'fragment' => 'GIcon',
            'attributes' => array(
                'target' => '_blank',
            ),
        )))),
        '#tree' => TRUE,
    );
    
    foreach ($node_types as $machine_name => $human_readable_name) {
      $geomap_icon_array[$machine_name] = array();

      //load the DB table as well. If there are huge numbers of node types, this query will overload
      $geomap_icon_table = db_select('geomap_nodetype_icon', 'gni')->fields('gni')->condition('type', $machine_name)->execute();
      if ($geomap_icon_table->rowCount()) {
        $geomap_icon_array[$machine_name] = $geomap_icon_table->fetchObject();
      }
    }

    foreach ($node_types as $k => $name) {
      $form['node_settings'][$k] = array(
          '#type' => 'fieldset',
          '#weight' => $group_weight++,
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#title' => t('@name settings', array(
              '@name' => $name,
          )),
      );

      $form['node_settings'][$k]['is_new'] = array(
          '#type' => 'value',
          '#weight' => $group_weight++,
          '#value' => (isset($geomap_icon_array[$k]->icon) && strlen($geomap_icon_array[$k]->icon) ? FALSE : TRUE),
      );

      $form['node_settings'][$k]['icon'] = array(
          '#title' => t('Icon'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#description' => 'http://',
          '#default_value' => (isset($geomap_icon_array[$k]->icon) && strlen($geomap_icon_array[$k]->icon) ? $geomap_icon_array[$k]->icon : ''),
      );

      $form['node_settings'][$k]['shadow'] = array(
          '#title' => t('Shadow Icon'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#description' => 'http://',
          '#default_value' => (isset($geomap_icon_array[$k]->icon) && strlen($geomap_icon_array[$k]->shadow) ? $geomap_icon_array[$k]->shadow : ''),
      );

      $form['node_settings'][$k]['transparent'] = array(
          '#title' => t('Transparent Icon'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#description' => 'http://',
          '#default_value' => (isset($geomap_icon_array[$k]->icon) && strlen($geomap_icon_array[$k]->transparent) ? $geomap_icon_array[$k]->transparent : ''),
      );

      $form['node_settings'][$k]['iconwidth'] = array(
          '#title' => t('Icon width'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#default_value' => (int) (isset($geomap_icon_array[$k]->iconwidth) ? $geomap_icon_array[$k]->iconwidth : 0),
      );

      $form['node_settings'][$k]['iconheight'] = array(
          '#title' => t('Icon height'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#default_value' => (int) (isset($geomap_icon_array[$k]->iconheight) ? $geomap_icon_array[$k]->iconheight : 0),
      );

      $form['node_settings'][$k]['iconanchor_x'] = array(
          '#title' => t('Icon Anchor X'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#default_value' => (int) (isset($geomap_icon_array[$k]->iconanchor_x) ? $geomap_icon_array[$k]->iconanchor_x : 0),
      );

      $form['node_settings'][$k]['iconanchor_y'] = array(
          '#title' => t('Icon Anchor Y'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#default_value' => (int) (isset($geomap_icon_array[$k]->iconanchor_y) ? $geomap_icon_array[$k]->iconanchor_y : 0),
      );

      $form['node_settings'][$k]['iconaltitude_start'] = array(
          '#title' => t('Altitude Start'),
          '#description' => t('13 >= altitude_start >= altitude_stop >= 0. If both are set to 0, they will be displayed at every altitude.'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#default_value' => isset($geomap_icon_array[$k]->iconaltitude_start) ? (int) $geomap_icon_array[$k]->iconaltitude_start : 13,
      );

      $form['node_settings'][$k]['iconaltitude_stop'] = array(
          '#title' => t('Altitude Stop'),
          '#description' => t('13 >= altitude_start >= altitude_stop >= 0. If both are set to 0, they will be displayed at every altitude.'),
          '#type' => 'textfield',
          '#weight' => $group_weight++,
          '#default_value' => (int) (isset($geomap_icon_array[$k]->iconaltitude_stop) ? $geomap_icon_array[$k]->iconaltitude_stop : 0),
      );
    }

    if (module_exists('taxonomy')) {
      $form['taxonomy_settings'] = array(
          '#type' => 'fieldset',
          '#weight' => $group_weight++,
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#title' => t('Icon Settings Per Taxonomy Term.'),
          '#description' => t('These settings will override the node type settings. If two terms are chosen for one node, then the taxomony weight system will determine the load order. Leaving any value blank will load the default value for Google Maps. See @link for information about icon settings.', array('@link' => l(t('Google Maps Reference'), 'http://code.google.com/intl/en-US/apis/maps/documentation/javascript/v2/reference.html', array(
              'fragment' => 'GIcon',
              'attributes' => array(
                  'target' => '_blank',
              ),
          )))),
          '#tree' => TRUE,
      );
      //In order to be able to edit this section, the user must have the right to administer taxonomy terms
      if (user_access('administer taxonomy')) {
        //because multiple tables have the same field names, we must then specify the alias
        $query = db_select('taxonomy_term_data', 'ttd');
        $query->innerJoin('taxonomy_vocabulary', 'tv', 'ttd.vid = tv.vid');
        $query->leftJoin('geomap_taxonomy_icon', 'gti', 'gti.tid = ttd.tid');
        $query->fields('ttd', array('tid', 'vid'));
        $query->fields('gti');
        $query->addField('ttd', 'name', 'term');
        $query->addField('tv', 'name', 'vocabulary');
        $query->orderBy('tv.weight');
        $query->orderBy('ttd.weight');
        $query->orderBy('tv.name');
        $query->orderBy('ttd.name');
        $resultset = $query->execute();

        if ($resultset->rowCount()) {
          foreach ($resultset as $result) {
            $form['taxonomy_settings'][$result->tid] = array(
                '#type' => 'fieldset',
                '#weight' => $group_weight++,
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
                '#title' => t('@vocabulary :: @term settings', array(
                    '@vocabulary' => strtoupper($result->vocabulary),
                    '@term' => strtoupper($result->term),
                )),
            );

            $form['taxonomy_settings'][$result->tid]['is_new'] = array(
                '#type' => 'value',
                '#value' => (strlen($result->icon) ? FALSE : TRUE),
            );

            $form['taxonomy_settings'][$result->tid]['icon'] = array(
                '#title' => t('Icon'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#description' => 'http://',
                '#default_value' => (strlen($result->icon) ? $result->icon : ''),
            );

            $form['taxonomy_settings'][$result->tid]['shadow'] = array(
                '#title' => t('Shadow Icon'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#description' => 'http://',
                '#default_value' => (strlen($result->shadow) ? $result->shadow : ''),
            );

            $form['taxonomy_settings'][$result->tid]['transparent'] = array(
                '#title' => t('Transparent Icon'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#description' => 'http://',
                '#default_value' => (strlen($result->transparent) ? $result->transparent : ''),
            );

            $form['taxonomy_settings'][$result->tid]['iconwidth'] = array(
                '#title' => t('Icon width'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#default_value' => (strlen($result->iconwidth) ? $result->iconwidth : ''),
            );

            $form['taxonomy_settings'][$result->tid]['iconheight'] = array(
                '#title' => t('Icon height'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#default_value' => (strlen($result->iconheight) ? $result->iconheight : ''),
            );

            $form['taxonomy_settings'][$result->tid]['iconanchor_x'] = array(
                '#title' => t('Icon Anchor X'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#default_value' => (strlen($result->iconanchor_x) ? $result->iconanchor_x : ''),
            );

            $form['taxonomy_settings'][$result->tid]['iconanchor_y'] = array(
                '#title' => t('Icon Anchor Y'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#default_value' => (strlen($result->iconanchor_y) ? $result->iconanchor_y : ''),
            );

            $form['taxonomy_settings'][$result->tid]['iconaltitude_start'] = array(
                '#title' => t('Altitude Start'),
                '#description' => t('13 >= altitude_start >= altitude_stop >= 0. If both are set to 0, they will be displayed at every altitude.'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#default_value' => (int) (strlen($result->iconaltitude_start) ? $result->iconaltitude_start : 13),
            );

            $form['taxonomy_settings'][$result->tid]['iconaltitude_stop'] = array(
                '#title' => t('Altitude Stop'),
                '#description' => t('13 >= altitude_start >= altitude_stop >= 0. If both are set to 0, they will be displayed at every altitude.'),
                '#type' => 'textfield',
                '#weight' => $group_weight++,
                '#default_value' => (int) (strlen($result->iconaltitude_stop) ? $result->iconaltitude_stop : 0),
            );
          }
        }
      }
      else {
        $form['taxonomy_settings'] = array(
            '#value' => t('You must have Taxonomy administrative rights to alter this section.'),
        );
      }
    }

    $form['#validate'] = array(
        "geomap_admin_form_validate",
    );
    $form['#submit'] = array(
        "geomap_admin_form_submit",
    );
  }

  return system_settings_form($form);
}

function _get_domain_keys() {
  $table = array();
  $table['rows'] = array();
  $table['headers'] = array(
      'Domain',
      'Keys',
      '',
  );

  $path_to_module = drupal_get_path('module', 'geomap');

  $query = db_select('geomap_googlemap_api_keys', 'ggak');
  $query->fields('ggak');
  $query->orderBy('domain');
  $resultset = $query->execute();

  if ($resultset->rowCount()) {
    foreach ($resultset as $result) {
      $text = t('delete @domain key', array(
          '@domain' => $result->domain,
      ));
      $table['rows'][] = array(
          $result->domain,
          $result->google_map_key,
          l(theme('image', array(
              'path' => $path_to_module . '/theme/cancel.png',
              'width' => $text,
              'height' => $text,
          )), 'geomap/admin/delete_key/' . $result->domain, array(
              'html' => TRUE,
          )),
      );
    }
  }

  if (!count($table['rows'])) {
    $table['rows'][] = array(
        array(
            'data' => t('Enter the first pair below'),
            'colspan' => 3,
        ),
    );
  }

  return theme('table', $table);
}

/**
 * Callback to verify if the domain entered on the admin form is correctly formatted
 * @todo
 * @see http://drupal.org/node/1354
 */
function geomap_admin_verify_domain() {
  $path_to_module = drupal_get_path('module', 'geomap');

  $domain = $_POST['domain'];
  $domain0 = '0' . $domain;
  if (strpos($domain0, 'www')) {
    $message = 'Do not include www';
    return theme('image', array('path' => $path_to_module . '/theme/cancel.png', 'width' => $message, 'height' => $message)) . ' ' . $message;
  }
  elseif (strpos($domain0, 'http')) {
    $message = 'Do not include http';
    return theme('image', array('path' => $path_to_module . '/theme/cancel.png', 'width' => $message, 'height' => $message)) . ' ' . $message;
  }

  //determine if the domain is already set
  $query = db_select('geomap_googlemap_api_keys', 'ggak');
  $query->fields('ggak');
  $query->condition('domain', $domain);
  $resultset = $query->execute();

  if ($resultset->rowCount()) {
    $message = "The domain has already been set. Delete it to reset it's key";
    return theme('image', array('path' => $path_to_module . '/theme/cancel.png', 'width' => $message, 'height' => $message)) . ' ' . $message;
  }

  $message = 'ok';
  return theme('image', array('path' => $path_to_module . '/theme/accept.png', 'width' => $message, 'height' => $message));
}

/**
 * Callback: add a domain key pair to the table
 * @todo
 * @see http://drupal.org/node/1354
 */
function geomap_admin_add_domain_key() {
  $path_to_module = base_path() . drupal_get_path('module', 'geomap');
  return theme('image', $path_to_module . '/theme/accept.png', 'ok', 'ok');
}

/**
 * Callback validate the form
 * @todo
 * @see http://drupal.org/node/1354
 */
function geomap_admin_form_validate($form, & $form_state) {
  switch ($form_state['values']['op']) {
    case 'Add Domain Key':
      //TODO: verify that the key is not already in the system
      //TODO: verify that the key is not 0 length
      //TODO: verify that the domain is properly formatted
      break;
    default:
      //TODO: verify that the altitude start and stop are properly set (13 >= start >= stop >= 0)
      break;
  }
}

/**
 * Callback submit function for the admin form
 * @todo
 * @see http://drupal.org/node/1354
 */
function geomap_admin_form_submit($form, $form_state) {
  //drupal_set_message('<pre>' . print_r($form_state['values'], 1) . '</pre>');
  switch ($form_state['values']['op']) {
    case t('Add Domain Key'):

      //@todo add proper filter_xss calls here
      //add the domain pair entered - validation has already been done that the domain is not already present in the table
      $record = new stdClass();
      $record->domain = $form_state['values']['domain'];
      $record->google_map_key = $form_state['values']['key'];
      $response = drupal_write_record('geomap_googlemap_api_keys', $record);

      switch ($response) {
        case SAVED_NEW:
          watchdog('Geomap', format_string('Google Maps API Key successfully added for domain @domain', array('@domain' => $form_state['values']['domain'])));
          drupal_set_message(t('Google Maps API Key successfully added for domain @domain', array('@domain' => $form_state['values']['domain'])), 'status');
          break;
        case SAVED_UPDATED:
          watchdog('Geomap', format_string('Google Maps API Key successfully updated for domain @domain', array('@domain' => $form_state['values']['domain'])));
          drupal_set_message(t('Google Maps API Key successfully updated for domain @domain', array('@domain' => $form_state['values']['domain'])), 'status');
          break;
        default:
          watchdog('Geomap', format_string('Google Maps API Key failed to update (@domain, @key)', array('@domain' => $form_state['values']['domain'], '@key' => $form_state['values']['key'])), NULL, WATCHDOG_ERROR);
          drupal_set_message(t('Google Maps API Key failed to update (@domain, @key)', array('@domain' => $form_state['values']['domain'], '@key' => $form_state['values']['key'])), 'error');
          break;
      }
      //exit
      break;

    case t('Save configuration'):
    default:
      //default submit button is pressed, add the icon settings

      //Node Settings
      foreach ($form_state['values']['node_settings'] as $nodetype => $icon) {
        if (strlen($icon['icon'])) {
          $record = new stdClass();
          $record->icon = $icon['icon'];
          $record->shadow = $icon['shadow'];
          $record->transparent = $icon['transparent'];
          $record->iconwidth = $icon['iconwidth'];
          $record->iconheight = $icon['iconheight'];
          $record->iconanchor_x = $icon['iconanchor_x'];
          $record->iconanchor_y = $icon['iconanchor_y'];
          $record->iconaltitude_start = 0;
          $record->iconaltitude_stop = 0;
          $record->type = $nodetype;

          $response = drupal_write_record('geomap_nodetype_icon', $record, ($icon['is_new'] ? NULL : array('type')));

          switch ($response) {
            case SAVED_NEW:
              watchdog('Geomap', format_string('Record successfully added for node type: @type', array('@type' => $nodetype)));
              drupal_set_message(t('Record successfully added for node type: @type', array('@type' => $nodetype)), 'status');
              break;
            case SAVED_UPDATED:
              watchdog('Geomap', format_string('Record successfully updated for node type: @type', array('@type' => $nodetype)));
              drupal_set_message(t('Record successfully updated for node type: @type', array('@type' => $nodetype)), 'status');
              break;
            default:
              watchdog('Geomap', format_string('Record failed to update for node type: @type', array('@type' => $nodetype)), NULL, WATCHDOG_ERROR);
              drupal_set_message(t('Record failed to update for node type: @type', array('@type' => $nodetype)), 'error');
              break;
          }
        }
        else {
          //delete all information for this nodetype in the table
          $query = db_delete('geomap_nodetype_icon');
          $query->condition('type', $nodetype);
          $query->execute();
        }
      }

      //Taxonomy Term settings
      foreach ($form_state['values']['taxonomy_settings'] as $tid => $icon) {
        if (strlen($icon['icon'])) {

          $record = new stdClass();
          $record->icon = $icon['icon'];
          $record->shadow = $icon['shadow'];
          $record->transparent = $icon['transparent'];
          $record->iconwidth = $icon['iconwidth'];
          $record->iconheight = $icon['iconheight'];
          $record->iconanchor_x = $icon['iconanchor_x'];
          $record->iconanchor_y = $icon['iconanchor_y'];
          $record->iconaltitude_start = $icon['iconaltitude_start'];
          $record->iconaltitude_stop = $icon['iconaltitude_stop'];
          $record->tid = $tid;

          $response = drupal_write_record('geomap_taxonomy_icon', $record, ($icon['is_new'] ? array() : array('tid')));

          switch ($response) {
            case SAVED_NEW:
              watchdog('Geomap', format_string('Record successfully added for taxonomy term: @term', array('@term' => $tid)));
              drupal_set_message(t('Record successfully added for taxonomy term: @term', array('@term' => $tid)), 'status');
              break;
            case SAVED_UPDATED:
              watchdog('Geomap', format_string('Record successfully updated for taxonomy term: @term', array('@term' => $tid)));
              drupal_set_message(t('Record successfully updated for taxonomy term: @term', array('@term' => $tid)), 'status');
              break;
            default:
              watchdog('Geomap', format_string('Record failed to update for taxonomy term: @term', array('@term' => $tid)), NULL, WATCHDOG_ERROR);
              drupal_set_message(t('Record failed to update for taxonomy term: @term', array('@term' => $tid)), 'error');
              break;
          }

        }
        else {
          //delete all information for this nodetype in the table
          $query = db_delete('geomap_taxonomy_icon');
          $query->condition('tid', $tid);
          $query->execute();
        }
      }
      break;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function geomap_admin_delete_key($domain = '') {
  if (!(strlen($domain))) {
    drupal_set_message(t('Domain not specified. Could not delete key.'));
    drupal_goto('admin/config/geomap');
    return;
  }

  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query("DELETE FROM {geomap_googlemap_api_keys} WHERE domain = '%s'", $domain) */
  $query = db_delete('geomap_googlemap_api_keys');
  $query->condition('domain', $domain);
  if ($query->execute()) {
    drupal_set_message(t('Google Maps API Key deleted successfully for @domain.', array('@domain' => $domain)));
    drupal_goto('admin/config/geomap');
    return;
  }

  drupal_set_message(t('Cannot delete domain key for @domain.', array('@domain' => $domain)));
  drupal_goto('admin/config/geomap');
  return;
}

/**
 * Menu item callback for geomap/admin/hook_geomap
 * Allows other modules to return a view
 *
 * List possible hooks here:
 *
 * @author Jeremy Chinquist
 * @param $op
 *   operations: {get_view, get_node, get_nodes}
 * @param $nids
 *   required
 * @param $latitude_a
 *   required
 * @param $longitutde_a
 *   required
 * @param $latitude_b
 *   optional
 * @param $longitude_b
 *   optional
 * @return json
 *   array of HTML arrays
 *
 */
function geomap_admin_hook_geomap($op = 'update_view', $nids = array(), $latitude_a = NULL, $longitude_a = NULL, $latitude_b = NULL, $longitude_b = NULL) {
  //this callback simply invokes the hook and returns.
  //functions impleming hook_geomap must NOT return a JSON encoded object. That will be encoded here.

  //if any are not sent, return an error
  if (!count($nids) || !is_array($nids) || $latitude_a === NULL || $longitude_a === NULL) {
    return drupal_json_encode(array(
        'error' => 'Missing arguments',
    ));
  }

  $return = module_invoke_all('geomap', filter_xss($op), $nids, (float) $latitude_a, (float) $longitude_a, (float) $latitude_b, (float) $longitude_b);

  //$return is already an array of "hopefully" html & microformat elements
  return drupal_json_encode($return);
}

/**
 * helper function to render the administration override display
 */
function _geomap_render_override_admin_display() {
  $path_to_module = drupal_get_path('module', 'geomap');

  $alt_text = array(
      'overridden' => t('File overridden'),
      'notoverridden' => t('File not overridden'),
  );

  //custom is currently a list. Check that we have overridden the following
  $check_for_files = array(
      'geomap.css',
      'Clusterer2.js',
      'jquery.googlemaps.js',
  );
  $custom = _geomap_get_customisations($path_to_module . '/custom');

  $counter = 1;
  $table = array(
      'header' => array(
          t('File name'),
          t('customized'),
      ),
      'rows' => array(),
  );

  //@todo: use array_keys and in_array so that no error is thrown
  foreach ($check_for_files as $file) {
    if (isset($custom[$file])) {
      $table['rows'][] = array(
          $counter++ . '. ' . $file,
          theme('image', array('path' => $path_to_module . '/theme/accept.png', 'width' => $alt_text['overridden'], 'height' => $alt_text['overridden'])) . ' ' . $alt_text['overridden'],
      );
    }
    else {
      $table['rows'][] = array(
          $counter++ . '. ' . $file,
          theme('image', array('path' => $path_to_module . '/theme/cancel.png', 'width' => $alt_text['notoverridden'], 'height' => $alt_text['notoverridden'])) . ' ' . $alt_text['notoverridden'],
      );
    }
  }

  return theme('table', $table);
}
