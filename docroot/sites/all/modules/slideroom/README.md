INTRODUCTION
------------

The SlideRoom module provides Drupal integration with the SlideRoom API (https://api.slideroom.com/),
for importing data from your organization's SlideRoom account into Drupal.

 * For a full description of the module, visit the project page:
   https://drupal.org/project/slideroom

 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/slideroom

REQUIREMENTS
------------

 * Libraries API (https://www.drupal.org/project/libraries)

 * SlideRoom API Wrapper for PHP (https://github.com/thinkshout/slideroom-api-php)

INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

 * Required: download the SlideRoom API Wrapper for PHP (https://github.com/thinkshout/slideroom-api-php),
   and place it in the libraries folder, as specified by Libraries API.

CONFIGURATION
-------------

 * Configure user permissions in Administration » People » Permissions:

   - Administer SlideRoom

     Access the SlideRoom configuration options.

 * Configure SlideRoom settings in Administration » Configuration » Web Services » SlideRoom.

   - Required: SlideRoom API OAuth token

     An OAuth token is required to access the API. To generate a token, visit the Developer API page,
     which can be found under Settings in the Review app.

   - Optional: SlideRoom API Request Throttle (hours)

     Minimum number of hours between new export requests to the SlideRoom API (e.g. 24 = one export per
     day). By default, new export requests are triggered every time cron runs. Set this to throttle API
     requests, or leave blank for no limit.

EXAMPLE USAGE
-------------

    /**
     * Implements hook_slideroom_application_export().
     */
    function my_module_slideroom_application_export() {
      $exports['my_export'] = array(
        'format' => 'csv',
        'tab.export' => 'My Custom Export',
      );

      return $exports;
    }

    /**
     * Implements hook_slideroom_application_export_complete().
     */
    function my_module_slideroom_application_export_complete($application_result, $export_result) {
      if ($application_result['name'] == 'my_export') {
        $csv_data = slideroom_parse_exported_files('csv', $export_result['response_data']);

        foreach ($csv_data as $data) {
          // ...
        }
      }
    }

MAINTAINERS
-----------

Supporting organizations:

 * ThinkShout - https://www.drupal.org/marketplace/thinkshout
