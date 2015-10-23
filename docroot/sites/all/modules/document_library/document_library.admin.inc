<?php

/**
 * @file
 * Admin features for the document_library module.
 */

/**
 * Admin Settings.
 */
function document_library_admin_settings() {
  module_load_include('inc', 'document_library', 'document_library.user');

  $form = array();

  // Search Form Settings.
  $form['document_library_allow_searching'] = array(
    '#title'            => t('Allow Searching Documents'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_allow_searching', 1),
    '#description'      => t('Determines whether or not the search form is displayed at the top of the document library.'),
  );

  $form['document_library_search_label_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Search Form Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_searching"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_search_label_settings']['document_library_search_legend'] = array(
    '#title'            => t('Search Documents Label'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_search_legend', t('Search Documents')),
    '#max_length'       => 255,
    '#description'      => t('This is the heading text for the search option.'),
  );

  $form['document_library_search_label_settings']['document_library_search_label'] = array(
    '#title'            => t('Search Box Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_search_label', t('SEARCH')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show in the search box.'),
  );

  $form['document_library_search_label_settings']['document_library_search_button_label'] = array(
    '#title'            => t('Search Button Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_search_button_label', t('GO')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show on the submit button.'),
  );

  // Filter Form Settings.
  $form['document_library_filter_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Filter Form Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_searching"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_filter_settings']['document_library_allow_filtering'] = array(
    '#title'            => t('Allow Filtering Documents'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_allow_filtering', 1),
    '#description'      => t('Determines whether or not the filter form is displayed at the top of the document library.'),
  );

  // Filter Form Settings - Labels.
  $form['document_library_filter_settings']['document_library_filter_label_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Filter Form Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_filtering"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_filter_settings']['document_library_filter_label_settings']['document_library_filter_legend'] = array(
    '#title'            => t('Filter Documents Label'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_filter_legend', t('Filter Documents')),
    '#max_length'       => 255,
    '#description'      => t('This is the heading text for the filtering options.'),
  );

  $form['document_library_filter_settings']['document_library_filter_label_settings']['document_library_search_options_label'] = array(
    '#title'            => t('Show Search Options Link Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_search_options_label', t('Show advanced search options')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show next to the search button to open the filtering options.'),
  );

  $form['document_library_filter_settings']['document_library_filter_label_settings']['document_library_search_options_open_label'] = array(
    '#title'            => t('Hide Search Options Link Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_search_options_open_label', t('Hide advanced search options')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show next to the search button to close the filtering options.'),
  );

  $form['document_library_filter_settings']['document_library_filter_label_settings']['document_library_filter_reset_label'] = array(
    '#title'            => t('Filter Reset Button Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_filter_reset_label', t('Reset')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show on the reset button.'),
  );

  $form['document_library_filter_settings']['document_library_filter_label_settings']['document_library_filter_button_label'] = array(
    '#title'            => t('Filter Submit Button Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_filter_button_label', t('Get Results')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show on the submit button.'),
  );

  // Filter Form Settings.
  $form['document_library_filter_settings']['document_library_filter_options_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Filter Form Options Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_filtering"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $field_instances = field_info_instances('node', 'document_library_document');

  foreach ($field_instances as $field_name => $field_instance) {
    $field_info = field_info_field($field_name);

    if ($field_info['type'] == 'taxonomy_term_reference') {
      $var          = 'document_library_' . $field_name . '_filter';
      $var_use      = $var . '_use';
      $var_label    = $var . '_label';
      $var_settings = $var_label . '_settings';

      $form['document_library_filter_settings']['document_library_filter_options_settings'][$var_use] = array(
        '#title'            => t('Use @label Filter', array('@label' => $field_instance['label'])),
        '#type'             => 'checkbox',
        '#default_value'    => variable_get($var_use, 1),
        '#description'      => t('Determines whether or not to include these options in the filter form. Taxonomy options can be !url.', array('!url' => l(t('configured here'), 'admin/structure/taxonomy'))),
      );

      $form['document_library_filter_settings']['document_library_filter_options_settings'][$var_settings] = array(
        '#type'             => 'fieldset',
        '#title'            => t('@label Filter Form Label Settings', array('@label' => $field_instance['label'])),
        '#states'           => array(
          'invisible' => array(
            ':input[name="' . $var_use . '"]' => array(
              'checked' => FALSE,
            ),
          ),
        ),
      );

      $form['document_library_filter_settings']['document_library_filter_options_settings'][$var_settings][$var_label] = array(
        '#title'            => t('Subject Area Filter Label'),
        '#type'             => 'textfield',
        '#default_value'    => variable_get($var_label, $field_instance['label']),
        '#max_length'       => 255,
        '#description'      => t('This is the sub-heading text for this filtering option.'),
      );
    }
  }

  // Sort Form Settings.
  $form['document_library_sort_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Sort Settings'),
  );

  $form['document_library_sort_settings']['document_library_allow_sorting'] = array(
    '#title'            => t('Allow Sorting Documents'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_allow_sorting', 1),
    '#description'      => t('Determines whether or not the sort form is displayed at the top of the document library.'),
  );

  $form['document_library_sort_settings']['document_library_sort_legend'] = array(
    '#title'            => t('Sort Documents Label'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_sort_legend', t('Sort Documents')),
    '#max_length'       => 255,
    '#description'      => t('This is the heading text for the sort option.'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_sorting"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $date_fields = array('0' => '-- None --');
  foreach ($field_instances as $field_name => $field_instance) {
    $field_info = field_info_field($field_name);
    if ($field_info['type'] == 'datetime') {
      $date_fields[$field_info['field_name']] = $field_instance['label'];
    }
  }

  $form['document_library_sort_settings']['document_library_default_sort_field'] = array(
    '#type'             => 'select',
    '#title'            => t('Sort Field'),
    '#default_value'    => variable_get('document_library_default_sort_field', 0),
    '#options'          => $date_fields,
    '#description'      => t('This setting controls which field the sort feature will compare on. If this setting is set to "None" will also diable the Sort form.'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_sorting"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_sort_settings']['document_library_sort_label_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Sort Form Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_sorting"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_sort_settings']['document_library_sort_label_settings']['document_library_sorting_label'] = array(
    '#title'            => t('Sorting Box Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_sorting_label', t('Sort by')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show in the sorting box.'),
  );

  $form['document_library_sort_settings']['document_library_sort_label_settings']['document_library_sort_button_label'] = array(
    '#title'            => t('Sort Button Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_sort_button_label', t('GO')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show on the submit button.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Sort Form Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_sorting"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_use_title_asc_sort'] = array(
    '#title'            => t('Use "Sort by Title: A-Z" sort option'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_use_title_asc_sort', 1),
    '#description'      => t('Determines whether or not to include this sort option in the sort form.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_title_asc_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Sort Form Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_use_title_asc_sort"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_title_asc_settings']['document_library_sorting_title_asc_label'] = array(
    '#title'            => t('Sorting Box Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_sorting_title_asc_label', t('Title: A-Z')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show in the sorting box.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_use_title_desc_sort'] = array(
    '#title'            => t('Use "Sort by Title: Z-A" sort option'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_use_title_desc_sort', 1),
    '#description'      => t('Determines whether or not to include this sort option in the sort form.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_title_desc_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Sort Form Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_use_title_desc_sort"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_title_desc_settings']['document_library_sorting_title_desc_label'] = array(
    '#title'            => t('Sorting Box Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_sorting_title_desc_label', t('Title: Z-A')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show in the sorting box.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_use_date_asc_sort'] = array(
    '#title'            => t('Use "Sort by date: newest to oldest" sort option'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_use_date_asc_sort', 1),
    '#description'      => t('Determines whether or not to include this sort option in the sort form.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_date_asc_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Sort Form Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_use_date_asc_sort"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_date_asc_settings']['document_library_sorting_date_asc_label'] = array(
    '#title'            => t('Sorting Box Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_sorting_date_asc_label', t('Date: newest to oldest')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show in the sorting box.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_use_date_desc_sort'] = array(
    '#title'            => t('Use "Sort by date: oldest to newest" sort option'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_use_date_desc_sort', 1),
    '#description'      => t('Determines whether or not to include this sort option in the sort form.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_date_desc_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Sort Form Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_use_date_desc_sort"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_js_submit'] = array(
    '#title'            => t('Autosubmit via Javascript'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_sort_js_submit', 1),
    '#description'      => t('Determines whether or not changes on the sort select are submitted automatically or a button press is required. This also determines whether a submit button is displayed or not.'),
  );

  $form['document_library_sort_settings']['document_library_sort_options_settings']['document_library_sort_date_desc_settings']['document_library_sorting_date_desc_label'] = array(
    '#title'            => t('Sorting Box Text'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_sorting_date_desc_label', t('Date: oldest to newest')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show in the sorting box.'),
  );

  $form['document_library_sort_settings']['document_library_default_sort_order'] = array(
    '#type'             => 'select',
    '#title'            => t('Default Sort Order'),
    '#default_value'    => variable_get('document_library_default_sort_order', 1),
    '#options'          => array(
      '1' => t('Sort by title: A-Z'),
      '2' => t('Sort by title: Z-A'),
      '3' => t('Sort by date: newest to oldest'),
      '4' => t('Sort by date: oldest to newest'),
    ),
    '#description'      => t('This setting controls which sort order will be used by default.'),
  );

  // Sort Form Settings.
  $form['document_library_link_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Permalink Settings'),
  );

  $form['document_library_link_settings']['document_library_show_link'] = array(
    '#title'            => t('Show Permalink'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_show_link', 1),
    '#description'      => t('Determines whether or not the permalink is shown in the search bar (only if a query or filter options exist).'),
  );

  $form['document_library_link_settings']['document_library_link_label_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Permalink Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_show_link"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_link_settings']['document_library_link_label_settings']['document_library_show_link_title_label'] = array(
    '#title'            => t('Permalink Link Title'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_show_link_title_label', t('Copy this link to share your filtered documents with others.')),
    '#max_length'       => 255,
    '#description'      => t('This is the link title that users will see if hovering over the permalink.'),
  );

  $form['document_library_link_settings']['document_library_link_label_settings']['document_library_show_link_label'] = array(
    '#title'            => t('Permalink Link Label'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_show_link_label', t('Permalink')),
    '#max_length'       => 255,
    '#description'      => t('This is the link label.'),
  );

  // Document Library Folder Block Settings.
  $form['document_library_folder_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Document Library Folder Block Settings'),
  );

  $form['document_library_folder_settings']['document_library_allow_folders'] = array(
    '#title'            => t('Create a Document Library Folder Block'),
    '#type'             => 'checkbox',
    '#default_value'    => variable_get('document_library_allow_folders', 1),
    '#description'      => t('Determines whether or not to create a block that uses a taxonomy vocabulary as a folder menu.'),
  );

  $form['document_library_folder_settings']['document_library_folder_label_settings'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Document Library Folder Label Settings'),
    '#states'           => array(
      'invisible' => array(
        ':input[name="document_library_allow_folders"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['document_library_folder_settings']['document_library_folder_label_settings']['document_library_folder_label'] = array(
    '#title'            => t('Document Library Folder Label'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_folder_label', t('Documents')),
    '#max_length'       => 255,
    '#description'      => t('This is the main folder label. Enter &lt;none&gt; to hide.'),
  );

  $form['document_library_folder_settings']['document_library_folder_label_settings']['document_library_default_folder_list'] = array(
    '#title'            => t('Document Library Folder List'),
    '#type'             => 'select',
    '#default_value'    => variable_get('document_library_default_folder_list', 'library_document_category'),
    '#description'      => t('This is the taxonomy list that will be used for the folder structure'),
    '#options'          => _document_library_get_vocabularies_for_folders(),
  );

  $form['document_library_no_data_message'] = array(
    '#title'            => t('No Results Message'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_no_data_message', t('There are currently no documents in the library that match your search criteria.')),
    '#max_length'       => 255,
    '#description'      => t('This is the text that will show if there are not results returned.'),
  );

  $form['document_library_per_page'] = array(
    '#title'            => t('Number of Results Per Page'),
    '#type'             => 'textfield',
    '#default_value'    => variable_get('document_library_per_page', 10),
    '#max_length'       => 255,
    '#description'      => t('This is the number of results that will display per page. Specify 0 to disable paging.'),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  return system_settings_form($form);
}
