<?php

/**
 * @file
 * Theme callbacks, preprocess functions and theme helper functions.
 */

/**
 * Implements hook_preprocess_dsb_portal_description().
 *
 * This pre-process function adds a lot more information for the template.
 * It will add the following variables:
 * - misc_information
 * - edu_information
 * - resource_information
 * - relationship_information
 * - classification_information
 *
 * See the template file source for more information.
 *
 * @see theme/dsb-portal-description.tpl.php
 */
function dsb_portal_preprocess_dsb_portal_description(&$vars) {
  $lom = $vars['description'];

  // Parse miscellaneous data.
  $misc_information = array();

  // Fetch the contributors.
  $misc_information['contributors'] = array();
  $contributors = $lom->getField('lifeCycle.contribute');

  if (!empty($contributors)) {
    $match;
    foreach ($contributors as $contributor) {
      // Reset the variables.
      $display_name = '';
      $filter_name = '';
      $organization_name = '';
      $organization_city = '';

      // Parse its VCARDs. As soon as we find the information we want, we keep it.
      foreach ($contributor['entity'] as $vcard) {
        if (empty($display_name)) {
          if (preg_match('/^FN:(.+)$/m', $vcard, $match)) {
            $display_name = trim($match[1]);
          }
        }

        if (empty($filter_name)) {
          if (preg_match('/^N:(.+)*$/m', $vcard, $match)) {
            $filter_name = trim(implode(' ', explode(';', $match[1])));
          }
        }

        if (empty($organization_name)) {
          if (preg_match('/^ORG:(.+)$/m', $vcard, $match)) {
            $organization_name = trim(trim($match[1]), ';');
          }
        }

        if (empty($organization_city)) {
          if (preg_match('/^ADR;WORK:(.+)$/m', $vcard, $match)) {
            $tmp = explode(';', $match[1]);
            $organization_city = trim($tmp[3]);
          }
        }
      }

      // If the display name is empty, we use the organization name.
      $display_name = empty($display_name) ? $organization_name : $display_name;

      // If the filter name is empty, we try to use the display name (might fail,
      // but it's worth a try in many cases).
      $filter_name = empty($filter_name) ? $display_name : $filter_name;

      $misc_information['contributors'][$contributor['role']['value']][] = array(
        'role' => _dsb_portal_theme_get_vocabulary_item_name($contributor['role']),
        'display_name' => $display_name,
        'filter_name' => $filter_name,
        'organization_name' => $organization_name,
        'organization_city' => $organization_city,
      );
    }
  }

  // Fetch the languages.
  $misc_information['languages'] = array();
  $languages = $lom->getField('general.language');
  if (!empty($languages)) {
    $language_list = dsb_portal_get_languages();

    foreach ($languages as $language) {
      if (isset($language_list[$language])) {
        // Try to localize the information, if possible.
        if (function_exists('locale')) {
          $misc_information['languages'][] = locale($language_list[$language]);
        }
        else {
          $misc_information['languages'][] = $language_list[$language];
        }
      }
    }
  }

  // Fetch the keywords, if any.
  $misc_information['keywords'] = array();
  $keywords = $lom->getField('general.keyword');
  if (!empty($keywords)) {
    foreach ($keywords as $keyword) {
      $misc_information['keywords'][] = _dsb_portal_theme_get_langstring_value($keyword);
    }

    // We sort them alphabetically.
    sort($misc_information['keywords']);
  }

  // Fetch the coverage, if any.
  $misc_information['coverage'] = array();
  $coverage = $lom->getField('general.coverage');
  if (!empty($coverage)) {
    foreach ($coverage as $keyword) {
      $misc_information['coverage'][] = array_values($keyword)[0];
    }
  }

  // Fetch the version information, if any.
  $version = $lom->getField('lifeCycle.version');
  if (!empty($version)) {
    $misc_information['version'] = _dsb_portal_theme_get_langstring_value($version);
  }

  // Parse the educational information.
  $edu_information = array();

  // Get the description.
  $description = $lom->getField('education.description', dsb_portal_lom_language_fallback_list());
  $edu_information['description'] = !empty($description) ? $description : '';

  // Get the resource types.
  $resource_types = $lom->getField('education.learningResourceType');
  $edu_information['resource_types'] = array();
  if (!empty($resource_types)) {
    foreach (array(
      'documentary',
      'pedagogical',
    ) as $sub_group) {
      if (!empty($resource_types[$sub_group])) {
        foreach ($resource_types[$sub_group] as $type) {
          $edu_information['resource_types'][$type['value']] = _dsb_portal_theme_get_vocabulary_item_name($type);
        }
      }
    }
  }

  // Get the intended user roles.
  $intended_user_roles = $lom->getField('education.intendedEndUserRole');
  $edu_information['intended_user_roles'] = array();
  if (!empty($intended_user_roles)) {
    foreach ($intended_user_roles as $role) {
      $edu_information['intended_user_roles'][$role['value']] = _dsb_portal_theme_get_vocabulary_item_name($role);
    }
  }

  // Get the age ranges.
  $age_ranges = $lom->getField('education.typicalAgeRange');
  $edu_information['age_ranges'] = array();
  if (!empty($age_ranges)) {
    foreach ($age_ranges as $age_range) {
      $age_range = _dsb_portal_theme_get_langstring_value($age_range);

      // 0-0 is a default value, and means none was given. Skip it.
      if ($age_range != '0-0') {
        $edu_information['age_ranges'][] = $age_range;
      }
    }
  }

  // Get the typical learning time.
  $difficulty = $lom->getField('education.difficultyLevel');
  if (!empty($difficulty)) {
    $edu_information['difficulty'] = $difficulty;
  }

  // Get the duration.
  $duration = $lom->getField('technical.duration');
  if (!empty($duration)) {
    $interval = new DateInterval($duration);
    $edu_information['duration'] = $interval->format('%H:%I:%S');
  }

  // Get the typical learning time.
  $learning_time = $lom->getField('education.typicalLearningTime.learningTime');
  if (!empty($learning_time)) {
    $edu_information['learning_time'] = _dsb_portal_theme_get_vocabulary_item_name($learning_time);
  }

  // Get the education context.
  $context = array();
  foreach ($lom->getField('education.context') as $value) {
    $context[$value['value']] = _dsb_portal_theme_get_vocabulary_item_name($value);
  }
  $edu_information['context'] = $context;

  // Parse the resource information data.
  $resource_information = array();
  $rights = $lom->getField('rights');
  $resource_information['cost'] = _dsb_portal_theme_get_vocabulary_item_name($rights['cost']);
  $resource_information['license'] = _dsb_portal_theme_get_langstring_value($rights['description']);

  $format = $lom->getField('technical.format');
  $resource_information['format'] = !empty($format) ? $format : '';

  $size = $lom->getField('technical.size');
  $resource_information['size'] = !empty($size) ? $size : '';

  $location = $lom->getField('technical.location');
  $resource_information['location'] = !empty($location) ? $location : '';

  $requirements = $lom->getField('technical.otherPlatformRequirements');
  $resource_information['requirements'] = !empty($requirements) ? $requirements : '';

  // Get the identifiers.
  $identifiers = $lom->getField('general.identifier');
  $resource_information['identifiers'] = array();
  if (!empty($identifiers)) {
    foreach ($identifiers as $identifier) {
      $resource_information['identifiers'][] = $identifier;
    }
  }

  // Get relationship information.
  $relationships = $lom->getField('relation');

  // Get classification information.
  $classification_information = array();
  $standard_classification = $lom->getField('classification');
  if (!empty($standard_classification)) {
    $classification_information[] = array();
    $i = count($classification_information) - 1;
    foreach ($standard_classification as $classification_entry) {
      $purpose = $classification_entry['purpose']['value'];
      if (!isset($classification_information[$i][$purpose])) {
        $langs = dsb_portal_lom_language_fallback_list();
        array_unshift($langs, 'en');
        $classification_information[$i]['source'] = _dsb_portal_theme_get_langstring_value($classification_entry['taxonPath'][0]['source'], $langs);
        $classification_information[$i][$purpose] = array();
      }

      $tree = &$classification_information[$i][$purpose];
      foreach ($classification_entry['taxonPath'] as $taxonomy_path) {
        // A taxonomy "path" is a linear representation of a hierarchy, in
        // order: the first item is the top-most parent, and the last item is
        // the "leaf" of the tree. We iterate over each item, but construct it
        // in tree form. This means this:
        // [one, two, three]
        // Becomes:
        // [
        //   one => [
        //     children => [
        //       two => [
        //       children => [
        //         three => [
        //           children => []
        //         ],
        //       ],
        //     ],
        //   ],
        // ]
        foreach ($taxonomy_path['taxon'] as $taxonomy_entry) {
          // Do we have an entry already ? If not, create one.
          if (!isset($tree[$taxonomy_entry['id']])) {
            $tree[$taxonomy_entry['id']] = array(
              'entry' => $taxonomy_entry['entry'],
              'children' => array(),
            );
          }

          // Use this as the new parent, which allows us to "recursively" add
          // taxonomy entries.
          $tree = &$tree[$taxonomy_entry['id']]['children'];
        }
      }
    }
  }

  $vars['misc_information'] = $misc_information;
  $vars['edu_information'] = $edu_information;
  $vars['resource_information'] = $resource_information;
  $vars['relationship_information'] = $relationships;
  $vars['classification_information'] = $classification_information;
}

/**
 * Implements hook_preprocess_dsb_portal_description_misc_info().
 *
 * Render the information as a table.
 */
function dsb_portal_preprocess_dsb_portal_description_misc_info(&$vars) {
  $misc_information = $vars['misc_information'];

  // Prepare the data for outputting the information as a table.
  $rows = array();

  // Contributors.
  if (!empty($misc_information['contributors'])) {
    // Little trick to get authors before publishers.
    ksort($misc_information['contributors']);

    foreach ($misc_information['contributors'] as $role => $contributors) {
      $formatted_contributors = array();
      foreach ($contributors as $contributor) {
        // Do we have a filter name value? If so, format the contributor
        // as a link.
        if (!empty($contributor['filter_name'])) {
          $formatted_contributors[] = dsb_portal_theme_contributor_filter_link(
            $contributor['display_name'],
            $contributor['filter_name']
          );
        }
        else {
          $formatted_contributors[] = check_plain($contributor['display_name']);
        }
      }

      $rows[] = array(
        // We can use the last contributor's role value; it applies to all
        // of them, as they are grouped by role.
        check_plain($contributor['role']),
        // Format the list of formatted contributors as a list of links.
        implode('<br />', $formatted_contributors),
      );
    }
  }

  // Language.
  if (!empty($misc_information['languages'])) {
    $rows[] = array(
      t("Language", array(), array('context' => 'dsb_portal:view')),
      implode(', ', array_map('check_plain', $misc_information['languages'])),
    );
  }

  // Language.
  if (!empty($misc_information['version'])) {
    $rows[] = array(
      t("Version", array(), array('context' => 'dsb_portal:view')),
      check_plain($misc_information['version']),
    );
  }

  // Keywords.
  if (!empty($misc_information['keywords'])) {
    $rows[] = array(
      t("Keywords", array(), array('context' => 'dsb_portal:view')),
      implode('<br />', array_map(function($item) {
        return dsb_portal_theme_keyword_filter_link($item);
      }, $misc_information['keywords'])),
    );
  }

  // Coverage.
  if (!empty($misc_information['coverage'])) {
    $rows[] = array(
      t("Coverage", array(), array('context' => 'dsb_portal:view')),
      implode('<br />', array_map(function($item) {
        return dsb_portal_theme_coverage_filter_link($item);
      }, $misc_information['coverage'])),
    );
  }

  $vars['table'] = theme('table', array(
    'rows' => $rows,
  ));
}

/**
 * Implements hook_preprocess_dsb_portal_description_edu_info().
 *
 * Render the information as a table.
 */
function dsb_portal_preprocess_dsb_portal_description_edu_info(&$vars) {
  $edu_information = $vars['edu_information'];

  // Prepare the data for outputting the information as a table.
  $rows = array();

  // Description.
  if (!empty($edu_information['description'])) {
    $rows[] = array(
      t("Pedagogical description", array(), array('context' => 'dsb_portal:view')),
      check_plain($edu_information['description']),
    );
  }

  // Resource types.
  if (!empty($edu_information['resource_types'])) {
    $resource_types = array();
    foreach ($edu_information['resource_types'] as $key => $value) {
      $resource_types[] = dsb_portal_theme_resource_type_filter_link($value, $key);
    }

    $rows[] = array(
      t("Learning resource types", array(), array('context' => 'dsb_portal:view')),
      implode('<br />', $resource_types),
    );
  }

  // Intended user roles.
  if (!empty($edu_information['intended_user_roles'])) {
    $intended_user_roles = array();
    foreach ($edu_information['intended_user_roles'] as $key => $value) {
      $intended_user_roles[] = dsb_portal_theme_user_role_filter_link($value, $key);
    }

    $rows[] = array(
      t("Target audience", array(), array('context' => 'dsb_portal:view')),
      implode('<br />', $intended_user_roles),
    );
  }

  // Age ranges.
  if (!empty($edu_information['age_ranges'])) {
    $age_ranges = array();
    foreach ($edu_information['age_ranges'] as $value) {
      $age_ranges[] = check_plain($value);
    }

    $rows[] = array(
      t("Target audience's age", array(), array('context' => 'dsb_portal:view')),
      implode('<br />', $age_ranges),
    );
  }

  // Duration.
  if (!empty($edu_information['duration'])) {
    $rows[] = array(
      t("Duration", array(), array('context' => 'dsb_portal:view')),
      check_plain($edu_information['duration']),
    );
  }

  // Difficulty.
  if (!empty($edu_information['difficulty'])) {
    $rows[] = array(
      t("Difficulty", array(), array('context' => 'dsb_portal:view')),
      dsb_portal_theme_difficulty_filter_link(
        _dsb_portal_theme_get_vocabulary_item_name($edu_information['difficulty']),
        $edu_information['difficulty']['value']
      ),
    );
  }

  // Typical learning time.
  if (!empty($edu_information['learning_time'])) {
    $rows[] = array(
      t("Learning time", array(), array('context' => 'dsb_portal:view')),
      check_plain($edu_information['learning_time']),
    );
  }

  // Number of lessons.
  if (!empty($edu_information['lessons'])) {
    $rows[] = array(
      t("Duration", array(), array('context' => 'dsb_portal:view')),
      check_plain($edu_information['lessons']),
    );
  }

  // Education context.
  $contexts = array();
  foreach ($edu_information['context'] as $key => $value) {
    $contexts[] = dsb_portal_theme_education_context_filter_link($value, $key);
  }
  $rows[] = array(
    t("Education context", array(), array('context' => 'dsb_portal:view')),
    implode('<br />', $contexts),
  );

  $vars['table'] = theme('table', array(
    'rows' => $rows,
  ));
}

/**
 * Implements hook_preprocess_dsb_portal_description_classification_info().
 *
 * Dummy implementation. The actual logic behind the rendering and parsing of
 * curricula data is done by submodules.
 *
 * @see modules/per/dsb_portal_per.module
 * @see modules/educa/dsb_portal_educa.module
 */
function dsb_portal_preprocess_dsb_portal_description_classification_info(&$vars) {
  $vars['trees'] = array();
}

/**
 * Implements hook_preprocess_dsb_portal_description_resource_info().
 *
 * Render the information as tables.
 */
function dsb_portal_preprocess_dsb_portal_description_resource_info(&$vars) {
  $resource_information = $vars['resource_information'];

  // Prepare the data for outputting the information as a table.
  // The rights table.
  $rows = array();

  if (!empty($resource_information['cost'])) {
    $rows[] = array(
      t("Cost", array(), array('context' => 'dsb_portal:view')),
      check_plain($resource_information['cost']),
    );
  }

  if (!empty($resource_information['license'])) {
    $rows[] = array(
      t("License", array(), array('context' => 'dsb_portal:view')),
      // @todo sanitize!
      $resource_information['license'],
    );
  }

  if (!empty($rows)) {
    $vars['rights_table'] = theme('table', array(
      'rows' => $rows,
    ));
  }

  // Prepare the technical information table.
  $rows = array();

  $rows[] = array(
    t("Technical requirements", array(), array('context' => 'dsb_portal:view')),
    !empty($resource_information['requirements']) ?
      check_plain($resource_information['requirements']) :
      '-',
  );

  if (!empty($resource_information['format'])) {
    $rows[] = array(
      t("Format", array(), array('context' => 'dsb_portal:view')),
      implode('<br />', array_map('check_plain', $resource_information['format'])),
    );
  }

  if (!empty($resource_information['size'])) {
    $rows[] = array(
      t("Size", array(), array('context' => 'dsb_portal:view')),
      check_plain($resource_information['size']),
    );
  }

  if (!empty($resource_information['location'])) {
    $rows[] = array(
      t("Location", array(), array('context' => 'dsb_portal:view')),
      implode('<br />', array_map('check_plain', $resource_information['location'])),
    );
  }

  if (!empty($rows)) {
    $vars['technical_table'] = theme('table', array(
      'rows' => $rows,
    ));
  }

  // Prepare the links table.
  $rows = array();

  if (!empty($resource_information['identifiers'])) {
    foreach ($resource_information['identifiers'] as $identifier) {
      $rows[] = array(
        check_plain($identifier['catalog']),
        _dsb_portal_theme_get_identifier_value($identifier),
      );
    }
  }

  if (!empty($rows)) {
    $vars['resource_table'] = theme('table', array(
      'rows' => $rows,
    ));
  }
}

/**
 * Implements hook_preprocess_dsb_portal_description_relationship_info().
 *
 * Render the information as a table.
 */
function dsb_portal_preprocess_dsb_portal_description_relationship_info(&$vars) {
  $relationship_information = $vars['relationship_information'];

  // Prepare the data for outputting the information as a table.
  $rows = array();

  foreach ($relationship_information as $relationship) {
    $rows[] = array(
      check_plain(_dsb_portal_theme_get_vocabulary_item_name($relationship['kind'])),
      array_reduce($relationship['resource'], function($carry, $resource) {
        $carry .= '<div class="dsb-portal-description-relationship-information__relationship">';
        if (!empty($resource['description'])) {
          $carry .= '<span class="dsb-portal-description-relationship-information__relationship__description">';
          $carry .= check_plain(_dsb_portal_theme_get_langstring_value($resource['description']));
          $carry .= '</span><br />';
        }
        $carry .= _dsb_portal_theme_get_identifier_value($resource['identifier']);
        $carry .= '</div>';

        return $carry;
      }, ''),
    );
  }

  $vars['table'] = theme('table', array(
    'rows' => $rows,
  ));
}

/**
 * Content partner filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * content partner (owner). This returns an HTML formatted link, either with
 * text or a logo.
 *
 * If $use_partner_logo is true, will try to fetch the content partner logo
 * from metaContributorLogos. If it isn't available, it will fallback to the
 * content partner name.
 *
 * @param \Educa\DSB\Client\Lom\LomDescriptionInterface $lom
 *    The LOM-CH description object.
 * @param bool $use_partner_logo
 *    (optional) Use the partner logo, if any. Defaults to false.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_theme_owner_filter_link($lom, $use_partner_logo = FALSE) {
  // Do we need to use the partner logo?
  if ($use_partner_logo) {
    if ($logos = $lom->getContributorLogos()) {
      // First, make sure we have a logo with an absolute URL. Many get
      // published with relative URLs, which are not usable for us.
      foreach ($logos as $logo) {
        if (preg_match('/^https?:\/\//', $logo)) {
          // We might want to let modules and themes alter the logo path.
          drupal_alter('dsb_portal_owner_logo_path', $logo);

          // We format it as an image.
          $link_html = theme('image', array(
            'path' => $logo,
          ));
        }
      }
    }
  }

  // Do we have a link text? If not, fallback to the content partner name.
  if (empty($link_html)) {
    // We first try the display name, falling back to the username if necessary.
    if (method_exists($lom, 'getOwnerDisplayName') && !$link_html = $lom->getOwnerDisplayName()) {
      $link_html = $lom->getOwnerUsername();
    }

    $link_html = check_plain($link_html);
  }

  $options = array(
    'html' => TRUE,
    'query' => array(
      'filter' => array(
        'ownerUsername' => array($lom->getOwnerUsername()),
      ),
    ),
  );

  return l($link_html, 'dsb-portal/search', $options);
}

/**
 * Contributor filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * contributor. This returns an HTML formatted link.
 *
 * @param string $contributor_name
 *    The name of the contributor, which will be used as the link text.
 * @param string $filter_value
 *    The value of the filter. This value must be something the API can
 *    understand. For example, the API can filter by a contributor's name, but
 *    only understands the "lastname firstname" format, and will not find
 *    any results for "firstname lastname".
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_theme_contributor_filter_link($contributor_name, $filter_value) {
  return l($contributor_name, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'contributor' => array($filter_value),
      ),
    ),
  ));
}

/**
 * Education context filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * education context. This returns an HTML formatted link.
 *
 * @param string $context_name
 *    The name of the context, which will be used as the link text.
 * @param string $filter_value
 *    The value of the filter. This value must be something the API can
 *    understand.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_theme_education_context_filter_link($context_name, $filter_value) {
  return l($context_name, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'context' => array($filter_value),
      ),
    ),
  ));
}

/**
 * Difficulty filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * difficulty level. This returns an HTML formatted link.
 *
 * @param string $difficulty_name
 *    The name of the difficulty, which will be used as the link text.
 * @param string $filter_value
 *    The value of the filter. This value must be something the API can
 *    understand.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_theme_difficulty_filter_link($difficulty_name, $filter_value) {
  return l($difficulty_name, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'difficulty' => array($filter_value),
      ),
    ),
  ));
}

/**
 * Keyword filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * keyword. This returns an HTML formatted link.
 *
 * @param string $keyword_name
 *    The name of the keyword, which will be used as the link text.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_theme_keyword_filter_link($keyword) {
  return l($keyword, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'keywords' => array($keyword),
      ),
    ),
  ));
}

/**
 * Coverage filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * coverage term. This returns an HTML formatted link.
 *
 * @param string $keyword_name
 *    The name of the keyword, which will be used as the link text.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_theme_coverage_filter_link($coverage) {
  return l($coverage, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'coverage' => array($coverage),
      ),
    ),
  ));
}

/**
 * Resource type filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * type of resource. This returns an HTML formatted link.
 *
 * @param string $resource_type
 *    The human-readable type of the resource, which will be used as the
 *    link text.
 * @param string $filter_value
 *    The value of the filter. This value must be something the API can
 *    understand.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_theme_resource_type_filter_link($resource_type, $filter_value) {
  return l($resource_type, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'learningResourceType' => array($filter_value),
      ),
    ),
  ));
}

/**
 * User role filter link.
 *
 * Prepare a link that can be used to filter search results by a specific
 * type of user role. This returns an HTML formatted link.
 *
 * @param string $user_role
 *    The human-readable name of the role, which will be used as the link text.
 * @param string $filter_value
 *    The value of the filter. This value must be something the API can
 *    understand.
 *
 * @return string
 *    A HTML-formatted string.
 */
function dsb_portal_theme_user_role_filter_link($user_role, $filter_value) {
  return l($user_role, 'dsb-portal/search', array(
    'query' => array(
      'filter' => array(
        'intendedEndUserRole' => array($filter_value),
      ),
    ),
  ));
}

/**
 * Get the Ontology human-readable name from a vocabulary entry.
 *
 * @param object $vocabulary_item
 *    An Ontology vocabulary item object.
 *
 * @return string
 *    The Ontology vocabulary item name.
 */
function _dsb_portal_theme_get_vocabulary_item_name($vocabulary_item) {
  global $language;
  return isset($vocabulary_item['ontologyName'][$language->language]) ? $vocabulary_item['ontologyName'][$language->language] : $vocabulary_item['value'];
}

/**
 * Get the localized entry from a LangString.
 *
 * @param object $langstring
 *    A LOM-CH LangString object.
 *
 * @return string
 *    The localized language string.
 */
function _dsb_portal_theme_get_langstring_value($langstring) {
  foreach (dsb_portal_lom_language_fallback_list() as $language) {
    if (isset($langstring[$language])) {
      return $langstring[$language];
    }
  }

  // If we didn't find a language key, we simply return the data as-is.
  return $langstring;
}

/**
 * Get the identifier value.
 *
 * @param array $identifier
 *    An identifier, with at least an entry and a catalog key.
 *
 * @return string
 *    A formatted identifier.
 */
function _dsb_portal_theme_get_identifier_value($identifier) {
  switch ($identifier['catalog']) {
    case 'URL':
    case 'REL_URL':
      $value = l(
        !empty($identifier['title']) ?
          _dsb_portal_theme_get_langstring_value($identifier['title']) :
          t("Link to resource", array(), array('context' => 'dsb_portal:view')),
        $identifier['entry'],
        array(
          'attributes' => array(
            'target' => '_blank',
          ),
        )
      );
      break;

    case 'DOI':
    case 'REL_DOI':
      $value = check_plain($identifier['entry']);

      if (!empty($identifier['title'])) {
        $value .= '<br />' . check_plain(_dsb_portal_theme_get_langstring_value($identifier['title']));
      }
      break;

    default:
      $value = check_plain($identifier['entry']);
      break;
  }

  return $value;
}
