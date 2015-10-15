<?php

/**
 * Plugin to do very simple mapping of tags and branches to versions.
 *
 * Tries to provide a sensible default behavior by extracting number parts
 * from the label name and using any non-numeric suffix as version extra.
 */
class VersioncontrolReleaseLabelVersionMapperGeneric implements VersioncontrolReleaseLabelVersionMapperInterface {

  public function GetVersionFromTag($tag_name, $project_node) {
    return $this->GetVersionFromLabel($tag_name, VERSIONCONTROL_LABEL_TAG, $project_node);
  }

  public function GetVersionFromBranch($branch_name, $project_node) {
    return $this->GetVersionFromLabel($branch_name, VERSIONCONTROL_LABEL_BRANCH, $project_node);
  }

  public function GetVersionFromLabel($label_name, $label_type, $project_node) {
    $fields = array('version_major', 'version_minor', 'version_patch');
    $version = array();

    $matched = preg_match_all('/[^\.\-]+/', $label_name, $matches, PREG_OFFSET_CAPTURE);
    $label_parts = $matched ? $matches[0] : array();

    while (!empty($fields)) { // Try to fill all regular version fields.
      $current_field = array_shift($fields);

      while (!empty($label_parts)) {
        $label_part = array_shift($label_parts);

        if (is_numeric($label_part[0])) {
          $version[$current_field] = $label_part[0];
          break; // next version field
        }
        elseif (!empty($version)) {
          // Non-numeric field after a numeric one was already assigned:
          // that sounds like a suffix like "beta1" or similar. Put back the
          // current label part so that it can still be extracted afterwards,
          // and fill up the regular fields in order to exit both loops.
          array_unshift($label_parts, $label_part);
          while (!empty($fields)) {
            $remaining_field = array_shift($fields);
            $version[$remaining_field] = FALSE;
          }
          break;
        }
      }
    }

    // Remove any fields padded with FALSE.
    $positive_version_numbers = FALSE;
    foreach ($version as $field => $number) {
      if ($number === FALSE) {
        unset($version[$field]);
      }
      elseif ($number > 0) {
        $positive_version_numbers = TRUE;
      }
    }

    if (!empty($positive_version_numbers)) {
      if ($label_type == VERSIONCONTROL_OPERATION_BRANCH) {
        // Branches always get a "-dev" appended. Looks good, and helps with
        // reverse engineering the version number.
        $version['version_extra'] = 'dev';
      }
      elseif (!empty($label_parts)) {
        // The next label part is the one after the last numeric part, like
        // "beta1". From that position on, use the rest of $label['name'] as
        // version extra.
        $label_part = array_shift($label_parts);
        $version['version_extra'] = substr($label['name'], $label_part[1]);
      }
      return (object) $version;
    }
    else {
      return empty($version) ? FALSE : ((object) $version);
    }
  }
}
