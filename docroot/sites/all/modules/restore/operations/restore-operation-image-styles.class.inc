<?php
/**
 * @file
 * Restore operation: image styles class.
 */

/**
 * The image styles restore operation class.
 */
class RestoreOperationImageStyles extends RestoreOperation {

  /**
   * Overriden RestoreOperation::item().
   */
  protected function item($key) {
    $style = image_style_load($key);
    return array(
      'label' => $style['label'],
      'effects' => $style['effects'],
    );
  }

  /**
   * Overriden RestoreOperation::itemLabel().
   */
  protected function itemLabel($key, $item) {
    return "{$key} - {$item['label']}";
  }

  /**
   * Overriden RestoreOperation::itemStatus().
   */
  protected function itemStatus($key, $item) {
    $style = image_style_load($key);
    $exists = $matches = FALSE;

    if (count($style['effects']) == count($item['effects'])) {
      $exists = $matches = TRUE;

      if ($style['label'] != $item['label']) {
        $matches = FALSE;
      }

      $idx = 0;
      $keys = array_keys($item['effects']);
      foreach ($style['effects'] as $effect) {
        if (
          !isset($item['effects'][$keys[$idx]]) ||
          ($effect['name'] != $item['effects'][$keys[$idx]]['name']) ||
          ($effect['data'] != $item['effects'][$keys[$idx]]['data'])
        ) {
          $matches = FALSE;
          break;
        }

        $idx++;
      }
    }

    if ($exists && $matches) {
      return RestoreScript::STATUS_ACTIVE;
    }
    elseif ($exists) {
      return RestoreScript::STATUS_OVERRIDEN;
    }
    else {
      return RestoreScript::STATUS_MISSING;
    }
  }

  /**
   * Overriden RestoreOperation::itemRestore().
   */
  public function itemRestore($key, $item) {
    $style = image_style_load($key);
    if (!$style || !isset($style['isid'])) {
      $style = array(
        'name' => $key,
        'label' => $item['label'],
        'effects' => array(),
      );

      image_style_save($style);
      $style = image_style_load($key);
    }

    $effects = $style['effects'];
    foreach ($item['effects'] as $action_effect) {
      $effect = array(
        'name' => $action_effect['name'],
      );

      foreach ($effects as $idx => $style_effect) {
        if ($style_effect['name'] == $action_effect['name']) {
          $effect = $style_effect;
          unset($effects[$idx]);
          break;
        }
      }

      $effect['isid'] = $style['isid'];
      $effect['data'] = $action_effect['data'];
      image_effect_save($effect);
    }

    if (count($effects)) {
      foreach ($effects as $effect) {
        image_effect_delete($effect);
      }
    }
  }

  /**
   * Overriden RestoreOperation::available().
   */
  public function available($filter = FALSE, $show_selected = FALSE) {
    $styles = array();

    foreach (image_styles() as $name => $style) {
      $label = $name . ' - ' . $style['label'];
      if (
        (!$show_selected && $this->has($name)) ||
        ($filter && stripos($label, $filter) === FALSE)
      ) {
        continue;
      }

      $styles[$name] = $label;
    }

    return $styles;
  }
}
