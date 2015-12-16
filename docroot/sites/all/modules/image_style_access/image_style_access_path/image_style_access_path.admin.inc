<?php
/**
 * @file
 * Administrative UI for path-based access control.
 */

/**
 * Path-based access plugin admin form controller.
 */
class ImageStyleAccessPathConfigFormController extends ImageStyleAccessConfigFormController {
  /**
   * {@inheritdoc}
   */
  public function form($form, &$form_state, ImageStyleAccessConfig $config) {
    form_load_include($form_state, 'inc', 'image_style_access_path', 'image_style_access_path.admin');

    // Build base form.
    if (empty($form_state['configure_ctools_access'])) {
      $form = parent::form($form, $form_state, $config);
      /** @var $config ImageStyleAccessConfig */
      $config = $form_state['config'];

      // Tweak path description.
      $description = t('!original_description You may also use %name to create named placeholders for variable parts of the path (e.g. "subfolder/%node/image").', array(
        '!original_description' => $form['target_base']['#description'],
      ));
      $form['target_base']['#description'] = $description;

      // Add button to refresh settings.
      $form['settings']['#prefix'] = '<div id="edit-settings-wrapper">';
      $form['settings']['#suffix'] = '</div>';
      $ajax = array(
        'callback' => 'image_style_access_path_ajax_rebuild_settings',
        'wrapper' => 'edit-settings-wrapper',
      );
      $form['settings']['refresh'] = array(
        '#type' => 'submit',
        '#name' => 'refresh',
        '#value' => t('Refresh context settings forms'),
        '#weight' => -5,
        '#limit_validation_errors' => array(),
        '#attributes' => array(
          'class' => array('js-hide'),
        ),
        '#ajax' => $ajax,
      );
      $form['target_base']['#ajax'] = array(
        'trigger_as' => array('name' => 'refresh'),
      ) + $ajax;

      // Extract named path contexts.
      $targetBase = $config->target_base;
      $contexts = array();
      foreach (explode('/', $targetBase) as $id => $targetBasePart) {
        if (strlen($targetBasePart) > 1 && $targetBasePart[0] == '%') {
          $contexts[$id] = substr($targetBasePart, 1);
        }
      }

      // Add context configuration form.
      $config->settings += array('contexts' => array());
      if (empty($contexts)) {
        $form['settings']['contexts'] = array(
          '#type' => 'value',
          '#value' => array(),
        );
      }
      else {
        $form['settings']['contexts'] = array(
          '#type' => 'fieldset',
          '#title' => t('Path contexts'),
          '#description' => t('Configure contexts for named variable placeholders in the location base.'),
        );
        $contextOptions = $this->getContextOptions();
        foreach ($contexts as $id => $context) {
          $config->settings['contexts'] += array($context => array(
            'name' => '',
            'identifier' => $context,
          ));
          $form['settings']['contexts'][$context] = array(
            '#theme_wrappers' => array('container'),
            '#attributes' => array(
              'class' => array('image-style-access-path-context', 'clearfix'),
            ),
          );
          $form['settings']['contexts'][$context]['context'] = array(
            '#type' => 'item',
            '#title' => t('Context'),
            '#markup' => check_plain($context),
          );
          $form['settings']['contexts'][$context]['keyword'] = array(
            '#type' => 'value',
            '#value' => $context,
          );
          $form['settings']['contexts'][$context]['id'] = array(
            '#type' => 'value',
            '#value' => $id,
          );
          $form['settings']['contexts'][$context]['name'] = array(
            '#type' => 'select',
            '#title' => t('Type'),
            '#options' => $contextOptions,
            '#default_value' => $config->settings['contexts'][$context]['name'],
            '#required' => TRUE,
            '#ajax' => array(
              'trigger_as' => array('name' => 'refresh'),
            ) + $ajax,
          );
          $form['settings']['contexts'][$context]['identifier'] = array(
            '#type' => 'textfield',
            '#title' => t('Administrative identifier'),
            '#default_value' => $config->settings['contexts'][$context]['identifier'],
            '#size' => 30,
            '#ajax' => array(
              'trigger_as' => array('name' => 'refresh'),
            ) + $ajax,
          );
        }
      }

      // Add CTools access configuration form.
      ctools_include('context');
      $config->settings += array('access' => array());
      $form_state['contexts'] = $this->getArgumentPlaceholders($config->settings['contexts']);
      $form['settings']['access'] = array(
        '#type' => 'fieldset',
        '#title' => t('Context condition'),
        '#description' => t('Configure the condition for available contexts to match this access configuration.'),
      );
      if (!empty($config->settings['access']['name'])) {
        $plugin = isset($form_state['plugin']) ? $form_state['plugin'] : ctools_get_access_plugin($config->settings['access']['name']);
        if (isset($plugin)) {
          if (empty($config->settings['access'])) {
            $config->settings['access'] = ctools_access_new_test($plugin);
          }
          $form['settings']['access']['summary'] = array(
            '#type' => 'item',
            '#title' => t('Condition summary'),
            '#markup' => ctools_access_summary($plugin, $form_state['contexts'], $config->settings['access']),
          );
        }
      }
      $form['settings']['access']['configure'] = array(
        '#type' => 'submit',
        '#name' => 'configure_access',
        '#value' => t('Configure condition'),
      );
    }
    // Build access plugin form.
    else {
      $form = $this->accessPluginForm($form, $form_state);
    }

    $path = drupal_get_path('module', 'image_style_access_path');
    $form['#attached']['css'][] = $path . '/image_style_access_path.css';
    return $form;
  }

  /**
   * Builds access plugin form.
   *
   * @see ctools_access_ajax_edit_item()
   */
  protected function accessPluginForm($form, &$form_state) {
    ctools_include('context');

    // Prepare form state.
    $config = $form_state['config'];
    $access = $config->settings['access'];
    if (!isset($form_state['plugin']) && !empty($access['name']) && $plugin = ctools_get_access_plugin($access['name'])) {
      $form_state['plugin'] = $plugin;
    }
    if (!isset($form_state['test']) && isset($form_state['plugin'])) {
      if (!empty($access)) {
        $form_state['test'] = $access;
      }
      else {
        $form_state['test'] = ctools_access_new_test($form_state['plugin']);
      }
    }
    if (!isset($form_state['contexts'])) {
      $form_state['contexts'] = $this->getArgumentPlaceholders($config->settings['contexts']);
    }

    // Add access plugin selector.
    $form['#prefix'] = '<div id="edit-access-wrapper">';
    $form['#suffix'] = '</div>';
    $ajax = array(
      'callback' => 'image_style_access_path_ajax_rebuild',
      'wrapper' => 'edit-access-wrapper',
    );
    $form['name'] = array(
      '#type' => 'select',
      '#title' => t('Condition'),
      '#options' => $this->getAccessOptions($form_state['contexts']),
      '#default_value' => isset($form_state['plugin']) ? $form_state['plugin']['name'] : NULL,
      '#required' => TRUE,
      '#description' => t('Choose the context condition to configure.'),
      '#weight' => -10,
      '#ajax' => array(
        'trigger_as' => array('name' => 'refresh'),
      ) + $ajax,
    );
    $form['refresh'] = array(
      '#type' => 'submit',
      '#name' => 'refresh',
      '#value' => t('Refresh form'),
      '#weight' => -5,
      '#limit_validation_errors' => array(),
      '#attributes' => array(
        'class' => array('js-hide'),
      ),
      '#ajax' => $ajax,
    );

    // Build form.
    if (isset($form_state['plugin']) && isset($form_state['test'])) {
      $plugin = $form_state['plugin'];
      $test = $form_state['test'];
      if (isset($plugin['required context'])) {
        $form['context'] = ctools_context_selector($form_state['contexts'], $plugin['required context'], $test['context']);
      }
      $form['settings'] = array('#tree' => TRUE);
      if ($function = ctools_plugin_get_function($plugin, 'settings form')) {
        $form = $function($form, $form_state, $test['settings']);
      }
      $form['not'] = array(
        '#type' => 'checkbox',
        '#title' => t('Reverse (NOT)'),
        '#default_value' => !empty($test['not']),
      );

      $form['save_access'] = array(
        '#type' => 'submit',
        '#value' => t('Save context condition'),
      );
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  protected function buildConfig($form, &$form_state) {
    // Stash original access settings.
    $form_state['config']->settings += array('access' => array());
    $access = $form_state['config']->settings['access'];

    // Build config.
    $config = parent::buildConfig($form, $form_state);

    // Restore access if not configuring access plugin.
    if (empty($form_state['configure_ctools_access'])) {
      $config->settings['access'] = $access;
    }

    // Remove refresh button value.
    if (isset($config->settings['refresh'])) {
      unset($config->settings['refresh']);
    }

    return $config;
  }

  /**
   * {@inheritdoc}
   *
   * This implementation additionally switches the form between the regular edit
   * form and the access plugin form and validates access form submission.
   */
  public function validate($form, &$form_state) {
    // Validate access form submission.
    if (!empty($form_state['configure_ctools_access'])) {
      // Rebuild form if refreshing.
      if ($form_state['triggering_element']['#name'] == 'refresh') {
        $form_state['rebuild'] = TRUE;
      }

      // Reset empty plugin.
      if (empty($form_state['values']['name'])) {
        $form_state['plugin'] = NULL;
        $form_state['test'] = NULL;
      }
      // Update form state for changed plugin.
      elseif (!isset($form_state['plugin']) || $form_state['plugin']['name'] != $form_state['values']['name']) {
        ctools_include('context');
        $form_state['plugin'] = ctools_get_access_plugin($form_state['values']['name']);
        $form_state['test'] = ctools_access_new_test($form_state['plugin']);
        $form_state['rebuild'] = TRUE;
      }

      // Invoke CTools settings form validation callback.
      if (empty($form_state['rebuild']) && isset($form_state['plugin'])) {
        ctools_include('plugins');
        if ($function = ctools_plugin_get_function($form_state['plugin'], 'settings form validate')) {
          $function($form, $form_state);
        }
      }
    }
    // Rebuild form if elected to configure access plugin.
    elseif ($form_state['triggering_element']['#name'] == 'configure_access') {
      $form_state['config'] = $this->buildConfig($form, $form_state);
      $form_state['configure_ctools_access'] = TRUE;
      $form_state['rebuild'] = TRUE;
    }
    // Rebuild form if path has changed or refreshing.
    elseif ($form_state['values']['target_base'] != $form_state['config']->target_base || $form_state['triggering_element']['#name'] == 'refresh') {
      $form_state['config'] = $this->buildConfig($form, $form_state);
      $form_state['rebuild'] = TRUE;
    }
    // Validate as usual.
    else {
      parent::validate($form, $form_state);
    }
  }

  /**
   * {@inheritdoc}
   *
   * This implementation additionally handles access form submission.
   */
  public function submit($form, &$form_state) {
    // Handle access form submission.
    if (!empty($form_state['configure_ctools_access'])) {
      // Invoke CTools settings form submit callback.
      ctools_include('plugins');
      if ($function = ctools_plugin_get_function($form_state['plugin'], 'settings form submit')) {
        $function($form, $form_state);
      }

      // Store access plugin settings.
      $form_state['test']['settings'] = $form_state['values']['settings'];
      if (isset($form_state['values']['context'])) {
        $form_state['test']['context'] = $form_state['values']['context'];
      }
      $form_state['test']['not'] = !empty($form_state['values']['not']);
      $form_state['config']->settings['access'] = $form_state['test'];

      // Rebuild form.
      $form_state['configure_ctools_access'] = FALSE;
      $form_state['rebuild'] = TRUE;
    }
    // Handle as usual.
    else {
      parent::submit($form, $form_state);
    }
  }

  /**
   * Prepares a list of CTools context options.
   */
  protected function getContextOptions() {
    ctools_include('context');
    $plugins = ctools_get_arguments();

    $options = array();
    foreach ($plugins as $id => $plugin) {
      if (empty($plugin['no ui'])) {
        $options[$id] = $plugin['title'];
      }
    }

    asort($options);

    $options = array('' => t('No context selected')) + $options;
    return $options;
  }

  /**
   * Prepares empty placeholders from an array of context items.
   */
  protected function getArgumentPlaceholders(array $items = array()) {
    // Prepare context objects.
    $arguments = array();
    foreach ($items as $keyword => $item) {
      if (!empty($item['name'])) {
        $arguments[] = $item + array(
          'keyword' => $keyword,
          'identifier' => '',
          'settings' => array(),
          'id' => 0,
        );
      }
    }

    ctools_include('context');
    $placeholders = ctools_context_get_placeholders_from_argument($arguments);

    // Add logged in user context.
    $placeholders['logged-in-user'] = ctools_access_get_loggedin_context();
    return $placeholders;
  }

  /**
   * Prepares a list of CTools access plugin options.
   */
  protected function getAccessOptions(array $contexts = array()) {
    // List plugins.
    ctools_include('context');
    $plugins = ctools_get_relevant_access_plugins($contexts);
    $options = array();
    foreach ($plugins as $id => $plugin) {
      $options[$id] = $plugin['title'];
    }

    asort($options);

    return $options;
  }
}

/**
 * Rebuilds form.
 */
function image_style_access_path_ajax_rebuild($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  return $form;
}

/**
 * Rebuilds settings form.
 */
function image_style_access_path_ajax_rebuild_settings($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  return $form['settings'];
}
