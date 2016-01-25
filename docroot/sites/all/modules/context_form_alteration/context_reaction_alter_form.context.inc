<?php
/**
 * @file
 * The Context reaction class.
 */

/**
 * Expose menu items as context reactions.
 */
class context_reaction_alter_form extends context_reaction {
  /**
   * Provide a form elements allowing adding fields and values.
   */
  public function options_form($context) {
    // Initalize.
    $form = array();
    $delta = 0;
    $notes = t('<h3>Alter Forms</h3>
      <ul>
        <li><b>Form ID:</b> Only perform this alter on this form (mulitple comma separated allowed).</li>
        <li><b>Parent Heirarchy:</b> From API structural location of element to alter, add or append (comma separated).
          Should be listed including the target element, eg: "group_THIS, field_THIS".</li>
        <li><b>Attributes:</b> use Drupal form API norms, eg: #default_value, #title, #maxlength. See !api_link.</li>
        <li><b>Value:</b> What to add, alter or append.</li>
        <li><b>Append:</b> Add to a currently existing value. The alteration will detect array or string data type.</li>
        <li><b>Translate:</b> If value is a string which should be registered for interface traslation, eg: button text, label, etc.</li>
      </ul><br>',
      array('!api_link' => l(
        t('Form API Reference'),
        'http://api.drupal.org/api/drupal/developer%21topics%21forms_api_reference.html/7'
      ))
    );
    $form['elements'] = array(
      '#theme' => 'context_form_alteration_settings_table',
      '#prefix' => '<div id="context-form-alteration-elements-table">' . $notes,
      '#suffix' => '</div>',
    );

    // Build existing entries.
    $values = $this->fetch_from_context($context);
    if (isset($values['elements'])) {
      foreach ($values['elements'] as $val) {
        $form['elements'][$delta] = $this->build_row(
          $delta,
          $val['form_id'],
          $val['parents'],
          $val['attribute'],
          $val['value'],
          $val['append'],
          $val['i18n']
        );
        $delta++;
      }
    }

    // Add placeholder rows.
    $number_of_empty_rows = 3;
    for ($i = 0; $i < $number_of_empty_rows; $i++) {
      $form['elements'][$delta] = $this->build_row($delta);
      $delta++;
    }

    return $form;
  }

  /**
   * Override of options_form_submit().
   *
   * Post submit specialty actions.
   */
  public function options_form_submit($values) {
    // Gotta catch em' all.
    foreach ($values['elements'] as $key => $option) {
      // Update the translation string registry.
      if (isset($option['i18n']) && $option['i18n']) {
        $option['value'] = i18n_string('form', $option['value'], array('update' => TRUE));
      }
      // Clean up empty values for storage and validate.
      if ($option['form_id'] == '' || ($option['parents'] == '' && $option['attribute'] == '')) {
        unset($values['elements'][$key]);
      }
    }

    return $values;
  }

  /**
   * {@inheritdoc}
   */
  public function execute(&$form) {
    $contexts = $this->get_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions[$this->plugin])) {
        foreach ($context->reactions[$this->plugin] as $option) {

          // Whether we should bother.
          foreach ($option as $opt) {
            // Allow altering mulitple forms.
            $forms = array();
            $forms = explode(',', $opt['form_id']);
            // Trim up the form text, allowing messier/easier user input.
            array_walk($forms, create_function('&$val', '$val = trim($val);'));
            foreach ($forms as $f) {
              if ($f === $form['#form_id']) {
                // Alter the form value.
                // @todo Maybe add option for forcing.
                if ($opt['parents']) {
                  $parents = array();
                  $parents = explode(',', $opt['parents']);
                  $parents[] = $opt['attribute'];
                }
                else {
                  $parents = array($opt['attribute']);
                }
                // Trim up the parent text, allowing messier/easier user input.
                array_walk($parents, create_function('&$val', '$val = trim($val);'));
                // Allow translating value; for button text, labels, etc.
                if (module_exists('i18n') && !empty($opt['i18n'])) {
                  $i18n_context = context_form_alteration_i18n_context($opt, $context);
                  $opt['value'] = i18n_string($i18n_context, $opt['value'], array('sanitize' => FALSE));
                }
                $append = empty($opt['append']) ? FALSE : $opt['append'];

                // Sanitize and set the value.
                $opt['value'] = filter_xss_admin($opt['value']);
                $this->craf_drupal_array_set_nested_value($form, $parents, $opt['value'], FALSE, $append);
              }
            }
          }
        }
      }
    }
  }

  /**
   * Build a form row.
   */
  protected function build_row($delta, $form_id = NULL, $parents = NULL, $attribute = NULL, $value = NULL, $append = FALSE, $i18n = FALSE) {
    $form['form_id'] = array(
      '#default_value' => $form_id,
      '#type' => 'textfield',
      '#size' => 20,
      '#maxlength' => 256,
    );
    $form['parents'] = array(
      '#default_value' => $parents,
      '#type' => 'textfield',
      '#size' => 20,
      '#maxlength' => 256,
    );
    $form['attribute'] = array(
      '#default_value' => $attribute,
      '#type' => 'textfield',
      '#size' => 20,
      '#maxlength' => 256,
    );
    $form['value'] = array(
      '#default_value' => $value,
      '#type' => 'textfield',
      '#size' => 20,
      '#maxlength' => NULL,
    );
    $form['append'] = array(
      '#default_value' => $append,
      '#type' => 'checkbox',
    );
    $form['i18n'] = array(
      '#default_value' => $i18n,
      '#type' => 'checkbox',
    );
    return $form;
  }

  /**
   * Override of drupal_array_set_nested_value(), allows append.
   */
  protected function craf_drupal_array_set_nested_value(array &$array, array $parents, $value, $force = FALSE, $append = FALSE) {
    $ref = &$array;
    foreach ($parents as $parent) {
      // PHP auto-creates container arrays and NULL entries without error if $ref
      // is NULL, but throws an error if $ref is set, but not an array.
      if ($force && isset($ref) && !is_array($ref)) {
        $ref = array();
      }
      $ref = &$ref[$parent];
    }
    if ($append) {
      if (is_array($ref)) {
        $ref[] = $value;
        return;
      }
      else {
        $ref .= $value;
      }
    }
    else {
      $ref = $value;
    }
  }
}
