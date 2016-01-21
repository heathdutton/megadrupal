<?php

/**
 * Apply image styles to favicon as context reactions.
 */
class context_reaction_derivicon extends context_reaction {
  /**
   * Options form.
   */
  function options_form($context) {
    $form = array();

    $options = array();
    foreach (image_styles() as $image_style) {
      $options[$image_style['name']] = $image_style['name'];
    }

    $form['image_style'] = array(
      '#type' => 'select',
      '#title' => t('Image style'),
      '#options' => $options,
      '#default_value' => isset($context->reactions['derivicon']['image_style']) ? $context->reactions['derivicon']['image_style'] : NULL,
    );

    return $form;
  }

  /**
   * Execute.
   */
  function execute(&$head_elements) {
    global $base_url;

    foreach ($this->get_contexts() as $context) {
      if (isset($context->reactions['derivicon'])) {
        $image_style = image_style_load($context->reactions['derivicon']['image_style']);

        foreach ($head_elements as $key => $head_element) {
          if ('link' == $head_element['#tag'] && isset($head_element['#attributes']['rel']) && 'shortcut icon' == $head_element['#attributes']['rel']) {
            $image_uri = str_replace($base_url . '/', '', $head_element['#attributes']['href']);
            $derivative_uri = image_style_path($image_style['name'], $image_uri);

            $success = file_exists($derivative_uri) || image_style_create_derivative($image_style, $image_uri, $derivative_uri);
            if ($success) {
              $head_elements[$key]['#attributes']['href'] = file_create_url($derivative_uri);
            }
          }
        }
      }
    }
  }
}
