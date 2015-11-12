<?php

/**
 * Implements hook_contextual_forms_info().
 *
 * Keys you may implement are:
 * - title: The human-readable name
 * - description: A short description of what the form does.
 * - function: (optional) Specify the function name that is the form builder,
 *   defaults to the machine name.
 * - category: (optional) Place the form in a specified content category in
 *   Page Manager.
 * - includes: (optional) Specify the file to include for the form builder, as
 *   is expected by $form_state['build_info']['files'].
 *   @see form_load_include(). Keys required are:
 *     - module: (optional) The module owning the include file. Auto-derived.
 *     - name: The name of file to load, without extension.
 *     - type: (optional) The type of include file, defaults to 'inc'
 * - contexts: (optional) An array of Ctools contexts to use as the form's
 *   arguments. Can either be Ctools context objects, or list of arrays
 *   containing the following keys:
 *     - title: The title of the context, use in UI selectors when multiple
 *       contexts qualify.
 *     - keyword: The name of the desired context. Usually this would be the
 *       name of an entity, like 'node', or 'user' but can also be a myriad of
 *       things. Use ctools_get_contexts() to see the list of possible contexts.
 *     - restrictions: (optional) an array of restrictions to narrow down the
 *       type of context that can be accepted. For instance, if the context is
 *       'node' then a restriction we may add is 'type' to equal 'story' if we
 *       only allow story node types.
 *     - required: (optional) Boolean value to determine if this context is
 *       absolutely required or not. Specifically, this influences whether
 *       `ctools_context_required` or `ctools_context_optional` is called.
 * - arguments: Deprecated, use 'contexts' instead.
 */
function hook_contextual_forms_info() {
  $forms['mymod_subscribe'] = array(
    'title' => 'MyMod Subscription Form',
    'description' => 'A form for requesting information from subscribers.',
    'category' => t('My Module'),
    'include' => array(
      // The form resides in mymod.admin.inc
      'module' => 'mymod',
      'name' => 'mymod.admin',
      'type' => 'inc',
    ),
    'function' => 'mymod_subscribe',

    'contexts' => array(
      new ctools_context_required(t('Newsletter'), 'node'),
      new ctools_context_optional(t('Subscriber'), 'user'),
      // Subscriber can optionally be expressed like this:
      // array(
      //   'title' => t('Subscriber'),
      //   'keyword' => 'user',
      //   'required' => FALSE,
      // ),
    ),
  );

  return $forms;
}
