<?php

class views_handler_field_amazon_participant extends views_handler_field_prerender_list {
  function construct() {
    parent::construct();
    $this->additional_fields['asin'] = array('table' => 'amazon_item', 'field' => 'asin');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['participant_type'] = array('default' => '**ALL**');
    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['participant_type'] = array(
      '#type' => 'select',
      '#title' => t('Participant restriction'),
      '#options' => array(
        '**ALL**' => t('Show all participants'),
        'author' => t('Author'),
        'artist' => t('Artist'),
        'actor' => t('Actor'),
        'director' => t('Director'),
        'creator' => t('Creator'),
      ),
      '#default_value' => $this->options['participant_type'],
    );
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['asin'];
  }

  function pre_render($values) {
    $this->items = array();

    $asins = array();
    foreach ($values as $result) {
      $asin = $this->get_value($result, NULL, TRUE);
      if (!empty($asin)) {
        $asins[] = $asin;
      }
    }

    if($asins) {
      $query = db_select('amazon_item_participant', 'aip')
        ->condition('asin', $asins, 'IN');

      if ($this->options['participant_type'] != '**ALL**') {
        $query = $query->condition('type', $this->options['participant_type']);
      }
      $query->fields('aip', array('participant', 'asin', 'type'));

      $results = $query->execute();

      while ($record = $results->fetchAssoc()) {
        $this->items[$record['asin']][] = array(
          'participant' => check_plain($record['participant']),
          'type' => check_plain($record['type'])
        );
      }
    }
  }

  function render_item($count, $item) {
    return $item['participant'];
  }

  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-name' . ']'] = t('The name of the participant.');
    $tokens['[' . $this->options['id'] . '-type' . ']'] = t('The participant type.');
  }

  function add_self_tokens(&$tokens, $item) {
    $tokens['[' . $this->options['id'] . '-name' . ']'] = $item['participant'];
    $tokens['[' . $this->options['id'] . '-type' . ']'] = $item['type'];
  }
}
