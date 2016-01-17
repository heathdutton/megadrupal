<?php
/**
 * @file
 * Provide views data and handlers for weblinks.module
 * Drupal 7 Version.
 */

/**
 * Field handler to provide renderer that turns a URL into various types of output.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_weblinks extends views_handler_field {

  /**
   * Modify the query to make sure title is available.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['title'] = array('table' => 'node', 'field' => 'title');
  }

  /**
   * Define options available for this field.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_type'] = array('default' => 'url');
    return $options;
  }

  /**
   * Build option configuration form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_type'] = array(
      '#title' => t('Show link as'),
      '#type' => 'select',
      '#options' => array(
        'url' => 'URL',
        'text' => 'Text',
        'title' => 'Title',
        'visit' => '"Link displays as" setting',
        ),
      '#default_value' => $this->options['link_type'],
    );
  }

  /**
   * Render field output to the browser.
   */
  function render($values) {
    $title = $values->{$this->aliases['title']};
    $url = $this->get_value($values);
    $type = $this->options['link_type'];
    switch ($type) {
      case 'url':
        $link = l($url, $url, _weblinks_set_attributes());
        return l($url, $url, _weblinks_set_attributes());

      case 'title':
        return l($title, $url, _weblinks_set_attributes());

      case 'visit':
        $values->url = $url;
        $values->title = $title;
        return theme('weblinks_node_view', array('node' => $values, 'options' => _weblinks_set_attributes()));

      default:  // Also 'text'
        return check_plain($url);
    }
  }
}

/**
 * Helper function to return attributes.
 */
function _weblinks_set_attributes() {
  $options = array('attributes' => array('rel' => 'tag'));
  if (variable_get('weblinks_external', TRUE)) {
    $options['attributes']['target'] = '_blank';
  }

  if (variable_get('weblinks_nofollow', FALSE)) {
    $options['attributes']['rel'] = 'nofollow, tag';
  }
  return $options;
}
