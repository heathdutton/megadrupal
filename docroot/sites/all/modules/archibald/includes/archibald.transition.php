<?php

/**
 * @file
 * LOM-CH data sanitization.
 *
 * The old data structure in CouchDB is *horrible*. It's a complete mess, and
 * doesn't represent the LOM-CH standard *at all*. This class tries to translate
 * LOM descriptions to a clean version, which respects the standard.
 */

namespace Educa\DSB;

class LomDataSanitization
{
    /**
     *
     * @param ArchibaldLom $lom
     *
     * @return string
     *   json formated lom object
     */
    public static function jsonExport($lom, $beautify = FALSE) {
      watchdog(
        'json_export',
        'LOM ID as tested: @lom_id',
        array(
          '@lom_id' => $lom->lom_id,
        ),
        WATCHDOG_INFO,
        'archibald/' . $lom->lom_id
      );

      // This is ugly, but we have had occurences of stacked archibald###archibald###
      $lom->lomId = str_replace('archibald###', '', $lom->lomId);
      $lom->lomId = 'archibald###' . $lom->lomId;

      watchdog(
        'json_export',
        'LOM ID as exported: @lom_id',
        array(
          '@lom_id' => $lom->lom_id,
        ),
        WATCHDOG_INFO,
        'archibald/' . $lom->lom_id
      );
      $lom = self::cleanUp($lom);
      return ($beautify && defined('JSON_PRETTY_PRINT') ? json_encode($lom, JSON_PRETTY_PRINT) : json_encode($lom));
    }

    /**
     * Sanitize LOM-CH data.
     *
     * Take a data structure from the old database format, and clean it up so it
     * respects the LOM-CH standard. We also systematically use camelCase for
     * properties, instead of a mix of snake_case and camelCase.
     *
     * @param object $lomData
     *    The messed up LOM-CH data.
     *
     * @return object
     *    The cleaned-up, ready to use LOM-CH data.
     */
    public static function cleanUp($lomData)
    {
        $newLomData = new \stdClass();

        // Lom ID. This is not part of the standard, but so crucial we still add
        // it.
        $newLomData->lomId = $lomData->lomId;

        if (!empty($lomData->general)) {
            $newLomData->general = self::sanitizeGeneralData($lomData->general);
        }

        if (!empty($lomData->lifeCycle)) {
            $newLomData->lifeCycle = self::sanitizeLifeCycleData($lomData->lifeCycle);
        }

        if (!empty($lomData->metaMetadata)) {
            $newLomData->metaMetadata = self::sanitizeMetaMetaDataData($lomData->metaMetadata);
        }

        if (!empty($lomData->technical)) {
            $newLomData->technical = self::sanitizeTechnicalData($lomData->technical);
        }

        if (!empty($lomData->education)) {
            $newLomData->education = self::sanitizeEducationData($lomData->education);
        }

        if (!empty($lomData->rights)) {
            $newLomData->rights = self::sanitizeRightsData($lomData->rights);
        }

        if (!empty($lomData->relation)) {
            $newLomData->relation = self::sanitizeRelationData($lomData->relation);
        }

        if (!empty($lomData->classification)) {
            $newLomData->classification = self::sanitizeClassificationData($lomData->classification);
        }

        if (!empty($lomData->curriculum)) {
            $newLomData->curriculum = self::sanitizeCurriculumData($lomData->curriculum);
        }

        return $newLomData;
    }

    /**
     * I'm sure Ackermann is a respectable gentleman. But God, do I *hate* his
     * work...
     *
     * This is just an alias for cleanUp(). Sue me.
     */
    public static function cleanUpAfterAckermannTheBoogieManOfSoftwareDevelopment($lomData)
    {
        return self::cleanUp($lomData);
    }

    /**
     * Convert the General data structure.
     *
     * @param object $oldGeneralData
     *    The old General LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeGeneralData($oldGeneralData)
    {
        $newGeneralData = new \stdClass();

        // Sanitize the identifier.
        $newGeneralData->identifier = array();
        foreach ($oldGeneralData->identifier as $key => $value) {
            if (!empty($value->catalog)) {
                $identifierData = $value;
            } else if (!empty($value->values->catalog)) {
                $identifierData = $value->values;
            } else {
                // @todo ??
                continue;
            }

            $newGeneralData->identifier[] = (object) array(
                'catalog' => $identifierData->catalog,
                'entry' => $identifierData->entry,
                'title' => !empty($identifierData->title) ?
                    self::convertLangString($identifierData->title) :
                    '',
            );
        }

        // Sanitize the title.
        $newGeneralData->title = self::convertLangString($oldGeneralData->title);

        // Language is one of the few (only ?) properties that are correct...
        $newGeneralData->language = $oldGeneralData->language;

        // Sanitize the description.
        $newGeneralData->description = self::convertLangString($oldGeneralData->description);

        // Sanitize keywords.
        $newGeneralData->keyword = array();
        if (!empty($oldGeneralData->keyword)) {
            foreach ($oldGeneralData->keyword as $keyword) {
                $newGeneralData->keyword[] = self::convertLangString($keyword);
            }
        }

        // Sanitize coverage.
        $newGeneralData->coverage = array();
        if (!empty($oldGeneralData->coverage)) {
            foreach ($oldGeneralData->coverage as $coverage) {
                $newGeneralData->coverage[] = self::convertLangString($coverage);
            }
        }

        // Sanitize aggregation level. Make sure we use camelCase.
        $newGeneralData->aggregationLevel = !empty($oldGeneralData->aggregation_level) ?
            self::convertVocabulary($oldGeneralData->aggregation_level) :
            self::convertVocabulary($oldGeneralData->aggregationLevel);

        return $newGeneralData;
    }

    /**
     * Convert the Life Cycle data structure.
     *
     * @param object $oldLyfeCycleData
     *    The old Life Cycle LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeLifeCycleData($oldLifeCycleData)
    {
        $newLifeCycleData = new \stdClass();

        // Sanitize the version.
        if (!empty($oldLifeCycleData->version)) {
            $newLifeCycleData->version = self::convertLangString($oldLifeCycleData->version);
        }

        // Sanitize contributor.
        if (isset($oldLifeCycleData->contributor)) {
            $contributors = $oldLifeCycleData->contributor;
        } else if (isset($oldLifeCycleData->contributer)) {
            $contributors = $oldLifeCycleData->contributer;
        } else if (isset($oldLifeCycleData->contributors)) {
            $contributors = $oldLifeCycleData->contributors;
        } else if (isset($oldLifeCycleData->contributers)) {
            $contributors = $oldLifeCycleData->contributers;
        } else if (isset($oldLifeCycleData->contribute)) {
            $contributors = $oldLifeCycleData->contribute;
        }

        if (!empty($contributors)) {
            $newLifeCycleData->contribute = array();
            foreach ($contributors as $contributor) {
                $newLifeCycleData->contribute[] = (object) array(
                    'role' => self::convertVocabulary($contributor->role),
                    'entity' => $contributor->entity,
                );
            }
        }

        return $newLifeCycleData;
    }

    /**
     * Convert the MetaMetaData data structure.
     *
     * @param object $oldMetaMetaData
     *    The old MetaMetaData LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeMetaMetaDataData($oldMetaMetaData)
    {
        $oldMetaMetaData->sortContribute('date', 'asc');

        $newMetaMetaData = new \stdClass();

        // Sanitize the identifier.
        $newMetaMetaData->identifier = array();
        foreach ($oldMetaMetaData->identifier as $key => $value) {
            if (!empty($value->catalog)) {
                $identifierData = $value;
            } else if (!empty($value->values->catalog)) {
                $identifierData = $value->values;
            } else {
                // @todo ??
                break;
            }

            $newMetaMetaData->identifier[] = (object) array(
                'catalog' => $identifierData->catalog,
                'entry' => $identifierData->entry,
            );
            break;
        }

        // Sanitize contributor.
        if (isset($oldMetaMetaData->contributor)) {
            $contributors = $oldMetaMetaData->contributor;
        } else if (isset($oldMetaMetaData->contributer)) {
            $contributors = $oldMetaMetaData->contributer;
        } else if (isset($oldMetaMetaData->contributors)) {
            $contributors = $oldMetaMetaData->contributors;
        } else if (isset($oldMetaMetaData->contributers)) {
            $contributors = $oldMetaMetaData->contributers;
        } else if (isset($oldMetaMetaData->contribute)) {
            $contributors = $oldMetaMetaData->contribute;
        }

        if (!empty($contributors)) {
            // We only need the last modifier
            // The unique entry obtained holds the last modified date
            $contributors = array($contributors[0], array_pop($contributors));

            $newMetaMetaData->contribute = array();
            foreach ($contributors as $contributor) {
                $newMetaMetaData->contribute[] = (object) array(
                    'role' => self::convertVocabulary($contributor->role),
                    'entity' => $contributor->entity,
                    'date' => !empty($contributor->date) ?
                        self::convertDateTime($contributor->date) :
                        null,
                );
            }
        }

        // Yay! Another field that is actually correct! Except for the
        // camelCase...
        $newMetaMetaData->metaDataSchema = $oldMetaMetaData->metadataSchema;

        // And yet another one! Home run in sight!
        $newMetaMetaData->language = $oldMetaMetaData->language;

        return $newMetaMetaData;
    }

    /**
     * You guessed it...
     *
     * Just an alias for sanitizeMetaMetaDataData(). If you have any trouble
     * with this stupid programmer humor, contact our legal department.
     */
    protected static function sanitizeMetaMetaMetaMetaMetaMetaDataDataDatData($oldMetaMetaData)
    {
        // Who TF thought it would be a good idea to use "metametdadata" ???
        return self::sanitizeMetaMetaDataData($oldMetaMetaData);
    }

    /**
     * Convert the Technical data structure.
     *
     * @param object $oldTechnicalData
     *    The old Technical LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeTechnicalData($oldTechnicalData)
    {
        $newTechnicalData = new \stdClass();

        // Sanitize format.
        if (!empty($oldTechnicalData->format)) {
            $newTechnicalData->format = $oldTechnicalData->format;
        }

        // Sanitize size.
        if (isset($oldTechnicalData->size)) {
            $newTechnicalData->size = $oldTechnicalData->size;
        }

        // Sanitize location.
        if (isset($oldTechnicalData->location)) {
            $newTechnicalData->location = $oldTechnicalData->location;
        }

        // Sanitize otherRequirements.
        if (isset($oldTechnicalData->otherPlattformRequirements)) {
            $newTechnicalData->otherPlatformRequirements = self::convertLangString($oldTechnicalData->otherPlattformRequirements);
        }

        // Sanitize duration.
        if (isset($oldTechnicalData->duration)) {
            $newTechnicalData->duration = self::convertDuration($oldTechnicalData->duration);
        }

        // Sanitize previewImage.
        if (isset($oldTechnicalData->previewImage)) {
            $newTechnicalData->previewImage = (object) array(
                'image' => $oldTechnicalData->previewImage->image,
                'copyright' => $oldTechnicalData->previewImage->copyrightDescription,
            );
        }

        return $newTechnicalData;
    }

    /**
     * Convert the Education data structure.
     *
     * @param object $oldEducationData
     *    The old Education LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeEducationData($oldEducationData)
    {
        $newEducationData = new \stdClass();

        // Sanitize learningResourceType. Make sure to use camelCase.
        if (isset($oldEducationData->learningResourceType)) {
            $resourceTypes = $oldEducationData->learningResourceType;
        } else if (isset($oldEducationData->values->learning_resource_type)) {
            $resourceTypes = $oldEducationData->values->learning_resource_type;
        }

        if (isset($resourceTypes)) {
            $newEducationData->learningResourceType = (object) array(
                'documentary' => array(),
                'pedagogical' => array(),
            );

            if (!empty($resourceTypes['documentary'])) {
                foreach ($resourceTypes['documentary'] as $value) {
                    $newEducationData->learningResourceType->documentary[] = self::convertVocabulary($value);
                }
            }

            if (!empty($resourceTypes['pedagogical'])) {
                foreach ($resourceTypes['pedagogical'] as $value) {
                    $newEducationData->learningResourceType->pedagogical[] = self::convertVocabulary($value);
                }
            }
        }

        // Sanitize intendedEndUserRole. Make sure to use camelCase.
        if (isset($oldEducationData->intendedEndUserRole)) {
            $intendedRoles = $oldEducationData->intendedEndUserRole;
        } else if (isset($oldEducationData->intended_end_user_role)) {
            $intendedRoles = $oldEducationData->intended_end_user_role;
        } else if (isset($oldEducationData->values->intendedEndUserRole)) {
            $intendedRoles = $oldEducationData->values->intendedEndUserRole;
        } else if (isset($oldEducationData->values->intended_end_user_role)) {
            $intendedRoles = $oldEducationData->values->intended_end_user_role;
        }

        if (isset($intendedRoles)) {
            $newEducationData->intendedEndUserRole = array();
            foreach ($intendedRoles as $value) {
                $newEducationData->intendedEndUserRole[] = self::convertVocabulary($value);
            }
        }

        // Sanitize context.
        if (isset($oldEducationData->context)) {
            $context = $oldEducationData->context;
        } else if (isset($oldEducationData->values->context)) {
            $context = $oldEducationData->values->context;
        }

        if (isset($context)) {
            $newEducationData->context = array();
            foreach ($context as $value) {
                $newEducationData->context[] = self::convertVocabulary($value);
            }
        }

        // Sanitize typicalAgeRange.
        if (isset($oldEducationData->typicalAgeRange)) {
            $newEducationData->typicalAgeRange = array();
            foreach ($oldEducationData->typicalAgeRange as $value) {
                $newEducationData->typicalAgeRange[] = self::convertLangString($value);
            }
        }

        // Sanitize difficultyLevel.
        if (isset($oldEducationData->difficult)) {
            $newEducationData->difficultyLevel = self::convertVocabulary($oldEducationData->difficult);
        }

        // Sanitize typicalLearningTime. Respect the standard hierarchy (one
        // extra sub-level).
        if (isset($oldEducationData->typicalLearningTime)) {
            $newEducationData->typicalLearningTime = (object) array(
                'learningTime' => self::convertVocabulary($oldEducationData->typicalLearningTime),
            );
        }

        // Sanitize description.
        if (isset($oldEducationData->description)) {
            $newEducationData->description = self::convertLangString($oldEducationData->description);
        }

        return $newEducationData;
    }

    /**
     * Convert the Rights data structure.
     *
     * @param object $oldRightsData
     *    The old Rights LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeRightsData($oldRightsData)
    {
        $newRightsData = new \stdClass();

        // Sanitize cost.
        if (!empty($oldRightsData->cost)) {
            $newRightsData->cost = self::convertVocabulary($oldRightsData->cost);
        }

        // Sanitize description.
        if (isset($oldRightsData->description)) {
            $newRightsData->description = self::convertLangString($oldRightsData->description);
        }

        return $newRightsData;
    }

    /**
     * Convert the Relation data structure.
     *
     * @param object $oldRelationData
     *    The old Relation LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeRelationData($oldRelationData)
    {
        $newRelationData = array();

        foreach ($oldRelationData as $relationData) {
            $newRelationData[] = (object) array(
                'kind' => self::convertVocabulary($relationData->kind),
                // Even though the standard says resource is an array, in
                // practice there's only one entry.
                'resource' => array((object) array(
                    'identifier' => (object) array(
                        'catalog' => $relationData->catalog,
                        'entry' => $relationData->value,
                    ),
                    'description' => self::convertLangString($relationData->description),
                )),
            );
        }

        return $newRelationData;
    }

    /**
     * Convert the Classification data structure.
     *
     * @param object $oldClassificationData
     *    The old Classification LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeClassificationData($oldClassificationData)
    {
        $newClassificationData = array();

        foreach ($oldClassificationData as $classificationData) {
            $taxonPath = array();
            foreach ($classificationData->taxonPath as $taxonPathData) {
                $taxon = array();
                foreach ($taxonPathData->taxon as $taxonData) {
                    $taxon[] = (object) array(
                        'id' => $taxonData->id,
                        'entry' => self::convertLangString($taxonData->entry),
                    );
                }
                $taxonPath[] = (object) array(
                    'source' => self::convertLangString($taxonPathData->source),
                    'taxon' => $taxon,
                );
            }
            $newClassificationData[] = (object) array(
                'purpose' => self::convertVocabulary($classificationData->purpose),
                'taxonPath' => $taxonPath,
            );
        }

        return $newClassificationData;
    }

    /**
     * Convert the Curriculum data structure.
     *
     * @param object $oldCurriculumData
     *    The old Curriculum LOM-CH data, as an object.
     *
     * @return object
     *    The sanitized LOM-CH data object.
     */
    protected static function sanitizeCurriculumData($oldCurriculumData)
    {
        $newCurriculumData = array();

        foreach ($oldCurriculumData as $curriculumData) {
            $newCurriculumData[] = (object) array(
                'source' => $curriculumData->source,
                'entity' => $curriculumData->entity,
            );
        }

        return $newCurriculumData;
    }

    /**
     * Sanitize a LangString field.
     *
     * @param object $oldLangString
     *    The old LangString data, as an object.
     *
     * @return object
     *    The sanitized LangString data.
     */
    protected static function convertLangString($oldLangString)
    {
        $newLangString = new \stdClass();

        if (!empty($oldLangString->strings)) {
            $strings = $oldLangString->strings;
        } else if (!empty($oldLangString->values->strings)) {
            $strings = $oldLangString->values->strings;
        } else {
            // @todo ??
            return $newLangString;
        }

        foreach ($strings as $lang => $value) {
            $newLangString->{$lang} = $value;
        }

        return $newLangString;
    }

    /**
     * Sanitize a Vocabulary field.
     *
     * @param object $oldVocabulary
     *    The old Vocabulary data, as an object.
     *
     * @return object
     *    The sanitized Vocabulary data.
     */
    protected static function convertVocabulary($oldVocabulary)
    {
        $newVocabulary = new \stdClass();

        if (!empty($oldVocabulary->source)) {
            $data = $oldVocabulary;
        } else if (!empty($oldVocabulary->values->source)) {
            $data = $oldLangString->values;
        } else {
            // @todo ??
            return $newVocabulary;
        }

        $newVocabulary->source = $data->source;
        $newVocabulary->value = $data->value;

        return $newVocabulary;
    }

    /**
     * Sanitize a DateTime field.
     *
     * @param object $oldDateTime
     *    The old DateTime data, as an object.
     *
     * @return string
     *    The sanitized DateTime data, as a single string.
     */
    protected static function convertDateTime($oldDateTime)
    {
        return isset($oldDateTime->datetime) ?
            $oldDateTime->datetime :
            $oldDateTime->values->datetime;
    }

    /**
     * Sanitize a Duration field.
     *
     * @param object $oldDuration
     *    The old Duration data, as an object.
     *
     * @return string
     *    The sanitized Duration data, as a single string.
     */
    protected static function convertDuration($oldDuration)
    {
        return isset($oldDuration->duration) ?
            $oldDuration->duration :
            $oldDuration->values->duration;
    }

}
