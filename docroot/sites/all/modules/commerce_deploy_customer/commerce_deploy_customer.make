; Commerce Deploy Customer Makefile

api = 2
core = 7.x

; Contribs

projects[google_analytics][version] = 1.4
projects[google_analytics][subdir] = contrib

projects[message][version] = 1.10
projects[message][subdir] = contrib

projects[message_notify][download][type] = git
projects[message_notify][download][branch] = 7.x-2.x
projects[message_notify][download][revision] = 7247ec2
projects[message_notify][subdir] = contrib

; Commerce Contribs

projects[commerce_addressbook][version]= 2.0-rc9
projects[commerce_addressbook][subdir] = contrib

projects[commerce_google_analytics][version] = 1.1
projects[commerce_google_analytics][subdir] = contrib

projects[commerce_message][download][type] = git
projects[commerce_message][download][branch] = 7.x-1.x
projects[commerce_message][download][revision] = ffdb79f
projects[commerce_message][subdir] = contrib
