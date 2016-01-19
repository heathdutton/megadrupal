Description
===========

Restricts allowed values in a date field exposed in a line item. The source
data for restrictions is picked from fields defined in the product display.

Installation
============

 * efq_extra_fields needs the patch in https://drupal.org/node/1857240

Configuration and Usage
=======================

 * Create a product display entity (tipically a content type).
 * Add fields to provide minimum date, maximum date and/or allowed values
   restrictions.
   - Minimum and maximum dates may be an absolute fixed value or
     a relative value. Create a date field for a fixed value, or an interval
     field for relative.
   - Allowed values are provided by a multivalued date field.
 * Enable Commerce Line Item UI module and add a date field to the line item
   at Store > Configuration > Line Item types. Configure your settings under
   "More settings and values" > Restrictions.
 * Mark the option "Include this field on Add to Cart forms for line items
   of this type."
