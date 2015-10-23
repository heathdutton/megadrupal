api = 2
core = 7.x

libraries[php-saml][type] = module
libraries[php-saml][download][type] = git
libraries[php-saml][download][url] = git@github.com:onelogin/php-saml.git
libraries[php-saml][destination] = "libraries"

libraries[xmlseclibs][download][type] = file
libraries[xmlseclibs][download][url] = https://xmlseclibs.googlecode.com/files/xmlseclibs-1.3.1.tar.gz
libraries[php-saml][destination] = "libraries"
