<?php

function asaf_example_api_depended_elements($form, &$form_state) {
  $form['time'] = array(
    '#markup' => t('Form generation time: @time', array('@time' => date('H:i:s'))),
  );

  $form['year_chooser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hierarchical year chooser'),
    '#collapsible' => TRUE,
  );



  $centuries = array();
  for ($i = 1; $i <= date('Y')/100+1; $i++) {
    $centuries[$i] = $i;
  }

  $form['year_chooser']['century'] = array(
    '#type' => 'select',
    '#title' => t('Century'),
    '#options' => array('' => '') + $centuries,
    '#asaf_control' => 'century',  // #asaf_control attribute means that this element can generate AJAX request to
                                   // redraw one or more form areas. This attribute value could be used in the form
                                   // submission or form constructor to define an element invoked form redrawing.

    '#asaf_target_area' => 'decade_and_year', // One or more areas names (array) which should be send to a browser.
                                              // If this attribute is empty, the default area (the whole form) will be
                                              // redrawn.
  );




  $form['year_chooser']['decade_and_year'] = array(
    '#asaf_area' => 'decade_and_year' // Registering area name. This name identifies the area which could be redrawn by
                                      // any Asaf-control (see #asaf_target_area above).
  );

  if (isset($form_state['values']['century']) && $form_state['values']['century']) {


    $decades = array();
    for($i = ($form_state['values']['century']-1)*100; $i < ($form_state['values']['century'])*100  && $i <= date('Y'); $i = $i+10) {
      $name = $i/10 . 'x';
      $decades[$i] = $name;
    }

    $form['year_chooser']['decade_and_year']['decade'] = array(
      '#type' => 'select',
      '#title' => t('Decade'),
      '#options' => array('' => '') + $decades,
      '#description' => t('Element generation time: @time', array('@time' => date('H:i:s'))),
      '#asaf_control' => 'decade',
      '#asaf_target_area' => 'year',
    );


    if (isset($form_state['values']['decade']) && $form_state['values']['decade'] && isset($decades[$form_state['values']['decade']])) {
      $range = array();
      for ($i = $form_state['values']['decade']; $i < $form_state['values']['decade'] + 10 && $i <= date('Y'); $i++) {
        $range[$i] = $i;
      }

      $form['year_chooser']['decade_and_year']['year'] = array(
        '#type' => 'select',
        '#title' => t('Year'),
        '#options' => $range,
        '#description' => t('Element generation time: @time', array('@time' => date('H:i:s'))),
        '#asaf_area' => 'year',
      );
    }
    else {
      // Even if an element is not going to be displayed on the form, we need to register an empty redrawable area to
      // have a wrapper on the client side, which will be used to insert an area content later.
      $form['year_chooser']['decade_and_year']['year'] = array(
        '#asaf_area' => 'year',
      );
    }
  }

  asaf_prepare_form($form, $form_state);

  return $form;
}