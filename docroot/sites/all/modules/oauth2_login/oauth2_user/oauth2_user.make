api = 2
core = 7.x

defaults[projects][subdir] = contrib

projects[http_client] = 2.x-dev

projects[wsclient][version] = 1.0
projects[wsclient][patch][] = https://drupal.org/files/wsclient-1285310-http_basic_authentication-14.patch
projects[wsclient][patch][] = https://drupal.org/files/issues/wsclient-2138617-oauth2_support.patch
