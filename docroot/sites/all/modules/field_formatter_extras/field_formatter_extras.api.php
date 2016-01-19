<?php

/**
 * @file
 * Hooks provided by the Field Formatter Extras module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the form elements for a formatter's settings.
 *
 * @param $wrapper_html_tags
 *   An array of HTML tags allowed to be used as field wrapper.
 * @param $context
 *   An array of additional context for the settings form.
 *   - field: The field structure being configured.
 *   - instance: The instance structure being configured.
 *   - view_mode: The view mode being configured.
 *   - form: The (entire) configuration form array, which will usually have no
 *     use here.
 *   - form_state: The form state of the (entire) configuration form.
 *
 * @see hook_field_formatter_settings()
 */
function hook_field_formatter_extras_wrapper_html_tags(array &$wrapper_html_tags, array $context) {

}

/**
 * @} End of "addtogroup hooks".
 */
