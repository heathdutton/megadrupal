<?php
// vim: set ts=2 sw=2 sts=2 et:

/**
 * @file
 * Common handler
 */

/**
 * Since the integration code highly depends on the LiteCommerce version being
 * integrated, this Drupal module only forwards Drupal hooks into a
 * LiteCommerce doing the actual work. That way shop owners can upgrade both
 * LiteCommerce and the integration code at once via the LiteCommerce automatic
 * upgrade function within the shop back end.
 *
 * The back side of this is the use of complex wrapper functions for Drupal
 * hooks. To prevent possible issues, get rid of global variables and make the
 * interface easier to understand the wrapper logic is localized in PHP classes
 * with private and protected class methods and static fields.
 */

/**
 * Handler
 *
 * TODO: Check if this class is needed in further
 */
abstract class LCConnector_Handler extends LCConnector_Abstract {
}
