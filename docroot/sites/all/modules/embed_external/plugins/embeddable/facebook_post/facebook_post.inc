<?php

/**
 * @file
 * Implements support for embedding Facebook posts.
 */

$plugin = array(
  'title' => 'Facebook Post',
  'description' => 'Embed a single post from Facebook',
  'category' => 'Social Media',
  'render_callback' => 'embed_external_facebook_post_render',
  'configure_form' => 'embed_external_facebook_post_configure',
  'configure_form_validate' => 'embed_external_facebook_post_configure_validate',
  'arguments' => array(
    'required' => array('post_url'),
    'optional' => array(),
  ),
);

/**
 * Configuration form.
 */
function embed_external_facebook_post_configure(&$form_state) {
  $form['facebook_post_post_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Post URL'),
    '#description' => t('Example: <em>https://www.facebook.com/nbcuniversal/posts/824821020912509</em>'),
    '#size' => 60,
    '#maxlength' => 1000,
    '#required' => TRUE,
    '#default_value' => !empty($form_state['configuration']->post_url) ? $form_state['configuration']->post_url : '',
  );

  return $form;
}

/**
 * Render callback for Facebook posts.
 */
function embed_external_facebook_post_render($html_dom, $attributes) {
  static $facebook_js_included = FALSE;

  $container_element = $html_dom->createElement('div');
  $container_element->setAttribute('class', 'embed-external-facebook-post');

  // If the Facebook JavaScript SDK hasn't been initialized yet, include it.
  if (!$facebook_js_included) {
    $fb_root = $html_dom->createElement('div');
    $fb_root->setAttribute('id', 'fb-root');
    $container_element->appendChild($fb_root);

    $fb_js = <<<EOL
(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/en_US/sdk.js#xfbml=1&version=v2.2";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));
EOL;
    $fb_js = $html_dom->createElement('script', $fb_js);
    $container_element->appendChild($fb_js);
    $facebook_js_included = TRUE;
  }

  $post_embed = $html_dom->createElement('div');
  $post_embed->setAttribute('class', 'fb-post');
  $post_embed->setAttribute('data-href', $attributes['post_url']);
  $post_embed->setAttribute('data-width', 500);

  $container_element->appendChild($post_embed);

  return $container_element;
}


/**
 * Form validation handler for embed_external_facebook_post_configure().
 * @see embed_external_facebook_post_configure().
 */
function embed_external_facebook_post_configure_validate(&$form_state, $form) {
  $facebook_post = trim($form_state['values']['facebook_post_post_url']);
  $pattern = '/^(https?:\/\/)?((w{3}\.)?)facebook.com\/(([a-z\d.]{5,})?)/';
  $message = t('@facebook_post is not a valid facebook post.', array('@facebook_post' => $facebook_post));
  if (!preg_match($pattern, $facebook_post, $matches)) {
    form_set_error('facebook_post_post_url', $message);
  }
}
