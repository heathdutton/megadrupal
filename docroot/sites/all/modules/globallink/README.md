# Globallink Module Redesign

## Table of Contents

- [Initial Installation](#initial-installation)
- [Enabling Multilingual Support](#enabling-multilingual-support)
- [GlobalLink Settings](#globallink-settings)
- [Adaptor Settings](#adaptor-settings)
- [Implementing the Node/Field Translation Filter](#implementing-the-nodefield-translation-filter)
- [Setting up the External (GlobalLink) Cron](#setting-up-the-external-globallink-cron)
- [Locale Mapping](#locale-mapping)
- [Field Configuration](#field-configuration)
- [Third Party Modules](#third-party-modules)

## Initial Installation

1.  Copy the `globallink` folder into `sites/all/modules` on your Drupal installation.
2.  On Drupal, go to your **Modules** tab.
3.  Enable the **GlobalLink Content** module and its dependencies to use Content Translations.
4.  Enable the **GlobalLink Entity Translation** module and its dependencies to use Entity Translations.
5.  Optionally enable the modules listed below to translate certain content types:
  - **GlobalLink Block Translation** - to translate Blocks.
  - **GlobalLink Built-in Interface Translation** – to translate Interfaces.
  - **GlobalLink Fieldable Panels Panes Translation** – to translate content from the Fieldable Panels Panes module.
  - **GlobalLink Menu Translation** – to translate Menus.
  - **GlobalLink Taxonomy Translation** – to translate Taxonomy.
  - **GlobalLink Webforms Translation** – to translate Webforms.
6.  Update your Drupal modules by running the Drupal update script.
7.  Restart your Drupal web server.  If the installation was successful, a **GlobalLink** tab will be visible on the Drupal menu.

## Enabling Multilingual Support

Multilingual support must be enabled manually in Drupal for each content type.

To enable translation for a content type:

1.  Go to **Publishing Options** for a content type in the GlobalLink module.
2.  Select:
  - **Enabled, with translation** - to use Content Translations.
  - **Enabled, with field translation** - to use Entity Translations.

Multilingual support for the content type is now enabled.

## GlobalLink Settings

To configure **GlobalLink Settings**:

1.  Go to the **Settings** tab in the **GlobalLink** module.
2.  Click **GLOBALLINK SETTINGS**.
3.  Fill out the required fields.  You can find a reference below.
4.  Click **Save and Test Settings**.
5.  If all of the information was entered correctly, a “Connection Successful” message appears. Otherwise, an error message appears, showing the nature of the error (e.g. “Invalid login information”).

\* denotes a value that will be provided by Translations.com.

| Field                   | Description |
| -----                   | ----------- |
| URL*                    | Points the adaptor to the GlobalLink URL where content will be submitted. |
| User ID*                | Username to log in to the GlobalLink instance. |
| Password*               | Password for the GlobalLink instance. |
| Project Short Code*     | Project identifier(s) in the GlobalLink instance (multiple values can be entered separated by commas). |
| Submission Name Prefix  | A prefix to identify the submission. |
| Classifier*             | The GlobalLink file format to submit content in. |
| Max Target Count        | The maximum number of results that the adaptor retrieves at a time from GlobalLink. The default value is 10. |

## Adaptor Settings

To configure **Adaptor Settings**:

1.  Go to the **Settings** tab in the **GlobalLink** module.
2.  Click **ADAPTOR SETTINGS**.
3.  Fill out the required fields.  You can find a reference below.
4.  Click **Save Adaptor Settings**.

| Field                                        | Description |
| -----                                        | ----------- |
| Dashboard Pager Limit                        | The number of records to be displayed per page on the dashboard.  Default value is 10. |
| Enable Preview For Receive Translations      | If set to **Yes**, translated content can be previewed in the **Receive Translations** tab.  If set to **No**, the Preview Translation feature will not be available. |
| Node/Field Translation Filter Implementation | If set to **Standard**, any language node of your Drupal’s default language will be eligible for translation and will therefore appear on the GlobalLink dashboard. <br /><br /> If set to **Using Hook (hook_globallink_is_translatable)**, you can filter out nodes from being displayed on the GlobalLink Dashboard (see [Implementing the Node/Field Translation Filter](#implementing-the-nodefield-translation-filter)). This will also allow you to run your code by implementing the **module_globallink_import_translation** function. |
| Publish Translated Content                   | If set to **No**, translated content imported by the adaptor will be available as a new unpublished version. If set to **Yes**, the newly imported translated content version will be published automatically. If set to **Use Source Content Setting**, the imported content will inherit the “Published” property of the source file. |
| Automatic Update Status                      | The adaptor can periodically query GlobalLink for translated content. This sets the frequency in which the polling is performed. If set to **Drupal Cron**, it will use the Drupal Cron’s time interval setting. If a different interval is required, **External Cron** must be chosen and a `globallink_cron.php` file must exist (provided by Translations.com). If set to **No**, translated content must be manually retrieved. |
| Logging                                      | Adaptor actions can be logged for eventual troubleshooting. If set to **Disabled** no actions are logged, if set to **Info** relevant actions are logged, and if set to **Debug** most actions are logged. |
| Proxy URL                                    | If your network includes a proxy, enter its URL (example URL: http://10.10.0.1 or http://10.10.0.1:8080, *no trailing slash "/") so that the GlobalLink adaptor can communicate with the Translations.com systems. |

## Implementing the Node/Field Translation Filter

If the **Hook (hook_globallink_is_translatable)** option was chosen for the **Node/Field Translation Filter Implementation** adaptor setting (see [Adaptor Settings](#adaptor-settings) for more information), the method must be implemented in the code. This allows for the following to be excluded from being sent for translation:

- Nodes: Any node that is not translatable can be excluded and will not be displayed on the GlobalLink dashboard for translation.
- Fields: Content of the excluded field(s) will not be sent for translation.

An example of this implementation can be found in the `hook_sample.php` file of the `globallink` folder. This is a hook to be called before the translated node is saved in Drupal, allowing for the customization of links, hiding source nodes, or any other applicable business logic.

Note:

- Nodes are checked only when the GlobalLink module adds the content to the GlobalLink&reg; dashboard.
- Fields are checked when the GlobalLink module sends content to GlobalLink&reg;.
- If a node or field is not translatable, the function will not return a value, not even false.

## Setting up the External (GlobalLink) Cron

Using the External Cron allows tasks to be scheduled at a time independent from the default Drupal Cron, therefore allowing for customized time intervals.

To use the External Cron:

1.  Confirm that the **Automatic Update Status** option is set to **External Cron** in the **Adaptor
Settings** tab of the GlobalLink module. (For more information, see [Adaptor Settings](#adaptor-settings)).
2.  Save the `globallink_cron.php.backup` file as `globallink_cron.php`  in your Drupal root folder.
3.  Add a new cron job on your operating system's cron install and set the time interval as desired.  In the `crontab`, execute the URL: http://drupal_site_url/globallink_cron.php.
4.  For security, add the following lines to the `.htaccess` file in your Drupal install root folder:

```
<Files "globallink_cron.php">
  Order deny,allow
  Allow from xxx.xxx.xxx.xxx
  Deny from all
</Files>
```

## Locale Mapping

This setting maps Drupal locales to GlobalLink&reg; locales.

Note: Before the locales are defined, the Drupal language must be added in the **Site Configuration** of your Drupal site.

To map locales in the GlobalLink module:

1.  Go to the **Locale Mapping** tab in the GlobalLink&reg; module.
2.  Select the **Drupal Locale** and its corresponding **GlobalLink Locale**, and click **Add**.

Once the mapping is added, content can be sent for translation into any of the defined locales.

## Field Configuration

Fields of each content type can be set as translatable, allowing them to be sent to GlobalLink&reg; for translation.

To configure a field as translatable:

1.  Go to the **Field Configuration** tab in the **GlobalLink&reg;** module.
2.  Select a content type from the **Content Type** dropdown list to see Fields only from that content type.
3.  If new Fields have been added after configuring the GlobalLink Drupal module, select them from the **Fields** dropdown list and click **Add** to add them to the list of Fields.
4.  Select the Fields to be marked as translatable using the checkbox column.
5.  Click the **Update** button to save your settings.

## Third Party Modules

The GlobalLink&reg; Drupal module supports translation of native Drupal Nodes, Blocks, Menus, and a limited number of third party modules:

- Field Collections
- Metatag
- Features
- Exclude Node Title
- Link
- Webform
- Revisions
- Entity
- Fieldable Panels Panes

If you intend to use the GlobalLink&reg; module to translate other Drupal elements or content managed by another module, this must be communicated to Translations.com, and its feasibility evaluated.
