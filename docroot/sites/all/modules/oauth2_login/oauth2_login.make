api = 2
core = 7.x

defaults[projects][subdir] = contrib

projects[oauth2_client][version] = 1.4

projects[hybridauth][version] = 2.13

libraries[hybridauth][directory_name] = hybridauth
libraries[hybridauth][download][type] = get
libraries[hybridauth][download][url] = https://github.com/hybridauth/hybridauth/archive/v2.4.1.tar.gz

libraries[hybridauth-drupaloauth2][directory_name] = hybridauth-drupaloauth2
libraries[hybridauth-drupaloauth2][download][type] = get
libraries[hybridauth-drupaloauth2][download][url] = https://github.com/FreeSoft-AL/hybridauth-drupaloauth2/archive/v1.2.tar.gz


;---------------------------------------
; Dependencies of submodule oauth2_user.
;---------------------------------------

projects[http_client] = 2.x-dev

projects[wsclient][version] = 1.0
projects[wsclient][patch][] = https://drupal.org/files/wsclient-1285310-http_basic_authentication-14.patch
projects[wsclient][patch][] = https://drupal.org/files/issues/wsclient-2138617-oauth2_support.patch
