<?php

$plugin = array(
  'title' => t('Contextual Forms'),
  'description' => t('Provide form builders as content.'),
  'content types' => 'contextual_forms_forms_content_type_content_types',
  'edit form' => 'contextual_forms_forms_content_type_edit_form',
  'render callback' => 'contextual_forms_forms_content_type_render',
  'admin title' => 'contextual_forms_forms_content_type_admin_title',
  'admin info' => 'contextual_forms_forms_content_type_admin_info',
);

/**
 * 'Content types' callback.
 */
function contextual_forms_forms_content_type_content_types($plugin) {
  return contextual_forms_content_type_info();
}

/**
 * 'Render callback' callback.
 * 
 *  Passes arguments to the form builder, possibly includes a file, returns
 *  fully built Drupal form.
 */
function contextual_forms_forms_content_type_render($subtype, $conf, $args, $context) {
  $info = contextual_forms_info();
  $form_info = $info[$subtype];
  $form_id = $form_info['function'];
  $form_state['build_info']['args'] = array();

  // Add any required contexts as arguments
  if (isset($context) && is_array($context)) {
    foreach ($context as $object) {
      // @fixme by filtering out null values, we are not honoring the placement
      // of arguments to the form builder's parameter list. This will become a
      // problem when for example, arg1 is NULL, but arg2 has a value. Arg2
      // will be mistakenly provided to parameter 1.
      // 
      // I'm only doing this because passing NULL values to
      // $form_state['build_info']['args'] will somehow get converted to blank
      // string values, and therefore replacing the usual default values in the
      // function signiture.
      if (!is_null($object->data)) {
        $form_state['build_info']['args'][] = $object->data;
      }
    }
  }

  // Ensure we apply any file includes to the form builder.
  if (isset($form_info['include']) && is_array($form_info['include'])) {
    $file = $form_info['include'] + array(
      'module' => $form_info['module'],
      'type' => 'inc',
    );
    if (isset($file['name'])) {
      form_load_include($form_state, $file['type'], $file['module'], $file['name']);
    }
  }
  $content = new stdClass();
  $content->title = $form_info['title'];
  $content->content = drupal_build_form($form_id, $form_state);  
  return $content;
}

/**
 * 'Edit form' callback.
 * Empty form so we can have the default override title and configure contexts.
 */
function contextual_forms_forms_content_type_edit_form($form, &$form_state) {
  return $form;
}

/**
 * Derives information ctools content-type plugin requires from
 * contextual_forms_info()
 */
function contextual_forms_content_type_info() {
  $types = array();
  foreach (contextual_forms_info() as $name => $info) {
    $types[$name] = array(
      'title' => $info['title'],
      'description' => $info['description'],
      'category' => $info['category'],
    ) + _contextual_forms_content_type_defaults();
    
    if (isset($info['contexts']) && is_array($info['contexts'])) {
      foreach ($info['contexts'] as $id => $context) {
        
        // Support the traditional Ctools way of adding contexts.
        if (is_object($context) && is_a($context, 'ctools_context_required')) {
          $types[$name]['required context'][] = $context;
        }
        
        // Derive the desired context.
        elseif (isset($context['keyword'])) {
          $required = isset($context['required']) ? $context['required'] : TRUE;
          $context_class = ($required) ? 'ctools_context_required' : 'ctools_context_optional';
          $title = isset($context['title']) ? $context['title'] : t('Context');
          if (isset($context['restrictions']) && is_array($context['restrictions'])) {
            $types[$name]['required context'][] = new $context_class($title, $context['keyword'], $context['restrictions']);
          }
          else {
            $types[$name]['required context'][] = new $context_class($title, $context['keyword']);
          }
        }
      }
    }
    
    // Set any contexts for arguments
    // @deprecated use 'contexts' key in hook_contextual_forms_info() instead.
    elseif (isset($info['arguments']) && is_array($info['arguments'])) {
      foreach ($info['arguments'] as $keyword => $context_info) {

        // simple argument will just use the default 'required' context.
        if (is_string($context_info)) {
          $types[$name]['required context'][] = new ctools_context_required($context_info, $keyword);
          continue;
        }

        // Derive the ctools context class to use as the 'required context'.
        else if(is_array($context_info) && isset($context_info['title']) && isset($context_info['type'])) {
          $class = 'ctools_context_' . $context_info['type'];
          $types[$name]['required context'][] = new $class($context_info['title'], $keyword);
          continue;
        }

        // If you're here something went wrong.
        watchdog('contextual_forms', 'Looks like you have some unexpected data in !keyword', array('!keyword' => $keyword, WATCHDOG_WARNING));
      }
    }
  }
  return $types;
}

/**
 * 'Admin title' callback.
 */
function contextual_forms_forms_content_type_admin_title($subtype, $conf, $pane_context) {
  $info = contextual_forms_info();
  $prefix = t('Contextual Forms') . ': ';

  // return the form's human-readable name...
  if (isset($info[$subtype]) && isset($info[$subtype]['title'])) {
    return $prefix . $info[$subtype]['title'];
  }

  // ... otherwise just return the machine-name
  return $prefix . $subtype;
}

/**
 * 'Admin info' callback.
 */
function contextual_forms_forms_content_type_admin_info($subtype, $conf, $context) {
  $info = contextual_forms_info();

  // return the form's human-readable name...
  if (isset($info[$subtype]) && isset($info[$subtype]['description'])) {
    $output = new stdClass();
    $output->title = t('Form Information');
    $output->content = $info[$subtype]['description'];
    return $output;
  }
}

/**
 * Provide defaults to sub-types.
 */
function _contextual_forms_content_type_defaults() {
  return array(
    'icon' => 'forms.png',
  );
}
