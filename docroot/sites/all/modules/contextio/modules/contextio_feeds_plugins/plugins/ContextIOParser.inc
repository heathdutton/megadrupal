<?php

/**
 * @file
 * ContextIOParser related classes.
 */

define('CONTEXTIOPARSER_CONSTANT_VALUE', 1);
define('CONTEXTIOPARSER_CONSTANT_VALUE_NAME', t('Constant value'));
define('CONTEXTIOPARSER_SIMPLE_REPLACE', 2);
define('CONTEXTIOPARSER_SIMPLE_REPLACE_NAME', t('Simple string replace'));
define('CONTEXTIOPARSER_REGEX_REPLACE', 3);
define('CONTEXTIOPARSER_REGEX_REPLACE_NAME', t('String replace with Regex'));
define('CONTEXTIOPARSER_ADD_DERIVATIVE', 'add_derivative');
define('CONTEXTIOPARSER_EDIT_DERIVATIVE', 'edit_derivative');
define('CONTEXTIOPARSER_DELETE_DERIVATIVE', 'delete_derivative');

/**
 * Context.IO specific abstract parser plugin.
 */
abstract class ContextIOParser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    // Get concrete parser's result.
    $result = $this->contextIOParse($source, $fetcher_result);
    // Add derivative sources to result items.
    if (!empty($result->items)) {
      foreach ($result->items as &$item) {
        $this->addDerivativeSources($item);
      }
    }
    return $result;
  }

  /**
   * Concrete parser specific parse() method.
   *
   * Allow concrete parsers to implement their own parse() method without
   * overriding parent's one.
   *
   * @see parse()
   */
  protected abstract function contextIOParse(FeedsSource $source, FeedsFetcherResult $fetcher_result);

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $config = array(
      'derivative_sources' => array(),
    );
    // Concrete parser's config defaults.
    $config += $this->contextIOConfigDefaults();
    return $config;
  }

  /**
   * Concrete parser specific configDefault() method.
   *
   * Allow concrete parsers to implement their own configDefaults() method
   * without overriding parent's one.
   *
   * @see configDefaults()
   */
  protected function contextIOConfigDefaults() {
    return array();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();

    // Allowed derivative source operations.
    $derivative_source_ops = array(
      CONTEXTIOPARSER_ADD_DERIVATIVE,
      CONTEXTIOPARSER_EDIT_DERIVATIVE,
      CONTEXTIOPARSER_DELETE_DERIVATIVE,
    );

    // Add a derivatives related configuration forms.
    if (!empty($_GET['op']) && in_array($_GET['op'], $derivative_source_ops)) {
      // Add new derivative source form.
      if ($_GET['op'] == CONTEXTIOPARSER_ADD_DERIVATIVE) {
        $this->derivativeSourceForm($form, $_GET['op']);
      }
      // Edit and delete derivative source forms.
      elseif (!empty($_GET['id']) && array_key_exists($_GET['id'], $this->config['derivative_sources'])) {
        // Delete operation.
        if ($_GET['op'] == CONTEXTIOPARSER_DELETE_DERIVATIVE) {
          // Check derivative dependents in case of delete operation.
          $dependents = $this->getDerivativeSourceDependents($this->config['derivative_sources'][$_GET['id']]);
          // No dependents found, delete operation allowed.
          if (empty($dependents['source'])) {
            $this->derivativeSourceForm($form, $_GET['op'], $this->config['derivative_sources'][$_GET['id']]);
          }
          // Delete operation not allowed, list derivative source configs.
          else {
            $this->derivativeSources($form);
          }
        }
        // Edit operation.
        else {
          $this->derivativeSourceForm($form, $_GET['op'], $this->config['derivative_sources'][$_GET['id']]);
        }
      }
      else {
        $this->derivativeSources($form);
      }
    }
    // List derivative source configurations.
    else {
      $this->derivativeSources($form);
    }

    // Concrete parser's config form elements.
    $form += $this->contextIOConfigForm($form_state);

    return $form;
  }

  /**
   * Concrete parser specific configForm() method.
   *
   * Allow concrete parsers to implement their own configForm() method
   * without overriding parent's one.
   *
   * @see configForm()
   */
  protected function contextIOConfigForm(&$form_state) {
    return array();
  }

  /**
   * Constructs a derivative source related configuration form.
   *
   * @param array $form
   *   The basic config form the derivative source form will be added to.
   * @param string $op
   *   The operation type of the form. The following values are available:
   *   - CONTEXTIOPARSER_ADD_DERIVATIVE: Add derivative form.
   *   - CONTEXTIOPARSER_EDIT_DERIVATIVE: Edit derivative form.
   *   - CONTEXTIOPARSER_DELETE_DERIVATIVE: Delete derivative form.
   * @param array $derivative
   *   An array of derivative properties.
   */
  protected function derivativeSourceForm(array &$form, $op = CONTEXTIOPARSER_ADD_DERIVATIVE, array $derivative = array()) {
    $form['#tree'] = TRUE;
    // Add-edit derivative source form.
    if (($op == CONTEXTIOPARSER_ADD_DERIVATIVE) || (($op == CONTEXTIOPARSER_EDIT_DERIVATIVE) && !empty($derivative))) {
      // Construct an array of available mapping sources. Initialize with all
      // sources including derivatives.
      $available_sources = $this->getMappingSources();
      // Limit available sources for edited derivatives.
      if ($op == CONTEXTIOPARSER_EDIT_DERIVATIVE) {
        foreach ($this->config['derivative_sources'] as $key => $derivative_source) {
          // Skip the derivative source if its num is greater or equal than the
          // num of the currently edited derivative source.
          if ($derivative['num'] <= $derivative_source['num']) {
            unset($available_sources[$key]);
          }
        }
      }

      // Construct source options.
      $source_options = array();
      foreach ($available_sources as $key => $source) {
        $source_options[$key] = $source['name'] . ' (' . $key . ')';
      }

      $form['derivative_source'] = array(
        '#type' => 'fieldset',
        '#title' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? t('Add derivative source') : t('Edit derivative source'),
        '#collapsed' => FALSE,
        '#collapsible' => FALSE,
      );
      // Store operation type for later usage.
      $form['derivative_source']['op'] = array(
        '#type' => 'value',
        '#value' => $op,
      );
      // Store original derivative settings for later usage.
      $form['derivative_source']['original'] = array(
        '#type' => 'value',
        '#value' => $derivative,
      );

      $form['derivative_source']['num'] = array(
        '#type' => 'value',
        '#value' => empty($derivative['num']) ? 0 : $derivative['num'],
      );
      $form['derivative_source']['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['name'],
        '#description' => t('The name of the derivative.'),
        '#required' => TRUE,
      );
      $form['derivative_source']['machine_name'] = array(
        '#type' => 'machine_name',
        '#machine_name' => array(
          'source' => array('derivative_source', 'name'),
          'exists' => 'contextio_feeds_plugins_derivative_source_exists',
        ),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['machine_name'],
        '#description' => t('The machine-readable name the derivative.'),
        '#used_machine_names' => array_keys($this->getMappingSources()),
        '#disabled' => ($op == CONTEXTIOPARSER_EDIT_DERIVATIVE),
      );
      $form['derivative_source']['description'] = array(
        '#type' => 'textfield',
        '#title' => t('Description'),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['description'],
        '#description' => t('A short description of the derivative.'),
        '#maxlength' => 255,
      );
      $form['derivative_source']['type'] = array(
        '#type' => 'radios',
        '#title' => t('Type'),
        '#options' => array(
          CONTEXTIOPARSER_CONSTANT_VALUE => CONTEXTIOPARSER_CONSTANT_VALUE_NAME,
          CONTEXTIOPARSER_SIMPLE_REPLACE => CONTEXTIOPARSER_SIMPLE_REPLACE_NAME,
          CONTEXTIOPARSER_REGEX_REPLACE => CONTEXTIOPARSER_REGEX_REPLACE_NAME,
        ),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? CONTEXTIOPARSER_SIMPLE_REPLACE : $derivative['type'],
        '#required' => TRUE,
      );

      $form['derivative_source']['source'] = array(
        '#type' => 'select',
        '#title' => t('Source') . ' ' . theme('form_required_marker', array()),
        '#options' => $source_options,
        '#empty_option' => t('- Select -'),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['source'],
        '#description' => t('The source of the derivative. Derivative sources with lower numbers could be sources as well.'),
        '#states' => array(
          'invisible' => array(
            ':input[name="derivative_source[type]"]' => array('value' => CONTEXTIOPARSER_CONSTANT_VALUE),
          ),
        ),
      );

      // Type: Constant value.
      $form['derivative_source']['value'] = array(
        '#type' => 'textfield',
        '#title' => t('Value') . ' ' . theme('form_required_marker', array()),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['value'],
        '#description' => t("The value of the derivative. May contain replacement patterns."),
        '#states' => array(
          'visible' => array(
            ':input[name="derivative_source[type]"]' => array('value' => CONTEXTIOPARSER_CONSTANT_VALUE),
          ),
        ),
      );
      // Type: Simple string replace.
      $form['derivative_source']['search_string'] = array(
        '#type' => 'textfield',
        '#title' => t('Search string') . ' ' . theme('form_required_marker', array()),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['search_string'],
        '#description' => t("The string value being searched for."),
        '#states' => array(
          'visible' => array(
            ':input[name="derivative_source[type]"]' => array('value' => CONTEXTIOPARSER_SIMPLE_REPLACE),
          ),
        ),
      );
      $form['derivative_source']['replacement'] = array(
        '#type' => 'textfield',
        '#title' => t('Replacement'),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['replacement'],
        '#description' => t(
          "The replacement string that replaces all occurrences of the search string. Leave empty to remove matched strings.
           May contain replacement patterns."
        ),
        '#states' => array(
          'visible' => array(
            ':input[name="derivative_source[type]"]' => array('value' => CONTEXTIOPARSER_SIMPLE_REPLACE),
          ),
        ),
      );
      // Type: String replace / extraction using regular expressions.
      $form['derivative_source']['regex_pattern'] = array(
        '#type' => 'textfield',
        '#title' => t('Regex pattern') . ' ' . theme('form_required_marker', array()),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['regex_pattern'],
        '#description' => t(
          "The regex pattern being searched for. Parenthesis can be used for references in the replacement field.
          For example /$([0-9]+?)/ will match strings that begin with numbers and the numbers will be captured by
          the first ($1) parenthesis pair."
        ),
        '#states' => array(
          'visible' => array(
            ':input[name="derivative_source[type]"]' => array('value' => CONTEXTIOPARSER_REGEX_REPLACE),
          ),
        ),
      );
      $form['derivative_source']['regex_replacement'] = array(
        '#type' => 'textfield',
        '#title' => t('Replacement'),
        '#default_value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? '' : $derivative['regex_replacement'],
        '#description' => t(
          "The replacement string that replaces all occurrences of the regex pattern. Leave empty to remove matched patterns.
           May contain replacement patterns or regex pattern component references of the form \$n or \${n}. Every such reference
           will be replaced by the text captured by the n'th parenthesized pattern in the pattern field. n can be from 0 to 99,
           and \$0 or \${0} refers to the text matched by the whole pattern."
        ),
        '#states' => array(
          'visible' => array(
            ':input[name="derivative_source[type]"]' => array('value' => CONTEXTIOPARSER_REGEX_REPLACE),
          ),
        ),
      );

      // Used replacement patterns (see configFormValidate() for details).
      $form['derivative_source']['used_replacement_patterns'] = array(
        '#type' => 'value',
        '#value' => ($op == CONTEXTIOPARSER_ADD_DERIVATIVE) ? array() : $derivative['used_replacement_patterns'],
      );

      // Replacement patterns.
      if (!empty($available_sources)) {
        $form['derivative_source']['replacement_patterns'] = array(
          '#type' => 'fieldset',
          '#title' => t('Replacement patterns'),
          '#collapsed' => TRUE,
          '#collapsible' => TRUE,
        );
        $form['derivative_source']['replacement_patterns']['description'] = array(
          '#prefix' => '<div class="description">',
          '#markup' => t('Replacement patterns can be used to substitute other source values. Derivative sources with lower numbers can be also referenced.'),
          '#suffix' => '</div><br/>',
        );

        $replacement_patterns = array();
        foreach ($available_sources as $key => $source) {
          $replacement_patterns[] = array(
            check_plain($source['name']),
            '[source:' . $key . ']',
            check_plain($source['description']),
          );
        }

        $form['derivative_source']['replacement_patterns']['table'] = array(
          '#theme' => 'table',
          '#header' => array(t('Name'), t('Token'), t('Description')),
          '#rows' => $replacement_patterns,
          '#caption' => NULL,
          '#colgroups' => array(),
          '#sticky' => FALSE,
          '#empty' => t('There are no available replacement patterns.'),
        );
      }
    }
    // Delete derivative source form.
    elseif (($op == CONTEXTIOPARSER_DELETE_DERIVATIVE) && !empty($derivative)) {
      $form['derivative_source'] = array(
        '#type' => 'fieldset',
        '#title' => t('delete derivative source'),
        '#collapsed' => FALSE,
        '#collapsible' => FALSE,
      );
      // Store operation type for later usage.
      $form['derivative_source']['op'] = array(
        '#type' => 'value',
        '#value' => $op,
      );
      // Store original derivative settings for later usage.
      $form['derivative_source']['original'] = array(
        '#type' => 'value',
        '#value' => $derivative,
      );

      $form['derivative_source']['message'] = array(
        '#prefix' => '<p>',
        '#markup' => t('Are you sure you want to delete the following derivative source?'),
        '#suffix' => '</p>',
      );
      $form['derivative_source']['name'] = array(
        '#prefix' => '<p>',
        '#markup' => check_plain($derivative['name']) . ' (' . $derivative['machine_name'] . ')',
        '#suffix' => '</p>',
      );
    }
  }

  /**
   * Helper function to list derivative source configs.
   *
   * @param array $form
   *   The basic config form the derivative config list will be added to.
   */
  protected function derivativeSources(array &$form) {
    $form['#tree'] = TRUE;
    $form['derivative_sources'] = array(
      '#type' => 'fieldset',
      '#title' => t('Derivative sources'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['derivative_sources']['description'] = array(
      '#prefix' => '<div class="description">',
      '#markup' => t('Derivative sources will provide additional custom mapping sources for processor plugins.'),
      '#suffix' => '</div>',
    );
    $form['derivative_sources']['action_links'] = array(
      '#theme' => 'item_list',
      '#items' => array(
        l(t('Add derivative source'), current_path(), array(
          'query' => array('op' => CONTEXTIOPARSER_ADD_DERIVATIVE),
        )),
      ),
      '#type' => 'ul',
      '#attributes' => array(
        'class' => array('action-links'),
      ),
      '#prefix' => '<br/>',
      '#suffix' => '<br/>',
    );

    if (!empty($this->config['derivative_sources'])) {
      $form['derivative_sources']['table'] = array(
        '#theme' => 'table',
        '#header' => array(
          array(
            'data' => t('Num'),
            'width' => '1%',
          ),
          array(
            'data' => t('Name'),
            'width' => '33%',
          ),
          t('Type'),
          t('Settings'),
          array(
            'data' => t('Operations'),
            'width' => '1%',
            'colspan' => 2,
          ),
        ),
        '#rows' => array(),
        '#caption' => NULL,
        '#colgroups' => array(),
        '#sticky' => FALSE,
        '#empty' => NULL,
      );

      // Get mapping sources with derivatives.
      $mapping_sources = $this->getMappingSources();

      $derivative_num = 1;
      // List derivative source configuration settings.
      foreach ($this->config['derivative_sources'] as $source) {
        // Construct a delete link for the derivative.
        $delete_link = '<div style="color: #666;">' . t('delete') . '</div>';

        // Check dependents.
        $dependents = $this->getDerivativeSourceDependents($source, TRUE);
        // Override delete link if no dependents found.
        if (empty($dependents['source'])) {
          $delete_link = l(t('delete'), current_path(), array(
            'query' => array(
              'op' => CONTEXTIOPARSER_DELETE_DERIVATIVE,
              'id' => $source['machine_name'],
            ),
          ));
        }

        // Construct a table row.
        $table_row = array(
          'num' => array(
            'data' => array(
              '#prefix' => '<div style="text-align: center;">',
              '#markup' => '#' . $derivative_num++,
              '#suffix' => '</div>',
            ),
          ),
          'name' => array(
            'data' => array(
              'name' => array(
                '#prefix' => '<div class="name">',
                '#markup' => check_plain($source['name']) . ' (' . $source['machine_name'] . ')',
                '#suffix' => '</div>',
              ),
            ),
            'class' => array('name'),
          ),
          'type' => array(
            'data' => array(),
          ),
          'settings' => array(
            'data' => array(),
          ),
          'edit' => array(
            'data' => array(
              '#markup' => l(t('edit'), current_path(), array(
                'query' => array(
                  'op' => CONTEXTIOPARSER_EDIT_DERIVATIVE,
                  'id' => $source['machine_name'],
                ),
              )),
            ),
          ),
          'delete' => array(
            'data' => array(
              '#markup' => $delete_link,
            ),
          ),
        );

        // Display description and dependents.
        if (!empty($source['description']) || !empty($dependents)) {
          $description = '<div class="description">';
          // Description.
          if (!empty($source['description'])) {
            $description .= '<div class="derivative-source-description"><p>' . check_plain($source['description']) . '</p></div>';
          }
          // Display dependents.
          if (!empty($dependents)) {
            $description .= '<div class="derivative-source-dependents">';
            // Required by.
            if (!empty($dependents['source'])) {
              $dependent_sources = array();
              foreach ($dependents['source'] as $dependent) {
                $dependent_sources[] = '#' . $dependent['num'] . ' ' . check_plain($dependent['name']) . ' (' . $dependent['machine_name'] . ')';
              }
              $description .= '<div class="required-by"><p>' . t('Required by: @dependents', array(
                '@dependents' => implode(', ', $dependent_sources),
              )) . '</p></div>';
            }
            // Used by.
            if (!empty($dependents['replacement_pattern'])) {
              $dependent_replacement_patterns = array();
              foreach ($dependents['replacement_pattern'] as $dependent) {
                $dependent_replacement_patterns[] = '#' . $dependent['num'] . ' ' . check_plain($dependent['name']) . ' (' . $dependent['machine_name'] . ')';
              }
              $description .= '<div class="used-by"><p>' . t('Used by: @dependents', array(
                '@dependents' => implode(', ', $dependent_replacement_patterns),
              )) . '</p></div>';
            }
            $description .= '</div>';
          }
          $description .= '</div>';

          $table_row['name']['data']['description']['#markup'] = $description;
        }

        // Display additional properties based on source type.
        switch ($source['type']) {
          case CONTEXTIOPARSER_CONSTANT_VALUE:
            $table_row['type']['data'] = array(
              '#markup' => CONTEXTIOPARSER_CONSTANT_VALUE_NAME,
            );

            // Mark replacement patterns "disabled" if no related derivatives
            // found (they have been deleted).
            $value = $source['value'];
            foreach ($source['used_replacement_patterns'] as $derivative_machine_name => $pattern) {
              if (!array_key_exists($derivative_machine_name, $mapping_sources)) {
                $value = str_replace($pattern, '<span style="color: #666;"><s>' . $pattern . '</s></span>', $value);
              }
            }

            $table_row['settings']['data']['value'] = array(
              '#prefix' => '<p>',
              '#markup' => t('<strong>Value:</strong> !value', array('!value' => $value)),
              '#suffix' => '</p>',
            );
            break;

          case CONTEXTIOPARSER_SIMPLE_REPLACE:
            $table_row['type']['data'] = array(
              '#markup' => CONTEXTIOPARSER_SIMPLE_REPLACE_NAME,
            );
            $table_row['settings']['data']['source'] = array(
              '#prefix' => '<p>',
              '#markup' => t('<strong>Source:</strong> @source', array(
                '@source' => $mapping_sources[$source['source']]['name'] . ' (' . $source['source'] . ')',
              )),
              '#suffix' => '</p>',
            );
            $table_row['settings']['data']['search_string'] = array(
              '#prefix' => '<p>',
              '#markup' => t('<strong>Search string:</strong> @search_string', array('@search_string' => $source['search_string'])),
              '#suffix' => '</p>',
            );

            // Mark replacement patterns "disabled" if no related derivatives
            // found (they have been deleted).
            $replacement = $source['replacement'];
            foreach ($source['used_replacement_patterns'] as $derivative_machine_name => $pattern) {
              if (!array_key_exists($derivative_machine_name, $mapping_sources)) {
                $replacement = str_replace($pattern, '<span style="color: #666;"><s>' . $pattern . '</s></span>', $replacement);
              }
            }

            $table_row['settings']['data']['replacement'] = array(
              '#prefix' => '<p>',
              '#markup' => t('<strong>Replacement:</strong> !replacement', array('!replacement' => $replacement)),
              '#suffix' => '</p>',
            );
            break;

          case CONTEXTIOPARSER_REGEX_REPLACE:
            $table_row['type']['data'] = array(
              '#markup' => CONTEXTIOPARSER_REGEX_REPLACE_NAME,
            );
            $table_row['settings']['data']['source'] = array(
              '#prefix' => '<p>',
              '#markup' => t('<strong>Source:</strong> @source', array(
                '@source' => $mapping_sources[$source['source']]['name'] . ' (' . $source['source'] . ')',
              )),
              '#suffix' => '</p>',
            );
            $table_row['settings']['data']['regex_pattern'] = array(
              '#prefix' => '<p>',
              '#markup' => t('<strong>Regex pattern:</strong> @regex_pattern', array('@regex_pattern' => $source['regex_pattern'])),
              '#suffix' => '</p>',
            );

            // Mark replacement patterns "disabled" if no related derivatives
            // found (they have been deleted).
            $regex_replacement = $source['regex_replacement'];
            foreach ($source['used_replacement_patterns'] as $derivative_machine_name => $pattern) {
              if (!array_key_exists($derivative_machine_name, $mapping_sources)) {
                $regex_replacement = str_replace($pattern, '<span style="color: #666;"><s>' . $pattern . '</s></span>', $regex_replacement);
              }
            }

            $table_row['settings']['data']['regex_replacement'] = array(
              '#prefix' => '<p>',
              '#markup' => t('<strong>Replacement:</strong> !regex_replacement', array('!regex_replacement' => $regex_replacement)),
              '#suffix' => '</p>',
            );
            break;
        }

        // Add table row.
        $form['derivative_sources']['table']['#rows'][] = $table_row;
      }
    }
    else {
      $form['derivative_sources']['table'] = array(
        '#prefix' => '<p>',
        '#markup' => t('No derivative sources have been added yet.'),
        '#suffix' => '</p>',
      );
    }
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    // Validate derivative source form. Required fields validated automatically.
    if (!empty($values['derivative_source']['op'])) {
      $op = $values['derivative_source']['op'];
      // Add or edit derivative.
      if (in_array($op, array(CONTEXTIOPARSER_ADD_DERIVATIVE, CONTEXTIOPARSER_EDIT_DERIVATIVE))) {
        // The name of the field where replacement patterns supported.
        $sub_patterns_field_name = NULL;

        switch ($values['derivative_source']['type']) {
          case CONTEXTIOPARSER_CONSTANT_VALUE:
            $sub_patterns_field_name = 'value';
            if (empty($values['derivative_source']['value'])) {
              form_set_error('derivative_source][value', t('Value field is required.'));
            }
            break;

          case CONTEXTIOPARSER_SIMPLE_REPLACE:
            $sub_patterns_field_name = 'replacement';
            if (empty($values['derivative_source']['source'])) {
              form_set_error('derivative_source][source', t('Source field is required.'));
            }
            if (empty($values['derivative_source']['search_string'])) {
              form_set_error('derivative_source][search_string', t('Search string field is required.'));
            }
            break;

          case CONTEXTIOPARSER_REGEX_REPLACE:
            $sub_patterns_field_name = 'regex_replacement';
            if (empty($values['derivative_source']['source'])) {
              form_set_error('derivative_source][source', t('Source field is required.'));
            }
            if (empty($values['derivative_source']['regex_pattern'])) {
              form_set_error('derivative_source][regex_pattern', t('Regex pattern field is required.'));
            }
            break;
        }

        // Check replacement patterns related circle references.
        if (!empty($values['derivative_source']['name']) && !empty($values['derivative_source']['machine_name'])) {
          $matches = array();
          $mapping_sources = $this->getMappingSources();
          preg_match_all('/(\[source:([a-z0-9_]+)\])/', $values['derivative_source'][$sub_patterns_field_name], $matches);
          if (!empty($matches[2])) {
            $matches_unique = array();
            foreach ($matches[2] as $index => $source_machine_name) {
              // Check if replacement pattern is exist.
              if (!array_key_exists($source_machine_name, $mapping_sources)) {
                form_set_error('derivative_source][' . $sub_patterns_field_name, t('Unknown replacement pattern: @token.', array(
                  '@token' => '[source:' . $source_machine_name . ']',
                )));
              }
              // Check if derivative source reference is valid.
              elseif (($op == CONTEXTIOPARSER_EDIT_DERIVATIVE) && array_key_exists($source_machine_name, $this->config['derivative_sources'])) {
                if ($values['derivative_source']['num'] <= $this->config['derivative_sources'][$source_machine_name]['num']) {
                  form_set_error('derivative_source][' . $sub_patterns_field_name, t('Invalid derivative source reference: @token.', array(
                    '@token' => '[source:' . $source_machine_name . ']',
                  )));
                }
                elseif (!array_key_exists($source_machine_name, $matches_unique)) {
                  $matches_unique[$source_machine_name] = $matches[1][$index];
                }
              }
              // Ignore match duplications.
              elseif (!array_key_exists($source_machine_name, $matches_unique)) {
                $matches_unique[$source_machine_name] = $matches[1][$index];
              }
            }
            // Save replacement pattern dependencies.
            $values['derivative_source']['used_replacement_patterns'] = $matches_unique;
          }
        }
      }
    }

    // Concrete parser's config form validation.
    $this->contextIOConfigFormValidate($values);
  }

  /**
   * Concrete parser specific configFormValidate() method.
   *
   * Allow concrete parsers to implement their own configFormValidate() method
   * without overriding parent's one.
   *
   * @see configFormValidate()
   */
  protected function contextIOConfigFormValidate(&$values) {
    // Parser specific config form validation.
  }

  /**
   * Override parent::configFormSubmit().
   */
  public function configFormSubmit(&$values) {
    // Save derivative source related settings if any.
    if (!empty($values['derivative_source']['op'])) {
      switch ($values['derivative_source']['op']) {
        case CONTEXTIOPARSER_ADD_DERIVATIVE:
          // Get the largest derivative source num.
          $max_num = 0;
          foreach ($this->config['derivative_sources'] as $derivative_source) {
            if ($derivative_source['num'] > $max_num) {
              $max_num = $derivative_source['num'];
            }
          }
          // The new derivative source will get the largest num.
          $values['derivative_source']['num'] = ($max_num + 1);
          // Save new settings.
          $this->config['derivative_sources'][$values['derivative_source']['machine_name']] = $values['derivative_source'];
          drupal_set_message(t('Derivative source has been saved.'));
          break;

        case CONTEXTIOPARSER_EDIT_DERIVATIVE:
          // Update existing settings.
          $this->config['derivative_sources'][$values['derivative_source']['machine_name']] = $values['derivative_source'];
          drupal_set_message(t('Derivative source has been updated.'));
          break;

        case CONTEXTIOPARSER_DELETE_DERIVATIVE:
          // Delete settings.
          unset($this->config['derivative_sources'][$values['derivative_source']['original']['machine_name']]);
          drupal_set_message(t('Derivative source has been deleted.'));
          break;
      }

      // Sort derivative sources by their num after operations.
      uasort($this->config['derivative_sources'], array('ContextIOParser', 'derivativeSourceNumCMP'));

      // Save config changes.
      $this->setConfig($this->config);
      $this->save();
      feeds_cache_clear(FALSE);
      // Redirect back.
      drupal_goto(current_path());
    }

    // Concrete parser's config form submission.
    $this->contextIOConfigFormSubmit($values);
  }

  /**
   * Compares the num of two derivative sources.
   *
   * This function is used in uasort PHP function calls.
   *
   * @param array $a
   *   Property array of the first derivative source.
   * @param array $b
   *   Property array of the second derivative source.
   *
   * @return int
   *   An integer value according to the comparison.
   *
   * @see configFormSubmit()
   */
  protected static function derivativeSourceNumCMP($a, $b) {
    if ($a['num'] == $b['num']) {
      return 0;
    }
    return ($a['num'] < $b['num']) ? -1 : 1;
  }

  /**
   * Concrete parser specific configFormSubmit() method.
   *
   * Allow concrete parsers to implement their own configFormSubmit() method
   * without overriding parent's one.
   *
   * @see configFormSubmit()
   */
  protected function contextIOConfigFormSubmit(&$values) {
    // Parser specific config form submission.
  }

  /**
   * Implements parent::getMappingSources().
   *
   * @param bool $include_derivatives
   *   Boolean indicating the result should include derivative sources or not.
   *
   * @return array
   *   An array of available mapping sources.
   */
  public function getMappingSources($include_derivatives = TRUE) {
    // Concrete parser specific mapping sources.
    $sources = $this->contextIOGetMappingSources();
    // Optionally add derivative sources.
    if ($include_derivatives) {
      foreach ($this->config['derivative_sources'] as $machine_name => $derivative_source) {
        $sources[$machine_name] = array(
          'name' => check_plain($derivative_source['name']),
          'description' => check_plain($derivative_source['description']),
        );
      }
    }
    return $sources;
  }

  /**
   * Concrete parser specific getMappingSources() method.
   *
   * Allow concrete parsers to implement their own getMappingSources() method
   * without overriding parent's one.
   *
   * @see getMappingSources()
   */
  protected abstract function contextIOGetMappingSources();

  /**
   * Helper function to get derivative source dependents.
   *
   * @param string $derivative_source
   *   Property array of the derivative source.
   * @param bool $check_replacement_patterns
   *   Boolean indicating that replacement patterns should be checked or not
   *   during the dependent search.
   *
   * @return array
   *   An associative array of dependents keyed by dependent type.
   */
  protected function getDerivativeSourceDependents($derivative_source, $check_replacement_patterns = FALSE) {
    $dependents = array();
    $source_num = 1;
    foreach ($this->config['derivative_sources'] as $source) {
      // Skip the derivative itself and derivatives with lower num.
      if ($source['num'] <= $derivative_source['num']) {
        $source_num++;
        continue;
      }
      // Check source dependents.
      if ($source['source'] == $derivative_source['machine_name']) {
        $dependents['source'][] = array(
          'name' => $source['name'],
          'machine_name' => $source['machine_name'],
          'num' => $source_num,
        );
      }
      // Check replacement_pattern dependents.
      if ($check_replacement_patterns && array_key_exists($derivative_source['machine_name'], $source['used_replacement_patterns'])) {
        $dependents['replacement_pattern'][] = array(
          'name' => $source['name'],
          'machine_name' => $source['machine_name'],
          'num' => $source_num,
        );
      }
      $source_num++;
    }
    return $dependents;
  }

  /**
   * Add derivative sources to a fetcher result item.
   *
   * @param array $item
   *   An array of a fetcher result item.
   */
  protected function addDerivativeSources(&$item) {
    // Process derivative sources.
    foreach ($this->config['derivative_sources'] as $derivative_source) {
      $search = array();
      $replace = array();
      // Collect used replacement pattern.
      foreach ($derivative_source['used_replacement_patterns'] as $key => $pattern) {
        // Ignore patterns which not exist (their derivative have been deleted).
        if (isset($item[$key])) {
          $search[] = $pattern;
          $replace[] = $item[$key];
        }
      }
      // Replace replacement patterns.
      switch ($derivative_source['type']) {
        case CONTEXTIOPARSER_CONSTANT_VALUE:
          $value = str_replace($search, $replace, $derivative_source['value']);
          $item[$derivative_source['machine_name']] = $value;
          break;

        case CONTEXTIOPARSER_SIMPLE_REPLACE:
          if (isset($item[$derivative_source['source']]) && is_string($item[$derivative_source['source']])) {
            $source = $item[$derivative_source['source']];
            $search_string = $derivative_source['search_string'];
            $replacement = str_replace($search, $replace, $derivative_source['replacement']);
            $item[$derivative_source['machine_name']] = str_replace($search_string, $replacement, $source);
          }
          break;

        case CONTEXTIOPARSER_REGEX_REPLACE:
          if (isset($item[$derivative_source['source']]) && is_string($item[$derivative_source['source']])) {
            $source = $item[$derivative_source['source']];
            $regex_pattern = $derivative_source['regex_pattern'];
            $regex_replacement = str_replace($search, $replace, $derivative_source['regex_replacement']);
            $item[$derivative_source['machine_name']] = preg_replace($regex_pattern, $regex_replacement, $source);
          }
          break;
      }
    }
  }

}
