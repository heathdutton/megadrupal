<?php

class context_as_reaction_context_reaction_more_context extends context_reaction {
  
  function options_form($context) {
    $values = $this->fetch_from_context($context);
    //namespace, attribute, value
    return array(
      '#tree' => TRUE,
      '#title' => t('Context Variable'),
      'namespace' => array(
        '#title' => t('Namespace'),
        '#type' => 'textfield',
        '#default_value' => isset($values['namespace']) ? $values['namespace'] : '',
      ),
      'attribute' => array(
        '#title' => t('Attribute'),
        '#type' => 'textfield',
        '#default_value' => isset($values['attribute']) ? $values['attribute'] : '',
      ),
      'value' => array(
        '#title' => t('Value'),
        '#type' => 'textfield',
        '#default_value' => isset($values['value']) ? $values['value'] : '',
      ),
    );
  }
  
  function options_form_submit($values) {
    return $values;
  }
  
  function execute() {
    $contexts = context_active_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions['context_as_reaction'])) {
        $additional_context =& $context->reactions['context_as_reaction'];
        if (!empty($additional_context['namespace']) && !empty($additional_context['attribute']) && !empty($additional_context['value'])) {
          context_set($additional_context['namespace'], $additional_context['attribute'], $additional_context['value']);
        }
      }
    }
  }
  
}