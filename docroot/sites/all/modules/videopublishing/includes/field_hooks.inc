<?php
/**
 * @file
 * Provides field hook implementations for the videopublishing module.
 */

/**
 * Generic hook implementations for the videopublishing module.
 */
class VideopublishingFieldHooks {

  const VIDEO_FIELD_MACHINE_NAME = 'videopublishing_video';
  const DEFAULT_WIDGET = 'videopublishing_video_default_widget';
  const DEFAULT_FORMATTER = 'videopublishing_video_default_formatter';


  /**
   * Provides an implementation for hook_field_info().
   */
  public static function info() {
    return array(
      self::VIDEO_FIELD_MACHINE_NAME => array(
        'label' => t('Videopublishing video'),
        'description' => t('This field stores information about videos uploaded on videopublishing.com.'),
        'default_widget' => self::DEFAULT_WIDGET,
        'default_formatter' => self::DEFAULT_FORMATTER,
      ),
    );
  }


  /**
   * Provides an implementation for hook_field_instance_settings_form().
   */
  public static function instanceSettingsForm($field, $instance) {
    if (!self::ckeckLibrary()) {
      return array();
    }
    $settings = $instance['settings'];

    if (isset($settings['player'])) {
      $entity_ids = self::getFieldDataEntityIds($instance);
      if (!empty($entity_ids)) {
        $form['warning'] = array(
          '#theme' => 'html_tag',
          '#tag' => 'p',
          '#value' => t('There are entities that use the current field instance sql table for storage. Entity count: !n. Changing the Player or Format settings below, will start a (posibly lengthy) update process on the previously mentioned entities. A part of this process may require one or more cron runs.', array(
            '!n' => count($entity_ids),
          )),
          '#attributes' => array('class' => array('messages', 'warning')),
        );
      }
    }

    $form['player'] = array(
      '#title' => t('Player'),
      '#type' => 'select',
      '#options' => VideopublishingUtil::getOptions(VideopublishingUtil::OPTIONS_PLAYER),
      '#default_value' => isset($settings['player']) ? $settings['player'] : NULL,
      '#required' => TRUE,
    );

    $form['format'] = array(
      '#title' => t('Format'),
      '#type' => 'select',
      '#options' => VideopublishingUtil::getOptions(VideopublishingUtil::OPTIONS_FORMAT),
      '#default_value' => isset($settings['format']) ? $settings['format'] : NULL,
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Returns the ids of the entities that use the given field instance.
   * This method assumes that SQL Field Storage is used.
   *
   * @return array
   */
  private static function getFieldDataEntityIds($instance) {
    $field = field_info_field($instance['field_name']);

    $field_data_table = key($field['storage']['details']['sql']['FIELD_LOAD_CURRENT']);
    $select = db_select($field_data_table, 'fd');
    $select->addField('fd', 'entity_id');

    return $select->execute()->fetchCol();
  }


  /**
   * Provides an implementation for hook_field_update_instance().
   */
  public static function fieldUpdateInstance($instance, $prior_instance) {
    $settings = $instance['settings'];
    $prior_settings = $prior_instance['settings'];

    if ((
        isset($prior_settings['player']) && isset($prior_settings['format']) &&
        isset($settings['player']) && isset($settings['format'])
      ) && (
        $settings['player'] != $prior_settings['player'] ||
        $settings['format'] != $prior_settings['format']
      )) {
      $entity_ids = self::getFieldDataEntityIds($instance);
      if (!empty($entity_ids)) {
        $controller = entity_get_controller($instance['entity_type']);
        foreach ($entity_ids as $id) {
          set_time_limit(30);
          $entity = current($controller->load(array($id)));
          $save = self::entitySave($entity, $instance['entity_type']);
        }
      }
    }
  }


  /**
   * Provides an implementation for hook_field_validate();
   */
  public static function vaidate($item, $field) {
    // For now, no validation is required.
    foreach ($items as $delta => $item) {
      if (!empty($item['vpid'])) {
        if (!strlen($item['vpid']) < 32) {
          $errors[$field['field_name']][$langcode][$delta][] = array(
            'error' => 'videopublishing_invalid_vpid',
            'message' => t('The video id is invalid.'),
          );
        }
      }
    }
  }

  /**
   * Provides an implementation for hook_field_is_empty();
   */
  public static function isEmpty($item, $field) {
    return !((bool) $item['vpid']);
  }


  /**
   * Provides an implementation for hook_field_formatter_info().
   */
  public static function formatterInfo() {
    $formatters = array();

    $formatters[self::DEFAULT_FORMATTER] = array(
      'label' => t('Videopublishing default formatter'),
      'field types' => array(self::VIDEO_FIELD_MACHINE_NAME),
      'settings' => array(),
    );

    return $formatters;
  }


  /**
   * Provides an implementation for hook_field_formatter_view().
   */
  public static function formatterView($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $element = array();

    switch ($display['type']) {
      case self::DEFAULT_FORMATTER:
        foreach ($items as $delta => $item) {
          $item['delta'] = $delta;

          $data = VideopublishingData::get($item['vpid']);
          $item['image'] = $data['image'];
          $item['thumbnail'] = $data['thumbnail'];

          $item['embed_code'] = VideopublishingEmbedCode::get($item['vpid'], $instance['settings']['format'], $instance['settings']['player']);

          $element[$delta] = array(
            '#theme' => VideopublishingHooks::THEME_VIDEO_DEFAULT_FORMATTER,
            '#item' => $item,
          );
        }
        break;
    }

    return $element;
  }


  /**
   * The field widget.
   */

  /**
   * Provides an implementation for hook_field_widget_info().
   */
  public static function widgetInfo() {
    return array(
      self::DEFAULT_WIDGET => array(
        'label' => t('VP Default'),
        'field types' => array(self::VIDEO_FIELD_MACHINE_NAME),
        'settings' => array(
          'progress_indicator' => 'throbber',
          'preview_image_style' => 'thumbnail',
        ),
        'behaviors' => array(
          'multiple values' => FIELD_BEHAVIOR_DEFAULT,
          'default value' => FIELD_BEHAVIOR_DEFAULT,
        ),
      ),
    );
  }


  /**
   * Provides an implementation for hook_field_widget_form().
   */
  public static function widgetForm(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
    if (!self::ckeckLibrary()) {
      return array();
    }

    $element += array(
      '#delta' => $delta,
    );
    $element['vpid'] = array();

    switch ($instance['widget']['type']) {

      case self::DEFAULT_WIDGET :
        $element['vpid'] += array(
          '#title' => t('Video'),
          '#type' => 'select',
          '#options' => VideopublishingUtil::getOptions(VideopublishingUtil::OPTIONS_VIDEO),
          '#default_value' => isset($items[$delta]['vpid']) ? $items[$delta]['vpid'] : NULL,
        );
        $element['title'] = array(
          '#title' => t('Tile'),
          '#type' => 'textfield',
          '#default_value' => isset($items[$delta]['title']) ? $items[$delta]['title'] : NULL,
        );
        $element['body'] = array(
          '#title' => t('Description'),
          '#type' => 'text_format',
          '#format' => isset($items[$delta]['body_format']) ? $items[$delta]['body_format'] : filter_default_format(),
          '#default_value' => isset($items[$delta]['body']) ? $items[$delta]['body'] : NULL,
        );

    }

    return $element;
  }


  /**
   * Provides an implementation for hook_field_widget_error().
   */
  public static function widgetError() {
    switch ($error['error']) {
      case 'videopublishing_invalid_vpid':
        form_error($element, $error['message']);
        break;
      }
  }


  /**
   * Provides an implementation for hook_field_presave().
   */
  public static function presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
    foreach ($items as $delta => $item) {
      set_time_limit(20);
      self::presaveItem($instance, $items[$delta]);
    }
  }


  /**
   * A presave item helper.
   */
  private static function presaveItem($instance, &$item) {
    if (isset($item['body']) && is_array($item['body']) && isset($item['body']['value']) && $item['body']['value']) {
      $item['body_format'] = $item['body']['format'];
      $item['body'] = $item['body']['value'];
    }

    self::presaveItemEmbedCode($instance, $item);
    self::presaveItemData($instance, $item);
  }


  /**
   * Gets the video embed code.
   *
   * Checks if the embed code for the [vpid, format_id, player_id] was already
   * generated. If so, it will reuse that embed code, else it will query the
   * videopublishing webservice for a new embed code.
   */
  private static function presaveItemEmbedCode($instance, &$item) {
    $vpid = $item['vpid'];
    $format_id = $instance['settings']['format'];
    $player_id = $instance['settings']['player'];
    $embed_code = VideopublishingEmbedCode::get($vpid, $format_id, $player_id);

    if ($embed_code) {
      $item['stream_id'] = $vpid . '_' . $format_id;
      $item['publish_id'] = VideopublishingEmbedCode::get($vpid, $format_id, $player_id, VideopublishingEmbedCode::FIELD_PUBLISH_ID);
    }
    else {
      $vp = VideopublishingApi::getInstance();

      $s = json_decode($vp->setStream($vpid, $format_id));
      $item['stream_id'] = $s->item;

      $p = json_decode($vp->publish($player_id, $item['stream_id'], 'stream'));
      $item['publish_id'] = $p->item;

      $queue = DrupalQueue::get(VideopublishingHooks::QUEUE_VIDEO_EMBED_CODE, TRUE);
      $queue_item = array(
        'vpid' => $item['vpid'],
        'format_id' => $instance['settings']['format'],
        'player_id' => $instance['settings']['player'],
        'publish_id' => $item['publish_id'],
      );
      $queue->createItem($queue_item);
    }
  }

  /**
   * Gets the video information.
   *
   * First, it checks if the data for the given video is already stored in the
   * database. If not, it will query the videopublishing webservice to get the
   * necessary information.
   */
  private static function presaveItemData($instance, &$item) {
    $data = VideopublishingData::get($item['vpid']);
    if (!$data) {
      $vp = VideopublishingApi::getInstance();
      $data = json_decode($vp->getVideoInfo($item['vpid']), TRUE);
      VideopublishingData::set($item['vpid'], $data['video']);
    }
  }

  /**
   * Checks if the VP library exists and if not, it sets warning and error
   * messages.
   *
   * @return bool
   */
  private static function ckeckLibrary() {
    $vp = VideopublishingApi::getInstance();
    if ($vp->libraryExists()) {
      $settings = LazyVars::get('videopublishing_settings', NULL);
      if (!$settings || !$settings['api_user'] || !$settings['api_key']) {
        drupal_set_message(t('The VideoPublishing config is missing.'), 'error', FALSE);
        drupal_set_message(drupal_render(VideopublishingUtil::getInstallInstructions(TRUE)), 'error', FALSE);
        return FALSE;
      }
      else {
        return TRUE;
      }
    }
    else {
      drupal_set_message(t('The VideoPublishing library is missing.'), 'error', FALSE);
      drupal_set_message(drupal_render(VideopublishingUtil::getInstallInstructions()), 'warning', FALSE);
      return FALSE;
    }
  }

  /**
   * Save any type of entity.
   */
  private static function entitySave($entity, $entity_type) {
    $info = entity_get_info($entity_type);
    if (method_exists($entity, 'save')) {
      return $entity->save();
    }
    elseif (isset($info['save callback'])) {
      return $info['save callback']($entity);
    }
    elseif (in_array('EntityAPIControllerInterface', class_implements($info['controller class']))) {
      return entity_get_controller($entity_type)->save($entity);
    }
    elseif (function_exists($entity_type . '_save')) {
      return call_user_func($entity_type . '_save', $entity);
    }
    else {
      return FALSE;
    }
  }

}