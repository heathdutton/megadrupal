api = 2
core = 7.x

defaults[projects][subdir] = contrib

projects[oauth2_client][version] = 1.3

projects[hybridauth][version] = 2.13

libraries[hybridauth][directory_name] = hybridauth
libraries[hybridauth][download][type] = get
libraries[hybridauth][download][url] = https://github.com/hybridauth/hybridauth/archive/v2.4.1.tar.gz

libraries[hybridauth-drupaloauth2][directory_name] = hybridauth-drupaloauth2
libraries[hybridauth-drupaloauth2][download][type] = get
libraries[hybridauth-drupaloauth2][download][url] = https://github.com/FreeSoft-AL/hybridauth-drupaloauth2/archive/v1.2.tar.gz
