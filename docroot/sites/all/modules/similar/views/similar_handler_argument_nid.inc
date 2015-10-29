<?php

/**
 * @file
 * Defines the Similar entries node ID argument.
 */

/**
 * Defines the similar entries View node ID argument.
 */
class similar_handler_argument_nid extends views_handler_argument_numeric {

  /**
   * Defines default values for argument settings.
   */
  public function option_definition() {
    $common_words = array('-not', '-and', '-a', '-or', '-the', '-but', '-therefore',
      '-because', '-can\'t', '-can', '-for', '-until', '-the', '-of', '-and', '-to',
      '-in', '-is', '-you', '-that', '-it', '-he', '-was', '-on', '-are', '-as', '-with',
      '-his', '-they', '-I', '-at', '-be', '-this', '-have', '-from', '-one', '-had',
      '-by', '-word', '-but', '-not', '-what', '-all', '-were', '-we', '-when', '-your',
      '-said', '-there', '-use', '-an', '-each', '-which', '-she', '-do', '-how',
      '-their', '-if', '-will', '-up', '-other', '-about', '-out', '-many', '-then',
      '-them', '-these', '-so', '-some', '-her', '-would', '-make', '-like', '-him',
      '-into', '-time', '-has', '-look', '-two', '-more', '-write', '-go', '-see', '-no',
      '-way', '-could', '-people', '-my', '-than', '-first', '-water', '-been', '-call',
      '-who', '-oil', '-its', '-now', '-find');

    $options = parent::option_definition();
    $options['default_action'] = array('default' => 'default');
    $options['default_argument_type'] = array('default' => 'node');
    $options['validate']['contains']['type'] = array('default' => 'node');
    $options['boolean_mode'] = array('default' => 0);
    $options['adjust_relevance'] = array('default' => 1);
    $options['title_relevance'] = array('default' => 1.4);
    $options['body_relevance'] = array('default' => 0.8);
    $options['source_relevance'] = array('default' => 1);
    $options['title_operator'] = array('default' => 'increase');
    $options['enable_custom_operators'] = array('default' => 0);
    $options['custom_operators'] = array('default' => implode(' | ', $common_words));
    if (module_exists('field')) {
      $options['include_fields'] = array('default' => 0);

      // Notify Views of individual field weights.
      // Note: the Views cache may need to be reset when fields are added.
      foreach (self::get_field_options() as $table => $field) {
        $options['included_fields:'. $table] = array('default' => 1);
      }
    }
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['not']);
    $form['default_argument_type']['#disabled'] = TRUE;

    $form['similar'] = array(
      '#type' => 'fieldset',
      '#title' => t('FULLTEXT search options'),
    );

    // Add fields for increasing or decreasing relevance of destination ndoe fields.
    $relevance_options = array(
        '0' => '0%',
        '0.1' => '10%',
        '0.2' => '20%',
        '0.3' => '30%',
        '0.4' => '40%',
        '0.5' => '50%',
        '0.6' => '60%',
        '0.7' => '70%',
        '0.8' => '80%',
        '0.9' => '90%',
        '1.0' => '100%',
        '1.1' => '110%',
        '1.2' => '120%',
        '1.3' => '130%',
        '1.4' => '140%',
        '1.5' => '150%',
        '1.6' => '160%',
        '1.7' => '170%',
        '1.8' => '180%',
        '1.9' => '190%',
        '2.0' => '200%',
    );
    $form['adjust_relevance'] = array(
      '#type' => 'checkbox',
      '#title' => t('Adjust relevance of fields'),
      '#default_value' => $this->options['adjust_relevance'],
      '#description' => t('Adjust the relevance of content titles, bodies, and other fields in matching.'),
      '#fieldset' => 'similar',
    );
    $form['title_relevance'] = array(
      '#type' => 'select',
      '#title' => t('Relevance of content\'s title.'),
      '#options' => $relevance_options,
      '#default_value' => $this->options['title_relevance'],
      '#description' => t('Increase or decrease the relevance of matches in destination content\'s title field.'),
      '#fieldset' => 'similar',
      '#dependency' => array('edit-options-adjust-relevance' => array('1')),
    );
    $form['body_relevance'] = array(
      '#type' => 'select',
      '#title' => t('Relevance of content\'s body.'),
      '#options' => $relevance_options,
      '#default_value' => $this->options['body_relevance'],
      '#description' => t('Increase or decrease the relevance of matches in '.
                          'destination content\'s body and other fields.'),
      '#fieldset' => 'similar',
      '#dependency' => array('edit-options-adjust-relevance' => array('1')),
    );

    // Add fields for enabling matching in fields defined by field module.
    if (module_exists('field')) {
      $field_options = self::get_field_options();

      $form['include_fields'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include content fields in matching'),
        '#default_value' => !empty($this->options['include_fields']),
        '#description' => t('Enable FULLTEXT queries on fields defined with Field module.'),
        '#fieldset' => 'similar',
      );
      foreach ($field_options as $table => $field) {
        $form['included_fields:'. $table] = array(
          '#type' => 'select',
          '#title' => $field,
          '#options' => $relevance_options,
          '#default_value' => isset($this->options['included_fields:'. $table]) ? $this->options['included_fields:'. $table] : 1,
          '#fieldset' => 'similar',
          '#dependency' => array(
            'edit-options-include-fields' => array('1'),
            'edit-options-adjust-relevance' => array('1'),
          ),
          '#dependency_count' => 2,
        );
      }
    }

    $form['boolean_mode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Execute search in boolean mode'),
      '#default_value' => !empty($this->options['boolean_mode']),
      '#fieldset' => 'similar',
      '#description' => t('Boolean mode enables the ability to increase or decrease the relevance of certain fields.<br />'.
                          'Note: Boolean mode may negatively affect the accuracy of view results.'),
    );

    // Add options for increasing or decreasing relevance of source node.
    $form['source_relevance'] = array(
      '#type' => 'checkbox',
      '#title' => t('Adjust source relevance'),
      '#default_value' => !empty($this->options['source_relevance']),
      '#description' => t('Increase the relevance of words in the title of the content being viewed.'),
      '#fieldset' => 'similar',
      '#dependency' => array('edit-options-boolean-mode' => array('1')),
    );
    $form['title_operator'] = array(
      '#type' => 'select',
      '#title' => t('Relevance of source content\'s title'),
      '#options' => array(
        'none' => t('No change'),
        'increase' => t('Increase title relevance'),
        'decrease' => t('Decrease title relevance'),
        'require_all' => t('Require all terms'),
        'require_some' => t('Require at least one term'),
        'require_complete' => t('Require complete title'),
        'exclude_all' => t('Exclude all terms'),
        'exclude_some' => t('Exclude at least one term'),
        'exclude_complete' => t('Exclude complete title'),
      ),
      '#default_value' => $this->options['title_operator'],
      '#fieldset' => 'similar',
      '#dependency' => array(
        'edit-options-boolean-mode' => array('1'),
        'edit-options-source-relevance' => array('1'),
      ),
      '#dependency_count' => 2,
    );

    // Add fields for creating custom relevance rules for specific words or phrases.
    $form['enable_custom_operators'] = array(
      '#type' => 'checkbox',
      '#title' => t('Set custom operators in words and phrases'),
      '#default_value' => !empty($this->options['enable_custom_operators']),
      '#fieldset' => 'similar',
      '#dependency' => array('edit-options-boolean-mode' => array('1')),
    );
    $form['custom_operators'] = array(
      '#type' => 'textfield',
      '#title' => t('Operators'),
      '#default_value' => $this->options['custom_operators'],
      '#description' => t('Use boolean logical operators to customize the FULLTEXT query. Each expression should '.
                          'be separated by a vertical bar |.<br />'.
                          'Available operators:  +  -  >  <  (  )  ~  "<br />'.
                          'See http://dev.mysql.com/doc/refman/5.5/en/fulltext-boolean.html for more information.'),
      '#maxlength' => 1024,
      '#fieldset' => 'similar',
      '#dependency' => array(
        'edit-options-boolean-mode' => array('1'),
        'edit-options-enable-custom-operators' => array('1'),
      ),
      '#dependency_count' => 2,
    );
  }

  /**
   * Overrides argument's default actions list to ensure that node ID
   * from URL is provided.
   */
  public function default_actions($which = NULL) {
    $defaults = parent::default_actions();
    unset($defaults['ignore'], $defaults['not found'], $defaults['summary'], $defaults['empty']);

    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }

  /**
   * Validates that the argument works.
   */
  public function validate_arg($arg) {
    if (!parent::validate_arg($arg)) {
      return FALSE;
    }

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }
    $this->view->nids = $this->value;
    return TRUE;
  }
  
  /**
   * Override the behavior of title(). Get the title of the node.
   */
  function title_query() {
    $titles = array();
  
    $result = db_query("SELECT n.title FROM {node} n WHERE n.nid IN (:nids)", array(':nids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }

  /**
   * Builds the query.
   */
  public function query($group_by = FALSE) {
    $boolean = !empty($this->options['boolean_mode']);
    $text = '';

    // Since the view could have multiple nid arguments, load each node
    // and populate the $text variable with node titles and bodies.
    foreach ($this->value as $nid) {
      $node = node_load($nid);
      if (isset($node->title) && isset($node->body) && isset($node->body[$node->language])) {
        // Remove punctuation from the title.
        $title = preg_replace('/[^a-z0-9 _-]+/i', '', $node->title);

        // Alter the relevancy of words in the node title if option is selected.
        if ($boolean && !empty($this->options['source_relevance']) && isset($this->options['title_operator'])) {
          $title = $this->alter_node_title($title);
        }

        // Strip tags and add slashes only to the body before adding the title.
        $body = trim(addslashes(strip_tags($node->body[$node->language][0]['value'])));
        $text .= " $title $body";
      }
    }

    // Perform replacements to add custom operators to node body and title words.
    if ($boolean && !empty($this->options['enable_custom_operators']) && !empty($this->options['custom_operators'])) {
      $replacements = array_map('trim', explode('|', $this->options['custom_operators']));
      $words = array_map('similar_handler_argument_nid::get_search_word', $replacements);
      $text = preg_replace($words, $replacements, $text);
    }

    $text = trim($text);
    $this->ensure_my_table();

    $query = $boolean ? $this->similar_build_query($text, 'IN BOOLEAN MODE') : $this->similar_build_query($text);
    
    // An empty string is passed for the table to bypass the table alias.
    $this->query->add_field('', $query, 'score');

    // Exclude the current node(s).
    if (count($this->value) > 1) {
      $this->query->add_where(0, 'node.nid', $this->value, 'NOT IN');
    }
    else {
      $this->query->add_where(0, 'node.nid', $this->value[0], '<>');
    }

    // Add a node_access tag to prevent conflict with node_access module.
    $this->query->add_tag('node_access');

    // Add a Similar Entries tag to indicate that filters and sort handlers are valid.
    $this->query->add_tag('similar_entries');
  }

  /**
   * Builds a query from argument configuration options.
   * @param $text
   *   The text of the current node to be used in the query.
   * @param $modifier
   *   An optional FULLTEXT search modifer. ie 'IN BOOLEAN MODE'.
   * @see similar_handler_argument_nid::query()
   */
  public function similar_build_query($text, $modifier = '') {
    $query = array();
    if (!empty($modifier)) {
      $modifier = " $modifier";
    }

    $weight = !empty($this->options['adjust_relevance']) && isset($this->options['title_relevance'])
      ? $this->options['title_relevance'] : 1;
    $query[] = "($weight * (MATCH(node.title) AGAINST('$text'$modifier)))";

    // The body field is always included in the query if it exists.
    if (db_table_exists('field_data_body')) {
      $weight = !empty($this->options['adjust_relevance']) ? $this->options['body_relevance'] : 1;
      $body_table = $this->query->add_table('field_data_body');

      if ($weight !== 0) {
        $query[] = "($weight * (MATCH($body_table.body_value) AGAINST('$text'$modifier)))";
      }
    }

    if (module_exists('field') && !empty($this->options['include_fields'])) {
      $indices = similar_get_indices();
      foreach (self::get_field_options() as $table => $value) {
        $weight = !empty($this->options['adjust_relevance']) && isset($this->options['included_fields:'. $table])
          ? $this->options['included_fields:'. $table] : 1;
        $alias = $this->query->ensure_table($table);
        $fields = "$alias." . implode(", $alias.", $indices[$table]);
        $query[] = "($weight * (MATCH($fields) AGAINST('$text'$modifier)))";
      }
    }
    // Return a query that calculates an average score to prevent scores from fluctuating too much.
    // We multiply the score by different amounts depending on the modifier used.
    return "((". implode(" + ", $query) . ") / ". count($query) .")";
  }

  /**
   * Converts a user-defined word with operator into a search word for use
   * in preg_replace().
   *
   * @see similar_handler_argument_nid::query()
   */
  public static function get_search_word($word) {
    return '/\b'. trim($word, '+-<>~()"') .'\b/i';
  }

  /**
   * Alters the node title's relevance for boolean searches.
   *
   * @param $title
   *   The node title with punctuation removed.
   * @param $operator
   *   The operator to apply to the node title.
   * @see similar_handler_argument_nid::query()
   */
  private function alter_node_title($title) {
    switch ($this->options['title_operator']) {
      case 'increase':
        return '>'. implode(' >', explode(' ', $title));
      case 'decrease':
        return '<'. implode(' <', explode(' ', $title));
      case 'require_all':
        return '+'. implode(' +', explode(' ', $title));
      case 'require_some':
        return "+($title)";
      case 'require_complete':
        return '"'. $title .'"';
      case 'exclude_all':
        return '-'. implode(' -', explode(' ', $title));
      case 'exclude_some':
        return "-($title)";
      case 'exclude_complete':
        return '-("'. $title .'")';
      default:
        return $title;
    }
  }

  /**
   * Returns an array of fields that have been indexed by Similar Entries.
   */
  private static function get_field_options() {
    $field_options = array();
    $indices = similar_get_indices();
    foreach (field_info_fields() as $field => $info) {
      if ($info['type'] == 'text') {
        $table = key($info['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
        if (isset($indices[$table])) {
          $field_options[$table] = $field;
        }
      }
    }
    return $field_options;
  }

}
