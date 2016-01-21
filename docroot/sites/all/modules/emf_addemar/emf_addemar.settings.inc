<?php

/**
 * This file is part of EMF Addemar Plugin.
 *
 * EMF Addemar Plugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * EMF Addemar Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EMF Addemar Plugin.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * Settings functions and callbacks.
 */

/**
 * Page callback for the Addemar plugin administration page.
 */
function emf_addemar_settings($form, $form_state) {
  $form = array();

  use_soap_error_handler(false);

  // info
  $form['emf_addemar_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#description' => t('Your Addemar token.'),
    '#default_value' => variable_get('emf_addemar_token', ''),
    '#required' => TRUE,
  );

  $form['emf_addemar_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Server'),
    '#description' => t('The Addemar server to connect to, by default this is ws.email.addemar.com'),
    '#default_value' => variable_get('emf_addemar_server', 'ws.email.addemar.com'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

