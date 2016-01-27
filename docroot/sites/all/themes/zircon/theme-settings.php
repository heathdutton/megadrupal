<?php
function zircon_form_system_theme_settings_alter(&$form, $form_state) {
  $form['nucleus']['about_nucleus'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feedback'),
    '#weight' => 40,
  );

  $form['nucleus']['about_nucleus']['about_nucleus_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('about-nucleus-wrapper')),
  );  
  $form['nucleus']['about_nucleus']['about_nucleus_wrapper']['about_nucleus_content'] = array(
    '#markup' => '<iframe width="100%" height="650" scrolling="no" class="nucleus_frame" frameborder="0" src="http://www.weebpal.com/static/feedback/"></iframe>',
  );
}
