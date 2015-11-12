<?php
/**
 *
 */
class flatfish_config extends ctools_export_ui {
  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);
    // Register migrations for each enabled configs
    foreach ($form_state['object']->items as $name => $config) {
      if ((isset($config->disabled) && FALSE == $config->disabled) || !isset($config->disabled)) {
        // TODO rewrite this, is component necessary?
        if ('Flatfish' == $config->machine_name) {
          $class = 'MediaMigration';
          $component = 'media';
        }
        else {
          $class = 'NodeMigration';
          $component = 'node';
        }
        $args = array('machine_name' => $config->machine_name, 'component' => $component);
        Migration::registerMigration($class, strtolower($config->machine_name . '_' . $component), $args);
      }
    }
  }
}
