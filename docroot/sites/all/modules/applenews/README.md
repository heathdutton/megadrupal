# Apple News

#### Table of Contents

1. [TL;DR](#tldr)
2. [Before you Start](#before)
3. [Minimum Requirements](#minimum-req)
4. [Installation](#installation)
    1. [Drush based installation](#drush-install)
    2. [Manually based installation](#manual-install)
5. [Configuration](#configuration)
    1. [Initial settings configuration](#initial)
    2. [Channel/Channel settings](#channel)
    3. [Node configuration](#node)
    4. [Preview post](#preview)
    5. [Delete from channel](#delete)
6. [Troubleshoot](#troubleshoot)
7. [Developer API](#developer-api)
8. [Contribute](#contribute)


## <a name="tldr"></a>TL;DR

[Install required  modules](https://www.drupal.org/documentation/install/modules-themes/modules-7):

-   [Libraries](https://www.drupal.org/project/libraries)
-   [Entity](https://www.drupal.org/project/entity)

[Install required Libraries](https://www.drupal.org/node/1440066):

-   [php-curl-class 4.6.9](https://github.com/php-curl-class/php-curl-class)
-   [AppleNewsAPI 0.3.7](https://github.com/chapter-three/AppleNewsAPI)

Enable:

-   Apple News (`applenews`) -- Core module, you will be able to push content but it won't look very appealing.
-   Apple News Example (`applenews_example`) -- Example implementation, defines a usable export and style.

Other modules:

-   Apple News Extras (`applenews_extras`) -- Extension module with support for various contrib field modules.

Go to `/admin/config/content/applenews/`:

1.  Configure your connection to the Apple Publisher.
2.  Add and configure an export.
3.  Preview exported content to test your export.
4.  Push.

## <a name="before"></a>Before You Start

Before you start you must have a working Drupal 7 site, permissions to administer modules and add code, and that you meet the [Minimum Requirements](#minimum-req).

To install Drupal see these guides: [Quick installation](https://www.drupal.org/documentation/install) and [Quick installation for developers](https://www.drupal.org/documentation/install/developers).

[Drush](https://github.com/drush-ops/drush) is not mandatory but will make for an easier, faster installation.

## <a name="minimum-req"></a>Minimum Requirements

These are the minimum requirements for the Apple News module.

* **PHP version 5.4 and above.** Version 5.3 and below will cause a fatal error.
* **Drupal 7.** The late version of Drupal 7 is recommended.
* **PHP compiled with cURL support**. Check by going to /admin/reports/status/php on your Drupal site and looking for 'curl'. If not enabled, do a Google search for 'enable curl php [your server type]'.
* **(Optional) Drush 5.9 and above**. If Drush is installed, follow the [Drush-based Installation](#drush-install) guide. Otherwise, follow the [Manual Installation](#manual-install) guide

## <a name="installation"></a>Installation

You can install the Apple News module using Drush or manually by adding the modules and libraries to your code base.

### <a name="drush-install"></a>Drush-based installation

To properly run Drush commands, first you must shell into the folder that contains the settings.php file for your Drupal site. Some possible settings.php file locations are:

```shell
sites/default/settings.php file
sites/mysite.local/settings.php
sites/mysuperawesomenewssite.com/settings.php
```

1. Download and enable the module dependencies for this module.

    -   [Libraries](https://www.drupal.org/project/libraries)
    -   [Entity](https://www.drupal.org/project/entity)

    ```shell
    $ drush dl libraries entity
    $ drush en libraries entity
    ```

2. In your terminal, navigate to your `sites/all/libraries` folder and run the following curl commands to download the proper library versions. If the `libraries` folder does not exist, create it before running the following command:

    ```shell
    $ curl -L https://github.com/php-curl-class/php-curl-class/archive/4.6.9.tar.gz | tar xz
    $ mv php-curl-class-4.6.9 php-curl-class
    ```

3. While still inside the libraries folder, run the following curl command:

    ```shell
    $ curl -L https://github.com/chapter-three/AppleNewsAPI/archive/0.3.7.tar.gz | tar xz
    $ mv AppleNewsAPI-0.3.7 AppleNewsAPI
    ```

4. After the libraries are downloaded, your should have directories matching the following paths. You may need to rename the folders you just downloaded to match):

    ```
    sites/all/libraries/AppleNewsAPI/[files start here]
    sites/all/libraries/php-curl-class/[files start here]
    ```

5.  [Add the applenews module](https://www.drupal.org/documentation/install/modules-themes) to your code base  and enable it using the Drush command:

    ```shell
    $ drush dl applenews
    $ drush en applenews -y
    ```

    The applenews_example module defines a nicely styled export and a few more sophisticated components.

    ```shell
    $ drush en applenews_example -y
    ```

If you enable the module before downloading the required libraries, you will receive error messages telling you download them. Please try re-installing the libraries or check out the [Troubleshooting Section](#troubleshooting).

After installing all modules and libraries, check the status of the installation on your sites Status Report page (`admin/reports/status`). Look for "Apple News" and "PHP Curl Class" and make sure they are green. If green, you are good to go and jump to the [Configuration Section](#configuration). If not, please see the [Troubleshooting section](#troubleshoot).

### <a name="manual-install"></a>Manual Installation

To manually install:

1.  Download this module and its dependencies:
    -   [Apple News](https://www.drupal.org/project/applenews)
    -   [Libraries](https://www.drupal.org/project/libraries)
    -   [Entity](https://www.drupal.org/project/entity)
2.  [Download the Apple News library (version 0.3.7)](https://github.com/chapter-three/AppleNewsAPI/archive/0.3.7.zip) into your sites/all/libraries folder. If the libraries folder does not exist, create it before downloading.
3.  [Download and install the PHP Curl Class library (version 4.6.9)](https://github.com/php-curl-class/php-curl-class/archive/4.6.9.tar.gz) into your libraries folder.
4.  After the libraries are downloaded, your should have directories matching the following paths. (You may need to rename the folders you just downloaded to match):

    ```shell
    sites/all/libraries/AppleNewsAPI/[files start here]
    sites/all/libraries/php-curl-class/[files start here]
    ```

5.  Go to `admin/modules` enable the Apple News module. This will enable the Libraries module and the Entities module, as well as any additional dependencies.

    The Apple News Example module defines a nicely styled export and a few more sophisticated components.

After installing all modules and libraries, check the status on your site's Status Report page (`admin/reports/status`). Make sure that "Apple News" and "PHP Curl Class" are green. If so, you can jump to the [Configuration section](#configuration). If not, see the [Troubleshooting section](#troubleshoot)


## <a name="configuration"></a>Configuration

Follow these configuration instructions to start publishing your content.

### <a name="initial"></a>Initial Settings configuration

1. Visit [apple.com](http://apple.com) to get your credentials and create a news channel that your Drupal site will use.

2. In your Drupal site, navigate to the "Apple news credentials page" (`admin/config/content/applenews/settings`) and add your Apple News credentials.

3. In your Drupal site, navigate to the "Apple news channels page" (`admin/config/content/applenews/settings/channels`) and add a channel ID from your Apple account. Please add one ID at a time. The channels are validated by the Apple credentials that you added to your Drupal site. If valid, it will fetch the channel information and add them to your site's list of channels.

### <a name="channel"></a>Export Configuration

An *export* is code that defines how to transform data in a Drupal site so it can be pushed to Apple News. The Apple News module defines a simple export, while the `applenews_example` module defines a more usable style.

To get started, we suggest enabling the `applenews_example` module and using that as a starting point.

1. In your Drupal site, navigate to the "Apple news export manager page" (`admin/config/content/applenews`).
2. Click on the **'edit'** link of the export you would like to connect to an Apple News channel.
3. On the Edit page, the minimum requirements to properly configure a channel to an Apple News channel are:
    1. Under "Add new component", select a component.
    2. Under "Channels", select the channel (Apple News Channel) that this export will be tied to. This export channel will get nodes, process them, and send them to the selected channel for display in the Apple News app.
    3. Under "Content types", select the content types that should be processed with this channel.
    4. Click **Save Changes**.
    5. After saving, you will see "edit" and "delete" options to the right of the new components we just added. Click on **"edit"**.
    6. Configure the component. Most components will require that you specify source fields and the component will use the data in those fields as content in the component.
    7. Click **Save Changes**.

### <a name="node"></a>Node Configuration

Once a content type is enabled in an export/channel, the option to add the individual post is in the node's add/edit page. If a content type is not added to any channel export, these options will not be available on the node add/edit page.

1. To add a node to the channel sent to Apple, select _"Publish to Apple"_ in the "Apple News" tab. If you want to temporarily stop publishing to Apple, or make revisions to the post before publishing or re-publishing to Apple, deselect this checkbox. It is the equivalent to the "Publish/Unpublish" feature with Drupal nodes.

2. Select one or more channels from the available list.

3. For each selected channel, select an available "section" that it belongs to. ("Sections" are created on apple.com, where you initially created the channel).

4. Once a node is initially published to an Apple news channel, it will display a general information section showing post date, share URL, and the section and channel where it is published.

### <a name="preview"></a>Preview a Post Before Publishing

If you want to preview a post before sending it to Apple, you will need to first download and install the Apple "News Preview" Application (LINK TBD).

1. After saving the node, return to the node's edit page
2. Find the "Apple News" tab, and click the "Download" link under "Preview." This will download a folder containing the specialy formatted file needed by the News Preview App.
2. Drag the whole folder into the App icon to open, and it will display the page just as the Apple News App will be displaying it.

### <a name="delete"></a>Delete a post from publishing

If you want to delete a post from a channel, but not delete the post itself, There is a **delete** link in the "Apple News" tab.


## Congrats!

You are now ready to start sharing your posts and articles with the Apple News Service and with the world. Happy Posting!


## <a name="troubleshoot"></a>Troubleshoot

If you are having trouble installing the module or its dependencies, review the common scenarios below.

**Problem:** I'm getting the error message that includes:

```shell
Fatal error: undefined '['
```

**Solution:** This means that your version of PHP does not meet minimum standards. Version 5.3 and below are not able to process the bracketed php formatting of the AppleNewsAPI library. Updating your version of PHP to 5.4 and above will fix this.

---

**Problem:** I'm getting the error message that includes:

```shell
SSL certificate problem: unable to get local issuer certificate
```

**Solution:** This is a mis-configuration in your server setup. Depending on what server OS you are using, the fix is different. Please see this StackOverflow post "[curl: (60) SSL certificate : unable to get local issuer certificate](http://stackoverflow.com/questions/24611640/curl-60-ssl-certificate-unable-to-get-local-issuer-certificate)" for more information on possible fixes specific for your system.

---

**Problem:** I'm getting the error message:

```shell
Please download PHP-Curl-Class (version 4.6.9) library to sites/all/libraries/php-curl-class
```

**Solution:** This means that the library has not been downloaded, the wrong version is in place, or the folder for the library is labeled wrong. Double check that the library was downloaded into `sites/all/libraries/php-curl-class/[files start here]`. Check that the version is 4.6.9 by opening up the composer.json file and search for "version": "4.6.9". Lastly, if still not resolved, make sure the folder is named `php-curl-class` and **NOT** something like `php-curl-class-master` or `php-curl-class-4.6.9`.

---

**Problem:** I'm getting the error message:

```shell
Please download AppleNewsAPI (version 0.3.7) library to sites/all/libraries/AppleNewsAPI
```

**Solution:** This means that the library has not been downloaded, the wrong version is in place, or the folder for the library is labeled wrong. Double check that the library was downloaded into `sites/all/libraries/AppleNewsAPI/[files start here]`. Check that the version is 0.3.7 by opening up the composer.json file and search for "version": "0.3.7". Lastly, if still not resolved, make sure the folder is named `AppleNewsAPI` and **NOT** something like `AppleNewsAPI-master` or `AppleNewsAPI-0.3.7`.

---

Please consult [drupal.org](http://drupal.org) for any issues outside of this scope.

## <a name="developer-api"></a>Developer API

The `applenews` module defines an API and class structure to make it easy to write code to export Drupal data into arbitrarily structured Apple News documents.

The module defines hooks to register custom export classes, modify existing export classes, and alter insert/update/delete operations on the Apple News Publisher API. See `applenews.api.php` for details.

### Writing Custom Exports

There are 3 base class/interfaces:

-   `ApplenewsExport` -- Defines an export and its configuration.
-   `ApplenewsSourceInterface` -- Defines a source of Drupal data.
-   `ApplenewsDestinationInterface` -- Defines the transformation of drupal data into Apple News Components.

The main module and sub-modules contain many usage examples.

## <a name="contribute"></a>Contribute

@todo issue queue, module page, repository, etc.

### Run Tests

To run module tests, enable the core "Testing" module, from the Modules admin page or with command line.

To enable and run tests from the UI:

1.  Navigate to the Testing admin page (`admin/config/development/testing`).
2.  Select "Apple News" from the list of tests
3.  Click **Run Tests**

To run test from command line, enter the following commands one at a time:

```shell
drush -y en simpletest
php scripts/run-tests.sh --verbose --color 'Apple News'
```
