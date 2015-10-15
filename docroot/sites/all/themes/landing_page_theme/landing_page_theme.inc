<?php
function landing_page_theme_exist_libraries() {
    if (!module_exists('color')) {
        drupal_set_message(t('Landing Page Theme require Color module!'), 'error');
    }
    if (!module_exists('libraries')) {
        drupal_set_message(t('Landing Page Theme require Libraries API module!'), 'error');
    }
    else {
        $appear = libraries_detect('appear');
        if ($appear['library path'] == "") {
            drupal_set_message(t('Landing Page Theme require Appear JQuery library!'), 'error');
        }
        else {
            libraries_load('appear');
        }

        $fontawesome = libraries_detect('fontawesome');
        if ($fontawesome['library path'] == "") {
            drupal_set_message(t('Landing Page Theme require Fontawesome library!'), 'error');
        }
        else {
            libraries_load('fontawesome');
        }
    }
}