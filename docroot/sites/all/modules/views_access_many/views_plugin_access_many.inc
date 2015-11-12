<?php

/**
 * The base plugin to handle access control.
 *
 * @ingroup views_access_plugins
 */
class views_plugin_access_many extends views_plugin_access {

  /**
   * The access plugins the user has chosen
   */
  private $access_plugins = array ();

  // Flag
  private $plugins_loaded = FALSE;

  /**
   * Loads & initializes all plugins
   * This needs to be called at the start of all our methods
   * that rely on our child access plugins
   */
  function load_plugins() {

    if ($this->plugins_loaded) {
      return;
    }

    $this->access_plugins = array ();

    if (empty($this->options['access_plugins'])) {
      return;
    }

    foreach ($this->options['access_plugins'] as $name) {

      // Load em
      $this->access_plugins[$name] = views_get_plugin('access', $name);
      if (!empty($this->access_plugins[$name])) {

        // Init em
        $this->access_plugins[$name]->init($this->view, $this->display);
        $this->access_plugins[$name]->set_default_options();

        // Set their options to what we have for them
        $this->options['access_plugins_options'][$name] = !empty($this->options['access_plugins_options'][$name]) ? $this->options['access_plugins_options'][$name] : array ();
        $this->access_plugins[$name]->unpack_options($this->access_plugins[$name]->options, $this->options['access_plugins_options'][$name]);
      }
      else {
        unset($this->access_plugins[$name]);
      }
    }

    $this->plugins_loaded = TRUE;
  }

  /**
   * Save plugins
   * This needs to be called at the end of all our methods
   * that rely on our child plugins
   */
  function save_plugins() {
    if (empty($this->access_plugins)) {
      return;
    }
    // Just need to save the plugin's options to ours
    // so they are stored by views
    $this->options['access_plugins_options'] = array ();
    foreach ($this->access_plugins as $plugin_name => $p) {
      $this->options['access_plugins_options'][$plugin_name] = $p->options;
    }
  }

  /**
   * Retrieve the default options when this is a new access
   * control plugin
   */
  function option_definition() {
    $options = array ();
    $options['access_plugins'] = array ('default' => array ());
    $options['access_plugins_options'] = array ('default' => array ());
    $options['blacklist'] = array ('default' => 'AND');
    return $options;
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {

    $plugin_names = views_fetch_plugin_names('access');
    unset($plugin_names['none']);

    $this->load_plugins();

    $form['access_plugins'] =  array (
      '#type' => 'checkboxes',
      '#options' => $plugin_names,
      '#default_value' => !empty($this->options['access_plugins']) ? array_keys($this->options['access_plugins']) : array (),
      '#description' => t('If you update the values of these checkboxes, submit and return to this form to update specific settings.'),
    );
    $form['blacklist'] =  array (
      '#type' => 'radios',
      '#options' => array ('AND' => t('Blacklist (AND)'), 'OR' => t('Whitelist (OR)')),
      '#default_value' => !empty($this->options['blacklist']) ? $this->options['blacklist'] : 'AND',
      '#description' => t('How to handle the return values of many access plugins. <em>Blacklist</em> - The view is accessible unless at least one access plugin denies access; <em>Whitelist</em> - The view is unaccessible unless at least one access plugin grants access.'),
    );

    $form['access_plugins_options'] = array (
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#title' => t('Access plugins options !summary_title', array ('!summary_title' => $this->summary_title())),
    );

    if (empty($this->access_plugins)) {
      $form['access_plugins_options']['refresh'] = array (
        '#value' => t('Choose some access plugins and submit & return to this form to set their options.'),
      );
    }

    else {
      $count = 0;
      foreach ($this->access_plugins as $plugin_name => $p) {
        if ($count++ > 0) {
          $form['access_plugins_options']["logical_operator_$count"] = array (
            '#value' => '<em>' . $this->options['blacklist'] . '</em>',
          );
        }
        $form['access_plugins_options'][$plugin_name] = array (
          '#type' => 'fieldset',
          '#title' => $plugin_names[$plugin_name],
          '#tree' => TRUE,
        );
        $p->options_form($form['access_plugins_options'][$plugin_name], $form_state);
      }
    }

    $this->save_plugins();
  }

  /**
   * Provide the default form form for validating options
   */
  function options_validate(&$form, &$form_state) {

    /* Note, some issues with the fact that it is
      the case that the form that is passed to access plugin's validation
      functions is just the section relevant to the plugin
      while the form_state is that of the whole form (which is needed)
      for form_set_error etc.
      This means that all access plugins actually look in
      $form_state['values']['access_options'] for their values.
      This itself means that we need to do some massaging of the form_state
      array so that when we ask a plugin to validate itself, we have
      altered the form_state so exactly only it's elements' values are in
      the form_state['values']['access_options'] bit.

      ALSO, when our display calls our options_submit it doesn't call our
      own init() function!! I don't know why, but this means we don't have any
      access to our own options in options_submit like we do in options_validate.
      This means we validate AND submit our child access_plugins in our options_validate
      and deal with our own options only in our options_submit.
      This shouldn't have an untoward affects..
    */

    $this->load_plugins();

    // FIRST validate ourselves

    // Keep a clean copy of this
    $access_options_values = $form_state['values']['access_options'];

    foreach ($this->access_plugins as $plugin_name => $p) {

      // Do not bother validating or submitting the form for a plugin
      // if the user just unticked it in this form submission
      if (empty($access_options_values['access_plugins'][$plugin_name])) {
        continue;
      }

      $form_state['values']['access_options'] = $access_options_values['access_plugins_options'][$plugin_name];
      $p->options_validate($form['access_plugins_options'][$plugin_name], $form_state);
      $p->options_submit($form['access_plugins_options'][$plugin_name], $form_state);
      // Copy that back since the access plugin may have changed something
      $access_options_values['access_plugins_options'][$plugin_name] = $form_state['values']['access_options'];
    }

    // Just like we never messed with it =p
    $form_state['values']['access_options'] = $access_options_values;

    // Note that our display handler itself will look through our own
    // form values and update our options accordingly after our options_submit
    // So it is meaningless to call $this->save_plugins()
  }

  /**
   * Provide the default form form for submitting options
   */
  function options_submit(&$form, &$form_state) {
    // Just deal with ourselves

    // I hate checkboxes.
    $form_state['values']['access_options']['access_plugins'] = array_filter($form_state['values']['access_options']['access_plugins']);
  }

  /**
   * Recurses through our child plugins to get meaningful description
   */
  function summary_title() {

    if (empty($this->options['blacklist'])) {
      $this->options['blacklist'] = 'AND';
    }

    $this->load_plugins();

    $plugin_names = views_fetch_plugin_names('access');

    $output = '(';
    if (empty($this->access_plugins)) {
      $output .= ($this->options['blacklist'] == 'AND') ? 'TRUE' : 'FALSE';
    }
    else {
      $count = 0;
      foreach ($this->access_plugins as $plugin_name => $p) {
        if ($count++ > 0) {
          $output .= ' ' . $this->options['blacklist'] . ' ';
        }

        if ($plugin_name == 'many') {
          $output .= $p->summary_title();
        }
        else {
          $output .= $p->summary_title();
        }
      }
    }
    $output .= ')';

    return $output;
  }

  /**
   * Determine if the current user has access or not.
   * Just grabs the callback and executes that. Is there a
   * reason this would not be equivalent to calling access on each plugin?
   * It would have to replicate the logic in the callback anyway.
   * I suppose it might be less efficient. Oh but it's easier.
   */
  function access($account) {
    $callback = $this->get_access_callback();
    if (is_array($callback) && function_exists($callback[0])) {
      return call_user_func_array($callback[0], array_merge($callback[1], array ('account' => $account)));
    }
    else {
      return TRUE;
    }
  }

  /**
   * Determine the access callback and arguments.
   *
   * This information will be embedded in the menu in order to reduce
   * performance hits during menu item access testing, which happens
   * a lot.
   *
   * @return an array; the first item should be the function to call,
   *   and the second item should be an array of arguments. The first
   *   item may also be TRUE (bool only) which will indicate no
   *   access control.)
   */
  function get_access_callback() {

    $this->load_plugins();

    $other_callbacks = array ();
    foreach ($this->access_plugins as $plugin_name => $p) {
      $other_callbacks[$plugin_name] = $p->get_access_callback();
    }

    $settings = array (
      'blacklist' => $this->options['blacklist'],
    );

    $this->save_plugins();

    return array (
      'views_plugin_access_many_access_callback',
      array ($other_callbacks, $settings),
    );
  }
}