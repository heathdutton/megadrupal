CRM Core Donation for Drupal 7.x
Notes on Usage
--------------

Table of Contents

1) Overview
2) Basic Configuration
3) Creating a front-facing online donation page
4) Working with online donation pages
5) Creating a page for capturing offline donations
6) Reports
7) Customizing / extending the components
8) Customizing / extending the reports

1) Overview
-----------
CRM Core is a set of modules for Drupal. It was built to provide novel
approaches for working and interacting with information about people,
activities and relationships in your Drupal website. It was also built
as a platform developers can use to build small, useful applications
that provide other functionality going beyond what the core modules
provide.

CRM Core Donation is one of these applications. It was built to extend
CRM Core and make it a platform for managing fundraising activity for
organizations.

Note the use of the word 'platform.' If you are looking for a tool that
allows you to create donation forms aimed at a public audience, this is
a great tool for that.

But CRM Core Donation also does a lot more than just process payments,
it's a system for managing the fundraising and development activities of
an organization. It comes with features designed for this purpose, along
with lots of ways to extend the way it works to suit your specific needs.

There are a few things to be aware of when working with CRM Core Donation.

- First off, a donation form is a form, and a donation page is a piece of
  content. You create forms for collecting information, and you create pages
  for presenting them. These are kept separate for a reason, it allows you
  to present the same forms for collecting information in different ways
  that are targeted to different audiences.

- Secondly, a donation form does not have to be public facing. If you are
  looking for a tool that allows you to handle offline donations, you can
  do that as well. You just deal with them differently - obviously, public
  users don't need to be entering offline donations.

- Third, there is a set of reports that comes with this module. They were
  built to give you insight into donation activity in your website. They
  were also built to be very flexible, and you can modify the reports
  themselves through a web-based admin interface. You can also create your
  own reports with little or no programming.

The rest of this document deals with the specific ways to work with donation
forms, donation pages, and reports. It provides a lot of information about
how to work with the tools included in this module, details about what to
expect from the reports, and some information about best practices when
extending this feature.

Just remember that, ideally, CRM Core Donation should be easy to configure.
If you are getting stuck, or if it's taking more than an hour to get started
with donation pages, it might be a good idea to reach out and ask for help.

2) Basic Configuration
----------------------
The main configuration page for CRM Core Donation can be found at
admin/config/crm-core/cmcd. This provides an option for selecting the
default email to be sent to users when they make a donation. It also includes an
option for setting the default colors used within reports.

There are a few things you want to consider when working with CRM Core Donation.

- Default Thank You Email: CRM Core Donation uses rules to send personalized
  emails thanking donors for giving. By default, this feature is turned off. To
  turn it on, go to admin/structure/crm-core/profile and select from the list of
  rules. You can modify this list by creating additional rules that include the
  tag 'CRM Core Donation Thank You Message.'

- Default Donation Processing Form: CRM Core Donation treats forms for processing
  payment separately from the page for displaying the forms. This makes sense when
  you consider the fact that good donation pages pages need to appeal to their
  audience, and there are a lot of ways to tweak your message to appeal to different
  audiences. Once you have created at least one donation processing form, set it as
  a default form at node/add/cmcd-page. Click the vertical tab labelled CRM Core
  Profile and select a form to automatically associate with new online donation
  pages.

3) Creating a front-facing online donation page
-----------------------------------------------
In order to create an online donation page, you need to create a donation form
before creating the page itself. The form is what captures the donation, the page is
what displays the form to users.

Once you have created at least one donation form, you will be able to configure it
as a default form to use for all new donation pages. More about this below.

Start by building a form in CRM Core Profile.

- Add a contact. You probably want to use a contact type of individual. Select the
  fields of contact information to collect within the form. You will need to add a
  name field to the record, along with any other information you want to capture
  about your donor.

- Add an activity of contact type Donation. For most online donation forms, you will
  want to add a number of fields for capturing information including a title, the
  receive date, the amount, the order, the online donation page, and the source.

- Add commerce components. You should add an amount field or a cart field to capture
  donation amounts. You can add a payment processing form for efficient checkouts.
  You can add a billing address to capture that separately from the contact record.

- Drag the fields into the order you wish for them to appear. Check the box off to
  the left in order to get fields to appear within your form. Set default amounts
  using the components to the right.

  When selecting what fields should appear in the form, it is important to consider
  what options you are looking to offer users. Users should probably not be able to
  see a field to select a source, for instance. Do not check the box to the left for
  this field. Users should usually be able to see a form for name, since you always
  need this field in order to process a payment. Make sure you select the box to the
  left for this field.

  This form supports tokens, and you will almost always want to set the following default
  values:

  - Activity Title: Online Donation

  - Order: set the default value to [commerce-order:order-id]. This will capture the order
    for the transaction in Drupal Commerce and give you easy access to the actual payment
    record.

  - Amount (CRM Core Activity): enter a default value for this field using tokens to
    [commerce-order:commerce-order-total:amount_decimal]. This will capture the amount from
    Drupal Commerce and ensure your reports are accurate.

  - Payment Type: this should usually be set to Credit Card for online payments.

  - Source: this can be set to anything you want.

- On the settings page, check the box to create a page. This will allow you to control some
  aspects of what happens after a donation is processed.

  - Enter a redirect path to push users to a thank you page once you have recieved a donation.

  - Enter a message to users to display a message after someone has made a donation.

- Under Contact Matching, you probably want to select the 'Apply default contact matching rules
  to this profile' option. This will ensure donations are matched up to existing contact records.

- Under Commerce Items, ensure payments will process accurately by taking the following steps:

  - Select a product. This field is an autocomplete, you can select products by name and SKU.
    The product will be used to process the payment, it tells Drupal Commerce what kind of thing
    is being used in the transaction.

  - If you are using an amount field, enter the amount here. If you want to allow users to select
    their own amounts, select one of the variable fields.

  - If you are using a cart, enter the line items and amounts here.

  - If you are using a payment processing form, you will need to select the name fields to be used
    for processing payments (most likely GIVEN and FAMILY). You will need to select an address field
    to be used for billing. You might want to select a payment processor for the transaction.

- After doing all this, save the profile. You now have a form for processing donations that can be
  associated with online donation pages.

- You can configure new online donation pages to display this form by default by going to
  admin/structure/types/manage/cmcd_page. Click on the vertical tab for CRM Core Profile. Check
  the box labelled 'Use CRM Core Profile for this node type.' This will allow you to select a donation
  form to appear by default when creating new donation pages.

- IMPORTANT: online donation forms will not appear as part of online donation pages unless the box
  in the previous step is checked.

Once you have created a donation form, you can create a donation page. Donation pages are nodes
and can be modified using any of the tools available for working with nodes in Drupal. When
creating a new online donation page, you can select any payment form created in CRM Core Profile.

IMPORTANT NOTE: when a donation form includes an Online Donation Page field, CRM Core Donation
will automatically attempt to populate it when the form is submitted. It is unnecessary to
prepopulate this field.

4) Working with online donation pages
-------------------------------------
When creating a new online donation page, there are some options available that go beyond basic
content entry.

- Select a donation form: users are allowed to override the default donation form settings
  when creating new donation pages. On the content edit screen, select the vertical tab for
  CRM Core Profile (note: this may have another label). There should be a drop down allowing
  users to select a donation form for this page.

- Select a thank you message: users are allowed to override the default thank you message sent
  when someone makes a donation. Select the vertical tab labelled CRM Core Donation and select
  a message from the 'Thank you email message' drop down to override the email being sent.

- Recommended Donation Amounts: If you are displaying a variable amount field in your donation form,
  you can set buttons for displaying recommended donation amounts. Users can check these buttons to
  automatically select an amount for their payment.

5) Creating a page for capturing offline donations
--------------------------------------------------
CRM Core Donation can also track offline donations. The process for creating an offline donation
form is similar to the process for creating a front-facing donation form, except that you will not
create a donation page. You will create a standalone form in CRM Core Profile and configure some
settings for the form to control workflow and security.

Follow the steps described in item #2, with the following additions:

- When adding fields for an activity type, you probably want to display the source and payment
  type fields.

- On the settings page, you probably want to include the following setttings:

  - Set the page path to a path users can use to get to your form in Drupal. Unlike a
    front facing donation page, offline donation forms should typically be displayed
    directly through CRM Core Profile.

  - Do not set a redirect path. In most cases, when people are entering offline donations,
    they will want to come directly back to this form.

  - Under permissions, select the roles that will have access to this form. It is important
    to secure forms for entering offline donation pages to prevent someone from accidentally
    finding your form and wreaking havoc on your records.

6) Reports
----------
CRM Core Donation ships with a number of reports. Reports can be found in CRM Core
on the contacts tab, and under admin/structure/crm-core/cmcd.

In the structure section, under the CRM Core tree, there is a report listing all
online donation pages. This can be accessed at admin/structure/crm-core/cmcd.

The reports available in CRM Core can be found at crm-core/reports and include:

- Donation Overview: provides an overview of all donation activity.

- Source Overview: provides details about donation activity summarized by source.

- Source Summary: provides details about donation activity for a selected source.
  This report includes details such as average donation amount, donation
  frequency, and other sources people like to contribute to.

- Online Donation Page Overview: provides a summary of performance for all
  online donation pages.

- Online Donation Page Summary: provides details about the donations that have
  been received from each donation page.

- Donation Detail: provides a detailed list of all donations.

- Donor Overview: provides some summary data about donors.

- Donor Detail: provides a detailed listing of all donors in the system.

- LYBUNT: provides a list of donors who contributed Last Year But Not This Year.

- SYBUNT: provides a list of donors who contributed Some Year But Not This Year.

7) Customizing / extending the components
-----------------------------------------
As mentioned above, this module ships with a content type and a CRM Core activity
type. Each of these components can be configured to taste using any of the tools
available within Drupal.

In most cases, configuring these components will consist of adding additional
fields. Great care has been taken to preserve namespaces and avoid field conflicts
with this module and other ones that can extend CRM Core. It is *very* easy
to get into situations where a feature conflicts with another one due to the use
of a field with the same name and a different purpose.

It's just as easy to avoid situations like this by following a few best practices
when customizing components:

- Always use namespaces for fields to avoid field conflicts with other modules.
  A field namespace is a set of letters applied to the machine name of the field
  being used. For CRM Core Donation, the field namespace is 'cmcd', and is applied to
  fields on the node and fields on the activity type.

- If you plan to distribute your feature to other people (which is part of being
  a responsible citizen of an open-source community), avoid using fields that modify
  the contact record. When you do, you are forcing other people to track information
  about contacts in a specific way. There's usually a better approach. Of course,
  if you don't plan to share your feature with anyone, feel free to include as
  many fields for the contact record as you like - this is appropriate in certain
  situations.

- If you want to create other components to help in fundraising, that's great!
  This module could really benefit from the insights of other people with an
  enthusiasm for online fundraising! Personal donation pages, gifts, pledges,
  terms of service agreements, donations on behalf of others would be great things
  to add to the module. When you do create your own components, export them into a
  separate feature with it's own namespace for fields. This is future-proofing -
  it will ensure you don't accidentally lose those components if you ever need to
  upgrade CRM Core Donation. It also makes it easy to share your work with other
  people, have it evaluated, and possibly get it merged with CRM Core Donation.

8) Customizing / Extending the reports
--------------------------------------
Also, the reports generated in CRM Core Donation are all registered
with hook_crm_core_report. It is important to register all reports
with this hook in order to ensure they are easy to find and can be
accessed by anyone using CRM Core. Anyone seeking to extend this module
is encouraged to use this hook in their own work.

The reports that ship with CRM Core Donation are limited to ones that
contain information directly related to this module. The reason for
this is simple: this module does not seek to force users to collect
information about contacts in a specific way.

For instance, it would not be hard to build a report that displays
donation levels by age group EXCEPT that this module does not collect
birthdays and CRM Core users could be tracking age in any one of a number
of ways. CRM Core Donation would not really know where that information
is kept without doing something more elaborate.

That does not mean someone could not build a general report that can return
information that includes age data. It simply means a developer would need
to allow users to configure a way to get at this information without
forcing someone to track age in a specific field.

Most of the reports in CRM Core Donation are built as panels, and most
of the panel components are built using views. In general, this is the
preferred way of creating new reports as it is a) portable (can be shared
across a range of sites) and b) customizable (can be easily modified
through a web-based administrative interface).

When extending the reports for CRM Core Donation, be careful to consider how other
people would use these reports. If you intend to distribute your reports, try to make
an effort to include options for selecting the appropriate fields to be used in the
reports. This may include the development of a custom views plug-in for selecting a
preconfigured field or building a custom module that houses a report.

Also, be careful to consider what paths will be used to access reports. There are
reserved paths in CRM Core (https://drupal.org/node/1950984) to think about when
authoring a report. Most likely, you want to use a path that includes the namespace
(mentioned in item #6) for your module, for a number of great reasons that are beyond
the scope of this documentation and explained in the CRM Core handbook.
