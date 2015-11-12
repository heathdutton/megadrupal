<?php
 
/**
 * Access plugin that provides property based access control.
 */
class views_entity_ac_access_plugin extends views_plugin_access_perm {
  function access($account) {
    $perm = parent::access();
    return views_entity_ac_access($this->options['entity_ac'], $perm, $account);
  }
  
  function get_access_callback() {
    $perm = parent::get_access_callback();
    return array('views_entity_ac_access', array($this->options['entity_ac'], $perm));
  }

  function summary_title() {
    $summary_title = parent::summary_title();
    $entity_info['label'] = '';
    if (isset($this->options['entity_ac'])) {
      $entity_option_array = explode('::', $this->options['entity_ac']);
      if (count($entity_option_array) > 1) {
        list($type, $name) = $entity_option_array;
      } else {
        $type = $entity_option_array[0];
      }
      $entity_info = entity_get_info($type);
      if (!empty($name)) {
        return t($entity_info['bundles'][$name]['label']) . ' - ' . $summary_title;
      }
    }
    return t($entity_info['label']) . ' - ' . $summary_title;
  }


  function option_definition() {
    $options = parent::option_definition();
    $options['entity_ac'] = array('default' => 'node');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Select the target entity type.
    $entity_type_options = array();
    
    foreach (entity_get_info() as $entity_type => $entity_info) {
      $entity_type_options[$entity_type][$entity_type] = $entity_info['label'];
      foreach($entity_info['bundles'] as $bundle_system_name => $bundle) {
        $entity_type_options[$entity_type][$entity_type .'::'. $bundle_system_name] = $bundle['label'];
      }
    }

    //ksort($entity_type_options);

    $form['entity_ac'] = array(
      '#type' => 'select',
      '#options' => $entity_type_options,
      '#title' => t('Entity Types'),
      '#default_value' => $this->options['entity_ac'],
      '#description' => t('Only users with the selected entity type will be able to access this display. Note that users with "access all views" can see any view, regardless of other permissions.'),
    );
  }
}

