<?php


/**
 * Display the settings form.
 */
function responsive_imagemaps_admin_settings($form, &$form_state) {
  $options = array(
    RESPONSIVE_IMAGEMAPS_ALWAYS => t('All pages'),
    RESPONSIVE_IMAGEMAPS_LISTED => t('Only the listed pages'),
  );
  
  $form['responsive_imagemaps_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Enable responsive image maps on specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('responsive_imagemaps_visibility', RESPONSIVE_IMAGEMAPS_ALWAYS),
  );
  
  $form['responsive_imagemaps_pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">Pages</span>',
    '#default_value' => variable_get('responsive_imagemaps_pages', ''),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#states' => array(
      // Only show this field when the 'Only the listed pages' option is enabled.
      'visible' => array(
        ':input[name="responsive_imagemaps_visibility"]' => array('value' => RESPONSIVE_IMAGEMAPS_LISTED),
      ),
    ),
  );
 
  return system_settings_form($form);
}
