<?php
/**
 * @file
 * Admin UI for file ownage settings.
 */

/**
 * Admin settings page.
 *
 * @return array
 *   A FAPI form
 */
function file_ownage_admin_settings($form, &$form_state) {
  $settings = variable_get('file_ownage_settings', file_ownage_default_settings());
  $form = array('file_ownage_settings' => file_ownage_settings_form($settings));
  // All settings set here get stored in one namespaced array.
  $form['#tree'] = TRUE;
  return system_settings_form($form);
}

/**
 * Subform that can be used to set the file_ownage preferences.
 *
 * Normally this is called as a straight system_settings_form,
 * but it may also be used as a subsettings config page when using rules,
 * if you need to make rules-based processes that have different, um, rules.
 *
 * @param array $settings
 *   Configs and context.
 *
 * @return array
 *   FAPI form.
 */
function file_ownage_settings_form(array $settings) {

  // Although I use a lot of fieldsets here for organization,
  // Not all of them are tree parents, so pay attention to #tree and #parents.
  $form = array();

  $form['embeds'] = array(
    '#type' => 'fieldset',
    // Short-circuit the tree-nesting of values,
    // this fieldset is just a visual wrapper, not structural.
    '#parents' => array('file_ownage_settings'),
    '#title' => t('Processing embeds witihin text'),
    '#description' => t('File Ownage can process text fields and improve file handling for links and embeds found in the markup.'),
  );

  $form['embeds']['node_save'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically process for embedded files on every node save.'),
    '#default_value' => $settings['node_save'],
  );

  $form['embeds']['show_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show an file embed action tab on each node.'),
    '#description' => '(May need a menu rebuild to take effect)',
    '#default_value' => $settings['show_tab'],
  );
  $form['embeds']['import_nearby'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import nearby files'),
    '#description' => t('When an embedded file is found that is local to the <em>website</em> (It probably still works), but not in our expected "files" directory, copy it to the correct files directory. This can help rationalize handmade "hybrid" drupal sites that contain legacy quirks.'),
    '#default_value' => $settings['import_nearby'],
  );

  //
  // Image ownage settings
  // .
  $form['image_ownage'] = array(
    '#type' => 'fieldset',
    '#attributes' => array(
      'id' => 'image_attach',
      'class' => array('filtering-fieldset'),
    ),
    '#title' => t('Image management'),
    '#description' => t('
      When an unattached image is found in an items markup,
      it\'s often useful to <em>attach</em> that image to the entity directly.
      Among other things, this helps ensure the files <em>usage</em> counter
      is kept up to date.
      Doing so is optional, but recommmended.
      <br/>
      Some of these methods can actually alter the markup - eg by removing
      an image that was pasted into the markup and instead attaching
      it as a feature image for more consistent displays.
      <br/>
      Additional custom ways of handling images can be added by other modules.
    '),
  );

  $attachment_methods = file_ownage_image_attachment_methods();
  $options = array();

  foreach ($attachment_methods as $method => $details) {
    $options[$method] = $details['label'];
    // If the method defines the need for further settings,
    // add those subsettings from the form callback.
    if (isset($details['subform']) && function_exists($details['subform'])) {
      $subform_func = $details['subform'];
      $subform = $subform_func(isset($settings[$method]) ? $settings[$method] : array());
    }
    else {
      $subform = array();
    }
    $form['image_ownage'][$method] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => $details['label'],
      '#attributes' => array(
        'class' => array(
          'filtered-fieldset-' . $method,
          'filtered-fieldset',
        ),
      ),
      '#description' => $details['description'],
    ) + $subform;
  }

  $form['image_ownage']['attach_method'] = array(
    '#weight' => -1,
    '#title' => "How to attach found images to the containing entity",
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $settings['image_ownage']['attach_method'],
    '#element_validate' => array('file_ownage_attach_method_validate'),
    '#attributes' => array('class' => array('filtering-selector')),
  );

  // File ownage also.
  $form['file_ownage'] = array(
    '#type' => 'fieldset',
    '#title' => 'Linked file management',
    '#description' => t(
      '
      Steps to try to manage <em>linked</em> files - (anchor references, not embeds)
      <small><code>&lt;a href="path/to/doc.pdf"&gt;pdf file&lt;/a&gt; ,
      &lt;a href="path/to/fullsize.jpg"&gt;large image&lt;/a&gt;</code></small>.
      If found, the contents of links like this should be absorbed and
      owned also.
      ', array()
    ),
    '#attributes' => array('class' => array('filtering-fieldset')),
  );
  $form['file_ownage']['import_files'] = array(
    '#weight' => -5,
    '#type' => 'checkbox',
    '#title' => t('Try to absorb linked files.'),
    '#default_value' => $settings['file_ownage']['import_files'],
  );
  $form['file_ownage']['suffix_list'] = array(
    '#weight' => -4,
    '#type' => 'textfield',
    '#title' => t('List of suffixes to treat as file resources'),
    '#default_value' => $settings['file_ownage']['suffix_list'],
    '#description' => t(
      '
      As we can\'t tell what file type is at the end of a link request,
      guess based on apparent suffix.
      Enter a comma-separated list, eg <code>pdf,doc,jpg</code>
      Be aware that this can cause direct links to remotely hosted resources
      to get copied locally.
      '
    ),
  );

  $attachment_methods = file_ownage_file_attachment_methods();
  $options = array();
  foreach ($attachment_methods as $method => $details) {
    $options[$method] = $details['label'];
    // If the method defines the need for further settings,
    // add those subsettings from the form callback.
    if (isset($details['subform']) && function_exists($details['subform'])) {
      $subform_func = $details['subform'];
      $subform = $subform_func(isset($settings[$method]) ? $settings[$method] : array());
    }
    else {
      $subform = array();
    }
    $form['file_ownage'][$method] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => $details['label'],
      '#attributes' => array(
        'class' => array(
          'filtered-fieldset-' . $method,
          'filtered-fieldset',
        ),
      ),
      '#description' => $details['description'],
    ) + $subform;
  }

  $form['file_ownage']['attach_method'] = array(
    '#weight' => -1,
    '#title' => "How to attach found images to the containing node",
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $settings['file_ownage']['attach_method'],
    '#element_validate' => array('file_ownage_attach_method_validate'),
    '#attributes' => array('class' => array('filtering-selector')),
  );


  // File ownage also.
  $form['filepaths'] = array(
    '#type' => 'fieldset',
    '#parents' => array('file_ownage_settings'),
    '#title' => 'Filepath Options',
  );

  $form['filepaths']['storage_path'] = array(
    '#title' => 'Storage path',
    '#type' => 'textfield',
    '#default_value' => $settings['storage_path'],
    '#description' => t('
      Optionally define a folder inside your files folder
      for all imported files to be put. EG <code>imported</code>.
      Fragments of the old folder hierarchy will also be built underneath here
      when importing, so it\'s usually OK to leave this blank.
    '),
  );
  $form['filepaths']['prettyfiles'] = array(
    '#title' => 'Pretty files',
    '#type' => 'checkbox',
    '#default_value' => $settings['prettyfiles'],
    '#description' => t('
      Support local filepaths of the form
      <code>http://example.com/files/filename.jpg</code> .
      Normal Drupal behaviour refers to files as being deep in your
      sites/site.name/files folder,
      but if you are using <a href="https://drupal.org/node/1960806#aegir_prettyfiles">Aegir or an .htaccess tweak</a>,
      you can shorten that.
      It\'s a lot more portable.
      ONLY use this if you know your server supports this option.
    '),
  );

  // Additional options.
  $strip_paths = array_filter((array) $settings['strip_paths']);
  $strip_paths[] = '';
  $form['filepaths']['strip_paths'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => 'Filepath cleanup',
    '#description' => t('
      You can provide a regular expression that could match part of a filepath 
      to strip when calculating the new import file storage path. 
      EG, if the target has paths like <code>_data/assets/image</code> 
      you can choose to throw away those extra layers.
      ', array()
    ),
  );

  foreach ($strip_paths as $pattern) {
    $form['filepaths']['strip_paths'][] = array(
      '#type' => 'textfield',
      '#default_value' => $pattern,
    );
  }
  $form['filepaths']['strip_paths']['help'] = array(
    '#type' => 'markup',
    '#markup' => t("
      By default, a folder structure reflecting where the original was fetched 
      from will be built to store the retrieved file in. 
      This preserves provenance and eliminates duplicates.
      However, if you want all files to be dropped in an directory unstructured,
      a filepath cleanup pattern of <code>|^.*/|</code> should remove that for you'
    "),
  );

  $form['domains'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => 'Domain management',
  );
  $form['domains']['domain_handler'] = array(
    '#type' => 'radios',
    '#title' => t('Restrict source domains'),
    '#options' => array(
      'ownage' => 'Own everything we can get. If references to external images are found, they will get localized',
      'exclude' => 'Do not own images from the listed domains',
      'include' => 'Own images only from these domains',
    ),
    '#default_value' => $settings['domains']['domain_handler'],
  );
  $form['domains']['domain_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Importing from external domains'),
    '#default_value' => implode("\n", $settings['domains']['domain_list']),
    '#description' => t('
      Add one URL per line. Start with http:// and feel
      free to include paths
      '
    ),
  );

  $form['lost_files'] = array(
    '#type' => 'fieldset',
    '#parents' => array('file_ownage_settings'),
    '#title' => t('Lost Files'),
  );
  $form['lost_files']['help'] = array(
    '#type' => 'markup',
    '#markup' => '<p>
      If the linked file cannot be found where expected,
      we can try looking in alternative locations. This could help
      with importing old content, or just fixing up broken links.
      </p><p>
      Enter some paths to scan to see if a matching file is found there.
      This can even be a remote URL.
      Paths will be searched in order, first match being the best.
      </p><p>
      To <em>search</em> for files, enter a path with a wildcard <b>*</b>
      and that location will be scanned for the best match to the expected
      filename. Wildcard searches will not work on URLs.
      If a match is found, the processing rules above will be used and the
      source link will be updated to the correct location.
      </p>
    ',
  );
  $form['lost_files']['show_repair_file_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a file repair action tab on file entity page.'),
    '#description' => 'Useful only if you are using file_entity or Media Suite. (May need a menu rebuild to take effect)',
    '#default_value' => $settings['show_repair_file_tab'],
  );
  $form['lost_files']['show_check_file_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a file check action tab on file entity page.'),
    '#description' => 'Useful only for debugging',
    '#default_value' => $settings['show_check_file_tab'],
  );

  $seek_paths = $settings['seek_paths'];
  // Eliminate blank values and add one blank at the end.
  $seek_paths = array_filter($seek_paths, 'trim');
  $seek_paths[] = '';

  $form['lost_files']['seek_paths'] = array(
    '#tree' => TRUE,
    '#description' => t('
      Paths to scan for this file, by filename and parentage.
      Can be a local filestream identifier (eg public://oldfiles)
      or a system path (eg /var/www/backup/assets)
      or even an URL (http://example.com/images/).

      Remote URL retrieval requires the <a href="https://www.drupal.org/project/remote_stream_wrapper">remote stream wrapper module</a>.

      For local filestreams, you can add a wildcard at the end also,
      eg public://imported/* .
      Without a wildcard, only reasonably precise matches will be checked.

      Include trailing slash.
    '),
  );
  foreach ($seek_paths as $delta => $path) {
    $form['lost_files']['seek_paths'][] = array(
      '#title' => t('Path'),
      '#type' => 'textfield',
      '#default_value' => isset($seek_paths[$delta]) ? $seek_paths[$delta] : '',
    );
  }

  $form['lost_files']['do_directory_scan'] = array(
    '#type' => 'checkbox',
    '#title' => t('Perform a recursive directory scan under the given paths.'),
    '#description' => 'This can be incredibly slow and it runs for every file',
    '#default_value' => $settings['do_directory_scan'],
  );

  $form['lost_files']['relink_behaviour'] = array(
    '#title' => t('When found:'),
    '#type' => 'radios',
    '#options' => array(
      'move_file' => 'Move file to expected location',
      'update_db' => 'Change reference in the database',
      'copy_file' => 'Copy file to expected location',
    ),
    '#required' => TRUE,
    '#default_value' => $settings['relink_behaviour'],
    '#description' => t('
      No matter what choices you set here,
      some behaviours may change depending on context.
      EG, if the file was found in a read-only location,
      it will not "move" but only copy.
      If an existing record for an expected file is already in the database
      (looks like a duplicate) then the database record may be merged.
    '),
  );



  $form['loglevel'] = array(
    '#title' => t('Log level'),
    '#description' => t('
      For debugging, messages at this level or higher
      will be echoed to the screen.
      "Notice" is probably the most informative as it describes changes
      that get made during the process.
    '),
    '#type' => 'select',
    '#options' => watchdog_severity_levels(),
    '#default_value' => $settings['loglevel'],
  );

  // The form has some UI additions to help select the input.
  drupal_add_js(drupal_get_path('module', 'file_ownage') . '/filtering_fieldset.js');
  return $form;
}

/**
 * Tidy up some values.
 *
 * Implements hook_FORM_ID_validate().
 */
function file_ownage_admin_settings_validate(&$form, &$form_state) {
  #$form_state['values']['file_ownage_settings']['domains']['domain_list'] = explode("\n", trim($form_state['values']['file_ownage_settings']['domains']['domain_list']));
}

/**
 * Implements hook_FORM_ID_validate().
 */
function file_ownage_attach_method_validate(&$element, &$form_state) {

  // Ensure some of the options have the required dependencies.
  switch ($element['#value']) {

    case 'file_ownage_attach_filefield':
      if (!module_exists('file')) {
        form_error($element, t('This method requires the filefield module to be enabled.'));
      }
      break;

    case 'file_ownage_attach_fileholder_nodereference':
      if (!module_exists('nodereference')) {
        form_error($element, t('This method requires the nodereference module to be enabled.'));
      }
      break;

  }
}


/**
 * Extra preferences to support the 'none' option.
 *
 * @param array $subsettings
 *   Settings values to expose.
 *
 * @return array
 *   A FAPI form fragment.
 *
 * @broken
 */
function file_ownage_attach_none_subform(array $subsettings = array()) {

  $available_filefields = file_ownage_available_fields(array('file', 'image'));
  $subsettings += array(
    'path' => '',
  );

  $subform['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Where found files should all be kept (inside the files directory %file_directory_path). Leaving it blank is fine.', array('%file_directory_path' => variable_get('file_public_path', NULL))),
    '#default_value' => $subsettings['path'],
    '#description' => t('This value may be overridden if using filefield or a module which changes your storage paths. This path is only used if using the simpler attachment options below.'),
  );

  return $subform;
}


/**
 * Extra preferences to support the filefield option.
 *
 * @param array $subsettings
 *   Settings values to expose.
 *
 * @return array
 *   A FAPI form fragment.
 */
function file_ownage_attach_filefield_subform(array $subsettings = array()) {

  $available_filefields = file_ownage_available_fields(array('file', 'image'));
  $subsettings += array(
    'filefield_name' => '',
  );

  $subform = array();
  $subform['about_filefields'] = array(
    '#type' => 'markup',
    '#value' => '<p>' . t('The filefields you select should be consistent and re-used throughout your <a href="!content_types_url">content types</a>. If a filefield is unavailable for a node, it will not be used.', array('!content_types_url' => url('admin/structure/types'))) . '</p>',
  );
  $subform['filefield_name'] = array(
    '#title' => "Which filefield to use for supplimentary images",
    '#type' => 'select',
    '#options' => $available_filefields,
    '#default_value' => $subsettings['filefield_name'],
    '#description' => 'This filefield is expected to have "unlimited" entries.',
  );
  return $subform;
}


/**
 * Extra preferences to support the feature filefield option.
 *
 * @param array $subsettings
 *   Settings values to expose.
 *
 * @return array
 *   A FAPI form fragment.
 */
function file_ownage_attach_feature_filefield_subform(array $subsettings = array()) {

  $subsettings += array(
    'feature_filefield_name' => 'image_feature',
    'feature_filefield_min_width' => '200',
    'remove_original' => TRUE,
  );
  $available_filefields = file_ownage_available_fields('image');

  $subform = array();
  $subform['about_filefields'] = array(
    '#type' => 'markup',
    '#value' => '<p>' . t('The filefields you select should be consistent and re-used throughout your <a href="!content_types_url">content types</a>. If a filefield is unavailable for a node, it will not be used.', array('!content_types_url' => url('admin/structure/types'))) . '</p>',
  );
  $subform['feature_filefield_name'] = array(
    '#title' => "Which filefield to use for feature file",
    '#type' => 'select',
    '#options' => $available_filefields,
    '#default_value' => $subsettings['feature_filefield_name'],
    '#description' => 'This option supports attaching one file that may be rendered uniquely (eg as a representative teaser image) while the others go inline. This filefield chosen here is expected to have a limit of one item.',
  );
  $subform['feature_filefield_min_width'] = array(
    '#title' => "What is the minimum width before an embedded image is eligible to become a featured image.",
    '#type' => 'textfield',
    '#size' => 4,
    '#default_value' => $subsettings['feature_filefield_min_width'],
    '#description' => 'You don\'t want small images accidentally being chosen to be the feature image. Enter a minimum width in pixels. Images smaller than that will not become features.',
  );
  $subform['remove_original'] = array(
    '#type' => 'checkbox',
    '#title' => t('When an embedded feature image is found and attached, remove it from the normal text flow'),
    '#default_value' => $subsettings['remove_original'],
    '#description' => t('This allows you to reformat the feature image and theme it your own way. It only applies on "feature" images, not supplimentary ones'),
  );

  return $subform;
}

/**
 * Extra preferences to support the imageholder_nodereference option.
 *
 * @param array $subsettings
 *   Settings values to expose.
 *
 * @return array
 *   A FAPI form fragment.
 */
function file_ownage_attach_fileholder_nodereference_subform(array $subsettings = array()) {

  // These defaults are unlikely to be right,
  // provided here as a hint for a vanilla installation.
  $subsettings += array(
    'imageholder_type' => 'image',
    'imagefield_name' => 'image',
    'fileholder_type' => 'file',
    'filefield_name' => 'file',
    'nodereference_field' => 'attached',
  );
  $available_content_types = node_type_get_names();
  $available_nodereferencefields = file_ownage_available_fields('node_reference');
  $available_imagefields = file_ownage_available_fields('image');
  $available_filefields = file_ownage_available_fields(array('file', 'image'));

  $subform = array();
  $subform['imageholder_type'] = array(
    '#title' => "This content type to create as a placeholder for new resources",
    '#type' => 'select',
    '#options' => $available_content_types,
    '#default_value' => $subsettings['imageholder_type'],
    '#description' => 'You may have to make a new content type called, eg, "image" and add an imagefield to it.',
  );
  $subform['imagefield_name'] = array(
    '#title' => "Which filefield on the target content type to store the image file in",
    '#type' => 'select',
    '#options' => $available_filefields,
    '#default_value' => $subsettings['imagefield_name'],
  );
  $subform['fileholder_type'] = array(
    '#title' => "This content type to create as a placeholder for new files",
    '#type' => 'select',
    '#options' => $available_content_types,
    '#default_value' => $subsettings['fileholder_type'],
    '#description' => 'You may have to make a new content type called, eg, "document" and add a filefield to it.',
  );
  $subform['filefield_name'] = array(
    '#title' => "Which filefield on the target document content type to store the file in",
    '#type' => 'select',
    '#options' => $available_filefields,
    '#default_value' => $subsettings['filefield_name'],
  );
  $subform['nodereference_field'] = array(
    '#title' => "The nodereference field used to link from the parent node content to the file holder.",
    '#type' => 'select',
    '#options' => $available_nodereferencefields,
    '#default_value' => $subsettings['nodereference_field'],
  );
  return $subform;
}


/**
 * Return an array of all instances of filefield from any content type.
 *
 * Lookup utility for the selection form.
 *
 * @param string|array $field_type
 *   Eg 'file', 'node_reference'. String or array of types.
 *
 * @return array
 *   List of field instances.
 */
function file_ownage_available_fields($field_type) {

  $field_type = is_array($field_type) ? $field_type : array($field_type);
  $available_fields = array();

  $field_instances = field_info_fields();
  foreach ($field_instances as $field_id => $field_def) {
    if (in_array($field_def['type'], $field_type)) {
      $available_fields[$field_id] = $field_def['field_name'];
    }
  }
  return $available_fields;
}
