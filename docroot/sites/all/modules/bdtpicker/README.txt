
Requirements

  - Bootstrap (library)
    Integrate "bootstrap" library somehow via the hook_libraries_info().
    The https://www.drupal.org/project/bootstrap_library module does the job for
    you.
    Bootstrap library requires jQuery version >= 1.9, so the jQuery Update
    module is also required. https://www.drupal.org/project/jquery_update

  - Bootstrap Date & Time picker (library)
    https://github.com/Eonasdan/bootstrap-datetimepicker
    https://github.com/Eonasdan/bootstrap-datetimepicker/releases
    https://github.com/Eonasdan/bootstrap-datetimepicker/archive/v3.1.3.tar.gz

    After unpacking there is a file:
    sites/all/libraries/bootstrap-datetimepicker/build/js/bootstrap-datetimepicker.min.js

  - Dependencies
    - https://www.drupal.org/project/libraries
      - libraries
    - https://www.drupal.org/project/date
      - date
      - date_api
    - https://www.drupal.org/project/moment
      - moment


Install

  Documentation missing.


Example

  However this module is designed to work together with Date Fields, but also
  works with plain "date_text" form elements.

  function mymodule_bdtpicker_demo_form($form, &$form_state) {
    $form['my_interval_start'] = array(
      '#type' => 'date_text',
      '#title' => t('My interval start'),
      '#bdtpicker' => TRUE,
      '#datepicker_next' => array(
        'name' => 'my_interval_end[date]',
        'type' => 'end',
      ),
      '#datepicker_chain' => array(
        'my_interval_end[date]' => 'next',
      ),
    );

    $form['my_interval_end'] = array(
      '#type' => 'date_text',
      '#title' => t('My interval end'),
      '#bdtpicker' => TRUE,
      '#datepicker_next' => array(
        'name' => 'my_interval_start[date]',
        'type' => 'start',
      ),
      '#datepicker_chain' => array(
        'my_interval_start[date]' => 'previous',
      ),
    );

    $form['actions'] = array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save'),
      ),
    );

    return $form;
  }

  function mymodule_bdtpicker_demo_form_submit(&$form, &$form_state) {
    drupal_set_message("<pre>\$form_state['values'] = " . check_plain(print_r($form_state['values'], TRUE)) . '</pre>');
  }
