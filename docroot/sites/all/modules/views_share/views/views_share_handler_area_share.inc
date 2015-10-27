<?php

/**
 * Base class for area handlers.
 *
 * @ingroup views_area_handlers
 */
class views_share_handler_area_share extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();
    $options['share_label'] = array('default' => 'Share this view', 'translatable' => TRUE); 
    $options['embed_label'] = array('default' => 'View original', 'translatable' => TRUE);
    $options['embed_width'] = array('default' => 400);
    $options['embed_height'] = array('default' => 250);
    $options['embed_style'] = array('default' => 'margin:0 auto; border:0;');
    $options['hide_exposed_filters'] = array('default' => FALSE, 'bool' => TRUE);
    $options['hide_header_footer'] = array('default' => FALSE, 'bool' => TRUE);
    $options['hide_attachments'] = array('default' => FALSE, 'bool' => TRUE);
    $options['rewrite_links'] = array('default' => FALSE, 'bool' => TRUE);
    $options['oembed'] = array('default' => FALSE, 'bool' => TRUE);
    $options['oembed_author_name'] = array('default' => '');
    $options['oembed_author_url'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['share_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label for share link'),
      '#description' => t('The label shown on the original view for the button that opens up the sharing dialog.'),
      '#default_value' => $this->options['share_label'],
    );
    $form['embed_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label for original link'),
      '#description' => t('The label shown on the embedded view for the button that links back to the original view.'),
      '#default_value' => $this->options['embed_label'],
    );
    $form['embed_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Embeddable frame width'),
      '#default_value' => $this->options['embed_width'],
      '#field_suffix' => 'px',
      '#size' => 10,
    );
    $form['embed_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Embeddable frame height'),
      '#default_value' => $this->options['embed_height'],
      '#field_suffix' => 'px',
      '#size' => 10,
    );
    $form['embed_style'] = array(
      '#type' => 'textarea',
      '#title' => t('Embeddable frame style'),
      '#default_value' => $this->options['embed_style'],
    );
    $form['hide_exposed_filters'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide exposed filters when embedding'),
      '#default_value' => $this->options['hide_exposed_filters'],
    );
    $form['hide_header_footer'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide header/footer elements when embedding'),
      '#default_value' => $this->options['hide_header_footer'],
    );
    $form['hide_attachments'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide attachments when embedding'),
      '#default_value' => $this->options['hide_attachments'],
    );
    $form['rewrite_links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Rewrite links to open in new tab when embedding'),
      '#default_value' => $this->options['rewrite_links'],
    );
    $query = (array)$this->view->exposed_input + array('format' => 'output_format');
    $form['oembed'] = array(
      '#type' => 'checkbox',
      '#title' => t('Provide oEmbed support'),
      '#default_value' => $this->options['oembed'],
      '#description' => t('Generate oEmbed discovery tags on the page and respond to oEmbed calls. The oEmbed endpoint for this display is <a href="!endpoint">!endpoint</a> where <code>output_format</code> can be "json" or "xml".', array(
        '!endpoint' => _views_share_embed_url($this->view, $query, TRUE, TRUE),
      )),
    );
    $form['oembed_author_name'] = array(
      '#type' => 'textfield',
      '#title' => t('oEmbed author name'),
      '#default_value' => $this->options['oembed_author_name'],
      '#dependency' => array('edit-options-oembed' => array(TRUE)),
      '#description' => t('Generate an "author_name" entry representing the name of the author/owner of this oEmbed resource (optional).'),
    );
    $form['oembed_author_url'] = array(
      '#type' => 'textfield',
      '#title' => t('oEmbed author URL'),
      '#default_value' => $this->options['oembed_author_url'],
      '#dependency' => array('edit-options-oembed' => array(TRUE)),
      '#description' => t('Generate an "author_url" entry representing the URL of the author/owner of this oEmbed resource (optional).'),
    );    
  }

  function render($empty = FALSE) {
    $query = drupal_get_query_parameters($_GET, array('q', 'referer'));

    // Show original view link when embedding.
    if (!empty($this->view->is_embedding)) {
      list($url, $options) = _views_share_view_url($this->view, $query);
      $options += array(
        'attributes' => array(
          'target' => '_blank',
          'class' => array('views-share-embedded'),
      ));
      return l($this->options['embed_label'], $url, $options);
    }

    // Add oEmbed metadata.
    if ($this->options['oembed']) {
      $this->add_oembed_head('json', $query);
      $this->add_oembed_head('xml', $query);
    }

    // Create our own javascript that will be used to theme a modal.
    ctools_include('ajax');
    ctools_include('modal');
    ctools_modal_add_js();

    $style = array(
      'views-share' => array(
        'modalSize' => array(
          'type' => 'scaled',
          'width' => 500,
          'height' => 240,
          'addWidth' => 0,
          'addHeight' => 0,
          'contentRight' => 0,
          'contentBottom' => 0,
        ),
        'modalTheme' => 'viewsShareModal',
        'modalOptions' => array(
          'opacity' => .5,
          'background-color' => '#FFF',
        ),
        'animation' => 'fadeIn',
        'animationSpeed' => 'normal',
        'closeText' => t('Close'),
      ),
    );

    // AJAX views can cause this to be called multiple times, which ends up confusing CTools.
    static $once = FALSE;
    if (!$once) {
      drupal_add_js($style, 'setting');
      $once = TRUE;
    }
    drupal_add_js(drupal_get_path('module', 'views_share') . '/theme/views-share.modal.js');
    drupal_add_css(drupal_get_path('module', 'views_share') . '/theme/views-share.modal.css');
    drupal_add_library('system', 'drupal.ajax');

    $args = empty($this->view->args) ? '' : '/' . implode('/', $this->view->args);
    $button_href = "view/{$this->view->name}/{$this->view->current_display}/nojs/share{$args}";
    $button_text = $this->options['share_label'];
    $button_attributes = array(
      'title' => t('Share @name', array('@name' => $this->view->display_handler->get_option('title'))),
      'class' => array('ctools-use-modal', 'ctools-modal-views-share', 'views-share'),
      'rel' => 'nofollow',
      'name' => _views_share_hashtag($this->view),
    );
    $button_query = drupal_get_query_parameters($_GET, array('q'));
    $button_query['referer'] = $_GET['q'];
    return l($button_text, $button_href, array(
      'query' => $button_query,
      'attributes' => $button_attributes,
    ));
  }

  /**
   * Generate oEmbed HEAD link tag for discovery.
   * @see http://oembed.com/#section4
   */
  function add_oembed_head($format, $query) {
    // TODO: Refresh link when AJAX view exposed filters are changed.
    $oembed_query = $query + array('format' => $format);
    $oembed_tag = array(
      '#tag' => 'link',
      '#attributes' => array(
        'rel' => 'alternate',
        'type' => "application/{$format}+oembed",
        'title' => $this->view->get_title(),
        'href' => _views_share_embed_url($this->view, $oembed_query, TRUE, TRUE),
      ),
    );
    drupal_add_html_head($oembed_tag, "oembed_{$format}_{$this->view->name}");
  }

}
