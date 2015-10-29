<?php

/**
 * @file
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 * class inherited from handler for Colorbox module.
 */

/**
 * A handler to provide a field that is completely customizable by the administrator.
 *
 * @ingroup views_field_handlers
 */
class imagepicker_colorbox_handler_field_colorbox extends colorbox_handler_field_colorbox {

  function option_definition() {
    $options = parent::option_definition();
    $options['hide'] = array('default' => 1);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Get a list of the available fields and arguments for token replacement.
    $options = array();
    $fields = array('trigger_field' => t('<None>'));
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $options[t('Fields')]["[$field]"] = $handler->ui_name();
      // We only use fields up to (and including) this one.
      if ($field == $this->options['id']) {
        break;
      }
      // If the instance has a custom name use that or use the title.
      // This helps if the same field is used more than once.
      $fields[$field] = (isset($handler->options['ui_name']) && $handler->options['ui_name'] ? $handler->options['ui_name'] : $handler->definition['title']);
    }

    $form['trigger_field'] = array(
      '#type' => 'select',
      '#title' => t('Trigger field'),
      '#description' => t('Select the field that should be turned into the trigger for the Colorbox.  Only fields that appear before this one in the field list may be used.'),
      '#options' => $fields,
      '#default_value' => $this->options['trigger_field'],
      '#weight' => -12,
    );

    $opts = array(0 => t('All'));
    $opts += drupal_map_assoc(range(1, 50));
    $form['hide'] = array(
      '#type' => 'select',
      '#title' => t('Links to show'),
      '#description' => t('How many links to show.'),
      '#options' => $opts,
      '#default_value' => $this->options['hide'],
      '#weight' => -5,
    );

  }

  function render($values) {
    // We need to have multiple unique IDs, one for each record.
    static $i = 0;
    $i++;

    // Return nothing if no trigger filed is selected.
    if (empty($this->options['trigger_field'])) {
      return;
    }

    // Get the token information and generate the value for the popup and the
    // caption.
    $tokens = $this->get_render_tokens($this->options['alter']);
    $popup = filter_xss_admin($this->options['popup']);
    $caption = filter_xss_admin($this->options['caption']);
    $popup = strtr($popup, $tokens);
    $caption = strtr($caption, $tokens);

    $width = $this->options['width'] ? $this->options['width'] : '600px';
    $height = $this->options['height'] ? $this->options['height'] : '400px';
    $gallery_id = !empty($this->options['custom_gid']) ? $this->options['custom_gid'] : ($this->options['gid'] ? 'gallery-' . $this->view->name : '');
    $link_text = $tokens["[{$this->options['trigger_field']}]"];
    $hide = $this->options['hide'];
    $link_options = array(
      'html' => TRUE,
      'fragment' => 'colorbox-inline-' . $i,
      'query' => array(
        'width' => $width,
        'height' => $height,
        'title' => $caption,
        'inline' => 'true'
      ),
      'attributes' => array(
        'class' => 'colorbox-inline' . ($hide && $i > $hide ? ' js-hide' : ''),
        'rel' => $gallery_id
      )
    );

    $link_tag = l($link_text, '', $link_options);

    // The outside div is there to hide all of the divs because if the specific Colorbox
    // div is hidden it won't show up as a Colorbox.
    return $link_tag . '<div style="display: none;"><div id="colorbox-inline-' . $i . '">' . $popup . '</div></div>';

  }

}
