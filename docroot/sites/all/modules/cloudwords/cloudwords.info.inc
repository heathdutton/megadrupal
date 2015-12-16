<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class CloudwordsTranslatableMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['textgroup']['options list'] = 'cloudwords_textgroup_options_list';

    $properties['type']['options list'] = 'cloudwords_type_options_list';

    $properties['language']['options list'] = 'cloudwords_metadata_language_list';

    $properties['status']['options list'] = 'cloudwords_status_options_list';

    $properties['translation_status']['options list'] = 'cloudwords_exists_options_list';

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The owner of the profile."),
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_setter_method',
      // 'setter permission' => 'administer profiles',
      // 'required' => TRUE,
      'schema field' => 'uid',
    );

    return $info;
  }
}
