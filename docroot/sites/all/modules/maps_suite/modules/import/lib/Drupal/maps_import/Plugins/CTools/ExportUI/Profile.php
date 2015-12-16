<?php

/**
 * @file
 * CTools exportable plugin for MaPS Import Profile.
 *
 * Note that since MaPS Suite module requires at least PHP 5.3, we do
 * need to take care of PHP4 compatibility.
 */

namespace Drupal\maps_import\Plugins\CTools\ExportUI;

use Drupal\maps_import\Cache\Object\Converter as CacheConverter;
use Drupal\maps_import\Cache\Object\Profile as CacheProfile;

/**
 * CTools Export UI class handler for MaPS Import Profile.
 */
class Profile extends \ctools_export_ui {

  /**
   * @inheritdoc
   */
  public function init($plugin) {
    module_load_include('inc', 'maps_import', 'includes/maps_import.ctools');

    // Remove the "list" local task that is automatically added by CTools.
    unset($plugin['menu']['items']['list']);
    return parent::init($plugin);
  }

  /**
   * @inheritdoc
   *
   * @return ProfileExportable
   */
  public function load_item($profile_name) {
    if ($profile = CacheProfile::getInstance()->loadSingle($profile_name, 'name')) {
      return new ProfileExportable($profile);
    }

    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function set_item_state($state, $js, $input, $profile) {
    maps_import_profile_export_set_status($profile, $state);

    if (!$js) {
      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }
    else {
      return $this->list_page($js, $input);
    }
  }

  /**
   * @inheritdoc
   */
  public function build_operations($profile) {
    $schema = ctools_export_get_schema($this->plugin['schema']);
    $operations = $this->plugin['allowed operations'];
    $operations['import'] = FALSE;
    $operations['edit']['weight'] = -10;
    $operations['delete']['weight'] = 10;

    if ($profile->{$schema['export']['export type string']} == t('Normal')) {
      $operations['revert'] = FALSE;
    }
    elseif ($profile->{$schema['export']['export type string']} == t('Overridden')) {
      $operations['delete'] = FALSE;
    }
    else {
      $operations['revert'] = FALSE;
      $operations['delete'] = FALSE;
    }
    if (empty($profile->disabled)) {
      $operations['enable'] = FALSE;
    }
    else {
      $operations['disable'] = FALSE;
    }

    $plugin = $this->plugin;

    foreach ($plugin['menu']['items'] as $item_id => &$item) {
      $item['path'] = strtr($item['path'], array('%maps_import_profile' => '%ctools_export_ui'));
    }

    $allowed_operations = array();

    foreach ($operations as $op => $info) {
      if (!empty($info)) {
        $allowed_operations[$op] = array(
          'title' => $info['title'],
          'href' => ctools_export_ui_plugin_menu_path($plugin, $op, $profile->{$this->plugin['export']['key']}),
        );
        if (!empty($info['ajax'])) {
          $allowed_operations[$op]['attributes'] = array('class' => array('use-ajax'));
        }
        if (!empty($info['token'])) {
          $allowed_operations[$op]['query'] = array('token' => drupal_get_token($op));
        }
        if (isset($info['weight'])) {
          $allowed_operations[$op]['weight'] = $info['weight'];
        }
      }
    }

    $allowed_operations = array_merge($allowed_operations, module_invoke_all('maps_import_profile_action_links', $profile->getProfile()));
    uasort($allowed_operations, 'drupal_sort_weight');

    return $allowed_operations;
  }

  /**
   * @inheritdoc
   */
  public function list_table_header() {
    return array(
      array('data' => t('Title'), 'class' => array('ctools-export-ui-title')),
      array('data' => t('Machine name'), 'class' => array('ctools-export-ui-name')),
      t('Publication'),
      t('Root object'),
      t('Format'),
      t('Differential'),
      t('Status'),
      array(
        'data' => array(
          '#theme' => 'html_tag',
          '#tag' => 'dfn',
          '#value' => t('M.O./R.'),
          '#attributes' => array(
            'title' => t('Max objects per request'),
          ),
        ),
      ),
      array(
        'data' => array(
          '#theme' => 'html_tag',
          '#tag' => 'dfn',
          '#value' => t('M.O./O.'),
          '#attributes' => array(
            'title' => t('Max objects per mapping operation'),
          ),
        ),
      ),
      array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage')),
      array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations')),
    );
  }

  /**
   * @inheritdoc
   */
  public function list_build_row($profile, &$form_state, $operations) {
    // Set up sorting
    $name = $profile->{$this->plugin['export']['key']};
    $schema = ctools_export_get_schema($this->plugin['schema']);

    // Note: $item->{$schema['export']['export type string']} should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($profile->disabled) . $name;
        break;
      case 'title':
        $this->sorts[$name] = $profile->{$this->plugin['export']['admin_title']};
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'storage':
        $this->sorts[$name] = $profile->{$schema['export']['export type string']} . $name;
        break;
    }

    $this->rows[$name]['data'] = array();
    $this->rows[$name]['class'] = !empty($profile->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data'][] = array('data' => check_plain($profile->{$this->plugin['export']['admin_title']}), 'class' => array('ctools-export-ui-title'));
    }
    $this->rows[$name]['data'][] = array('data' => check_plain($name), 'class' => array('ctools-export-ui-name'));

    $this->rows[$name]['data'][] = check_plain($profile->getProfile()->getPublicationId());
    $this->rows[$name]['data'][] = check_plain($profile->getProfile()->getRootObjectId());
    $this->rows[$name]['data'][] = drupal_strtoupper(check_plain($profile->getProfile()->getFormat()));
    $this->rows[$name]['data'][] = $profile->getProfile()->getOptionsItem('differential') ? t('Enabled') : t('Disabled');
    $this->rows[$name]['data'][] = $profile->getProfile()->isEnabled() ? t('Enabled') : t('Disabled');
    $this->rows[$name]['data'][] = check_plain($profile->getProfile()->getMaxObjectsPerRequest());
    $this->rows[$name]['data'][] = check_plain($profile->getProfile()->getMaxObjectsPerOp());

    $this->rows[$name]['data'][] = array('data' => check_plain($profile->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage'));

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$name]['data'][] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = $profile->{$this->plugin['export']['admin_description']};
    }
  }

  /**
   * @inheritdoc
   */
  public function edit_finish_validate(&$form, &$form_state) {
    if ($form_state['op'] != 'edit') {
      // Validate the export key. Fake an element for form_error().
      $export_key = $this->plugin['export']['key'];
      $element = array(
        '#value' => $form_state['input'][$export_key],
        '#parents' => array($export_key),
      );
      $form_state['plugin'] = $this->plugin;
      ctools_export_ui_edit_name_validate($element, $form_state);
    }
  }

  /**
   * @inheritdoc
   *
   * We do not want to delete an existing profile, because all related
   * objects and entities should be re-attached to the overriding profile.
   *
   * @todo add options to select the update mode (affect entities to the
   *   new profile, remove related entities, unlink them)
   */
  public function edit_wizard_finish(&$form_state) {
    $form_state['complete'] = TRUE;

    if ($form_state['form type'] === 'import') {
      $form_state['item']->setPid(NULL);

      if (!empty($form_state['item']->export_ui_allow_overwrite)) {
        $export_key = $this->plugin['export']['key'];

        if ($profile = CacheProfile::getInstance()->loadSingle($form_state['input'][$export_key], 'name')) {
          if (isset($form_state['item']->old_profile) && $profile->getPid() === $form_state['item']->old_profile->getPid()) {
            if (isset($form_state['input']['converters'])) {
              $form_state['item']->old_converters = $form_state['input']['converters'];
            }
          }

          $form_state['item']->old_profile = $profile;
        }
      }
    }

    $this->edit_cache_clear($form_state['item'], $form_state['form type']);
  }

  /**
   * @inheritdoc
   */
  public function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    if ($form_state['form type'] === 'import' && !empty($form_state['item']->export_ui_allow_overwrite)) {
      if ($profile = CacheProfile::getInstance()->loadSingle($form_state['item']->getName(), 'name')) {
        $form_state['item']->old_profile = $profile;
        $form['converters'] = array('#tree' => TRUE);
        $options = array();

        foreach ($form_state['item']->getConverters() as $name => $converter) {
          $options[$name] = t('@title (@name)', array('@title' => $converter->getTitle(), '@name' => $name));
        }

        foreach (CacheConverter::getInstance()->load(array($profile->getPid()), 'pid') as $cid => $converter) {
          $element = &$form['converters'][$converter->getCid()];
          $element = array(
            '#type' => 'fieldset',
            '#title' => t('@title (@name)', array('@title' => $converter->getTitle(), '@name' => $converter->getName())),
          );

          $element['mode'] = array(
            '#type' => 'radios',
            '#title' => t('Replace mode'),
            '#options' => array(
              'unlink' => t('Keep all the entities that have been processed by this converter but desynchronize them from MaPS Suite'),
              'delete' => t('Delete all entities that have been created by the original converter'),
            ),
            '#default_value' => 'unlink',
          );

          if ($options) {
            $element['override'] = array(
              '#type' => 'select',
              '#title' => t('Override this converter'),
              '#options' => $options,
            );

            $element['mode']['#options']['reassign'] = t('Assign all entities that have been processed by this converter to another');

            if (isset($options[$converter->getName()])) {
              $element['mode']['#default_value'] = 'reassign';
              $element['override']['#default_value'] = $converter->getName();
            }
          }
        }
      }
    }
  }

}
