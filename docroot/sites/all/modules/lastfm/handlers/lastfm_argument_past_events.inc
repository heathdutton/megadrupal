<?php
// $Id;

/**
 * @file
 *   Last.fm argument handler for arguments that can get either current or
 *   past events. 
 */

/**
 * Argument handler for retrieving past events. 
 *
 * If an argument can retrieve either current or past events, it implements
 * this class in order to offer this option.
 */
class lastfm_argument_past_events extends lastfm_argument_text {
  
  function option_definition() {
    $options = parent::option_definition();
    $options['past_events'] = array('' => '-');
    return $options;
  }

  function options_form(&$form, $form_state) {
    parent::options_form($form, $form_state);
    $form['past_events'] = array(
      '#title' => t('Show past events'),
      '#description' => t('Display only past events instead of future events.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['past_events'],
    );
  }

  function query() {
    if (isset($this->argument, $this->definition['lastfm'])) {
      $this->query->args[$this->definition['lastfm']['arg']] = $this->argument;

      // If the box to show past events is checked, then change the method to be this
      // argument's past_events_method as defined in the argument definition. Otherwise
      // use the standard method which only shows current events.
      if ($this->options['past_events']) {
        $this->query->args['method'] = $this->definition['lastfm']['past_events_method'];
      }
      else {
        $this->query->args['method'] = $this->definition['lastfm']['method'];        
      }
    }
  }

}
