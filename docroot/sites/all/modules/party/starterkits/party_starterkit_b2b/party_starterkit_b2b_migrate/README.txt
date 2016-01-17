Introduction
------------

This module provides an example of migrating CSV formatted data to Party entities, 
using typical fields required by B2B CRM implementations.

The migration is based on the Drupal migrate module, for more information see:
http://drupal.org/project/migrate.

Two CSV files are required, one for organizations, and one for contacts.  Example 
organization and contact CSV files are included with this module.  Note that the 
two files have the exact same field layout.  This was done to accomodate the situation
where each organization has exactly one contact.  In that case, the two CSV files can
be identical.  

For situations where organizaitions have 0 or many contacts, then two unique CSV files
are created, still using the same field layout.  The CSV contact fields are ignored by the
organization migration, and the CSV organization fields are ignored by the contact migration,
except for org_id.

Requirements
------------

PHP 5.3
Drupal greater than 7.14
Dependencies:
 - Entity API
 - CTools
 - Views
 - Party/Party Hat/Party User/Party Profile/Party Activity
 - Panels
 - Relations
 - Party
 - Party B2B Starter Kit
 - Migrate
 - Migrate Extras
 
Installation
------------

Install and enable the Party B2B Startkit Migrate module as usual.

