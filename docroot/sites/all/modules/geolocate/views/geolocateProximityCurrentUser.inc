<?php

/**
 * @file
 * Contains geolocateProximityCurrentUser.
 */

class geolocateProximityCurrentUser extends geolocateProximityBase implements geolocateProximityPluginInterface {
  public function option_definition(&$options, $views_plugin) {
    $options['geolocate_proximity_current_user_field'] = array(
      'default' => '',
    );
    $options['geolocate_proximity_current_user_delta'] = array(
      'default' => 0,
    );
  }

  public function options_form(&$form, &$form_state, $views_plugin) {
    $geolocate_fields = _geolocate_get_geolocate_fields();
    $field_options = array();
    foreach ($geolocate_fields as $key => $field) {
      $field_options[$key] = $key;
    }

    $form['geolocate_proximity_current_user_field'] = array(
      '#type' => 'select',
      '#title' => t('Source Field'),
      '#default_value' => $views_plugin->options['geolocate_proximity_current_user_field'],
      '#options' => $field_options,
      '#dependency' => array(
        'edit-options-source' => array('current_user'),
      ),
    );
  }

  public function getSourceValue($views_plugin) {
    global $user;
    $user_object = user_load($user->uid);

    $geofield_name = $views_plugin->options['geolocate_proximity_current_user_field'];
    $delta = $views_plugin->options['geolocate_proximity_current_user_delta'];

    if (!empty($geolocate_name)) {
      $field_data = field_get_items('user', $user_object, $geolocate_name);

      if ($field_data != FALSE) {
        return array(
          'latitude' => $field_data[$delta]['lat'],
          'longitude' => $field_data[$delta]['lon'],
        );
      }
    }

    return FALSE;
  }
}
