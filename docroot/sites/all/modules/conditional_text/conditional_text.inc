<?php

/**
 * @file
 * Conditional text API.
 *
 * These functions are responsible for parsing, evaluating (with a proper
 * plugin) and rendering the text.
 */

/**
 * @defgroup conditional_text_api Conditional Text API
 * @{
 */

/**
 * Builds a settings form from the plugins.
 *
 * @return array
 *   The settings form, assembled from the plugins.
 */
function conditional_text_build_condition_settings_form(&$form_state,
                                                        $filter) {
  return array_filter(
    array_map(function ($plugin) use (&$form_state, $filter) {
      $settings = $plugin['settings']($form_state,
          isset($filter->settings[$plugin['name']]) ?
            $filter->settings[$plugin['name']] : NULL);
        return $settings ?: NULL;
    },
    conditional_text_get_plugins(CONDITIONAL_TEXT_CONDITION_PLUGIN)));
}

/**
 * Collects filter tips from the plugins.
 *
 * @param stdClass $filter
 *   An object representing the filter.
 * @param stdClass $format
 *   An object representing the text format the filter is contained in.
 * @param boolean $long
 *   Whether this callback should return a short tip to display in a
 *   form (FALSE), or whether a more elaborate filter tips should be returned
 *   for theme_filter_tips() (TRUE).
 *
 * @return array
 *   List of the collected filter tips.
 */
function conditional_text_collect_filter_tips($filter, $format, $long) {
  $callback = $long ? 'long help' : 'short help';
  return array_filter(array_map(function ($plugin) use ($callback, $filter, $format, $long) {
      $tip = maybe_call(array_value($plugin, $callback), $filter, $format);
      return $tip ?: maybe_call(array_value($plugin, 'help'), $filter,
        $format, $long);
    },
    conditional_text_get_plugins(CONDITIONAL_TEXT_CONDITION_PLUGIN)));
}

/**
 * Finds an appropriate condition plugin for an identifier token.
 *
 * @param string $identifier
 *   Plugin identifier.
 *
 * @return array
 *   The plugin.
 */
function conditional_text_get_condition_plugin($identifier) {
  $condition_plugins =
    conditional_text_get_plugins(CONDITIONAL_TEXT_CONDITION_PLUGIN);
  $candidates = array_filter($condition_plugins,
    function ($plugin) use ($identifier) {
      return $plugin['identifier token'] == $identifier;
    });
  return count($candidates) ? head($candidates) : NULL;
}

/**
 * Collects and formats the reasons of a decision.
 *
 * @param string $condition
 *   Condition string.
 * @param null|array $context
 *   Optional context array.
 *
 * @return string
 *   Formatted string of decisions.
 */
function conditional_text_get_reasons($condition, $context) {
  $operators = conditional_text_get_operators();
  $tokens = _conditional_text_normalize_conditions(
    conditional_text_tokenize($condition), $operators);

  $processed_tokens = array_map(function ($item) use ($operators, $context) {
      // operator
      if (count($item) == 1 && in_array(head($item), $operators)) {
        return t(head($item));
      }
      else {
        $plugin = conditional_text_get_condition_plugin(head($item));
        return ($plugin && !empty($plugin['reason'])) ?
          $plugin['reason'](tail($item), $context) :
          implode(' ', tail($item));
      }
    }, $tokens);

  return implode(' ', $processed_tokens);
}

/**
 * Evaluates a condition using a condition plugin.
 *
 * @param array $condition
 *   Condition tokens.
 * @param array $context
 *   Context array.
 *
 * @return bool|null
 *   Evaluation result or NULL, in case of an error.
 */
function conditional_text_evaluate_condition($condition, $context) {
  $identifier = array_shift($condition);
  $plugin = conditional_text_get_condition_plugin($identifier);
  return (isset($plugin['evaluate']) && is_callable($plugin['evaluate'])) ?
    $plugin['evaluate']($condition,
                        isset($context[$plugin['name']]) ?
                          $context[$plugin['name']] : NULL) :
    NULL;
}

/**
 * Evaluates a boolean condition.
 *
 * @param array $condition
 *   Array containing only one item, which is either the string 'true' or the
 *   string 'false'.
 * @param array $context
 *   Context array.
 *
 * @return bool
 */
function conditional_text_evaluate_bool_condition($condition, $context) {
  return head($condition) == 'true';
}

/**
 * Evaluates a series of conditions.
 *
 * @param array $conditions
 *   List of tokens.
 * @param array $context
 *   Context array.
 * @param callable $condition_evaluator
 *
 * @return bool
 *   Result of the evaluation.
 */
function conditional_text_evaluate_conditions($conditions, $context,
    $condition_evaluator = 'conditional_text_evaluate_condition') {
  $operators = conditional_text_get_operators();
  return _conditional_text_evaluate_conditions(
    _conditional_text_normalize_conditions($conditions, $operators),
    $operators, $context, $condition_evaluator);
}

/**
 * Recursive helper function for conditional_text_evaluate_conditions().
 *
 * @param array $conditions
 *   Normalized array of conditions.
 * @param array $operators
 *   Keyed array of the operators. The key is the operator,
 *   the value is a keyed array:
 *   - arity: arity of the operator. Currently operators with arity 1 (in
 *     prefix position) and 2 (infix position) are supported
 *   - evaluate: callable, with $arity number of arguments.
 *   See conditional_text_evaluate_conditions() for examples.
 * @param array $context
 *   Context array.
 * @param callable $condition_evaluator
 *   Callable to evaluate the condition.
 * @param null|bool $out
 *   Internal use only.
 *
 * @return bool
 *   Result of the evaluation.
 *
 * @see conditional_text_evaluate_conditions()
 * @see _conditional_text_normalize_conditions()
 */
function _conditional_text_evaluate_conditions($conditions,
                                               $operators,
                                               $context,
                                               $condition_evaluator,
                                               $out = NULL) {
  if (count($conditions)) {
    $item = array_shift($conditions);
    if (is_array($item)) {
      return _conditional_text_evaluate_conditions(
        $conditions,
        $operators,
        $context,
        $condition_evaluator,
        $condition_evaluator($item, $context));
    }
    else if (isset($operators[$item])) {
      switch ($operators[$item]['arity']) {
        case CONDITIONAL_TEXT_ARITY_PREFIX:
          return $operators[$item]['evaluate'](
            _conditional_text_evaluate_conditions(
              $conditions,
              $operators,
              $context,
              $condition_evaluator));
        case CONDITIONAL_TEXT_ARITY_INFIX:
          return $operators[$item]['evaluate']($out,
            _conditional_text_evaluate_conditions(
              $conditions,
              $operators,
              $context,
              $condition_evaluator));
      }
    }
  }

  return $out;
}

/**
 * Prepares the condition array for processing.
 *
 * @param array $conditions
 *   List of tokens.
 * @param array $operators
 *   Operator array. See _conditional_text_evaluate_conditions() for the
 *   description of the operator array.
 *
 * @return array
 *   Normalized array. This is a list, with two kind of items:
 *   - string: operator
 *   - array: list of tokens to evaluate
 */
function _conditional_text_normalize_conditions($conditions, $operators) {
  $acc = array(array());

  foreach ($conditions as $condition) {
    if (isset($operators[$condition])) {
      $acc[] = $condition;
      $acc[] = array();
    }
    else {
      $acc[count($acc) - 1][] = $condition;
    }
  }

  return array_filter($acc);
}

/**
 * Tokenizes an expression.
 *
 * @param string $string
 *   String to conditional_text_tokenize.
 * @param array $tokens
 *   Accumulator for tail recursion. Internal use only.
 *
 * @return array
 *   List of tokens.
 */
function conditional_text_tokenize($string, $tokens = array('')) {
  if ($string == '') {
    return array_filter($tokens);
  }

  switch ($string[0]) {
    case '=':
      if (isset($tokens[count($tokens) - 2]) &&
          ($tokens[count($tokens) - 2] == '>' ||
           $tokens[count($tokens) - 2] == '<' ||
           $tokens[count($tokens) - 2] == '=')) {
        $tokens[count($tokens) - 2] .= $string[0];
        break;
      }
      // no break here
    case '<': case '>':
      $tokens[] = $string[0];
      // no break here
    case ' ': case "\n": case "\t":
      $tokens[] = '';
      break;
    default:
      $tokens[count($tokens) - 1] .= $string[0];
      break;
  }

  return conditional_text_tokenize(substr($string, 1), $tokens);
}

/**
 * Parses a text and transforms the conditional statements.
 *
 * @param string $text
 *   Text to parse.
 * @param callback $render
 *   Renderer function. Defaults to conditional_text_render().
 * @param callback $evaluate
 *   Evaluator function. Defaults to conditional_text_evaluate().
 * @param callback $reasons
 *   Reason renderer function. Defaults to conditional_text_get_reasons().
 *
 * @return string
 *   Transformed text.
 *
 * @see conditional_text_render()
 * @see conditional_text_evaluate()
 */
function conditional_text_parse($text,
                                $context,
                                $render = 'conditional_text_render',
                                $evaluate = 'conditional_text_evaluate',
                                $reasons = 'conditional_text_get_reasons') {
  // @todo Fix the parsing to support nested BBCode.
  return preg_replace_callback(
    '#\[condition ([^\]]+)\]([^\[]*)\[/condition\]#im',
    function ($matches) use ($context, $render, $evaluate, $reasons) {
      list(, $condition, $text) = $matches;
      return $render($evaluate($condition, $context),
                     $text,
                     $reasons($condition, $context),
                     $context);
    }, html_entity_decode($text));
}

/**
 * Evaluates an expression.
 *
 * @param string $condition
 *   Condtion text.
 * @param array $context
 *   Context array.
 *
 * @return bool
 *   Evaluation result.
 */
function conditional_text_evaluate($condition, $context) {
  $conditions = conditional_text_tokenize($condition);
  return conditional_text_evaluate_conditions($conditions, $context);
}

/**
 * Renders a conditional text block.
 *
 * @param bool $result
 *   Whether to show or hide the text.
 * @param string $text
 *   Text to render.
 * @param array $context
 *   Context array.
 *
 * @return string
 *   Rendered text.
 */
function conditional_text_render($result, $text, $reasons, $context) {
  $plugin = conditional_text_get_plugin(CONDITIONAL_TEXT_DISPLAY_PLUGIN,
    isset($context['display']) ? $context['display'] :
      variable_get('conditional_text_default_display_plugin'));

  if ($plugin && isset($plugin['theme'])) {
    if (is_array($plugin['theme'])) {
      if (!empty($plugin['theme'][$result ? 'true' : 'false'])) {
        return theme($plugin['theme'][$result ? 'true' : 'false'],
                     array(
                          'text' => $text,
                          'reason' => $reasons,
                     ));
      }
    }
    else {
      return theme($plugin['theme'],
                   array(
                        'text' => $text,
                        'reason' => $reasons,
                        'result' => $result,
                   ));
    }
  }

  return $text;
}

/**
 * Finds an item in a form.
 *
 * @param array $form
 *   Form structure array.
 * @param string $key
 *   Name of the item to find.
 *
 * @return array|null
 *   The item or NULL if not found.
 */
function &conditional_text_find_subtree(&$form, $key) {
  if (is_array($form)) {
    if (!empty($form[$key])) {
      return $form[$key];
    }
    else {
      foreach ($form as &$item) {
        $result = &conditional_text_find_subtree($item, $key);
        if ($result) {
          return $result;
        }
      }
    }
  }

  // The function expects to return a reference, so if we just say
  // 'return NULL;' here then it will generate an E_NOTICE.
  return reference(NULL);
}

/**
 * Collects values in a tree with a given key.
 *
 * @param $tree
 *   Tree to search in.
 * @param $key
 *   Key of the values.
 * @param array $values
 *   Used internally.
 *
 * @return array
 *   List of the found values.
 */
function conditional_text_collect_tree_values($tree, $key, $values = array()) {
  foreach ($tree as $k => $v) {
    if ($k === $key) {
      $values[] = $v;
    }
    elseif (is_array($v)) {
      $values = conditional_text_collect_tree_values($v, $key, $values);
    }
  }

  return $values;
}

/**
 * @} End of "defgroup conditional_text_api".
 */
