Panopoly Config
===

This module let's you build custom configuration pages for your Panopoly distributions.

## Features

* Build configuration pages from a simple hook.
* Separate configuration pages.
* Sync configuration and site variables.
* Easily create an configuration form for the distribution installation.

## How it works

The module reads configuration from a hook and build separate configuration pages for each config group.

### Sample code

    <?php

    /**
     * Implements hook_panopoly_config_info()
     */
    function MODULE_NAME_panopoly_config_info() {
      $info = array();

      // Contact.
      $info['contact'] = array(
        'title' => t('Contact'),
        'description' => t('Contact and opening hours.'),
        'weight' => 20,
        'config' => array(
          'phone' => array(
            'title' => t('Phone'),
            'type' => 'string',
            'description' => t('Phone number for restaurant.'),
            'show_on_install' => TRUE,
          ),
          'address' => array(
            'title' => t('Address'),
            'type' => 'text',
            'description' => t('Restaurant address.'),
            'show_on_install' => TRUE,
          ),
          'opening_hours' => array(
            'title' => t('Opening Hours'),
            'type' => 'text',
            'description' => t('Restaurant opening hours.'),
            'show_on_install' => TRUE,
          ),
        ),
      );

      return $info;
    }

    ?>

This code will build a configuraton page at **Configuration --> Distribution Name --> Contact**. 

### What is variable syncing?

This means that we will automatically update a config variable when its site variable equivalent is updated and vice versa.

Say, your hook defines a *site_name* config, when the *site_name* variable changes, we'll update your config as well and vice versa.

This is useful if you want to build simpler config pages for your Distribution.

### How do I add this to my distribution installation tasks?

Easy. Add a new task to your profile as follows. We will handle the rest.

    <?php

    /**
     * Implements hook_install_tasks().
     */
    function PROFILE_install_tasks($install_state) {
      // Add a configuration task.
      $tasks['configure_profile'] = array(
        'display_name' => st('Configure profile'),
        'type' => 'form',
      );
    }

    /**
     * Callback for configure profile.
     */
    function configure_profile($form, &$form_state, &$install_state) {
      module_load_include('inc', 'panopoly_config', 'panopoly_config.profile');
      $form += panopoly_config_get_profile_form();
      return $form;
    }

    ?>
