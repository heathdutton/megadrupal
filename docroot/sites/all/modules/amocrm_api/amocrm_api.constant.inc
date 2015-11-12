<?php

/**
 * @file
 * Defined constants for amoCRM API module.
 */

/**
 * Normal text box.
 */
define('AMOCRM_API_FIELD_TYPE_TEXT', 1);

/**
 * Text field to transmit only numbers.
 */
define('AMOCRM_API_FIELD_TYPE_NUMERIC', 2);

/**
 * Field indicates only the presence or absence of a property
 * (for example: "yes" / "no").
 */
define('AMOCRM_API_FIELD_TYPE_CHECKBOX', 3);

/**
 * Field type list to select one element.
 */
define('AMOCRM_API_FIELD_TYPE_SELECT', 4);

/**
 * Field type list to select multiple items in the list.
 */
define('AMOCRM_API_FIELD_TYPE_MULTISELECT', 5);

/**
 * Field data type returns and takes values in the format (Ymd H: i: s).
 */
define('AMOCRM_API_FIELD_TYPE_DATE', 6);

/**
 * Normal text field for entering the URL's.
 */
define('AMOCRM_API_FIELD_TYPE_URL', 7);

/**
 * Textarea field containing a large amount of text.
 */
define('AMOCRM_API_FIELD_TYPE_MULTITEXT', 8);

/**
 * Textarea field containing a large amount of text.
 */
define('AMOCRM_API_FIELD_TYPE_TEXTAREA', 9);

/**
 * Field type switch.
 */
define('AMOCRM_API_FIELD_TYPE_RADIOBUTTON', 10);

/**
 * Short address field.
 */
define('AMOCRM_API_FIELD_TYPE_STREETADDRESS', 11);

/**
 * Address field (in the interface is a set of several fields).
 */
define('AMOCRM_API_FIELD_TYPE_SMART_ADDRESS', 13);

/**
 * Field type the date on which the search is carried out without taking
 * into account the year the value format (Ymd H: i: s)
 */
define('AMOCRM_API_FIELD_TYPE_BIRTHDAY', 14);

/**
 * amoCRM entity type Contact.
 */
define('AMOCRM_API_ENTITY_CONTACT', 1);

/**
 * amoCRM entity type Lead.
 */
define('AMOCRM_API_ENTITY_LEAD', 2);

/**
 * amoCRM entity type Company.
 */
define('AMOCRM_API_ENTITY_COMPANY', 3);
