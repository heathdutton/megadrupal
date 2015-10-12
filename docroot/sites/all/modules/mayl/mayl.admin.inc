<?php
/**
 * @file
 * Settings for mirror-as-you-link module.
 */

/*
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; version 2 of the License.
 *  
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *  
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @author Victor Shnayder (shnayder seas.harvard.edu)
 * @author Dan Margo (dmargo eecs.harvard.edu)
 *
 */

/**
 * Settings form for MAYL.
 */
function mayl_admin_settings() {
  $form['mayl_href_css_class'] = array(
    '#title' => t('Cited link class'),
    '#description' => t('CSS class for cited links.'),
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#default_value' => variable_get('mayl_href_css_class', 'mirrored'),
  );
  $form['mayl_mirror_css_class'] = array(
    '#title' => t('Mirror link class'),
    '#description' => t('CSS class for links to mirrors.'),
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#default_value' => variable_get('mayl_mirror_css_class', 'mirror'),
  );
  $form['mayl_wget_path'] = array(
    '#title' => t('Wget path'),
    '#description' => t('Path to wget on the server that runs Drupal'),
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#default_value' => variable_get('mayl_wget_path', '/usr/bin/wget'),
    '#after_build' => array('_mayl_admin_wget_check'),
  );
  $form['mayl_user_agent'] = array(
    '#title' => t('User agent'),
    '#description' => t('User agent to use when mirroring'),
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#default_value' => variable_get('mayl_user_agent', 'Mozilla/5.0'),
  );
  $form['mayl_mirror_directory'] = array(
    '#title' => t('Mirror directory'),
    '#description' => t('Directory to store mirrored content in.'),
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#default_value' => variable_get('mayl_mirror_directory', 'public://mirror'),
    '#after_build' => array('system_check_directory'),
  );

  return system_settings_form($form);
}

