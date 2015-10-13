<?php

/**
 * @file
 * Contains an implementation of vCard items based on fields on a row plugin.
 */

/**
 * Renders a vCard item based on fields.
 */
class views_vcards_plugin_row_vcard extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();
    $options['name_email']['first'] = array('default' => '');
    $options['name_email']['middle'] = array('default' => '');
    $options['name_email']['last'] = array('default' => '');
    $options['name_email']['full'] = array('default' => '');
    $options['name_email']['title'] = array('default' => '');
    $options['name_email']['email'] = array('default' => '');
    $options['name_email']['email2'] = array('default' => '');
    $options['name_email']['email3'] = array('default' => '');
    $options['home']['home_address'] = array('default' => '');
    $options['home']['home_city'] = array('default' => '');
    $options['home']['home_state'] = array('default' => '');
    $options['home']['home_zip'] = array('default' => '');
    $options['home']['home_country'] = array('default' => '');
    $options['home']['home_phone'] = array('default' => '');
    $options['home']['home_cellphone'] = array('default' => '');
    $options['home']['home_website'] = array('default' => '');
    $options['work']['work_title'] = array('default' => '');
    $options['work']['work_company'] = array('default' => '');
    $options['work']['work_address'] = array('default' => '');
    $options['work']['work_city'] = array('default' => '');
    $options['work']['work_state'] = array('default' => '');
    $options['work']['work_zip'] = array('default' => '');
    $options['work']['work_country'] = array('default' => '');
    $options['work']['work_phone'] = array('default' => '');
    $options['work']['work_fax'] = array('default' => '');
    $options['work']['work_website'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $initial_labels = array('' => t('- None -'));
    $view_fields_labels = $this->display->handler->get_field_labels();
    $view_fields_labels = array_merge($initial_labels, $view_fields_labels);

    $form['name_email'] = array(
      '#type' => 'fieldset',
      '#title' => t('Name and E-mail'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['name_email']['first'] = array(
      '#type' => 'select',
      '#title' => t('First name'),
      '#description' => t('The field that is going to be used as the first name for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['name_email']['first'],
      '#required' => TRUE,
    );

    $form['name_email']['middle'] = array(
      '#type' => 'select',
      '#title' => t('Middle name'),
      '#description' => t('The field that is going to be used as the middle name for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['name_email']['middle'],
      '#required' => FALSE,
    );

    $form['name_email']['last'] = array(
      '#type' => 'select',
      '#title' => t('Last name'),
      '#description' => t('The field that is going to be used as the last name for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['name_email']['last'],
      '#required' => TRUE,
    );

    $form['name_email']['full'] = array(
      '#type' => 'select',
      '#title' => t('Full name'),
      '#description' => t('The field that is going to be used as the full name for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['name_email']['full'],
      '#required' => TRUE,
    );

    $form['name_email']['title'] = array(
      '#type' => 'select',
      '#title' => t('Title'),
      '#description' => t('The field that is going to be used as the title for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['name_email']['title'],
      '#required' => FALSE,
    );

    $form['name_email']['email'] = array(
      '#type' => 'select',
      '#title' => t('Primary E-mail'),
      '#description' => t('The field that is going to be used as the primary e-mail address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['name_email']['email'],
      '#required' => TRUE,
    );

    $form['name_email']['email2'] = array(
      '#type' => 'select',
      '#title' => t('Secondary E-mail'),
      '#description' => t('The field that is going to be used as the secondary e-mail address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['name_email']['email2'],
      '#required' => FALSE,
    );

    $form['name_email']['email3'] = array(
      '#type' => 'select',
      '#description' => t('The field that is going to be used as the tertiary e-maila address for each vCard.'),
      '#title' => t('Tertiary E-mail'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['name_email']['email3'],
      '#required' => FALSE,
    );

    $form['home'] = array(
      '#type' => 'fieldset',
      '#title' => t('Home'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['home']['home_address'] = array(
      '#type' => 'select',
      '#title' => t('Address'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['home']['home_address'],
      '#required' => FALSE,
    );

    $form['home']['home_city'] = array(
      '#type' => 'select',
      '#title' => t('City'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['home']['home_city'],
      '#required' => FALSE,
    );

    $form['home']['home_state'] = array(
      '#type' => 'select',
      '#title' => t('State/Province'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['home']['home_state'],
      '#required' => FALSE,
    );

    $form['home']['home_zip'] = array(
      '#type' => 'select',
      '#title' => t('Zip'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['home']['home_zip'],
      '#required' => FALSE,
    );

    $form['home']['home_country'] = array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['home']['home_country'],
      '#required' => FALSE,
    );

    $form['home']['home_phone'] = array(
      '#type' => 'select',
      '#title' => t('Phone'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['home']['home_phone'],
      '#required' => FALSE,
    );

    $form['home']['home_cellphone'] = array(
      '#type' => 'select',
      '#title' => t('Cellphone'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['home']['home_cellphone'],
      '#required' => FALSE,
    );

    $form['home']['home_website'] = array(
      '#type' => 'select',
      '#title' => t('Website'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['home']['home_website'],
      '#required' => FALSE,
    );

    $form['work'] = array(
      '#type' => 'fieldset',
      '#title' => t('Work'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['work']['work_title'] = array(
      '#type' => 'select',
      '#title' => t('Title'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_title'],
      '#required' => FALSE,
    );

    $form['work']['work_company'] = array(
      '#type' => 'select',
      '#title' => t('Company'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_company'],
      '#required' => FALSE,
    );

    $form['work']['work_address'] = array(
      '#type' => 'select',
      '#title' => t('Address'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_address'],
      '#required' => FALSE,
    );

    $form['work']['work_city'] = array(
      '#type' => 'select',
      '#title' => t('City'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_city'],
      '#required' => FALSE,
    );

    $form['work']['work_state'] = array(
      '#type' => 'select',
      '#title' => t('State/Province'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_state'],
      '#required' => FALSE,
    );

    $form['work']['work_zip'] = array(
      '#type' => 'select',
      '#title' => t('Zip'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_zip'],
      '#required' => FALSE,
    );

    $form['work']['work_country'] = array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_country'],
      '#required' => FALSE,
    );

    $form['work']['work_phone'] = array(
      '#type' => 'select',
      '#title' => t('Phone'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_phone'],
      '#required' => FALSE,
    );

    $form['work']['work_fax'] = array(
      '#type' => 'select',
      '#title' => t('Fax'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_fax'],
      '#required' => FALSE,
    );

    $form['work']['work_website'] = array(
      '#type' => 'select',
      '#title' => t('Website'),
      '#description' => t('The field that is going to be used as the address for each vCard.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['work']['work_website'],
      '#required' => FALSE,
    );
  }

  function validate() {
    $errors = parent::validate();
    $required_options = array(
      'name_email' => array('full'),
      'home' => array(),
      'work' => array(),
    );

    foreach ($required_options as $group => $options) {
      foreach ($options as $option) {
        if (empty($this->options[$group][$option])) {
          $errors[] = t('Row style plugin requires specifying which views fields to use for RSS item.');
          break;
        }
      }
    }
    return $errors;
  }


  function render($row) {
    static $row_index;
    if (!isset($row_index)) {
      $row_index = 0;
    }

    // Create the vCard item object.
    $item = new stdClass();
    $item->first_name     = $this->get_field($row_index, $this->options['name_email']['first']);
    $item->middle_name    = $this->get_field($row_index, $this->options['name_email']['middle']);
    $item->last_name      = $this->get_field($row_index, $this->options['name_email']['last']);
    $item->full_name      = $this->get_field($row_index, $this->options['name_email']['full']);
    $item->title          = $this->get_field($row_index, $this->options['name_email']['title']);
    $item->email          = $this->get_field($row_index, $this->options['name_email']['email']);
    $item->email2         = $this->get_field($row_index, $this->options['name_email']['email2']);
    $item->email3         = $this->get_field($row_index, $this->options['name_email']['email3']);
    $item->home_address   = $this->get_field($row_index, $this->options['home']['home_address']);
    $item->home_city      = $this->get_field($row_index, $this->options['home']['home_city']);
    $item->home_state     = $this->get_field($row_index, $this->options['home']['home_state']);
    $item->home_zip       = $this->get_field($row_index, $this->options['home']['home_zip']);
    $item->home_country   = $this->get_field($row_index, $this->options['home']['home_country']);
    $item->home_phone     = $this->get_field($row_index, $this->options['home']['home_phone']);
    $item->home_cellphone = $this->get_field($row_index, $this->options['home']['home_cellphone']);
    $item->home_website   = $this->get_field($row_index, $this->options['home']['home_website']);
    $item->work_title     = $this->get_field($row_index, $this->options['work']['work_title']);
    $item->work_company   = $this->get_field($row_index, $this->options['work']['work_company']);
    $item->work_address   = $this->get_field($row_index, $this->options['work']['work_address']);
    $item->work_city      = $this->get_field($row_index, $this->options['work']['work_city']);
    $item->work_state     = $this->get_field($row_index, $this->options['work']['work_state']);
    $item->work_zip       = $this->get_field($row_index, $this->options['work']['work_zip']);
    $item->work_country   = $this->get_field($row_index, $this->options['work']['work_country']);
    $item->work_phone     = $this->get_field($row_index, $this->options['work']['work_phone']);
    $item->work_fax       = $this->get_field($row_index, $this->options['work']['work_fax']);
    $item->work_website   = $this->get_field($row_index, $this->options['work']['work_website']);

    $row_index++;

    return theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'row' => $item,
        'field_alias' => isset($this->field_alias) ? $this->field_alias : '',
      )
    );
  }


  function get_field($index, $field_id) {
    // If there is no style plugin return an empty string.
    if (empty($this->view->style_plugin) || !is_object($this->view->style_plugin) || empty($field_id)) {
      return '';
    }
    return $this->view->style_plugin->get_field($index, $field_id);
  }
}
