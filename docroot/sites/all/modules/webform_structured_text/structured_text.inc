<?php
/**
 * Include file for structured text component.
 **/

define('WFST_PAD_CHARACTER', chr(31)); // ASCII Unit Separator

/**
 * function to handle difference between version 3 and 4 of Webform.
 * @param string $prop The property needed.
 * @return string The property name to use elsewhere.
 */
function _webform_structured_text_get_wf_property($prop = 'required') {
  $info = system_get_info('module', 'webform');
  $version = array();
  preg_match('/^[0-9]+\.x\-([0-9]+)\..*$/', $info['version'], $version);
  $properties = array(
    'required' => array(
      '3' => 'mandatory',
      '4' => 'required',
    ),
    'filter_xss' => array(
      '3' => '_webform_filter_xss',
      '4' => 'webform_filter_xss',
    ),
    'filter_desc' => array(
      '3' => '_webform_filter_descriptions',
      '4' => 'webform_filter_descriptions',
    ),
  );
  if ( $prop == 'version' ) return $version[1];
  return $properties[$prop][$version[1]];
}


/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_structured_text() {
  return array(
    'name' => '',
    'form_key' => NULL,
    _webform_structured_text_get_wf_property('required') => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'mask' => '',
      'auto_skip' => TRUE,
      'mask_description' => FALSE,
      'mask_regex' => '',
      'mask_labels' => '',
      'description' => '',
      'placeholders' => '',
      'analysis_display' => FALSE,
      'analysis_display_identifying' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_structured_text() {
  return array(
    'webform_structured_text' => array(
      'render element' => 'element',
      'file' => 'structured_text.inc',
      'path' => drupal_get_path('module', 'webform_structured_text'),
    ),
    'webform_display_structured_text' => array(
      'render element' => 'element',
      'file' => 'structured_text.inc',
      'path' => drupal_get_path('module', 'webform_structured_text'),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_structured_text($component) {
  $form = array();

  $form['extra']['mask'] = array(
    '#type' => 'textfield',
    '#title' => t('Input mask'),
    '#default_value' => $component['extra']['mask'],
    '#description' => t('Enter an input mask.  Use the following special characters to represent
          parts of the mask.
          <ul>
          <li>9 - only a digit can be input</li>
          <li>x - any non-blank character can be input</li>
          <li>a - only letters can be input (upper or lower case)</li>
          <li>r{<em>n</em>{,<em>s</em>}} - any characters with the maximum length
                  of the field specified by <em>n</em> where <em>n</em> is an integer
                  greater than 0.  If <em>n</em> is omitted, the maximum length
                  will be 1.  Optionally, a display size <em>s</em> for the
                  field can also be specified by following the maximum length with
                  a comma and an integer value greater than zero.  For example,
                  "r20,10" creates a field maximum 20 characters long with a display
                  length of 10 characters.  Other than for maximum length, this
                  mask relies completely on a regex to validate the field\'s input.
                  Thus, if you don\'t specify a regex expression, any characters
                  can be entered into the field, and the field can be left blank,
                  even if the component is marked as required.</li>
          <li>^9 - outputs the character "9"</li>
          <li>^x - outputs the character "x"</li>
          <li>^a - outputs the character "a"</li>
          <li>^r - outputs the character "r"</li>
          <li>^^ - outputs the character "^"</li>
          </ul>
          all other characters in the mask will be rendered as mark-up around / between
          the textboxes for the portions of the mask that are denoted by the digit nine,
          or lower-case characters "x", "a", or "r".  If you want a certain combination of
          letters and numbers, use "x" or "r" as your mask character, and specify a RegEx
          pattern(s) below to check the pattern.'),
    '#weight' => -3.9,
    '#required' => TRUE,
  );
  $form['extra']['auto_skip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-skip to next chunk'),
    '#default_value' => $component['extra']['auto_skip'],
    '#description' => t('Individual chunks of the field are rendered as text boxes
          with markup in between for parts of the mask that are not the digit nine,
          or lower-case characters "x" or "a".  Check this box to have the cursor
          automatically move to the next chunk in the field when the previous one
          is filled.' ),
    '#weight' => -3.8,
  );
  $form['extra']['mask_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include mask description'),
    '#default_value' => $component['extra']['mask_description'],
    '#description' => t('Check this box to show a description of the mask in the field\'s
          description area.  This will appear appended to the description you specify
          below (if any).  If you don\'t provide a description below and this box
          is checked, the mask description will still show.' ),
    '#weight' => -3.7,
  );
  $form['extra']['mask_regex'] = array(
    '#type' => 'textarea',
    '#title' => t('Mask RegEx validation'),
    '#default_value' => $component['extra']['mask_regex'],
    '#description' => t('If you want to further validate the pattern of a chunk, you
          can do that by specifying a RegEx expression here that will be used to
          further validate the user\'s input.  Each validation should be on a separate
          line, in the form<br><br>
          <em>chunk-number</em>|<em>error-message</em>|<em>regex-expression</em><br><br>
          with each portion separated by a vertical bar / pipe character, where:<br>
          - <em>chunk-number</em> is an integer corresponding to the ordinal position
          of the chunk,<br>
          - <em>error-message</em> is the error message to display if the user input
          for that chunk doesn\'t match the RegEx expression, and<br>
          - <em>regex-expression</em> is the expression to match against.<br><br>
          For example, to validate a Canadian postal code, which has the format
          letter-number-letter number-letter-number (eg., M5B 2J7), if you wanted
          two chunks, the mask would be "xxx xxx", with validations being<br><br>
          1|First part of postal code must be letter-number-letter.|/[A-Z][0-9][A-Z]/<br>
          2|Second part of postal code must be number-letter-number.|/[0-9][A-Z][0-9]/<br><br>
          As a second example, if you wanted a North American style phone number with optional
          extension, the mask would be "(999) 999-9999  E^xt: r10,5" (note the escaped "x"
          in "Ext"), with the validation being<br><br>
          4|Please specify only digits for the extension.|/^[0-9]*$/<br><br>
          You do not have to specify a validation for every chunk.  For more on
          RegEx expressions and preg_match (which is what is used), see !link.',
          array('!link' => '<a href="http://ca2.php.net/preg_match" target=_blank>http://ca2.php.net/preg_match</a>',)),
    '#weight' => -3.6,
    '#element_validate' => array('_webform_structured_text_regex_validate',),
  );
  $form['extra']['mask_labels'] = array(
    '#type' => 'textarea',
    '#title' => t('Mask labels'),
    '#default_value' => $component['extra']['mask_labels'],
    '#description' => t('You can specify invisible labels for each chunk using a
          format similar to the RegEx validation.  This can be useful for making
          the form accessible using a screen reader.  Use the form<br><br>
          <em>chunk-number</em>|<em>label-text</em><br><br>For example<br><br>
          1|Area code<br>
          2|Exchange<br>
          3|Number<br><br>
          If you don\'t specify mask labels, generic "<em>component_title</em>,
          part <em>n</em>" labels will be used, where <em>n</em> is the chunk number.' ),
    '#weight' => -3.55,
    '#element_validate' => array('_webform_structured_text_labels_validate',),
  );
  $form['extra']['placeholders'] = array(
    '#type' => 'textarea',
    '#title' => t('Placeholders'),
    '#default_value' => $component['extra']['placeholders'],
    '#description' => t('You can specify placeholders for each chunk using a
          format similar to the Mask Labels validation.  Use the form<br><br>
          <em>chunk-number</em>|<em>placeholder-text</em><br><br>For example<br><br>
          1|First Name<br>
          2|Last Name' ),
    '#weight' => -3.53,
    '#element_validate' => array('_webform_structured_text_placeholders_validate',),
  );
  $form['extra']['css_classes'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS Class(es)'),
    '#default_value' => ( isset($component['extra']['css_classes']) ? $component['extra']['css_classes'] : '' ),
    '#description' => t('You can specify CSS class(es) here if you like to help with theming.
          Separate each class with a space.  Class(es) will be applied to the highest-level
          &lt;div&gt; for the component.  Each part of the component will have classes
          "structured-text" and "part-<em>n</em>" where <em>n</em> is the part of the
          mask with <em>n</em> starting at zero.  Markup portions of the mask also have
          a class "markup".'),
    '#weight' => -3.5,
    '#required' => FALSE,
  );
  $form['extra']['analysis_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show responses in analysis'),
    '#default_value' => $component['extra']['analysis_display'],
    '#description' => t('Show each answer in the analysis results'),
    '#weight' => 3,
    '#parents' => array('extra', 'analysis_display'),
  );
  $other_components = array('' => t('Submission ID')) +
      db_query('SELECT cid, form_key FROM {webform_component} WHERE nid = :nid ORDER BY weight, form_key',
          array(':nid' => $component['nid'],))->fetchAllKeyed();
  $form['extra']['analysis_display_identifying'] = array(
    '#type' => 'select',
    '#title' => t('Identifying field'),
    '#default_value' => $component['extra']['analysis_display_identifying'],
    '#description' => t('Which component do you want to use to identify results?'),
    '#options' => $other_components,
    '#weight' => 4,
    '#parents' => array('extra', 'analysis_display_identifying'),
  );
  return $form;
}

/**
 * Validate that any supplied Mask Regex Validation rules are properly formed.
 * @param array $element The form element holding the regex lines.
 * @param array $form_state Current state of the form.
 */
function _webform_structured_text_regex_validate($element, &$form_state) {
  if ( !empty($element['#value']) ) {
    $lines = array_filter(explode("\n", $element['#value']));
    foreach ( $lines as $line_number => $line ) {
      $parts = explode('|', $line, 3);
      $parts[1] = trim($parts[1]);
      $parts[2] = trim($parts[2]);
      if ( count($parts) != 3 || empty($parts[1]) || empty($parts[2]) ) {
        form_error($element, t('Line !line of the Mask Regex Validation is ill-formed.
                There must be three parts to each line, separaged by vertical bars / pipes.  Please
                review the detailed instructions for the Mask Regex Validation and try again.',
                array('!line' => $line_number + 1,)));
      }
      else {
        if ( !is_numeric($parts[0]) || (int) $parts[0] != $parts[0] || $parts[0] < 1 ) {
          form_error($element,t('Line !line of Mask Regex Validation must start with
                  a positive integer signifying the chunk number of the mask.  "@value" is
                  not a valid chunk number.',
                  array('!line' => $line_number + 1, '@value' => $parts[0],)));
        }
      }
    }
  }
}

/**
 * Validate that any supplied Mask Labels are properly formed.
 * @param array $element The form element holding the label lines.
 * @param array $form_state Current state of the form.
 */
function _webform_structured_text_labels_validate($element, &$form_state) {
  if ( !empty($element['#value']) ) {
    $lines = array_filter(explode("\n", $element['#value']));
    foreach ( $lines as $line_number => $line ) {
      $parts = explode('|', $line, 2);
      $parts[1] = trim($parts[1]);
      if ( count($parts) != 2 || empty($parts[1]) ) {
        form_error($element, t('Line !line of the Mask Labels is ill-formed.
                There must be two parts to each line, separaged by vertical bars / pipes.  Please
                review the instructions for the Mask Labels and try again.',
                array('!line' => $line_number + 1,)));
      }
      else {
        if ( !is_numeric($parts[0]) || (int) $parts[0] != $parts[0] || $parts[0] < 1 ) {
          form_error($element,t('Line !line of Mask Labels must start with
                  a positive integer signifying the chunk number of the mask.  "@value" is
                  not a valid chunk number.',
                  array('!line' => $line_number + 1, '@value' => $parts[0],)));
        }
      }
    }
  }
}

/**
 * Validate that any supplied Placeholders are properly formed.
 * @param array $element The form element holding the label lines.
 * @param array $form_state Current state of the form.
 */
function _webform_structured_text_placeholders_validate($element, &$form_state) {
  if ( !empty($element['#value']) ) {
    $lines = array_filter(explode("\n", $element['#value']));
    foreach ( $lines as $line_number => $line ) {
      $parts = explode('|', $line, 2);
      $parts[1] = trim($parts[1]);
      if ( count($parts) != 2 || empty($parts[1]) ) {
        form_error($element, t('Line !line of the Placeholders is ill-formed.
                There must be two parts to each line, separaged by vertical bars / pipes.  Please
                review the instructions for the Placeholders and try again.',
                array('!line' => $line_number + 1,)));
      }
      else {
        if ( !is_numeric($parts[0]) || (int) $parts[0] != $parts[0] || $parts[0] < 1 ) {
          form_error($element,t('Line !line of Placeholders must start with
                  a positive integer signifying the chunk number of the mask.  "@value" is
                  not a valid chunk number.',
                  array('!line' => $line_number + 1, '@value' => $parts[0],)));
        }
      }
    }
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_structured_text($component, $value = NULL, $filter = FALSE) {
  $description = webform_structured_text_field_description($component);
  $xss_filter = _webform_structured_text_get_wf_property('filter_xss');
  $desc_filter = _webform_structured_text_get_wf_property('filter_desc');
  $form_item = array(
    '#type' => 'webform_structured_text',
    '#input' => TRUE,
    '#title' => $filter ? $xss_filter($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#required' => $component[_webform_structured_text_get_wf_property('required')],
    '#description'   => $filter ? $desc_filter($description) : $description,
    '#process' => array('webform_structured_text_expand_field',),
    '#pre_render' => array('webform_element_title_display'),
    '#post_render' => array('webform_element_wrapper'),
    '#prefix' => '<div class="webform-component-' . $component['type'] .
            ( !empty($component['extra']['css_classes']) ? ' ' . check_plain($component['extra']['css_classes']) : '' ) .
            '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
    '#theme' => 'webform_structured_text',
    '#theme_wrappers' => array('webform_element'),
    '#element_validate' => array('webform_validate_structured_text'),
    '#mask' => webform_structured_text_parse_mask($component['extra']['mask']),
    '#mask_regex' => $component['extra']['mask_regex'],
    '#mask_labels' => $component['extra']['mask_labels'],
    '#placeholders' => $component['extra']['placeholders'],
    '#auto_skip' => $component['extra']['auto_skip'],
    '#translatable' => array('title', 'description', 'mask_labels', 'placeholders',),
  );

  if (isset($value)) {
    $form_item['#default_value'] = webform_structured_text_parse_value($form_item['#mask'], $value[0]);
  }

  return $form_item;
}

/**
 * Form API #process function for Webform structured text fields.  Breaks up the
 * form element into the combination of mark-up and text input fields specified
 * in the mask.
 */
function webform_structured_text_expand_field($element) {
  static $js_added = FALSE;
  $js_ids = array();
  $mask_labels = array();
  if ( !empty($element['#mask_labels']) ) {
    $mask_labels_lines = array_filter(explode("\n", $element['#mask_labels']));
    foreach ( $mask_labels_lines as $line ) {
      list($chunk, $label) = explode('|', $line, 2);
      $mask_labels[$chunk] = $label;
    }
  }
  $placeholders = array();
  if ( !empty($element['#placeholders']) ) {
    $placeholders_lines = array_filter(explode("\n", $element['#placeholders']));
    foreach ( $placeholders_lines as $line ) {
      list($chunk, $placeholder) = explode('|', $line, 2);
      $placeholders[$chunk] = $placeholder;
    }
  }
  $chunk = 0;

  // For each part of the mask, the labels and placeholders as well as markup portions
  // of the mask are managed by webform_localization.  #title and #attributes are
  // sanitised later in drupal_render, so no strip_tags of the user input is done
  // here.  However, the markup is not, so strip_tags is done on the translated string.
  foreach ( $element['#mask'] as $part => $details ) {
    if ( $details['type'] != 'markup' ) {
      $chunk++;
      $element['part-' . $part] = array(
        '#title' => ( empty($mask_labels[$chunk])
              ? t('!eltitle, part !chunk',
                      array(
                        '!eltitle' => $element['#title'],
                        '!chunk' => $chunk,
                      )
                  )
              : $mask_labels[$chunk] ),
        '#title_display' => 'invisible',
        '#type' => 'textfield',
        '#size' => ( isset($details['size']) && $details['size'] > 0 ? $details['size'] : $details['length'] ),
        '#maxlength' => $details['length'],
        '#default_value' => ( isset($element['#default_value'][$part]) ? $element['#default_value'][$part] : '' ),
        '#attributes' => array(
          'class' => array('structured-text part-' . $part,),
        ),
      );
      if ( !empty($placeholders[$chunk]) ) {
        $element['part-' .  $part]['#attributes']['placeholder'] = $placeholders[$chunk];
      }
      $js_ids[] = $part;
    }
    else {
      $element['part-' . $part] = array(
        '#type' => 'markup',
        '#markup' => strip_tags(_webform_structured_text_t(
                $element['#webform_component']['nid'] . ':' . $element['#webform_component']['cid'] . ":mask:$part",
                $details['value'])),
        '#prefix' => '<div class="structured-text markup part-' . $part . '">',
        '#suffix' => '</div>',
      );
    }
  }
  if ( count($js_ids) > 1 && $element['#auto_skip'] ) {
    foreach ( $js_ids as $index => $id ) {
      if ( isset($js_ids[$index + 1]) ) {
        $element['part-' . $id]['#attributes']['onkeyup'] = "webform_structured_text_jump(event, '{$element['#id']}-part-$id',
          '{$element['#id']}-part-{$js_ids[$index + 1]}');";
      }
    }
    if ( !$js_added ) {
      $script = "function webform_structured_text_jump(event, from_id, to_id) {
                  var $ = jQuery;
                  if ( event.keyCode != 9 && event.keyCode != 16 ) {
                    fromfield = $('#' + from_id);
                    if ( fromfield.val().length >= fromfield.attr('maxlength') ) {
                      $('#' + to_id).focus().select();
                    }
                  }
                }";
      drupal_add_js($script, 'inline');
      $js_added = TRUE;
    }
  }
  return $element;
}

/**
 * Validation function for structured text.  Ensure that user input conforms to
 * the mask, and that no portions are left empty.
 *
 * @param array $element The the structured text element, including values.
 * @param array $form_state The state of the form.
 */
function webform_validate_structured_text($element, $form_state) {
  // Gather the actual input.
  $chunks = array();
  foreach ( $element['#mask'] as $part => $details ) {
    if ( $details['type'] != 'markup' ) {
      $chunks[$part] = $element['part-' . $part]['#value'];
    }
  }
  // Determine if any input fields are missing.  If they're all empty, then they're not missing.
  $numb_chunks = count($chunks);
  $numb_not_empty = count(array_filter($chunks));
  $numb_empty = $numb_chunks - $numb_not_empty;
  $missing_chunks = ( $numb_chunks != $numb_not_empty && $numb_chunks != $numb_empty );
  // Now evaluate each text field.
  $mask_regex = array();
  if ( !empty($element['#mask_regex']) ) {
    $mask_regex_lines = array_filter(explode("\n", $element['#mask_regex']));
    foreach ( $mask_regex_lines as $line ) {
      list($chunk, $message, $regex) = explode('|', $line, 3);
      $mask_regex[$chunk] = array(
        'message' => $message,
        'regex' => $regex,
      );
    }
  }
  $component = 0;  // Used to point the user to the correct chunk of the whole input field in error messages.
  foreach ( $chunks as $part => $value ) {
    $component++;
    $error_1 = $error_n = '';
    switch ( $element['#mask'][$part]['type'] ) {
      case '9':
        if ( ( $element['#required'] || $missing_chunks ) && $value == '' ||
                $value != '' && (
                        preg_match('/[^0-9]/', $value) ||
                        drupal_strlen($value) < $element['#mask'][$part]['length'] ) ) {
          $error_1 = t('digit');
          $error_n = t('digits');
        }
        break;

      case 'a':
        if ( ( $element['#required'] || $missing_chunks ) && $value == '' ||
                $value != '' && (
                        preg_match('/[^a-z]/i', $value) ||
                        drupal_strlen($value) < $element['#mask'][$part]['length'] ) ) {
          $error_1 = t('alpha character (a-z)');
          $error_n = t('alpha characters (a-z)');
        }
        break;

      case 'x':
        if ( ( $element['#required'] || $missing_chunks ) && $value == '' ||
                $value != '' && ( drupal_strlen($value) < $element['#mask'][$part]['length'] ) ) {
          $error_1 = t('character');
          $error_n = t('characters');
        }
        break;

      case 'r': // r types are completely evaluated by the regex expression for validity
        break;
    }
    if ( $error_1 ) {
      $component_field = implode('][', array_merge($element['#parents'], array('part-' . $part,)));
      form_set_error($component_field,
              format_plural($element['#mask'][$part]['length'],
                      '%element field !part part must be 1 !singular.',
                      '%element field !part part must be @count !plural.',
                  array(
                    '%element' => $element['#title'],
                    '!part' => t($component . _webform_structured_text_ordinal_suffix($component)),
                    '!singular' => $error_1,
                    '!plural' => $error_n,
                  )));
    }
    elseif ( ( $element['#required'] || !$missing_chunks && $numb_not_empty != 0 || $element['#mask'][$part]['type'] == 'r' ) &&
            !empty($mask_regex[$component]['regex']) ) { // no errors so far, and we have a regex, so test it
      if ( !preg_match(trim($mask_regex[$component]['regex']), $value) ) {
        $component_field = implode('][', array_merge($element['#parents'], array('part-' . $part,)));
        form_set_error($component_field, strip_tags(_webform_structured_text_t(
                $element['#webform_component']['nid'] . ':' . $element['#webform_component']['cid'] . ":regex:$component:message",
                $mask_regex[$component]['message'])));
      }
    }
  }
}

/**
 * Theme a webform structured text element.
 */
function theme_webform_structured_text($variables) {
  $element = $variables['element'];
  $output = '<div class="webform-container-inline">' . drupal_render_children($element) . '</div>';
  return $output;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_structured_text($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_structured_text',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#field_prefix' => ( isset($component['extra']['field_prefix']) ? $component['extra']['field_prefix'] : '' ),
    '#field_suffix' => ( isset($component['extra']['field_suffix']) ? $component['extra']['field_suffix'] : '' ),
    '#component' => $component,
    '#format' => $format,
    '#value' => $value[0] != '' ? webform_structured_text_format_value($component['extra']['mask'], $value[0], $component, $format) : '',
    '#translatable' => array('title',),
  );
}


/**
 * Implements _webform_submit_component().
 */
function _webform_submit_structured_text($component, $value) {
  $combined_value = '';
  if ( ( $empty_test = implode('', $value) ) && !empty($empty_test) ) {
    $mask = webform_structured_text_parse_mask($component['extra']['mask']);
    foreach ( $value as $chunk => $input ) {
      $chunk = substr($chunk, 5);  // The array index starts with 'part-', so get rid of it.
      if ( $mask[$chunk]['type'] == 'r' ) {                // If this is a regex chunk...
        // Pad it to the full length so as not to merge anything that may have come after.
        $input = str_pad($input, $mask[$chunk]['length'], WFST_PAD_CHARACTER);
      }
      $combined_value .= $input;
    }
  }
  return $combined_value;
}

/**
 * Implements _webform_help_component().
 */
function _webform_help_structured_text($section) {
  switch ( $section ) {
    case 'admin/settings/webform#structured_text':
      return t('Allows creation of stuctured / input-masked text.');
  }
}


/**
 * Format the output of data for this component.
 */
function theme_webform_display_structured_text($variables) {
  $element = $variables['element'];
  $prefix = $element['#format'] == 'html' ? '' : $element['#field_prefix'];
  $suffix = $element['#format'] == 'html' ? '' : $element['#field_suffix'];
  $value = $element['#format'] == 'html' ? str_replace('&amp;nbsp;', '&nbsp;', check_plain($element['#value'])) : $element['#value'];
  return $value !== '' ? ($prefix . $value . $suffix) : ' ';
}

/**
 * Implements _webform_analysis_component.
 */
function _webform_analysis_structured_text($component, $sids = array(), $single = FALSE) {

  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('sid', 'data',))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid']);

  if ( count($sids) ) {
    $query->condition('sid', $sids, 'IN');
  }
  $nonblanks = 0;
  $submissions = 0;
  $responses = array();

  $result = $query->execute();
  foreach ( $result as $data ) {
    if ( drupal_strlen(trim($data['data'])) > 0 ) {
      $nonblanks++;
      if ( $component['extra']['analysis_display'] ) {
        if ( $component['extra']['analysis_display_identifying'] == '' ) {
          $identifier = $data['sid'];
        }
        else {
          $identifier = db_query('SELECT data FROM {webform_submitted_data} WHERE nid = :nid AND cid = :cid AND sid = :sid LIMIT 1',
                  array(
                    ':nid' => $component['nid'],
                    ':cid' => $component['extra']['analysis_display_identifying'],
                    ':sid' => $data['sid'],
                  ))->fetchField();
        }
      $responses[] = array('<div class="webform-response">' . l($identifier, "node/{$component['nid']}/submission/{$data['sid']}") . ': ' .
        webform_structured_text_format_value($component['extra']['mask'], $data['data'], $component) . '</div>', '');
      }
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);

  if ( $component['extra']['analysis_display'] ) {
    $rows = array_merge($rows, $responses);
  }

  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_structured_text($component, $value) {
  return ( empty($value[0]) ? '' : webform_structured_text_format_value($component['extra']['mask'], $value[0], $component) );
}

/**
 * Implements _webform_cvs_headers_component.
 */
function _webform_csv_headers_structured_text($component, $export_options) {
  return array('', '', $component['name'],);
}

/**
 * Implements _webform_cvs_data_component().
 */
function _webform_csv_data_structured_text($component, $export_options, $value) {
  return ( empty($value[0]) ? '' : webform_structured_text_format_value($component['extra']['mask'], $value[0], $component, 'text') );
}

/**
 * Helper function to parse a mask string into an array of mask parts.
 * @param string $mask The mask input by the user when the component was configured.
 * @return array Mask parts as a zero-indexed array, with the elements of the array
 *      itself an array of:
 *      'type' => one of '9', 'a', 'x', 'r', or 'markup',
 *      'length' => the length of that part of the mask, or max-length where 'type' == 'r',
 *      'value' => the actual markup in the case of 'type' == 'markup', empty otherwise,
 *      'size' => where 'type' == 'r', the optional size of the field.
 */
function webform_structured_text_parse_mask($mask) {
  $mask_array = array();
  $mask = (string) $mask;
  $mask_length = drupal_strlen($mask);
  $where = '';
  $part = -1;
  for ( $i = 0 ; $i < $mask_length ; $i++ ) {
    $type = ( in_array($mask[$i], array('9', 'a', 'x', 'r',)) ? $mask[$i] : 'markup' );
    if ( $where != $type ) {
      $where = $type;
      $mask_array[++$part] = array(
        'type' => $type,
        'length' => 0,
        'value' => '',
      );
    }
    if ( $type == 'r' ) { // digits after an 'r' mask give max length and size if not followed by a comma and other digits
      if ( preg_match('/[1-9]/', $mask[$i+1]) ) { // there's at least one digit
        $i++;
        while ( $i < $mask_length && preg_match('/[0-9]/', $mask[$i]) ) {
          $mask_array[$part]['length'] .= $mask[$i++];
        }
        $mask_array[$part]['length'] = (int) $mask_array[$part]['length'];
        if ( $i < $mask_length && $mask[$i] == ',' &&
                $i + 1 < $mask_length && preg_match('/[1-9]/', $mask[$i + 1]) ) { // a number after the commas specifies the display size of the field
          $i++;
          $mask_array[$part]['size'] = '';
          while ( $i < $mask_length && preg_match('/[0-9]/', $mask[$i]) ) {
            $mask_array[$part]['size'] .= $mask[$i++];
          }
          $mask_array[$part]['size'] = (int) $mask_array[$part]['size'];
        }
        $i--;  // back up the counter for regular expressions because it will be advanced by the for loop.
      }
      else {
        $mask_array[$part]['length'] = 1;
      }
    }
    else {
      $mask_array[$part]['length']++;
    }
    if ( $type == 'markup' ) {
      if ( $mask[$i] == '^' ) {
        if ( $i + 1 < $mask_length && in_array($mask[$i + 1], array('9', 'a', 'x', 'r', '^')) ) {
          $mask_array[$part]['value'] .= $mask[++$i];
        }
      }
      else {
        if ( $mask[$i] == ' ' ) {
          // Special treatment for spaces.  Start off a string of spaces with
          // a regular one, and then toggle between non-breaking and reagular
          // to achieve the spacing specified in the mask.
          if ( $i == 0 || $mask[$i-1] != ' ' ) { // Reset space character if it's the first in a series.
            $space = '&nbsp;';
          }
          $space = ( $space == ' ' ? '&nbsp;' : ' ' );
          $mask_array[$part]['value'] .= $space;
        }
        else {
          $mask_array[$part]['value'] .= $mask[$i];
        }
      }
    }
  }
  return $mask_array;
}

/**
 * Helper function to take a string value and parse it into an array based on a mask.
 * @param mixed $mask Either the string mask, or the mask array.
 * @param string $value The string value to be parsed.
 * @return array The string $value broken into parts in an array, indexed by the part location.
 */
function webform_structured_text_parse_value($mask, $value) {
  $mask_array = is_array($mask) ? $mask : webform_structured_text_parse_mask($mask);
  $value_array = array();
  $start = 0;
  foreach ( $mask_array as $part => $details ) {
    if ( $details['type'] != 'markup' ) {
      $value_array[$part] = str_replace(WFST_PAD_CHARACTER, '', drupal_substr($value, $start, $details['length']));
      $start += $details['length'];
    }
  }
  return $value_array;
}

/**
 * Helper function to format a value per a mask.
 * @param mixed $mask Either the string mask, or the mask array.
 * @param string $value The value to be formatted.
 * @param array $component The component array itself.
 * @param string $format What format (HTML or text) should be returned.
 *        Specifying 'html' forces a pass of the output through check_plain.
 * @return string The formatted value.
 */
function webform_structured_text_format_value($mask, $value, $component, $format = 'html') {
  $mask_array = is_array($mask) ? $mask : webform_structured_text_parse_mask($mask);
  $value = webform_structured_text_parse_value($mask_array, $value);
  if ( ( $empty_test = implode('', $value) ) && ( empty($empty_test) ) ) {
    return '';
  }
  $output = '';
  foreach ( $mask_array as $part => $details ) {
    if ( $details['type'] != 'markup' ) {
      $output .= $value[$part];
    }
    else {
      $output .= _webform_structured_text_t("{$component['nid']}:{$component['cid']}:mask:$part", $details['value']);
    }
  }
  return ( $format == 'html' ? str_replace('&amp;nbsp;', '&nbsp;', check_plain($output)) : $output );
}

/**
 * Helper function to generate the description for the control, optionally indluding
 * input format instructions / description.
 * @param type $component
 * @return string The component's description
 */
function webform_structured_text_field_description($component) {
  $description = $component['extra']['description'];
  if ( $component['extra']['mask_description'] ) {
    $mask_array = webform_structured_text_parse_mask($component['extra']['mask']);
    $output = array();
    foreach ( $mask_array as $details ) {
      switch ( $details['type'] ) {
        case '9':
          $output[] = format_plural($details['length'], '1 digit', '@count digits');
          break;
        case 'a':
          $output[] = format_plural($details['length'], '1 alpha (a-z) character', '@count alpha (a-z) characters');
          break;
        case 'x':
        case 'r':
          $output[] = format_plural($details['length'], '1 character', '@count characters');
          break;
        default:
          break;
      }
    }
    if ( !empty($output) ) {
      $last = array_pop($output);
      $output = ( !empty($output) ? implode(', ', $output) . ', ' . t('and') . ' ' . $last : $last );
      $description .= ' <strong>' . t('Input format:') . '</strong> ' . $output . '.';
    }
  }
  return $description;
}

/**
 * Helper function to display the ordinal ending for an integer.
 * @param int $num The number for which the ordinal ending is desired.
 * @return string The ordinal ending.
 */
function _webform_structured_text_ordinal_suffix($num){
  if ( $num < 4 || $num > 20 ){
    switch ( $num % 10 ) {
      case 1: return 'st';
      case 2: return 'nd';
      case 3: return 'rd';
    }
  }
  return 'th';
}

/**
 * Dummy function to help with translation.
 */
function _webform_translate_dummy() {
  t('1st');
  t('2nd');
  t('3rd');
  t('4th');
  t('5th');
  t('6th');
  t('7th');
  t('8th');
  t('9th');
  t('10th');
  t('11th');
  t('12th');
  t('13th');
  t('14th');
  t('15th');
  t('16th');
  t('17th');
  t('18th');
  t('19th');
  t('20th');
  // More than 20 parts and you're on your own for translating that.
}

/**
 * Implements hook_webform_component_insert.
 */
function webform_structured_text_webform_component_insert($component) {
  webform_structured_text_i18n_update_strings($component);
}

/**
 * Implements hook_webform_component_update.
 */
function webform_structured_text_webform_component_update($component) {
  webform_structured_text_i18n_update_strings($component);
}

/**
 * Implements hook_webform_component_delete.
 */
function webform_structured_text_webform_component_delete($component) {
  webform_structured_text_i18n_update_strings($component, 'remove');
}

