<?php

/**
 * @file
 * Register a set of tags with metatag module.
 */

/**
 * Implements hook_metatag_info().
 */
function opentags_metatag_info() {
  $info['groups']['opentags'] = array(
    'label' => t('OpenTags'),
    'form' => array(
      '#weight' => 900,
      '#description' => t('OpenTags is a set of AGLS metatags.'),
      '#attributes' => array(
        'class' => array('opentags-container'),
      ),
      '#attached' => array(
        'js' => array(
          OPENTAGS_MODULE_PATH . '/widgets/js/opentagsui.js' => array(
            'scope' => 'footer',
          ),
          OPENTAGS_MODULE_PATH . '/widgets/js/opentagsdropdown.js' => array(
            'scope' => 'footer',
          ),
          OPENTAGS_MODULE_PATH . '/widgets/js/opentagsautocombo.js' => array(
            'scope' => 'footer',
          ),
          OPENTAGS_MODULE_PATH . '/js/opentags.form.js' => array(
            'scope' => 'footer',
          ),
        ),
        'css' => array(
          OPENTAGS_MODULE_PATH . '/widgets/css/opentagsui.css' => array(
            'type' => 'file',
          ),
        ),
      ),
    ),
  );

  $info['tags']['DCTERMS.audience'] = array(
    'label' => t('Audience'),
    'description' => t("Audience"),
    'class' => 'DrupalTextMetaTag',
    'group' => 'opentags',
    'form' => array(
      '#attributes' => array(
        'class' => array('opentags-src-field-hidden', 'opentags-src-audience'),
      ),
    ),
  );

  $info['tags']['DCTERMS.coverage'] = array(
    'label' => t('Coverage'),
    'description' => t("Coverage"),
    'class' => 'DrupalTextMetaTag',
    'group' => 'opentags',
    'form' => array(
      '#attributes' => array(
        'class' => array('opentags-src-field-hidden', 'opentags-src-coverage'),
      ),
    ),
  );

  $info['tags']['AGLSTERMS.function'] = array(
    'label' => t('Function'),
    'description' => t("Function"),
    'class' => 'DrupalTextMetaTag',
    'group' => 'opentags',
    'form' => array(
      '#attributes' => array(
        'class' => array('opentags-src-field-hidden', 'opentags-src-function'),
      ),
    ),
  );

  $info['tags']['DCTERMS.language'] = array(
    'label' => t('Language'),
    'description' => t("Language"),
    'class' => 'DrupalTextMetaTag',
    'group' => 'opentags',
    'form' => array(
      '#attributes' => array(
        'class' => array('opentags-src-field-hidden', 'opentags-src-language'),
      ),
    ),
  );

  $info['tags']['DCTERMS.type'] = array(
    'label' => t('Type'),
    'description' => t("Type"),
    'class' => 'DrupalTextMetaTag',
    'group' => 'opentags',
    'form' => array(
      '#attributes' => array(
        'class' => array('opentags-src-field-hidden', 'opentags-src-type'),
      ),
    ),
  );

  $info['tags']['DCTERMS.subject'] = array(
    'label' => t('Subject'),
    'description' => t("Subject"),
    'class' => 'DrupalTextMetaTag',
    'group' => 'opentags',
    'form' => array(
      '#attributes' => array(
        'class' => array('opentags-src-field-hidden', 'opentags-src-subject'),
      ),
    ),
  );

  return $info;
}
