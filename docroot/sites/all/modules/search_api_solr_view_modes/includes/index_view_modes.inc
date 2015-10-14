<?php

class SearchAPIAlterIndexViewModes extends SearchApiAbstractAlterCallback {

  /**
   * Only support indexes containing entities.
   * @see SearchApiAlterCallbackInterface::supportsIndex()
   */
  public function supportsIndex(SearchApiIndex $index) {
    return (bool) entity_get_info($index->item_type);
  }

  public function configurationForm() {
    $info = entity_get_info($this->index->item_type);
    $view_modes = array();
    foreach ($info['view modes'] as $key => $mode) {
      $view_modes[$key] = $mode['label'];
    }
    $this->options += array('modes' => reset($view_modes));
    if (count($view_modes) > 1) {
      $form['modes'] = array(
        '#type' => 'select',
        '#title' => t('View mode'),
        '#options' => $view_modes,
        '#multiple' => TRUE,
        '#default_value' => $this->options['modes'],
      );
    }
    else {
      $form['modes'] = array(
        '#type' => 'value',
        '#value' => $this->options['modes'],
      );
      if ($view_modes) {
        $form['note'] = array(
          '#markup' => '<p>' . t('Entities of type %type have only a single view mode. ' .
              'Therefore, no selection needs to be made.', array('%type' => $info['label'])) . '</p>',
        );
      }
      else {
        $form['note'] = array(
          '#markup' => '<p>' . t('Entities of type %type have no defined view modes. ' .
              'This might either mean that they are always displayed the same way, or that they cannot be processed by this alteration at all. ' .
              'Please consider this when using this alteration.', array('%type' => $info['label'])) . '</p>',
        );
      }
    }
    return $form;
  }

  public function alterItems(array &$items) {
    // Prevent session information from being saved while indexing.
    drupal_save_session(FALSE);

    // Force the current user to anonymous to prevent access bypass in search
    // indexes.
    $original_user = $GLOBALS['user'];
    $GLOBALS['user'] = drupal_anonymous_user();

    $type = $this->index->item_type;
    $modes = empty($this->options['modes']) ? 'full' : $this->options['modes'];
    foreach ($items as $id => &$item) {
      // Since we can't really know what happens in entity_view() and render(),
      // we use try/catch. This will at least prevent some errors, even though
      // it's no protection against fatal errors and the like.
      foreach ($modes as $mode) {
        $field = 'search_api_viewed_' . $mode;

        try {
          $render = entity_view($type, array(entity_id($type, $item) => $item), $mode);
          $text = render($render);
          if (!$text) {
            $item->{$field} = NULL;
            continue;
          }
          $item->{$field} = $text;
        }
        catch (Exception $e) {
          $item->{$field} = NULL;
        }
      }
    }

    // Restore the user.
    $GLOBALS['user'] = $original_user;
    drupal_save_session(TRUE);
  }

  public function propertyInfo() {
    $info = array();
    $modes = empty($this->options['modes']) ? 'full' : $this->options['modes'];

    foreach ($modes as $mode) {
      $info['search_api_viewed_' . $mode] = array(
        'label' => t('Entity HTML output (@mode view mode)', array('@mode' => $mode)),
        'description' => t('The static HTML of the entity rendered in the @mode mode when viewed.', array('@mode' => $mode)),
        'type' => 'text',
      );
    }

    return $info;
  }
}