core = 7.x

api = 2

;projects[drupal][version] = "7.27"
projects[] = drupal

projects[libraries][subdir] = "contrib"
projects[libraries][version] = "2.2"

projects[background_process][subdir] = "contrib"
projects[background_process][version] = "1.16"

projects[classloader][subdir] = "contrib"
projects[classloader][version] = "1.1"

projects[ctools][subdir] = "contrib"
projects[ctools][version] = "1.4"

projects[defaultcontent][subdir] = "contrib"
projects[defaultcontent][version] = "1.0-alpha9"

projects[entity][subdir] = "contrib"
projects[entity][version] = "1.5"

projects[entity_translation][subdir] = "contrib"
projects[entity_translation][version] = "1.0-beta3"

projects[inject][subdir] = "contrib"
projects[inject][version] = "2.3"

projects[features][subdir] = "contrib"
projects[features][version] = "2.0"

projects[i18n][subdir] = "contrib"
projects[i18n][version] = "1.10"

projects[progress][subdir] = "contrib"
projects[progress][version] = "1.4"

projects[rules][subdir] = "contrib"
projects[rules][version] = "2.6"

projects[title][subdir] = "contrib"
projects[title][version] = "1.0-alpha7"

projects[ultimate_cron][subdir] = "contrib"
projects[ultimate_cron][version] = "1.10"

projects[ultimate_cron_queue_scaler][subdir] = "contrib"
projects[ultimate_cron_queue_scaler][version] = "1.0"

projects[variable][subdir] = "contrib"
projects[variable][version] = "2.4"

projects[views][subdir] = "contrib"
projects[views][version] = "3.7"

projects[views_bulk_operations][subdir] = "contrib"
projects[views_bulk_operations][version] = "3.2"

; Library: Smartling Translation API
; ---------------------------------------
libraries[api][destination] = "libraries"
libraries[smartling][download][type] = git
libraries[smartling][download][url] = https://github.com/Smartling/api-sdk-php.git
libraries[smartling][directory] = "smartling"
