<?php

/**
 * @file
 * A plugin for parsing xml/html documents.
 */

/**
 * Implements hook_rules_data_info().
 */
function rules_xpath_parser_rules_data_info() {
  return array(
    'dom' => array(
      'label' => t('DOM'),
      //'wrap' => TRUE,
    ),
    'dom_document' => array(
      'label' => t('DOM Document'),
      //'wrap' => TRUE,
      'parent' => 'dom',
    ),
    'dom_element' => array(
      'label' => t('DOM Element'),
      //'wrap' => TRUE,
      'parent' => 'dom',
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function rules_xpath_parser_rules_action_info() {
  $items = array();
  $items['import_xml'] = array(
    'label' => t('Import an XML document'),
    'base' => 'rules_xpath_parser_import_xml',
    'group' => t('Data'),
    'parameter' => array(
      'raw_xml' => array(
        'type' => 'text',
        'label' => 'Raw XML',
      ),
    ),
    'provides' => array(
      'xml_document' => array(
        'type' => 'dom_document',
        'label' => t('XML Document'),
      ),
    ),
  );
  $items['import_html'] = array(
    'label' => t('Import an HTML document'),
    'base' => 'rules_xpath_parser_import_html',
    'group' => t('Data'),
    'parameter' => array(
      'raw_html' => array(
        'type' => 'text',
        'label' => 'Raw HTML',
      ),
    ),
    'provides' => array(
      'html_document' => array(
        'type' => 'dom_document',
        'label' => t('HTML Document'),
      ),
    ),
  );
  $items['evaluate_xpath'] = array(
    'label' => t('Retrieve node(s) from an XML document'),
    'base' => 'rules_xpath_parser_evaluate_xpath',
    'group' => t('Data'),
    'parameter' => array(
      'type' => array(
        'type' => 'text',
        'label' => t('Result type'),
        'options list' => 'rules_xpath_parser_action_evaluate_xpath_options',
        'description' => t('Specifies the type of the variable that should be added.'),
        'restriction' => 'input',
      ),
      'dom_document' => array(
        'type' => 'dom_document',
        'label' => t('XML/HTML document'),
      ),
      'context_node' => array(
        'type' => 'dom_element',
        'label' => t('Context node'),
        'optional' => TRUE,
      ),
      'xpath' => array(
        'type' => 'text',
        'label' => t('XPath expression'),
      ),
    ),
    'provides' => array(
      'result' => array(
        'type' => 'unkown',
        'label' => t('XPath result'),
      ),
    ),
    'callbacks' => array(
      'form_alter' => 'rules_xpath_parser_action_evaluate_xpath_form_alter',
      'validate' => 'rules_action_create_type_validate',
    ),
  );
  return $items;
}

/**
 * Info alteration callback for xpath evaluate action.
 */
function rules_xpath_parser_evaluate_xpath_info_alter(&$element_info, RulesAbstractPlugin $element) {
  if (isset($element->settings['type']) && $type = $element->settings['type']) {
    $cache = rules_get_cache();
    $type_info = $cache['data_info'][$type];
    $element_info['provides']['result']['type'] = $type;
    $element_info['provides']['result']['label'] = $type_info['label'];
  }
}

function rules_xpath_parser_import_xml($raw_xml) {
  $doc = new DOMDocument();
  $doc->loadXML($raw_xml);
  return array('xml_document' => $doc);
}

function rules_xpath_parser_import_html($raw_html) {
  $doc = new DOMDocument();
  $doc->loadHTML($raw_html);
  return array('html_document' => $doc);
}

function rules_xpath_parser_evaluate_xpath($result_type, $dom_document, $context_node, $xpath, $options) {
  $xpath_obj = new DOMXPath($dom_document);
  $namespaces = simplexml_import_dom($dom_document)->getNamespaces(TRUE);

  foreach ($namespaces as $prefix => $namespace) {
    if ($prefix === '') {
      $xpath_obj->registerNamespace('__default__', $namespace);
      $parser = new RulesXPathQueryParser($xpath);
      $xpath = $parser->getQuery();
    }
    else {
      $xpath_obj->registerNamespace($prefix, $namespace);
    }
  }
  if ($context_node) {
    $result = $xpath_obj->evaluate($xpath, $context_node);
  }
  else {
    $result = $xpath_obj->evaluate($xpath);
  }

  if (!is_object($result)) {
    return array('result' => $result);
  }
  $array = array();
  foreach ($result as $item) {
    if ($context_node) {
      $array[] = $item->nodeValue;
    }
    else {
      $array[] = $item;
    }
  }
  return array('result' => $array);
}

function rules_xpath_parser_action_evaluate_xpath_options() {
  return RulesPluginUI::getOptions('data');
}

/**
 * Form alter callback for actions relying on the entity type or the data type.
 */
function rules_xpath_parser_action_evaluate_xpath_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  rules_action_type_form_alter($form, $form_state, $options, $element);
}


/**
 * Pseudo-parser of XPath queries.
 *
 * @param $query
 *   An XPath query string.
 * @return string
 *   An XPath query string with the __default__ namespace added.
 */
class RulesXPathQueryParser {
  function __construct($query) {
    $this->query = preg_replace('/\s+\(\s*/', '(', $query);

    $this->word_boundaries = array(
      '[', ']', '=', '(', ')', '.', '<', '>', '*', '!', '|', '/', ',', ' ', ':',
    );
    $this->in_quotes = FALSE;
    $this->quote_char = '';
    $this->word = '';
    $this->output = '';
    $this->prev_boundary = '';
    $this->axis = '';
    $this->skip_next_word = FALSE;
    $this->start();
  }

  function start() {
    for ($i=0; $i < strlen($this->query); $i++) {
      $this->i = $i;
      $c = $this->query[$i];

      if ($c == '"' || $c == "'") {
        $this->handle_quote($c);
        continue;
      }
      if ($this->in_quotes) {
        $this->word .= $c;
        continue;
      }

      if (in_array($c, $this->word_boundaries)) {
        $this->handle_word_boundary($c);
      }
      else {
        $this->word .= $c;
      }
    }
    $this->handle_word();
  }

  function handle_quote($c) {
    if ($this->in_quotes && $c == $this->quote_char) {
      $this->in_quotes = FALSE;
      $this->word .= $c;
      $this->output .= $this->word;
      $this->word = '';
    }
    elseif (!$this->in_quotes) {
      $this->in_quotes = TRUE;
      $this->handle_word();
      $this->word = $c;
      $this->quote_char = $c;
    }
    else {
      $this->word .= $c;
    }
  }

  function handle_word_boundary($c) {
    if (in_array($this->word, array('div', 'or', 'and', 'mod')) &&
        $this->prev_boundary == ' ' && $c == ' ') {
      $this->output .= $this->word;
    }
    else {
      $this->handle_word($c);
    }
    $this->output .= $c;
    $this->word = '';
    $this->prev_boundary = $c;
  }

  function handle_word($c='') {
    if ($this->word == '') {
      return;
    }
    if ($c == ':' && $this->query[$this->i + 1] == ':') {
      $this->axis = $this->word;
    }
    if ($c == ':' && $this->query[$this->i - 1] != ':'  &&
        $this->query[$this->i + 1] != ':') {
      $this->output .= $this->word;
      $this->skip_next_word = TRUE;
      return;
    }
    if ($this->skip_next_word) {
      $this->skip_next_word = FALSE;
      $this->output .= $this->word;
      return;
    }
    if (is_numeric($this->word) ||
        $this->axis == 'attribute' ||
        strpos($this->word, '@') === 0 ||
        $c == '(' ||
        $c == ':') {
      $this->output .= $this->word;
      return;
    }
    $this->output .= '__default__:' . $this->word;
  }

  function getQuery() {
    return $this->output;
  }
}
