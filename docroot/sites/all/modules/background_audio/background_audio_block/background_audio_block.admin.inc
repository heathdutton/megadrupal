<?php
/**
 * @file
 * Pages for the Background Audio Block module.
 */

function background_audio_block_file_badelete($form, &$form_state, $file) {
  $form['#file'] = $file;

  return confirm_form(
    $form,
    t('Are you sure you want to delete !file from playlist?', array('!file' => $file->filename)),
    'admin/structure/block/manage/background_audio_block/background_audio/configure'
  );
}

function background_audio_block_file_badelete_submit($form, &$form_state) {
  $file = $form['#file'];
  file_usage_delete($file, 'background_audio', 'block', 0);
  file_delete($file);

  $files = variable_get('background_audio_files', array());
  unset($files[$file->fid]);
  variable_set('background_audio_files', $files);

  $form_state['redirect'] = 'admin/structure/block/manage/background_audio_block/background_audio/configure';
}
