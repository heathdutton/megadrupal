<?php

/**
 * @file
 * Field handler for graphic representations of boolean fields.
 */

/**
 * A handler to provide graphic displays for booleans.
 *
 * Based on: views_handler_field_boolean
 *
 * @ingroup views_field_handlers
 */
class boolean_icon_handler_field_boolean_graphic extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['not'] = array('definition bool' => 'reverse');
    $options['click_publish'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['not'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reverse'),
      '#description' => t('If checked, true will be displayed as false.'),
      '#default_value' => $this->options['not'],
    );

    $form['click_publish'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add click-publish link'),
      '#description' => t('Icon will become a clickable link, that will be used to publish/unpublish entites.'),
      '#default_value' => $this->options['click_publish'],
    );

    parent::options_form($form, $form_state);
  }

  function render($values) {
    $value = $this->get_value($values);
    if (!empty($this->options['not'])) {
      $value = !$value;
    }

    if (empty($this->options['click_publish'])) {
      $icon = $value ?
            variable_get('boolean_icon_true_ico', drupal_get_path('module', 'boolean_icon') . '/on.png'):
            variable_get('boolean_icon_false_ico', drupal_get_path('module', 'boolean_icon') . '/off.png');
      $title = $value ? t('True') : t('False');

      // Return element.
      return array(
        '#theme' => 'image',
        '#path' => $icon,
        '#title' => $title,
      );
    }
    else {

      // Add the necessary libraries
      drupal_add_library('system', 'drupal.ajax');

      // Get type of entitiy and ID.
      $type = $this->table;
      $id = $values->{$this->view->base_field};

      // If the node is locked don't allow the change of status.
      if ($type == 'node' && module_exists('content_lock') && content_lock_fetch_lock($id) != FALSE) {
          // Return element.
          return array(
            '#theme' => 'image',
            '#path' => variable_get('boolean_icon_locked_ico', drupal_get_path('module', 'boolean_icon') . '/locked.png'),
            '#title' => t('Locked'),
          );
      }
      else {
        // Tokens are required because they prevent CSRF,
        // https://security.drupal.org/node/2429.
        $action = $value ? 'unpublish' : 'publish';
        $token = boolean_icon_token_generate($type, $id, $action);
        $img = $value ?
          variable_get('boolean_icon_true_ico', drupal_get_path('module', 'boolean_icon') . '/on.png') :
          variable_get('boolean_icon_false_ico', drupal_get_path('module', 'boolean_icon') . '/off.png');

        return  array(
          '#theme' => 'link',
          '#text' => theme('image', array('path' => $img)),
          '#path' => 'ajax/boolean-icon/' . $action . '/' . $type . '/' . $id . '/nojs',
          '#options' => array(
            'attributes' => array('class' => 'publishit-link-' . $type . '-' . $id . ' use-ajax'),
            'query' => array('token' => $token),
            'html' => TRUE,
          ),
        );
      }
    }
  }
}
