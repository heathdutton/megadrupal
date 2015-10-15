<?php

/**
 * @file
 * Contain function for uninstalling Auto Product Display
 */

/**
 * The configuration form for auto product display.
 * Contain a dropdown list for choosing the default content type for
 * auto product display creation.
 */
function _commerce_apd_configuration($form, &$form_state) {
  // Create breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t("Home"), "<front>"),
      l(t("Administration"), "admin"),
      l(t("Store"), "admin/commerce"),
      l(t("Configuration"), "admin/commerce/config"),
    )
  );

  // Product and product display binding configuration
  $form["commerce_apd_configuration"]["product_display"] = array(
    "#title" => t("Product display configuration"),
    "#type" => "fieldset",
    "#description" => t("Choose the product display to be used for each product type."),
    "#tree" => TRUE,
  );

  // Select options
  $options = array();

  // Get all the content types which have the product reference field.
  $rs = db_query("SELECT DISTINCT a.bundle, c.name FROM {field_config_instance} a
    INNER JOIN {field_config} b ON a.field_id = b.id
    INNER JOIN {node_type} c ON a.bundle = c.type
    WHERE a.entity_type='node' AND b.type='commerce_product_reference' AND b.module='commerce_product_reference'");
  while ($data = $rs->fetchObject()) {
    $options[$data->bundle] = $data->name;
  }

  // Select the list of product type.
  $rs = db_query("SELECT type, name, description FROM {commerce_product_type}");
  if ($rs->rowCount() > 0) {
    $product_display = variable_get("commerce_apd_product_display", array());

    while ($data = $rs->fetchObject()) {
      // The dropdown list for choosing default product display for each product type.
      $form["commerce_apd_configuration"]["product_display"][$data->type] = array(
        "#title" => $data->name,
        "#description" => $data->description,
        "#type" => "select",
        "#options" => $options,
        "#default_value" => (isset($product_display[$data->type]) ? $product_display[$data->type] : ''),
      );
    }
  }

  // The option to set if product display creation form should be displayed automatically.
  $form["commerce_apd_configuration"]["show_product_display_form"] = array(
    "#title" => t("Show product display form automatically"),
    "#description" => t("Choose 'Yes' to automatically display the default product display form and also hide the checkbox to display or hide product display form."),
    "#type" => "select",
    "#options" => array(1 => t('Yes'), 0 => t('No')),
    "#required"  => TRUE,
    "#default_value" => variable_get("commerce_apd_show_product_display_form", 0),
  );

  // The option to set if product display creation form should be displayed automatically.
  $form["commerce_apd_configuration"]["auto_delete_display"] = array(
    "#title" => t("Delete product display automatically after product deletion"),
    "#description" => t("Choose 'Yes' to automatically delete the product display after the product deletion."),
    "#type" => "select",
    "#options" => array(1 => t('Yes'), 0 => t('No')),
    "#required"  => TRUE,
    "#default_value" => variable_get("commerce_apd_auto_delete_display", 0),
  );

  // Form submit button.
  $form["commerce_apd_configuration"]["submit"] = array(
    "#value" => t("Submit"),
    "#type" => "submit",
    "#submit" => array("_commerce_apd_configuration_submit"),
  );

  return $form;
}

/**
 * Handle the configuration form submission.
 */
function _commerce_apd_configuration_submit($form, &$form_state) {
  $product_display = array();
  while (list($k, $v) = each($form_state["values"]["product_display"])) {
    $product_display[$k] = $v;
  }

  // Saved the selected content type to variable.
  variable_set("commerce_apd_product_display", $product_display);
  variable_set("commerce_apd_show_product_display_form", $form_state["values"]["show_product_display_form"]);
  variable_set("commerce_apd_auto_delete_display", $form_state["values"]["auto_delete_display"]);

  // Display the message to user.
  drupal_set_message(t("The configuration option has been saved."));
}
