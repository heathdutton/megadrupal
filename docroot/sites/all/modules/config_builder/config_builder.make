core = 7.x
api = 2

projects[ctools][subdir] = 'contrib'
projects[ctools][version] = '1.0'

projects[form_builder][subdir] = 'contrib'
projects[form_builder][version] = '1.0'
; Escape single quotes in export - http://drupal.org/node/1684166#comment-6253628
projects[form_builder][patch][] = 'http://drupal.org/files/form_builder_quotes.patch'

projects[options_element][subdir] = 'contrib'
projects[options_element][version] = '1.7'

projects[variable][subdir] = 'contrib'
projects[variable][version] = '2.1'
