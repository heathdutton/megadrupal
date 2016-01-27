<?php

/**
 * Expose paths as a context condition.
 */
class context_condition_domain extends context_condition_path {

  /**
   * Execute.
   */
  function execute() {
    if ($this->condition_used()) {
      // Include both the path alias and normal path for matching.
      $current_domain = $_SERVER['HTTP_HOST'];

      foreach ($this->get_contexts() as $context) {
        $domains = $this->fetch_from_context($context, 'values');
        if ($this->match($current_domain, $domains)) {
          $this->condition_met($context);
        }
      }
    }
  }

  /**
   * Match the subject against a set of regex patterns.
   * Similar to drupal_match_path() but also handles negation through the use
   * of the ~ character.
   *
   * @param mixed $subject
   *   The subject string or an array of strings to be matched.
   * @param array $patterns
   *   An array of patterns. Any patterns that begin with ~ are considered
   *   negative or excluded conditions.
   * @param boolean $domain
   *   Whether the given subject should be matched as a Drupal path. If TRUE,
   *   '<front>' will be replaced with the site frontpage when matching against
   *   $patterns.
   */
  protected function match($subject, $patterns, $path = FALSE) {
    static $regexps;
    $match = FALSE;
    $positives = $negatives = 0;
    $subject = !is_array($subject) ? array($subject) : $subject;
    foreach ($patterns as $pattern) {
      if (strpos($pattern, '~') === 0) {
        $negate = TRUE;
        $negatives++;
      }
      else {
        $negate = FALSE;
        $positives++;
      }
      $pattern = ltrim($pattern, '~');
      if (!isset($regexps[$pattern])) {
        $regexps[$pattern] = '/^('. preg_replace(array('/(\r\n?|\n)/', '/\\\\\*/'), array('|', '.*'), preg_quote($pattern, '/')) .')$/';
      }
      foreach ($subject as $value) {
        if (preg_match($regexps[$pattern], $value)) {
          if ($negate) {
            return FALSE;
          }
          $match = TRUE;
        }
      }
    }
    // If there are **only** negative conditions and we've gotten this far none
    // we actually have a match.
    if ($positives === 0 && $negatives) {
      return TRUE;
    }
    return $match;
  }
}
