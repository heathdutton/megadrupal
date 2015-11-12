<?php
/**
 * @file
 *  Constants for the "Mixed Fraction" module.
 *
 *  © 2014-2015 RedBottle Design, LLC and Five Star Tool Company.
 *  All rights reserved. This code is licensed under the GNU GPL v2 or later.
 */

/**
 * The machine name of fraction fields.
 *
 * @var string
 */
define('FRMI_FIELD_TYPE_FRACTION', 'fraction');

/**
 * The machine name of mixed fraction widgets.
 *
 * @var string
 */
define('FRMI_WIDGET_TYPE_MIXED_FRACTION', 'fraction_mixed');

/**
 * The machine name of mixed fraction field formatters.
 *
 * @var string
 */
define('FRMI_FORMATTER_TYPE_MIXED_FRACTION', 'fraction_mixed');

/**
 * The name of the setting that specifies for the separator between the whole
 * number and the fraction.
 *
 * @var string
 */
define('FRMI_SETTING_WHOLE_NUMBER_SEPARATOR', 'whole_number_separator');

/**
 * The name of the setting that specifies for the separator between the
 * numerator and denominator.
 *
 * @var string
 */
define('FRMI_SETTING_FRACTION_SEPARATOR', 'fraction_separator');

/**
 * Type of mixed fraction which consists of both a whole number and fraction.
 */
define('FRMI_FRACTION_TYPE_WHOLE_AND_FRACTION', 1);

/**
 * Type of mixed fraction which consists only of a whole number.
 */
define('FRMI_FRACTION_TYPE_WHOLE_ONLY', 2);

/**
 * Type of mixed fraction which consists only of a fraction (i.e. not really
 * mixed).
 */
define('FRMI_FRACTION_TYPE_FRACTION_ONLY', 3);