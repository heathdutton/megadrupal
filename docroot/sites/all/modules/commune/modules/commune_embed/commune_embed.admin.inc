<?php

/**
 * @file
 * Contains the administrative functions of the Shout Box module.
 *
 * This file is included by the Shout box admin settings form.
 */

/**
 * Admin configure form control on page.
 */
function _commune_embed_configure_form() {

 $form['clear_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear cache'),
  );

  $form['clear_cache']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear Cached Embeds'),
    '#submit' => array('commune_embed_clear_cache'),
  );

  $form['commune_embed'] = array (
    '#type' => 'fieldset',
    '#title' => t('Enable embedly'),
  );

  $form['commune_embed']['commune_embed_use_embedly'] = array (
      '#type' => 'checkbox',
      '#title' => t ('Use Embedly for Preview'),
      '#default_value' => variable_get ('commune_embed_use_embedly', FALSE), 
  );

  $form['commune_embed']['commune_embed_embedly_key'] = array (
      '#type' => 'textfield',
      '#title' => t ("Developer key for embedly"),
      '#default_value' => variable_get ('commune_embed_embedly_key'),
      '#description' => t ('Please enter your embedly key here. With Embedly you can embed media from over 250 providers, and surface popular content with analytics. You can gain deeper insights on the articles shared on your site, and improve load times with image optimization. Learn more at ') . l (t ('embed.ly'), 'http://embed.ly') . ' and open a free account to get started. You can still use the module without embed.ly but links and files will be displayed only as links',
  );
  
  $form['commune_embed_image'] = array (
    '#type' => 'fieldset',
    '#title' => t('Image Style'),
  );

  $form['commune_embed_image']['commune_embed_image_style'] = array (
      '#type' => 'select',
      '#title' => t ('Select Image Style to Display embedded images'),
      '#default_value' => variable_get ('commune_embed_image_style', ''),
      '#options' => image_style_options(), 
  );

  return system_settings_form ($form);
}

function commune_embed_clear_cache($form, &$form_state) {
  db_delete('commune_embed_cache')->execute();
  drupal_set_message(t('All Embeds cleared.'));
}

