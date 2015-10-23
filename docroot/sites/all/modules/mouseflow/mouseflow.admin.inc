<?php

/**
 *      @file
 *      mouseflow.admin.inc
 *      this file will be delete and it's code add to the original module.
 *
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/**
 *
 *
 *
 *
 *
 */
function mouseflow_configure_options($form_state) {


  $form = array();



  $form['mouseflow_code'] = array(
    '#title' => t('Please add mouseflow js code'),
    '#type' => 'textarea',
    '#default_value' => variable_get('mouseflow_code', NULL)
  );




  return system_settings_form($form);
}
