<?php

/**
 * @file
 * Views related hooks and handlers for Views Current Path.
 */

/**
 * Implements hook_views_data().
 */
function views_current_path_views_data() {
  $data['views']['current_path'] = array(
    'title' => t('Current Path'),
    'help' => t('The path of the page currently being viewed.'),
    'field' => array(
      'handler' => 'views_current_path_views_handler_field_current_path',
    ),
  );
  return $data;
}

/**
 * Views field handler for current path.
 *
 * @ingroup views_field_handlers
 */
class views_current_path_views_handler_field_current_path extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  function query() {
    // Do nothing -- to override the parent query.
  }

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['path_format'] = array('default' => 'raw-internal');
    $options['qs_support_fieldset']['query_string_support'] = array('default' => 'bypass-query-string');

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    $alias_required_message = module_exists('path') ? '' : '<em>'
      . t('Note: the Path module must be enabled for the "Alias" options to work.') . '</em>';
    
    // Determine the URL prefix.
    global $base_url;
    $url_options = array();
    if (function_exists('locale_language_url_rewrite_url')) {
      $url_current_path = url(current_path());
      locale_language_url_rewrite_url($url_current_path, $url_options);
    }
    $raw_relative_prefix = base_path() . (isset($url_options['prefix']) ? $url_options['prefix'] : '');
    $raw_absolute_prefix = $base_url . $raw_relative_prefix;

    $raw_example = 'node/215';
    $alias_example = 'pages/example-path';
    $query_string_example = '?nid=357&tid=271';
    $query_string_valid_path_example = '[current_path]tid=[tid]';
    $query_string_invalid_path_example = '[current_path]?tid=[tid]';
    
    $form['path_format'] = array(
      '#type' => 'radios',
      '#title' => t('Output style'),
      '#description' =>   $alias_required_message,
      '#options' => array(
        'raw-internal' => t('Raw internal path (e.g. @example)',
          array('@example' => $raw_example)),
        'raw-relative' => t('Raw relative URL (e.g. @example)',
          array('@example' => $raw_relative_prefix . $raw_example)),
        'raw-absolute' => t('Raw absolute URL (e.g. @example)',
          array('@example' => $raw_absolute_prefix . $raw_example)),
        'alias-internal' => t('Alias internal path (e.g. @example)',
          array('@example' => $alias_example)),
        'alias-relative' => t('Alias relative URL (e.g. @example)',
          array('@example' => $raw_relative_prefix . $alias_example)),
        'alias-absolute' => t('Alias absolute URL (e.g. @example)',
          array('@example' => $raw_absolute_prefix . $alias_example)),
      ),
      '#default_value' => $this->options['path_format'],
    );

    $form['qs_support_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Query string support'),
      '#dependency' => array('radio:options[path_format]' => array('alias-relative')),
    );

    $qs_message = '<p>' . t('Query strings are included in the') . ' "<strong>' . t('Alias relative URL') . '</strong>" ' . t('output style.') . '</p>';
    $qs_message .= '<p>' . t('Use query string support when creating a Views path link rewrite and a query string is to be included. ');
    $qs_message .= t('For example, if building a link such as "@example" with multiple key-value pairs, the "concatentate" option would be required. ',
      array('@example' => $raw_relative_prefix . $alias_example . $query_string_example));
    $qs_message .= '</p><p><em>' . t('Note: If using this feature, do not include a question mark (?) in the Views path link rewrite. ');
    $qs_message .= t('A proper rewrite for query string support would be "@example_valid", and not "@example_invalid". ',
      array('@example_valid' => $query_string_valid_path_example, '@example_invalid' => $query_string_invalid_path_example));
    $qs_message .= t('A question mark will be added by the module if necessary.') . '</em></p>';
    $form['qs_support_fieldset']['query_string_support'] = array(
      '#type' => 'radios',
      '#title' => t('Select an option'),
      '#options' => array(
        'bypass-query-string' => t('Bypass query string support'),
        'remove-query-string' => t('Remove existing query string on the current path'),
        'replace-query-string' => t('Replace existing query string on the current path with values passed through path rewrite'),
        'concat-query-string' => t('Concatenate existing query string on the current path with values passed through the path rewrite'),
      ),
      '#description' => $qs_message,
      '#default_value' => $this->options['qs_support_fieldset']['query_string_support'],
    );

    $form['view_edit_notice'] = array(
      '#markup' => '<p>Note: ' . t('The placeholder @placeholder will be used for the field value while editing the view.',
        array('@placeholder' => '[' . $this->options['id'] . ']')) . '</p>',
    );

    parent::options_form($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  function render($values) {
    // Display a placeholder (i.e. field id) when editing the view.
    if (strpos(current_path(), 'admin/structure/views/nojs/preview/' . $this->view->name . '/') === 0) {
      return '[' . $this->options['id'] . ']';
    }

    $path_format = $this->options['path_format'];
    $query_string_support = $this->options['qs_support_fieldset']['query_string_support'];

    // In case the path module has been disabled, revert "alias" to "raw".
    if (strpos($path_format, 'alias') === 0 && !module_exists('path')) {
      $path_format = str_replace('alias-', 'raw-', $path_format);
    }

    // Determine the URL prefix.
    global $base_url;
    $url_options = array();
    if (function_exists('locale_language_url_rewrite_url')) {
      $url_current_path = url(current_path());
      locale_language_url_rewrite_url($url_current_path, $url_options);
    }
    $raw_relative_prefix = base_path() . (isset($url_options['prefix']) ? $url_options['prefix'] : '');
    $raw_absolute_prefix = $base_url . $raw_relative_prefix;

    // Determine the path.
    switch ($path_format) {
      case 'raw-internal':
        $output = current_path();
        break;
      case 'raw-relative':
        $output = $raw_relative_prefix . current_path();
        break;
      case 'raw-absolute':
        $output = $raw_absolute_prefix . current_path();
        break;
      case 'alias-internal':
        $output = request_path();
        break;
      case 'alias-relative':
        $output = request_uri();
        // If using alias-relative, process query string support setting.
        switch ($query_string_support) {
          // If bypass is selected, skip any changes.
          case 'bypass-query-string':
            break;
          case 'remove-query-string':
            if (stripos($output, '?') !== FALSE) {
              $output = strtok($output, '?');
            }
            break;
          case 'replace-query-string':
            if (stripos($output, '?') !== FALSE) {
              $output = strtok($output, '?') . '?';
            }
            break;
          case 'concat-query-string':
            if (stripos($output, '?') !== FALSE) {
              $output .= '&';
            }
            else {
              $output .= '?';
            }
            break;
        }
        break;
      case 'alias-absolute':
        $output = url(current_path(), array('absolute' => TRUE));
        break;
      default:
        $output = current_path();
    }

    return $output;
  }

}
