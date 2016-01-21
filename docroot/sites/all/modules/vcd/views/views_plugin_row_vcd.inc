<?php
/**
 * @file
 * Contains the VCD Object row style plugin.
 */

/**
 * The VCD row plugin. It extends the DS Object row plugin
 *
 * This plugin adds one extra fields to the DS Object row plugin and allows for
 * contextual based build mode switching.
 *
 * @ingroup views_row_plugins
 */
class views_plugin_row_vcd extends views_plugin_ds_entity_view {
  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    // @todo We load the original DS options but these are not fully functional (yet)
    parent::options_form(&$form, &$form_state);

    $view_mode_options = array();
    $entity_type = $this->view->base_table;
    // In case we're working with users, change the entity type variable.
    if ($entity_type == 'users') $entity_type = 'user';
    $entity_view_modes = ds_entity_view_modes($entity_type);
    foreach ($entity_view_modes as $key => $view_mode) {
      $view_mode_options[$key] = $view_mode['label'];
    }

    // Set an extra fieldset
    $form['context_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Contextual build modes'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['context_fieldset']['contextual'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable context switching.'),
      '#description' => t('Enabling contextual switching only overrides the Default Build Mode for this view.'),
      '#default_value' => ($this->options['contextual']) ? $this->options['contextual'] : FALSE,
    );
    
    // List all enabled contexts that load the vcd_views_context_reaction plugin
    $contexts = context_enabled_contexts();
    $contexts_values = array();
    foreach ($contexts as $name => $context) {
      $reactions = array_keys($context->reactions);
      if (in_array('vcd_views_context_reaction', $reactions)) {
        $conditions = array_keys($context->conditions);
        $form['context_fieldset']['context_' . $name] = array(
          '#type' => 'select',
          '#title' => t('Set buildmode for context %s', array('%s' => $name)),
          '#options' => $view_mode_options,
          '#description' => t('Condtions: %s, Reactions: %e', array('%s' => implode(', ', $conditions), '%e' => implode(' ,', $reactions))),
          '#default_value' => $this->options['context_fieldset']['context_' . $name],
        );
      }
    }
	}

  /**
   * Reset all fieldsets except for changing.
   */
	function options_submit($form, &$form_state) {
    parent::options_submit($form, $form_state);
    foreach ($form_state['values']['row_options']['context_fieldset'] as $context => $view_mode) {
      $form_state['values']['row_options'][$context] = $view_mode;
    }
  }
  
  /**
   * Overrides DS' default rendering of a Views' row object
   */
  function ds_views_row_render_entity($view_mode, &$row) {
    if ($this->options['contextual']) {
      if (($plugin = context_get_plugin('reaction', 'vcd_views_context_reaction')) && $this->options['contextual']) {
        // We need to call all contextual conditions here. Context module invokes them through
        // context_page_alter() which is after Views' preprocess functions have been called.
        // This is not necessarily bad because Context loads them in a static cached object.
        module_invoke_all('context_page_condition');
        foreach (context_active_contexts() as $key => $context) {
          // When multiple contexts are active, only the last active context will set the build mode
          if (in_array('context_' . $key, array_keys($this->options))) {
            $plugin->execute($view_mode, $this->options['context_' . $key]);
          }
        }
      }      
    }

    $content = parent::ds_views_row_render_entity($view_mode, $row);
    return $content;  
  }
}
