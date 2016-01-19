<?php

/**
 * @file
 * Builds placeholder replacement tokens amoCRM data.
 */

/**
 * Implements hook_token_info().
 */
function amocrm_token_info() {
  $lead_type = 'amocrm_' . AMOCRM_TYPE_LEAD;
  $contact_type = 'amocrm_' . AMOCRM_TYPE_CONTACT;
  $company_type = 'amocrm_' . AMOCRM_TYPE_COMPANY;

  // Define token types.
  $types[$lead_type] = array(
    'name' => t('amoCRM'),
    'description' => t("Lead's tokens available from amoCRM."),
  );
  $types[$contact_type] = array(
    'name' => t('amoCRM'),
    'description' => t("Contact's tokens available from amoCRM."),
  );
  $types[$company_type] = array(
    'name' => t('amoCRM'),
    'description' => t("Company's tokens available from amoCRM."),
  );

  // Lead's tokens.
  $leads['id'] = array(
    'name' => t('ID'),
    'description' => t('ID of lead.'),
  );
  $leads['name'] = array(
    'name' => t('Name'),
    'description' => t('Name of lead.'),
  );
  $leads['price'] = array(
    'name' => t('Price'),
    'description' => t('Price of lead.'),
  );
  $leads['changed'] = array(
    'name' => t('Changed'),
    'description' => t('Last modified time.'),
  );
  $leads['created'] = array(
    'name' => t('Created'),
    'description' => t('Creation time.'),
  );
  $leads['custom-{name}'] = array(
    'name' => t('Any custom field'),
    'description' => t('Content of any custom field, example: "[custom-{city}]".'),
  );

  // Contact's tokens.
  $contacts['id'] = array(
    'name' => t('ID'),
    'description' => t('ID of contact.'),
  );
  $contacts['name'] = array(
    'name' => t('Name'),
    'description' => t('Name of contact.'),
  );
  $contacts['changed'] = array(
    'name' => t('Changed'),
    'description' => t('Last modified time.'),
  );
  $contacts['created'] = array(
    'name' => t('Created'),
    'description' => t('Creation time.'),
  );
  $contacts['custom-{name}'] = array(
    'name' => t('Any custom field'),
    'description' => t('Content of any custom field, example: "[custom-{Email}]".'),
  );

  // Company's tokens.
  $companies['id'] = array(
    'name' => t('ID'),
    'description' => t('ID of company.'),
  );
  $companies['name'] = array(
    'name' => t('Name'),
    'description' => t('Name of company.'),
  );
  $companies['changed'] = array(
    'name' => t('Changed'),
    'description' => t('Last modified time.'),
  );
  $companies['created'] = array(
    'name' => t('Created'),
    'description' => t('Creation time.'),
  );
  $companies['custom-{name}'] = array(
    'name' => t('Any custom field'),
    'description' => t('Content of any custom field, example: "[custom-{Email}]".'),
  );

  return array(
    'types' => $types,
    'tokens' => array(
      $lead_type => $leads,
      $contact_type => $contacts,
      $company_type => $companies,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function amocrm_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // Do not handle any other types of tokens.
  if (strpos($type, 'amocrm_') === FALSE) {
    return $replacements;
  }

  $data = $data[$type];
  $data['custom_fields'] = !empty($data['custom_fields'])
    ? $data['custom_fields']
    : array();

  // Lead's tokens.
  if ($type == 'amocrm_' . AMOCRM_TYPE_LEAD) {
    foreach ($tokens as $name => $original) {

      // Proceed with custom fields.
      if (strpos($name, 'custom-{') !== FALSE) {
        $name = explode('-', $name);
        $name = trim($name[1], " \t\n\r\0\x0B{}");

        foreach ($data['custom_fields'] as $cfield) {
          if ($name == $cfield['name']) {

            $values = array();
            foreach ($cfield['values'] as $value) {
              // Date field.
              if (is_string($value)) {
                $values[] = format_date($value);
              }
              // Any other fields.
              else {
                $values[] = $value['value'];
              }
            }

            $replacements[$original] = implode(', ', $values);
            continue 2;
          }
        }

        $replacements[$original] = $original;
        continue;
      }

      // Proceed with default, simple fields.
      switch ($name) {
        case 'id':
        case 'name':
        case 'price':
          $replacements[$original] = $data[$name];
          break;

        case 'changed':
          $replacements[$original] = format_date($data['last_modified']);
          break;

        case 'created':
          $replacements[$original] = format_date($data['date_create']);
          break;
      }
    }
  }
  // Contact's and Company's tokens.
  elseif ($type == 'amocrm_' . AMOCRM_TYPE_CONTACT || $type == 'amocrm_' . AMOCRM_TYPE_COMPANY) {
    foreach ($tokens as $name => $original) {

      // Proceed with custom fields.
      if (strpos($name, 'custom-{') !== FALSE) {
        $name = explode('-', $name);
        $name = trim($name[1], " \t\n\r\0\x0B{}");

        foreach ($data['custom_fields'] as $cfield) {
          if ($name == $cfield['name']) {

            $values = array();
            foreach ($cfield['values'] as $value) {
              // Date field.
              if (is_string($value)) {
                $values[] = format_date($value);
              }
              // Any other fields.
              else {
                $values[] = $value['value'];
              }
            }

            $replacements[$original] = implode(', ', $values);
            continue 2;
          }
        }

        $replacements[$original] = $original;
        continue;
      }

      // Proceed with default, simple fields.
      switch ($name) {
        case 'id':
        case 'name':
          $replacements[$original] = $data[$name];
          break;

        case 'changed':
          $replacements[$original] = format_date($data['last_modified']);
          break;

        case 'created':
          $replacements[$original] = format_date($data['date_create']);
          break;
      }
    }
  }

  return $replacements;
}
