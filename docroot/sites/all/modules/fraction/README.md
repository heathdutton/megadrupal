FRACTION
========

A Fraction class and field type for Drupal.

OVERVIEW
--------

This module provides two things:

1. A Fraction PHP class for representing and working with fractions.
2. A Fraction field with 2 widgets and formatters.

FRACTION CLASS
--------------

Usage:

    $fraction = fraction(1, 2);

Get the numerator and denominator (as strings):

    $numerator = $fraction->getNumerator();
    $denominator = $fraction->getDenominator();

Convert the fraction to a decimal with precision 2:

    $precision = 2;
    $decimal =  $fraction->toDecimal($precision);

Multiply fractions:

    $fraction1 = fraction(2, 3);
    $fraction2 = fraction(1, 2);
    $fraction1->multiply($fraction2);

Operations are performed using the BCMath PHP extension, when available. Otherwise, normal PHP float operations are used.

FRACTION FIELD
--------------

The Fraction field allows you to easily add fraction-based decimal storage to your entities using Drupal's Field API.

### Widgets

(for editing the field)

**Fraction** - Two separate text fields for specifying the numerator and denominator separately. This is the best way to
store very specific fractions, ie: 1/3.

**Decimal** - A single textfield that accepts a decimal representation of a fraction (ie: 0.33333). This is useful when
working with prices. Note that the decimal is converted into a fraction with a base-10 denominator before saving (ie:
0.33 becomes 33/100).

### Formatters

(for viewing the field)

**Fraction** - Simply displays the fraction's numerator and denominator separated with a slash, ie: 1/3. The separator
can be configured per-field.

**Decimal** - Displays the fraction as a decimal with a fixed precision. For example, the fraction 1/3 can be
represented with a precision of 5 as 0.33333.

AUTOMATIC PRECISION
-------------------

Fraction can automatically determine the precision of a fraction, as long as the denominator is base 10.

Automatic precision can be used in the fraction class's toDecimal() method by setting the second parameter to TRUE:

    $precision = 2;
    $auto_precision = TRUE;
    $decimal =  $fraction->toDecimal($precision, TRUE);

In the above example, if the fraction's denominator is not base 10, then the precision defined in the first parameter
will be used. If the denominator is base 10, the precision will be automatically calculated.

The "Decimal" widget and formatter both provide an option to enable automatic precision.

DATABASE STORAGE
----------------

Fractions are stored in the database using two columns: a numerator and a denominator.

The numerator is represented as a signed BIGINT, which allows for a range of values between -9223372036854775808 and
9223372036854775807.

The denominator is represented as an unsigned INT, which allows for a range of values between 0 and 4294967295.

For modules that want to implement their own numerator and denominator columns in a database table, the following
schema can be used as an example (for use in hook_schema()):

    /**
     * Implements hook_schema().
     */
    function mymodule_schema() {
      $schema['mymodule_table'] = array(
        'fields' => array(
    
          ...
    
          'value_numerator' => array(
            'description' => 'Value numerator',
            'type' => 'int',
            'size' => 'big',
            'not null' => TRUE,
            'default' => 0,
          ),
          'value_denominator' => array(
            'description' => 'Value denominator',
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'default' => 1,
          ),
    
          ...
    
        ),
      );
      return $schema;
    }

VIEWS INTEGRATION
-----------------

Views integration is provided by Views itself on behalf of the core Field module. Fraction extends some of the field
handlers to allow sorting and filtering by the decimal equivalent of the fraction value (calculated via formula in the
database query).

For modules that want to implement their own fraction-based database storage (see Database Storage above), Fraction
also provides a general-purpose Views field handler that can be used in hook_views_data(). Using the same example
described in Database Storage above, here is what that would look like:

    /**
     * Implements hook_views_data().
     */
    function mymodule_views_data() {
    
      ...
    
      // Value numerator.
      $data['mymodule_table']['value_numerator'] = array(
        'title' => t('Value numerator'),
        'help' => t('The stored numerator value.'),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      );
    
      // Value denominator.
      $data['mymodule_table']['value_denominator'] = array(
        'title' => t('Value denominator'),
        'help' => t('The stored denominator value.'),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      );
    
      // Create a new decimal column with fraction decimal handlers.
      $fraction_fields = array(
        'numerator' => 'value_numerator',
        'denominator' => 'value_denominator',
      );
      $data['mymodule_table']['value_decimal'] = array(
        'title' => t('Value (decimal)'),
        'help' => t('Decimal equivalent of the value.'),
        'real field' => 'value_numerator',
        'field' => array(
          'handler' => 'fraction_handler_field',
          'additional fields' => $fraction_fields,
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'fraction_handler_sort_decimal',
          'additional fields' => $fraction_fields,
        ),
        'filter' => array(
          'handler' => 'fraction_handler_filter_decimal',
          'additional fields' => $fraction_fields,
        )
      );
    
      return $data;
    }

USING THIS MODULE FOR PRICE STORAGE
-----------------------------------

This module aims to solve an outstanding issue with price storage in Drupal. Storing prices in the database can be
tricky if you don't know what the decimal precision will need to be from the beginning. This is often the case when
multiple currencies need to be supported, or when you need extra flexibility with decimal precision.

[Drupal Commerce](http://drupal.org/project/commerce, for instance, ties precision directly to currency. So if all of
your products are sold with a two-decimal price, but you want to add one with three decimal places, you need to add a
second (duplicate) currency for just those products. There is an outstanding issue in the Commerce queue that is
discussing solutions (one of which is to use a Fraction-based approach): https://www.drupal.org/node/1125706

The alternative to using a decimal-based storage is to use floats. This brings more issues with precision, due to the
way that floating-point numbers are represented in lower-level system storage. The limitations of float precision are
outlined on PHP's website: http://www.php.net/manual/en/language.types.float.php (If you're REALLY interested, here's a
great video that explains floating-point rounding errors: https://www.youtube.com/watch?v=PZRI1IfStY0)

Fraction overcomes these issues by storing the value as a fraction, using two integer fields in the database. Decimal
representation of this fraction is provided in higher-level functions.

The best way to use this module for price storage is to utilize the "Decimal field" widget to collect the price, and the
"Decimal" formatter to display it. This approach hides the fact that there are actually two numbers being stored in the
database: numerator and denominator. This is achieved through a simple decimal-to-fraction conversion when a decimal
value is saved.

When a decimal is converted to a fraction, the numerator and denominator are calculated such that the denominator is a
power of 10. For example, the decimal 13.95 becomes 1395/100 in the database.

This means that for price storage, this module supports up to 9 decimal places of precision (because the maximum
denominator is 4,294,967,295, meaning the highest possible base-10 number that fits into that is 1,000,000,000).
Ultimately, the maximum numerator depends on the size of the denominator. But, using the maximum precision (of 9 decimal
places), the biggest number this can store is 9,223,372,036.854775807.

INSTALLATION
------------

Install as you would normally install a contributed drupal module. See:
http://drupal.org/documentation/install/modules-themes/modules-7 for further
information.

MAINTAINERS
-----------

Current maintainers:
 * Michael Stenta (m.stenta) - https://drupal.org/user/581414
