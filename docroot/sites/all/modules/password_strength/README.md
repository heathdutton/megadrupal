## Password Strength

Realistic password strength measurement for Drupal using the
Zxcvbn-PHP library.

### Dependencies

https://github.com/bjeavons/zxcvbn-php

### Install

You can choose to either use xautoload or composer_manager.

1. https://www.drupal.org/project/xautoload

Clone into lib: `cd lib && git clone https://github.com/bjeavons/zxcvbn-php`

2. https://www.drupal.org/project/composer_manager

If you have already composer_manager installed and you use drush to enable the
password_strength module, drush will automatically download the right dependency
for you. If you prefer to do this manually, follow the composer_manager
instructions, for example using drush:
`drush composer-json-rebuild && drush composer-manager update`

### Quickstart

1. Set a required password score of 1
  `drush vset password_strength_default_required_score 1`
2. Go to password change form
3. Enter new password 'password' and see validation fail

### Hooks

Implement hook_password_strength_minimum_score_alter(&$score, $account){} to
override the global password_strength_default_required_score variable for a user
account.