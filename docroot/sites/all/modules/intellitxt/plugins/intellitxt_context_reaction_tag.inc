<?php

class intellitxt_context_reaction_tag extends context_reaction {

  function options_form($context) {
    $options = array();
    $tags = variable_get('intellitxt_tags', array());
    foreach ($tags as $tag) {
      $options[key($tag)] = key($tag) . ' (' . current($tag) . ')';
    }
    asort($options);

    return array(
      'tag' => array(
        '#title' => $this->title,
        '#description' => $this->description,
        '#options' => $options,
        '#type' => 'select',
        '#default_value' => $this->fetch_from_context($context),
      ),
      'position' => array(
        '#title' => 'Tag Position',
        '#description' => 'Where the tag is rendered on the page',
        '#options' => array('HEAD' => 'In the <HEAD>', 'BODY' => 'Above the </BODY>'),
        '#type' => 'select',
        '#default_value' => $this->fetch_from_context($context),
      ),
    );
  }

  function execute() {
    $contexts = $this->get_contexts();
    foreach ($contexts as $context) {
      $machine_name = $context->reactions[$this->plugin]['tag'];
      if ($machine_name) {
        $position = $context->reactions[$this->plugin]['position'];
        intellitxt_add_tag($machine_name, $position);
      }
    }
  }
}