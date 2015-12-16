<?php

/**
 * @file AUL grants field.
 */

/**
 * Class for AUL grants field.
 */
class views_handler_aul_user_grants_field extends views_handler_field {

  function pre_render(&$values) {
    parent::pre_render($values);
    
    drupal_add_css(drupal_get_path('module', 'aul_views') .'/styles/aul_views.css');
  }
  
  function admin_summary() {
    $aul_user_label = '';
    if (!empty($this->options['aul_user'])) {
      $aul_user = $this->options['aul_user'];
      $aul_user_label = ' =' . $aul_user['entity_label'] . ' (' . $aul_user['entity_id'] . ')';
    }
    
    return $this->label() . $aul_user_label;
  }
  
  /**
   * Overrides views_handler_filter::options_definition().
   */
  function option_definition() {

    $options = parent::option_definition();

    $options['label'] = array('default' => t('Grants'));
    $options['render_label'] = array('default' => TRUE);
    $options['render_label_collon'] = array('default' => TRUE);

    $options['aul_user'] = array('default' => NULL);
    $options['aul_source'] = array('default' => AUL_DEFAULT_SOURCE);

    return $options;
  }
  
  /**
   * Overrides views_handler_filter::options_form.
   */
  public function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);
    
    $form['aul_user'] = array(
      '#type' => 'entityreference',
      '#title' => t('User'),
      '#description' => t('Choose user for access context. Default value - current logged in user.'),
      '#era_entity_type' => 'user',
      '#era_cardinality' => 1,
      '#default_value' => (int) $this->options['aul_user']['entity_id'],
    );

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
    
    $uid = $this->aul['uid'];
    $username = $this->aul['username'];
    $nid = $values->nid;
    $source = $this->options['aul_source'];
    $node_grants = array();
    $output = '';
    $operations = array(
      'view' => t('View'),
      'update' => t('Update'),
      'delete' => t('Delete'),
    );

    // If user can bypass node access, he always has all the grants.
    if (user_access('bypass node access', user_load($uid))) {
      $grants = array(
        'view' => 1,
        'update' => 1,
        'delete' => 1,
      );
    }
    // Get user grants for node grouped by user id.
    else {
      $results = aul_get_aul(array(
        'key_id' => $uid,
        'key_name' => 'uid',
        'nid' => $nid,
        'source' => $source,
      ));
      $grants = array();
      foreach ($results as $result) {
        $grants[$result->op] = $result->value;
      }
    }

    // Format output string.
    if ($grants) {
      foreach ($operations as $op => $operation_name) {
        if (!empty($grants[$op])) {
          $node_grants[$op] = $operation_name;
        }
      }
      if($node_grants) {
        $node_grants_text = implode(', ', $node_grants);
        $output = "{$username}({$uid}): {$node_grants_text}";
      }
    }
    
    // @todo: Next code works but we need to check if we need it.
    // If there are no AUL grants but node in query get other grants form the
    // node_access table.
    else {
      $account = user_load($uid);
      
      $operations = array(
        'view' => t('View'),
        'update' => t('Update'),
        'delete' => t('Delete'),
      );
      
      static $results = array();
      
      foreach ($operations as $op => $operation_name) {
        
        if(empty($results[$op])) {

          // Get actual user grants from node access table.
          $grants = node_access_grants($op, $account);

          $query = db_select('node_access', 'na')
           ->fields('na', array('nid'));

          $grant_conditions = db_or();
          // If any grant exists for the specified user, then user has access
          // to the node for the specified operation.
          foreach ($grants as $realm => $gids) {
            if(empty($gids)) {
              continue;
            }
            if(is_array($gids)) {
              foreach ($gids as $gid) {
                $grant_conditions->condition(db_and()
                  ->condition('na.gid', $gid)
                  ->condition('na.realm', $realm)
                );
              }
            }
            else {
              $grant_conditions->condition(db_and()
                ->condition('na.gid', $gids)
                ->condition('na.realm', $realm)
              );
            }
          }
          // Attach conditions to the query for nodes.
          if (count($grant_conditions->conditions())) {
            $query->condition($grant_conditions);
          }
          $query->condition('na.grant_' . $op, 1, '>=');

          $results[$op] = $query->execute()->fetchCol('nid');
        }
        
        if(!empty($results[$op]) && in_array($nid, $results[$op])){
          $node_grants[] = $operation_name;
        }
      }
      
      if($node_grants) {
        $node_grants_text = implode(', ', $node_grants);
        $output = "<div class=\"aul-grants-default\">{$username}({$uid}) " . t('fixed permissions') . ": {$node_grants_text}</div>";
      }
    }
    
    return $output;
  }

  /**
   * Overrides views_handler_field::query.
   */
  public function query() {
    // Get the user context once in query and save it to view.
    $this->aul = _aul_views_get_user_context($this);
  }

}
