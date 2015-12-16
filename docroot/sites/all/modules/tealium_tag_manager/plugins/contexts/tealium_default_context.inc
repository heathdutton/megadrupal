<?php
/**
 * @file
 * Sample ctools context type plugin that shows how to create
 * a context from an arg.
 */

define('TEALIUM_CONTEXT_PLUGIN_NAME', 'tealium_default_context');
define('TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME', 'substitution_token_class');

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Tealium Default Context'),
  'description' => t('Providing context for token replacements.'),
  'context' => 'ctools_context_create_tealium_default_context',
  'keyword' => TEALIUM_CONTEXT_PLUGIN_NAME,
  'context name' => TEALIUM_CONTEXT_PLUGIN_NAME,
  'convert list' => 'ctools_context_tealium_default_convert_list',
  'convert' => 'ctools_context_tealium_default_convert',
  'edit form' => 'tealium_default_context_settings_form',
);

/**
 * Implements hook_block_form().
 */
function tealium_default_context_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];

  if (empty($conf)) {
    $conf = array(
      TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME => 'global',
    );
  }

  $form[TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME] = array(
    '#type' => 'textfield',
    '#title' => t('Substitution token class'),
    '#size' => 50,
    '#description' => t('The name of the substitution token class to be used to pull out the exposed tokens in the journey (first param of token_replace).'),
    '#default_value' => isset($conf[TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME]) ? $conf[TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME] : '',
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function tealium_default_context_settings_form_submit($form, &$form_state) {

  // This will either be the value set previously or a value set by the
  // validator.
  $form_state['conf'][TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME] = $form_state['values'][TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME];
}

/**
 * Create a context from manual configuration or from an argument on the URL.
 *
 * @param bool $empty
 *   Context empty of not.
 * @param array $data
 *   Context Data.
 * @param bool $conf
 *   Configuration.
 *
 * @return object
 *   Ctools context object.
 */
function ctools_context_create_tealium_default_context($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context(TEALIUM_CONTEXT_PLUGIN_NAME);
  $context->plugin = TEALIUM_CONTEXT_PLUGIN_NAME;

  if (isset($data[TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME])) {
    $context->data = new stdClass();
    $context->data->{TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME} = $data[TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME];
  }

  return $context;
}

/**
 * Provide a list of sub-keywords.
 *
 * This is used to provide keywords from the context for use in a content type,
 * pane, etc.
 */
function ctools_context_tealium_default_convert_list() {
  $list = array();
  $token_substitution_class = NULL;

  // Include tokens provided by rml_feetopay module.
  if (module_exists('token')) {
    if ($token_substitution_class = tealium_default_context_get_setting(TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME)) {
      // We fetch the list of tokens.
      $tokens = tealium_token_info();
      $tealium_tokens = $tokens['tokens'];
      foreach ($tealium_tokens['tealium_tokens'] as $index => $token) {
        $list[$index] = $index;
      }
    }
  }

  return $list;
}

/**
 * Convert a context into a string to be used as a keyword by content types,etc.
 *
 * Executed when page is rendered.
 */
function ctools_context_tealium_default_convert($context, $type) {
  $text_to_return = NULL;

  if (module_exists('token') && isset($context->data->{TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME}) &&
    !empty($context->data->{TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME})) {
    // List of tokens and return them in as array.
    $list_of_tokens = tealium_token_info();
    $key = $list_of_tokens['tokens'][$context->data->{TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME}];

    if (!empty($key)) {
      // We build the tokenized text, ie: [tealium_tokens:tealium_appstep].
      $txt_to_replace = '[' . $context->data->{TEALIUM_SUBSTITUTION_TOKEN_CLASS_FIELD_NAME}
      . ':' . $type . ']';

      // And we search for candidates.
      $text_to_return = token_replace($txt_to_replace);

      return $text_to_return;
    }

  }
}

/**
 * Return the default substitution token class set in the context settings form.
 *
 * Use this function only on admin context to pull out the tokens from a given
 * substitution class where the handler is present in the URL (admin context).
 *
 * @param string $context_form_setting_key
 *   Context form settings element name.
 *
 * @return bool|string
 *   Value of the form element or FALSE if not found.
 */
function tealium_default_context_get_setting($context_form_setting_key) {
  $path_fragments = explode('/', $_GET['q']);
  $token_list = FALSE;

  // Try and find the page context handler name from the path.
  $page_cache = page_manager_cache_load('page');

  if (!empty($path_fragments)) {
    // @todo This is bit dirty, but can't see a better way.
    // @see route_get_current_page_subtask_name()
    if (!empty($page_cache) && !empty($page_cache->handlers)) {
      foreach ($page_cache->handlers as $handler) {
        foreach ($path_fragments as $path_fragment) {
          // Does this make sense instead of using directly handler->name?
          if ($path_fragment == $handler->name && !empty($handler->conf['contexts'])) {
            foreach ($handler->conf['contexts'] as $context) {
              if (!empty($context[$context_form_setting_key])) {
                $tokens[$context['keyword']] = $context[$context_form_setting_key];

              }
            }
          }
        }
      }
      $token_list = $tokens;
    }

    // We haven't found the handler yet, so we try to find it in the
    // page manager default handlers.
    // @todo page_manager_page_load_all() return all the pages with the current
    // page on the top of the array. Investigate this further. If this is the
    // case we can use this to implement  a nice and easy fallback mechanism if
    // the plugin handler is not present for some reason.
    $pages = page_manager_page_load_all();
    if (!empty($pages)) {
      foreach ($pages as $page) {
        if (!empty($page->default_handlers)) {
          foreach ($page->default_handlers as $handler) {
            foreach ($path_fragments as $path_fragment) {
              if ($path_fragment == $handler->name && !empty($handler->conf['contexts'])) {
                foreach ($handler->conf['contexts'] as $context) {
                  if (!empty($context['context_settings'][$context_form_setting_key])) {
                    $tokens[$context['keyword']] = $context[$context_form_setting_key];
                    return $context['context_settings'][$context_form_setting_key];
                  }
                }
              }
            }
          }
        }
        $token_list = $tokens;
      }
    }

  }

  // All attempts to find the handler and the default context settings failed.
  return $token_list;
}
