/**
 * Implements hook_form_BASE_FORM_ID_alter().
 */
function <?php print $basename; ?>_form_BASE_FORM_ID_alter(&\$form, &\$form_state, \$form_id) {
  ${1:// Modification for the form with the given BASE_FORM_ID goes here. For
  // example, if BASE_FORM_ID is "node_form", this code would run on every
  // node form, regardless of node type.

  // Add a checkbox to the node form about agreeing to terms of use.
  \$form['terms_of_use'] = array(
    '#type' => 'checkbox',
    '#title' => t("I agree with the website's terms and conditions."),
    '#required' => TRUE,
  );}
}

$2