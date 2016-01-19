CRM Core Donation for Drupal 7.x
--------------------------------
CRM Core Donation is a tool for managing fundraising in a Drupal website.
It extends CRM Core by adding the ability to process online donations and
track donation activity in useful ways from within your Drupal website.

With it, you get access to these basic features:

- Build online donation forms for processing donations, both online and
  offline.

- Create multiple online donation pages, which display forms for accepting
  online donations.

- Customize the appearance of online donation pages using other tools from
  Drupal, including photos, videos, audio, twitter feeds, and all sorts of
  other stuff.

- Track offline donations and process offline payments to provide a complete
  donor management solution.

- Send personalized email messages to donors upon completion of transactions.

- Select payment processors to use for transactions on a page-by-page basis.

- Generate detailed reports on donation activity that includes information
  about sources, individual page performance, overall fundraising activities,
  donor details, LYBUNT, SYBUNT and more.

In addition, you have access to additional features through the underlying
CRM Core platform:

- Contact record management, and the ability to get a 360 degree view of
  a donor's actions from within your site.

- A drag and drop form builder for creating online donation forms,
  controlling the workflow surrounding payment processing, and securing
  access to individual forms.

- Tight integration with user accounts, allowing you to customize content
  based on someone's donation history.

- Contact matching, de-duplication and other basic features for managing
  information about people.

CRM Core Donation is built to work with several other modules within
Drupal, including Drupal Commerce (for processing payments) and Rules
(for sending messages to donors). Front-facing public donation pages can easily
be extended using any other Drupal modules to provide a comprehensive user
experience.

Requirements
------------
* CRM Core Contact
* CRM Core Activity
* CRM Core Profile
* CRM Core Commerce Items
* Commerce
* Features

Installation
------------
1) Download CRM Core Donation to the modules directory of your website.

2) Enable the module from the features page under admin/structure/features.

3) Create at least one product in Drupal Commerce you plan to use for
   processing donations at admin/commerce/products/add/product.

4) Configure at least one payment processor in Drupal Commerce. Payment
   processors can be found at admin/commerce/config/payment-methods.

5) (Optional) Goto admin/structure/crm-core/contact-types and add fields
   to your contact types. Most likely, you will want to collect information
   about individuals.

6) (Optional) Configure the matching rules for CRM Core in order to ensure
   the system does not create duplicate records each time a new donation
   is entered. Matching engines can be turned on at admin/config/crm-core/match
   and there is link to configure the rules for each contact type
   on that page.

How to Use This Module
----------------------
See the README-usage.txt file (contained with this module) for instructions
on how to work with offline and online donation pages.

This file deals with the specific ways to work with donation forms, donation
pages, and reports. It provides a lot of information about how to work with
the tools included in this module, details about what to expect from the reports,
and some information about best practices when extending this feature.

Components
----------
CRM Core Donation was built with the features module and includes a number of
components that allow it to work.

1) Donation Page (node)

A content type used to store online donation pages. It uses the following fields
to aid in the donation process:

- Recommended Donation Amounts: used to generate buttons in CRM Core Profile
  for providing users with recommended donation amounts.

2) Donation (CRM Core Activity)

An activity type used to store details about online donations. It uses the
following fields for storing details about donations:

- Receive Date: the date a donation was actually received. All reports generate
  information using this field.

- Amount: the total amount of a donation. All reports generate information
  using this field.

- Source: where a donation came from. The default values can be modified at
  admin/structure/crm-core/activity-types/manage/cmcd_donation/fields/field_cmcd_source.

- Payment Type: used to indicate what type of payment was received (i.e. check,
  credit card, cash, bitcoins, etc). The specific values for this field can be
  modified at
  admin/structure/crm-core/activity-types/manage/cmcd_donation/fields/field_cmcd_payment_type.

- Donation Page: used to store the online donation page used to capture this donation.
  This field should only be used to store information about Donation Pages in Drupal,
  and not for storing other URLs.

- Order: the commerce order used to process a payment.

3) Donation Thank You Email (rules) - a rule used to send thank you emails to
   donors upon receiving a donation. The content in the default email can be edited
   at admin/config/workflow/rules/components/manage/rules_cmcd_thank_you_message.


Maintainers
-----------

- techsoldaten - http://drupal.org/user/16339
- RoSk0 - https://drupal.org/user/325151

Development sponsored by Trellon.



