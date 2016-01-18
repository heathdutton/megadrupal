<?php
// $Id: FeedsREGEXParser.inc,v 1.6.2.2.2.1 2010/10/12 06:31:11 twistor Exp $

/**
 * @file
 *
 * Provides the class for FeedsREGEXParser.
 */

class FeedsREGEXParser extends FeedsParser {

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    // Should we try some regex to find the title, assuming it's HTML?
    // $batch->setTitle();
    $result = new FeedsParserResult();

    $this->source_config = $source->getConfigFor($this);
    $this->matchall = array_keys(array_filter($this->source_config['matchall']));
    $this->matchall[] = '_context_';

    $matches = $this->pregMatch($fetcher_result->getRaw(), $this->source_config['context'], '_context_');
    foreach ($matches as $match) {
      $parsed_item = array();
      foreach ($this->source_config['sources'] as $source => $regex) {
        $parsed_item[$source] = $this->parseSourceElement($match, $regex, $source);
      }
      $result->addItem($parsed_item);
    }
    return $result;
  }

  protected function pregMatch($text, $regex, $source) {
    if (in_array($source, $this->matchall)) {
      $success = @preg_match_all($regex, $text, $matches);
    }
    else {
      $success = @preg_match($regex, $text, $matches);
    }
    if ($success === FALSE) {
      throw new Exception(t('There was an error with the regex: %query', array('%query' => $regex)));
    }
    if (isset($matches[1])) {
      return $matches[1];
    }
    return $matches[0];
  }

  protected function parseSourceElement($item, $regex, $source) {
    if ($regex == '') {
      return;
    }
    $results = $this->pregMatch($item, $regex, $source);
    /**
     * If their is one result, return it directly.  If there is more than one,
     * return the array.
     */
    if (is_array($results)) {
      return (count($results) <= 1) ? reset($results) : $results;
    }
    else {
      return $results;
    }
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['#weight'] = -10;

    $mappings_ = feeds_importer($this->id)->processor->config['mappings'];
    $uniques = $mappings = array();

    foreach ($mappings_ as $mapping) {
      if (strpos($mapping['source'], 'regexparser:') === 0) {
        $mappings[$mapping['source']] = $mapping['target'];
        if ($mapping['unique']) {
          $uniques[] = $mapping['target'];
        }
      }
    }

    if (empty($mappings)) {
      $form['error_message']['#value'] = 'FeedsREGEXParser: No mappings were defined.';
      return $form;
    }

    $form['context'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Context'),
      '#required'      => TRUE,
      '#description'   => t('This is the base REGEX, all other queries will run in this context.'),
      '#default_value' => isset($source_config['context']) ? $source_config['context'] : '',
    );

    $form['sources'] = array(
      '#type' => 'fieldset',
    );

    if (!empty($uniques)) {
      $items = array(
        format_plural(count($uniques),
          t('Field <strong>!column</strong> is mandatory and considered unique: only one item per !column value will be created.',
            array('!column' => implode(', ', $uniques))),
          t('Fields <strong>!columns</strong> are mandatory and values in these columns are considered unique: only one entry per value in one of these columns will be created.',
            array('!columns' => implode(', ', $uniques)))),
      );
      $form['sources']['help']['#value'] = '<div class="help">' . theme('item_list', $items) . '</div>';
    }

    foreach ($mappings as $source => $target) {
      $form['sources'][$source] = array(
        '#type'          => 'textfield',
        '#title'         => $target,
        '#description'   => t('The REGEX for this field.'),
        '#default_value' => isset($source_config['sources'][$source]) ? $source_config['sources'][$source] : '',
      );
    }

    $form['matchall'] = array(
      '#type'          => 'checkboxes',
      '#title'         => t('Select which ones you want to use preg_match_all on'),
      '#options'       => $mappings,
      '#default_value' => isset($source_config['matchall']) ? $source_config['matchall'] : array(),
    );
    
    return $form;
  }

  /**
  * Override parent::getMappingSources().
  */
  public function getMappingSources() {
    return array(
      'regexparser:0' => array(
        'name' => t('REGEX'),
        'description' => t('Allows you to configure a regular expression that will populate this field.'),
      ),
    ) + parent::getMappingSources();
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    return array(
      'context'  => '',
      'sources'  => array(),
      'matchall' => array(),
    );
  }

  /**
   * Override parent::sourceFormValidate().
   *
   * Simply trims all values from the form. That way when testing them
   * later we can be sure that there aren't any strings with spaces in them.
   *
   * @param &$values
   *   The values from the form to validate, passed by reference.
   */
  public function sourceFormValidate(&$values) {
    $values['context'] = trim($values['context']);
    foreach ($values['sources'] as &$regex) {
      $regex = trim($regex);
    }
  }
}

/**
 * Implementation of hook_form_feeds_ui_mapping_form_alter().
 *
 * This is an interesting bit of work. Each source name has to be unique,
 * but we have no idea how many to create with getMappingSources() because we
 * don't know how many targets there are going to be.
 *
 * Solution is to keep track in the form how many have been added.
 */
function feeds_regex_parser_form_feeds_ui_mapping_form_alter(&$form, &$form_state) {
  $newest_regex_mapping = array();
  foreach ($form['#mappings'] as $mapping) {
    if (strpos($mapping['source'], 'regexparser:') === 0) {
      $newest_regex_mapping = $mapping;
    }
  }
  if (!empty($newest_regex_mapping)) {
    list($a, $count) = explode(':', $newest_regex_mapping['source']);
    $default_source = $a . ':' . '0';
    $label = $form['source']['#options'][$default_source];
    unset($form['source']['#options'][$default_source]);
    $form['source']['#options'][$a . ':' . ++$count] = $label;
  }
}
