<?php
/**
 * @file
 * Installation support functions for embed_media_field module.
 */

function _embed_media_field_update_all_media_tokens($entity_meta) {
  $txn = db_transaction();

  try {
    $result = array();
    $all_files_missing_uuids = array();

    foreach ($entity_meta as $entity_type => $meta) {
      $entity_info = entity_get_info($entity_type);
      $bundle_fields = array();
      foreach (embed_assets_field_get_entity_asset_fields($entity_type) as $bundle_name => $types) {
        if (!empty($types['media'])) {
          $bundle_fields[$bundle_name] = $types['media'];
        }
      }

      foreach ($bundle_fields as $bundle => $field) {
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', $entity_type);
        $query->entityCondition('bundle', $bundle);
        $query_result = $query->execute();
        $entity_ids = array_keys($query_result[$entity_type]);

        $entities = entity_load($entity_type, $entity_ids);
        $keys = $entity_info['entity keys'];
        $id_key = $keys['id'];
        $revision_key = $keys['revision'];
        $result[$entity_type] = array();

        foreach ($entities as $entity) {
          $entity_id = $entity->$id_key;
          $revision_id = $revision_key ? $entity->$revision_key : $entity_id;
          $fids = array();

          foreach ($meta['body_fields'] as $body_field_name => $body_field_key) {
            if (empty($entity->$body_field_name)) {
              continue;
            }
            foreach ($entity->$body_field_name as $lang => $values) {
              foreach ($values as $delta => $value) {
                $body = $value[$body_field_key];
                $data = _embed_media_field_scan_for_media_tokens($body);

                $result[$entity_type][$entity_id][] = $data;

                $all_files_missing_uuids = array_merge($all_files_missing_uuids, $data['missing_uuids']);
                if ($data['replaced_tokens'] < 1) {
                  continue;
                }
                $fids = array_merge($fids, $data['fids']);

                  // Save the body text with the new media tokens that include uuids
                foreach (array('field_data', 'field_revision') as $table_prefix) {
                  // Update both the field_data_* and field_revision_* tables
                  db_update("{$table_prefix}_{$body_field_name}")
                    ->fields(array(
                      "{$body_field_name}_{$body_field_key}" => $data['body'],
                    ))
                    ->condition('entity_type', $entity_type, '=')
                    ->condition('deleted', 0, '=')
                    ->condition('entity_id', $entity_id, '=')
                    ->condition('language', $lang, '=')
                    ->condition('delta', $delta, '=')
                    ->condition('revision_id', $revision_id, '=')
                    ->execute();
                }
              }
            }
          }

          if (count($fids) < 1) {
            continue;
          }

          // Insert the media assets for this entity
          // Save the body text with the new media tokens that include uuids
          $attachment_number = 0;
          $language = LANGUAGE_NONE;
          foreach ($fids as $fid) {
            foreach (array('field_data', 'field_revision') as $table_prefix) {
              $insert = db_insert($table_prefix . '_' . $meta['asset_field']);
              $insert->fields(array(
                'entity_type' => $entity_type,
                'bundle' => $bundle,
                'deleted' => 0,
                'entity_id' => $entity_id,
                'revision_id' => $revision_id,
                'language' => $language,
                'delta' => $attachment_number,
                $meta['asset_field'] . '_fid' => $fid,
              ));
              $insert->execute();
            }
            $attachment_number++;
          }

          $total_replaced = array_sum(array_map(function($data) {
            return $data['replaced_tokens'];
          }, $result[$entity_type][$entity_id]));

          $total_missing_uuids = array_sum(array_map(function($data) {
            return count($data['missing_uuids']);
          }, $result[$entity_type][$entity_id]));

          drush_print("Replaced and attached media tokens in $entity_type/$entity_id for fids: " . implode(',', $fids));
        }
      }
    }

    if (count($all_files_missing_uuids) > 0) {
      drush_print("The following files were missing uuids and their tokens did not get replaced, nor their assets attached:");
      foreach ($result as $entity_type => $entities) {
        foreach ($entities as $id => $entity) {
          foreach ($entity as $value) {
            if (!empty($value['missing_uuids'])) {
              drush_print("$entity_type/$id:" . implode(',', $value['missing_uuids']));
            }
          }
        }
      }
      drush_print("All fids missing uuids: " . implode(',', array_unique($all_files_missing_uuids)));
    }

    drush_print("\nDone");
  } catch (Exception $e) {
    $txn->rollback();
    throw $e;
  }
}

function _embed_media_field_scan_for_media_tokens($body) {
  preg_match_all('/\[\[.*?("fid":"(\d+)").*?\]\]/', $body, $matches);

  $fids = array();
  $missing_uuids = array();
  $tokens = $matches[0];
  $token_count = 0;

  if (!empty($tokens)) {
    $fid_attributes = $matches[1];
    $fids = $matches[2];
    $matching_file_uuids = entity_get_uuid_by_id('file', $fids);

    // We are replacing the part of media tokens which looks like "fid":"###" with the file's uuid (eg. "fid":"uuid-<uuid>")
    $replacements = array();
    foreach ($fids as $index => $fid) {
      // Unless the token references a real file with a uuid, don't replace it
      if (!empty($matching_file_uuids[$fid])) {
        // We have a file id matching the uuid in this token
        $replacement = str_replace(
          $fid_attributes[$index],
          '"fid":"uuid-' . $matching_file_uuids[$fid] . '"',
          $tokens[$index]
        );
        $replacements[$tokens[$index]] = $replacement;
        $token_count++;
      }
      else {
        $missing_uuids[] = $fid;
      }
    }

    $body = str_replace(
      array_keys($replacements),
      array_values($replacements),
      $body
    );
  }

  // Look for uuid-links to files
  if (preg_match_all('#\[uuid-link:file:([^\]]+)\]#', $body, $matches)) {
    $uuids = $matches[1];
    $fids = array_merge($fids, entity_get_id_by_uuid('file', $uuids));
  }

  $fids = array_unique($fids);

  return array(
    'body' => $body,
    'fids' => $fids,
    'replaced_tokens' => $token_count,
    'missing_uuids' => $missing_uuids,
  );
}
