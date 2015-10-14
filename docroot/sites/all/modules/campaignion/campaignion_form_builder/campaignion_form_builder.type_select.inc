<?php

/**
 * Invoked by campaignion_form_builder_form_builder_element_types().
 */
function _campaignion_form_builder_form_builder_types_select() {
  require_once drupal_get_path('module', 'webform') . '/components/select.inc';
  $fields = array();

  $select_element = function($component, $extra, $items, $weight) {
    $extra += [
      'aslist' => TRUE,
      'multiple' => FALSE,
      'items' => _webform_select_options_to_text($items),
    ];
    $element = [
      'title' => $component['name'],
      'weight' => $weight,
      'properties' => ['default_value', 'options', 'multiple', 'other_option', 'other_text',],
      'palette_group' => 'supporter_data',
      'unique' => TRUE,
      'default' => _form_builder_webform_default('select', $extra, $component),
    ];
    $element['default']['#form_builder']['element_type'] = 'select';
    return $element;
  };

  // Token default-values are not handled properly in webform yet.
  $fields['salutation'] = $select_element(
    ['name' => t('Salutation'), /*'value' => '%get[p1]'*/],
    ['other_option' => TRUE],
    ['fam' => t('Family'), 'mrs' => t('Mrs'), 'mr' => t('Mr'), 'org' => t('Organisation')],
    -82
  );

  $fields['gender'] = $select_element(
    ['name' => t('Gender'), /*'value' => '%get[p2]'*/],
    [],
    ['f' => t('female'), 'm' => t('male'), 'o' => t('other')],
    -81
  );

  include_once(drupal_get_path('module', 'webform') . '/includes/webform.options.inc');
  $fields['country'] = $select_element(
    ['name' => t('Country'), /*'value' => '%get[p9]'*/],
    [],
    webform_options_countries(NULL, NULL, NULL, NULL),
    -73
  );


  $fields['email_newsletter'] = $select_element(
    ['name' => t('Email Newsletter'), 'value' => 1],
    ['aslist' => FALSE, 'multiple' => TRUE],
    ['1' => t("Yes, I'd like to get email newsletters")],
    -69
  );
  $fields['email_newsletter']['title'] = t('Newsletter');
  $fields['email_newsletter']['default']['#form_builder']['element_type'] = 'checkboxes';

  $fields['direct_mail_newsletter'] = $select_element(
    ['name' => t('Direct Mail Newsletter'), 'value' => 1],
    ['aslist' => FALSE, 'multiple' => TRUE],
    ['1' => t("Yes, I'd like to get direct mail newsletters")],
    -68
  );
  $fields['direct_mail_newsletter']['title'] = t('Snail Mail Opt-In');
  $fields['direct_mail_newsletter']['default']['#form_builder']['element_type'] = 'checkboxes';

  return $fields;
}
