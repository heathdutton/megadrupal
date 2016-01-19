<?php
// $Id

/**
 * @file
 * Contains the 'ctools_field' markup field handler.
 */

/**
 * Field handler to display custom markup text.
 */
class views_ctools_field_handler_field extends views_handler_field {

  function query() {
    // If this is a global content with no context, rewrite so as to select a null column
    if ($this->table == 'ctools_field_global') {
      $this->field_alias = 'ctools_field_markup_' . $this->position;

      if (!empty($this->options['value']) && $value = $this->options['value']) {
        $this->query->add_field('', "'$value'", $this->field_alias);
      }
    }
    else {
      parent::query();
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['views_ctools_field'] = array('default' => array());
    return $options;
  }

  function options_form(&$form, &$form_state) {
    ctools_include('plugins');
    ctools_include('context');
    ctools_include('content');
    
    parent::options_form($form, $form_state);
    
    $plugin_id = $this->definition['ctools_plugin_name'];
    $plugin = ctools_get_plugins('ctools', 'content_types', $plugin_id);

    if (empty($this->options['views_ctools_field'])) {
      if (!empty($plugin['defaults'])) {
        $this->options['views_ctools_field'] = $defaults;
      }
      else {
        $this->options['views_ctools_field'] = array();
      }
    }
    
    $form['views_ctools_field'] = $this->getContentTypePluginForm($plugin);
  }
  
  function getContentTypePluginForm($plugin) {
    if (!empty($plugin['edit form'])) {
      $edit_form_callback = $plugin['edit form'];
    }
    else {
      $edit_form_callback = $plugin['module'] . '_' . $plugin_id . '_content_type_edit_form';
    }
    
    if (function_exists($edit_form_callback)) {
      $form['views_ctools_field'] = array(
        '#type' => 'fieldset',
        '#title' => t('@field field settings', array('@field' => $plugin['title'])),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
      );
      $sub_form_state = array('conf' => $this->options['views_ctools_field']);
      return $edit_form_callback($form['views_ctools_field'], $sub_form_state);
    }
  }

  function render($values) {
    ctools_include('plugins');
    ctools_include('context');
    ctools_include('content');
    
    $plugin_id = $this->definition['ctools_plugin_name'];
    $plugin = ctools_get_plugins('ctools', 'content_types', $plugin_id);

    $conf = $this->options['views_ctools_field'];
    
    if ($this->definition['type']) {
      // Determine the correct field
      if (!empty($this->relationship)) {
        $entity_id_field = $this->relationship . '_' . $this->definition['real field'];
      }
      else {
        $entity_id_field = $this->definition['real field'];
      }
      
      $entity_type = $this->definition['type'];
      $entity_id = $values->{$entity_id_field};
      $entity = entity_load($this->definition['type'], array($entity_id));
      $entity = reset($entity); 
      
      $entity_context = ctools_context_create($entity_type, $entity);
      $entity_context->identifier = t('This @id', array('@id' => $entity_type));
      $entity_context->keyword = $entity_type;
      
      $context = array($entity_type => $entity_context);
      $conf['context'] = $entity_type;
      
      $ctools_content = ctools_content_render($plugin_id, $plugin_id, $conf, array(), array(), $context);
    }
    else {
      $ctools_content = ctools_content_render($plugin_id, $plugin_id, $conf);
    }

    if (isset($ctools_content->content)) {
      return $ctools_content->content;
    }

    return '';
  }

  function options_submit(&$form, &$form_state) {
    $plugin_id = $this->options['field'];
    $plugin = ctools_get_plugins('ctools', 'content_types', $this->options['field'], $plugin_id);
    
    $newconf = $form_state['values']['options']['views_ctools_field'];
    
    if (!empty($plugin['edit form'])) {
      $submit_form_callback = $plugin['edit form'] . '_submit';
    }
    else {
      $submit_form_callback = $plugin['module'] . '_' . $plugin_id . '_content_type_edit_form_submit';
    }
    
    if (function_exists($submit_form_callback)) {
      $form = $this->getContentTypePluginForm($plugin);
      $state = array('values' => $newconf);
      $submit_form_callback($form, $state);
      $this->options['views_ctools_field'] = $state['conf'];
    }
    else {
      $this->options['views_ctools_field'] = $options_new;
    }
    
    parent::options_submit($form, $form_state);
  }
}
