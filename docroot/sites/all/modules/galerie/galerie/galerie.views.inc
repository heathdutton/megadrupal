<?php

/**
 * Implements hook_views_data().
 */
function galerie_views_data() {
  return array(
    'galerie' => array(
      'table' => array(
        'group' => t('Galerie'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
          ),
        ),
      ),
      'type' => array(
        'title' => t('Type'),
        'help' => t('The gallery type.'),
        'field' => array(
          'handler' => 'views_handler_field_galerie_type',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_galerie_type',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_galerie_type',
        ),
      ),
      'style' => array(
        'title' => t('Style'),
        'help' => t('The gallery style.'),
        'field' => array(
          'handler' => 'views_handler_field_galerie_style',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_galerie_style',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_galerie_style',
        ),
      ),
      'slideshow' => array(
        'title' => t('Slideshow'),
        'help' => t('Is the slideshow enabled.'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
          'click sortable' => TRUE,
          'output formats' => array(
            'enabled-disabled' => array(t('Enabled'), t('Disabled')),
          ),
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_boolean_operator',
          'label' => t('Enabled'),
          'type' => 'yes-no',
          'use equal' => TRUE, // Use slideshow = 1 instead of slideshow <> 0 in WHERE statment
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
    ),
  );
}


/**
 * Filter by galerie type.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_galerie_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Galerie types');
      $types = galerie_types();
      $options = array();
      foreach ($types as $type => $info) {
        $options[$type] = t($info['title']);
      }
      asort($options);
      $this->value_options = $options;
    }
  }
}

/**
 * Field handler to translate a galerie type into its readable form.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_galerie_type extends views_handler_field_node_type {
  /**
   * Provide machine_name option for to node type display.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['machine_name'] = array(
      '#title' => t('Output machine name'),
      '#description' => t('Display field as the galerie type machine name.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['machine_name']),
    );
  }

  /**
    * Render node type as human readable name, unless using machine_name option.
    */
  function render_name($data, $values) {
    if ($this->options['machine_name'] != 1 && $data !== NULL && $data !== '') {
      $types = galerie_types();
      return t($this->sanitize_value($types[$data]['title']));
    }
    return $this->sanitize_value($data);
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->render_name($value, $values), $values);
  }
}

/**
 * Argument handler to accept a galerie type.
 */
class views_handler_argument_galerie_type extends views_handler_argument_string {
  function construct() {
    parent::construct('type');
  }

  /**
   * Override the behavior of summary_name(). Get the user friendly version
   * of the node type.
   */
  function summary_name($data) {
    return $this->galerie_type($data->{$this->name_alias});
  }

  /**
   * Override the behavior of title(). Get the user friendly version of the
   * node type.
   */
  function title() {
    return $this->galerie_type($this->argument);
  }

  function galerie_type($type) {
    $types = galerie_types();
    if (isset($types[$type]['title'])) {
      $output = $types[$type]['title'];
    } else {
      $output = t('Unknown galerie type');
    }
    return check_plain($output);
  }
}

/**
 * Filter by galerie style.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_galerie_style extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Galerie types');
      $styles = galerie_styles();
      $options = array();
      foreach ($styles as $style => $title) {
        $options[$style] = $title;
      }
      asort($options);
      $this->value_options = $options;
    }
  }
}

/**
 * Field handler to translate a galerie style into its readable form.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_galerie_style extends views_handler_field_galerie_type {
  /**
   * Provide machine_name option for to node type display.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['machine_name'] = array(
      '#title' => t('Output machine name'),
      '#description' => t('Display field as the galerie style machine name.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['machine_name']),
    );
  }

  /**
    * Render node type as human readable name, unless using machine_name option.
    */
  function render_name($data, $values) {
    if ($this->options['machine_name'] != 1 && $data !== NULL && $data !== '') {
      $types = galerie_styles();
      return t($this->sanitize_value($types[$data]));
    }
    return $this->sanitize_value($data);
  }
}

/**
 * Argument handler to accept a galerie style.
 */
class views_handler_argument_galerie_style extends views_handler_argument_string {
  function construct() {
    parent::construct('type');
  }

  /**
   * Override the behavior of summary_name(). Get the user friendly version
   * of the node type.
   */
  function summary_name($data) {
    return $this->galerie_style($data->{$this->name_alias});
  }

  /**
   * Override the behavior of title(). Get the user friendly version of the
   * node type.
   */
  function title() {
    return $this->galerie_style($this->argument);
  }

  function galerie_style($style) {
    $styles = galerie_styles();
    if (isset($styles[$style])) {
      $output = $styles[$style];
    } else {
      $output = t('Unknown galerie style');
    }
    return check_plain($output);
  }
}

