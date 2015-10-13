<?php

/**
 * @file
 * Definition of ViewsHandlerFieldViewsCtoolsDropbutton.
 */

/**
 * Provides a handler that adds ctools dropbutton.
 *
 * @ingroup views_field_handlers
 */
class ViewsHandlerFieldViewsCtoolsDropbutton extends views_handler_field {

  /**
   * @inheritdoc
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['fields'] = array('default' => array());
    $options['destination'] = array('default' => TRUE, 'bool' => TRUE);
    return $options;
  }

  /**
   * @inheritdoc
   */
  function options_form(&$form, &$form_state) {
    $parent_form = $form;
    parent::options_form($parent_form, $form_state);
    $form['custom_label'] = $parent_form['custom_label'];
    $form['label'] = $parent_form['label'];
    $all_fields = $this->view->display_handler->get_field_labels();
    // Offer to include only those fields that follow this one.
    $field_options = array_slice($all_fields, 0, array_search($this->options['id'], array_keys($all_fields)));
    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields'),
      '#description' => t('Fields to be included as contextual links.'),
      '#options' => $field_options,
      '#default_value' => $this->options['fields'],
    );
    $form['destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include destination'),
      '#description' => t('Include a "destination" parameter in the link to return the user to the original view upon completing the contextual action.'),
      '#default_value' => $this->options['destination'],
    );
  }

  /**
   * @inheritdoc
   */
  function render($values) {
    $links = array();
    foreach ($this->options['fields'] as $field) {
      if (empty($this->view->style_plugin->rendered_fields[$this->view->row_index][$field])) {
        continue;
      }
      $title = $this->view->field[$field]->last_render_text;
      $path = '';
      if (!empty($this->view->field[$field]->options['alter']['path'])) {
        $path = $this->view->field[$field]->options['alter']['path'];
      }
      if (!empty($title) && !empty($path)) {
        // Make sure that tokens are replaced for this paths as well.
        $tokens = $this->get_render_tokens(array());
        $path = strip_tags(decode_entities(strtr($path, $tokens)));

        $path_parts = parse_url($path);
        $links[$field] = array(
          'href' => $path_parts['path'],
          'title' => $title,
        );

        // Add get parameters if they exist.
        $query_parts = array();
        if (!empty($path_parts['query'])) {
          parse_str($path_parts['query'], $query_parts);
          $links[$field]['query'] = $query_parts;
        }
        // Add url destination if not set and required by options.
        if (empty($query_parts['destination']) && !empty($this->options['destination'])) {
          if (empty($links[$field]['query'])) {
            $links[$field]['query'] = array();
          }
          $links[$field]['query'] = array_merge($links[$field]['query'], drupal_get_destination());
        }

        // Add link classes if they exist.
        if (!empty($this->view->field[$field]->options['alter']['link_class'])) {
          $link_classes = $this->view->field[$field]->options['alter']['link_class'];
          $links[$field]['attributes']['class'] = explode(' ', $link_classes);
        }
      }
    }

    if (!empty($links)) {
      $build = array(
        '#theme' => 'links__ctools_dropbutton',
        '#links' => $links,
        '#attributes' => array(
          'class' => array('inline', 'links', 'actions', 'horizontal', 'right')
        ),
      );
      return drupal_render($build);
    }
    else {
      return '';
    }
  }

  /**
   * @inheritdoc
   */
  function query() { }
}
