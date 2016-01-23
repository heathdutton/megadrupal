<?php
/**
 * @file
 * Page and form callback.
 */

/**
 * Form callback to import the job translation.
 */
function tmgmt_server_mailer_import_form($form, &$form_state, TMGMTJob $job) {

  drupal_set_title(t('Import translation for job %job', array('%job' => $job->label())), PASS_THROUGH);

  $form_state['tmgmt_job'] = $job;

  // If we have a job item not in active state it means the xliff translation has
  // been uploaded.
  /** @var TMGMTJobItem $item */
  foreach ($job->getItems() as $item) {
    if (!$item->isActive()) {
      $form['info'] = array(
        '#type' => 'markup',
        '#markup' => t('The translation of this job has been already imported, it is not possible to import a new translation.'),
      );
      return $form;
    }
  }

  $supported_formats = array_keys(tmgmt_file_format_plugin_info());
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('File file'),
    '#size' => 50,
    '#description' => t('Supported formats: @formats.', array('@formats' => implode(', ', $supported_formats))),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Import form submit callback.
 *
 * It duplicates functionality from tmgmt_file_import_form_submit() and changes
 * only the messages handling and content.
 */
function tmgmt_server_mailer_import_form_submit($form, &$form_state) {
  // Ensure we have the file uploaded.
  $job = $form_state['tmgmt_job'];
  $supported_formats = array_keys(tmgmt_file_format_plugin_info());
  if ($file = file_save_upload('file', array('file_validate_extensions' => array(implode(' ', $supported_formats))))) {
    $extension = pathinfo($file->uri, PATHINFO_EXTENSION);
    $controller = tmgmt_file_format_controller($extension);
    if ($controller) {
      // Validate the file.
      $validated_job = $controller->validateImport($file->uri);
      if (!$validated_job) {
        $job->addMessage('Failed to validate file, import aborted.', array(), 'error');
      }
      elseif ($validated_job->tjid != $job->tjid) {
        $uri = $validated_job->uri();
        $label = $validated_job->label();
        $job->addMessage('Import file is from job <a href="@url">@label</a>, import aborted. Make sure you used correct import link.',
          array('@url' => url($uri['path']), '@label' => $label));
      }
      else {
        try {
          // Validation successful, start import.
          $job->addTranslatedData($controller->import($file->uri));
          $job->addMessage('Successfully imported file.');
        } catch (Exception $e) {
          $job->addMessage('File import failed with the following message: @message', array('@message' => $e->getMessage()), 'error');
        }
      }
    }
  }
  foreach ($job->getMessagesSince() as $message) {
    // Ignore debug and status messages.
    if ($message->type == 'debug' || $message->type == 'status') {
      continue;
    }
    if ($text = $message->getMessage()) {
      drupal_set_message(filter_xss($text), $message->type);
    }
  }
  drupal_set_message(t('The translation file has been successfully imported.'));
}
