INSTALLATION
============

Enable the module at Administer >> Modules.


USAGE
============

You may user new Forms API element type: transfer_slider.

Example:

function example_form() {

  $form['slider'] = array(
    '#type' => 'transfer_slider',
    '#title' => t('Slider test'),
    '#left_value' => 0,
    '#right_value' => 200,
    '#left' => t('Left input'),
    '#right' => t('Right input'),
    '#size' => 4,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}

function example_form_submit($form, &$form_state) {
  $left_value   = $form_state['values']['slider']['left'];
  $right_value  = $form_state['values']['slider']['right'];  
  drupal_set_message(t('Left value is !left_value. Right value is !right_value.', array('!left_value' => $left_value, '!right_value' => $right_value)));
}

DEVELOPERS
===========

Initial development: Roman Grachev (http://graker.ru/)
Futher development: Maslouski Yauheni (http://drupalace.ru/)