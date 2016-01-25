<?php

class CiviCrmFlexiformImageElement extends CiviCrmEntitiesFlexiformElementBase {


  /**
   * Return the form element for this FlexiformElement.
   */
  public function form($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    $parents = $form['#parents'];
    $parents[] = $this->element_info['name'];

    // Work out the default value.
    $file = null;

    // Should we load an initial value from an existing entity?
    if ($entity->id && isset($entity->{$this->element_info['name']})) {
      $default = $entity->{$this->element_info['name']};
    }
    elseif (!empty($this->settings['default_value']['default_value'])) {
      $default = $this->settings['default_value']['default_value'];
    }

    if(!empty($default)) {
      $file = self::getFilebyPath($default);
    }

    $form[$this->element_namespace] = array(
      '#type' => 'managed_file',
      '#parents' => $parents,
      '#title' => $this->label(),
      '#upload_location' => 'public://',
      '#upload_validators' => array(),
      '#default_value' => ($file ? $file->fid : null),
    );

    $form = parent::form($form, $form_state, $entity);

    return $form;
  }

  /**
   * Validate the form element.
   */
  public function formValidate($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    parent::formValidate($form, $form_state, $entity, $language);
  }

  /**
   * Submit the form element.
   */
  public function formSubmit($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    parent::formSubmit($form, $form_state, $entity, $language);
  }

  private static function getFilebyPath($file_src) {
    $wrapper = file_stream_wrapper_get_instance_by_uri('public://');
    $public_path = $wrapper->realpath();
    $public_path = str_replace(DRUPAL_ROOT, '', $public_path);
    $uri = str_replace($public_path, 'public:/', $file_src);
    $files = file_load_multiple(array(), array('uri' => $uri));
    if(!empty($files)) {
      return reset($files);
    }
    return null;
  }

  /**
   * Extract the submitted values for this form element.
   */
  public function formExtractValues($form, &$form_state, $entity) {
    $value = parent::formExtractValues($form, $form_state, $entity);
    $uid = CRM_Core_BAO_UFMatch::getUFId($entity->id);
    if(empty($value['fid'])) {
      // If the original file was removed, we need to tell Drupal about the change so the cron will clean it up.
      // Check if originally there was a file and if the current upload is empty.
      if(isset($entity->{$this->element_info['name']})) {
        self::removeFile($entity->{$this->element_info['name']}, $uid);
      }

      return "";
    }
    // Check if nothing has changed.
    elseif($form[$this->element_namespace]['#default_value'] == $value['fid']) {
      return $entity->{$this->element_info['name']};
    }

    // We have a new uploaded file, continue to save its usage and return the path to Flexiform.

    // Set the file's status so the cron won't clean it up thinking it was never used.
    $file = file_load($value['fid']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'user', 'user', $uid);

    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $src = $wrapper->realpath(); // Returns the full system path.
    // Shorten just to relative path.
    $src = str_replace(DRUPAL_ROOT, '', $src);
    return $src;
  }

  private static function removeFile($file_src, $uid) {
    if (!empty($file_src)) {
      $file = self::getFilebyPath($file_src);
      file_usage_delete($file, 'user', 'user', $uid);
      $file->status = 0;
      file_save($file);      
    }
  }

  /**
   * {@inheritdoc}
   */
  public function configureForm($form, &$form_state, $flexiform) {
    $form = parent::configureForm($form, $form_state, $flexiform);

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function configureFormSubmit($form, &$form_state, $flexiform) {

    parent::configureFormSubmit($form, $form_state, $flexiform);
  }

}
