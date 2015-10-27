<?php
/**
 * @file
 *  mailtemplate.php
 *
 * Preprocess and custom theme functions for the Wildfire example template
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *

/**
 * Preprocessor function for the Wildfire example template
 *
 * There's a couple of pointers to be aware of with creating your own templates:
 *
 * 1/ Use the wildfire theme functions rather than the Drupal defaults
 *  There are a few custom wildfire template specific theme functions available
 *  to use. To ensure that paths to images are all constructed correctly, and
 *  that content is displayed right, please use these theme functions in the
 *  relevant places in the template:
 *
 *    wildfire_image
 *    wildfire_image_path
 *    wildfire_field_render
 *
 *  Usage examples for all the above theme functions can be found below in this
 *  example template.
 *
 * @param array $vars
 *  Array of template variables to preprocess.
 *
 * @return NULL
 */
function wildfire_example_newsletter_preprocess_mailtemplate(&$vars) {

  // As there is only one value in this region simplify this variable.
  $vars['snippets_editors_note'] = $vars['snippets_editors_note'][0];

  // All links should look the same, so let's pre-define their formatting.
  $vars['linkstyle'] = 'color: ' . $vars['colors']['links'] . '; text-decoration: none;';

  // Make a link to the site login
  $vars['login'] = l(t('log in'), 'user/login', array(
    'absolute' => TRUE,
    'attributes' => array(
      'style' => $vars['linkstyle'],
    )
  ));

  // Make a link to the 'contact-us' page on the site. You might have to adjust
  // this if your contact page resides on a different path.
  $vars['contact'] = l(
    theme(
      'wildfire_image',
      array(
        'template' => $vars['template']['name'],
        'path' => 'button-contact.gif',
        'attributes' => array(
          'style' => 'border: none; margin-top: 10px;'
        ),
        'getsize' => FALSE,
      )
    ),
    'contact-us',
    array(
      'absolute' => TRUE,
      'html' => TRUE,
    )
  );

  // As the code to generate images is quite long, we do that here at the top,
  // rather than inline to make the resulting HTML parts of the template easier
  // to follow.
  $vars['images'] = wildfire_example_newsletter_images($vars);

  // Uncomment this line to turn on the variable debugging, don't leave this
  // enabled on live sites!!

  // wildfire_variable_output($vars);
}


/**
 * We define any custom images through the theme_wildfire_image function. This
 * is because all images must have absolute paths for mailing out. This function
 * always looks in the {templates location}/mailtemplates/{template name}/images
 * folder first.
 *
 * @param array $vars
 *   Array of variables from the main preprocess function
 */
function wildfire_example_newsletter_images($vars) {

  $images = array();

  // The logo path is derived from the template 'logo' settings.
  $images['header'] = theme('wildfire_image',
    array(
      'template' => $vars['template']['name'],
      'path' => $vars['files']['logo'],
      'alt' => 'Logo',
      'title' => 'Logo',
    )
  );

  // Spacers are commonly used to fix table rendering errors in Outlook and
  // other non standards compliant email clients.
  $images['spacer'] = theme('wildfire_image',
    array(
      'template' => $vars['template']['name'],
      'path' => 'spacer.gif',
      'attributes' => array(
        'width' => 30,
        'height' => 1,
        'style' => 'border: none;'
      ),
    )
  );

  // Generate images for sidebar block titles, the use of alt text is important,
  // if a user is viewing the email without images (which most do by default)
  // then this is the fallback.
  $images['sidebar-issue'] = theme('wildfire_image',
    array(
      'template' => $vars['template']['name'],
      'path' => 'bg-sidebar-issue.gif',
      'alt' => 'In this issue',
      'title' => 'In this issue',
    )
  );

  $images['sidebar-categories'] = theme('wildfire_image',
    array(
      'template' => $vars['template']['name'],
      'path' => 'bg-sidebar-categories.gif',
      'alt' => 'Categories',
      'title' => 'Categories',
    )
  );

  $images['sidebar-contact'] = theme('wildfire_image',
    array(
      'template' => $vars['template']['name'],
      'path' => 'bg-sidebar-contact.gif',
      'alt' => 'Contact',
      'title' => 'Contact',
    )
  );

  $images['sidebar-unsubscribe'] = theme('wildfire_image',
    array(
      'template' => $vars['template']['name'],
      'path' => 'bg-sidebar-unsubscribe.gif',
      'alt' => 'Unsubscribe',
      'title' => 'Unsubscribe',
    )
  );

  return $images;

}

/**
 * Use this block to aid in debugging your template.
 * DON'T LEAVE ENABLED IN PRODUCTION TEMPLATES!
 *
 * @param bool $print
 *   Set to true to display variables available to the template
 */
function wildfire_variable_output($vars) {
  // Create a simple debug function
  if (function_exists('dsm')) {
    return dsm(get_defined_vars());
  }
  else {
    return drupal_set_message(t('<pre>' . print_r(get_defined_vars(), TRUE) . '</pre>'));
  }
}
