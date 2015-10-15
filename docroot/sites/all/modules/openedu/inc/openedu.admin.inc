<?php

/**
 * openedu.admin.inc
 * Developed @ ImageX Media http://www.imagexmedia.com/
 * @package OpenEDU
 * @author Shea McKinney <shea@imagexmedia.com> @sherakama
 *
 * Description:
 * Administrative and configuration pages for the OpenEDU suite.
 *
 * @todo
 *  - Create a list of todo items.
 *
 *
 */

/**
 * Implements hook_form().
 */
function openedu_admin_config_form($form, &$form_state) {


  $form['openedu_text'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#title' => t('Introduction'),
  );

    $form['openedu_text']['introduction'] = array(
      '#markup' => t('For more information on OpenEDU please visit') . " " .
                  l('Our Website', 'http://www.imagexmedia.com/openedu/'),
    );




  // Extra Handlers
  $form['#validate'][] = 'openedu_admin_config_form_validate';
  $form['#submit'][] = 'openedu_admin_config_form_submit';

  return system_settings_form($form);
}

/**
 * OpenEDU admin config settings form custom validate actions
 * @param $form
 * @param $form_state
 *
 */

function openedu_admin_config_form_validate($form, $form_state)
{
  // Nothing here yet.
}


/**
 * OpenEDU admin config settings form custom submit actions
 * @param $form
 * @param $form_state
 *
 */

function openedu_admin_config_form_submit($form, $form_state)
{
  // Nothing here yet.
}
