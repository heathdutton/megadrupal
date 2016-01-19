<?php
/**
 * @file
 */


$plugin = array(
  'title' => 'Slate Render',
);


/**
 *
 */
function slate_slate_render_content_type_content_type($subtype) {
  $types = slate_slate_render_content_type_content_types();
  if (isset($types[$subtype])) {
    return $types[$subtype];
  }
}

/**
 *
 */
function slate_slate_render_content_type_content_types() {
  $types = array();

  foreach (slate_get_slates() as $slate => $slate_info) {
    $plugin = array(
      'title' => $slate_info['name'],
      'category' => 'Slate',
      'edit form' => 'slate_slate_render_content_type_content_type_settings',
    );

    foreach ($slate_info['models'] as $model => $model_info) {
      $model_plugin = slate_load_model($model_info);

      if (empty($model_plugin)) {
        // @todo this is bad!

        continue(2);
      }

      $required_contexts = $model_plugin->requiredContexts();

      if (!empty($required_contexts)) {
        $plugin['required context'] = $required_contexts;
      }
    }

    $types['slate_render:' . $slate] = $plugin;
  }

  return $types;
}

/**
 *
 */
function slate_slate_render_content_type_content_type_settings($form, &$form_state) {
  return $form;
}

/**
 *
 */
function slate_slate_render_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context)) {
    return FALSE;
  }

  list(, $slate) = explode(':', $subtype);

  $slate_info = slate_get_slate($slate);

  $models = array();

  foreach ($slate_info['models'] as $model => $model_type) {
    $models[$model] = array_shift($context)->data;
  }

  $block = new stdClass();
  $block->module = 'slate';
  $block->title = $slate_info['name'];
  $block->content = slate_render($slate, $models);

  return $block;
}

/**
 *
 */
function slate_slate_render_content_type_admin_title($subtype, $conf, $context) {
  list(, $slate) = explode(':', $subtype);

  $slate_info = slate_get_slate($slate);

  return t('Rendered using slate "@name"', array('@name' => $slate_info['name']));
}
