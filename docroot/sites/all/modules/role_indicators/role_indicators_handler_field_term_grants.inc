<?php

class role_indicators_handler_field_term_grants extends taxonomy_access_handler_field_term_grants {
  var $terms;

  function pre_render(&$values) {
    parent::pre_render($values);
    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['taxonomy_access_tid']})) {
        $tids[] = $result->{$this->aliases['taxonomy_access_tid']};
      }
    }
    $this->terms = taxonomy_term_load_multiple($tids);
  }

  function allow_advanced_render() {
    return FALSE;
  }

  function render($values) {
    // Try to retrieve the loaded term. If the user does not have access to
    // view the term, it will not be loaded, so bail out without printing
    // anything in that case.
    $tid = $values->{$this->aliases['taxonomy_access_tid']};
    if (isset($this->terms[$tid])) {
      $term = $this->terms[$tid];
    }
    else {
      return '';
    }

    // Retrieve the role IDs set earlier in the pre_render stage (in
    // the taxonomy_access handler).
    if (isset($this->items[$tid]) && is_array($this->items[$tid])) {
      $rids = array_keys($this->items[$tid]);
    }
    else {
      $rids = array();
    }
    // Use the role IDs to get a list of role indicator classes.
    $classes = role_indicators_get_classes($rids);

    // Build the link.
    $uri = role_indicators_get_uri($term);
    $path = $uri['path'];
    $options = $uri['options'];
    if (is_array($options['attributes']['class'])) {
      $options['attributes']['class'] = array_merge($options['attributes']['class'], $classes);
    }
    else {
      $options['attributes']['class'] = $classes;
    }
    return l('', $path, $options);
  }
}
