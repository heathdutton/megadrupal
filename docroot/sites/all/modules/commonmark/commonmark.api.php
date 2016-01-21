<?php
/**
 * @file
 * CommonMark module APIs.
 */

/**
 * Allows modules to provide extensions for CommonMark.
 *
 * @return array
 *   An associative array of extension information, keyed by a unique extension
 *   machine name and an associative array value containing the following
 *   key-value pairs:
 *   - class: (string|array) The fully qualified PHP class name of the extension
 *     or an array of class names to try until a valid class is found. The class
 *     must extend from Drupal\CommonMark\Extension or implement the interface
 *     Drupal\CommonMark\ExtensionInterface. It must also implement at least one
 *     of the following League\CommonMark extension interfaces:
 *     - League\CommonMark\Block\Parser\BlockParserInterface
 *     - League\CommonMark\Block\Renderer\BlockRendererInterface
 *     - League\CommonMark\Extension\ExtensionInterface
 *     - League\CommonMark\Inline\Parser\InlineParserInterface
 *     - League\CommonMark\Inline\Processor\InlineProcessorInterface
 *     - League\CommonMark\Inline\Renderer\InlineRendererInterface
 *   - class arguments: (array)(optional) An array of arguments to pass to the
 *     extension's class constructor, if any.
 *   - class arguments callback: (callable)(optional) A callback that implements
 *     callback_commonmark_extension_class_arguments().
 *   - description: (string)(optional) A human readable description.
 *   - enabled: (bool)(optional) State indicating whether or not the extension
 *     is currently enabled or not. If provided in the hook definition, it will
 *     set the "default" state of the extension.
 *   - method: (string) The appropriate \League\CommonMark\Environment method
 *     to invoke that will add the extension to the environment. Generally
 *     speaking, you will never have to manually specify this. Just ensure your
 *     class implements the proper interface.
 *   - method arguments: (array) The arguments to pass to the method invocation.
 *     You will only ever need to specify this property if the extension is a
 *     block or inline renderer which needs to specify the appropriate "class"
 *     before the newly instantiated extension. This defaults to:
 *     [\Drupal\CommonMark\Environment::EXTENSION_INSTANCE_PLACEHOLDER]
 *   - module: (string) The name of the module that defined the extension. This
 *     value is set automatically and cannot be overridden.
 *   - name: (string) The unique machine name of the extension. This value is
 *     set automatically and cannot be overridden.
 *   - settings: (array) An associative array of settings containing key-value
 *     pairs.
 *   - title: (string)(optional) A human readable title. If omitted, the machine
 *     name of the extension will be used.
 *   - type: (string) The type of CommonMark extension this extension is. This
 *     value is set automatically, you do not need to implement it. Its value
 *     will be one of the following:
 *     - BlockParser
 *     - BlockRenderer
 *     - Extension
 *     - InlineParser
 *     - InlineProcessor
 *     - InlineRenderer
 *   - url: (string)(optional) A URL to the extension source or documentation.
 *
 * @see commonmark_commonmark_extension_info()
 */
function hook_commonmark_extension_info() {
  // Example taken from commonmark_commonmark_extension_info().
  $extensions['enhanced_links'] = [
    'title' => t('Enhanced Links'),
    'description' => t('Extends CommonMark to provide additional enhancements when rendering links.'),
    'class' => '\Drupal\CommonMark\Inline\LinkRenderer',
    'method arguments' => ['Link', \Drupal\CommonMark\Environment::EXTENSION_INSTANCE_PLACEHOLDER],
    'settings' => [
      'external_new_window' => TRUE,
    ],
  ];
  return $extensions;
}

/**
 * Provides an extension with dynamic arguments before it is instantiated.
 *
 * Callback for hook_commonmark_extension_info().
 *
 * @param array $extension
 *   The extension information array (from a hook_commonmark_extension_info()
 *   definition).
 * @param \Drupal\CommonMark\Environment $environment
 *   The current environment instance.
 *
 * @ingroup callbacks
 *
 * @see \Drupal\CommonMark\Environment::__construct()
 */
function callback_commonmark_extension_class_arguments(array $extension, \Drupal\CommonMark\Environment $environment) {
  // Supply dynamic arguments here.
  return [];
}

/**
 * Allows extensions to provide settings on the CommonMark administrative form.
 *
 * Note, this hook is only called for the module that actually defined the
 * extension.
 *
 * @param array $extension
 *   The extension information array (from a hook_commonmark_extension_info()
 *   definition).
 * @param array $element
 *   The fieldset element that belongs to the extension.
 * @param array $form_state
 *   The form state array, passed by reference.
 * @param array $form
 *   The entire form array, passed by reference.
 *
 * @return array
 *   The modified element array.
 *
 * @see hook_commonmark_extension_info()
 * @see commonmark_commonmark_extension_settings()
 */
function hook_commonmark_extension_settings(array $extension, array $element, array &$form_state, array &$form) {
  // Example taken from commonmark_commonmark_extension_settings().
  switch ($extension['name']) {
    case 'enhanced_links':
      // The key for the element must match a setting name defined in the
      // extension information settings array.
      $element['external_new_window'] = [
        '#type' => 'checkbox',
        '#title' => t('Open external links in new windows'),
        '#description' => t('...'),
        // The extension settings array already contains the correct value
        // to use for the "default value" (whether it comes from global config,
        // filter config or a $form_state value). If you need to reference the
        // original value, it is stored in $extension['original settings'].
        '#default_value' => $extension['settings']['external_new_window'],
      ];
      break;
  }
  return $element;
}

/**
 * Allows extensions to provide settings on the CommonMark administrative form.
 *
 * Note, this hook is only called for the module that actually defined the
 * extension.
 *
 * @param array $extension
 *   The extension information array (from a hook_commonmark_extension_info()
 *   definition).
 * @param array $groups
 *   An associative array of groups, passed by reference. Keyed by a group
 *   identifier, containing the key-value pairs:
 *   - title: (string) A human readable title for the group (to be displayed
 *     in the vertical tab).
 *   - items: (array) An indexed array of "tip" items containing for the group
 *     with the key-value pairs:
 *     - title: (string)(optional) A human readable title for the item.
 *     - description: (string)(optional) Some helpful text providing additional
 *       details about the item.
 *     - tags: (array) An associative array keyed by tag name containing an
 *       an array of markdown examples to display.
 *     - strip_p: (bool) Toggle determining whether or not to strip wrapping
 *       <p> tags from the rendered HTML output. This generally should be left
 *       enabled unless dealing specifically with <p> tags. Defaults to TRUE.
 * @param array $build
 *   The entire build render array.
 * @param object $filter
 *   An object representing the filter.
 * @param object $format
 *   An object representing the text format the filter is contained in.
 * @param bool $long
 *   Whether this callback should return a short tip to display in a form
 *   (FALSE), or whether a more elaborate filter tips should be returned for
 *   theme_filter_tips() (TRUE).
 *
 * @see commonmark_filter_tips()
 * @see commonmark_commonmark_extension_tips()
 */
function hook_commonmark_extension_tips(array $extension, array &$groups, array &$build, $filter, $format, $long) {
  // Example taken from commonmark_commonmark_extension_tips().
  switch ($extension['name']) {
    case 'enhanced_links':
      if ($extension['settings']['external_new_window']) {
        // @codingStandardsIgnoreStart Ignore coding standards during this
        // section of code. Concatenated t() strings are ok here.
        $groups['links']['items'][0]['tags']['a'][] = '[' . t('External link opens in new window') . '](http://example.com)';
        // @codingStandardsIgnoreEnd
      }
      break;
  }
}
