<?php

/**
 * @file
 * Definition of ShareMessage entity class.
 */


/*
 * Entity class for the ShareMessage entity.
 */
class ShareMessage extends Entity {

  /**
   * Overrides Entity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $profileid = variable_get('sharemessage_addthis_profile_id', 1);

    $context = array('sharemessage' => $this, 'view_mode' => $view_mode);
    if ($node = menu_get_object()) {
      $context['node'] = $node;
    }
    drupal_alter('sharemessage_token_context', $this, $context);

    // Add OG Tags to the page if there are none added yet and a corresponding
    // view mode was set (or it was altered into such a view mode above).
    $og_view_modes = array('full', 'only_og_tags', 'no_attributes');
    if (strpos(drupal_get_html_head(), 'property="og:') == FALSE && in_array($context['view_mode'], $og_view_modes)) {
      $this->addOGTags($context);
    }

    // Add addThis buttons.
    $content = array();
    if ($view_mode != 'only_og_tags') {
      $attributes_view_modes = array('full', 'attributes_only');
      $content['addthis'] = array(
        '#type' => 'container',
        '#attributes' => in_array($context['view_mode'], $attributes_view_modes) ? $this->buildAdditionalAttributes($context) : $this->buildAttributes(),
        'services' => array(
          '#markup' => $this->build_services_part($context),
        ),
        'additional_services' => array(
          '#markup' => $this->build_additional_services_part(),
        ),
        'addthis_js' => array(
          '#attached' => array(
            'js' => array(
              array(
                'data' => array(
                  'addthis_config' => array(
                    'data_track_addressbar' => TRUE,
                  ),
                ),
                'type' => 'setting',
              ),
              array(
                'data' => '//s7.addthis.com/js/300/addthis_widget.js#pubid=' . $profileid,
                'type' => 'external',
              ),
            ),
          ),
        ),
      );
    }

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Function that adds OG tags in the header of the page.
   */
  private function addOGTags($context) {
    // Basic Metadata (og:title, og:type, og:image, og:url).
    // OG: Title.
    $og_title = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => 'og:title',
        'content' => $this->getTokenizedField('sharemessage_title', $context),
      ),
    );
    drupal_add_html_head($og_title, 'og_title');

    // Base value for og:type meta tag.
    $type = 'website';
    // Add Multiple og tags used for sharing of videos.
    if ($video_url = $this->getTokenizedField('sharemessage_video_url', $context, NULL)) {
      $og_video = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
          'property' => 'og:video',
          'content' => $video_url . '?fs=1',
        ),
      );
      drupal_add_html_head($og_video, 'og_video');
      $og_video = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
          'property' => 'og:video:width',
          'content' => variable_get('sharemessage_shared_video_width', 360),
        ),
      );
      drupal_add_html_head($og_video, 'og_video_width');
      $og_video = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
          'property' => 'og:video:height',
          'content' => variable_get('sharemessage_shared_video_height', 270),
        ),
      );
      drupal_add_html_head($og_video, 'og_video_height');
      $og_video = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
          'property' => 'og:video:type',
          'content' => 'application/x-shockwave-flash',
        ),
      );
      drupal_add_html_head($og_video, 'og_video_type');
      // Override og:type to video.
      $type = 'video';
    }

    // OG: Image, can be used for video thumbnails as well.
    if (isset($this->sharemessage_image[LANGUAGE_NONE][0]['uri'])) {
      $image_url = file_create_url($this->sharemessage_image[LANGUAGE_NONE][0]['uri']);
    }
    else {
      $image_url = $this->getTokenizedField('sharemessage_image_url', $context);
    }
    if ($image_url) {
      $og_image = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
          'property' => 'og:image',
          'content' => $image_url,
        ),
      );
      drupal_add_html_head($og_image, 'og_image');
    }

    // OG: Type.
    $og_type = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => 'og:type',
        'content' => $type,
      ),
    );
    drupal_add_html_head($og_type, 'og_type');

    // OG: URL.
    $og_url = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => 'og:url',
        'content' => $this->getUrl($context),
      ),
    );
    drupal_add_html_head($og_url, 'og_url');

    // OG: Description.
    $og_description = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => 'og:description',
        'content' => $this->getTokenizedField('sharemessage_long', $context),
      ),
    );
    drupal_add_html_head($og_description, 'og_description');
  }

  /**
   * Function that adds icon style as part of addThis widget.
   */
  private function buildAttributes() {
    $icon_style = $this->getIconStyle();

    return array(
      'class' => array('addthis_toolbox', $this->getAddthisStyleClass($icon_style), $icon_style),
    );
  }

  /**
   * Function that adds icon style with addThis:attributes
   * (url, title, description) as part of addThis widget.
   */
  function buildAdditionalAttributes($context) {
    $attributes = $this->buildAttributes();
    $attributes['addthis:url'] = $this->getUrl($context);
    $attributes['addthis:title'] = $this->getTokenizedField('sharemessage_title', $context);
    $attributes['addthis:description'] = $this->getTokenizedField('sharemessage_long', $context);
    return $attributes;
  }

  /**
   * Gets a field value and runs it through token_replace().
   *
   * @param $field_name
   * @param $context
   * @param $default
   *
   * @return
   *   If existent, the field value with tokens replace, the default otherwise.
   */
  protected function getTokenizedField($field_name, $context, $default = '') {
    if ($items = field_get_items('sharemessage', $this, $field_name)) {
      return strip_tags(token_replace($items[0]['value'], $context));
    }
    return $default;
  }

  /**
   * Function that adds services as part of addThis widget.
   */
  private function build_services_part($context) {
    $services = !empty($this->settings['services']) ? $this->settings['services'] : variable_get('sharemessage_default_services', array());

    // Get some additional namespaced button attributes.
    $namespace_attributes = $this->getNameSpaceAttributes($this->getIconStyle());

    // Configured.
    $services_HTML = '';
    if (!empty($services)) {
      foreach ($services as $key => $service) {
        if ($key == 'twitter' && field_get_items('sharemessage', $this, 'sharemessage_short')) {
          // @todo. This doesn't work, should be printed here.
          drupal_add_js("var addthis_share = { templates: { twitter: '" . $this->getTokenizedField('sharemessage_short', $context) . "', } }", array('type' => 'inline'));
        }
        $namespace_attribute = !empty($namespace_attributes[$key]) ? $namespace_attributes[$key] : '';
        $services_HTML .= '<a class="addthis_button_' . $key . '" ' . $namespace_attribute . '></a>';
      }
    }
    else {
      // Default.
      $services_HTML .= '
        <a class="addthis_button_preferred_1"></a>
        <a class="addthis_button_preferred_2"></a>
        <a class="addthis_button_preferred_3"></a>
        <a class="addthis_button_preferred_4"></a>
        <a class="addthis_button_preferred_5"></a>';
    }

    return $services_HTML;
  }

  /**
   * Getter for the share URL.
   *
   * @param array $context
   *   The context for the token replacements.
   *
   * @return string
   *   The URL for this share message.
   */
  private function getUrl($context) {
    $options = array('absolute' => TRUE);
    if (!empty($this->settings['enforce_usage'])) {
      $options['query'] = array('smid' => $this->smid);
    }
    return url($this->getTokenizedField('sharemessage_url', $context, current_path()), $options);
  }

  /**
   * Function that adds additional services as part of addThis widget.
   */
  private function build_additional_services_part() {
    $additional_services = isset($this->settings['additional_services']) ? $this->settings['additional_services'] : variable_get('sharemessage_default_additional_services', TRUE);

    $additional = '';
    if ($additional_services && $this->getIconStyle() != 'addthis_counter_style') {
      $additional .= '<a class="addthis_button_compact"></a>';
    }
    $counter = isset($this->settings['counter']) ? $this->settings['counter'] : variable_get('sharemessage_default_counter', FALSE);
    if ($counter) {
      $additional .= '<a class="addthis_counter ' . $counter . '"></a>';
    }
    elseif ($this->getIconStyle() == 'addthis_counter_style' && !empty($this->settings['additional_services'])) {
      $additional .= '<a class="addthis_counter"></a>';
    }
    return $additional;
  }

  /**
   * Getter for the style category class of addthis widgets.
   *
   * @param string $icon_style
   *   The icon style to find the style class for.
   *
   * @return string
   *   The addthis style category CSS class.
   */
  protected function getAddthisStyleClass($icon_style) {
    // @todo there are more styles to cover in this map.
    $styles_map = array(
      'addthis_default_style' => array(
         'addthis_16x16_style',
         'addthis_32x32_style',
      ),
      'addthis_floating_style' => array(
        'addthis_counter_style',
      ),
    );

    // Find the match for the given icon style.
    foreach ($styles_map as $addthis_style_class => $styles) {
      if (in_array($icon_style, $styles)) {
        return $addthis_style_class;
      }
    }

    // Fallback to the default if no class in the map was matched.
    return 'addthis_default_style';
  }

  /**
   * Getter for additional button attributes.
   *
   * @param string $icon_style
   *   The used icon style of the sharemessage.
   *
   * @return array
   *   Array of namespace attributes keyed by service name.
   */
  protected function getNamespaceAttributes($icon_style) {
    // Floating counter style attributes.
    if ($icon_style == 'addthis_counter_style') {
      return array(
        'facebook_like' => 'fb:like:layout="box_count"',
        'tweet' => 'tw:count="vertical"',
        'google_plusone' => 'g:plusone:size="tall"',
      );
    }

    // Default attributes.
    return array(
      'facebook_like' => 'fb:like:layout="button_count"',
      'pinterest_pinit' => 'pi:pinit:layout="horizontal"',
    );
  }

  protected function getIconStyle() {
    return !empty($this->settings['icon_style']) ? $this->settings['icon_style'] : variable_get('sharemessage_default_icon_style', 'addthis_16x16_style');
  }

}
