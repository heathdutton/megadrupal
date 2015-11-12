<?php
/**
 * Created by JetBrains PhpStorm.
 * User: Nico Heulsen
 * Email: info@artx.be
 * Date: 28/11/11
 * Time: 16:53
 */
 

/**
 * Return the main page of the backdrop items
 *
 * @return string
 */
function backdrop_list_page() {
  $output = '';
  $path = drupal_get_path('module', 'backdrop');

  //add css and js
  drupal_add_css($path . '/css/backdrop.css');
  drupal_add_js($path . '/js/backdrop.js');

  //upload form + button
  $output .= '<div class="backdrop-upload-wrapper">';
  $output .= l(t('+ Add backdrop'), $_GET['q'], array('attributes' => array('id' => 'lnk-add-backdrop')));

  $upload_form = drupal_get_form('backdrop_upload_form');
  $output .= drupal_render($upload_form);
  $output .= '</div>';

  //backdrop overview
  $backdrops = backdrop_load_all_backdrops();

  $output .= '<div class="backdrop-overview-wrapper">';
  $output .= '  <div class="backdrop-active clearfix">';
  $output .= '  <h2>' . t('Active backdrop') . '</h2>';
  //show active backdrop
  if (!empty($backdrops[1])) {
    backdrop_prepare_backdrop($backdrops[1][0]);
    $output .= theme('backdrop_active', array('backdrop' => $backdrops[1][0]));
  }
  $output .= '</div>';

  $output .= '  <div class="backdrop-inactive clearfix">';
  $output .= '  <h2>' . t('Inactive backdrops') . '</h2>';
  //show inactive backdrops
  if (!empty($backdrops[0])) {
    $items = array();
    foreach ((array)$backdrops[0] as $raw) {
      backdrop_prepare_backdrop($raw);
      $output .= theme('backdrop_inactive', array('backdrop' => $raw));
    }
  }
  $output .= '  </div>';
  $output .= '</div>';


  return $output;
}

/**
 * Set the currennt backdrop as active backdrop
 *
 * @param $backdrop
 * @return void
 */
function backdrop_set_active_form($form, &$form_state, $backdrop = NULL) {
  $form['backdrop'] = array(
    '#type' => 'value',
    '#value'=> $backdrop,
    );

  $question = t('Are you sure you want to set backdrop <strong>' . $backdrop['name'] . '</strong> active?');
  $path = 'admin/structure/media/backdrop';
  $description = 'Settings this backdrop active will disable the current backdrop.';

  return confirm_form($form, $question, $path, $description);
}

/**
 * Submithandler set backdrop active
 *
 * @param $form
 * @param $form_state
 * @param null $backdrop
 * @return void
 */
function backdrop_set_active_form_submit($form, &$form_state, $backdrop = NULL) {
  if ($form_state['values']['confirm']) {
    backdrop_set_all_backdrops_inactive();
    backdrop_set_backdrop_active($form_state['values']['backdrop']['id']);
  }

  drupal_goto('admin/structure/media/backdrop');
}

/**
 * Set a backdrop inactive
 *
 * @param $backdrop
 * @return array
 */
function backdrop_set_inactive_form($form, &$form_state, $backdrop = NULL) {
  $form['backdrop'] = array(
    '#type' => 'value',
    '#value'=> $backdrop,
    );

  $question = t('Are you sure you want to set backdrop <strong>' . $backdrop['name'] . '</strong> inactive?');
  $path = 'admin/structure/media/backdrop';
  $description = 'Settings this backdrop inactive will remove the backgroundstyle.';

  return confirm_form($form, $question, $path, $description);
}

/**
 * Submithandler set backdrop inactive
 *
 * @param $form
 * @param $form_state
 * @param null $backdrop
 * @return void
 */
function backdrop_set_inactive_form_submit($form, &$form_state, $backdrop = NULL) {
  if ($form_state['values']['confirm']) {
    backdrop_set_all_backdrops_inactive();
  }

  drupal_goto('admin/structure/media/backdrop');
}
