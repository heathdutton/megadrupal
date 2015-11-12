<?php

/**
 * @file AUL grants field.
 */

/**
 * Class for AUL grants field.
 */
class views_handler_aul_grants_field extends views_handler_field {

  /**
   * Overrides views_handler_filter::options_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['label'] = array('default' => t('Grants'));
    $options['render_label'] = array('default' => TRUE);
    $options['render_label_collon'] = array('default' => TRUE);
    $options['aul_source'] = array('default' => AUL_DEFAULT_SOURCE);

    return $options;
  }
  
  /**
   * Overrides views_handler_filter::options_form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['aul_source'] = array(
      '#type' => 'textfield',
      '#title' => t('AUL source'),
      '#description' => t('Name of AUL source. Default value is "@source"', array(
        '@source' => AUL_DEFAULT_SOURCE,
      )),
      '#required' => TRUE,
      '#default_value' => $this->options['aul_source'],
    );
    
    return $form;
  }

  /**
   * Overrides views_handler_field::render().
   */
  public function render($values) {
    $nid = $values->nid;
    $source = $this->options['aul_source'];
    $operations = array(
      'view' => t('View'),
      'update' => t('Update'),
      'delete' => t('Delete'),
    );

    // Get user grants for node grouped by user id.
    $results = aul_get_uids_with_grants($nid, $source);

    // Format output string.
    $output = array();
    if ($results) {
      foreach ($results as $uid => $grants) {
        $node_grants = array();
        foreach ($operations as $op => $operation_name) {
          if (!empty($grants[$op])) {
            $node_grants[$op] = $operation_name;
          }
        }
        $account = user_load($uid);
        if($node_grants) {
          $node_grants_text = implode(', ', $node_grants);
          $output[] = "{$account->name}({$uid}): {$node_grants_text}";
        }
      }
    }

    return implode('<br/>', $output);
  }

  /**
   * @inheritdoc
   */
  public function query() {}

}
