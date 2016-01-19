<?php

class chardin_context_reaction_add extends context_reaction {
  function options_form($context) {		
		return array();
  }

  function options_form_submit($values) {
    return $values;
  }

  function execute(&$vars = NULL) {
    $contexts = $this->get_contexts();

    foreach ($contexts as $context) {
      if (!empty($context->reactions[$this->plugin])) {
        $library = libraries_load('chardin');
        $library_loaded = $library && !empty($library['loaded']);

        $js_name = 'chardinjs.min.js';
        $base_path = 'sites/all/libraries/chardin';

        if (!$library_loaded) {
          drupal_set_message(t('Can\'t load Chardin.js library. Please download !url jQuery plugin and extract it to @path, so @js can be found at @full_path. Also please purge version info from chardin file names (both .js and .css)', array(
            '!url' => l(t('Chardin.js'), 'https://github.com/heelhook/chardin.js'),
            '@path' => $base_path,
            '@js' => $js_name,
            '@full_path' => $base_path . '/' . $js_name,
          )), 'error');
          return FALSE;
        }

        drupal_add_js(drupal_get_path('module', 'chardin') . '/js/chardin.context.js', array('type' => 'file', 'scope' => 'footer', 'weight' => JS_DEFAULT));
	
        $chardin_context = $context->reactions[$this->plugin];
				
				//we don't need these to set values;
				unset($chardin_context['elements']['add_element']);
				unset($chardin_context['elements']['remove_element']);
				
        if(empty($chardin_context['elements'])) return FALSE;
	
        $js_settings = array(
          'chardinContext' => $chardin_context,
        );
				
        drupal_add_js($js_settings, 'setting');
      }
    }
  }
}