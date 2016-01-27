core = 7.x
api = 2

; Libraries
libraries[quail][download][type] = "get"
libraries[quail][download][url] = "http://quail-lib.googlecode.com/files/quail-lib-0.4.1.tar.gz"
libraries[quail][directory_name] = "quail"
libraries[quail][destination] = "libraries"
libraries[quail][patch][1948300_1] = "http://drupal.org/files/quail-bug_fixes.patch"
libraries[quail][patch][1948300_2] = "http://drupal.org/files/quail-reduce-pNotUsedAsHeader-false_positives.patch"
libraries[quail][patch][1948300_3] = "http://drupal.org/files/quail-php_54-2.patch"
