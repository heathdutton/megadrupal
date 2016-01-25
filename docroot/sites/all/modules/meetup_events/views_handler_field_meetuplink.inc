<?php
/**
 * Render a field as a meetup url
 *
 */
class views_handler_field_meetuplink extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['rsvp_text'] = '';
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['rsvp_text'] = array(
      '#title' => t('RSVP text'),
      '#type' => 'textfield',
      '#default_value' => $this->options['rsvp_text'],
      '#description' => t('What you want the RSVP text link to be')
    );
  }

   function render($values) {
     $value = $values->{$this->field_alias};
     $api = variable_get('meetup_api', array());
     $link = '<a href="http://www.meetup.com/' . $api["group_url"] .
       '/events/' . $value . '/"  data-event="' . $value .
       '" class="mu-rsvp-btn">' . $this->options['rsvp_text'] . '</a>';
     return filter_xss($link);
   }
}
