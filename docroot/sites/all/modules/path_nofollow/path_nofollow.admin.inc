<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function path_nofollow_settings_form() {
  $form = array(
    PATH_NOFOLLOW_PATH_MATCH => array(
      '#type' => 'textarea',
      '#title' => 'Hide for search engines',
      '#description' => 'Enter paths (one per line) to exclude from search engine indexing. Wildcards are allowed. (ie: admin/*)',
      '#default_value' => variable_get(PATH_NOFOLLOW_PATH_MATCH, 'admin/*'),
    ),
  );
  
  return system_settings_form($form);
}