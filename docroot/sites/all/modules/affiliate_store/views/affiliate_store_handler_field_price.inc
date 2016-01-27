<?php

/**
 * @file
 * Views field handler for price.
 */

/**
 * Field handler for price.
 *
 * Make sure price value of 0.0... is not output when options hide if empty and
 * count 0 as empty are selected. The base view handler for numeric field does
 * not handles float zero properly.
 *
 * @ingroup views_field_handlers
 */
class affiliate_store_handler_field_price extends views_handler_field_numeric {
  /**
   * Render a price value.
   */
  function render($values) {
    $value = $this->get_value($values);
    if (!empty($this->options['set_precision'])) {
      $value = number_format(
        $value,
        $this->options['precision'],
        $this->options['decimal'],
        $this->options['separator']
      );
    }
    else {
      $remainder = abs($value) - intval(abs($value));
      $value = $value > 0 ? floor($value) : ceil($value);
      $value = number_format($value, 0, '', $this->options['separator']);
      if ($remainder) {
        // The substr may not be locale safe.
        $value .= $this->options['decimal'] . substr($remainder, 2);
      }
    }

    // Check to see if hiding should happen before adding prefix and suffix.
    // Make sure price value of 0.0... is not output when specified so.
    $price = (float) $value;
    if ($this->options['hide_empty']
      && !$price
      && (($price !== 0.0) || $this->options['empty_zero'])
    ) {
      return '';
    }

    // Should we format as a plural.
    if (!empty($this->options['format_plural'])) {
      $value = format_plural(
        $value,
        $this->options['format_plural_singular'],
        $this->options['format_plural_plural']
      );
    }

    return $this->sanitize_value($this->options['prefix'], 'xss')
      . $this->sanitize_value($value)
      . $this->sanitize_value($this->options['suffix'], 'xss');
  }
}
