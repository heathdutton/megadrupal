<?php
/**
 * @file
 * Repository entity metadata controller implementation.
 */

/**
 * Repository entity metadata controller.
 */
class PHRepositoryMetadataController extends EntityDefaultMetadataController {
  /**
   * Adjusts default entity properties.
   */
  public function entityPropertyInfo() {
    $propertyInfo = parent::entityPropertyInfo();
    $properties = &$propertyInfo[$this->type]['properties'];

    // Tweak entity key properties.
    $properties['type']['label'] = t('Repository type');
    $properties['type']['description'] = t('Type of the project repository providing detail fields and source code management functionality.');

    // Add summary pseudo-property.
    $properties['summary'] = array(
      'type' => 'text',
      'label' => t('Summary'),
      'description' => t('A textual summary of the repository, used as a label.'),
      'getter callback' => 'ph_repository_get_summary',
    );

    return $propertyInfo;
  }
}
