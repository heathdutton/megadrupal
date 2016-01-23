<?php
/**
 * Field handler for the compound collapsing field.
 */
class views_compound_field_handler_field_collapsing extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['included_fields'] = array('default' => array());
    $options['field_span']     = array('default' => 1);
    $options['field_class']     = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array();
    $fields = $this->view->display_handler->get_handlers('field');

    // Get the unique id of this field (ie allow more than one of this handler).
    $this_id = $this->options['id'];

    // Build up the options from all the fields up to this one but no further.
    foreach ($fields as $field_id => $handler) {
      if ($field_id == $this_id) {
        break;
      }
      $label = empty($handler->options['label']) ? $field_id : $handler->options['label'];
      $options[$field_id] = $label;
    }

    $form['included_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Collapsing fields'),
      '#options' => $options,
      '#default_value' => isset($this->options['included_fields']) ? $this->options['included_fields'] : '',
      '#description' => t('Fields to collapse: only the first field that has content will be output. Only fields that are ordered above this one may be included. You should set the fields selected here to exclude from display if you do not want them output twice.'),
    );

    $form['field_span'] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($this->options['field_span']) ? $this->options['field_span'] : 1,
      '#title' => t('Wrap output in a span'),
      '#id' => 'views-collapsing-span',
    );

    // Make the other form items dependent upon it.
    ctools_include('dependent');
    ctools_add_js('dependent');

    $form['field_class'] = array(
      '#type' => 'textfield',
      '#title' => t('CSS class'),
      '#default_value' => isset($this->options['field_class']) ? $this->options['field_class'] : '',
      '#description' => t('Additional CSS classes to add to wrapped output.'),
      '#dependency' => array('views-collapsing-span' => array(1)),
    );
  }

  /**
   * Validate the form options to check we have two or more fields selected.
   */
  function options_validate($form, &$form_state) {
    parent::options_validate($form, $form_state);

    if (count(array_filter($form_state['values']['options']['included_fields'])) < 2) {
      form_error($form['included_fields'], t('You must select at least two fields.'));
    }
  }

  /**
   * Validate the whole view to check a collapsing field comes after its options.
   */
  function validate() {
    $errors = parent::validate();
    foreach ($this->view->display as $display_id => $display) {
      if (isset($display->display_options['fields'])) {
        $seen = array();
        foreach ($display->display_options['fields'] as $field_id => $field) {
          if ($field['field'] == 'views_collapsing') {
            // For a collapsing field, check all its selected fields have
            // already been seen; that is, they come before it.
            foreach (array_filter($field['included_fields']) as $included_field_id) {
              if (!isset($seen[$included_field_id])) {
                $errors[] = t("Field @field is included in the collapsing field but is not listed before it.", array(
                  '@field' => $included_field_id,
                ));
              }
            }
          }
          else {
            // For any other kind of field, just notice we've seen it.
            $seen[$field_id] = TRUE;
          }

        }
      }
    }

    return $errors;
  }

  function query() {
    // Do nothing -- to override the parent query.
  }

  /**
   * Render the field.
   */
  function render($values) {
    $render = array();
    // Intersect the actual fields with the field names in the settings to get
    // the fields we want to work with.
    $fields = array_intersect_key($this->view->display_handler->get_handlers('field'), array_filter($this->options['included_fields']));
    //dsm($this->options['included_fields'], 'o');
    //dsm($fields, 'fields');
    foreach ($fields as $field => $handler) {
      if (isset($handler->last_render)) {
        $render[$field] = $handler->last_render;
      }
    }

    // Take the first non-empty value.
    foreach ($render as $field => $value) {
      if (!empty($value)) {
        // Note that we don't run this through check_plain() as this has
        // already been done by the prior handlers' render() methods.
        $output = $value;
        break;
      }
    }

    // If the user has opted to wrap the output in a span we do so.
    if ($this->options['field_span']) {
      // Form a class name from the final field used so themers can identify the
      // actual field the data comes from.
      $used_field_class = 'views-field-' . drupal_clean_css_identifier($field);
      // Add the class from the admin settings.
      if (isset($this->options['field_class'])) {
        $used_field_class .= ' ' . $this->options['field_class'];
      }

      // @todo? Technically this should go in a theme function, but this somehow
      // feels like overkill :/ Leaving it thus until either I find I need it
      // theming or someone complains ;)
      // This is a SPAN because there is no way to avoid everything we output
      // here falling inside a SPAN class="field-content"; putting a DIV here
      // would be invalid HTML.
      // We have to add a SPAN here to get out class used, as there is no way
      // to influence the class given to the DIV, which is set in
      // template_preprocess_views_view_fields().
      $output = '<span class="' . $used_field_class . '">'
        // Add an inner SPAN for theming, but use a different class so that
        // a universal .field-content selector doesn't hit this twice.
        . '<span class="field-content-collapsing">'
        . $output
        . '</span>'
        . '</span>';
    }
    return $output;
  }
}
