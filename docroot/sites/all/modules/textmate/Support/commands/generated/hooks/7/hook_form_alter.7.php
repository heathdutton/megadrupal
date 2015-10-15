/**
 * Implements hook_form_alter().
 */
function <?php print $basename; ?>_form_alter(&\$form, &\$form_state, \$form_id) {
  ${1:if (isset(\$form['type']) && \$form['type']['#value'] . '_node_settings' == \$form_id) {
    \$form['workflow']['upload_' . \$form['type']['#value']] = array(
      '#type' => 'radios',
      '#title' => t('Attachments'),
      '#default_value' => variable_get('upload_' . \$form['type']['#value'], 1),
      '#options' => array(t('Disabled'), t('Enabled')),
    );
  \}}
}

$2