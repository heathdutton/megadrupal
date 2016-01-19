<?php

/**
 * @file
 * Functions implementing hooks for context reactions.
 */

module_load_include('admin.inc', 'site_banner', 'site_banner');

/**
 * Handle enabling/disabling the site banner and debug mode.
 */
class SiteBannerContextReactionChangeBannerStatus extends context_reaction {
  /**
   * Admin panel for setting banner background color for selected contexts.
   */
  public function options_form($context) {
    $existing_status = site_banner_get_site_banner_status($context);
    $existing_debug_status = site_banner_get_site_banner_debug_status($context);

    return site_banner_generate_banner_status_form_elements($existing_status, $existing_debug_status);
  }

  /**
   * Validate hook for context-specific site banner background color settings.
   */
  public function options_form_validate($form_state) {
    // Not linked into context module.
    // TODO: raise bug to allow validation of reaction fields.
  }

  /**
   * Submit handler for context-specific site banner background color settings.
   */
  public function options_form_submit($values) {
    return $values;
  }

  /**
   * Output banner background colour for active contexts.
   */
  public function execute(&$site_banner_status, &$site_banner_debug_status) {
    $contexts = context_active_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions['change_banner_status'])) {
        $site_banner_status |= site_banner_get_site_banner_status($context);
        $site_banner_debug_status |= site_banner_get_site_banner_debug_status($context);
      }
    }
  }
}

/**
 * Handle changing the site banner text reaction.
 */
class SiteBannerContextReactionChangeBannerText extends context_reaction {
  /**
   * Administration panel for setting banner text for selected contexts.
   */
  public function options_form($context) {
    $default_prepend_text = site_banner_get_prepend_site_banner_text($context);
    $default_banner_text = site_banner_get_site_banner_text($context);
    $default_delimiter_text = site_banner_get_delimiter($context);
    $default_append_text = site_banner_get_append_text($context);

    // Get all enabled contexts and provide help on existing context values.
    $prepend_text_description = 'For all contexts tagged with this context\'s
      tag, this is text that will be added before all active context\'s
      banner text.';
    $delimiter_text_description = 'For all contexts tagged with this context\'s
      tag, this is text that will be added in between successive banner texts.
      This is typically a single character like a backslash \'\\\' or comma
      \',\'.';
    $append_text_description = 'For all contexts tagged with this context\'s
      tag, this is text that will be added after all active context\'s
      banner text.';

    $prepend_examples = '';
    $delimiter_examples = '';
    $append_examples = '';
    foreach (context_enabled_contexts() as $context_iterator) {
      if (isset($context_iterator->tag) && !empty($context_iterator->tag)) {
        $prefix = "{$context_iterator->name} (tag = {$context_iterator->tag},";
        $prepend_examples = " $prefix prepend text = " . filter_xss_admin(site_banner_get_prepend_site_banner_text($context_iterator)) . ')';
        $delimiter_examples = " $prefix delimiter text = " . filter_xss_admin(site_banner_get_delimiter($context_iterator)) . ')';
        $append_examples = " $prefix append text = " . filter_xss_admin(site_banner_get_append_text($context_iterator)) . ')';
      }
    }

    if (!empty($prepend_examples)) {
      $prepend_text_description .= " Existing values are: $prepend_examples.";
      $append_text_description .= " Existing values are: $append_examples.";
      $delimiter_text_description .= " Existing values are: $delimiter_examples.";
    }

    $form = site_banner_generate_banner_text_form_elements($default_banner_text);

    $form['site_banner_tag_prepend_text'] = array(
      '#title' => t('Prepended banner text'),
      '#description' => $prepend_text_description,
      '#type' => 'textfield',
      '#size' => 50,
      '#maxlength' => 100,
      '#default_value' => $default_prepend_text,
    );

    $form['site_banner_text']['#description']
      = t('The text to be added to the site banner.');

    $form['site_banner_tag_delimiter_text'] = array(
      '#title' => t('Banner text delimiter'),
      '#description' => $delimiter_text_description,
      '#type' => 'textfield',
      '#size' => 50,
      '#maxlength' => 100,
      '#default_value' => $default_delimiter_text,
    );

    $form['site_banner_tag_append_text'] = array(
      '#title' => t('Appended banner text'),
      '#description' => $append_text_description,
      '#type' => 'textfield',
      '#size' => 50,
      '#maxlength' => 100,
      '#default_value' => $default_append_text,
    );

    return $form;
  }

  /**
   * Submit handler for context-specific site banner settings.
   */
  public function options_form_submit($values) {
    return $values;
  }

  /**
   * Output banner text for active contexts.
   */
  public function execute(&$banner_text) {
    $contexts = context_active_contexts();
    site_banner_generate_context_banner_text_from_contexts($contexts, $banner_text);
  }
}


/**
 * Handle changing the site banner background colors reaction.
 */
class SiteBannerContextReactionChangeBannerBackgroundColor extends context_reaction {
  /**
   * Admin panel for setting banner background color for selected contexts.
   */
  public function options_form($context) {
    $existing_color_code = site_banner_get_existing_background_color($context);
    return site_banner_generate_bg_color_form_elements($existing_color_code);
  }

  /**
   * Validate hook for context-specific site banner background color settings.
   */
  public function options_form_validate($form_state) {
    // Not linked into context module.
    // TODO: raise bug to allow validation of reaction fields.
    site_banner_validate_background_color_code_form($form_state);
  }

  /**
   * Submit handler for context-specific site banner background color settings.
   */
  public function options_form_submit($values) {
    $is_custom_selected = array_key_exists('site_banner_background_color_select', $values) ? $values['site_banner_background_color_select'] : 1;

    $color_code = site_banner_get_background_color_code(
      $values['site_banner_background_color_form'],
      $values['site_banner_background_custom_color_form'],
      $is_custom_selected
    );
    return $color_code;
  }

  /**
   * Output banner background colour for active contexts.
   */
  public function execute(&$background_color_as_text) {
    $contexts = context_active_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions['change_banner_background_color'])) {
        $background_color_as_text = site_banner_get_existing_background_color($context);
      }
    }
  }
}


/**
 * Handle changing the site banner text color reaction.
 */
class SiteBannerContextReactionChangeBannerTextColor extends context_reaction {
  /**
   * Administration panel for setting banner text color for selected contexts.
   */
  public function options_form($context) {
    $existing_color_code = site_banner_get_existing_text_color($context);
    return (array) site_banner_generate_text_color_form_elements($existing_color_code);
  }

  /**
   * Validate hook for context-specific site banner text color settings.
   */
  public function options_form_validate($form_state) {
    // Not linked into context module.
    // TODO: raise bug to allow validation of reaction fields.
    site_banner_validate_text_color_code_form($form_state);
  }

  /**
   * Submit handler for context-specific site banner text color settings.
   */
  public function options_form_submit($values) {
    $is_custom_selected = array_key_exists('site_banner_text_color_select', $values) ? $values['site_banner_text_color_select'] : 1;

    $color_code = site_banner_get_text_color_code(
      $values['site_banner_text_color_form'],
      $values['site_banner_text_custom_color_form'],
      $is_custom_selected
    );
    return $color_code;
  }

  /**
   * Output banner text colour for active contexts.
   */
  public function execute(&$text_color_as_text) {
    $contexts = context_active_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions['change_banner_text_color'])) {
        $text_color_as_text = site_banner_get_existing_text_color($context);
      }
    }
  }
}

/**
 * Returns the existing text color if defined.
 *
 * @param array $context
 *   the context array with settings storing text color if previously saved.
 *
 * @return string
 *   the HTML hexadecimal color code
 */
function site_banner_get_existing_text_color($context) {
  if (!empty($context->reactions['change_banner_text_color'])) {
    return $context->reactions['change_banner_text_color'];
  }
  else {
    // Get first element in text color array.
    $colors = array_keys(site_banner_get_text_colors());
    return $colors[0];
  }
}

/**
 * Returns the existing background color.
 *
 * @param array $context
 *   context array with the predefined background color.
 *
 * @return string
 *   the HTML hexadecimal color code
 */
function site_banner_get_existing_background_color($context) {
  if (!empty($context->reactions['change_banner_background_color'])) {
    return $context->reactions['change_banner_background_color'];
  }
  else {
    // Get first element in background color array.
    $colors = array_keys(site_banner_get_background_colors());
    return $colors[0];
  }
}

/**
 * Returns the prepended site banner text from a context.
 *
 * @param array $context
 *   the context array.
 *
 * @return array
 *   the prepended banner text
 */
function site_banner_get_prepend_site_banner_text($context) {
  return site_banner_get_text_from_context($context, 'site_banner_tag_prepend_text');
}

/**
 * Returns the site banner status from a context.
 *
 * @param array $context
 *   the context array.
 *
 * @return bool
 *   whether the banner is enabled for a context or not.
 */
function site_banner_get_site_banner_status($context) {
  if (!empty($context->reactions['change_banner_status']['site_banner_status'])) {
    return $context->reactions['change_banner_status']['site_banner_status'];
  }
  else {
    // If undefined then set as false.
    return 0;
  }
}

/**
 * Returns the site banner debug mode status from a context.
 *
 * @param array $context
 *   the context array.
 *
 * @return bool
 *   whether debug status is active or not.
 */
function site_banner_get_site_banner_debug_status($context) {
  if (!empty($context->reactions['change_banner_status']['site_banner_debug_status'])) {
    return $context->reactions['change_banner_status']['site_banner_debug_status'];
  }
  else {
    // If undefined then set as false.
    return 0;
  }
}

/**
 * Returns the site banner text from a context.
 *
 * @param array $context
 *   the context array.
 *
 * @return array
 *   the banner text
 */
function site_banner_get_site_banner_text($context) {
  return site_banner_get_text_from_context($context, 'site_banner_text');
}

/**
 * Returns the site banner delimiter text from a context.
 *
 * @param array $context
 *   the context array.
 *
 * @return string
 *   the banner text delimiter
 */
function site_banner_get_delimiter($context) {
  return site_banner_get_text_from_context(
    $context,
    'site_banner_tag_delimiter_text'
  );
}

/**
 * Returns the appended site banner text from a context.
 *
 * @param array $context
 *   the context array.
 *
 * @return string
 *   the appended banner text
 */
function site_banner_get_append_text($context) {
  return site_banner_get_text_from_context(
    $context,
    'site_banner_tag_append_text'
  );
}

/**
 * Returns the site banner text from a context.
 *
 * @param array $context
 *   the context array.
 * @param string $param
 *   the section of the banner text context array
 *
 * @return array
 *   the requested banner text
 */
function site_banner_get_text_from_context($context, $param) {
  $return_text = '';

  $context_active = isset($context->reactions);
  $reaction_exists = isset($context->reactions['change_banner_text']);
  $param_exists = isset($context->reactions['change_banner_text'][$param]);

  if ($context_active && $reaction_exists && $param_exists) {
    $return_text = $context->reactions['change_banner_text'][$param];
  }
  return $return_text;
}
