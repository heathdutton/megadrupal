<?php

/**
 * @file
 * Contains the 'entity_gallery' argument validator plugin.
 */

/**
 * Validate whether an argument is an acceptable entity gallery.
 */
class views_plugin_argument_validate_entity_gallery extends views_plugin_argument_validate {
  function option_definition() {
    $options = parent::option_definition();
    $options['types'] = array('default' => array());
    $options['access'] = array('default' => FALSE, 'bool' => TRUE);
    $options['access_op'] = array('default' => 'view');
    $options['egid_type'] = array('default' => 'egid');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $types = entity_gallery_type_get_types();
    $options = array();
    foreach ($types as $type => $info) {
      $options[$type] = check_plain(t($info->name));
    }

    $form['types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Gallery types'),
      '#options' => $options,
      '#default_value' => $this->options['types'],
      '#description' => t('Choose one or more gallery types to validate with.'),
    );

    $form['access'] = array(
      '#type' => 'checkbox',
      '#title' => t('Validate user has access to the content'),
      '#default_value' => $this->options['access'],
    );
    $form['access_op'] = array(
      '#type' => 'radios',
      '#title' => t('Access operation to check'),
      '#options' => array('view' => t('View'), 'update' => t('Edit'), 'delete' => t('Delete')),
      '#default_value' => $this->options['access_op'],
      '#dependency' => array('edit-options-validate-options-entity-gallery-access' => array(TRUE)),
    );

    $form['egid_type'] = array(
      '#type' => 'select',
      '#title' => t('Filter value format'),
      '#options' => array(
        'egid' => t('Entity gallery ID'),
        'egids' => t('Entity gallery IDs separated by , or +'),
      ),
      '#default_value' => $this->options['egid_type'],
    );
  }

  function options_submit(&$form, &$form_state, &$options = array()) {
    // filter trash out of the options so we don't store giant unnecessary arrays
    $options['types'] = array_filter($options['types']);
  }

  function convert_options(&$options) {
    if (!isset($options['types']) && !empty($this->argument->options['validate_argument_entity_gallery_type'])) {
      $options['types'] = isset($this->argument->options['validate_argument_entity_gallery_type']) ? $this->argument->options['validate_argument_entity_gallery_type'] : array();
      $options['access'] = !empty($this->argument->options['validate_argument_entity_gallery_access']);
      $options['access_op'] = isset($this->argument->options['validate_argument_entity_gallery_access_op']) ? $this->argument->options['validate_argument_entity_gallery_access_op'] : 'view';
      $options['egid_type'] = isset($this->argument->options['validate_argument_egid_type']) ? $this->argument->options['validate_argument_egid_type'] : array();
    }
  }

  function validate_argument($argument) {
    $types = $this->options['types'];

    switch ($this->options['egid_type']) {
      case 'egid':
        if (!is_numeric($argument)) {
          return FALSE;
        }
        $entity_gallery = entity_gallery_load($argument);
        if (!$entity_gallery) {
          return FALSE;
        }

        if (!empty($this->options['access'])) {
          if (!entity_gallery_access($this->options['access_op'], $entity_gallery)) {
            return FALSE;
          }
        }

        // Save the title() handlers some work.
        $this->argument->validated_title = check_plain($entity_gallery->title);

        if (empty($types)) {
          return TRUE;
        }

        return isset($types[$entity_gallery->type]);
      break;
      case 'egids':
        $egids = new stdClass();
        $egids->value = array($argument);
        $egids = views_break_phrase($argument, $egids);
        if ($egids->value == array(-1)) {
          return FALSE;
        }

        $test = drupal_map_assoc($egids->value);
        $titles = array();

        $result = db_query("SELECT * FROM {entity_gallery} WHERE egid IN (:egids)", array(':egids' =>  $egids->value));
        foreach ($result as $entity_gallery) {
          if ($types && empty($types[$entity_gallery->type])) {
            return FALSE;
          }

          if (!empty($this->options['access'])) {
            if (!entity_gallery_access($this->options['access_op'], $entity_gallery)) {
              return FALSE;
            }
          }

          $titles[] = check_plain($entity_gallery->title);
          unset($test[$entity_gallery->egid]);
        }

        $this->argument->validated_title = implode($egids->operator == 'or' ? ' + ' : ', ', $titles);
        // If this is not empty, we did not find a nid.
        return empty($test);
    }
  }
}
